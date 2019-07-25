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
#include "lex.h"       // Token, Token_Kind, lex, token_array_get_info_of
#include "map.h"       // Map
#include "type.h"      // Type, make_typspec_*,
#include "common.h"    // s32 , s64, etc.
#include "utility.h"   // info, error, warning, success, strf, get_file_content
#include <stdarg.h>    // va_list, va_start, va_end
#include <stdio.h>     // printf, vprintf
#include <stdlib.h>    // xmalloc
#include <string.h>    // strcmp

#define DEBUG_START \
    xassert(ctx); \
    debug("%s: %s", __func__, token_to_str(currTok(ctx)));

#define UNARY_OP_COUNT 11
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
    TOKEN_MINUS_MINUS,
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//              Each construct of the language gets its own function
//------------------------------------------------------------------------------

typedef enum {
    BLOCK_IMPLICITLY_RETURN_LAST_EXPR = 1 << 0
} Parser_Flags;

typedef struct {
    Token* tokens;
    char* file;
    char* dir;
    s64 lines;
    s64 comments;
    bool inside_parens;
    bool inside_asm;
    u32 flags;
    Map* symbols;
    List* loads;
    Token top_tok;
    Token curr_tok;
    Token prev_tok;
    AST* llast_if_statement; // used for dangling else
    AST* olast_if_statement; // used for dangling else
} Parser_Context;
Parser_Context make_parser_context(void);

AST* parse_statement                (Parser_Context* ctx);
AST* parse_primary                  (Parser_Context* ctx);
AST* parse_identifier               (Parser_Context* ctx);
AST* parse_expression_identifier    (Parser_Context* ctx);
AST* parse_def                      (Parser_Context* ctx);
AST* parse_variable_decl            (Parser_Context* ctx, Loc_Info lc, char* ident);
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
AST* parse_prefix                   (Parser_Context* ctx);
AST* parse_postfix                  (Parser_Context* ctx);
AST* parse_asm                      (Parser_Context* ctx);
AST* parse_cast                     (Parser_Context* ctx);

void skip_comments_or_newlines(Parser_Context* ctx);

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

AST* parse_enum_signature             (Parser_Context* ctx, Loc_Info lc, char* enum_name);
AST* parse_struct_signature           (Parser_Context* ctx, Loc_Info lc, char* struct_name);
Type* parse_extern_function_signature  (Parser_Context* ctx, char* func_name);
Type* get_type                         (Parser_Context* ctx);

s64 get_integer(Parser_Context* ctx);
f64 get_float(Parser_Context* ctx);
int get_tok_precedence(Parser_Context* ctx);
Token next_tok(Parser_Context* ctx);
Token_Kind next_tok_kind(Parser_Context* ctx);
Loc_Info loc(Parser_Context* ctx);
bool tok_is_on_same_line(Parser_Context* ctx);
bool next_tok_is_on_same_line(Parser_Context* ctx);
bool tok_is(Parser_Context* ctx, Token_Kind kind);
void eat(Parser_Context* ctx);
void eat_kind(Parser_Context* ctx, Token_Kind kind);
void set_if_statement(Parser_Context* ctx, AST* if_statement);
void restore_if_statement(Parser_Context* ctx);
void set_dangling_else(Parser_Context* ctx, AST* else_block);

Token_Kind tokKind(Parser_Context* ctx);
char* tokValue(Parser_Context* ctx);
Token currTok(Parser_Context* ctx);
Token prevTok(Parser_Context* ctx);

List* parse_terminated_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind delimiter);
List* parse_delimited_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind delimiter);

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

AST* _parse(Parser_Context* ctx, char* file);

Parsed_File parse(char* file) {
    push_timer(strf("%s: %s", (char*)__func__, file));
    Map* symbols = make_map();

    map_set(symbols, "void", make_type_void());
    map_set(symbols, "bool", make_type_int(1, TYPE_IS_UNSIGNED));
    map_set(symbols, "char", make_type_int(1, TYPE_IS_UNSIGNED));
    map_set(symbols, "int", make_type_int(DEFAULT_INT_BYTE_SIZE, false));
    map_set(symbols, "float", make_type_float(DEFAULT_FLOAT_BYTE_SIZE));
    map_set(symbols, "double", make_type_float(8));

    map_set(symbols, "s8", make_type_int(1, TYPE_IS_SIGNED));
    map_set(symbols, "s16", make_type_int(2, TYPE_IS_SIGNED));
    map_set(symbols, "s32", make_type_int(4, TYPE_IS_SIGNED));
    map_set(symbols, "s64", make_type_int(8, TYPE_IS_SIGNED));

    map_set(symbols, "u8", make_type_int(1, TYPE_IS_UNSIGNED));
    map_set(symbols, "u16", make_type_int(2, TYPE_IS_UNSIGNED));
    map_set(symbols, "u32", make_type_int(4, TYPE_IS_UNSIGNED));
    map_set(symbols, "u64", make_type_int(8, TYPE_IS_UNSIGNED));

    map_set(symbols, "f32", make_type_float(4));
    map_set(symbols, "f64", make_type_float(8));

     // Parse
    Parser_Context pctx = make_parser_context();
    pctx.file = file;
    pctx.symbols = symbols;
    AST* ast = _parse(&pctx, file);

    pop_timer();

    return (Parsed_File){ast, pctx.symbols, pctx.lines, pctx.comments};
}


AST* _parse(Parser_Context* ctx, char* file) {
    push_timer(strf("%s: %s", (char*)__func__, file));

    debug("Parsing file %s", file);


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

    ctx->file = file;

    // Add it to the list of loaded files.
    list_append(ctx->loads, ctx->file);

    Lexed_File lf = lex(ctx->file);
    ctx->tokens = lf.tokens.data;
    ctx->lines += lf.lines;
    ctx->comments += lf.comments;

    debug("%s",  get_colored_minimap_of_file(ctx->file, '_'));
    eat(ctx); // prime the first token so loc() gets the right line and col
    Loc_Info lc = loc(ctx);
    List* top_level_ast = parse_terminated_list(ctx, parse_statement, TOKEN_EOF);
    AST* ast = make_ast_module(lc, ctx->file, top_level_ast);


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

    pop_timer();
    return ast;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

AST* parse_statement(Parser_Context* ctx) {
    DEBUG_START;
    ctx->top_tok = ctx->curr_tok;

    AST* result = NULL;
    switch (tokKind(ctx)) {
    default:               result =  parse_expression(ctx);    break;
    case TOKEN_EOF:        eat(ctx);                           break;
    case TOKEN_SEMICOLON: {
        eat(ctx);
        result = make_ast_nop(loc(ctx));
        break;
    } break;
    case TOKEN_NEWLINE: {
        eat(ctx);
    } break;
    case TOKEN_COMMENT:
    {
        result = make_ast_comment(loc(ctx), tokValue(ctx)); 
        eat(ctx); 
    } break;

    case TOKEN_EQ_GT:       result =  parse_block(ctx);         break;

    // @Audit, @Buggy, @DoesntMakeSense
    case TOKEN_IDENTIFIER:  result = parse_expression(ctx);     break;

    case TOKEN_DEF:         result =  parse_def(ctx);           break;
    case TOKEN_ASM:         result =  parse_asm(ctx);           break;
    case TOKEN_IF:          result =  parse_if(ctx);            break;
    case TOKEN_IS:          result =  parse_is(ctx);            break;
    case TOKEN_ELSE:        result =  parse_dangling_else(ctx); break;
    case TOKEN_DEFER:       result =  parse_defer(ctx);         break;
    case TOKEN_FOR:         result =  parse_for(ctx);           break;
    case TOKEN_WHILE:       result =  parse_while(ctx);         break;
    case TOKEN_BLOCK_START: result =  parse_block(ctx);         break;
    case TOKEN_RETURN:      result =  parse_return(ctx);        break;
    case TOKEN_BREAK:       result =  parse_break(ctx);         break;
    case TOKEN_CONTINUE:    result =  parse_continue(ctx);      break;
    case TOKEN_FALLTHROUGH: result =  parse_fallthrough(ctx);   break;
    case TOKEN_LOAD:        result =  parse_load(ctx);          break;
    case TOKEN_LINK:        result =  parse_link(ctx);          break;
    case TOKEN_EXTERN:      result =  parse_extern(ctx);        break;
    }

    // If we've parsed a statement, the next terminal is extranous.
    if (result && tok_is(ctx, TOKEN_SEMICOLON)) eat(ctx);

    return result;
}

AST* parse_primary(Parser_Context* ctx) {
    DEBUG_START;
    AST* result = NULL;
    switch (tokKind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(tokKind(ctx));
    case TOKEN_COMMENT: {
        result = make_ast_comment(loc(ctx), tokValue(ctx)); 
        eat(ctx); 
    } break;

    // @Audit: Should a primary expression really eat terminal tokens? I belive only the parse_statement should handle that.
    case TOKEN_SEMICOLON:   break; // fallthrough
    case TOKEN_DOT_DOT_DOT: result = make_ast_var_args(loc(ctx)); eat(ctx); break;
    case TOKEN_TRUE:        result = make_ast_int(loc(ctx), 1, make_type_int(1, TYPE_IS_SIGNED)); eat(ctx); break;
    case TOKEN_FALSE:       result = make_ast_int(loc(ctx), 0, make_type_int(1, TYPE_IS_SIGNED)); eat(ctx); break;
    case TOKEN_IDENTIFIER:  result = parse_identifier(ctx); break;
    case TOKEN_DOLLAR_SIGN: result = parse_note(ctx); break;
    case TOKEN_ASM:         result = parse_asm(ctx); break;
    case TOKEN_FLOAT:       result = parse_float(ctx); break;
    case TOKEN_CHAR:        result = parse_char(ctx); break;
    case TOKEN_HEX:         // fallthrough
    case TOKEN_INTEGER:     result = parse_integer(ctx); break;
    case TOKEN_STRING:      result = parse_string(ctx); break;

    case TOKEN_OPEN_PAREN:  result = parse_parens(ctx); break;
    case TOKEN_CLOSE_PAREN: break;

    case TOKEN_BLOCK_START: result = parse_block(ctx); break;
    case TOKEN_CAST: result = parse_cast(ctx); break;
    }

    // Eat extranous terminals
    // if (tok_is(ctx, TOKEN_SEMICOLON)) eat(ctx);

    return result;
}

AST* parse_identifier(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    switch (tokKind(ctx)) {
    case TOKEN_IDENTIFIER:  return parse_variable_decl(ctx, lc, ident);
    case TOKEN_OPEN_PAREN:  return parse_function_call(ctx, lc, ident);
    default:                return make_ast_ident(lc, ident);
    }
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
    if (!ext) {
        file = strf("%s.thi", file);
    } else if (strcmp(ext, ".thi") != 0) {
        Loc_Info la = loc(ctx);
        error("[%s:%s:%s] '%s' is not a .thi file.", ctx->file, la.line, la.col, file);
    }
    eat_kind(ctx, TOKEN_STRING);
    
    char* last_dir = get_file_directory(ctx->file);
    char* filepath = strf("%s%s", last_dir ? last_dir : "", file);
    AST* module = _parse(ctx, filepath);
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

        eat(ctx); // eat 'in'

        AST* start = parse_expression(ctx);
        AST* end = NULL;
        if (tok_is(ctx, TOKEN_DOT_DOT)) {
            // We are in a 'for call in 0 .. calls' type of 
            eat(ctx); // eat '..'
            end = parse_expression(ctx);
        } else {
            // We are in a 'for call in calls' type of 
            // for-in. 
            // Which is only supported if 'start' is of 
            // array type.
            // which we do not yet support
            error("we do not yet support for-in with array types.");
        }

        AST* it = make_ast_ident(loc(ctx), init->Ident.name);
        init = make_ast_binary(loc(ctx), TOKEN_EQ, it, start);
        cond = make_ast_binary(loc(ctx), TOKEN_LT, it, end);
        step = make_ast_unary(loc(ctx), TOKEN_PLUS_PLUS, it);
        then_block = parse_block(ctx);

        // Place the 'param' variable at the start of the block
        // list_prepend(then_block->Block.stmts, init);
    } else {
        cond = parse_statement(ctx);
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
    list_foreach(body->Block.stmts) {
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
    AST* else_block = parse_statement(ctx);
    set_dangling_else(ctx, else_block);
    return NULL;
}

void maybe_convert_if_to_switch(AST* node) {

    AST* then_block = node->If.then_block;

    // check for any AST_IS inside
    List* stmts = then_block->Block.stmts;
    bool if_statement_is_actually_a_switch = false;

    list_foreach(stmts) {
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
        list_foreach(stmts) {
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

    u32 flags = 0;
    Token_Kind delimitor = TOKEN_BLOCK_END;

    skip_comments_or_newlines(ctx);

    // Check for single line implicit returned 
    // ex. def my_func() => 1 

    if (tok_is(ctx, TOKEN_EQ_GT) && tok_is_on_same_line(ctx)) {
        flags |= BLOCK_LAST_EXPR_IS_IMPLICITLY_RETURNED;
        delimitor = TOKEN_NEWLINE;
        eat(ctx);
    } else {
        eat_kind(ctx, TOKEN_BLOCK_START);
    }
    List* stmts = parse_terminated_list(ctx, parse_statement, delimitor);
    eat(ctx);
    AST* block = make_ast_block(lc, stmts);
    
    // @Audit @Cleanup @Ugly
    if (flags & BLOCK_LAST_EXPR_IS_IMPLICITLY_RETURNED) {
        AST* last_stmt = stmts->tail->data;
        stmts->tail->data = make_ast_return(last_stmt->loc_info, last_stmt);
    }

    block->Block.flags = flags;
    xassert(block);
    restore_if_statement(ctx);
    return block;
}

AST* parse_return(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat_kind(ctx, TOKEN_RETURN);
    AST* expr = tok_is_on_same_line(ctx) ? parse_expression(ctx) : make_ast_nop(lc);
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

    xassert(tokKind(ctx) == TOKEN_OPEN_PAREN);
    eat(ctx);

    List* params_t = make_list();
    List* params = make_list();
    bool has_multiple_arguments = false;
    u32 flags = 0;

    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);
        AST* param = parse_expression(ctx);

        Type_Name_Pair* tp = xmalloc(sizeof(*tp));
        tp->name = get_ast_name(param);
        tp->type = param->type;

        list_append(params_t, tp);
        list_append(params, param);
        has_multiple_arguments = true;
    }

    eat_kind(ctx, TOKEN_CLOSE_PAREN);

    char* func_name = ident;

    Type* ret_type = (tok_is_on_same_line(ctx) && !tok_is(ctx, TOKEN_NEWLINE) && !tok_is(ctx, TOKEN_BLOCK_START)) ? get_type(ctx) : make_type_void();
    Type* func_type = make_type_function(func_name, params_t, ret_type, flags);

    AST* func_body = parse_block(ctx);

    return make_ast_function(lc, func_name, params, func_type, func_body);
}

AST* parse_def(Parser_Context* ctx) {
    DEBUG_START;

    Loc_Info lc = loc(ctx);

    xassert(tokKind(ctx) == TOKEN_DEF);
    eat(ctx);

    char* ident = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);

    switch(tokKind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(tokKind(ctx));
    case TOKEN_STRUCT:     return parse_struct_signature(ctx, lc, ident);
    case TOKEN_ENUM:       return parse_enum_signature(ctx, lc, ident);
    case TOKEN_OPEN_PAREN: return parse_function_decl(ctx, lc, ident);
    }

    UNREACHABLE;
    return NULL;
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
    // debug("missing loc_info for binary nodes");

    AST* expr = NULL;

    // If this is a binop, find its precedence.
    while(true) {
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
    xassert(tok_is(ctx, TOKEN_AS));
    eat(ctx);
    AST* type_expr = parse_expression(ctx);
    return make_ast_as(lc, expr, type_expr);
}

AST* read_subscript_expr(Parser_Context* ctx, AST* expr) {
    Loc_Info lc = loc(ctx);
    xassert(tok_is(ctx, TOKEN_OPEN_BRACKET));
    eat(ctx);
    AST* sub = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_BRACKET);
    sub = make_ast_subscript(lc, expr, sub);
    return sub;
}

AST* read_field_access(Parser_Context* ctx, AST* expr) {
    Loc_Info lc = loc(ctx);
    xassert(tok_is(ctx, TOKEN_DOT));
    eat(ctx);
    char* field_name = tokValue(ctx);
    eat_kind(ctx, TOKEN_IDENTIFIER);
    AST* field = make_ast_field_access(lc, expr, field_name);
    return field;
}

AST* parse_postfix_tail(Parser_Context* ctx, AST* primary_expr) {
    DEBUG_START;
    while(true) {
        switch(tokKind(ctx)) {
        default: return primary_expr;
        // @Audit
        // @Bug: this is causing endless loop in basic_setup.thi
        case TOKEN_MINUS_MINUS: // fallthrough
        case TOKEN_PLUS_PLUS: {
            Token_Kind op = tokKind(ctx);
            eat(ctx);
            primary_expr = make_ast_post_inc_or_dec(primary_expr->loc_info, op, primary_expr);
            break;
        }
        // case TOKEN_COMMA: {
        //     List* exprs = parse_delimited_list(ctx, parse_expression, TOKEN_COMMA);
        //     parse_
        //     primary_expr = make_ast_comma_separated_list(primary_expr->loc_info, parse_delimited_list(ctx, parse_expression, TOKEN_COMMA));
        //     warning("%s", ast_to_str(primary_expr));
        // } break;
        case TOKEN_AS:
            primary_expr = read_as(ctx, primary_expr);
            break;
        case TOKEN_OPEN_BRACKET:
            primary_expr = read_subscript_expr(ctx, primary_expr);
            break;
        case TOKEN_DOT:
            primary_expr = read_field_access(ctx, primary_expr);
            break;
        }
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

    foreach(i, UNARY_OP_COUNT) {
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
        switch(unary->Unary.op) {
        default: break;
        case TOKEN_SIZEOF:
            ast_replace(unary, make_ast_sizeof(unary->loc_info, unary->Unary.operand));
            break;
        case TOKEN_TYPEOF: 
            ast_replace(unary, make_ast_typeof(unary->loc_info, unary->Unary.operand));
            break;
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
    
    switch (tokKind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(tokKind(ctx));
    case TOKEN_HEX:         expr = parse_integer(ctx); break;
    case TOKEN_INTEGER:     expr = parse_integer(ctx); break;
    }
    
    return make_ast_note(lc, expr);
}

AST* parse_expression(Parser_Context* ctx) {
    DEBUG_START;
    AST* lhs = parse_prefix(ctx);
    AST* expr = lhs ? parse_binary(ctx, 0, lhs) : NULL;
    return expr;
}

AST* parse_float(Parser_Context* ctx) {
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    f64 value = get_float(ctx);
    Type* type = make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
    // Check for a suffix
    // f32: f F
    // Suffix can be ex. 1.53f or 53.50FFFFFF
    if (tok_is(ctx, TOKEN_IDENTIFIER))
    {
        char* tok_val = tokValue(ctx);
        u64 i = 0;
        char c;
        while((c = tok_val[i]) == 'f' || c == 'F') {
            switch(c) {
            case 'f': // fallthrough
            case 'F': 
                type->Float.bytes = 4;
                break;
            }
            ++i;
        }
        // if i has been changed, we have gotten a suffix.
        // Eat it.c
        if (i != 0) eat(ctx);
        if (xstrlen(tok_val) != i) {
            error("[%s:%d:%d] unknown character '%c' in float suffix ", ctx->file, lc.line, lc.col, tok_val[i]);
        }
    }
    AST* res = make_ast_float(lc, value, type);
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
    s64 value = get_integer(ctx);
    Type* type = type = make_type_int(DEFAULT_INT_BYTE_SIZE, TYPE_IS_SIGNED);
    // Check for a suffix
    // u32: u U
    // s32: l L
    // s64: ll LL 
    // Suffix can be ex. 532llU
    if (tok_is(ctx, TOKEN_IDENTIFIER))
    {
        char* tok_val = tokValue(ctx);

        u64 i = 0;
        char c;
        while((c = tok_val[i]) == 'u' || c == 'U' || c == 'l' || c == 'L' ) {
            switch(c) {
            default: break;
            case 'u': // fallthrough
            case 'U': 
                type->Int.is_unsigned = true;
                break;
            // Might be 'll'
            case 'l': {
                if (tok_val[i+1] == 'l') {
                    type->Int.bytes = 8;
                    ++i;
                } else {
                    type->Int.bytes = 4;
                }
                break;
            }
            case 'L': {
                if (tok_val[i+1] == 'L') {
                    type->Int.bytes = 8;
                    ++i;
                } else {
                    type->Int.bytes = 4;
                }
                break;
            }
            }
            ++i;
        }
        // if i has been changed, we have gotten a suffix.
        // Eat it.c
        if (i != 0) eat(ctx);
        if (xstrlen(tok_val) != i) {
            error("[%s:%d:%d] unknown character '%c' in integer suffix ", ctx->file, lc.line, lc.col, tok_val[i]);
        }
    }
    AST* res = make_ast_int(lc, value, type);
    return res;
}

AST* parse_parens(Parser_Context* ctx) {
    DEBUG_START;
    xassert(tok_is(ctx, TOKEN_OPEN_PAREN));
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(lc, expr);
}

void eat_block_start(Parser_Context* ctx) {
    skip_comments_or_newlines(ctx);
    eat_kind(ctx, TOKEN_BLOCK_START);
}


// @Cleanup @Audit @Volatile @Ugly
AST* parse_asm(Parser_Context* ctx) {
    DEBUG_START;
 
    Loc_Info lc = loc(ctx);
 
    xassert(tok_is(ctx, TOKEN_ASM));
    eat(ctx);

    eat_block_start(ctx);
    
    List* stmts = make_list();
    string* line = make_string("");
    Loc_Info loc_of_line = loc(ctx);
    
    while(true) {
        if(tok_is(ctx, TOKEN_COMMENT)) continue;

        if (tok_is(ctx, TOKEN_NEWLINE)) {
            list_append(stmts, make_ast_string(loc_of_line, string_data(line)));
            loc_of_line = loc(ctx);
            line = make_string("");
        } else {
            debug("%s %s", token_kind_to_str(tokKind(ctx)), tokValue(ctx));
            string_append_f(line, "%s ", tokValue(ctx));
        }

        eat(ctx);
        if (tok_is(ctx, TOKEN_BLOCK_END)) break;
    }

    // for (; ctx->curr_tok.kind != TOKEN_BLOCK_END; eat(ctx)) {
    //     if(tok_is(ctx, TOKEN_COMMENT)) continue;
    //     if (tok_is(ctx, TOKEN_NEWLINE) && next_tok_kind(ctx) != TOKEN_BLOCK_END) {
    //         list_append(stmts, make_ast_string(loc_of_line, string_data(line)));
    //         loc_of_line = loc(ctx);
    //         line = make_string("");
    //     } else {
    //         debug("%s %s", token_kind_to_str(tokKind(ctx)), tokValue(ctx));
    //         string_append_f(line, "%s ", tokValue(ctx));
    //     }
    // }
    eat(ctx); // eat the block end
    AST* block = make_ast_block(lc, stmts);
    return make_ast_asm(lc, block);
}

AST* parse_cast(Parser_Context* ctx) {
    DEBUG_START;
    xassert(tok_is(ctx, TOKEN_CAST));
    Loc_Info lc = loc(ctx);
    eat(ctx);
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    AST* desired_type = parse_expression(ctx);
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    AST* operand = parse_expression(ctx);
    return make_ast_cast(lc, desired_type, operand);
}

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

AST* parse_enum_signature(Parser_Context* ctx, Loc_Info lc, char* enum_name) {
    DEBUG_START;
    xassert(tokKind(ctx) == TOKEN_ENUM);
    eat(ctx);
    AST* block = parse_block(ctx);
    List* members = block->Block.stmts;
    return make_ast_enum(lc, enum_name, members);
}

AST* parse_struct_signature(Parser_Context* ctx, Loc_Info lc, char* struct_name) {
    DEBUG_START;
    xassert(tokKind(ctx) == TOKEN_STRUCT);
    eat(ctx);
    AST* block = parse_block(ctx);
    List* members = block->Block.stmts;
    return make_ast_struct(lc, struct_name, members);
}

Type* parse_extern_function_signature(Parser_Context* ctx, char* func_name) {
    DEBUG_START;
    eat_kind(ctx, TOKEN_OPEN_PAREN);
    List* params = make_list();
    bool has_multiple_arguments = false;
    u32 flags = 0;
    while (!tok_is(ctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(ctx, TOKEN_COMMA);

        Type_Name_Pair* tp = xmalloc(sizeof(*tp));
        tp->name = NULL;
        tp->type = get_type(ctx);

        if (tp->type->kind == TYPE_VAR_ARGS) {
            flags |= TYPE_FLAG_HAS_VAR_ARG;
        }
        list_append(params, tp);
        has_multiple_arguments = true;
    }
    eat_kind(ctx, TOKEN_CLOSE_PAREN);
    Type* ret_type = (tok_is_on_same_line(ctx) && !tok_is(ctx, TOKEN_NEWLINE) && !tok_is(ctx, TOKEN_BLOCK_START)) ? get_type(ctx) : make_type_void();
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
        char* type_name = token_value(ctx->curr_tok);
        eat_kind(ctx, TOKEN_IDENTIFIER);
        // debug_no_newline("..looking for %s", type_name);
        type = map_get(ctx->symbols, type_name);
        if (!type) {
            // debug("..didn't find it. Saved to unresolved types");
            type = make_type_unresolved(type_name);
            type->name = type_name;
            // debug("found unknown type %s. will resolve later", type_name);
        } 
        // else debug("..Found it.");
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
        Type* ret_type = (tok_is_on_same_line(ctx) && !tok_is(ctx, TOKEN_NEWLINE) && !tok_is(ctx, TOKEN_BLOCK_START)) ? get_type(ctx) : make_type_void();
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

    xassert(type);
    return type;
}

#define BIN_OP_COUNT 35
struct
{
    Token_Kind kind;
    s32 p;
} binop_precedence[BIN_OP_COUNT] = {
    {TOKEN_OPEN_PAREN,    15}, // ()
    {TOKEN_OPEN_BRACKET,  15}, // []
    {TOKEN_DOT,           15}, // .
    {TOKEN_ASTERISK,      13}, // *
    {TOKEN_FWSLASH,       13}, // /
    {TOKEN_PERCENT,       13}, // %
    {TOKEN_PLUS,          12}, // +
    {TOKEN_MINUS,         12}, // -
    {TOKEN_LT_LT,         11}, // <<
    {TOKEN_GT_GT,         11}, // >>
    {TOKEN_LT,            10}, // <
    {TOKEN_LT_EQ,         10}, // <=
    {TOKEN_GT,            10}, // >
    {TOKEN_GT_EQ,         10}, // >=
    {TOKEN_EQ_EQ,         9},  // ==
    {TOKEN_BANG_EQ,       9},  // !=
    {TOKEN_AND,           8},  // &
    {TOKEN_HAT,           7},  // ^
    {TOKEN_PIPE,          6},  // |
    {TOKEN_AND_AND,       5},  // &&
    {TOKEN_PIPE_PIPE,     4},  // ||
    {TOKEN_QUESTION_MARK, 3},  // ?
    {TOKEN_COLON,         3},  // :
    {TOKEN_EQ,            2},  // =
    {TOKEN_PLUS_EQ,       2},  // +=
    {TOKEN_MINUS_EQ,      2},  // -=
    {TOKEN_ASTERISK_EQ,   2},  // *=
    {TOKEN_FWSLASH_EQ,    2},  // /=
    {TOKEN_PERCENT_EQ,    2},  // %=
    {TOKEN_AND_EQ,        2},  // &=
    {TOKEN_HAT_EQ,        2},  // ^=
    {TOKEN_PIPE_EQ,       2},  // |=
    {TOKEN_LT_LT_EQ,      2},  // <<=
    {TOKEN_GT_GT_EQ,      2},  // >>=
    // {TOKEN_COMMA,         1},  // ,
};

Parser_Context make_parser_context(void) {
    Parser_Context ctx;
    ctx.flags = 0;
    ctx.tokens = NULL;
    ctx.inside_parens = false;
    ctx.inside_asm = false;
    ctx.curr_tok.kind = TOKEN_UNKNOWN;
    ctx.llast_if_statement = NULL;
    ctx.olast_if_statement = NULL;
    ctx.loads = make_list();
    ctx.loads = make_list();
    ctx.symbols = NULL; // we borrow anothers map
    ctx.lines = 0; // we borrow anothers map
    ctx.comments = 0; // we borrow anothers map
    return ctx;
}

Token_Kind tokKind(Parser_Context* ctx) {
    return ctx->curr_tok.kind;
}
char* tokValue(Parser_Context* ctx) {
    return token_value(ctx->curr_tok);
}
Token currTok(Parser_Context* ctx) {
    return ctx->curr_tok;
}
Token prevTok(Parser_Context* ctx) {
    return ctx->prev_tok;
}

s64 get_integer(Parser_Context* ctx) {
    DEBUG_START;

    s64 value = 0;
    switch (ctx->curr_tok.kind) {
    ERROR_UNHANDLED_TOKEN_KIND(ctx->curr_tok.kind);
    case TOKEN_CHAR: {
        u8 c = token_value(ctx->curr_tok)[0];
        if (c == '\\') {
            u8 c = token_value(ctx->curr_tok)[1];
            switch (c) {
            case 'a':  value = 7;  break;
            case 'n':  value = 10; break;
            case 't':  value = 9;  break;
            case '\\': value = 92; break;
            case '\'': value = 27; break;
            case '"':  value = 22; break;
            }
        } else
            value = c;
        break;
    }
    case TOKEN_INTEGER: value = atoll(token_value(ctx->curr_tok)); break;
    case TOKEN_HEX: value = strtoll(token_value(ctx->curr_tok), NULL, 0); break;
    }
    eat(ctx);

    return value;
}

f64 get_float(Parser_Context* ctx) {
    DEBUG_START;
    f64 value = atof(token_value(ctx->curr_tok));
    eat_kind(ctx, TOKEN_FLOAT);
    return value;
}

int get_tok_precedence(Parser_Context* ctx) {
    DEBUG_START;
    foreach(i, BIN_OP_COUNT) 
        if (binop_precedence[i].kind == ctx->curr_tok.kind)
            return binop_precedence[i].p;
    return -1;
}

Token next_tok(Parser_Context* ctx) {
    if (!ctx->tokens) {
        Token token;
        token.kind = TOKEN_EOF;
        return token;
    }
    return (*ctx->tokens);
}

Token_Kind next_tok_kind(Parser_Context* ctx) {
    if (!ctx->tokens) return TOKEN_EOF;
    Token_Kind kind = (*ctx->tokens).kind;
    return kind;
}

bool tok_is_on_same_line(Parser_Context* ctx) {
    s64 l1 = ctx->curr_tok.line;
    s64 l2 = ctx->prev_tok.line;
    return l1 == l2;
}

bool next_tok_is_on_same_line(Parser_Context* ctx) {
    Token t2 = next_tok(ctx);
    s64 l1 = ctx->curr_tok.line;
    s64 l2 = t2.line;
    return l1 == l2;
}

bool tok_is(Parser_Context* ctx, Token_Kind kind) {
    return ctx->curr_tok.kind == kind;
}

void eat(Parser_Context* ctx) {
    ctx->prev_tok = ctx->curr_tok = *ctx->tokens++;
}

void eat_kind(Parser_Context* ctx, Token_Kind kind) {
    Token_Kind tk = ctx->curr_tok.kind;
    if (tk != kind) {
        error("[%s:%s:%s] expected '%s' got '%s'", give_unique_color(ctx->file), give_unique_color(strf("%d", ctx->curr_tok.line)), give_unique_color(strf("%d", ctx->curr_tok.col)), give_unique_color(token_kind_to_str(kind)), give_unique_color(tokValue(ctx)));
    }
    eat(ctx);
}

void set_if_statement(Parser_Context* ctx, AST* if_statement) {
    ctx->olast_if_statement = ctx->llast_if_statement;
    ctx->llast_if_statement = if_statement;
}

void restore_if_statement(Parser_Context* ctx) {
    ctx->llast_if_statement = ctx->olast_if_statement;
}

void set_dangling_else(Parser_Context* ctx, AST* else_block) {
    ctx->llast_if_statement->If.else_block = else_block;
}

Loc_Info loc(Parser_Context* ctx) {
    s64 line = ctx->curr_tok.line;
    s64 col = ctx->curr_tok.col;
    return (Loc_Info){line, col};
}


List* parse_terminated_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind terminator) {
    List* nodes = make_list();
    while (!tok_is(ctx, terminator)) {
        AST* node = (*parseFunc)(ctx);
        if (node) list_append(nodes, node);
    }
    return nodes;
}
List* parse_delimited_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind delimiter) {
    List* nodes = make_list();
    while (tok_is(ctx, delimiter)) {
        eat(ctx); // eat delimitor
        AST* node = (*parseFunc)(ctx);
        if (node) list_append(nodes, node);
    }
    return nodes;
}
