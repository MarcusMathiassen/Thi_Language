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
#include "utility.h" // push_timer, pop_timer, get_file_content

typedef struct {
    Token* tokens;
    Token curr;
} ParserContext;

static Token_Kind kind(ParserContext* ctx) {
    return ctx->curr.kind;
}

static char* value(ParserContext* ctx) {
    return token_value(ctx->curr);
}

static void eat(ParserContext* ctx) {
    ctx->curr = *ctx->tokens++;
}

static bool tokIs(ParserContext* ctx, Token_Kind tk) {
    return tk == kind(ctx);
}

static void match(ParserContext* ctx, Token_Kind tk) {
    if (!tokIs(ctx, tk))
        error("expected '%s' got '%s'", token_kind_to_str(tk), value(ctx));
    eat(ctx);
}

static Loc_Info loc(ParserContext* ctx) {
    s64 line = ctx->curr.line;
    s64 col = ctx->curr.col;
    return (Loc_Info){line, col};
}


// ident ident 
// ident
static AST* parseParens(ParserContext* ctx) {
    xassert(tokIs(ctx, TOKEN_OPEN_PAREN));
    AST* result = NULL;
    eat(ctx);
    match(ctx, TOKEN_CLOSE_PAREN);
    return result;
}
static AST* parseIdentifier(ParserContext* ctx) {
    AST* result = NULL;
    switch(kind(ctx)) {
    ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
    case TOKEN_OPEN_PAREN: result = parseParens(ctx); break;
    }
    return result;
}

static AST* parseStatement(ParserContext* ctx) {
    switch(kind(ctx)) {
	ERROR_UNHANDLED_TOKEN_KIND(kind(ctx));
    case TOKEN_IDENTIFIER: return parseIdentifier(ctx);
    }
    UNREACHABLE;
    return NULL;
}

AST* parse2(char* source) { // @Temp: name
    
    ParserContext ctx;
    ctx.tokens = lex(source).tokens.data;

    eat(&ctx); // prime the first token. DO NOT REMOVE THIS.

    List* statements = make_list();
    while (kind(&ctx) != TOKEN_EOF) {
        AST* statement = parseStatement(&ctx);
        xassert(statement);
        list_append(statements, statement);
    }

    AST* ast = NULL;
    return ast;
}
