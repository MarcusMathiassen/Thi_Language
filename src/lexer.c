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

#include "lexer.h"

#include "constants.h"
#include "typedefs.h"
#include "utility.h" // info, warning, xmalloc, xrealloc
#include <assert.h>  // assert
#include <ctype.h>   // isalnum, isdigit
#include <string.h>  // strncmp

//------------------------------------------------------------------------------
//                              lexer.c
//------------------------------------------------------------------------------

typedef enum {
    KEY_IN,
    KEY_DEF,
    KEY_LINK,
    KEY_TYPE,
    KEY_TRUE,
    KEY_FALSE,
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
    KEY_ENUM,
    KEY_BREAK,
    KEY_CONTINUE,
    KEY_AS,
    KEY_IS,
    KEY_FALLTHROUGH,
    KEY_TYPEOF,
    __KEY_COUNT__
} Keyword_Kind;

typedef struct
{
    s64   len;
    char* str;
} Intern;

typedef struct
{
    Intern* data;
    s64     count;
    s64     allocated;
} Intern_Array;

typedef struct
{
    char*        file;
    char*        stream;
    char*        position_of_newline;
    char*        start_of_line;
    s64          line_count;
    s64          comment_count;
    s64          previous_indentation_level;
    s64          current_indentation_level;
    Intern_Array interns;
    char*        keywords[__KEY_COUNT__];
} Lexer_Context;

//------------------------------------------------------------------------------
//                               Lexer Functions
//------------------------------------------------------------------------------

Token get_token(Lexer_Context* ctx);
bool  is_valid_digit(u8 c);
bool  is_valid_identifier(u8 c);
int   get_keyword_index(char* identifier);
Token_Kind
get_identifier_kind(char* identifier);
void         token_array_append(Token_Array* l, Token t);
Token_Array  make_token_array(void);
Intern_Array make_intern_array(void);

void  intern_array_append(Intern_Array* l, Intern intern);
char* intern_range(Intern_Array* intern_array, char* start, char* end);
char* intern(Intern_Array* intern_array, char* str);

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* STATIC_KEYWORDS_ARRAY[__KEY_COUNT__] = {
    "in",
    "def",
    "link",
    "type",
    "true",
    "false",
    "defer",
    "extern",
    "load",
    "cast",
    "sizeof",
    "if",
    "else",
    "for",
    "while",
    "return",
    "enum",
    "break",
    "continue",
    "as",
    "is",
    "fallthrough",
    "typeof",
};

void lexer_test(void) {
    // char* source =
        // "type v2\n    x: f32\n    y: f32\n    core()\n        return 1\n";
    // Lexed_File lf     = generate_tokens_from_source(source);
    // Token*     tokens = lf.tokens.data;
    // // info(source);
    // // print_tokens(lf.tokens);
    // assert(tokens[0].kind == TOKEN_TYPE);         // type
    // assert(tokens[1].kind == TOKEN_IDENTIFIER);   // v2
    // assert(tokens[2].kind == TOKEN_BLOCK_START);  //
    // assert(tokens[3].kind == TOKEN_IDENTIFIER);   // x
    // assert(tokens[4].kind == TOKEN_COLON);        // :
    // assert(tokens[5].kind == TOKEN_IDENTIFIER);   // f32
    // assert(tokens[6].kind == TOKEN_IDENTIFIER);   // y
    // assert(tokens[7].kind == TOKEN_COLON);        // :
    // assert(tokens[8].kind == TOKEN_IDENTIFIER);   // f32
    // assert(tokens[9].kind == TOKEN_IDENTIFIER);   // core
    // assert(tokens[10].kind == TOKEN_OPEN_PAREN);  // (
    // assert(tokens[11].kind == TOKEN_CLOSE_PAREN); // )
    // assert(tokens[12].kind == TOKEN_BLOCK_START); //
    // assert(tokens[13].kind == TOKEN_RETURN);      // return
    // assert(tokens[14].kind == TOKEN_INTEGER);     // 1
    // assert(tokens[15].kind == TOKEN_BLOCK_END);   //
    // assert(tokens[16].kind == TOKEN_BLOCK_END);   //
    // assert(tokens[17].kind == TOKEN_EOF);         //

    // lf     = generate_tokens_from_source("0.3453 1e3 0x043 'x' 100_000 100_000.00");
    // tokens = lf.tokens.data;
    // assert(tokens[0].kind == TOKEN_FLOAT);
    // assert(tokens[1].kind == TOKEN_INTEGER);
    // assert(tokens[2].kind == TOKEN_HEX);
    // assert(tokens[3].kind == TOKEN_CHAR);
    // assert(tokens[4].kind == TOKEN_INTEGER);
    // assert(tokens[5].kind == TOKEN_FLOAT);
}


#define LEXER_ERROR(x) error("[%s:%d:%d] %s", ctx.file, token.line_pos, token.col_pos, x)
Lexed_File generate_tokens_from_file(char* file) {

    char* source = get_file_content(file);

    Lexer_Context ctx;
    ctx.file                       = file;
    ctx.stream                     = source;
    ctx.position_of_newline        = source;
    ctx.start_of_line              = source;
    ctx.line_count                 = 1;
    ctx.comment_count              = 0;
    ctx.previous_indentation_level = 0;
    ctx.current_indentation_level  = 0;
    ctx.interns                    = make_intern_array();

    for (s64 i = 0; i < __KEY_COUNT__; ++i)
        ctx.keywords[i] = intern(&ctx.interns, STATIC_KEYWORDS_ARRAY[i]);

    Token_Array tokens = make_token_array();

    double start_time = get_time();

    Token token;
    token.kind = TOKEN_UNKNOWN;
    while (token.kind != TOKEN_EOF) {

        Token last_token = token;
        token = get_token(&ctx);

        if (
            last_token.kind == TOKEN_NEWLINE && 
            token.kind != TOKEN_NEWLINE && 
            token.kind != TOKEN_COMMENT) {
            if (token.col_pos % DEFAULT_INDENT_LEVEL != 0) LEXER_ERROR("indentation error.");
            ctx.current_indentation_level = token.col_pos;
            warning("token.col_pos: %d", token.col_pos);
        }

        if (ctx.current_indentation_level > ctx.previous_indentation_level) {
            Token t;
            t.kind                         = TOKEN_BLOCK_START;
            t.value                        = "{";
            t.line_pos                     = ctx.line_count;
            t.col_pos                      = ctx.stream - ctx.position_of_newline;
            ctx.previous_indentation_level = ctx.current_indentation_level;
            token_array_append(&tokens, t);
        } 
        while (ctx.current_indentation_level < ctx.previous_indentation_level) {
            Token t;
            t.kind     = TOKEN_BLOCK_END;
            t.value    = "}";
            t.line_pos = ctx.line_count;
            t.col_pos  = ctx.stream - ctx.position_of_newline;
            ctx.previous_indentation_level -= DEFAULT_INDENT_LEVEL;
            token_array_append(&tokens, t);
        }
        
        if (token.kind != TOKEN_UNKNOWN) 
            token_array_append(&tokens, token);
        
    }

    Lexed_File lf;
    lf.tokens   = tokens;
    lf.seconds  = get_time() - start_time;
    lf.comments = ctx.comment_count;
    lf.lines    = ctx.line_count;

    return lf;
}

Lexed_File generate_tokens_from_source(char* source) {
    Lexer_Context ctx;
    ctx.stream                     = source;
    ctx.position_of_newline        = source;
    ctx.start_of_line              = source;
    ctx.line_count                 = 1;
    ctx.comment_count              = 0;
    ctx.previous_indentation_level = 0;
    ctx.current_indentation_level  = 0;
    ctx.interns                    = make_intern_array();

    for (s64 i = 0; i < __KEY_COUNT__; ++i)
        ctx.keywords[i] = intern(&ctx.interns, STATIC_KEYWORDS_ARRAY[i]);

    Token_Array tokens = make_token_array();

    double start_time = get_time();

    Token token;
    token.kind = TOKEN_UNKNOWN;
    while (token.kind != TOKEN_EOF) {

        Token last_token = token;
        token = get_token(&ctx);

        if (
            last_token.kind == TOKEN_NEWLINE && 
            token.kind != TOKEN_NEWLINE && 
            token.kind != TOKEN_COMMENT) {
            if (token.col_pos % DEFAULT_INDENT_LEVEL != 0) LEXER_ERROR("indentation error.");
            ctx.current_indentation_level = token.col_pos;
            warning("token.col_pos: %d", token.col_pos);
        }

        if (ctx.current_indentation_level > ctx.previous_indentation_level) {
            Token t;
            t.kind                         = TOKEN_BLOCK_START;
            t.value                        = "{";
            t.line_pos                     = ctx.line_count;
            t.col_pos                      = ctx.stream - ctx.position_of_newline;
            ctx.previous_indentation_level = ctx.current_indentation_level;
            token_array_append(&tokens, t);
        } 
        while (ctx.current_indentation_level < ctx.previous_indentation_level) {
            Token t;
            t.kind     = TOKEN_BLOCK_END;
            t.value    = "}";
            t.line_pos = ctx.line_count;
            t.col_pos  = ctx.stream - ctx.position_of_newline;
            ctx.previous_indentation_level -= DEFAULT_INDENT_LEVEL;
            token_array_append(&tokens, t);
        }
        
        if (token.kind != TOKEN_UNKNOWN) 
            token_array_append(&tokens, token);
        
    }

    Lexed_File lf;
    lf.tokens   = tokens;
    lf.seconds  = get_time() - start_time;
    lf.comments = ctx.comment_count;
    lf.lines    = ctx.line_count;

    return lf;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

#define CASE_SINGLE_TOKEN(c1, t_kind) \
    case c1:                          \
        token.kind = t_kind;          \
        ++c;

#define skip_whitespace(c)          \
    while (*c == ' ' || *c == '\t') \
        ++c;

#define skip_comment(c) \
    while (*c != '\n')  \
        ++c;

Token get_token(Lexer_Context* ctx) {
    char* c = ctx->stream;

entry:
    skip_whitespace(c);

    Token token;
    token.kind     = TOKEN_UNKNOWN;
    token.value    = c;
    ctx->start_of_line = c;
    token.col_pos  = ctx->start_of_line - ctx->position_of_newline;
    token.line_pos = ctx->line_count;

    switch (*c) {
    default: break;
    case '#':
        // token.value = c;
        skip_comment(c);
        ctx->comment_count += 1;
        // token.kind = TOKEN_COMMENT;
        goto entry;
    case '\n':
        token.kind = TOKEN_NEWLINE;
        ++c; // skip the newline
        ctx->position_of_newline = c;
        ++ctx->line_count;
        goto end;
    }

    switch (*c) {
    default: ERROR_UNHANDLED_KIND(strf("%c", *c));

        // case '\r': /* fallthrough */
        // case '\t': {
        //     // Skip whitespace
        //     bool has_newline = false;
        // skip:
        //     while (*c == ' ' || *c == '\n' || *c == '\r' || *c == '\t') {
        //         if (*c == '\n') {
        //             has_newline = true;
        //             ctx->line_count += 1;
        //             ctx->position_of_newline = c;
        //         }
        //         ++c;
        //     }
        //     // HACK
        //     if (*c == '#') {
        //         ++c;
        //         while (*c != '\n') {
        //             ++c;
        //         }
        //         ctx->comment_count += 1;
        //         goto skip;
        //     }

        //     ctx->start_of_line = c;
        //     if (has_newline) {
        //         ctx->current_indentation_level = c - ctx->position_of_newline - 1;
        //     }
        // } break;

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
        CASE_SINGLE_TOKEN('{', TOKEN_BLOCK_START);
        break;
        CASE_SINGLE_TOKEN('}', TOKEN_BLOCK_END);
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
        CASE_SINGLE_TOKEN(';', TOKEN_TERMINAL);
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
            CASE_SINGLE_TOKEN('/', TOKEN_ASTERISK_FWSLASH);
            break;
            CASE_SINGLE_TOKEN('=', TOKEN_ASTERISK_EQ);
            break;
        }
        break;

        CASE_SINGLE_TOKEN('|', TOKEN_PIPE);
        switch (*c) {
            CASE_SINGLE_TOKEN('>', TOKEN_PIPE_GT);
            break;
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
        if (isdigit(*c) || *c == '.') {
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
    }

end:
    token.value = intern_range(&ctx->interns, token.value, c);
    if (token.kind == TOKEN_IDENTIFIER) {
        s64 i = 0;
        while (i < __KEY_COUNT__) {
            if (token.value == ctx->keywords[i]) {
                break;
            }
            i += 1;
        }
        // clang-format off
        switch (i) {
        case KEY_IN:          token.kind = TOKEN_IN;          break;
        case KEY_DEF:         token.kind = TOKEN_DEF;         break;
        case KEY_LINK:        token.kind = TOKEN_LINK;        break;
        case KEY_TYPE:        token.kind = TOKEN_TYPE;        break;
        case KEY_TRUE:        token.kind = TOKEN_TRUE;        break;
        case KEY_FALSE:       token.kind = TOKEN_FALSE;       break;
        case KEY_DEFER:       token.kind = TOKEN_DEFER;       break;
        case KEY_EXTERN:      token.kind = TOKEN_EXTERN;      break;
        case KEY_LOAD:        token.kind = TOKEN_LOAD;        break;
        case KEY_CAST:        token.kind = TOKEN_CAST;        break;
        case KEY_SIZEOF:      token.kind = TOKEN_SIZEOF;      break;
        case KEY_IF:          token.kind = TOKEN_IF;          break;
        case KEY_ELSE:        token.kind = TOKEN_ELSE;        break;
        case KEY_FOR:         token.kind = TOKEN_FOR;         break;
        case KEY_WHILE:       token.kind = TOKEN_WHILE;       break;
        case KEY_RETURN:      token.kind = TOKEN_RETURN;      break;
        case KEY_ENUM:        token.kind = TOKEN_ENUM;        break;
        case KEY_BREAK:       token.kind = TOKEN_BREAK;       break;
        case KEY_CONTINUE:    token.kind = TOKEN_CONTINUE;    break;
        case KEY_AS:          token.kind = TOKEN_AS;          break;
        case KEY_IS:          token.kind = TOKEN_IS;          break;
        case KEY_FALLTHROUGH: token.kind = TOKEN_FALLTHROUGH; break;
        case KEY_TYPEOF:      token.kind = TOKEN_TYPEOF;      break;
        }
        // clang-format on
    } else if (token.kind == TOKEN_CHAR || token.kind == TOKEN_STRING) {
        ++c; // we skip the last '
    }

    ctx->stream = c;

    return token;
}

char* token_kind_to_str(Token_Kind kind) {
    // clang-format off
    switch (kind) {
    default: ERROR_UNHANDLED_KIND(strf("%d", kind));
    case TOKEN_UNKNOWN:           return "TOKEN_UNKNOWN";
    case TOKEN_EOF:               return "TOKEN_EOF";
    case TOKEN_COMMENT:           return "TOKEN_COMMENT";
    case TOKEN_BLOCK_START:       return "TOKEN_BLOCK_START";
    case TOKEN_BLOCK_END:         return "TOKEN_BLOCK_END";
    case TOKEN_TERMINAL:          return "TOKEN_TERMINAL";
    case TOKEN_WHITESPACE:        return "TOKEN_WHITESPACE";
    case TOKEN_NEWLINE:           return "TOKEN_NEWLINE";
    case TOKEN_IDENTIFIER:        return "TOKEN_IDENTIFIER";
    case TOKEN_IN:                return "in";
    case TOKEN_DEF:               return "def";
    case TOKEN_IS:                return "is";
    case TOKEN_AS:                return "as";
    case TOKEN_CAST:              return "cast";
    case TOKEN_TYPEOF:            return "typeof";
    case TOKEN_SIZEOF:            return "sizeof";
    case TOKEN_LINK:              return "link";
    case TOKEN_EXTERN:            return "extern";
    case TOKEN_LOAD:              return "load";
    case TOKEN_TRUE:              return "true";
    case TOKEN_FALSE:             return "false";
    case TOKEN_TYPE:              return "type";
    case TOKEN_FALLTHROUGH:       return "fallthrough";
    case TOKEN_DEFER:             return "defer";
    case TOKEN_IF:                return "if";
    case TOKEN_ELSE:              return "else";
    case TOKEN_FOR:               return "for";
    case TOKEN_WHILE:             return "while";
    case TOKEN_RETURN:            return "return";
    case TOKEN_ENUM:              return "enum";
    case TOKEN_BREAK:             return "break";
    case TOKEN_CONTINUE:          return "continue";
    case TOKEN_STRING:            return "TOKEN_STRING";
    case TOKEN_CHAR:              return "TOKEN_CHAR";
    case TOKEN_INTEGER:           return "TOKEN_INTEGER";
    case TOKEN_NUMBER:            return "TOKEN_NUMBER";
    case TOKEN_HEX:               return "TOKEN_HEX";
    case TOKEN_FLOAT:             return "TOKEN_FLOAT";
    case TOKEN_PIPE_PIPE:         return "||";
    case TOKEN_AND_AND:           return "&&";
    case TOKEN_PLUS_EQ:           return "+=";
    case TOKEN_MINUS_EQ:          return "-=";
    case TOKEN_FWSLASH_EQ:        return "/=";
    case TOKEN_HAT_EQ:            return "^=";
    case TOKEN_ASTERISK_EQ:       return "*=";
    case TOKEN_PIPE_EQ:           return "|=";
    case TOKEN_PERCENT_EQ:        return "%=";
    case TOKEN_AND_EQ:            return "&=";
    case TOKEN_PLUS:              return "+";
    case TOKEN_PLUS_PLUS:         return "++";
    case TOKEN_MINUS:             return "-";
    case TOKEN_MINUS_MINUS:       return "--";
    case TOKEN_MINUS_MINUS_MINUS: return "---";
    case TOKEN_EQ_EQ:             return "==";
    case TOKEN_EQ_GT:             return "=>";
    case TOKEN_BANG_EQ:           return "!=";
    case TOKEN_COLON:             return ":";
    case TOKEN_COLON_COLON:       return "::";
    case TOKEN_COLON_EQ:          return ":=";
    case TOKEN_RIGHT_ARROW:       return "->";
    case TOKEN_PIPE:              return "|";
    case TOKEN_TILDE:             return "~";
    case TOKEN_AT:                return "@";
    case TOKEN_DOLLAR_SIGN:       return "$";
    case TOKEN_HAT:               return "^";
    case TOKEN_BANG:              return "!";
    case TOKEN_AND:               return "&";
    case TOKEN_LT:                return "<";
    case TOKEN_LT_EQ:             return "<=";
    case TOKEN_LT_LT:             return "<<";
    case TOKEN_LT_LT_EQ:          return "<<=";
    case TOKEN_GT:                return ">";
    case TOKEN_GT_GT:             return ">>";
    case TOKEN_GT_EQ:             return ">=";
    case TOKEN_GT_GT_EQ:          return ">>=";
    case TOKEN_OPEN_PAREN:        return "(";
    case TOKEN_CLOSE_PAREN:       return ")";
    case TOKEN_OPEN_BRACKET:      return "[";
    case TOKEN_CLOSE_BRACKET:     return "]";
    case TOKEN_OPEN_BRACE:        return "{";
    case TOKEN_CLOSE_BRACE:       return "}";
    case TOKEN_COMMA:             return ",";
    case TOKEN_DOT:               return ".";
    case TOKEN_DOT_DOT:           return "..";
    case TOKEN_DOT_DOT_DOT:       return "...";
    case TOKEN_SEMICOLON:         return ";";
    case TOKEN_ASTERISK:          return "*";
    case TOKEN_ASTERISK_FWSLASH:  return "*/";
    case TOKEN_QUESTION_MARK:     return "?";
    case TOKEN_PERCENT:           return "%";
    case TOKEN_FWSLASH:           return "/";
    case TOKEN_FWSLASH_FWSLASH:   return "//";
    case TOKEN_FWSLASH_ASTERISK:  return "/*";
    case TOKEN_BWSLASH:           return "\\";
    case TOKEN_HASH:              return "#";
    case TOKEN_EQ:                return "=";
    case TOKEN_PIPE_GT:           return "|>";
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

char* token_to_str(Token token) {
    return strf("%s :: %s %lld:%lld", token.value, token_kind_to_str(token.kind), token.line_pos, token.col_pos);
}
void print_token(Token token) {
    info("%s %s", token_kind_to_str(token.kind), token.value);
}
void print_tokens(Token_Array tokens) {
    info("Printing tokens..");
    for (int i = 0; i < tokens.count; ++i)
        print_token(tokens.data[i]);
}

bool is_valid_identifier(u8 c) {
    return isalnum(c) || c == '_';
}
bool is_valid_digit(u8 c) {
    return isdigit(c) || c == '.' || c == '_' || c == 'e' || c == 'x';
}

Token_Array make_token_array() {
    Token_Array l;
    l.count     = 0;
    l.allocated = TOKEN_ARRAY_STARTING_ALLOC;
    l.data      = xmalloc(l.allocated * sizeof(Token));
    return l;
}

void token_array_append(Token_Array* l, Token t) {
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(Token));
    }
    l->data[l->count] = t;
    l->count += 1;
}

Intern_Array make_intern_array() {
    Intern_Array l;
    l.count     = 0;
    l.allocated = INTERN_ARRAY_STARTING_ALLOC;
    l.data      = xmalloc(l.allocated * sizeof(Intern));
    return l;
}

void intern_array_append(Intern_Array* l, Intern intern) {
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(Intern));
    }
    l->data[l->count] = intern;
    l->count += 1;
}

char* intern(Intern_Array* interns, char* str) {
    return intern_range(interns, str, str + strlen(str));
}

char* intern_range(Intern_Array* interns, char* start, char* end) {
    s64 len = end - start;

    for (s64 i = 0; i < interns->count; ++i) {
        Intern intern = (Intern)interns->data[i];
        if (intern.len == len && strncmp(intern.str, start, len) == 0) {
            return intern.str;
        }
    }

    char* str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = 0;

    Intern intern;
    intern.len = len;
    intern.str = str;
    intern_array_append(interns, intern);

    return str;
}
