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

#ifndef LEXER_H
#define LEXER_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                              lexer.h
//------------------------------------------------------------------------------

typedef enum {

    TOKEN_UNKNOWN,
    TOKEN_EOF,

    TOKEN_BLOCK_START,
    TOKEN_BLOCK_END,
    TOKEN_TERMINAL,

    TOKEN_COMMENT,
    TOKEN_WHITESPACE,
    TOKEN_NEWLINE,
    TOKEN_IDENTIFIER,

    TOKEN_DEF,
    TOKEN_IS,
    TOKEN_IN,
    TOKEN_CAST,
    TOKEN_TYPEOF,
    TOKEN_SIZEOF,
    TOKEN_LINK,
    TOKEN_FALLTHROUGH,
    TOKEN_EXTERN,
    TOKEN_LOAD,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_TYPE,
    TOKEN_DEFER,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_ENUM,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_AS,

    TOKEN_NUMBER,
    TOKEN_CHAR,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_HEX,
    TOKEN_STRING,
    TOKEN_PIPE_PIPE,
    TOKEN_AND_AND,
    TOKEN_PLUS_EQ,
    TOKEN_MINUS_EQ,
    TOKEN_HAT_EQ,
    TOKEN_ASTERISK_EQ,
    TOKEN_PIPE_EQ,
    TOKEN_PERCENT_EQ,
    TOKEN_AND_EQ,
    TOKEN_BITWISE_LEFTSHIFT,
    TOKEN_BITWISE_LEFTSHIFT_EQ,
    TOKEN_BITWISE_RIGHTSHIFT,
    TOKEN_BITWISE_RIGHTSHIFT_EQ,
    TOKEN_EQ_EQ,
    TOKEN_EQ_GT,
    TOKEN_BANG_EQ,
    TOKEN_COLON_COLON,
    TOKEN_COLON_EQ,
    TOKEN_RIGHT_ARROW,
    TOKEN_PIPE,
    TOKEN_TILDE,
    TOKEN_AT,
    TOKEN_DOLLAR_SIGN,
    TOKEN_HAT,
    TOKEN_BANG,
    TOKEN_AND,
    TOKEN_LT,
    TOKEN_LT_EQ,
    TOKEN_LT_LT,
    TOKEN_LT_LT_EQ,
    TOKEN_GT,
    TOKEN_GT_EQ,
    TOKEN_GT_GT,
    TOKEN_GT_GT_EQ,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_DOT_DOT,
    TOKEN_DOT_DOT_DOT,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_ASTERISK,
    TOKEN_ASTERISK_FWSLASH,
    TOKEN_MINUS,
    TOKEN_MINUS_MINUS,
    TOKEN_MINUS_MINUS_MINUS,
    TOKEN_QUESTION_MARK,
    TOKEN_PLUS,
    TOKEN_PLUS_PLUS,
    TOKEN_PERCENT,
    TOKEN_FWSLASH,
    TOKEN_FWSLASH_ASTERISK,
    TOKEN_FWSLASH_FWSLASH,
    TOKEN_FWSLASH_EQ,
    TOKEN_BWSLASH,
    TOKEN_HASH,
    TOKEN_EQ,
    TOKEN_PIPE_GT,
    __TOKEN_COUNT__
} Token_Kind;

typedef struct
{
    Token_Kind kind;
    char* value;
    s64 line_pos, col_pos;
} Token;

typedef struct
{
    Token* data;
    s64 count;
    s64 allocated;
} Token_Array;

typedef struct
{
    Token_Array tokens;
    s64 lines;
    s64 comments;
    f64 seconds;
} Lexed_File;

void print_tokens(Token_Array tokens);
void print_token(Token token);
char* token_to_str(Token token);
char* token_to_json(Token token);
char* token_kind_to_str(Token_Kind kind);
Lexed_File generate_tokens_from_file(char* file);
Lexed_File generate_tokens_from_source(char* source);
void lexer_test(void);

#define THI_SYNTAX_POINTER TOKEN_ASTERISK
#define THI_SYNTAX_ADDRESS TOKEN_AND
#define THI_SYNTAX_SCOPE_BEGIN TOKEN_OPEN_BRACE
#define THI_SYNTAX_SCOPE_END TOKEN_CLOSE_BRACE
#define THI_SYNTAX_DEF TOKEN_COLON
#define THI_SYNTAX_CONSTANT_DEF TOKEN_COLON_COLON
#define THI_SYNTAX_VARIABLE_DECL_TYPE_INF TOKEN_CLOSE_BRACE
#define THI_SYNTAX_ASSIGNMENT TOKEN_EQ

#endif
