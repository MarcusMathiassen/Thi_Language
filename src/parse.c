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

typedef struct {
    Token* tokens;
    Token curr;
    Stack* stack;
} Parser_Context;


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

static AST* parse_expression(Parser_Context* ctx);
static AST* parse_primary(Parser_Context* ctx);
static AST* parse_statement(Parser_Context* ctx);

// Expressions
static AST* parse_literal   (Parser_Context* ctx);
static AST* parse_unary     (Parser_Context* ctx);
static AST* parse_binary    (Parser_Context* ctx);
static AST* parse_grouping  (Parser_Context* ctx);
static AST* parse_list_expr (Parser_Context* ctx, Token_Kind delimitor);
static AST* parse_list_stmt (Parser_Context* ctx, Token_Kind delimitor);
static AST* parse_call      (Parser_Context* ctx);
static AST* parse_ident     (Parser_Context* ctx);

// Statements
static AST* parse_def         (Parser_Context* ctx);
static AST* parse_return      (Parser_Context* ctx);
static AST* parse_if          (Parser_Context* ctx);
static AST* parse_for         (Parser_Context* ctx);
static AST* parse_while       (Parser_Context* ctx);
static AST* parse_else        (Parser_Context* ctx);
static AST* parse_load        (Parser_Context* ctx);
static AST* parse_link        (Parser_Context* ctx);
static AST* parse_fallthrough (Parser_Context* ctx);
static AST* parse_continue    (Parser_Context* ctx);
static AST* parse_break       (Parser_Context* ctx);
static AST* parse_enum        (Parser_Context* ctx);
static AST* parse_in          (Parser_Context* ctx);
static AST* parse_defer       (Parser_Context* ctx);
static AST* parse_is          (Parser_Context* ctx);

static AST* parse_expression(Parser_Context* ctx)
{
    DEBUG_START;
    switch(kind(ctx))
    {
        ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
        case TOKEN_LITERAL: return parse_literal(ctx);
        case TOKEN_TRUE:    { Loc_Info lc = loc(ctx); eat(ctx); return make_ast_literal_int(lc, 1); }
        case TOKEN_FALSE:   { Loc_Info lc = loc(ctx); eat(ctx); return make_ast_literal_int(lc, 0); }
        case TOKEN_OPEN_PAREN: return parse_grouping(ctx);
    }

    UNFINISHED;
    return NULL;
}
static AST* parse_primary(Parser_Context* ctx)
{
    DEBUG_START;
    switch(kind(ctx))
    {
        ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
        case TOKEN_LITERAL: return parse_literal(ctx);
        case TOKEN_TRUE:    { Loc_Info lc = loc(ctx); eat(ctx); return make_ast_literal_int(lc, 1); }
        case TOKEN_FALSE:   { Loc_Info lc = loc(ctx); eat(ctx); return make_ast_literal_int(lc, 0); }
        case TOKEN_OPEN_PAREN: return parse_grouping(ctx);
        case TOKEN_CLOSE_PAREN: return NULL; // if a grouping is empty, this will get called
    }

    UNREACHABLE;
    return NULL;
}
static AST* parse_statement(Parser_Context* ctx) {
    DEBUG_START;
    switch (kind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));

    // case TOKEN_IDENTIFIER:  return parse_variableDecl(ctx);

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
static AST* parse_literal(Parser_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    char* v = value(ctx);
    eat(ctx);
    return make_ast_literal(lc, v);
}
static AST* parse_unary(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_binary(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static AST* parse_grouping(Parser_Context* ctx)
{
    DEBUG_START;
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expression(ctx);
    match(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(lc, expr);
}
static AST* parse_list_expr(Parser_Context* ctx, Token_Kind delimitor)
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
static AST* parse_list_stmt(Parser_Context* ctx, Token_Kind delimitor)
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
static AST* parse_call      (Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_ident(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_def(Parser_Context* ctx)
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
static AST* parse_return(Parser_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_RETURN);
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expression(ctx);
    return make_ast_return(lc, expr);
}
static AST* parse_if(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_for(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_while(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_else(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_load(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_link(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_fallthrough(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_continue(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_break(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_enum(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_in(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_defer(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
static AST* parse_is(Parser_Context* ctx)
{
    DEBUG_START;
    UNFINISHED;
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
