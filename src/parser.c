#include "parser.h"
#include "ast.h" // AST, ast_make_*
#include "constants.h"
#include "lexer.h" // Token, Token_Kind, generate_tokens_from_source, token_array_get_info_of
#include "list.h"  // List
#include "map.h"   // Map
#include "parser_utility.h"
#include "type.h"     // Type, make_typspec_*,
#include "typedefs.h" // s32 , s64, etc.
#include "utility.h"  // info, error, warning, success, strf, get_file_content
#include <assert.h>   // assert
#include <stdarg.h>   // va_list, va_start, va_end
#include <stdio.h>    // printf, vprintf
#include <stdlib.h>   // xmalloc
#include <string.h>   // strcmp
//------------------------------------------------------------------------------
//                              parser.c
//------------------------------------------------------------------------------

#define DEBUG_START \
    // info("%s: %s", __func__, token_to_str(ctx->curr_tok));                  \
    assert(ctx);

#define UNARY_OP_COUNT 8
Token_Kind unary_ops[UNARY_OP_COUNT] = {
    TOKEN_BANG,
    THI_SYNTAX_POINTER,
    TOKEN_PLUS,
    THI_SYNTAX_ADDRESS,
    TOKEN_MINUS,
    TOKEN_TILDE,
    TOKEN_DOT,
    TOKEN_SIZEOF
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//              Each construct of the language gets its own function
//------------------------------------------------------------------------------

// clang-format off
AST* parse_statement                (Parser_Context* ctx);
AST* parse_primary                  (Parser_Context* ctx);
AST* parse_identifier               (Parser_Context* ctx);
AST* parse_expression_identifier    (Parser_Context* ctx);
AST* parse_variable_decl            (Parser_Context* ctx, char* ident);
AST* parse_constant_decl            (Parser_Context* ctx, char* ident);
AST* parse_function_call            (Parser_Context* ctx, char* ident);
AST* parse_block                    (Parser_Context* ctx);
AST* parse_return                   (Parser_Context* ctx);
AST* parse_note                     (Parser_Context* ctx);
AST* parse_expression               (Parser_Context* ctx);
AST* parse_unary                    (Parser_Context* ctx);
AST* parse_binary                   (Parser_Context* ctx, s8 min_prec, AST* lhs);
AST* parse_integer                  (Parser_Context* ctx);
AST* parse_float                    (Parser_Context* ctx);
AST* parse_parens                   (Parser_Context* ctx);
AST* parse_defer                    (Parser_Context* ctx);
AST* parse_load                     (Parser_Context* ctx);
AST* parse_extern                   (Parser_Context* ctx);
AST* parse_link                     (Parser_Context* ctx);
AST* parse_if                       (Parser_Context* ctx);
AST* parse_is                       (Parser_Context* ctx);
AST* parse_fallthrough              (Parser_Context* ctx);
AST* parse_dangling_else            (Parser_Context* ctx);
AST* parse_for                      (Parser_Context* ctx);
AST* parse_while                    (Parser_Context* ctx);
AST* parse_break                    (Parser_Context* ctx);
AST* parse_continue                 (Parser_Context* ctx);
AST* parse_string                   (Parser_Context* ctx);
AST* parse_sizeof                   (Parser_Context* ctx);
AST* parse_cast                     (Parser_Context* ctx);
AST* parse_enum                     (Parser_Context* ctx);
AST* parse_struct                   (Parser_Context* ctx);
AST* parse_comma_delim_list         (Parser_Context* ctx);
AST* parse_prefix                   (Parser_Context* ctx);
AST* parse_postfix                  (Parser_Context* ctx);
// clang-format on

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

// clang-format off
Type* parse_enum_signature             (Parser_Context* ctx, char* enum_name);
Type* parse_struct_signature           (Parser_Context* ctx, char* struct_name);
Type* parse_function_signature         (Parser_Context* ctx, char* func_name);
Type* parse_extern_function_signature  (Parser_Context* ctx, char* func_name);
// clang-format on

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

Parsed_File
generate_ast_from_tokens(Token_Array tokens) {
    info("Generating ast from tokens..");

    List* ast = make_list();

    Parser_Context ctx = make_parser_context();
    ctx.tokens         = tokens;

    eat(&ctx);

    while (!tok_is(&ctx, TOKEN_EOF)) {
        AST* stmt = parse_statement(&ctx);
        if (stmt) {
            list_append(ast, stmt);
        }
    }

    Parsed_File pf;
    pf.ast              = ast;
    pf.unresolved_types = ctx.unresolved_types;
    pf.variables_in_need_of_type_inference =
        ctx.variables_in_need_of_type_inference;
    pf.externs      = ctx.externs;
    pf.loads        = ctx.loads;
    pf.links        = ctx.links;
    pf.enums        = ctx.enums;
    pf.structs      = ctx.structs;
    pf.field_access = ctx.field_access;
    pf.subscripts   = ctx.subscripts;
    pf.calls        = ctx.calls;
    pf.constants    = ctx.constants;
    pf.identifiers  = ctx.identifiers;
    pf.symbols      = ctx.symbols;
    return pf;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

AST* parse_statement(Parser_Context* ctx) {
    DEBUG_START;
    // clang-format off
    switch (ctx->curr_tok.kind) {
    case TOKEN_BLOCK_START: return parse_block(ctx);
    case TOKEN_ENUM:        return parse_enum(ctx);
    case TOKEN_STRUCT:      return parse_struct(ctx);
    case TOKEN_IDENTIFIER:  return parse_expression(ctx);
    case TOKEN_RETURN:      return parse_return(ctx);
    case TOKEN_BREAK:       return parse_break(ctx);
    case TOKEN_CONTINUE:    return parse_continue(ctx);
    case TOKEN_IF:          return parse_if(ctx);
    case TOKEN_IS:          return parse_is(ctx);
    case TOKEN_FALLTHROUGH: return parse_fallthrough(ctx);
    case TOKEN_ELSE:        return parse_dangling_else(ctx);
    case TOKEN_DEFER:       return parse_defer(ctx);
    case TOKEN_FOR:         return parse_for(ctx);
    case TOKEN_WHILE:       return parse_while(ctx);
    case TOKEN_EXTERN:      return parse_extern(ctx);
    case TOKEN_LOAD:        return parse_load(ctx);
    case TOKEN_LINK:        return parse_link(ctx);
    default: error("Invalid statement token '%s'", token_to_str(ctx->curr_tok));
    }
    // clang-format on
    return NULL;
}

AST* parse_primary(Parser_Context* ctx) {
    DEBUG_START;
    // clang-format off
    switch (ctx->curr_tok.kind) {
    case TOKEN_DOT_DOT_DOT: eat(ctx); return make_ast_var_args(ctx->curr_tok);
    case TOKEN_TRUE:        eat(ctx); return make_ast_int(ctx->curr_tok, 1);
    case TOKEN_FALSE:       eat(ctx); return make_ast_int(ctx->curr_tok, 0);
    // case TOKEN_SIZEOF:      return parse_sizeof(ctx);
    case TOKEN_CAST:        return parse_cast(ctx);
    case TOKEN_IDENTIFIER:  return parse_identifier(ctx);
    case TOKEN_DOLLAR_SIGN: return parse_note(ctx);
    case TOKEN_FLOAT:       return parse_float(ctx);
    case TOKEN_CHAR:        // fallthrough
    case TOKEN_HEX:         // fallthrough
    case TOKEN_INTEGER:     return parse_integer(ctx);
    case TOKEN_STRING:      return parse_string(ctx);
    case TOKEN_OPEN_PAREN:  return parse_parens(ctx);
    default: error("Invalid primary token '%s'", token_to_str(ctx->curr_tok));
    }
    // clang-format on
    return NULL;
}

AST* parse_identifier(Parser_Context* ctx) {
    DEBUG_START;

    char* ident = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_IDENTIFIER);

    // clang-format off
    switch (ctx->curr_tok.kind) {
    case TOKEN_COLON:       // fallthrough
    case TOKEN_COLON_EQ:    return parse_variable_decl(ctx, ident);
    case TOKEN_COLON_COLON: return parse_constant_decl(ctx, ident);
    case TOKEN_OPEN_PAREN:  return parse_function_call(ctx, ident);
    }
    // clang-format on

    AST* i = make_ast_ident(ctx->curr_tok, ident);
    ast_ref_list_append(&ctx->identifiers, i);

    return i;
}

AST* parse_expression_identifier(Parser_Context* ctx) {
    DEBUG_START;

    char* ident = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_IDENTIFIER);

    if (ctx->curr_tok.kind == TOKEN_OPEN_PAREN) {
        return parse_function_call(ctx, ident);
    }

    AST* i = make_ast_ident(ctx->curr_tok, ident);
    ast_ref_list_append(&ctx->identifiers, i);

    return i;
}

AST* parse_struct(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_STRUCT);
    char* ident = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_struct_signature(ctx, ident);
    AST*  s    = make_ast_struct(ctx->curr_tok, type);
    map_set(ctx->symbols, ident, type);
    ast_ref_list_append(&ctx->structs, s);
    return s;
}

AST* parse_enum(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_ENUM);
    char* ident = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_enum_signature(ctx, ident);
    AST*  e    = make_ast_enum(ctx->curr_tok, type);
    map_set(ctx->symbols, ident, type);
    ast_ref_list_append(&ctx->enums, e);
    return e;
}

AST* parse_load(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_LOAD);
    char* str = strf("%s.thi", ctx->curr_tok.value);
    eat_kind(ctx, TOKEN_STRING);
    list_append(ctx->loads, str);
    return make_ast_load(ctx->curr_tok, str);
}

AST* parse_link(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_LINK);
    char* str = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_STRING);
    list_append(ctx->links, str);
    return make_ast_link(ctx->curr_tok, str);
}

AST* parse_extern(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_EXTERN);
    char* ident = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_extern_function_signature(ctx, ident);
    AST*  e    = make_ast_extern(ctx->curr_tok, type);
    map_set(ctx->symbols, ident, type);
    ast_ref_list_append(&ctx->externs, e);
    return e;
}

AST* parse_continue(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_CONTINUE);
    return make_ast_continue(ctx->curr_tok);
}

AST* parse_break(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_BREAK);
    return make_ast_break(ctx->curr_tok);
}

AST* parse_while(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_WHILE);
    AST* cond       = parse_expression(ctx);
    AST* then_block = parse_block(ctx);
    return make_ast_while(ctx->curr_tok, cond, then_block);
}

AST* parse_for(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_FOR);
    AST* init = parse_statement(ctx);
    eat_kind(ctx, TOKEN_COMMA);
    AST* cond = parse_expression(ctx);
    eat_kind(ctx, TOKEN_COMMA);
    AST* step       = parse_expression(ctx);
    AST* then_block = parse_block(ctx);
    return make_ast_for(ctx->curr_tok, init, cond, step, then_block);
}

AST* parse_defer(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_DEFER);
    AST* block = parse_block(ctx);
    return make_ast_defer(ctx->curr_tok, block);
}

AST* parse_fallthrough(Parser_Context* ctx) {
    DEBUG_START;
    Token t = ctx->curr_tok;
    eat_kind(ctx, TOKEN_FALLTHROUGH);
    return make_ast_fallthrough(t);
}
AST* parse_is(Parser_Context* ctx) {
    DEBUG_START;
    Token t = ctx->curr_tok;
    eat_kind(ctx, TOKEN_IS);
    AST* expr = parse_expression(ctx);
    AST* body = parse_block(ctx);

    bool has_fallthrough = false;
    LIST_FOREACH(body->Block.stmts) {
        AST* stmt = (AST*)it->data;
        if (stmt->kind == AST_FALLTHROUGH) {
            has_fallthrough = true;
            break;
        }
    }

    return make_ast_is(t, expr, body, has_fallthrough);
}

AST* parse_dangling_else(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_ELSE);
    AST* else_block = parse_block(ctx);
    set_dangling_else(ctx, else_block);
    return NULL;
}

AST* parse_if(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_IF);
    AST* cond       = parse_expression(ctx);
    AST* then_block = parse_block(ctx);
    AST* else_block = NULL;
    if (tok_is(ctx, TOKEN_ELSE)) {
        eat_kind(ctx, TOKEN_ELSE);
        else_block = parse_block(ctx);
    }
    AST* if_statement =
        make_ast_if(ctx->curr_tok, cond, then_block, else_block);
    set_if_statement(ctx, if_statement);
    return if_statement;
}

AST* parse_sizeof(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_SIZEOF);
    AST* expr = parse_expression(ctx);
    return make_ast_sizeof(ctx->curr_tok, expr);
}

AST* parse_string(Parser_Context* ctx) {
    DEBUG_START;
    char* value = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_STRING);
    return make_ast_string(ctx->curr_tok, value);
}

AST* parse_block(Parser_Context* ctx) {
    DEBUG_START;

    AST* block = NULL;

    // Is it a single statement?
    if (tok_is_on_same_line(ctx)) {
        List* stmts = make_list();
        AST*  stmt  = parse_statement(ctx);
        if (stmt) list_append(stmts, stmt);
        block = make_ast_block(ctx->curr_tok, stmts);
    }

    if (!block) {
        List* stmts = make_list();
        eat_kind(ctx, TOKEN_BLOCK_START);
        while (!tok_is(ctx, TOKEN_BLOCK_END)) {
            AST* stmt = parse_statement(ctx);
            if (stmt) {
                list_append(stmts, stmt);
            }
        }
        eat_kind(ctx, TOKEN_BLOCK_END);
        block = make_ast_block(ctx->curr_tok, stmts);
    }

    assert(block);

    restore_if_statement(ctx);

    return block;
}

AST* parse_return(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_RETURN);
    AST* expr = tok_is_on_same_line(ctx) ? parse_expression(ctx) : NULL;
    return make_ast_return(ctx->curr_tok, expr);
}

AST* parse_function_call(Parser_Context* ctx, char* ident) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    List* args                   = make_list();
    bool  has_multiple_arguments = false;
    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);
        AST* arg = parse_expression(ctx);
        // warning("%s", ast_to_json(arg));
        list_append(args, arg);
        has_multiple_arguments = true;
    }
    eat_kind(ctx, TOKEN_CLOSE_PAREN);

    // Is it a function def? It is if..
    if (tok_is_on_same_line(ctx) && tok_is(ctx, TOKEN_EQ_GT)) {
        // since '=>' means single statement function and implicit return
        // ..we only parse a SINGLE statement.
        eat_kind(ctx, TOKEN_EQ_GT);
        bool has_var_args = false;
        LIST_FOREACH(args) {
            AST* d = it->data;
            if (d->kind == AST_VAR_ARGS) {
                error("found var args %s", ast_to_str(d));
                has_var_args = true;
                break;
            }
        }
        Type* type = make_type_function(ident, args, NULL, has_var_args);
        map_set(ctx->symbols, ident, type);
        return make_ast_function(ctx->curr_tok, type, parse_block(ctx));
    }

    AST* call = make_ast_call(ctx->curr_tok, ident, args);
    ast_ref_list_append(&ctx->calls, call);
    return call;
}

AST* parse_constant_decl(Parser_Context* ctx, char* ident) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_COLON_COLON);
    AST* value         = parse_expression(ctx);
    AST* constant_decl = make_ast_constant_decl(ctx->curr_tok, ident, value);
    ast_ref_list_append(&ctx->constants, constant_decl);
    return constant_decl;
}

AST* parse_variable_decl(Parser_Context* ctx, char* ident) {
    DEBUG_START;

    char* variable_name  = ident;
    Type* variable_type  = NULL;
    AST*  variable_value = NULL;

    bool variable_needs_type_inference = false;

    switch (ctx->curr_tok.kind) {
    default:
        error("unhandled case: %s, %s, %s",
              token_kind_to_str(ctx->curr_tok.kind),
              __func__,
              __LINE__);
    case TOKEN_COLON: {
        eat_kind(ctx, TOKEN_COLON);
        variable_type = get_type(ctx);
        if (tok_is(ctx, TOKEN_EQ)) {
            eat_kind(ctx, TOKEN_EQ);
            variable_value = parse_expression(ctx);
        }
    } break;

    case TOKEN_COLON_EQ: {
        eat_kind(ctx, TOKEN_COLON_EQ);
        variable_value                = parse_expression(ctx);
        variable_needs_type_inference = true;
    } break;
    }

    AST* var_decl = make_ast_variable_decl(ctx->curr_tok, variable_name, variable_type, variable_value);
    if (variable_needs_type_inference) {
        ast_ref_list_append(&ctx->variables_in_need_of_type_inference,
                            var_decl);
    }

    if (tok_is(ctx, TOKEN_COMMA)) {
        // warning("%s", ctx->curr_tok.value);
    }

    return var_decl;
}

AST* parse_binary(Parser_Context* ctx, s8 expr_prec, AST* lhs) {
    DEBUG_START;

    AST* expr = NULL;

    // If this is a binop, find its precedence.
    for (;;) {
        s32 tok_prec = get_tok_precedence(ctx);

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (tok_prec < expr_prec) {
            expr = lhs;
            break;
        }

        // Okay, we know this is a binop.
        Token_Kind binary_op_token = ctx->curr_tok.kind;
        eat(ctx); // eat binop

        // Parse the unary expression after the binary operator.
        AST* rhs = parse_unary(ctx);
        if (!rhs) {
            expr = NULL;
            break;
        }
        // If BinOp binds less tightly with rhs than the operator after rhs, let
        // the pending operator take rhs as its lhs.
        s32 next_prec = get_tok_precedence(ctx);
        if (tok_prec < next_prec) {
            rhs = parse_binary(ctx, tok_prec + 1, rhs);

            if (!rhs) {
                expr = NULL;
                break;
            }
        }

        // Merge lhs/rhs.
        lhs = make_ast_binary(ctx->curr_tok, binary_op_token, lhs, rhs);
    }

    return expr;
}

AST* read_subscript_expr(Parser_Context* ctx, AST* expr) {
    eat_kind(ctx, TOKEN_OPEN_BRACKET);
    AST* sub = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_BRACKET);
    sub = make_ast_subscript(ctx->curr_tok, expr, sub);
    ast_ref_list_append(&ctx->subscripts, sub);
    return sub;
}

AST* read_field_access(Parser_Context* ctx, AST* expr) {
    eat_kind(ctx, TOKEN_DOT);
    char* field_name = ctx->curr_tok.value;
    eat_kind(ctx, TOKEN_IDENTIFIER);
    AST* field = make_ast_field_access(ctx->curr_tok, expr, field_name);
    // ast_ref_list_append(&ctx->field_access, field);
    return field;
}

AST* parse_postfix_tail(Parser_Context* ctx, AST* primary_expr) {
    DEBUG_START;
    if (!primary_expr) return NULL;
    for (;;) {
        if (tok_is(ctx, TOKEN_OPEN_BRACKET)) {
            primary_expr = read_subscript_expr(ctx, primary_expr);
            continue;
        }
        if (tok_is(ctx, TOKEN_DOT)) {
            primary_expr = read_field_access(ctx, primary_expr);
            continue;
        }
        // if (tok_is(ctx, TOKEN_COMMA)) {
        //     warning("first %s", ast_to_str(primary_expr));
        //     eat(ctx);
        //     warning("second %s", ctx->curr_tok.value);
        //     // primary_expr = read_field_access(ctx, primary_expr);
        //     continue;
        // }
        return primary_expr;
    }
}

AST* parse_prefix(Parser_Context* ctx) {
    DEBUG_START;
    return parse_unary(ctx);
}

AST* parse_postfix(Parser_Context* ctx) {
    DEBUG_START;
    AST* primary_expr = parse_primary(ctx);
    return parse_postfix_tail(ctx, primary_expr);
}

AST* parse_cast(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_CAST);
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    Type* type = get_type(ctx);
    eat_kind(ctx, TOKEN_COMMA);
    AST* expr = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_cast(ctx->curr_tok, expr, type);
}

AST* parse_unary(Parser_Context* ctx) {
    DEBUG_START;
    AST* unary = NULL;

    for (s8 i = 0; i < UNARY_OP_COUNT; ++i) {
        if (tok_is(ctx, unary_ops[i])) {
            Token_Kind op = ctx->curr_tok.kind;
            eat(ctx);
            AST* operand = parse_unary(ctx);
            if (operand) {
                unary = make_ast_unary(ctx->curr_tok, op, operand);
            }
        }
    }

    if (unary) {
        switch(unary->Unary.op) {
        case TOKEN_SIZEOF: {
            AST* node = make_ast_sizeof(unary->t, unary->Unary.operand);
            info("replaced %s with %s", ast_to_str(unary), ast_to_str(node));
            ast_replace(unary, node);
        } break;
        }
    }

    // If the current token is not an operator, it must be a primary expression.
    return unary ? unary : parse_postfix(ctx);
}

AST* parse_note(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_DOLLAR_SIGN);
    AST* expr = NULL;
    // clang-format off
    switch (ctx->curr_tok.kind) {
    case TOKEN_HEX:         expr = parse_integer(ctx); break;
    case TOKEN_INTEGER:     expr = parse_integer(ctx); break;
    default: error("unknown token when expecting a note");
    }
    // clang-format on
    return make_ast_note(ctx->curr_tok, expr);
}

AST* parse_expression(Parser_Context* ctx) {
    DEBUG_START;
    AST* lhs  = parse_prefix(ctx);
    AST* expr = lhs ? parse_binary(ctx, 0, lhs) : NULL;
    return expr;
}

AST* parse_float(Parser_Context* ctx) {
    DEBUG_START;
    AST* res = make_ast_float(ctx->curr_tok, get_float(ctx));
    return res;
}

AST* parse_integer(Parser_Context* ctx) {
    DEBUG_START;
    AST* res = make_ast_int(ctx->curr_tok, get_integer(ctx));
    return res;
}

AST* parse_parens(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    AST* expr = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(ctx->curr_tok, expr);
}

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

Type* parse_enum_signature(Parser_Context* ctx, char* enum_name) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_BLOCK_START);
    List* members = make_list();
    while (!tok_is(ctx, TOKEN_BLOCK_END)) {
        AST* expr = parse_identifier(ctx);
        if (expr) {
            list_append(members, expr);
        }
    }
    eat_kind(ctx, TOKEN_BLOCK_END);
    return make_type_enum(enum_name, members);
}

Type* parse_struct_signature(Parser_Context* ctx, char* struct_name) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_BLOCK_START);
    List* members = make_list();
    while (!tok_is(ctx, TOKEN_BLOCK_END)) {
        AST* expr = parse_statement(ctx);
        if (expr) {
            list_append(members, expr);
        }
    }
    eat_kind(ctx, TOKEN_BLOCK_END);
    return make_type_struct(struct_name, members);
}

Type* parse_extern_function_signature(Parser_Context* ctx, char* func_name) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    bool  has_var_args           = false;
    List* args                   = make_list();
    bool  has_multiple_arguments = false;
    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);
        if (tok_is(ctx, TOKEN_DOT_DOT_DOT)) {
            // var args
            eat(ctx);
            has_var_args = true;
        } else {
            Type* type = get_type(ctx);
            AST*  expr = make_ast_variable_decl(ctx->curr_tok, NULL, type, NULL);
            list_append(args, expr);
        }
        has_multiple_arguments = true;
    }
    eat_kind(ctx, TOKEN_CLOSE_PAREN);

    Type* ret_type = NULL;
    if (tok_is_on_same_line(ctx) && tok_is(ctx, TOKEN_IDENTIFIER)) {
        ret_type = get_type(ctx);
    }

    return make_type_function(func_name, args, ret_type, has_var_args);
}

Type* parse_function_signature(Parser_Context* ctx, char* func_name) {
    DEBUG_START;

    eat_kind(ctx, TOKEN_OPEN_PAREN);

    bool  has_var_args           = false;
    List* args                   = make_list();
    bool  has_multiple_arguments = false;

    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);

        if (tok_is(ctx, TOKEN_DOT_DOT_DOT)) {
            eat(ctx);
            has_var_args = true;
        } else {
            char* ident = ctx->curr_tok.value;
            eat_kind(ctx, TOKEN_IDENTIFIER);
            AST* expr = parse_variable_decl(ctx, ident);
            list_append(args, expr);
        }

        has_multiple_arguments = true;
    }

    eat_kind(ctx, TOKEN_CLOSE_PAREN);

    return make_type_function(func_name, args, NULL, has_var_args);
}
