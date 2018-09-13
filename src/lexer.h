#ifndef LEXER_H
#define LEXER_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                              lexer.h
//------------------------------------------------------------------------------

typedef enum
{
    TOKEN_UNKNOWN,
    TOKEN_EOF,

    TOKEN_IDENTIFIER,

    TOKEN_FOREIGN,
    TOKEN_NIL,
    TOKEN_PRINT,
    TOKEN_LOAD,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_CAST,
    TOKEN_MALLOC,
    TOKEN_SIZEOF,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_STRUCT,
    TOKEN_ENUM,
    TOKEN_REPEAT,
    TOKEN_BREAK,
    TOKEN_CONTINUE,

    TOKEN_NUMBER,

    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_HEX,

    TOKEN_STRING,

    TOKEN_PIPE_PIPE,
    TOKEN_AND_AND,
    TOKEN_PLUS_EQUAL,
    TOKEN_MINUS_EQUAL,
    TOKEN_FWSLASH_EQUAL,
    TOKEN_HAT_EQUAL,
    TOKEN_ASTERISK_EQUAL,
    TOKEN_PIPE_EQUAL,
    TOKEN_PERCENT_EQUAL,
    TOKEN_AND_EQUAL,
    TOKEN_BITWISE_LEFTSHIFT,
    TOKEN_BITWISE_RIGHTSHIFT,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,
    TOKEN_COLON_COLON,
    TOKEN_COLON_EQUAL,
    TOKEN_RIGHT_ARROW,
    TOKEN_PIPE,
    TOKEN_TILDE,
    TOKEN_AT,
    TOKEN_DOLLAR_SIGN,
    TOKEN_HAT,
    TOKEN_BANG,
    TOKEN_AND,
    TOKEN_LESS_EQUAL,
    TOKEN_LESS_LESS,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_GREATER_GREATER,
    TOKEN_GREATER_EQUAL,
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
    TOKEN_MINUS,
    TOKEN_MINUS_MINUS,
    TOKEN_MINUS_MINUS_MINUS,
    TOKEN_QUESTION_MARK,
    TOKEN_PLUS,
    TOKEN_PLUS_PLUS,
    TOKEN_PERCENT,
    TOKEN_FWSLASH,
    TOKEN_BWSLASH,
    TOKEN_HASH,
    TOKEN_EQUAL,

} Token_Kind;

#define THI_SYNTAX_POINTER TOKEN_ASTERISK
#define THI_SYNTAX_ADDRESS TOKEN_AND

typedef struct Token Token;

struct Token {
    Token_Kind kind;
    const char* value;
};

Token* generate_tokens_from_source(char* source);
void print_tokens(Token* tokens);
void print_token(Token token);
const char* token_kind_to_str(Token_Kind kind);

#endif
