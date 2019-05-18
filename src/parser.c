// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

//------------------------------------------------------------------------------
//                              parser.c
//------------------------------------------------------------------------------

#include "parser.h"

#include "ast.h"            // AST, ast_make_*
#include "constants.h"      // DEFAULT_INT_BYTE_SIZE, etc.
#include "lexer.h"          // Token, Token_Kind, generate_tokens_from_source, token_array_get_info_of
#include "map.h"            // Map
#include "parser_utility.h" // utility funcs
#include "type.h"           // Type, make_typspec_*,
#include "typedefs.h"       // s32 , s64, etc.
#include "utility.h"        // info, error, warning, success, strf, get_file_content
#include <assert.h>         // assert
#include <stdarg.h>         // va_list, va_start, va_end
#include <stdio.h>          // printf, vprintf
#include <stdlib.h>         // xmalloc
#include <string.h>         // strcmp

#define DEBUG_START \
    assert(ctx);    \
    // info("%s: %s", __func__, token_to_str(currTok(ctx)));

#define UNARY_OP_COUNT 9
Token_Kind unary_ops[UNARY_OP_COUNT] = {
    TOKEN_BANG,
    THI_SYNTAX_POINTER,
    TOKEN_PLUS,
    THI_SYNTAX_ADDRESS,
    TOKEN_MINUS,
    TOKEN_TILDE,
    TOKEN_DOT,
    TOKEN_SIZEOF,
    TOKEN_TYPEOF,
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
List* generate_ast_from_tokens         (Token* tokens);
// clang-format on

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

AST* parse(Parser_Context* ctx, char* file) {

    // Save state to recover from later
    char* last_file = ctx->file;
    char* last_dir  = ctx->dir;

    // Set the some state
    ctx->file = file;
    ctx->dir  = get_file_directory(file);

    char*  source    = get_file_content(file);
    Token* tokens    = generate_tokens_from_source(source);
    List*  top_level = generate_ast_from_tokens(tokens);

    AST* top_level_scope = make_ast_block(currTok(ctx), top_level);

    // Wrap it all in a module
    AST* ast = make_ast_module(currTok(ctx), file, top_level_scope);

    // Recover state
    ctx->file = last_file;
    ctx->dir  = last_dir;

    return ast;
}

List* generate_ast_from_tokens(Token* tokens) {
    info("Generating ast from tokens..");

    Parser_Context ctx = make_parser_context();
    ctx.tokens         = tokens;

    eat(&ctx); // prep the first token

    List* ast = make_list();

    while (!tok_is(&ctx, TOKEN_EOF)) {
        AST* stmt = parse_statement(&ctx);
        assert(stmt);
        list_append(ast, stmt);
    }

    return ast;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

AST* parse_statement(Parser_Context* ctx) {
    DEBUG_START;
    // clang-format off
    switch (tokKind(ctx)) {
    case TOKEN_BLOCK_START:         return parse_block(ctx);
    case TOKEN_ENUM:                return parse_enum(ctx);
    case TOKEN_STRUCT:              return parse_struct(ctx);
    case TOKEN_IDENTIFIER:          return parse_expression(ctx);
    case TOKEN_RETURN:              return parse_return(ctx);
    case TOKEN_BREAK:               return parse_break(ctx);
    case TOKEN_CONTINUE:            return parse_continue(ctx);
    case TOKEN_IF:                  return parse_if(ctx);
    case TOKEN_IS:                  return parse_is(ctx);
    case TOKEN_FALLTHROUGH:         return parse_fallthrough(ctx);
    case TOKEN_ELSE:                return parse_dangling_else(ctx);
    case TOKEN_DEFER:               return parse_defer(ctx);
    case TOKEN_FOR:                 return parse_for(ctx);
    case TOKEN_WHILE:               return parse_while(ctx);
    case TOKEN_EXTERN:              return parse_extern(ctx);
    case TOKEN_LOAD:                return parse_load(ctx);
    case TOKEN_LINK:                return parse_link(ctx);
    default: error("Invalid statement token '%s'", token_to_str(currTok(ctx)));
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

AST* parse_primary(Parser_Context* ctx) {
    DEBUG_START;
    // clang-format off
    switch (tokKind(ctx)) {
    case TOKEN_DOT_DOT_DOT: eat(ctx); return make_ast_var_args(currTok(ctx));
    case TOKEN_TRUE:        eat(ctx); return make_ast_int(currTok(ctx), 1);
    case TOKEN_FALSE:       eat(ctx); return make_ast_int(currTok(ctx), 0);
    case TOKEN_IDENTIFIER:  return parse_identifier(ctx);
    case TOKEN_DOLLAR_SIGN: return parse_note(ctx);
    case TOKEN_FLOAT:       return parse_float(ctx);
    case TOKEN_CHAR:        // fallthrough
    case TOKEN_HEX:         // fallthrough
    case TOKEN_INTEGER:     return parse_integer(ctx);
    case TOKEN_STRING:      return parse_string(ctx);
    case TOKEN_OPEN_PAREN:  return parse_parens(ctx);
    default: error("Invalid primary token '%s'", token_to_str(currTok(ctx)));
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

AST* parse_identifier(Parser_Context* ctx) {
    DEBUG_START;

    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);

    // clang-format off
    switch (tokKind(ctx)) {
    case TOKEN_COLON:       // fallthrough
    case TOKEN_COLON_EQ:    return parse_variable_decl(ctx, ident);
    case TOKEN_COLON_COLON: return parse_constant_decl(ctx, ident);
    case TOKEN_OPEN_PAREN:  return parse_function_call(ctx, ident);
    default:                return make_ast_ident(currTok(ctx), ident);
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

AST* parse_expression_identifier(Parser_Context* ctx) {
    DEBUG_START;
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    if (tokKind(ctx) == TOKEN_OPEN_PAREN) return parse_function_call(ctx, ident);
    return make_ast_ident(currTok(ctx), ident);
}

AST* parse_struct(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_STRUCT);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_struct_signature(ctx, ident);
    return make_ast_struct(currTok(ctx), type);
}

AST* parse_enum(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_ENUM);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_enum_signature(ctx, ident);
    return make_ast_enum(currTok(ctx), type);
}

AST* parse_load(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_LOAD);
    char* str = strf("%s.thi", tokValue(ctx));
    eat_kind(ctx, TOKEN_STRING);
    AST* ast = parse(ctx, str);
    list_append(ctx->loads, str);
    return ast;
}

AST* parse_link(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_LINK);
    char* str = tokValue(ctx);
    eat_kind(ctx, TOKEN_STRING);
    return make_ast_link(currTok(ctx), str);
}

AST* parse_extern(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_EXTERN);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_extern_function_signature(ctx, ident);
    AST*  e    = make_ast_extern(currTok(ctx), type);
    return e;
}

AST* parse_continue(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_CONTINUE);
    return make_ast_continue(currTok(ctx));
}

AST* parse_break(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_BREAK);
    return make_ast_break(currTok(ctx));
}

AST* parse_while(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_WHILE);
    AST* cond       = parse_expression(ctx);
    AST* then_block = parse_block(ctx);
    return make_ast_while(currTok(ctx), cond, then_block);
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
    return make_ast_for(currTok(ctx), init, cond, step, then_block);
}

AST* parse_defer(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_DEFER);
    AST* block = parse_block(ctx);
    return make_ast_defer(currTok(ctx), block);
}

AST* parse_fallthrough(Parser_Context* ctx) {
    DEBUG_START;
    Token t = currTok(ctx);
    eat_kind(ctx, TOKEN_FALLTHROUGH);
    return make_ast_fallthrough(t);
}
AST* parse_is(Parser_Context* ctx) {
    DEBUG_START;
    Token t = currTok(ctx);
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

void maybe_convert_if_to_switch(AST* expr) {

    AST* then_block = expr->If.then_block;

    // check for any AST_IS inside
    List* stmts                             = then_block->Block.stmts;
    bool  if_statement_is_actually_a_switch = false;

    LIST_FOREACH(stmts) {
        AST* stmt = it->data;
        if (stmt->kind == AST_IS) {
            if_statement_is_actually_a_switch = true;
            break;
        }
    }

    // Go through it again and make sure every statement
    // is a case
    if (if_statement_is_actually_a_switch) {
        // Find all NON switchy things and post and error
        LIST_FOREACH(stmts) {
            AST* stmt = it->data;
            if (stmt->kind != AST_IS) {
                error(
                    "%s\nonly 'case' statements are allowed inside an if "
                    "switch",
                    ast_to_str(stmt));
            }
        }

        // Transform this if into a switch
        ast_replace(expr, make_ast_switch(expr->t, expr));
    }
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
    AST* if_statement = make_ast_if(currTok(ctx), cond, then_block, else_block);
    set_if_statement(ctx, if_statement);

    maybe_convert_if_to_switch(if_statement);

    return if_statement;
}

AST* parse_string(Parser_Context* ctx) {
    DEBUG_START;
    char* value = tokValue(ctx);
    eat_kind(ctx, TOKEN_STRING);
    return make_ast_string(currTok(ctx), value);
}

AST* parse_block(Parser_Context* ctx) {
    DEBUG_START;

    AST* block = NULL;

    // Is it a single statement?
    if (tok_is_on_same_line(ctx)) {
        List* stmts = make_list();
        AST*  stmt  = parse_statement(ctx);
        if (stmt) list_append(stmts, stmt);
        block = make_ast_block(currTok(ctx), stmts);
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
        block = make_ast_block(currTok(ctx), stmts);
    }

    assert(block);

    restore_if_statement(ctx);

    return block;
}

AST* parse_return(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_RETURN);
    AST* expr = tok_is_on_same_line(ctx) ? parse_expression(ctx) : NULL;
    return make_ast_return(currTok(ctx), expr);
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
        return make_ast_function(currTok(ctx), type, parse_block(ctx));
    } else if (tok_is(ctx, TOKEN_BLOCK_START)) {
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
        return make_ast_function(currTok(ctx), type, parse_block(ctx));
    }

    return make_ast_call(currTok(ctx), ident, args);
}

AST* parse_constant_decl(Parser_Context* ctx, char* ident) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_COLON_COLON);
    AST* value = parse_expression(ctx);
    return make_ast_constant_decl(currTok(ctx), ident, value);
}

AST* parse_variable_decl(Parser_Context* ctx, char* ident) {
    DEBUG_START;

    char* variable_name  = ident;
    Type* variable_type  = NULL;
    AST*  variable_value = NULL;

    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    case TOKEN_COLON:
        eat_kind(ctx, TOKEN_COLON);
        variable_type = get_type(ctx);
        if (tok_is(ctx, TOKEN_EQ)) {
            eat_kind(ctx, TOKEN_EQ);
            variable_value = parse_expression(ctx);
        }
        break;

    case TOKEN_COLON_EQ:
        eat_kind(ctx, TOKEN_COLON_EQ);
        variable_value = parse_expression(ctx);
        break;
    }

    return make_ast_variable_decl(currTok(ctx), variable_name, variable_type, variable_value);
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
        Token_Kind binary_op_token = tokKind(ctx);
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
        lhs = make_ast_binary(currTok(ctx), binary_op_token, lhs, rhs);
    }

    return expr;
}

AST* read_as(Parser_Context* ctx, AST* expr) {
    eat_kind(ctx, TOKEN_AS);
    AST* type_expr = parse_expression(ctx);
    return make_ast_as(currTok(ctx), expr, type_expr);
}

AST* read_subscript_expr(Parser_Context* ctx, AST* expr) {
    eat_kind(ctx, TOKEN_OPEN_BRACKET);
    AST* sub = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_BRACKET);
    sub = make_ast_subscript(currTok(ctx), expr, sub);
    return sub;
}

AST* read_field_access(Parser_Context* ctx, AST* expr) {
    eat_kind(ctx, TOKEN_DOT);
    char* field_name = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    AST* field = make_ast_field_access(currTok(ctx), expr, field_name);
    return field;
}

AST* parse_postfix_tail(Parser_Context* ctx, AST* primary_expr) {
    DEBUG_START;
    assert(primary_expr);
    for (;;) {
        if (tok_is(ctx, TOKEN_AS)) {
            primary_expr = read_as(ctx, primary_expr);
            continue;
        } else if (tok_is(ctx, TOKEN_OPEN_BRACKET)) {
            primary_expr = read_subscript_expr(ctx, primary_expr);
            continue;
        } else if (tok_is(ctx, TOKEN_DOT)) {
            primary_expr = read_field_access(ctx, primary_expr);
            continue;
        }
        return primary_expr;
    }
    UNREACHABLE;
    return NULL;
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

AST* parse_unary(Parser_Context* ctx) {
    DEBUG_START;
    AST* unary = NULL;

    for (s8 i = 0; i < UNARY_OP_COUNT; ++i) {
        if (tok_is(ctx, unary_ops[i])) {
            Token_Kind op = tokKind(ctx);
            eat(ctx);
            AST* operand = parse_unary(ctx);
            if (operand) {
                unary = make_ast_unary(currTok(ctx), op, operand);
            }
        }
    }

    if (unary) {
        if (unary->Unary.op == TOKEN_SIZEOF) {
            ast_replace(unary, make_ast_sizeof(unary->t, unary->Unary.operand));
        } else if (unary->Unary.op == TOKEN_TYPEOF) {
            ast_replace(unary, make_ast_typeof(unary->t, unary->Unary.operand));
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
    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    case TOKEN_HEX:         expr = parse_integer(ctx); break;
    case TOKEN_INTEGER:     expr = parse_integer(ctx); break;
    }
    // clang-format on
    return make_ast_note(currTok(ctx), expr);
}

AST* parse_expression(Parser_Context* ctx) {
    DEBUG_START;
    AST* lhs  = parse_prefix(ctx);
    AST* expr = lhs ? parse_binary(ctx, 0, lhs) : NULL;
    return expr;
}

AST* parse_float(Parser_Context* ctx) {
    DEBUG_START;
    AST* res = make_ast_float(currTok(ctx), get_float(ctx));
    return res;
}

AST* parse_integer(Parser_Context* ctx) {
    DEBUG_START;
    AST* res = make_ast_int(currTok(ctx), get_integer(ctx));
    return res;
}

AST* parse_parens(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    AST* expr = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(currTok(ctx), expr);
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
            AST*  expr = make_ast_variable_decl(currTok(ctx), NULL, type, NULL);
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
            char* ident = tokValue(ctx);
            eat_kind(ctx, TOKEN_IDENTIFIER);
            AST* expr = parse_variable_decl(ctx, ident);
            list_append(args, expr);
        }

        has_multiple_arguments = true;
    }

    eat_kind(ctx, TOKEN_CLOSE_PAREN);

    return make_type_function(func_name, args, NULL, has_var_args);
}
