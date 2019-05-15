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
    KEY_STRUCT,
    KEY_ENUM,
    KEY_BREAK,
    KEY_CONTINUE,
    KEY_AS,
    KEY_IS,
    KEY_FALLTHROUGH,
    KEY_TYPEOF,
    KEY_COUNT
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
    char*        stream;
    char*        position_of_newline;
    char*        start_of_line;
    s64          line_count;
    s64          comment_count;
    s64          previous_indentation_level;
    s64          current_indentation_level;
    Intern_Array interns;
    char*        keywords[KEY_COUNT];
} Lexer_Context;

//------------------------------------------------------------------------------
//                               Lexer Functions
//------------------------------------------------------------------------------

Token get_token(Lexer_Context* lctx);
bool  is_valid_digit(u8 c);
bool  is_valid_identifier(u8 c);
int   get_keyword_index(char* identifier);
Token_Kind
get_identifier_kind(char* identifier);
void token_array_append(Token_Array* l, Token t);
Token_Array
make_token_array();
Intern_Array
make_intern_array();

void  intern_array_append(Intern_Array* l, Intern intern);
char* intern_range(Intern_Array* intern_array, char* start, char* end);
char* intern(Intern_Array* intern_array, char* str);

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* STATIC_KEYWORDS_ARRAY[KEY_COUNT] = {
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
    "struct",
    "enum",
    "break",
    "continue",
    "as",
    "is",
    "fallthrough",
    "typeof",
};

void lexer_test(void) {
    char* source =
        "struct v2\n    x: f32\n    y: f32\n    core()\n        return 1\n";
    Lexed_File lf = generate_tokens_from_source(source);
    // info(source);
    // print_tokens(lf.tokens);
    assert(lf.tokens.data[0].kind == TOKEN_STRUCT);       // struct
    assert(lf.tokens.data[1].kind == TOKEN_IDENTIFIER);   // v2
    assert(lf.tokens.data[2].kind == TOKEN_BLOCK_START);  //
    assert(lf.tokens.data[3].kind == TOKEN_IDENTIFIER);   // x
    assert(lf.tokens.data[4].kind == TOKEN_COLON);        // :
    assert(lf.tokens.data[5].kind == TOKEN_IDENTIFIER);   // f32
    assert(lf.tokens.data[6].kind == TOKEN_IDENTIFIER);   // y
    assert(lf.tokens.data[7].kind == TOKEN_COLON);        // :
    assert(lf.tokens.data[8].kind == TOKEN_IDENTIFIER);   // f32
    assert(lf.tokens.data[9].kind == TOKEN_IDENTIFIER);   // core
    assert(lf.tokens.data[10].kind == TOKEN_OPEN_PAREN);  // (
    assert(lf.tokens.data[11].kind == TOKEN_CLOSE_PAREN); // )
    assert(lf.tokens.data[12].kind == TOKEN_BLOCK_START); //
    assert(lf.tokens.data[13].kind == TOKEN_RETURN);      // return
    assert(lf.tokens.data[14].kind == TOKEN_INTEGER);     // 1
    assert(lf.tokens.data[15].kind == TOKEN_BLOCK_END);   //
    assert(lf.tokens.data[16].kind == TOKEN_BLOCK_END);   //
    assert(lf.tokens.data[17].kind == TOKEN_EOF);         //
}

Lexed_File
generate_tokens_from_source(char* source) {
    Lexer_Context lctx;
    lctx.stream                     = source;
    lctx.position_of_newline        = source;
    lctx.start_of_line              = source;
    lctx.line_count                 = 1;
    lctx.comment_count              = 0;
    lctx.previous_indentation_level = 0;
    lctx.current_indentation_level  = 0;
    lctx.interns                    = make_intern_array();

    for (s64 i = 0; i < KEY_COUNT; ++i) {
        lctx.keywords[i] = intern(&lctx.interns, STATIC_KEYWORDS_ARRAY[i]);
    }

    Token_Array tokens = make_token_array();

    f64 start_time = get_time();

    while (true) {
        Token token = get_token(&lctx);

        if (lctx.current_indentation_level > lctx.previous_indentation_level) {
            Token t;
            t.kind                          = TOKEN_BLOCK_START;
            t.value                         = "";
            t.line_pos                      = lctx.line_count;
            t.col_pos                       = lctx.stream - lctx.position_of_newline;
            lctx.previous_indentation_level = lctx.current_indentation_level;
            token_array_append(&tokens, t);
        }

        while (lctx.current_indentation_level <
               lctx.previous_indentation_level) {
            Token t;
            t.kind     = TOKEN_BLOCK_END;
            t.value    = "";
            t.line_pos = lctx.line_count;
            t.col_pos  = lctx.stream - lctx.position_of_newline;
            lctx.previous_indentation_level -= 4;
            token_array_append(&tokens, t);
        }

        if (token.kind != TOKEN_UNKNOWN && token.kind != TOKEN_COMMENT) {
            token_array_append(&tokens, token);
        }
        if (token.kind == TOKEN_EOF) {
            break;
        }
    }

    Lexed_File lf;
    lf.tokens   = tokens;
    lf.lines    = lctx.line_count;
    lf.comments = lctx.comment_count;
    lf.seconds  = (get_time() - start_time) / 1e3;

    info("lexed %lld lines, %lld comments", lctx.line_count, lctx.comment_count);

    return lf;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

#define CASE_SINGLE_TOKEN(c1, t_kind) \
    case c1: token.kind = t_kind; ++c;

Token get_token(Lexer_Context* lctx) {
    char* c = lctx->stream;

    Token token;
    token.kind     = TOKEN_UNKNOWN;
    token.value    = c;
    token.line_pos = lctx->line_count;
    token.col_pos  = c - lctx->position_of_newline;

    switch (*c) {
    case '#': {
        ++c;
        while (*c != '\n') {
            ++c;
        }
        lctx->comment_count += 1;
        token.kind = TOKEN_COMMENT;
    } break;
    case ' ':  /* fallthrough */
    case '\n': /* fallthrough */
    case '\r': /* fallthrough */
    case '\t': {
        // Skip whitespace

        bool has_newline = false;
    skip:
        while (*c == ' ' || *c == '\n' || *c == '\r' || *c == '\t') {
            if (*c == '\n') {
                has_newline = true;
                lctx->line_count += 1;
                lctx->position_of_newline = c;
            }
            ++c;
        }
        // HACK
        if (*c == '#') {
            ++c;
            while (*c != '\n') {
                ++c;
            }
            lctx->comment_count += 1;
            goto skip;
        }

        lctx->start_of_line = c;
        if (has_newline) {
            lctx->current_indentation_level = c - lctx->position_of_newline - 1;
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
            CASE_SINGLE_TOKEN('/', TOKEN_ASTERISK_FWSLASH)
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
    }

    token.value = intern_range(&lctx->interns, token.value, c);
    if (token.kind == TOKEN_IDENTIFIER) {
        s64 i = 0;
        while (i < KEY_COUNT) {
            if (token.value == lctx->keywords[i]) {
                break;
            }
            i += 1;
        }
        // clang-format off
        switch (i) {
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
        case KEY_STRUCT:      token.kind = TOKEN_STRUCT;      break;
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

    lctx->stream = c;

    return token;
}

char* token_kind_to_str(Token_Kind kind) {
    // clang-format off
    switch (kind) {
    case TOKEN_UNKNOWN:           return "TOKEN_UNKNOWN";
    case TOKEN_EOF:               return "TOKEN_EOF";
    case TOKEN_COMMENT:           return "TOKEN_COMMENT";
    case TOKEN_BLOCK_START:       return "TOKEN_BLOCK_START";
    case TOKEN_BLOCK_END:         return "TOKEN_BLOCK_END";
    case TOKEN_WHITESPACE:        return "TOKEN_WHITESPACE";
    case TOKEN_NEWLINE:           return "TOKEN_NEWLINE";
    case TOKEN_IDENTIFIER:        return "TOKEN_IDENTIFIER";
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
    case TOKEN_STRUCT:            return "struct";
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
    default: warning("Unhandled token kind.");
    }
    // clang-format on
    return "";
}

char* token_to_str(Token token) {
    return strf("%s :: %s %lld:%lld", token.value, token_kind_to_str(token.kind), token.line_pos, token.col_pos);
}
void print_token(Token token) {
    if (token.kind == TOKEN_IDENTIFIER || token.kind == TOKEN_INTEGER)
        info("%s", token.value);
    else
        info("%s", token_kind_to_str(token.kind));
}
void print_tokens(Token_Array tokens) {
    info("Printing tokens..");
    for (s64 i = 0; i < tokens.count; i += 1) {
        print_token(tokens.data[i]);
    }
}

bool is_valid_identifier(u8 c) {
    return isalnum(c) || c == '_';
}
bool is_valid_digit(u8 c) {
    return isdigit(c) || c == '.' || c == '_' || c == 'e' || c == 'x';
}

Token_Array
make_token_array() {
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

Intern_Array
make_intern_array() {
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
