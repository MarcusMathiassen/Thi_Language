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

#ifndef PARSER_H
#define PARSER_H
#include "common.h"   // AST
#include "ast.h"   // AST
#include "map.h"   // Map
#include "list.h"  // List
#include "lex.h" // Token
typedef struct {
    Token* tokens;
    char* file;
    char* dir;
    u64 lines;
    u64 comments;
    bool inside_parens;
    bool inside_asm;
    Map* symbols;
    List* loads;
    Token top_tok;
    Token curr_tok;
    Token prev_tok;
    AST* llast_if_statement; // used for dangling else
    AST* olast_if_statement; // used for dangling else
} Parser_Context;
Parser_Context make_parser_context(void);
AST* parse_file(Parser_Context* ctx, char* file);
#endif
