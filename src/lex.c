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
    STATE_END = 0,

    STATE_IDENTIFIER,
    STATE_OPERATOR,
    STATE_NUMBER,
    STATE_STRING,
    STATE_CHAR,
    STATE_COMMENT,
    STATE_NEWLINE,

    _STATE_LAST_FINAL_,

    STATE_IN_WHITESPACE,
    STATE_IN_IDENTIFIER,
    STATE_IN_OPERATOR,
    STATE_IN_NUMBER,
    STATE_IN_STRING,
    STATE_IN_CHAR,
    STATE_IN_COMMENT,
    STATE_IN_NEWLINE,
    
    _STATE_COUNT_
} State_Kind;

char* state_kind_to_str(State_Kind kind) {
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("%d", kind));
    case STATE_END:           return "STATE_END";
    case STATE_IDENTIFIER:    return "STATE_IDENTIFIER";
    case STATE_OPERATOR:      return "STATE_OPERATOR";
    case STATE_NUMBER:        return "STATE_NUMBER";
    case STATE_STRING:        return "STATE_STRING";
    case STATE_CHAR:          return "STATE_CHAR";
    case STATE_COMMENT:       return "STATE_COMMENT";
    case STATE_NEWLINE:       return "STATE_NEWLINE";
    case _STATE_LAST_FINAL_:  return "_STATE_LAST_FINAL_";
    case STATE_IN_WHITESPACE: return "STATE_IN_WHITESPACE";
    case STATE_IN_IDENTIFIER: return "STATE_IN_IDENTIFIER";
    case STATE_IN_OPERATOR:   return "STATE_IN_OPERATOR";
    case STATE_IN_NUMBER:     return "STATE_IN_NUMBER";
    case STATE_IN_STRING:     return "STATE_IN_STRING";
    case STATE_IN_CHAR:       return "STATE_IN_CHAR";
    case STATE_IN_COMMENT:    return "STATE_IN_COMMENT";
    case STATE_IN_NEWLINE:    return "STATE_IN_NEWLINE";
    case _STATE_COUNT_:       return "_STATE_COUNT_";
    }
    UNREACHABLE;
    return NULL;
}

typedef enum {
    EQUIV_EOF = 0,
    
    EQUIV_WHITESPACE,

    EQUIV_IDENTIFIER,
    EQUIV_OPERATOR,
    EQUIV_NUMBER,
    EQUIV_STRING,
    EQUIV_CHAR,
    EQUIV_COMMENT,
    EQUIV_NEWLINE,
    _EQUIV_COUNT_
} Equivalence_Kind;

char* equivalence_kind_to_str(Equivalence_Kind kind) {
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("%d", kind));
    case EQUIV_EOF:        return "EQUIV_EOF";
    case EQUIV_WHITESPACE: return "EQUIV_WHITESPACE";
    case EQUIV_IDENTIFIER: return "EQUIV_IDENTIFIER";
    case EQUIV_OPERATOR:   return "EQUIV_OPERATOR";
    case EQUIV_NUMBER:     return "EQUIV_NUMBER";
    case EQUIV_STRING:     return "EQUIV_STRING";
    case EQUIV_CHAR:       return "EQUIV_CHAR";
    case EQUIV_COMMENT:    return "EQUIV_COMMENT";
    case EQUIV_NEWLINE:    return "EQUIV_NEWLINE";
    case _EQUIV_COUNT_:    return "_EQUIV_COUNT_";
    }
    UNREACHABLE;
    return NULL;
}

static State_Kind transition[_STATE_COUNT_][_EQUIV_COUNT_] = {

    // Parsing identifers
    [STATE_IN_IDENTIFIER] [EQUIV_IDENTIFIER] = STATE_IN_IDENTIFIER, // cont..
    [STATE_IN_IDENTIFIER] [EQUIV_NUMBER]     = STATE_IN_IDENTIFIER, // cont..
    [STATE_IN_IDENTIFIER] [EQUIV_EOF]        = STATE_IDENTIFIER, // end
    [STATE_IN_IDENTIFIER] [EQUIV_WHITESPACE] = STATE_IDENTIFIER, // end
    [STATE_IN_IDENTIFIER] [EQUIV_NEWLINE]    = STATE_IDENTIFIER, // end
    [STATE_IN_IDENTIFIER] [EQUIV_OPERATOR]   = STATE_IDENTIFIER, // end
    [STATE_IN_IDENTIFIER] [EQUIV_COMMENT]    = STATE_IDENTIFIER, // end
    [STATE_IN_IDENTIFIER] [EQUIV_STRING]     = STATE_IDENTIFIER, // end
    [STATE_IN_IDENTIFIER] [EQUIV_CHAR]       = STATE_IDENTIFIER, // end
    //

    // Parsing whitespace 
    [STATE_IN_WHITESPACE] [EQUIV_WHITESPACE]  = STATE_IN_WHITESPACE, // cont..
    [STATE_IN_WHITESPACE] [EQUIV_IDENTIFIER]  = STATE_IN_IDENTIFIER, // end
    [STATE_IN_WHITESPACE] [EQUIV_COMMENT]     = STATE_IN_COMMENT, // end    
    [STATE_IN_WHITESPACE] [EQUIV_NEWLINE]     = STATE_NEWLINE, // end
    [STATE_IN_WHITESPACE] [EQUIV_OPERATOR]    = STATE_OPERATOR, // end
    [STATE_IN_WHITESPACE] [EQUIV_STRING]      = STATE_STRING, // end
    [STATE_IN_WHITESPACE] [EQUIV_CHAR]        = STATE_CHAR, // end
    [STATE_IN_WHITESPACE] [EQUIV_NUMBER]      = STATE_NUMBER, // end
    // 

    // Parsing comments
    [STATE_IN_COMMENT] [EQUIV_IDENTIFIER] = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_OPERATOR]   = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_STRING]     = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_CHAR]       = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_NUMBER]     = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_COMMENT]    = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_WHITESPACE] = STATE_IN_COMMENT, // cont..
    [STATE_IN_COMMENT] [EQUIV_NEWLINE]    = STATE_COMMENT, // end
    //
};

static bool in_token[] = {
    [STATE_IN_WHITESPACE] = false,
    [STATE_IN_IDENTIFIER] = true,
    [STATE_IN_OPERATOR]   = true,
    [STATE_IN_NUMBER]     = true,
    [STATE_IN_STRING]     = true,
    [STATE_IN_CHAR]       = true,
    [STATE_IN_COMMENT]    = true,
    [STATE_IN_NEWLINE]    = false,
};

static Equivalence_Kind equivalence[] = {
    ['\0']    = EQUIV_EOF,
    ['\n']    = EQUIV_NEWLINE,
    [' ']     = EQUIV_WHITESPACE,
    ['!']     = EQUIV_OPERATOR,
    ['"']     = EQUIV_STRING,
    ['#']     = EQUIV_COMMENT,
    ['$']     = EQUIV_OPERATOR,
    ['%']     = EQUIV_OPERATOR,
    ['&']     = EQUIV_OPERATOR,
    ['\'']    = EQUIV_CHAR,
    ['(']     = EQUIV_OPERATOR,
    [')']     = EQUIV_OPERATOR,
    ['*']     = EQUIV_OPERATOR,
    ['+']     = EQUIV_OPERATOR,
    [',']     = EQUIV_OPERATOR,
    ['-']     = EQUIV_OPERATOR,
    ['.']     = EQUIV_OPERATOR,
    ['/']     = EQUIV_OPERATOR,
    ['0']     = EQUIV_NUMBER,
    ['1']     = EQUIV_NUMBER,
    ['2']     = EQUIV_NUMBER,
    ['3']     = EQUIV_NUMBER,
    ['4']     = EQUIV_NUMBER,
    ['5']     = EQUIV_NUMBER,
    ['6']     = EQUIV_NUMBER,
    ['7']     = EQUIV_NUMBER,
    ['8']     = EQUIV_NUMBER,
    ['9']     = EQUIV_NUMBER,
    [':']     = EQUIV_OPERATOR,
    [';']     = EQUIV_OPERATOR,
    ['<']     = EQUIV_OPERATOR,
    ['=']     = EQUIV_OPERATOR,
    ['>']     = EQUIV_OPERATOR,
    ['?']     = EQUIV_OPERATOR,
    ['@']     = EQUIV_OPERATOR,
    ['A']     = EQUIV_IDENTIFIER,
    ['B']     = EQUIV_IDENTIFIER,
    ['C']     = EQUIV_IDENTIFIER,
    ['D']     = EQUIV_IDENTIFIER,
    ['E']     = EQUIV_IDENTIFIER,
    ['F']     = EQUIV_IDENTIFIER,
    ['G']     = EQUIV_IDENTIFIER,
    ['H']     = EQUIV_IDENTIFIER,
    ['I']     = EQUIV_IDENTIFIER,
    ['J']     = EQUIV_IDENTIFIER,
    ['K']     = EQUIV_IDENTIFIER,
    ['L']     = EQUIV_IDENTIFIER,
    ['M']     = EQUIV_IDENTIFIER,
    ['N']     = EQUIV_IDENTIFIER,
    ['O']     = EQUIV_IDENTIFIER,
    ['P']     = EQUIV_IDENTIFIER,
    ['Q']     = EQUIV_IDENTIFIER,
    ['R']     = EQUIV_IDENTIFIER,
    ['S']     = EQUIV_IDENTIFIER,
    ['T']     = EQUIV_IDENTIFIER,
    ['U']     = EQUIV_IDENTIFIER,
    ['V']     = EQUIV_IDENTIFIER,
    ['W']     = EQUIV_IDENTIFIER,
    ['X']     = EQUIV_IDENTIFIER,
    ['Y']     = EQUIV_IDENTIFIER,
    ['Z']     = EQUIV_IDENTIFIER,
    ['[']     = EQUIV_OPERATOR,
    ['\\']    = EQUIV_OPERATOR,
    [']']     = EQUIV_OPERATOR,
    ['^']     = EQUIV_OPERATOR,
    ['_']     = EQUIV_IDENTIFIER,
    ['`']     = EQUIV_STRING,
    ['a']     = EQUIV_IDENTIFIER,
    ['b']     = EQUIV_IDENTIFIER,
    ['c']     = EQUIV_IDENTIFIER,
    ['d']     = EQUIV_IDENTIFIER,
    ['e']     = EQUIV_IDENTIFIER,
    ['f']     = EQUIV_IDENTIFIER,
    ['g']     = EQUIV_IDENTIFIER,
    ['h']     = EQUIV_IDENTIFIER,
    ['i']     = EQUIV_IDENTIFIER,
    ['j']     = EQUIV_IDENTIFIER,
    ['k']     = EQUIV_IDENTIFIER,
    ['l']     = EQUIV_IDENTIFIER,
    ['m']     = EQUIV_IDENTIFIER,
    ['n']     = EQUIV_IDENTIFIER,
    ['o']     = EQUIV_IDENTIFIER,
    ['p']     = EQUIV_IDENTIFIER,
    ['q']     = EQUIV_IDENTIFIER,
    ['r']     = EQUIV_IDENTIFIER,
    ['s']     = EQUIV_IDENTIFIER,
    ['t']     = EQUIV_IDENTIFIER,
    ['u']     = EQUIV_IDENTIFIER,
    ['v']     = EQUIV_IDENTIFIER,
    ['w']     = EQUIV_IDENTIFIER,
    ['x']     = EQUIV_IDENTIFIER,
    ['y']     = EQUIV_IDENTIFIER,
    ['z']     = EQUIV_IDENTIFIER,
    ['{']     = EQUIV_OPERATOR,
    ['|']     = EQUIV_OPERATOR,
    ['}']     = EQUIV_OPERATOR,
    ['~']     = EQUIV_OPERATOR,
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
    Token_Kind kind = TOKEN_UNKNOWN;
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

        // skip_whitespace(c);

        u64 len = 0;
        State_Kind state = STATE_IN_WHITESPACE;
        do {
            s32 ch = *c++;
            Equivalence_Kind eq = equivalence[ch];
            state = transition[state][eq];
            len += in_token[state];
            // info ("ch %c, state %s, eq %s len %d", ch, state_kind_to_str(state), equivalence_kind_to_str(eq), len);
        } while (state > _STATE_LAST_FINAL_); // jumps out on 0

        // info ("state %s", state_kind_to_str(state));

        // need to backtrack a bit
        char* start = --c - len;
        char* end = c;

        // Set the indenation level
        col = start - position_of_newline;

        switch(state) {
        ERROR_UNHANDLED_KIND(state_kind_to_str(state));
        
        case STATE_END: kind = TOKEN_EOF; break;
        case STATE_IDENTIFIER: {
            kind = TOKEN_IDENTIFIER;

            // after parsing an identifier, check if its a keywords
            char* value = intern_range(&interns, start, end);
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
            ++start; // skip the first '
            ++comments;
        } break;
        case STATE_NEWLINE:
        {
            kind = TOKEN_NEWLINE;
            position_of_newline = ++start;
            end = ++c;// skip the newline
            ++line;
        } break;
        }

        if (last_token_kind == TOKEN_NEWLINE && kind != TOKEN_NEWLINE && kind != TOKEN_COMMENT && (col % DEFAULT_INDENT_LEVEL == 0)) {
                
            current_indentation_level = col;
            
            if (current_indentation_level > previous_indentation_level) {
                previous_indentation_level = current_indentation_level;
                token_array_append(&tokens, (Token){TOKEN_BLOCK_START, start-1, start, line, col});
            } else while (current_indentation_level < previous_indentation_level) {
                previous_indentation_level -= DEFAULT_INDENT_LEVEL;
                token_array_append(&tokens, (Token){TOKEN_BLOCK_END, start-1, start, line, col});
            }

            // else error("[%s:%d:%d] indentation error.", file, line, col);
        }


        xassert(kind != TOKEN_UNKNOWN);
        token_array_append(&tokens, (Token){kind, start, end, line, col});

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

    case TOKEN_BANG:              return "!";
    case TOKEN_DOUBLE_QUOTATION:  return "\"";
    case TOKEN_HASH:              return "#";
    case TOKEN_DOLLAR_SIGN:       return "$";
    case TOKEN_PERCENT:           return "%";
    case TOKEN_AND:               return "&";
    case TOKEN_SINGLE_QUOTATION:  return "'";
    case TOKEN_OPEN_PAREN:        return "(";
    case TOKEN_CLOSE_PAREN:       return ")";
    case TOKEN_ASTERISK:          return "*";
    case TOKEN_PLUS:              return "+";
    case TOKEN_COMMA:             return ",";
    case TOKEN_MINUS:             return "-";
    case TOKEN_DOT:               return ".";
    case TOKEN_FWSLASH:           return "/";
    case TOKEN_COLON:             return ":";
    case TOKEN_SEMICOLON:         return ";";
    case TOKEN_LT:                return "<";
    case TOKEN_EQ:                return "=";
    case TOKEN_GT:                return ">";
    case TOKEN_QUESTION_MARK:     return "?";
    case TOKEN_AT:                return "@";
    case TOKEN_OPEN_BRACKET:      return "[";
    case TOKEN_BWSLASH:           return "\\";
    case TOKEN_CLOSE_BRACKET:     return "]";
    case TOKEN_HAT:               return "^";
    case TOKEN_BACK_TICK:         return "`";
    case TOKEN_OPEN_BRACE:        return "{";
    case TOKEN_PIPE:              return "|";
    case TOKEN_CLOSE_BRACE:       return "}";
    case TOKEN_TILDE:             return "~";

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

    case TOKEN_PIPE_PIPE:         return "||";
    case TOKEN_AND_AND:           return "&&";
    case TOKEN_PLUS_EQ:           return "+=";
    case TOKEN_MINUS_EQ:          return "-=";
    case TOKEN_HAT_EQ:            return "^=";
    case TOKEN_ASTERISK_EQ:       return "*=";
    case TOKEN_PIPE_EQ:           return "|=";
    case TOKEN_PERCENT_EQ:        return "%=";
    case TOKEN_AND_EQ:            return "&=";
    case TOKEN_EQ_EQ:             return "==";
    case TOKEN_EQ_GT:             return "=>";
    case TOKEN_BANG_EQ:           return "!=";
    case TOKEN_COLON_COLON:       return "::";
    case TOKEN_COLON_EQ:          return ":=";
    case TOKEN_MINUS_GT:          return "->";
    case TOKEN_LT_EQ:             return "<=";
    case TOKEN_LT_LT:             return "<<";
    case TOKEN_LT_LT_EQ:          return "<<=";
    case TOKEN_GT_EQ:             return ">=";
    case TOKEN_GT_GT:             return ">>";
    case TOKEN_GT_GT_EQ:          return ">>=";
    case TOKEN_DOT_DOT:           return "..";
    case TOKEN_DOT_DOT_DOT:       return "...";
    case TOKEN_MINUS_MINUS:       return "--";
    case TOKEN_MINUS_MINUS_MINUS: return "---";
    case TOKEN_PLUS_PLUS:         return "++";
    case TOKEN_FWSLASH_EQ:        return "/=";
    case TOKEN_PIPE_GT:           return "|>";
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
