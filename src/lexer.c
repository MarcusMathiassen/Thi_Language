#include "lexer.h"
#include "typedefs.h"

#include "globals.h" // get_source_file
#include "string.h"  // str_intern_range
#include "utility.h" // info, success, error, warning, xmalloc, xrealloc
#include <assert.h>  // assert
#include <ctype.h>   // isalnum, isdigit, isspace, isalpha
#include <stdlib.h>  // xmalloc
#include <string.h>  // strncmp

//------------------------------------------------------------------------------
//                              lexer.c
//------------------------------------------------------------------------------

typedef struct {
    char* current_char_in_stream;
    char* position_of_newline;
    char* start_of_line;
    u64   line_count;
    u64   comment_count;
    u64   previous_indentation_level;
    u64   current_indentation_level;
} Lexer_Context;

typedef struct {
    Token_Kind kind;
    char*      value;
    s64        line_pos;
    s64        col_pos;
    char*      line_start;
} Token_Fat;

//------------------------------------------------------------------------------
//                               Lexer Functions
//------------------------------------------------------------------------------

Token_Fat   get_next_token(Lexer_Context* lctx);
bool        is_valid_digit(u8 c);
bool        is_valid_identifier(u8 c);
int         get_keyword_index(char* identifier);
Token_Kind  get_identifier_kind(char* identifier);
u64         get_line_pos(Lexer_Context* lctx);
void        token_array_append(Token_Array* l, Token_Fat t);
Token_Array make_token_array();
//------------------------------------------------------------------------------
//                               Keywords
//------------------------------------------------------------------------------
typedef enum {
    KEY_LINK,
    KEY_TYPE,
    KEY_TRUE,
    KEY_FALSE,
    KEY_DEF,
    KEY_DO,
    KEY_DEFER,
    KEY_EXTERN,
    KEY_LOAD,
    KEY_CAST,
    KEY_SIZEOF,
    KEY_IF,
    KEY_ELSE,
    KEY_FOR,
    KEY_WHILE,
    KEY_RETURN,
    KEY_STRUCT,
    KEY_ENUM,
    KEY_BREAK,
    KEY_CONTINUE,
    KEY_AS,
} Keyword_Kind;

#define KEYWORD_COUNT 21
char* keywords_str[KEYWORD_COUNT] = {
    "link",   "type", "true", "false", "def",   "do",     "defer",  "extern", "load",  "cast",
    "sizeof", "if",   "else", "for",   "while", "return", "struct", "enum",   "break", "continue", "as",
};

char** keywords = NULL;

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

Token_Array generate_tokens_from_source(char* source) {

    Lexer_Context lctx;
    lctx.line_count                 = 1;
    lctx.comment_count              = 0;
    lctx.position_of_newline        = NULL;
    lctx.previous_indentation_level = 0;
    lctx.current_indentation_level  = 0;
    lctx.current_char_in_stream     = source; // u8 in stream
    lctx.start_of_line              = source;

    Token_Array token_array = make_token_array();

    if (!keywords) {
        keywords     = xmalloc(sizeof(char*) * KEYWORD_COUNT);
        keywords[0]  = str_intern("link");
        keywords[1]  = str_intern("type");
        keywords[2]  = str_intern("true");
        keywords[3]  = str_intern("false");
        keywords[4]  = str_intern("def");
        keywords[5]  = str_intern("do");
        keywords[6]  = str_intern("defer");
        keywords[7]  = str_intern("extern");
        keywords[8]  = str_intern("load");
        keywords[9]  = str_intern("cast");
        keywords[10] = str_intern("sizeof");
        keywords[11] = str_intern("if");
        keywords[12] = str_intern("else");
        keywords[13] = str_intern("for");
        keywords[14] = str_intern("while");
        keywords[15] = str_intern("return");
        keywords[16] = str_intern("struct");
        keywords[17] = str_intern("enum");
        keywords[18] = str_intern("break");
        keywords[19] = str_intern("continue");
        keywords[20] = str_intern("as");
    }

    // LEX
    for (;;) {
        Token_Fat token = get_next_token(&lctx);
        if (lctx.current_indentation_level > lctx.previous_indentation_level) {
            Token_Fat t;
            t.kind                          = TOKEN_BLOCK_START;
            t.value                         = "";
            t.line_pos                      = lctx.line_count;
            t.col_pos                       = get_line_pos(&lctx);
            t.line_start                    = lctx.start_of_line;
            lctx.previous_indentation_level = lctx.current_indentation_level;
            token_array_append(&token_array, t);
        }
        while (lctx.current_indentation_level < lctx.previous_indentation_level) {
            Token_Fat t;
            t.kind       = TOKEN_BLOCK_END;
            t.value      = "";
            t.line_pos   = lctx.line_count;
            t.col_pos    = get_line_pos(&lctx);
            t.line_start = lctx.start_of_line;
            lctx.previous_indentation_level -= 4;
            token_array_append(&token_array, t);
        }
        if (token.kind != TOKEN_UNKNOWN && token.kind != TOKEN_COMMENT) token_array_append(&token_array, token);
        if (token.kind == TOKEN_EOF) {
            break;
        }
    }

    info("lexed %lld lines, %lld comments", lctx.line_count, lctx.comment_count);

    return token_array;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

#define CASE_SINGLE_TOKEN(c1, t_kind)                                                                                  \
    case c1: token.kind = t_kind; ++c;

Token_Fat get_next_token(Lexer_Context* lctx) {
    char* c = lctx->current_char_in_stream;

    Token_Fat token;
    token.kind       = TOKEN_UNKNOWN;
    token.value      = c;
    token.line_pos   = lctx->line_count;
    token.col_pos    = get_line_pos(lctx);
    token.line_start = lctx->start_of_line;

    switch (*c) {
    case '#': {
        lctx->comment_count += 1;
        ++c;
        token.value = c;
        token.kind  = TOKEN_COMMENT;
        while (*c != '\n' && *c != '\r') {
            ++c;
        }
    } break;
    case ' ':
    case '\n':
    case '\r':
    case '\t': {
        // Skip whitespace
        bool has_newline = false;
        while (*c == ' ' || *c == '\n' || *c == '\r' || *c == '\t') {
            if (*c == '\n') {
                has_newline = true;
                lctx->line_count += 1;
                lctx->position_of_newline = c;
            }
            ++c;
        }
        lctx->start_of_line = c;
        if (has_newline) {
            lctx->current_indentation_level = c - lctx->position_of_newline - 1;
            // info("current_indentation_level: %lld\nprevious_indentation_level: %lld", lctx->current_indentation_level, lctx->previous_indentation_level);
        }
    } break;
        CASE_SINGLE_TOKEN('\0', TOKEN_EOF);
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

    case '\'': {
        token.kind = TOKEN_CHAR;
        ++c;
        token.value = c;
        if (*token.value == '\\') ++c;
        ++c;
    } break;
    case '"': {
        token.kind = TOKEN_STRING;
        c++;
        token.value = c;
        while (*c != '"') {
            ++c;
        }
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
        bool is_hex   = false;
        bool is_float = false;

        // Number: [0-9._]+e[0-9]+
        if (isdigit(*c)) {
            while (is_valid_digit(*c) || (is_hex && is_valid_identifier(*c))) {
                if (*c == 'x') is_hex = true;
                if (*c == '.') is_float = true;
                ++c;
            }
        }

        token.kind = TOKEN_INTEGER;
        if (is_hex) token.kind = TOKEN_HEX;
        if (is_float) token.kind = TOKEN_FLOAT;
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
        while (is_valid_identifier(*c))
            ++c;
        token.kind = TOKEN_IDENTIFIER;
    } break;

    default:
        error("Unhandled token character '%c' in file %s line %d col %d", *c, get_source_file(), lctx->line_count,
              get_line_pos(lctx));
    }

    token.value = str_intern_range(token.value, c);
    switch (token.kind) {
    case TOKEN_IDENTIFIER: token.kind = get_identifier_kind(token.value); break;
    case TOKEN_STRING: ++c; break; // we skip the last '"'
    }

    lctx->current_char_in_stream = c;

    return token;
}

char* token_kind_to_str(Token_Kind kind) {
    switch (kind) {
    case TOKEN_UNKNOWN: return "unknown";
    case TOKEN_EOF: return "eof";
    case TOKEN_COMMENT: return "comment";
    case TOKEN_BLOCK_START: return "block_start";
    case TOKEN_BLOCK_END: return "block_end";
    case TOKEN_WHITESPACE: return "whitespace";
    case TOKEN_NEWLINE: return "newline";
    case TOKEN_IDENTIFIER: return "identifier";
    case TOKEN_AS: return "as";
    case TOKEN_CAST: return "cast";
    case TOKEN_SIZEOF: return "sizeof";
    case TOKEN_LINK: return "link";
    case TOKEN_EXTERN: return "extern";
    case TOKEN_LOAD: return "load";
    case TOKEN_DO: return "do";
    case TOKEN_TRUE: return "true";
    case TOKEN_FALSE: return "false";
    case TOKEN_TYPE: return "type";
    case TOKEN_DEFER: return "defer";
    case TOKEN_IF: return "if";
    case TOKEN_DEF: return "def";
    case TOKEN_ELSE: return "else";
    case TOKEN_FOR: return "for";
    case TOKEN_WHILE: return "while";
    case TOKEN_RETURN: return "return";
    case TOKEN_STRUCT: return "struct";
    case TOKEN_ENUM: return "enum";
    case TOKEN_BREAK: return "break";
    case TOKEN_CONTINUE: return "continue";
    case TOKEN_STRING: return "string";
    case TOKEN_CHAR: return "char";
    case TOKEN_INTEGER: return "integer";
    case TOKEN_NUMBER: return "number";
    case TOKEN_HEX: return "hex";
    case TOKEN_FLOAT: return "float";
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
    case TOKEN_PLUS: return "+";
    case TOKEN_PLUS_PLUS: return "++";
    case TOKEN_MINUS: return "-";
    case TOKEN_MINUS_MINUS: return "--";
    case TOKEN_MINUS_MINUS_MINUS: return "---";
    case TOKEN_EQ_EQ: return "==";
    case TOKEN_EQ_GT: return "=>";
    case TOKEN_BANG_EQ: return "!=";
    case TOKEN_COLON: return ":";
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
    case TOKEN_LT: return "<";
    case TOKEN_LT_EQ: return "<=";
    case TOKEN_LT_LT: return "<<";
    case TOKEN_LT_LT_EQ: return "<<=";
    case TOKEN_GT: return ">";
    case TOKEN_GT_GT: return ">>";
    case TOKEN_GT_EQ: return ">=";
    case TOKEN_GT_GT_EQ: return ">>=";
    case TOKEN_OPEN_PAREN: return "(";
    case TOKEN_CLOSE_PAREN: return ")";
    case TOKEN_OPEN_BRACKET: return "[";
    case TOKEN_CLOSE_BRACKET: return "]";
    case TOKEN_OPEN_BRACE: return "{";
    case TOKEN_CLOSE_BRACE: return "}";
    case TOKEN_COMMA: return ",";
    case TOKEN_DOT: return ".";
    case TOKEN_DOT_DOT: return "..";
    case TOKEN_DOT_DOT_DOT: return "...";
    case TOKEN_SEMICOLON: return ";";
    case TOKEN_ASTERISK: return "*";
    case TOKEN_QUESTION_MARK: return "?";
    case TOKEN_PERCENT: return "%";
    case TOKEN_FWSLASH: return "/";
    case TOKEN_BWSLASH: return "\\";
    case TOKEN_HASH: return "#";
    case TOKEN_EQ: return "=";
    default: warning("Unhandled token kind.");
    }
    return "";
}

char* token_to_str(Token token) { return strf("%s :: %s", token.value, token_kind_to_str(token.kind)); }

void print_token(Token token) { info("%s %s", token_kind_to_str(token.kind), token.value); }
void print_tokens(Token_Array token_array) {
    info("Printing tokens..");
    for (s64 i = 0; i < token_array.count; i += 1) {
        print_token(token_array.data[i]);
    }
}

bool is_valid_identifier(u8 c) { return isalnum(c) || c == '_'; }
bool is_valid_digit(u8 c) { return isdigit(c) || c == '.' || c == '_' || c == 'e' || c == 'x'; }
u64  get_line_pos(Lexer_Context* lctx) { return lctx->current_char_in_stream - lctx->position_of_newline; }
int  get_keyword_index(char* identifier) {
    for (s64 i = 0; i < KEYWORD_COUNT; ++i) {
        if (identifier == keywords[i]) {
            return i;
        }
    }
    return -1;
}

Token_Kind get_identifier_kind(char* identifier) {
    switch (get_keyword_index(identifier)) {
    case KEY_LINK: return TOKEN_LINK;
    case KEY_TYPE: return TOKEN_TYPE;
    case KEY_TRUE: return TOKEN_TRUE;
    case KEY_FALSE: return TOKEN_FALSE;
    case KEY_DEF: return TOKEN_DEF;
    case KEY_DO: return TOKEN_DO;
    case KEY_DEFER: return TOKEN_DEFER;
    case KEY_EXTERN: return TOKEN_EXTERN;
    case KEY_LOAD: return TOKEN_LOAD;
    case KEY_CAST: return TOKEN_CAST;
    case KEY_SIZEOF: return TOKEN_SIZEOF;
    case KEY_IF: return TOKEN_IF;
    case KEY_ELSE: return TOKEN_ELSE;
    case KEY_FOR: return TOKEN_FOR;
    case KEY_WHILE: return TOKEN_WHILE;
    case KEY_RETURN: return TOKEN_RETURN;
    case KEY_STRUCT: return TOKEN_STRUCT;
    case KEY_ENUM: return TOKEN_ENUM;
    case KEY_BREAK: return TOKEN_BREAK;
    case KEY_CONTINUE: return TOKEN_CONTINUE;
    case KEY_AS: return TOKEN_AS;
    }
    return TOKEN_IDENTIFIER;
}

Token_Array make_token_array() {
    Token_Array l;
    l.count     = 0;
    l.allocated = TOKEN_ARRAY_STARTING_ALLOC;
    l.data      = xmalloc(l.allocated * sizeof(Token));
    l.info      = xmalloc(l.allocated * sizeof(Token_Info));
    return l;
}

void token_array_append(Token_Array* l, Token_Fat t) {
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(Token));
        l->info = xrealloc(l->info, l->allocated * sizeof(Token_Info));
    }

    Token token;
    token.id    = l->count;
    token.kind  = t.kind;
    token.value = t.value;

    Token_Info info;
    info.line_pos   = t.line_pos;
    info.col_pos    = t.col_pos;
    info.line_start = t.line_start;

    l->data[l->count] = token;
    l->info[l->count] = info;

    l->count += 1;
}

Token      token_array_at(Token_Array token_array, s64 token_index) { return token_array.data[token_index]; }
Token_Info token_array_get_info_of(Token_Array token_array, s64 token_id) { return token_array.info[token_id]; }
