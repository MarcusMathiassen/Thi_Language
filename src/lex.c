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

#include "lex.h"
#include "common.h"
#include "utility.h" // info, warning, xmalloc, xrealloc, xstrlen
#include <ctype.h> // isalnum, isdigit
#include <string.h>  // strncmp
#include <stdlib.h>  // malloc

typedef struct
{
    s64 len;
    char* str;
} Intern;

typedef struct
{
    Intern* data;
    s64 count;
    s64 allocated;
} Intern_Array;

Token_Array make_token_array(void);
void token_array_append(Token_Array* l, Token t);

Intern_Array make_intern_array(void);
void intern_array_append(Intern_Array* l, Intern intern);

char* intern(Intern_Array* interns, char* str);
char* intern_range(Intern_Array* interns, char* start, char* end);

// @Volatile
typedef enum {
    KEY_ASM,
    KEY_DEF,
    KEY_LINK,
    KEY_LOAD,
    KEY_TRUE,
    KEY_FALSE,
    KEY_DEFER,
    KEY_EXTERN,
    KEY_SIZEOF,
    KEY_TYPEOF,
    KEY_IF,
    KEY_ELSE,
    KEY_FOR,
    KEY_WHILE,
    KEY_IN,
    KEY_RETURN,
    KEY_ENUM,
    KEY_STRUCT,
    KEY_BREAK,
    KEY_CONTINUE,
    KEY_FALLTHROUGH,
    KEY_AS,
    KEY_IS,
    _KEY_COUNT_,
} Keyword_Kind;


typedef enum {
    STATE_END,
    STATE_IDENTIFIER,
    STATE_OPERATOR,
    STATE_NUMBER,
    STATE_STRING,
    STATE_CHAR,
    STATE_COMMENT,
    STATE_NEWLINE,
    _STATE_COUNT_
} State_Kind;

static State_Kind states[] = {
    ['\0']    = STATE_END,
    ['\n']    = STATE_NEWLINE,
    ['!']     = STATE_OPERATOR,
    ['"']     = STATE_STRING,
    ['#']     = STATE_COMMENT,
    ['$']     = STATE_OPERATOR,
    ['%']     = STATE_OPERATOR,
    ['&']     = STATE_OPERATOR,
    ['\'']    = STATE_CHAR,
    ['(']     = STATE_OPERATOR,
    [')']     = STATE_OPERATOR,
    ['*']     = STATE_OPERATOR,
    ['+']     = STATE_OPERATOR,
    [',']     = STATE_OPERATOR,
    ['-']     = STATE_OPERATOR,
    ['.']     = STATE_OPERATOR,
    ['/']     = STATE_OPERATOR,
    ['0']     = STATE_NUMBER,
    ['1']     = STATE_NUMBER,
    ['2']     = STATE_NUMBER,
    ['3']     = STATE_NUMBER,
    ['4']     = STATE_NUMBER,
    ['5']     = STATE_NUMBER,
    ['6']     = STATE_NUMBER,
    ['7']     = STATE_NUMBER,
    ['8']     = STATE_NUMBER,
    ['9']     = STATE_NUMBER,
    [':']     = STATE_OPERATOR,
    [';']     = STATE_OPERATOR,
    ['<']     = STATE_OPERATOR,
    ['=']     = STATE_OPERATOR,
    ['>']     = STATE_OPERATOR,
    ['?']     = STATE_OPERATOR,
    ['@']     = STATE_OPERATOR,
    ['A']     = STATE_IDENTIFIER,
    ['B']     = STATE_IDENTIFIER,
    ['C']     = STATE_IDENTIFIER,
    ['D']     = STATE_IDENTIFIER,
    ['E']     = STATE_IDENTIFIER,
    ['F']     = STATE_IDENTIFIER,
    ['G']     = STATE_IDENTIFIER,
    ['H']     = STATE_IDENTIFIER,
    ['I']     = STATE_IDENTIFIER,
    ['J']     = STATE_IDENTIFIER,
    ['K']     = STATE_IDENTIFIER,
    ['L']     = STATE_IDENTIFIER,
    ['M']     = STATE_IDENTIFIER,
    ['N']     = STATE_IDENTIFIER,
    ['O']     = STATE_IDENTIFIER,
    ['P']     = STATE_IDENTIFIER,
    ['Q']     = STATE_IDENTIFIER,
    ['R']     = STATE_IDENTIFIER,
    ['S']     = STATE_IDENTIFIER,
    ['T']     = STATE_IDENTIFIER,
    ['U']     = STATE_IDENTIFIER,
    ['V']     = STATE_IDENTIFIER,
    ['W']     = STATE_IDENTIFIER,
    ['X']     = STATE_IDENTIFIER,
    ['Y']     = STATE_IDENTIFIER,
    ['Z']     = STATE_IDENTIFIER,
    ['[']     = STATE_OPERATOR,
    ['\\']    = STATE_OPERATOR,
    [']']     = STATE_OPERATOR,
    ['^']     = STATE_OPERATOR,
    ['_']     = STATE_IDENTIFIER,
    ['`']     = STATE_STRING,
    ['a']     = STATE_IDENTIFIER,
    ['b']     = STATE_IDENTIFIER,
    ['c']     = STATE_IDENTIFIER,
    ['d']     = STATE_IDENTIFIER,
    ['e']     = STATE_IDENTIFIER,
    ['f']     = STATE_IDENTIFIER,
    ['g']     = STATE_IDENTIFIER,
    ['h']     = STATE_IDENTIFIER,
    ['i']     = STATE_IDENTIFIER,
    ['j']     = STATE_IDENTIFIER,
    ['k']     = STATE_IDENTIFIER,
    ['l']     = STATE_IDENTIFIER,
    ['m']     = STATE_IDENTIFIER,
    ['n']     = STATE_IDENTIFIER,
    ['o']     = STATE_IDENTIFIER,
    ['p']     = STATE_IDENTIFIER,
    ['q']     = STATE_IDENTIFIER,
    ['r']     = STATE_IDENTIFIER,
    ['s']     = STATE_IDENTIFIER,
    ['t']     = STATE_IDENTIFIER,
    ['u']     = STATE_IDENTIFIER,
    ['v']     = STATE_IDENTIFIER,
    ['w']     = STATE_IDENTIFIER,
    ['x']     = STATE_IDENTIFIER,
    ['y']     = STATE_IDENTIFIER,
    ['z']     = STATE_IDENTIFIER,
    ['{']     = STATE_OPERATOR,
    ['|']     = STATE_OPERATOR,
    ['}']     = STATE_OPERATOR,
    ['~']     = STATE_OPERATOR,
};

#define is_valid_identifier(c)  isalnum(c) || c == '_'
#define is_valid_digit(c)       isdigit(c) || c == '_' || c == '.' || c == 'e' || c == 'x'

#define CASE_SINGLE_TOKEN(c1, t_kind) \
    case c1:                          \
        kind = t_kind;                \
        ++c;

#define skip_whitespace(c)          \
    while (*c == ' ' || *c == '\t') \
        ++c;

#define skip_comment(c) \
    while (*c != '\n')  \
        ++c;

Lexed_File lex(char* file) {
    push_timer(strf("%s: %s", (char*)__func__, file));

    // @Volatile
    static char* keywords_as_strings[_KEY_COUNT_] = {
        "asm",
        "def",
        "link",
        "load",
        "true",
        "false",
        "defer",
        "extern",
        "sizeof",
        "typeof",
        "if",
        "else",
        "for",
        "while",
        "in",
        "return",
        "enum",
        "struct",
        "break",
        "continue",
        "fallthrough",
        "as",
        "is",
    };
    
    static char* interned_keywords[_KEY_COUNT_];

    Intern_Array interns = make_intern_array();
    
    // @Performance:
    //      This only needs to run once, but is run every time.
    foreach(i, _KEY_COUNT_)
        interned_keywords[i] = intern(&interns, keywords_as_strings[i]);

    // Start parsing tokens
    Token_Kind kind = _TOKEN_COUNT_;
    Token_Kind last_token_kind;
    Token_Array tokens = make_token_array();
    char* c = get_file_content(file);
    char* position_of_newline = c;
    s64 current_indentation_level = 0 ;
    s64 previous_indentation_level = 0 ;

    s64 comments = 0;
    s64 line = 0;
    s64 col = 0;

    do {

        last_token_kind = kind;

        skip_whitespace(c);

        char* start = c;
        char* end = start;

        // Set the indenation level
        col = start - position_of_newline;

        State_Kind state = states[(s32)*c];

        switch(state) {
        ERROR_UNHANDLED_KIND(strf("%d", state));
        
        case STATE_END: kind = TOKEN_EOF; break;
        case STATE_IDENTIFIER: {
            kind = TOKEN_IDENTIFIER;

            ++c; // we can skip the first one, we already know it is one
            while (is_valid_identifier(*c))
                ++c;

            // after parsing an identifier, check if its a keywords
            char* value = intern_range(&interns, start, c);
            foreach(i, _KEY_COUNT_) {
                if (interned_keywords[i] == value) {
                    kind = i + _TOKEN_KEYWORDS_START_+1; // keywords start at _TOKEN_KEYWORDS_START_+1 @Volatile
                    break;
                }
            }
            start = value; // @Hack: we're using something in a way it was ot ment to be used
        } break;

        case STATE_OPERATOR: {
            kind = (Token_Kind)*c;
            switch (*c) {
                default: 
                    ++c; 
                    break;

                CASE_SINGLE_TOKEN('{', TOKEN_BLOCK_START);
                break;
                CASE_SINGLE_TOKEN('}', TOKEN_BLOCK_END);
                break;

                CASE_SINGLE_TOKEN('/', '/');
                switch (*c) {
                    CASE_SINGLE_TOKEN('=', TOKEN_FWSLASH_EQ);
                    break;
                }
                break;
                CASE_SINGLE_TOKEN('!', '!');
                switch (*c) {
                    CASE_SINGLE_TOKEN('=', TOKEN_BANG_EQ);
                    break;
                }
                break;
                CASE_SINGLE_TOKEN('*', '*');
                switch (*c) {
                    CASE_SINGLE_TOKEN('=', TOKEN_ASTERISK_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('|', '|');
                switch (*c) {
                    CASE_SINGLE_TOKEN('>', TOKEN_PIPE_GT);
                    break;
                    CASE_SINGLE_TOKEN('|', TOKEN_PIPE_PIPE);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_PIPE_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('<', '<');
                switch (*c) {
                    CASE_SINGLE_TOKEN('<', TOKEN_LT_LT);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_LT_EQ);
                    break;
                }
                break;
                CASE_SINGLE_TOKEN('>', '>');
                switch (*c) {
                    CASE_SINGLE_TOKEN('>', TOKEN_GT_GT);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_GT_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('.', '.');
                switch (*c) {
                    CASE_SINGLE_TOKEN('.', TOKEN_DOT_DOT);
                    switch (*c) {
                        CASE_SINGLE_TOKEN('.', TOKEN_DOT_DOT_DOT);
                        break;
                    }
                    break;
                }
                break;

                CASE_SINGLE_TOKEN(':', ':');
                switch (*c) {
                    CASE_SINGLE_TOKEN(':', TOKEN_COLON_COLON);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_COLON_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('-', '-');
                switch (*c) {
                    CASE_SINGLE_TOKEN('-', TOKEN_MINUS_MINUS);
                    switch (*c) {
                        CASE_SINGLE_TOKEN('-', TOKEN_MINUS_MINUS_MINUS);
                        break;
                    }
                    break;
                    CASE_SINGLE_TOKEN('>', TOKEN_MINUS_GT);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_MINUS_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('+', '+');
                switch (*c) {
                    CASE_SINGLE_TOKEN('+', TOKEN_PLUS_PLUS);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_PLUS_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('%', '%');
                switch (*c) {
                    CASE_SINGLE_TOKEN('=', TOKEN_PERCENT_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('&', '&');
                switch (*c) {
                    CASE_SINGLE_TOKEN('&', TOKEN_AND_AND);
                    break;
                    CASE_SINGLE_TOKEN('=', TOKEN_AND_EQ);
                    break;
                }
                break;

                CASE_SINGLE_TOKEN('=', '=');
                switch (*c) {
                    CASE_SINGLE_TOKEN('=', TOKEN_EQ_EQ);
                    break;
                    CASE_SINGLE_TOKEN('>', TOKEN_EQ_GT);
                    break;
                }
            }
            end = c;
        } break;
        case STATE_NUMBER:
        {
            bool is_hex = false;
            bool is_float = false;

            // Number: [0-9._]+e[0-9]+
            if (isdigit(*c) || *c == '.') {
                while (is_valid_digit(*c) || (is_hex && is_valid_identifier(*c))) {
                    // @Audit
                    if (*c == 'x') is_hex = true;
                    if (*c == '.') is_float = true;
                    ++c;
                }
            }
            end = c;
            kind = is_hex ? TOKEN_HEX : is_float ? TOKEN_FLOAT : TOKEN_INTEGER;
        } break;
        case STATE_STRING: {
            kind = TOKEN_STRING;
            start = ++c; // skip the first "
            while (*c != '"')
                ++c;
            end = c++; // skip the last "
        } break;
        case STATE_CHAR: {
            kind = TOKEN_CHAR;
            start = ++c; // skip the first '
            if (*c == '\\') c += 2;
            end = c++; // skip the last '
        } break;
        case STATE_COMMENT:
        {
            kind = TOKEN_COMMENT;
            start = ++c; start = ++c; // skip the hash
            skip_comment(c);
            end = c;
            ++comments;
        } break;
        case STATE_NEWLINE:
        {
           kind = TOKEN_NEWLINE;
            end = ++c; // skip the newline
            position_of_newline = c;
            ++line;
        } break;
        }

        if (last_token_kind == TOKEN_NEWLINE && kind != TOKEN_NEWLINE && kind != TOKEN_COMMENT && (col % DEFAULT_INDENT_LEVEL == 0)) {
                
            current_indentation_level = col;
            
            if (current_indentation_level > previous_indentation_level) {
                previous_indentation_level = current_indentation_level;
                token_array_append(&tokens, (Token){TOKEN_BLOCK_START, start, end, line, col});
            } else while (current_indentation_level < previous_indentation_level) {
                previous_indentation_level -= DEFAULT_INDENT_LEVEL;
                token_array_append(&tokens, (Token){TOKEN_BLOCK_END, start, end, line, col});
            }

            // else error("[%s:%d:%d] indentation error.", file, line, col);
        }


        xassert(kind != TOKEN_UNKNOWN);

        token_array_append(&tokens, (Token){kind, start, end, line, col});

    // } while(c && kind != TOKEN_EOF);
    } while(kind != TOKEN_EOF);

    // info("Printing tokens..");
    // foreach(i, tokens.count) {
    //     info("kind: %s value: '%s' start: %llu end: %llu", 
    //         ucolor(token_kind_to_str(tokens.data[i].kind)), 
    //         ucolor(token_value(tokens.data[i])),
    //         tokens.data[i].start, tokens.data[i].end);
    // }
    // error("fe");

    pop_timer();
    // error("%f seconds", tm->ms * 0.001);

    return (Lexed_File){tokens, line, comments};
}

void lexer_test(void) {

}

char* token_value(Token token) {
    return (
        token.kind > _BEGINNING_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_ && 
        token.kind < _END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_) ? token.start : strn(token.start, token.end);
}

char* token_kind_to_str(Token_Kind kind) {
    TASSERT_KIND_IN_RANGE(TOKEN, kind);
    switch (kind) {
    case TOKEN_UNKNOWN:           return "TOKEN_UNKNOWN";
    case TOKEN_EOF:               return "TOKEN_EOF";
    case TOKEN_WHITESPACE:        return "TOKEN_WHITESPACE";
    case TOKEN_BANG:              return "TOKEN_BANG";
    case TOKEN_DOUBLE_QUOTATION:  return "TOKEN_DOUBLE_QUOTATION";
    case TOKEN_HASH:              return "TOKEN_HASH";
    case TOKEN_DOLLAR_SIGN:       return "TOKEN_DOLLAR_SIGN";
    case TOKEN_PERCENT:           return "TOKEN_PERCENT";
    case TOKEN_AND:               return "TOKEN_AND";
    case TOKEN_SINGLE_QUOTATION:  return "TOKEN_SINGLE_QUOTATION";
    case TOKEN_OPEN_PAREN:        return "TOKEN_OPEN_PAREN";
    case TOKEN_CLOSE_PAREN:       return "TOKEN_CLOSE_PAREN";
    case TOKEN_ASTERISK:          return "TOKEN_ASTERISK";
    case TOKEN_PLUS:              return "TOKEN_PLUS";
    case TOKEN_COMMA:             return "TOKEN_COMMA";
    case TOKEN_MINUS:             return "TOKEN_MINUS";
    case TOKEN_DOT:               return "TOKEN_DOT";
    case TOKEN_FWSLASH:           return "TOKEN_FWSLASH";
    case TOKEN_COLON:             return "TOKEN_COLON";
    case TOKEN_SEMICOLON:         return "TOKEN_SEMICOLON";
    case TOKEN_LT:                return "TOKEN_LT";
    case TOKEN_EQ:                return "TOKEN_EQ";
    case TOKEN_GT:                return "TOKEN_GT";
    case TOKEN_QUESTION_MARK:     return "TOKEN_QUESTION_MARK";
    case TOKEN_AT:                return "TOKEN_AT";
    case TOKEN_OPEN_BRACKET:      return "TOKEN_OPEN_BRACKET";
    case TOKEN_BWSLASH:           return "TOKEN_BWSLASH";
    case TOKEN_CLOSE_BRACKET:     return "TOKEN_CLOSE_BRACKET";
    case TOKEN_HAT:               return "TOKEN_HAT";
    case TOKEN_BACK_TICK:         return "TOKEN_BACK_TICK";
    case TOKEN_OPEN_BRACE:        return "TOKEN_OPEN_BRACE";
    case TOKEN_PIPE:              return "TOKEN_PIPE";
    case TOKEN_CLOSE_BRACE:       return "TOKEN_CLOSE_BRACE";
    case TOKEN_TILDE:             return "TOKEN_TILDE";
    case TOKEN_NEWLINE:           return "TOKEN_NEWLINE";
    case TOKEN_COMMENT:           return "TOKEN_COMMENT";
    case TOKEN_LITERAL:           return "TOKEN_LITERAL";
    case TOKEN_BLOCK_START:       return "TOKEN_BLOCK_START";
    case TOKEN_BLOCK_END:         return "TOKEN_BLOCK_END";
    case TOKEN_INTEGER:           return "TOKEN_INTEGER";
    case TOKEN_FLOAT:             return "TOKEN_FLOAT";
    case TOKEN_HEX:               return "TOKEN_HEX";
    case TOKEN_STRING:            return "TOKEN_STRING";
    case TOKEN_CHAR:              return "TOKEN_CHAR";

    case _BEGINNING_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_:  return "_BEGINNING_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_";
    case _TOKEN_KEYWORDS_START_:  return "_TOKEN_KEYWORDS_START_";
    case TOKEN_ASM:               return "TOKEN_ASM";
    case TOKEN_DEF:               return "TOKEN_DEF";
    case TOKEN_LINK:              return "TOKEN_LINK";
    case TOKEN_LOAD:              return "TOKEN_LOAD";
    case TOKEN_TRUE:              return "TOKEN_TRUE";
    case TOKEN_FALSE:             return "TOKEN_FALSE";
    case TOKEN_DEFER:             return "TOKEN_DEFER";
    case TOKEN_EXTERN:            return "TOKEN_EXTERN";
    case TOKEN_SIZEOF:            return "TOKEN_SIZEOF";
    case TOKEN_TYPEOF:            return "TOKEN_TYPEOF";
    case TOKEN_IF:                return "TOKEN_IF";
    case TOKEN_ELSE:              return "TOKEN_ELSE";
    case TOKEN_FOR:               return "TOKEN_FOR";
    case TOKEN_WHILE:             return "TOKEN_WHILE";
    case TOKEN_IN:                return "TOKEN_IN";
    case TOKEN_RETURN:            return "TOKEN_RETURN";
    case TOKEN_ENUM:              return "TOKEN_ENUM";
    case TOKEN_STRUCT:            return "TOKEN_STRUCT";
    case TOKEN_BREAK:             return "TOKEN_BREAK";
    case TOKEN_CONTINUE:          return "TOKEN_CONTINUE";
    case TOKEN_FALLTHROUGH:       return "TOKEN_FALLTHROUGH";
    case TOKEN_AS:                return "TOKEN_AS";
    case TOKEN_IS:                return "TOKEN_IS";

    case TOKEN_IDENTIFIER:        return "TOKEN_IDENTIFIER";

    case _END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_: return "_END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_";

    case TOKEN_PIPE_PIPE:         return "TOKEN_PIPE_PIPE";
    case TOKEN_AND_AND:           return "TOKEN_AND_AND";
    case TOKEN_PLUS_EQ:           return "TOKEN_PLUS_EQ";
    case TOKEN_MINUS_EQ:          return "TOKEN_MINUS_EQ";
    case TOKEN_HAT_EQ:            return "TOKEN_HAT_EQ";
    case TOKEN_ASTERISK_EQ:       return "TOKEN_ASTERISK_EQ";
    case TOKEN_PIPE_EQ:           return "TOKEN_PIPE_EQ";
    case TOKEN_PERCENT_EQ:        return "TOKEN_PERCENT_EQ";
    case TOKEN_AND_EQ:            return "TOKEN_AND_EQ";
    case TOKEN_EQ_EQ:             return "TOKEN_EQ_EQ";
    case TOKEN_EQ_GT:             return "TOKEN_EQ_GT";
    case TOKEN_BANG_EQ:           return "TOKEN_BANG_EQ";
    case TOKEN_COLON_COLON:       return "TOKEN_COLON_COLON";
    case TOKEN_COLON_EQ:          return "TOKEN_COLON_EQ";
    case TOKEN_MINUS_GT:          return "TOKEN_MINUS_GT";
    case TOKEN_LT_EQ:             return "TOKEN_LT_EQ";
    case TOKEN_LT_LT:             return "TOKEN_LT_LT";
    case TOKEN_LT_LT_EQ:          return "TOKEN_LT_LT_EQ";
    case TOKEN_GT_EQ:             return "TOKEN_GT_EQ";
    case TOKEN_GT_GT:             return "TOKEN_GT_GT";
    case TOKEN_GT_GT_EQ:          return "TOKEN_GT_GT_EQ";
    case TOKEN_DOT_DOT:           return "TOKEN_DOT_DOT";
    case TOKEN_DOT_DOT_DOT:       return "TOKEN_DOT_DOT_DOT";
    case TOKEN_MINUS_MINUS:       return "TOKEN_MINUS_MINUS";
    case TOKEN_MINUS_MINUS_MINUS: return "TOKEN_MINUS_MINUS_MINUS";
    case TOKEN_PLUS_PLUS:         return "TOKEN_PLUS_PLUS";
    case TOKEN_FWSLASH_EQ:        return "TOKEN_FWSLASH_EQ";
    case TOKEN_PIPE_GT:           return "TOKEN_PIPE_GT";
    case _TOKEN_COUNT_:           return "_TOKEN_COUNT_";
    }
    UNREACHABLE;
    return NULL;
}

Token_Array make_token_array(void) {
    Token_Array l;
    l.count = 0;
    l.allocated = TOKEN_ARRAY_STARTING_ALLOC;
    l.data = xmalloc(l.allocated * sizeof(*l.data));
    return l;
}

void token_array_append(Token_Array* l, Token t) {
    if (l->count == l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(*l->data));
    }
    l->data[l->count++] = t;
}

Intern_Array make_intern_array(void) {
    Intern_Array l;
    l.count = 0;
    l.allocated = INTERN_ARRAY_STARTING_ALLOC;
    l.data = xmalloc(l.allocated * sizeof(Intern));
    return l;
}

void intern_array_append(Intern_Array* l, Intern intern) {
    if (l->count == l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(*l->data));
    }
    l->data[l->count++] = intern;
}

char* intern(Intern_Array* interns, char* str) {
    return intern_range(interns, str, str + xstrlen(str));
}

char* intern_range(Intern_Array* interns, char* start, char* end) {
    s64 len = end - start;

    foreach(i, interns->count) {
        Intern intern = interns->data[i];
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
