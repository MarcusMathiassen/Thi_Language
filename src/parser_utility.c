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

#include "parser_utility.h"
#include "utility.h"
#include <assert.h>
#include <ctype.h>  // atoll
#include <stdlib.h> // xmalloc

#define DEBUG_START \
    assert(ctx);    \
    // info("%s: %s", __func__, token_to_str(ctx->curr_tok));

#define BIN_OP_COUNT 35
struct
{
    Token_Kind kind;
    s32        p;
} binop_precedence[BIN_OP_COUNT] = {
    {TOKEN_OPEN_PAREN, 15},   // ()
    {TOKEN_OPEN_BRACKET, 15}, // []
    {TOKEN_DOT, 15},          // .
    {TOKEN_ASTERISK, 13},     // *
    {TOKEN_FWSLASH, 13},      // /
    {TOKEN_PERCENT, 13},      // %
    {TOKEN_PLUS, 12},         // +
    {TOKEN_MINUS, 12},        // -
    {TOKEN_LT_LT, 11},        // <<
    {TOKEN_GT_GT, 11},        // >>
    {TOKEN_LT, 10},           // <
    {TOKEN_LT_EQ, 10},        // <=
    {TOKEN_GT, 10},           // >
    {TOKEN_GT_EQ, 10},        // >=
    {TOKEN_EQ_EQ, 9},         // ==
    {TOKEN_BANG_EQ, 9},       // !=
    {TOKEN_AND, 8},           // &
    {TOKEN_HAT, 7},           // ^
    {TOKEN_PIPE, 6},          // |
    {TOKEN_AND_AND, 5},       // &&
    {TOKEN_PIPE_PIPE, 4},     // ||
    {TOKEN_QUESTION_MARK, 3}, // ?
    {TOKEN_COLON, 3},         // :
    {TOKEN_EQ, 2},            // =
    {TOKEN_PLUS_EQ, 2},       // +=
    {TOKEN_MINUS_EQ, 2},      // -=
    {TOKEN_ASTERISK_EQ, 2},   // *=
    {TOKEN_FWSLASH_EQ, 2},    // /=
    {TOKEN_PERCENT_EQ, 2},    // %=
    {TOKEN_AND_EQ, 2},        // &=
    {TOKEN_HAT_EQ, 2},        // ^=
    {TOKEN_PIPE_EQ, 2},       // |=
    {TOKEN_LT_LT_EQ, 2},      // <<=
    {TOKEN_GT_GT_EQ, 2},      // >>=
    // {TOKEN_COMMA, 1},          // ,
};

Parser_Context
make_parser_context(void) {
    Parser_Context ctx;
    ctx.tokens             = NULL;
    ctx.curr_tok.kind      = TOKEN_UNKNOWN;
    ctx.llast_if_statement = NULL;
    ctx.olast_if_statement = NULL;
    ctx.loads              = make_list();
    ctx.symbols            = NULL; // we borrow anothers map
    return ctx;
}

Token_Kind tokKind(Parser_Context* ctx) {
    return ctx->curr_tok.kind;
}
char* tokValue(Parser_Context* ctx) {
    return ctx->curr_tok.value;
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
    case TOKEN_CHAR: {
        u8 c = ctx->curr_tok.value[0];
        if (c == '\\') {
            u8 c = ctx->curr_tok.value[1];
            switch (c) {
            case 'a': value = 7; break;
            case 'n': value = 10; break;
            case 't': value = 9; break;
            case '\\': value = 92; break;
            case '\'': value = 27; break;
            case '"': value = 22; break;
            }
        } else
            value = c;
        break;
    }
    case TOKEN_INTEGER: value = atoll(ctx->curr_tok.value); break;
    case TOKEN_HEX: value = strtoll(ctx->curr_tok.value, NULL, 0); break;
    default: ERROR_UNHANDLED_KIND(token_kind_to_str(ctx->curr_tok.kind));
    }
    eat(ctx);

    return value;
}

f64 get_float(Parser_Context* ctx) {
    DEBUG_START;
    f64 value = atof(ctx->curr_tok.value);
    eat_kind(ctx, TOKEN_FLOAT);
    return value;
}

int get_tok_precedence(Parser_Context* ctx) {
    DEBUG_START;
    for (int i = 0; i < BIN_OP_COUNT; ++i)
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

Token_Kind
next_tok_kind(Parser_Context* ctx) {
    if (!ctx->tokens) return TOKEN_EOF;
    Token_Kind kind = (*ctx->tokens).kind;
    return kind;
}

bool tok_is_on_same_line(Parser_Context* ctx) {
    s64 l1 = ctx->curr_tok.line_pos;
    s64 l2 = ctx->prev_tok.line_pos;
    return l1 == l2;
}

bool next_tok_is_on_same_line(Parser_Context* ctx) {
    Token t2 = next_tok(ctx);
    s64   l1 = ctx->curr_tok.line_pos;
    s64   l2 = t2.line_pos;
    return l1 == l2;
}
bool tok_is(Parser_Context* ctx, Token_Kind kind) {
    return ctx->curr_tok.kind == kind;
}

void eat(Parser_Context* ctx) {
    // Set the previous token
    ctx->prev_tok = ctx->curr_tok;

    // Get the current token
    ctx->curr_tok = *ctx->tokens;

    // Ready the next token
    if (ctx->tokens) ++ctx->tokens;
}

void eat_kind(Parser_Context* ctx, Token_Kind kind) {
    Token_Kind tk = ctx->curr_tok.kind;
    if (tk != kind) {
        error("[%s:%s:%s] expected '%s' got '%s'", give_unique_color(ctx->file), give_unique_color(strf("%d", ctx->curr_tok.line_pos)), give_unique_color(strf("%d", ctx->curr_tok.col_pos)), give_unique_color(token_kind_to_str(kind)), give_unique_color(tokValue(ctx)));
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
    Loc_Info loc_info;
    loc_info.line_pos = ctx->curr_tok.line_pos;
    loc_info.col_pos  = ctx->curr_tok.col_pos;
    return loc_info;
}
