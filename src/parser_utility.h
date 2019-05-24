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

#ifndef PARSER_UTILITY_H
#define PARSER_UTILITY_H

#include "ast.h"
#include "lexer.h"
#include "list.h"
#include "map.h"
#include "type.h"
#include "typedefs.h"

typedef struct
{
    Token* tokens;

    char* file;
    char* dir;

    u64 lines;
    u64 comments;

    bool inside_parens;

    Map*  symbols;
    List* loads;

    Token top_tok;
    Token curr_tok;
    Token prev_tok;

    AST* llast_if_statement; // used for dangling else
    AST* olast_if_statement; // used for dangling else

} Parser_Context;

Parser_Context make_parser_context(void);
s64            get_integer(Parser_Context* ctx);
f64            get_float(Parser_Context* ctx);
int            get_tok_precedence(Parser_Context* ctx);
Token          next_tok(Parser_Context* ctx);
Token_Kind
next_tok_kind(Parser_Context* ctx);
Loc_Info loc(Parser_Context* ctx);
bool     tok_is_on_same_line(Parser_Context* ctx);
bool     next_tok_is_on_same_line(Parser_Context* ctx);
bool     tok_is(Parser_Context* ctx, Token_Kind kind);
void     eat(Parser_Context* ctx);
void     eat_kind(Parser_Context* ctx, Token_Kind kind);
void     set_if_statement(Parser_Context* ctx, AST* if_statement);
void     restore_if_statement(Parser_Context* ctx);
void     set_dangling_else(Parser_Context* ctx, AST* else_block);

Token_Kind tokKind(Parser_Context* ctx);
char*      tokValue(Parser_Context* ctx);
Token      currTok(Parser_Context* ctx);
Token      prevTok(Parser_Context* ctx);

#endif
