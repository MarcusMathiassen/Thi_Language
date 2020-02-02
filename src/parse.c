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
#include "stack.h" // Stack
#include "utility.h" // push_timer, pop_timer, get_file_content

#define DEBUG_START \
    xassert(ctx); \
    debug("%s: %s", __func__, token_kind_to_str(kind(ctx)));

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

typedef struct {
    Token* tokens;
    Token curr;
    Stack* stack;


    Unary_Kind

} Parser_Context;


static AST* parse_statement(Parser_Context* ctx);
static AST* parse_expression(Parser_Context* ctx);

static Token_Kind kind(Parser_Context* ctx) {
    return ctx->curr.kind;
}

static char* value(Parser_Context* ctx) {
    return token_value(ctx->curr);
}

static void eat(Parser_Context* ctx) {
    ctx->curr = *ctx->tokens++;
}

static bool tokIs(Parser_Context* ctx, Token_Kind tk) {
    return tk == kind(ctx);
}

static void match(Parser_Context* ctx, Token_Kind tk) {
    if (!tokIs(ctx, tk))
        error("expected '%s' got '%s'", token_kind_to_str(tk), value(ctx));
    eat(ctx);
}

static Loc_Info loc(Parser_Context* ctx) {
    s64 line = ctx->curr.line;
    s64 col = ctx->curr.col;
    return (Loc_Info){line, col};
}

AST* parse_prefix(Parser_Context* ctx) {
    DEBUG_START;
    return parse_unary(ctx);
}

AST* parse_postfix(Parser_Context* ctx) {
    DEBUG_START;
    AST* primary_expr = parse_primary(ctx);
    DEBUG_START;
    while(true) {
        Loc_Info lc = loc(ctx);
        switch(tokKind(ctx)) {
        default: return primary_expr;
        case TOKEN_MINUS_MINUS: // fallthrough
        case TOKEN_PLUS_PLUS: {
            Token_Kind op = tokKind(ctx);
            eat(ctx);
            primary_expr = make_ast_post_inc_or_dec(lc, op, primary_expr);
        } break;
        case TOKEN_COMMA: {
            List* exprs = parse_delimited_list(ctx, parse_expression, TOKEN_COMMA);
            list_prepend(exprs, primary_expr);
            primary_expr = make_ast_comma_separated_list(lc, exprs);
        } break;
        case TOKEN_AS: {
            eat(ctx);
            AST* type_expr = parse_expression(ctx);
            primary_expr = make_ast_as(lc, primary_expr, type_expr);
        } break;
        case TOKEN_OPEN_BRACKET: {
            eat(ctx);
            AST* sub = parse_expression(ctx);
            eat_kind(ctx, TOKEN_CLOSE_BRACKET);
            sub = make_ast_subscript(lc, primary_expr, sub);
            primary_expr = sub;
        } break;
        case TOKEN_DOT: {
            eat(ctx);
            char* field_name = tokValue(ctx);
            eat_kind(ctx, TOKEN_IDENTIFIER);
            AST* field = make_ast_field_access(lc, primary_expr, field_name);
            primary_expr = field;
        } break;
        }
    }
    UNREACHABLE;
    return NULL;
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

    return unary ? unary : parse_postfix(ctx);
}

static AST* parse_parens(Parser_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_OPEN_PAREN);
    Loc_Info lc = loc(ctx);
    eat(ctx);

    // there might not be anything after this parens
    AST* grouped = NULL;
    if (!tokIs(ctx, TOKEN_CLOSE_PAREN))
        grouped = parse_statement(ctx);

    match(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(lc, grouped);
}

static AST* parse_return(Parser_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_RETURN);
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expression(ctx);
    return make_ast_return(lc, expr);
}
static AST* parse_def(Parser_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_DEF);
    Loc_Info lc = loc(ctx);
    eat(ctx);
    char* name = value(ctx);
    match(ctx, TOKEN_IDENTIFIER);
    AST* stmt = parse_statement(ctx);
    return make_ast_def(lc, name, stmt);
}

static AST* parse_identifier(Parser_Context* ctx) {
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_IDENTIFIER);

    Loc_Info lc = loc(ctx);
    char* name = value(ctx);
    eat(ctx);

    switch (kind(ctx)) {
    default: return make_ast_ident(lc, name);        
    }

    UNREACHABLE;
    return NULL;
}
static AST* parse_identifier_expression(Parser_Context* ctx) {
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_IDENTIFIER);

    Loc_Info lc = loc(ctx);
    char* name = value(ctx);
    eat(ctx);

    switch (kind(ctx)) {
    default: return make_ast_ident(lc, name);        
    }

    UNREACHABLE;
    return NULL;
}

static AST* parse_statement(Parser_Context* ctx) {
    DEBUG_START;
    switch (kind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
    case TOKEN_IDENTIFIER: return parse_identifier(ctx);
    case TOKEN_DEF:        return parse_def(ctx);
    case TOKEN_OPEN_PAREN: return parse_parens(ctx);
    case TOKEN_RETURN:     return parse_return(ctx);
    }
    UNREACHABLE;
    return NULL;
}

AST* parse2(char* source) { // @Temp: name

    List* nodes = make_list();

    Parser_Context ctx;
    ctx.stack = make_stack();
    ctx.tokens = lex(source).tokens.data;
    
    eat(&ctx); // prime the first token
    
    while(kind(&ctx) != TOKEN_EOF) {
        AST* node = parse_statement(&ctx);
        if (node) list_append(nodes, node);
    }

    list_foreach(nodes) debug("%s", ast_to_str(it->data));

    UNREACHABLE; // temp
    return NULL;
}
