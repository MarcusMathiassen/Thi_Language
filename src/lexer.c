#include "lexer.h"

#include "globals.h"         // get_source_file
#include "stretchy_buffer.h" // sb_push, sb_count
#include "string.h"          // str_intern_range
#include "typedefs.h"
#include "utility.h" // info, success, error, warning
#include <assert.h>  // assert
#include <ctype.h>   // isalnum, isdigit, isspace, isalpha
#include <stdlib.h>  // malloc
#include <string.h>  // strncmp

//------------------------------------------------------------------------------
//                              lexer.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Character Stream
//------------------------------------------------------------------------------

char* c = NULL;
Token token;
u64 line_count = 1;
char* start_of_line = NULL;
//------------------------------------------------------------------------------

//                               Lexer Functions
//------------------------------------------------------------------------------

Token get_token(void);
bool is_valid_digit(void);
bool is_valid_identifier(void);

int get_keyword_index(char* identifier);
Token_Kind get_identifier_kind(char* identifier);
//------------------------------------------------------------------------------
//                               Keywords
//------------------------------------------------------------------------------
typedef enum
{
    KEY_FOREIGN,
    KEY_NIL,
    KEY_PRINT,
    KEY_LOAD,
    KEY_TRUE,
    KEY_FALSE,
    KEY_CAST,
    KEY_MALLOC,
    KEY_SIZEOF,
    KEY_IF,
    KEY_ELSE,
    KEY_FOR,
    KEY_WHILE,
    KEY_RET,
    KEY_STRUCT,
    KEY_ENUM,
    KEY_REPEAT,
    KEY_BREAK,
    KEY_CONTINUE,
    KEY_DO,

} Keyword_Kind;

#define KEYWORD_COUNT 20
char* keywords_str[KEYWORD_COUNT] = {
    "foreign", "nil", "print", "load", "true",   "false", "cast",   "malloc", "sizeof",   "if",
    "else", "for", "while", "ret",  "struct", "enum",  "repeat", "break",  "continue", "do",
};

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

void print_token(Token token) { info("%s %s", token_kind_to_str(token.kind), token.value); }

void print_tokens(Token* tokens)
{
    i32 count = sb_count(tokens);
    info("Printing %d tokens..", count);
    for (int i = 0; i < count; ++i)
        print_token(tokens[i]);
}

Token* generate_tokens_from_source(char* source)
{
    info("Generating Tokens from Source");
    // info("%s", source);

    //
    Token* tokens = NULL;

    // 'c' represents the current character in the stream.
    // char* gets set to the start of the stream
    c = source;
    start_of_line = c;
    token.kind = TOKEN_UNKNOWN;

    // Fill the tokens
    while (token.kind != TOKEN_EOF) {
        get_token();
        sb_push(tokens, token);
        // info("%s %s", token_kind_to_str(token.kind), token.value);
    }

    // Print some result info
    info("Lines: %d | Tokens: %d", line_count, sb_count(tokens));

    return tokens;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

u64 get_line_pos() { return c - start_of_line + 1; }

const char EOF = '\0';

void skip_whitespace()
{
    while (*c == ' ' || *c == '\t' || *c == '\n' || *c == '\r') {
        if (*c == '\n') ++line_count;
        ++c;
    }
}

void skip_line()
{
    while (*c != '\n' && *c != '\r') {
        ++c;
    }
    if (*c != EOF) {
        ++line_count;
        start_of_line = c;
    }
}

void skip_comments()
{
    if (*c == '#') {
        skip_line();
    }
}

void scan_digit()
{
    bool is_hex = false;
    bool is_float = false;

    // Number: [0-9._]+e[0-9]+
    if (isdigit(*c)) {
        while (is_valid_digit() || (is_hex && is_valid_identifier())) {
            // if (!is_valid_digit()) break;
            if (*c == 'x') is_hex = true;
            if (*c == '.') is_float = true;
            ++c;
        }
    }

    token.kind = TOKEN_INTEGER;
    if (is_hex) token.kind = TOKEN_HEX;
    if (is_float) token.kind = TOKEN_FLOAT;
}

int get_keyword_index(char* identifier)
{
    for (int i = 0; i < KEYWORD_COUNT; ++i)
        if (strcmp(identifier, keywords_str[i]) == 0) return i;
    return -1;
}
Token_Kind get_identifier_kind(char* identifier)
{

    switch (get_keyword_index(identifier)) {
    case KEY_FOREIGN: return TOKEN_FOREIGN;
    case KEY_NIL: return TOKEN_NIL;
    case KEY_PRINT: return TOKEN_PRINT;
    case KEY_LOAD: return TOKEN_LOAD;
    case KEY_TRUE: return TOKEN_TRUE;
    case KEY_FALSE: return TOKEN_FALSE;
    case KEY_CAST: return TOKEN_CAST;
    case KEY_MALLOC: return TOKEN_MALLOC;
    case KEY_SIZEOF: return TOKEN_SIZEOF;
    case KEY_IF: return TOKEN_IF;
    case KEY_ELSE: return TOKEN_ELSE;
    case KEY_FOR: return TOKEN_FOR;
    case KEY_WHILE: return TOKEN_WHILE;
    case KEY_RET: return TOKEN_RETURN;
    case KEY_STRUCT: return TOKEN_STRUCT;
    case KEY_ENUM: return TOKEN_ENUM;
    case KEY_REPEAT: return TOKEN_REPEAT;
    case KEY_BREAK: return TOKEN_BREAK;
    case KEY_CONTINUE: return TOKEN_CONTINUE;
    }

    return TOKEN_IDENTIFIER;
}

inline bool is_valid_identifier() { return isalnum(*c) || *c == '_'; }
inline bool is_valid_digit() { return isdigit(*c) || *c == '.' || *c == '_' || *c == 'e' || *c == 'x'; }

#define CASE_SINGLE_TOKEN(c1, t_kind)                                                                                  \
    case c1: token.kind = t_kind; ++c;

Token get_token()
{
scan:
    skip_whitespace();
    skip_comments();

    token.kind = TOKEN_UNKNOWN;
    token.value = c;

    switch (*c) {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        goto scan;

        CASE_SINGLE_TOKEN(EOF, TOKEN_EOF);
        break;
        CASE_SINGLE_TOKEN('(', TOKEN_OPEN_PAREN);
        break;
        CASE_SINGLE_TOKEN(')', TOKEN_CLOSE_PAREN);
        break;
        CASE_SINGLE_TOKEN('[', TOKEN_OPEN_BRACKET);
        break;
        CASE_SINGLE_TOKEN(']', TOKEN_CLOSE_BRACKET);
        break;
        CASE_SINGLE_TOKEN('{', TOKEN_OPEN_BRACE);
        break;
        CASE_SINGLE_TOKEN('}', TOKEN_CLOSE_BRACE);
        break;
        CASE_SINGLE_TOKEN(',', TOKEN_COMMA);
        break;
        CASE_SINGLE_TOKEN('~', TOKEN_TILDE);
        break;
        CASE_SINGLE_TOKEN('$', TOKEN_DOLLAR_SIGN);
        break;
        CASE_SINGLE_TOKEN('@', TOKEN_AT);
        break;
        CASE_SINGLE_TOKEN('^', TOKEN_HAT);
        break;
        CASE_SINGLE_TOKEN(';', TOKEN_SEMICOLON);
        break;
        CASE_SINGLE_TOKEN('?', TOKEN_QUESTION_MARK);
        break;
        CASE_SINGLE_TOKEN('\\', TOKEN_BWSLASH);
        break;

        CASE_SINGLE_TOKEN('/', TOKEN_FWSLASH);
        switch (*c) {
            CASE_SINGLE_TOKEN('=', TOKEN_FWSLASH_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('!', TOKEN_BANG);
        switch (*c) {
            CASE_SINGLE_TOKEN('=', TOKEN_BANG_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('*', TOKEN_ASTERISK);
        switch (*c) {
            CASE_SINGLE_TOKEN('=', TOKEN_ASTERISK_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('|', TOKEN_PIPE);
        switch (*c) {
            CASE_SINGLE_TOKEN('|', TOKEN_PIPE_PIPE);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_PIPE_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('<', TOKEN_LT);
        switch (*c) {
            CASE_SINGLE_TOKEN('<', TOKEN_LT_LT);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_LT_EQ);
            break;
        }
        break;
        CASE_SINGLE_TOKEN('>', TOKEN_GT);
        switch (*c) {
            CASE_SINGLE_TOKEN('>', TOKEN_GT_GT);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_GT_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('.', TOKEN_DOT);
        switch (*c) {
            CASE_SINGLE_TOKEN('.', TOKEN_DOT_DOT);
            switch (*c) {
                CASE_SINGLE_TOKEN('.', TOKEN_DOT_DOT_DOT);
                break;
            }
            break;
        }
        break;

        CASE_SINGLE_TOKEN(':', TOKEN_COLON);
        switch (*c) {
            CASE_SINGLE_TOKEN(':', TOKEN_COLON_COLON);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_COLON_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('-', TOKEN_MINUS);
        switch (*c) {
            CASE_SINGLE_TOKEN('-', TOKEN_MINUS_MINUS);
            switch (*c) {
                CASE_SINGLE_TOKEN('-', TOKEN_MINUS_MINUS_MINUS);
                break;
            }
            break;
            CASE_SINGLE_TOKEN('>', TOKEN_RIGHT_ARROW);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_MINUS_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('+', TOKEN_PLUS);
        switch (*c) {
            CASE_SINGLE_TOKEN('+', TOKEN_PLUS_PLUS);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_PLUS_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('%', TOKEN_PERCENT);
        switch (*c) {
            CASE_SINGLE_TOKEN('=', TOKEN_PERCENT_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('&', TOKEN_AND);
        switch (*c) {
            CASE_SINGLE_TOKEN('&', TOKEN_AND_AND);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_AND_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('=', TOKEN_EQ);
        switch (*c) {
            CASE_SINGLE_TOKEN('=', TOKEN_EQ_EQ);
            break;
            CASE_SINGLE_TOKEN('>', TOKEN_EQ_GT);
            break;
        }
        break;

    case '"': {
        token.kind = TOKEN_STRING;
        c++;
        token.value = c;
        while (*c != '"')
            ++c;
        token.value = str_intern_range(token.value, c++);
        return token;
    } break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        scan_digit();
    } break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '_': {
        ++c;
        while (is_valid_identifier())
            ++c;
        token.value = str_intern_range(token.value, c);
        token.kind = get_identifier_kind(token.value);
        return token;
    } break;

    default:
        error("Unhandled token character %c in file %s line %d col %d", *c, get_source_file(), line_count,
              get_line_pos());
    }

    if (token.kind != TOKEN_EOF) {
        token.value = str_intern_range(token.value, c);
    }
    return token;
}

char* token_kind_to_str(Token_Kind kind)
{
    switch (kind) {
    case TOKEN_EOF: return "eof";
    case TOKEN_UNKNOWN: return "unknown";
    case TOKEN_NIL: return "nil";
    case TOKEN_CAST: return "cast";
    case TOKEN_MALLOC: return "malloc";
    case TOKEN_SIZEOF: return "sizeof";
    case TOKEN_PRINT: return "print";
    case TOKEN_LOAD: return "load";
    case TOKEN_FOREIGN: return "foreign";
    case TOKEN_RETURN: return "ret";
    case TOKEN_TRUE: return "true";
    case TOKEN_FALSE: return "false";
    case TOKEN_IF: return "if";
    case TOKEN_ELSE: return "else";
    case TOKEN_FOR: return "for";
    case TOKEN_WHILE: return "while";
    case TOKEN_IDENTIFIER: return "identifier";
    case TOKEN_BREAK: return "break";
    case TOKEN_REPEAT: return "repeat";
    case TOKEN_STRUCT: return "struct";
    case TOKEN_ENUM: return "enum";
    case TOKEN_CONTINUE: return "continue";
    case TOKEN_WHITESPACE: return "whitespace";
    case TOKEN_PIPE_PIPE: return "||";
    case TOKEN_AND_AND: return "&&";
    case TOKEN_PLUS_EQ: return "+=";
    case TOKEN_MINUS_EQ: return "-=";
    case TOKEN_FWSLASH_EQ: return "/=";
    case TOKEN_HAT_EQ: return "^=";
    case TOKEN_ASTERISK_EQ: return "*=";
    case TOKEN_PIPE_EQ: return "|=";
    case TOKEN_PERCENT_EQ: return "%=";
    case TOKEN_AND_EQ: return "&=";
    case TOKEN_PLUS_PLUS: return "++";
    case TOKEN_MINUS_MINUS: return "--";
    case TOKEN_BITWISE_LEFTSHIFT: return "<<=";
    case TOKEN_BITWISE_RIGHTSHIFT: return ">>=";
    case TOKEN_EQ_EQ: return "==";
    case TOKEN_EQ_GT: return "=>";
    case TOKEN_BANG_EQ: return "!=";
    case TOKEN_COLON_COLON: return "::";
    case TOKEN_COLON_EQ: return ":=";
    case TOKEN_RIGHT_ARROW: return "->";
    case TOKEN_PIPE: return "|";
    case TOKEN_TILDE: return "~";
    case TOKEN_AT: return "@";
    case TOKEN_DOLLAR_SIGN: return "$";
    case TOKEN_HAT: return "^";
    case TOKEN_BANG: return "!";
    case TOKEN_AND: return "&";
    case TOKEN_LT_EQ: return "<=";
    case TOKEN_LT_LT: return "<<";
    case TOKEN_LT: return "<";
    case TOKEN_GT: return ">";
    case TOKEN_GT_GT: return ">>";
    case TOKEN_GT_EQ: return ">=";
    case TOKEN_OPEN_PAREN: return "(";
    case TOKEN_CLOSE_PAREN: return ")";
    case TOKEN_OPEN_BRACKET: return "[";
    case TOKEN_CLOSE_BRACKET: return "]";
    case TOKEN_OPEN_BRACE: return "{";
    case TOKEN_CLOSE_BRACE: return "}";
    case TOKEN_COMMA: return ",";
    case TOKEN_DOT: return ".";
    case TOKEN_DOT_DOT: return "..";
    case TOKEN_COLON: return ":";
    case TOKEN_SEMICOLON: return ";";
    case TOKEN_ASTERISK: return "*";
    case TOKEN_MINUS: return "-";
    case TOKEN_QUESTION_MARK: return "?";
    case TOKEN_PLUS: return "+";
    case TOKEN_COMMENT: return "#";
    case TOKEN_PERCENT: return "%";
    case TOKEN_FWSLASH: return "/";
    case TOKEN_BWSLASH: return "\\";
    case TOKEN_HASH: return "#";
    case TOKEN_EQ: return "=";
    case TOKEN_STRING: return "string";
    case TOKEN_INTEGER: return "integer";
    case TOKEN_NUMBER: return "number";
    case TOKEN_HEX: return "hex";
    case TOKEN_FLOAT: return "float";
    case TOKEN_MINUS_MINUS_MINUS: return "---";
    case TOKEN_DOT_DOT_DOT: return "...";
    }
    error("Unhandled token kind.");
    return "TOKEN_UNKNOWN";
}
