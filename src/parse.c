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



static AST* parse_expr(Parser_Context* ctx);
static AST* parse_stmt(Parser_Context* ctx);
static AST* parse_literal(Parser_Context* ctx);
static List* parse_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind delimiter);


static List* parse_list(Parser_Context* ctx, AST* (*parseFunc)(Parser_Context*), Token_Kind delimiter) {
    DEBUG_START;
    List* nodes = make_list();
    while (tokIs(ctx, delimiter)) {
        eat(ctx); // eat delimitor
        AST* node = (*parseFunc)(ctx);
        xassert(node);
        list_append(nodes, node);
    }
    return nodes;
}


static AST* parse_literal(Parser_Context* ctx)
{
    DEBUG_START;

    // switch(kind(ctx))
    // {
    //     ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
    //     case TOKEN_INTEGER:  return parse_integer(ctx);
    //     case TOKEN_FLOAT:    return parse_float(ctx);
    //     case TOKEN_HEX:      return parse_hex(ctx);
    //     case TOKEN_STRING:   return parse_string(ctx);
    //     case TOKEN_CHAR:     return parse_char(ctx);
    // }

    UNFINISHED;
    return NULL;
}

static AST* parse_expr(Parser_Context* ctx)
{
    DEBUG_START;
    switch(kind(ctx))
    {
        ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
        case TOKEN_LITERAL:  return parse_literal(ctx);
    }

    UNFINISHED;
    return NULL;
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
        grouped = parse_stmt(ctx);

    match(ctx, TOKEN_CLOSE_PAREN);
    return make_ast_grouping(lc, grouped);
}

static AST* parse_return(Parser_Context* ctx)
{
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_RETURN);
    Loc_Info lc = loc(ctx);
    eat(ctx);
    AST* expr = parse_expr(ctx);
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
    AST* stmt = parse_stmt(ctx);
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

static AST* parse_stmt(Parser_Context* ctx) {
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
        AST* node = parse_stmt(&ctx);
        if (node) list_append(nodes, node);
    }

    list_foreach(nodes) debug("%s", ast_to_str(it->data));

    UNREACHABLE; // temp
    return NULL;
}
