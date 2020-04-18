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

#ifndef LEX_H
#define LEX_H

#include "common.h"

/*
     -- ASCII TABLE -- 

      32   @ 64  ` 96
    ! 33   A 65  a 97
    " 34   B 66  b 98
    # 35   C 67  c 99
    $ 36   D 68  d 100
    % 37   E 69  e 101
    & 38   F 70  f 102
    ' 39   G 71  g 103
    ( 40   H 72  h 104
    ) 41   I 73  i 105
    * 42   J 74  j 106
    + 43   K 75  k 107
    , 44   L 76  l 108
    - 45   M 77  m 109
    . 46   N 78  n 110
    / 47   O 79  o 111
    0 48   P 80  p 112
    1 49   Q 81  q 113
    2 50   R 82  r 114
    3 51   S 83  s 115π
    4 52   T 84  t 116
    5 53   U 85  u 117
    6 54   V 86  v 118
    7 55   W 87  w 119
    8 56   X 88  x 120
    9 57   Y 89  y 121
    : 58   Z 90  z 122
    ; 59   [ 91  { 123
    < 60   \ 92  | 124
    = 61   ] 93  } 125
    > 62   ^ 94  ~ 126
    ? 63   _ 95  
*/


// @Volatile
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_UNKNOWN,

    TOKEN_WHITESPACE = 32,
    TOKEN_BANG,
    TOKEN_DOUBLE_QUOTATION,
    TOKEN_HASH,
    TOKEN_DOLLAR_SIGN,
    TOKEN_PERCENT,
    TOKEN_AND,
    TOKEN_SINGLE_QUOTATION,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_ASTERISK,
    TOKEN_PLUS,
    TOKEN_COMMA,
    TOKEN_MINUS, 
    TOKEN_DOT,
    TOKEN_FWSLASH = 47,
    TOKEN_COLON = 58,
    TOKEN_SEMICOLON,
    TOKEN_LT,
    TOKEN_EQ,
    TOKEN_GT,
    TOKEN_QUESTION_MARK,
    TOKEN_AT = 64,
    TOKEN_OPEN_BRACKET = 91,
    TOKEN_BWSLASH,
    TOKEN_CLOSE_BRACKET,
    TOKEN_HAT,
    TOKEN_BACK_TICK = 96,
    TOKEN_OPEN_BRACE = 123,
    TOKEN_PIPE,
    TOKEN_CLOSE_BRACE,
    TOKEN_TILDE = 126,

    // Constructs
    TOKEN_NEWLINE = 200, // Everything before this is reserved for ASCII
    TOKEN_COMMENT,
    TOKEN_LITERAL,
    TOKEN_BLOCK_START,
    TOKEN_BLOCK_END,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_HEX,
    TOKEN_STRING,
    TOKEN_CHAR,


    // @Volatile: do not move me please.
    _BEGINNING_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_,

    // Keywords
    // @Volatile:
    //      if you change this block you must update these places:
    //          1. keyword_lookup_table
    //          2. After parsing identifiers, the keywords check.
    _TOKEN_KEYWORDS_START_ = 300,
    TOKEN_ASM,
    TOKEN_DEF,
    TOKEN_LINK,
    TOKEN_LOAD,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_DEFER,
    TOKEN_EXTERN,
    TOKEN_SIZEOF,
    TOKEN_TYPEOF,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_IN,
    TOKEN_RETURN,
    TOKEN_ENUM,
    TOKEN_STRUCT,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_FALLTHROUGH,
    TOKEN_AS,
    TOKEN_IS,
    TOKEN_CAST,
    //
    TOKEN_IDENTIFIER,

    _END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_, // @Volatile: do not move me please.

    // Double and tripple tokens
    TOKEN_PIPE_PIPE = 400, // ||
    TOKEN_AND_AND, // &&
    TOKEN_PLUS_EQ, // +=
    TOKEN_MINUS_EQ, // -=
    TOKEN_HAT_EQ, // ^=
    TOKEN_ASTERISK_EQ, // *=
    TOKEN_PIPE_EQ, // |=
    TOKEN_PERCENT_EQ, // %=
    TOKEN_AND_EQ, // &= 
    TOKEN_EQ_EQ, // == 
    TOKEN_EQ_GT, // =>
    TOKEN_BANG_EQ, // !=
    TOKEN_COLON_COLON, // ::
    TOKEN_COLON_EQ, // :=
    TOKEN_MINUS_GT, // ->
    TOKEN_LT_EQ, // <=
    TOKEN_LT_LT, // <<
    TOKEN_LT_LT_EQ, // <<=
    TOKEN_GT_EQ, // >=
    TOKEN_GT_GT, // >>
    TOKEN_GT_GT_EQ, // >>=
    TOKEN_DOT_DOT, // ..
    TOKEN_DOT_DOT_DOT, // ...
    TOKEN_MINUS_MINUS, // --
    TOKEN_MINUS_MINUS_MINUS, // ---
    TOKEN_PLUS_PLUS, // ++
    TOKEN_FWSLASH_EQ, // /=
    TOKEN_PIPE_GT, // |>

    _TOKEN_COUNT_,
} Token_Kind;

// We need the stringified value of these when LEXING:
//      1. Identifiers
//
// Of these, these get interened:
//      1. Identifiers
//      2. ???
// Everything else can be stringified at a later point

typedef struct {
    Token_Kind kind;

    // These are used to create the stringified value
    char* start;
    char* end;

    s64 line;
    s64 col;

} Token;

typedef struct {
    Token_Kind kind;
    char* start, end;
} Token2;

typedef struct
{
    Token* data;
    s64 count;
    s64 allocated;
} Token_Array;

typedef struct {
    Token_Array tokens;
    s64 lines;
    s64 comments;
} Lexed_File;

Lexed_File lex(char* source);

char* token_to_str(Token token);
char* token_value(Token token);
char* token_kind_to_str(Token_Kind kind);
char* token_kind_to_str_actual(Token_Kind kind);

s64 get_line_and_col(char* source, Token token);
void lexer_test(void);

void initilize_lex(void);

#define THI_SYNTAX_POINTER TOKEN_ASTERISK
#define THI_SYNTAX_ADDRESS TOKEN_AND
#define THI_SYNTAX_SCOPE_BEGIN TOKEN_OPEN_BRACE
#define THI_SYNTAX_SCOPE_END TOKEN_CLOSE_BRACE
#define THI_SYNTAX_DEF TOKEN_COLON
#define THI_SYNTAX_CONSTANT_DEF TOKEN_COLON_COLON
#define THI_SYNTAX_VARIABLE_DECL_TYPE_INF TOKEN_CLOSE_BRACE
#define THI_SYNTAX_ASSIGNMENT TOKEN_EQ

#ifdef NDEBUG
#define ERROR_UNHANDLED_TOKEN(x)
#define ERROR_UNHANDLED_TOKEN_KIND(x)
#else
#define ERROR_UNHANDLED_TOKEN(x) ERROR_UNHANDLED_KIND(token_to_str(x))
#define ERROR_UNHANDLED_TOKEN_KIND(x) ERROR_UNHANDLED_KIND(token_kind_to_str(x))
#endif

#endif
