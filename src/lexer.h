#ifndef LEXER_H
#define LEXER_H

#include "list.h" // list

//------------------------------------------------------------------------------
//                              lexer.h
//------------------------------------------------------------------------------

typedef enum
{
    TOKEN_UNKNOWN,
    TOKEN_EOF,

    TOKEN_IDENTIFIER,

    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_CAST,
    TOKEN_SIZEOF,

    TOKEN_FOREIGN,
    TOKEN_LOAD,

    TOKEN_DEFER,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,

    TOKEN_STRUCT,
    TOKEN_ENUM,
    TOKEN_BREAK,
    TOKEN_CONTINUE,

    TOKEN_NUMBER,

    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_HEX,

    TOKEN_STRING,

    TOKEN_PIPE_PIPE,
    TOKEN_AND_AND,
    TOKEN_PLUS_EQ,
    TOKEN_MINUS_EQ,
    TOKEN_FWSLASH_EQ,
    TOKEN_HAT_EQ,
    TOKEN_ASTERISK_EQ,
    TOKEN_PIPE_EQ,
    TOKEN_PERCENT_EQ,
    TOKEN_AND_EQ,
    TOKEN_BITWISE_LEFTSHIFT,
    TOKEN_BITWISE_RIGHTSHIFT,
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
    TOKEN_LT_EQ,
    TOKEN_LT_LT,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_GT_GT,
    TOKEN_GT_EQ,
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
    TOKEN_EQ,
    TOKEN_COMMENT,
    TOKEN_WHITESPACE,

} Token_Kind;

#define THI_SYNTAX_POINTER TOKEN_ASTERISK
#define THI_SYNTAX_ADDRESS TOKEN_AND

#define THI_SYNTAX_SCOPE_BEGIN TOKEN_OPEN_BRACE
#define THI_SYNTAX_SCOPE_END TOKEN_CLOSE_BRACE

#define THI_SYNTAX_DEF TOKEN_COLON
#define THI_SYNTAX_CONSTANT_DEF TOKEN_COLON_COLON
#define THI_SYNTAX_VARIABLE_DECL_TYPE_INF TOKEN_CLOSE_BRACE

#define THI_SYNTAX_ASSIGNMENT TOKEN_EQ

typedef struct Token Token;

struct Token
{
    Token_Kind kind;
    char* value;
};

List* generate_tokens_from_source(char* source_file);
void print_tokens(List* tokens);
void print_token(Token token);
char* token_kind_to_str(Token_Kind kind);

#endif
