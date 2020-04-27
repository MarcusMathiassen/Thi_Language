typedef enum
{
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

INTERNAL u8* state_kind_to_str(State_Kind kind)
{
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

typedef enum
{
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

INTERNAL u8* equivalence_kind_to_str(Equivalence_Kind kind)
{
    switch (kind)
    {
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

INTERNAL State_Kind transition[_STATE_COUNT_][_EQUIV_COUNT_] =
{
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

INTERNAL s8 in_token[] =
{
    [STATE_IN_WHITESPACE] = 0,
    [STATE_IN_IDENTIFIER] = 1,
    [STATE_IN_OPERATOR]   = 1,
    [STATE_IN_NUMBER]     = 1,
    [STATE_IN_STRING]     = 1,
    [STATE_IN_CHAR]       = 1,
    [STATE_IN_COMMENT]    = 1,
    [STATE_IN_NEWLINE]    = 0,
};

INTERNAL Equivalence_Kind equivalence[] =
{
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
typedef enum
{
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
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_INDENT,
    TOKEN_DEDENT,

    _START_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_,
    
    _TOKEN_KEYWORDS_START_, // start of keywords
    TOKEN_DEF,
    TOKEN_TRUE,
    TOKEN_FALSE,
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
    TOKEN_IS,
    TOKEN_CAST,

    TOKEN_ATOM,

    _END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_, // @Volatile: do not move me please.


    _Token_Kind_count_
} Token_Kind;

INTERNAL u8*
token_kind_to_str(Token_Kind kind)
{
    ASSERT_KIND_IN_RANGE(Token_Kind, kind);
    switch(kind)
    {
        ERROR_UNHANDLED_KIND(strf("%d", kind));
        case TOKEN_EOF:              return "TOKEN_EOF";
        case TOKEN_UNKNOWN:          return "TOKEN_UNKNOWN";
        case TOKEN_WHITESPACE:       return "TOKEN_WHITESPACE";
        case TOKEN_BANG:             return "TOKEN_BANG";
        case TOKEN_DOUBLE_QUOTATION: return "TOKEN_DOUBLE_QUOTATION";
        case TOKEN_HASH:             return "TOKEN_HASH";
        case TOKEN_DOLLAR_SIGN:      return "TOKEN_DOLLAR_SIGN";
        case TOKEN_PERCENT:          return "TOKEN_PERCENT";
        case TOKEN_AND:              return "TOKEN_AND";
        case TOKEN_SINGLE_QUOTATION: return "TOKEN_SINGLE_QUOTATION";
        case TOKEN_OPEN_PAREN:       return "TOKEN_OPEN_PAREN";
        case TOKEN_CLOSE_PAREN:      return "TOKEN_CLOSE_PAREN";
        case TOKEN_ASTERISK:         return "TOKEN_ASTERISK";
        case TOKEN_PLUS:             return "TOKEN_PLUS";
        case TOKEN_COMMA:            return "TOKEN_COMMA";
        case TOKEN_MINUS:            return "TOKEN_MINUS";
        case TOKEN_DOT:              return "TOKEN_DOT";
        case TOKEN_FWSLASH:          return "TOKEN_FWSLASH";
        case TOKEN_COLON:            return "TOKEN_COLON";
        case TOKEN_SEMICOLON:        return "TOKEN_SEMICOLON";
        case TOKEN_LT:               return "TOKEN_LT";
        case TOKEN_EQ:               return "TOKEN_EQ";
        case TOKEN_GT:               return "TOKEN_GT";
        case TOKEN_QUESTION_MARK:    return "TOKEN_QUESTION_MARK";
        case TOKEN_AT:               return "TOKEN_AT";
        case TOKEN_OPEN_BRACKET:     return "TOKEN_OPEN_BRACKET";
        case TOKEN_BWSLASH:          return "TOKEN_BWSLASH";
        case TOKEN_CLOSE_BRACKET:    return "TOKEN_CLOSE_BRACKET";
        case TOKEN_HAT:              return "TOKEN_HAT";
        case TOKEN_BACK_TICK:        return "TOKEN_BACK_TICK";
        case TOKEN_OPEN_BRACE:       return "TOKEN_OPEN_BRACE";
        case TOKEN_PIPE:             return "TOKEN_PIPE";
        case TOKEN_CLOSE_BRACE:      return "TOKEN_CLOSE_BRACE";
        case TOKEN_TILDE:            return "TOKEN_TILDE";
        case TOKEN_NEWLINE:          return "TOKEN_NEWLINE";
        case TOKEN_COMMENT:          return "TOKEN_COMMENT";
        case TOKEN_ATOM:             return "TOKEN_ATOM";
        case TOKEN_INT:              return "TOKEN_INT";
        case TOKEN_FLOAT:            return "TOKEN_FLOAT";
        case TOKEN_STRING:           return "TOKEN_STRING";
        case TOKEN_CHAR:             return "TOKEN_CHAR";
        case TOKEN_INDENT:           return "TOKEN_INDENT";
        case TOKEN_DEDENT:           return "TOKEN_DEDENT";

        case TOKEN_DEF:              return "TOKEN_DEF";
        case TOKEN_TRUE:             return "TOKEN_TRUE";
        case TOKEN_FALSE:            return "TOKEN_FALSE";
        case TOKEN_SIZEOF:           return "TOKEN_SIZEOF";
        case TOKEN_TYPEOF:           return "TOKEN_TYPEOF";
        case TOKEN_IF:               return "TOKEN_IF";
        case TOKEN_ELSE:             return "TOKEN_ELSE";
        case TOKEN_FOR:              return "TOKEN_FOR";
        case TOKEN_WHILE:            return "TOKEN_WHILE";
        case TOKEN_IN:               return "TOKEN_IN";
        case TOKEN_RETURN:           return "TOKEN_RETURN";
        case TOKEN_ENUM:             return "TOKEN_ENUM";
        case TOKEN_STRUCT:           return "TOKEN_STRUCT";
        case TOKEN_BREAK:            return "TOKEN_BREAK";
        case TOKEN_CONTINUE:         return "TOKEN_CONTINUE";
        case TOKEN_FALLTHROUGH:      return "TOKEN_FALLTHROUGH";
        case TOKEN_IS:               return "TOKEN_IS";
        case TOKEN_CAST:             return "TOKEN_CAST";
    }
    UNREACHABLE;
    return NULL;
}

typedef struct
{
    Token_Kind kind;
    u8* start;
    u8* end;
    s64 line;
    s64 col;
} Token;

typedef struct
{
    u8* str;
    s64 len;
} Intern;

typedef struct
{
    Intern* data;
    s64 count;
    s64 allocated;
} Intern_Array;

GLOBAL_VARIABLE Intern_Array interns;

INTERNAL u8*
intern_range(u8* start, u8* end)
{
    s64 len = end - start;
    foreach(i, interns.count)
    {
        Intern intern = interns.data[i];
        if (intern.len == len && strncmp(intern.str, start, len) == 0)
            return intern.str;
    }
    u8* str = strn(start, end);

    while (interns.count >= interns.allocated)
    {
        interns.allocated *= 2;
        interns.data = xrealloc(interns.data, sizeof(Intern) * interns.allocated);
    }

    interns.data[interns.count++] = (Intern){str, len};

    return str;
}


INTERNAL u8*
intern(u8* str)
{
    return intern_range(str, str + strlen(str));
}

// @Volatile
typedef enum
{
    KEY_DEF,
    KEY_TRUE,
    KEY_FALSE,
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
    KEY_IS,
    KEY_CAST,
    _Keyword_Kind_count_,
} Keyword_Kind;
GLOBAL_VARIABLE u8* interned_keywords[_Keyword_Kind_count_];
// @Volatile
GLOBAL_VARIABLE u8* keywords_as_strings[_Keyword_Kind_count_] = {
    "def",
    "true",
    "false",
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
    "is",
    "cast",
};

INTERNAL void
interns_initilize()
{
    interns.allocated = _Keyword_Kind_count_;
    interns.data = xmalloc(sizeof(Intern) * interns.allocated);
    foreach(i, _Keyword_Kind_count_)
        interned_keywords[i] = intern(keywords_as_strings[i]);
}


INTERNAL u8*
token_value(Token token)
{
    return (
        token.kind > _START_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_ && 
        token.kind < _END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_) ? token.start : strn(token.start, token.end);
}

#define IS_NUMBER (*c > 47 && *c < 58)
#define IS_DIGIT (*c > 47 && *c < 58 || *c == '.')
#define IS_ALPHA (*c > 64 && *c < 91 || *c > 96 && *c < 123 || *c == '_')

#define SKIP_WHITESPACE \
    while (*c == ' ' || *c == '\t') ++c;

#define GOTO_END_OF_NUMBER \
    while (IS_NUMBER || *c == '.') ++c;

#define GOTO_END_OF_ATOM \
    while (IS_ALPHA || IS_NUMBER) ++c;

#define GOTO_END_OF_LINE \
    while (*c != '\n') ++c;

INTERNAL Token
get_token(u8** source)
{
    if (!interns.data)
        interns_initilize();

    // Our character pointer
    u8* c = *source;
    u8* position_of_newline = c;
    s64 col = 1;

    u64 len = 0;
    State_Kind state = STATE_IN_WHITESPACE;
    
    do // This is based on http://nothings.org/computer/lexing.html
    {
        
        s32 ch              = *c++;
        Equivalence_Kind eq =  equivalence[ch];
        state               =  transition[state][eq];
        len                 += in_token[state];

    } while (state > _STATE_LAST_FINAL_);

    // Set the START and END for this token.
    // These *might* change underneath here, usually they don't, but sometimes they do.
    u8* start = --c - len; // need to backtrack a bit
    u8* end = c;

    // Set the column
    col = start - position_of_newline;

    // Token starts of as unknown
    Token_Kind kind = TOKEN_UNKNOWN;

    switch(state)
    {
        ERROR_UNHANDLED_KIND(state_kind_to_str(state));
        
        case STATE_END:
        {
            kind = TOKEN_EOF;
        } break;

        case STATE_IDENTIFIER:
        {
            kind = TOKEN_ATOM;

            // Intern the string and check if it's a keyword
            u8* value = intern_range(start, end);
            foreach(i, _Keyword_Kind_count_)
            {
                // interned strings point to the same address
                if (interned_keywords[i] == value)
                {
                    kind = (Token_Kind)(i + _TOKEN_KEYWORDS_START_+1); // keywords start at _TOKEN_KEYWORDS_START_+1 @Volatile
                    break;
                }
            }
            start = value; // @Hack: we're using something in a way it was ot ment to be used
        } break;

        case STATE_OPERATOR:
        {
            kind = (Token_Kind)*c;
            end = ++c;
        } break;

        case STATE_NUMBER:
        {
            b8 has_decimal = false;
            while (IS_DIGIT)
            {
                if (*c == '.') has_decimal = true;
                ++c;
            }
            end = c;
            kind = has_decimal ? TOKEN_FLOAT : TOKEN_INT;
        } break;
        
        case STATE_STRING:
        {
            kind = TOKEN_STRING;
            ++c;
            start = c;
            while (*c != '"') ++c;
            end = c++;
        } break;

        case STATE_CHAR:
        {
            kind = TOKEN_CHAR;
            start = ++c;
            if (*c == '\\') ++c;
            end = ++c;
            ++c;
        } break;

        case STATE_COMMENT:
        {
            kind = TOKEN_COMMENT;
            start = ++c;
            GOTO_END_OF_LINE;
            end = c;
        } break;

        case STATE_NEWLINE:
        {
            kind = TOKEN_NEWLINE;
            position_of_newline = start+1;
            end = ++c;
        } break;
    }

    // update the source cursor position
    *source = c;

    return (Token)
    {
        .kind = kind,
        .start = start,
        .end = end,
        .col = col,
    };
}
