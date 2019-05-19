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
//                               cst.h
//------------------------------------------------------------------------------

#ifndef CST_H
#define CST_H

#include "lexer.h" // Token_Kind, Token
#include "list.h"  // List
#include "type.h"  // Type

typedef enum {
    CST_TOKEN,
    CST_PROGRAM,
    CST_MODULE,
    CST_IDENTIFIER,
    CST_SPACE_SEPARATED_IDENTIFIER_LIST,
    CST_COMMA_SEPARATED_LIST,
    CST_COUNT,
} CST_Kind;

//------------------------------------------------------------------------------
//                                  CST Structures
//------------------------------------------------------------------------------

typedef struct CST CST;

struct CST {
    CST_Kind kind;
    Token    token;
    union {
        struct
        {
            List* modules;
        } Program;
        struct
        {
            char* name;
            List* nodes;
        } Module;
        struct
        {
            char* name;
        } Identifier;
        struct
        {
            List* identifiers;
        } Space_Separated_Identifier_List;
        struct
        {
            List* nodes;
        } Comma_Separated_List;
    };
};

List* generate_cst_from_tokens(Token* tokens);

CST* make_cst_token(Token token);
CST* make_cst_program(List* modules);
CST* make_cst_module(char* name, List* nodes);
CST* make_cst_identifier(char* name);
CST* make_cst_space_separated_identifier_list(List* identifiers);
CST* make_cst_comma_separated_list(List* arguments);

void  cst_tests(void);
char* cst_kind_to_str(CST_Kind kind);
char* cst_to_str(CST* node);

#endif
