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

#include "ast.h"       // AST, ast_make_*
#include "constants.h" // DEFAULT_INT_BYTE_SIZE, etc.
// #include "cst.h"            // CST
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
    info("%s: %s", __func__, token_to_str(currTok(ctx)));

#define UNARY_OP_COUNT 10
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
    TOKEN_PLUS_PLUS,
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//              Each construct of the language gets its own function
//------------------------------------------------------------------------------

// clang-format off
AST* parse_top_level                (Parser_Context* ctx);
AST* parse_statement                (Parser_Context* ctx);
AST* parse_primary                  (Parser_Context* ctx);
AST* parse_identifier               (Parser_Context* ctx);
AST* parse_top_level_identifier     (Parser_Context* ctx);
AST* parse_expression_identifier    (Parser_Context* ctx);
AST* parse_def                      (Parser_Context* ctx);
AST* parse_variable_decl            (Parser_Context* ctx, Loc_Info lc, char* ident);
AST* parse_constant_decl            (Parser_Context* ctx, Loc_Info lc, char* ident);
AST* parse_function_decl            (Parser_Context* ctx, Loc_Info lc, char* ident);
AST* parse_function_call            (Parser_Context* ctx, Loc_Info lc, char* ident);
AST* parse_block                    (Parser_Context* ctx);
AST* parse_return                   (Parser_Context* ctx);
AST* parse_note                     (Parser_Context* ctx);
AST* parse_expression               (Parser_Context* ctx);
AST* parse_unary                    (Parser_Context* ctx);
AST* parse_binary                   (Parser_Context* ctx, s8 min_prec, AST* lhs);
AST* parse_integer                  (Parser_Context* ctx);
AST* parse_char                     (Parser_Context* ctx);
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
AST* parse_comma_delim_list         (Parser_Context* ctx);
AST* parse_prefix                   (Parser_Context* ctx);
AST* parse_postfix                  (Parser_Context* ctx);

void skip_comments_or_newlines(Parser_Context* ctx);

// clang-format on

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

// clang-format off
Type* parse_enum_signature             (Parser_Context* ctx, char* enum_name);
Type* parse_type_signature             (Parser_Context* ctx, char* struct_name);
Type* parse_extern_function_signature  (Parser_Context* ctx, char* func_name);
List* generate_ast_from_tokens         (Parser_Context* ctx);
Type* get_type                         (Parser_Context* ctx);
// clang-format on

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

AST* parse(Parser_Context* ctx, char* file) {

    info("Parsing %s", file);

    Loc_Info lc = loc(ctx);

    // Save state
    Token* saved_tokens = ctx->tokens;
    char* saved_file = ctx->file;
    char* saved_dir = ctx->dir;
    Token saved_top_tok = ctx->top_tok;
    Token saved_curr_tok = ctx->curr_tok;
    Token saved_prev_tok = ctx->prev_tok;
    AST* saved_llast_if_statement = ctx->llast_if_statement;
    AST* saved_olast_if_statement = ctx->olast_if_statement;
    //

    char* last_dir = get_file_directory(ctx->file);

    ctx->file = strf("%s%s", last_dir ? last_dir : "", file);

    // Check if we've already loaded the file before.
    bool already_loaded = false;
    LIST_FOREACH(ctx->loads) {
        // info(it->data);
        char* already_loaded_file = it->data;
        if (strcmp(ctx->file, already_loaded_file) == 0) already_loaded = true;
    }
    if (already_loaded) return NULL;

    char* file_path = ctx->file;

    // Add it to the list of loaded files.
    list_append(ctx->loads, file_path);

    Lexed_File lf = generate_tokens_from_file(file_path);

    ctx->tokens = lf.tokens.data;
    ctx->lines += lf.lines;
    ctx->comments += lf.comments;

    List* top_level_ast = generate_ast_from_tokens(ctx);
    AST* ast = make_ast_module(lc, file_path, top_level_ast);

    // Restore state
    ctx->tokens = saved_tokens;
    ctx->file = saved_file;
    ctx->dir = saved_dir;
    ctx->top_tok = saved_top_tok;
    ctx->curr_tok = saved_curr_tok;
    ctx->prev_tok = saved_prev_tok;
    ctx->llast_if_statement = saved_llast_if_statement;
    ctx->olast_if_statement = saved_olast_if_statement;
    //

    return ast;
}

List* generate_ast_from_tokens(Parser_Context* ctx) {
    info("Generating AST from tokens..");
    eat(ctx); // prep the first token
    List* ast = make_list();
    while (!tok_is(ctx, TOKEN_EOF)) {
        AST* stmt = parse_statement(ctx);
        if (stmt) list_append(ast, stmt);
    }
    return ast;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

AST* parse_top_level(Parser_Context* ctx) {
    DEBUG_START;
    ctx->top_tok = ctx->curr_tok;
    // clang-format off
    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    case TOKEN_IDENTIFIER:          return parse_top_level_identifier(ctx);
    case TOKEN_EXTERN:              return parse_extern(ctx);
    case TOKEN_LOAD:                return parse_load(ctx);
    case TOKEN_LINK:                return parse_link(ctx);
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

AST* parse_delimited(Parser_Context* ctx, Token_Kind delimiter) {
    Token_Kind kind = tokKind(ctx);
    if (kind == delimiter) return NULL;
    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    }
    UNREACHABLE;
    return NULL;
}

// List* top_level = parse_delimited_and_terminated_list(ctx, TOKEN_NEWLINE, TOKEN_EOF);
// List* stmts = parse_delimited_and_terminated_list(ctx, TOKEN_NEWLINE, TOKEN_BLOCK_END);

List* parse_delimited_and_terminated_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*, Token_Kind), Token_Kind delimiter, Token_Kind terminator) {
    List* nodes = make_list();
    while (!tok_is(ctx, terminator)) {
        AST* node = (*parseFunc)(ctx, delimiter);
        if (node) list_append(nodes, node);
    }
    return nodes;
}

List* parse_delimited_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind delimiter) {
    List* nodes = make_list();
    while (!tok_is(ctx, delimiter)) {
        AST* node = (*parseFunc)(ctx);
        if (node) list_append(nodes, node);
    }
    return nodes;
}

AST* parse_statement(Parser_Context* ctx) {
    DEBUG_START;
    ctx->top_tok = ctx->curr_tok;

    AST* result = NULL;
    // clang-format off
    switch (tokKind(ctx)) {
    default:                        result =  parse_expression(ctx);    break;
    case TOKEN_EOF:                 eat(ctx); break;
    case TOKEN_NEWLINE:             eat(ctx); result = NULL; break;
    case TOKEN_COMMENT:             result = make_ast_comment(loc(ctx), tokValue(ctx)); eat(ctx); break;
    case TOKEN_DEF:                 result =  parse_def(ctx);           break;

    case TOKEN_IF:                  result =  parse_if(ctx);            break;
    case TOKEN_IS:                  result =  parse_is(ctx);            break;
    case TOKEN_ELSE:                result =  parse_dangling_else(ctx); break;
    case TOKEN_DEFER:               result =  parse_defer(ctx);         break;
    case TOKEN_FOR:                 result =  parse_for(ctx);           break;
    case TOKEN_WHILE:               result =  parse_while(ctx);         break;

    case TOKEN_RETURN:              result =  parse_return(ctx);        break;
    case TOKEN_BREAK:               result =  parse_break(ctx);         break;
    case TOKEN_CONTINUE:            result =  parse_continue(ctx);      break;
    case TOKEN_FALLTHROUGH:         result =  parse_fallthrough(ctx);   break;
    case TOKEN_LOAD:                result =  parse_load(ctx);          break;
    case TOKEN_LINK:                result =  parse_link(ctx);          break;
    case TOKEN_EXTERN:              result =  parse_extern(ctx);        break;
    }
    // clang-format on
    return result;
}

AST* parse_primary(Parser_Context* ctx) {
    DEBUG_START;
    AST* result = NULL;
start:
    // clang-format off
    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    case TOKEN_COMMENT:             result = make_ast_comment(loc(ctx), tokValue(ctx)); eat(ctx); break;
    case TOKEN_NEWLINE:
        eat(ctx); 
        if (!ctx->inside_parens) { result = NULL; break;}
        else goto start;
    case TOKEN_DOT_DOT_DOT: result = make_ast_var_args(loc(ctx)); eat(ctx); break;
    case TOKEN_TRUE:        result = make_ast_int(loc(ctx), 1, make_type_int(1, 1)); eat(ctx); break;
    case TOKEN_FALSE:       result = make_ast_int(loc(ctx), 0, make_type_int(1, 1)); eat(ctx); break;
    case TOKEN_IDENTIFIER:  result = parse_identifier(ctx); break;
    case TOKEN_DOLLAR_SIGN: result = parse_note(ctx); break;
    case TOKEN_FLOAT:       result = parse_float(ctx); break;
    case TOKEN_CHAR:        result = parse_char(ctx); break;
    case TOKEN_HEX:         // fallthrough
    case TOKEN_INTEGER:     result = parse_integer(ctx); break;
    case TOKEN_STRING:      result = parse_string(ctx); break;
    case TOKEN_OPEN_PAREN:  result = parse_parens(ctx); break;
    }
    // clang-format on
    return result;
}

AST* parse_top_level_identifier(Parser_Context* ctx) {
    DEBUG_START;

    Loc_Info lc = loc(ctx);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);

    // clang-format off
    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    case TOKEN_COLON_COLON: return parse_constant_decl(ctx, lc, ident);
    case TOKEN_OPEN_PAREN:  return parse_function_decl(ctx, lc, ident);
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

AST* parse_identifier(Parser_Context* ctx) {
    DEBUG_START;

    Loc_Info lc = loc(ctx);

    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);

    // clang-format off
    switch (tokKind(ctx)) {
    // case TOKEN_COLON:       // fallthrough
    case TOKEN_IDENTIFIER:  return parse_variable_decl(ctx, lc, ident);
    case TOKEN_COLON_COLON: return parse_constant_decl(ctx, lc, ident);
    case TOKEN_OPEN_PAREN:  return parse_function_call(ctx, lc, ident);
    default:                return make_ast_ident(lc, ident);
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

AST* parse_expression_identifier(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    if (tokKind(ctx) == TOKEN_OPEN_PAREN) return parse_function_call(ctx, lc, ident);
    return make_ast_ident(lc, ident);
}

AST* parse_load(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_LOAD);
    char* file = tokValue(ctx);
    char* ext = get_file_extension(file);
    if (!ext) file = strf("%s.thi", file);
    eat_kind(ctx, TOKEN_STRING);
    AST* module = parse(ctx, file);
    return make_ast_load(lc, file, module);
}

AST* parse_link(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_LINK);
    char* str = tokValue(ctx);
    eat_kind(ctx, TOKEN_STRING);
    return make_ast_link(lc, str);
}

AST* parse_extern(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_EXTERN);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    Type* type = parse_extern_function_signature(ctx, ident);
    AST* e = make_ast_extern(lc, type);
    return e;
}

AST* parse_continue(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_CONTINUE);
    return make_ast_continue(lc);
}

AST* parse_break(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_BREAK);
    return make_ast_break(lc);
}

AST* parse_while(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_WHILE);
    AST* cond = parse_expression(ctx);
    AST* then_block = parse_block(ctx);
    return make_ast_while(lc, cond, then_block);
}

AST* parse_for(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_FOR);

    AST* init = parse_statement(ctx);
    AST* cond = NULL;
    AST* step = NULL;
    AST* then_block = NULL;

    if (tok_is(ctx, TOKEN_IN)) {

        // for param in params
        // expands into..
        // for it = 0, it < len(params), ++it
        //     param := params[it]

        // // WE ARE A 'FOR IN'
        // eat_kind(ctx, TOKEN_IN);

        // // Parse the rhs
        // AST* after_in = parse_expression(ctx);

        // AST* it = make_ast_ident(loc(ctx), DEFAULT_FOR_IN_ITERATOR_NAME);
        // AST* it_var = make_ast_binary(loc(ctx), TOKEN_EQ, init, make_ast_subscript(loc(ctx), after_in, it));

        // init = make_ast_binary(loc(ctx), TOKEN_EQ, it, make_ast_int(loc(ctx), 0));
        // cond = make_ast_binary(loc(ctx), TOKEN_LT, it, make_ast_int(loc(ctx), 7)); //@HARDCODED
        // step = make_ast_unary(loc(ctx), TOKEN_PLUS_PLUS, it);
        // then_block = parse_block(ctx);

        // // Place the 'param' variable at the start of the block
        // list_prepend(then_block->Block.stmts, it_var);
    } else {
        cond = parse_expression(ctx);
        step = parse_statement(ctx);
        then_block = parse_block(ctx);
    }
    return make_ast_for(lc, init, cond, step, then_block);
}

AST* parse_defer(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_DEFER);
    AST* block = parse_block(ctx);
    return make_ast_defer(lc, block);
}

AST* parse_fallthrough(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_FALLTHROUGH);
    return make_ast_fallthrough(lc);
}
AST* parse_is(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_IS);
    AST* expr = parse_expression(ctx);
    AST* body = parse_block(ctx);

    bool has_fallthrough = false;
    LIST_FOREACH(body->Block.stmts) {
        AST* stmt = it->data;
        if (stmt->kind == AST_FALLTHROUGH) {
            has_fallthrough = true;
            break;
        }
    }

    return make_ast_is(lc, expr, body, has_fallthrough);
}

AST* parse_dangling_else(Parser_Context* ctx) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_ELSE);
    AST* else_block = parse_block(ctx);
    set_dangling_else(ctx, else_block);
    return NULL;
}

void maybe_convert_if_to_switch(AST* node) {

    AST* then_block = node->If.then_block;

    // check for any AST_IS inside
    List* stmts = then_block->Block.stmts;
    bool if_statement_is_actually_a_switch = false;

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
        ast_replace(node, make_ast_switch(node->loc_info, node));
    }
}

AST* parse_if(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_IF);
    AST* cond = parse_expression(ctx);
    AST* then_block = parse_block(ctx);
    AST* else_block = NULL;
    if (tok_is(ctx, TOKEN_ELSE)) {
        eat_kind(ctx, TOKEN_ELSE);
        else_block = parse_block(ctx);
    }
    AST* if_statement = make_ast_if(lc, cond, then_block, else_block);
    set_if_statement(ctx, if_statement);

    maybe_convert_if_to_switch(if_statement);

    return if_statement;
}

AST* parse_string(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    char* value = tokValue(ctx);
    eat_kind(ctx, TOKEN_STRING);
    return make_ast_string(lc, value);
}

AST* parse_block(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);

    AST* block = NULL;
    u32 flags = 0;

    // Skip junk
    skip_comments_or_newlines(ctx);

    // are we parsing a single line expression list thingy?
    // if (tok_is_on_same_line(ctx)) {
    //     if (tok_is(ctx, TOKEN_EQ_GT)) { // =>
    //         eat(ctx);
    //         flags = BLOCK_LAST_EXPR_IS_IMPLICITLY_RETURNED;
    //     }
    //     List* stmts = make_list();
    //     AST*  stmt  = parse_statement(ctx);
    //     stmt        = make_ast_return(stmt->loc_info, stmt);
    //     list_append(stmts, stmt);
    //     block = make_ast_block(loc(ctx), stmts);
    // }

    eat_kind(ctx, TOKEN_BLOCK_START);
    List* stmts = parse_delimited_list(ctx, parse_statement, TOKEN_BLOCK_END);
    eat_kind(ctx, TOKEN_BLOCK_END);
    block = make_ast_block(lc, stmts);

    block->Block.flags |= flags;

    assert(block);

    restore_if_statement(ctx);

    return block;
}

AST* parse_return(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_RETURN);
    AST* expr = tok_is_on_same_line(ctx) ? parse_expression(ctx) : NULL;
    return make_ast_return(lc, expr);
}

AST* parse_function_call(Parser_Context* ctx, Loc_Info lc, char* ident) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    List* args = make_list();
    bool has_multiple_arguments = false;
    ctx->inside_parens = true;
    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);
        AST* arg = parse_expression(ctx);
        if (arg) list_append(args, arg);
        has_multiple_arguments = true;
    }
    ctx->inside_parens = false;
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_call(lc, ident, args);
}

AST* parse_function_decl(Parser_Context* ctx, Loc_Info lc, char* ident) {
    DEBUG_START;

    eat_kind(ctx, TOKEN_OPEN_PAREN);
    List* params_t = make_list();
    List* params = make_list();
    bool has_multiple_arguments = false;
    u32 flags = 0;

    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);
        AST* param = parse_expression(ctx);

        Type_Name_Pair* tp = xmalloc(sizeof(Type_Name_Pair));
        tp->name = get_ast_name(param);
        tp->type = param->type;

        list_append(params_t, tp);
        list_append(params, param);
        has_multiple_arguments = true;
    }

    eat_kind(ctx, TOKEN_CLOSE_PAREN);

    char* func_name = ident;
    Type* ret_type = (tok_is_on_same_line(ctx) && !tok_is(ctx, TOKEN_NEWLINE)) ? get_type(ctx) : make_type_void();
    Type* func_type = make_type_function(func_name, params_t, ret_type, flags);
    AST* func_body = parse_block(ctx);

    return make_ast_function(lc, func_name, params, func_type, func_body);
}

AST* parse_constant_decl(Parser_Context* ctx, Loc_Info lc, char* ident) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_COLON_COLON);
    AST* value = parse_expression(ctx);
    return make_ast_constant_decl(lc, ident, value);
}

AST* parse_def(Parser_Context* ctx) {
    DEBUG_START;

    Loc_Info lc = loc(ctx);

    eat_kind(ctx, TOKEN_DEF);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);

    if (tok_is(ctx, TOKEN_OPEN_PAREN)) {
        return parse_function_decl(ctx, lc, ident);
    }

    AST* body = parse_block(ctx);

    // figure out if it's an enum or struct
    bool is_enum = true;

    List* members = body->Block.stmts;
    LIST_FOREACH(members) {
        AST* stmt = it->data;
        switch (stmt->kind) {
        case AST_IDENT: break;
        case AST_CONSTANT_DECL: break;
        default:
            is_enum = false;
            break;
        }
    }

    return is_enum ? make_ast_enum(lc, ident, members) : make_ast_struct(lc, ident, members);
}

AST* parse_variable_decl(Parser_Context* ctx, Loc_Info lc, char* ident) {
    DEBUG_START;
    Type* type = get_type(ctx);
    AST* value = NULL;
    if (tok_is(ctx, TOKEN_EQ)) {
        eat(ctx);
        value = parse_expression(ctx);
    }
    return make_ast_variable_decl(lc, ident, type, value);
}

AST* parse_binary(Parser_Context* ctx, s8 expr_prec, AST* lhs) {
    DEBUG_START;
    warning("missing loc_info for binary nodes");

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
        lhs = make_ast_binary(loc(ctx), binary_op_token, lhs, rhs);
    }

    return expr;
}

void skip_comments_or_newlines(Parser_Context* ctx) {
    while (tok_is(ctx, TOKEN_COMMENT) || tok_is(ctx, TOKEN_NEWLINE)) {
        eat(ctx);
    }
}

AST* read_as(Parser_Context* ctx, AST* expr) {
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_AS);
    AST* type_expr = parse_expression(ctx);
    return make_ast_as(lc, expr, type_expr);
}

AST* read_subscript_expr(Parser_Context* ctx, AST* expr) {
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_OPEN_BRACKET);
    AST* sub = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_BRACKET);
    sub = make_ast_subscript(lc, expr, sub);
    return sub;
}

AST* read_field_access(Parser_Context* ctx, AST* expr) {
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_DOT);
    char* field_name = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    AST* field = make_ast_field_access(lc, expr, field_name);
    return field;
}

AST* parse_postfix_tail(Parser_Context* ctx, AST* primary_expr) {
    DEBUG_START;
    // assert(primary_expr);
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
    Loc_Info lc = loc(ctx);
    AST* unary = NULL;

    for (s8 i = 0; i < UNARY_OP_COUNT; ++i) {
        if (tok_is(ctx, unary_ops[i])) {
            Token_Kind op = tokKind(ctx);
            eat(ctx);
            AST* operand = parse_unary(ctx);
            if (operand) {
                unary = make_ast_unary(lc, op, operand);
            }
        }
    }

    if (unary) {
        if (unary->Unary.op == TOKEN_SIZEOF) {
            ast_replace(unary, make_ast_sizeof(unary->loc_info, unary->Unary.operand));
        } else if (unary->Unary.op == TOKEN_TYPEOF) {
            ast_replace(unary, make_ast_typeof(unary->loc_info, unary->Unary.operand));
        }
    }

    // If the current token is not an operator, it must be a primary expression.
    return unary ? unary : parse_postfix(ctx);
}

AST* parse_note(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_DOLLAR_SIGN);
    AST* expr = NULL;
    // clang-format off
    switch (tokKind(ctx)) {
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(tokKind(ctx)));
    case TOKEN_HEX:         expr = parse_integer(ctx); break;
    case TOKEN_INTEGER:     expr = parse_integer(ctx); break;
    }
    // clang-format on
    return make_ast_note(lc, expr);
}

AST* parse_expression(Parser_Context* ctx) {
    DEBUG_START;
    AST* lhs = parse_prefix(ctx);
    AST* expr = lhs ? parse_binary(ctx, 0, lhs) : NULL;

    // is it an expression list?
    // if (tok_is_on_same_line(ctx) && tok_is(ctx, TOKEN_COMMA)) {
    //     List* exprs = make_list();
    //     list_append(exprs, expr);
    //     do {
    //         eat(ctx); // eat the comma
    //         AST* expr = parse_expression(ctx);
    //         list_append(exprs, expr);
    //     } while (tok_is(ctx, TOKEN_COMMA));
    //     expr = make_ast_block(loc(ctx), exprs);
    // }
    return expr;
}

AST* parse_float(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    AST* res = make_ast_float(lc, get_float(ctx));
    return res;
}

AST* parse_char(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    AST* res = make_ast_char(lc, (char)get_integer(ctx));
    return res;
}

AST* parse_integer(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    AST* res = make_ast_int(lc, get_integer(ctx), make_type_int(DEFAULT_INT_BYTE_SIZE, 0));
    return res;
}

AST* parse_parens(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    ctx->inside_parens = true;
    AST* expr = parse_expression(ctx);
    ctx->inside_parens = false;
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(lc, expr);
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

Type* parse_type_signature(Parser_Context* ctx, char* struct_name) {
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
    List* params = make_list();
    bool has_multiple_arguments = false;
    u32 flags = 0;
    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);

        Type_Name_Pair* tp = xmalloc(sizeof(Type_Name_Pair));
        tp->name = NULL;
        tp->type = get_type(ctx);

        if (tp->type->kind == TYPE_VAR_ARGS) {
            flags |= TYPE_FLAG_HAS_VAR_ARG;
        }
        list_append(params, tp);
        has_multiple_arguments = true;
    }
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    Type* ret_type = (tok_is_on_same_line(ctx) && !tok_is(ctx, TOKEN_NEWLINE)) ? get_type(ctx) : make_type_void();
    return make_type_function(func_name, params, ret_type, flags);
}

Type* get_type(Parser_Context* ctx) {
    DEBUG_START;
    Type* type = NULL;
    if (tok_is(ctx, TOKEN_DOT_DOT_DOT)) {
        eat(ctx);
        return make_type_var_args();
    }
    if (tok_is(ctx, TOKEN_IDENTIFIER)) {
        char* type_name = ctx->curr_tok.value;
        eat_kind(ctx, TOKEN_IDENTIFIER);
        info_no_newline("..looking for %s", type_name);
        type = map_get(ctx->symbols, type_name);
        if (!type) {
            info("..didn't find it. Saved to unresolved types");
            type = make_type_unresolved(type_name);
            type->name = type_name;
            info("found unknown type %s. will resolve later", type_name);
        } else
            info("..Found it.");
        type->name = type_name;
    } else if (tok_is(ctx, TOKEN_OPEN_PAREN)) {
        eat_kind(ctx, TOKEN_OPEN_PAREN);
        List* args = make_list();
        bool has_var_args = false;
        bool has_multiple_arguments = false;
        while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
            if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);
            if (tok_is(ctx, TOKEN_DOT_DOT_DOT)) {
                eat(ctx);
                has_var_args = true;
            } else {
                AST* arg = parse_identifier(ctx);
                list_append(args, arg);
            }
            has_multiple_arguments = true;
        }
        eat_kind(ctx, TOKEN_CLOSE_PAREN);
        Type* ret_type = (tok_is_on_same_line(ctx) && !tok_is(ctx, TOKEN_NEWLINE)) ? get_type(ctx) : make_type_void();
        type = make_type_function(NULL, args, ret_type, has_var_args);
    }

    switch (ctx->curr_tok.kind) {
    case THI_SYNTAX_POINTER: {
        while (tok_is(ctx, THI_SYNTAX_POINTER)) {
            eat_kind(ctx, THI_SYNTAX_POINTER);
            type = make_type_pointer(type);
        }
    } break;
    case TOKEN_OPEN_BRACKET: {
        while (tok_is(ctx, TOKEN_OPEN_BRACKET)) {
            eat_kind(ctx, TOKEN_OPEN_BRACKET);
            s64 size = 0;
            if (tok_is(ctx, TOKEN_INTEGER) || tok_is(ctx, TOKEN_HEX)) {
                size = get_integer(ctx);
            }
            eat_kind(ctx, TOKEN_CLOSE_BRACKET);
            type = make_type_array(type, size);
        }
    } break;
    default: break;
    }

    assert(type);
    return type;
}
