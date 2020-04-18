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

#include "common.h"
#include "parse.h"
#include "lex.h"
#include "stack.h"
#include "map.h"
#include "utility.h" // push_timer, pop_timer, get_file_content

#define DEBUG_START \
    xassert(ctx); \
    debug("%s: %s", __func__, ucolor(token_kind_to_str(ctx->curr.kind)));


#define UNARY_OP_COUNT 11
static Token_Kind unary_ops[UNARY_OP_COUNT] = {
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

#define BIN_OP_COUNT 35
static struct { Token_Kind kind; s32 p; } binop_precedence[BIN_OP_COUNT] = {
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

typedef struct {
    Token* tokens;
    Token curr;
    Stack* stack;
} Parse_Context;

static Token_Kind kind(Parse_Context* ctx) {
    return ctx->curr.kind;
}

static char* value(Parse_Context* ctx) {
    return token_value(ctx->curr);
}

static void eat(Parse_Context* ctx) {
    ctx->curr = *ctx->tokens++;
}

static bool tokIs(Parse_Context* ctx, Token_Kind tk) {
    return tk == kind(ctx);
}

static void match(Parse_Context* ctx, Token_Kind tk) {
    if (!tokIs(ctx, tk))
        error("expected '%s' got '%s'", token_kind_to_str(tk), value(ctx));
    eat(ctx);
}

static Loc_Info loc(Parse_Context* ctx) {
    s64 line = ctx->curr.line;
    s64 col = ctx->curr.col;
    return (Loc_Info){line, col};
}

static s8 get_tok_precedence(Parse_Context* ctx)
{
    DEBUG_START;
    Token_Kind k = kind(ctx);
    foreach(i, BIN_OP_COUNT) 
        if (k == binop_precedence[i].kind)
            return binop_precedence[i].p;
    return -1;
}

static List* parse_terminated_list(Parse_Context* ctx, AST* (*parseFunc)(Parse_Context*), Token_Kind terminator) {
    List* nodes = make_list();
    while (!tokIs(ctx, terminator)) {
        AST* node = (*parseFunc)(ctx);
        if (node) list_append(nodes, node);
    }
    return nodes;
}
static List* parse_delimited_list(Parse_Context* ctx, AST* (*parseFunc)(Parse_Context*), Token_Kind delimiter) {
    List* nodes = make_list();
    while (tokIs(ctx, delimiter)) {
        eat(ctx); // eat delimitor
        AST* node = (*parseFunc)(ctx);
        if (node) list_append(nodes, node);
    }
    return nodes;
}


static AST* parse_expression(Parse_Context* ctx);
static AST* parse_primary(Parse_Context* ctx);
static AST* parse_statement(Parse_Context* ctx);
static AST* parse_postfix(Parse_Context* ctx);

// Expressions
static AST* parse_literal   (Parse_Context* ctx);
static AST* parse_unary     (Parse_Context* ctx);
static AST* parse_binary    (Parse_Context* ctx, s8 expr_prec, AST* lhs);
static AST* parse_grouping  (Parse_Context* ctx);
static AST* parse_list_expr (Parse_Context* ctx, Token_Kind delimitor);
static AST* parse_list_stmt (Parse_Context* ctx, Token_Kind delimitor);
static AST* parse_call      (Parse_Context* ctx);
static AST* parse_ident     (Parse_Context* ctx);


// Statements
static AST* parse_comment     (Parse_Context* ctx);
static AST* parse_block       (Parse_Context* ctx);
static AST* parse_def         (Parse_Context* ctx);
static AST* parse_return      (Parse_Context* ctx);
static AST* parse_if          (Parse_Context* ctx);
static AST* parse_for         (Parse_Context* ctx);
static AST* parse_while       (Parse_Context* ctx);
static AST* parse_else        (Parse_Context* ctx);
static AST* parse_load        (Parse_Context* ctx);
static AST* parse_link        (Parse_Context* ctx);
static AST* parse_fallthrough (Parse_Context* ctx);
static AST* parse_continue    (Parse_Context* ctx);
static AST* parse_break       (Parse_Context* ctx);
static AST* parse_enum        (Parse_Context* ctx);
static AST* parse_in          (Parse_Context* ctx);
static AST* parse_defer       (Parse_Context* ctx);
static AST* parse_is          (Parse_Context* ctx);

static AST* parse_postfix(Parse_Context* ctx)
{
    DEBUG_START;
    AST* primary_expr = parse_primary(ctx);
    while(true) {
        Loc_Info lc = loc(ctx);
        switch(kind(ctx)) {
        default: return primary_expr;
        // case TOKEN_MINUS_MINUS: // fallthrough
        // case TOKEN_PLUS_PLUS: {
        //     Token_Kind op = tokKind(ctx);
        //     eat(ctx);
        //     primary_expr = make_ast_post_inc_or_dec(lc, op, primary_expr);
        // } break;
        case TOKEN_COMMA: {
            List* exprs = parse_delimited_list(ctx, parse_expression, TOKEN_COMMA);
            list_prepend(exprs, primary_expr);
            primary_expr = make_ast_comma_separated_list(lc, exprs);
        } break;
        // case TOKEN_AS: {
        //     eat(ctx);
        //     AST* type_expr = parse_expression(ctx);
        //     primary_expr = make_ast_as(lc, primary_expr, type_expr);
        // } break;
        // case TOKEN_OPEN_BRACKET: {
        //     eat(ctx);
        //     AST* sub = parse_expression(ctx);
        //     eat_kind(ctx, TOKEN_CLOSE_BRACKET);
        //     sub = make_ast_subscript(lc, primary_expr, sub);
        //     primary_expr = sub;
        // } break;
        // case TOKEN_DOT: {
        //     eat(ctx);
        //     char* field_name = tokValue(ctx);
        //     eat_kind(ctx, TOKEN_IDENTIFIER);
        //     AST* field = make_ast_field_access(lc, primary_expr, field_name);
        //     primary_expr = field;
        // } break;
        }
    }
    UNREACHABLE;
    return NULL;
}
static AST* parse_expression(Parse_Context* ctx)
{
    DEBUG_START;
    AST* lhs = parse_unary(ctx);
    AST* expr = lhs ? parse_binary(ctx, 0, lhs) : NULL;
    return expr;
}
static AST* parse_primary(Parse_Context* ctx)
{
    DEBUG_START;
    switch(kind(ctx))
    {
        ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
        case TOKEN_COMMENT:    return parse_comment(ctx);
        case TOKEN_IDENTIFIER: return parse_ident(ctx);
        case TOKEN_LITERAL: return parse_literal(ctx);
        case TOKEN_TRUE:    { Loc_Info lc = loc(ctx); eat(ctx); return make_ast_literal_int(lc, 1); }
        case TOKEN_FALSE:   { Loc_Info lc = loc(ctx); eat(ctx); return make_ast_literal_int(lc, 0); }
        case TOKEN_OPEN_PAREN: return parse_grouping(ctx);
        case TOKEN_CLOSE_PAREN: return NULL; // if a grouping is empty, this will get called
    }

    UNREACHABLE;
    return NULL;
}
static AST* parse_statement(Parse_Context* ctx) {
    DEBUG_START;
    switch (kind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));

    case TOKEN_IDENTIFIER:  return parse_expression(ctx);

    case TOKEN_OPEN_BRACE:  // fallthrough
    case TOKEN_BLOCK_START: return parse_block(ctx);
    case TOKEN_COMMENT:     return parse_comment(ctx);

    case TOKEN_DEF:         return parse_def(ctx);
    case TOKEN_RETURN:      return parse_return(ctx);
    case TOKEN_IF:          return parse_if(ctx);
    case TOKEN_FOR:         return parse_for(ctx);
    case TOKEN_WHILE:       return parse_while(ctx);
    case TOKEN_ELSE:        return parse_else(ctx);
    case TOKEN_LOAD:        return parse_load(ctx);
    case TOKEN_LINK:        return parse_link(ctx);
    case TOKEN_FALLTHROUGH: return parse_fallthrough(ctx);
    case TOKEN_CONTINUE:    return parse_continue(ctx);
    case TOKEN_BREAK:       return parse_break(ctx);
    case TOKEN_ENUM:        return parse_enum(ctx);
    case TOKEN_IN:          return parse_in(ctx);
    case TOKEN_DEFER:       return parse_defer(ctx);
    case TOKEN_IS:          return parse_is(ctx);

    }
    UNREACHABLE;
    return NULL;
}
static AST* parse_comment(Parse_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    char* text = value(ctx);
    eat(ctx);
    return make_ast_comment(lc, text);
}
static AST* parse_block(Parse_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat(ctx);
    List* stmts = parse_terminated_list(ctx, parse_statement, TOKEN_BLOCK_END);
    eat(ctx);
    return make_ast_block(lc, stmts);
}
static AST* parse_literal(Parse_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    char* v = value(ctx);
    eat(ctx);
    return make_ast_literal(lc, v);
}
static AST* parse_unary(Parse_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    AST* unary = NULL;
    Token_Kind op = kind(ctx);
    foreach(i, UNARY_OP_COUNT) {
        if (op == unary_ops[i])
        {
            eat(ctx);
            AST* operand = parse_unary(ctx);
            if (operand)
                unary = make_ast_unary(lc, op, operand);
            break;
        }
    }
    return unary ? unary : parse_postfix(ctx);
}
static AST* parse_binary(Parse_Context* ctx, s8 expr_prec, AST* lhs)
{
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
        Token_Kind binary_op_token = kind(ctx);
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

static AST* parse_grouping(Parse_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expression(ctx);
    match(ctx, TOKEN_CLOSE_PAREN);

    // Is it a function signature?
    if (kind(ctx) == TOKEN_MINUS_GT) {
        eat(ctx);
        Loc_Info lcc = loc(ctx);
        AST* params = expr;
        AST* type_expr = parse_expression(ctx);
        AST* stmt = parse_statement(ctx);
        return make_ast_lambda(lc, params, type_expr);
    }

    return make_ast_grouping(lc, expr);
}
static AST* parse_list_expr(Parse_Context* ctx, Token_Kind delimitor)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    List* nodes = make_list();
    while (tokIs(ctx, delimitor)) {
        eat(ctx); // eat delimitor
        AST* node = parse_expression(ctx);
        xassert(node);
        list_append(nodes, node);
    }
    return make_ast_list(lc, LIST_EXPR, delimitor, nodes);
}
static AST* parse_list_stmt(Parse_Context* ctx, Token_Kind delimitor)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    List* nodes = make_list();
    while (tokIs(ctx, delimitor)) {
        eat(ctx); // eat delimitor
        AST* node = parse_statement(ctx);
        xassert(node);
        list_append(nodes, node);
    }
    return make_ast_list(lc, LIST_STMT, delimitor, nodes);
}
static AST* parse_call      (Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_ident(Parse_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_IDENTIFIER);
    Loc_Info lc = loc(ctx);

    char* name = value(ctx);
    eat(ctx);

    switch(kind(ctx))
    {
        case TOKEN_COLON:
        {
            eat(ctx);
            AST* type_expr = parse_expression(ctx);
            char* type_name = value(ctx);
            Type* type = make_type_unresolved(type_name);
            return make_ast_variable_decl(lc, name, type, NULL);
        } break;
        case TOKEN_COLON_EQ:
        {
            eat(ctx);
            AST* val = parse_expression(ctx);
            return make_ast_variable_decl(lc, name, NULL, val);
        } break;
        case TOKEN_OPEN_PAREN:
        {
            eat(ctx);
            List* args = parse_terminated_list(ctx, parse_expression, TOKEN_CLOSE_PAREN);
            eat(ctx);
            return make_ast_call(lc, name, args);
        } break;
        default: return make_ast_ident(lc, name);
    }

    UNREACHABLE;
    return NULL;
}
static AST* parse_def(Parse_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_DEF);
    Loc_Info lc = loc(ctx);
    eat(ctx);
    char* name = value(ctx);
    match(ctx, TOKEN_IDENTIFIER);
    AST* expr = parse_expression(ctx);
    return make_ast_def(lc, name, expr);
}
static AST* parse_return(Parse_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_RETURN);
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expression(ctx);
    return make_ast_return(lc, expr);
}
static AST* parse_if(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_for(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_while(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_else(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_load(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_link(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_fallthrough(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_continue(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_break(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_enum(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_in(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_defer(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_is(Parse_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

AST* parse_module(char* path)
{
    char* source = get_file_content(path);
    char* module_name = get_file_name(path);
    Lexed_File lf = lex(source);
    Parse_Context ctx;
    ctx.tokens = lf.tokens.data;
    ctx.stack = make_stack();
    eat(&ctx); // prime the first token
    Loc_Info lc = loc(&ctx);
    List* top_level = parse_terminated_list(&ctx, parse_statement, TOKEN_EOF);
    AST* module = make_ast_module(lc,  module_name, top_level);
    return module;
}
