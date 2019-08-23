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

static AST* parse_function_or_call(Parser_Context* ctx) {
    // DEBUG_START;
    // xassert(kind(ctx) == TOKEN_OPEN_PAREN);
    // eat(ctx);
    // AST* expr = parse_expression(ctx);
    // match(ctx, TOKEN_CLOSE_PAREN);
    return NULL;
}

static AST* parse_identifier(Parser_Context* ctx) {
    DEBUG_START;
    xassert(kind(ctx) == TOKEN_IDENTIFIER);

    Loc_Info lc = loc(ctx);
    char* name = value(ctx);
    eat(ctx);

    switch (kind(ctx)) {
    case TOKEN_OPEN_PAREN: return parse_function_or_call(ctx);
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
