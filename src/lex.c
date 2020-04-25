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
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_BLOCK_START,
    TOKEN_BLOCK_END,

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


    _token_kind_t_count_
} token_kind_t;

internal u8*
token_kind_to_str(token_kind_t kind)
{
    ASSERT_KIND_IN_RANGE(token_kind_t, kind);
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
        case TOKEN_BLOCK_START:      return "TOKEN_BLOCK_START";
        case TOKEN_BLOCK_END:        return "TOKEN_BLOCK_END";

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
    token_kind_t kind;
    u8* start;
    u8* end;
} token_t;

typedef struct
{
    u8* str;
    s64 len;
} intern_t;

typedef struct
{
    intern_t* data;
    s64 count;
    s64 allocated;
} intern_array_t;

global_variable intern_array_t interns;

internal u8*
intern_range(u8* start, u8* end)
{
    s64 len = end - start;
    foreach(i, interns.count)
    {
        intern_t intern = interns.data[i];
        if (intern.len == len && strncmp(intern.str, start, len) == 0)
            return intern.str;
    }
    u8* str = strn(start, end);

    while (interns.count >= interns.allocated)
    {
        interns.allocated *= 2;
        interns.data = xrealloc(interns.data, sizeof(intern_t) * interns.allocated);
    }

    interns.data[interns.count++] = (intern_t){str, len};

    return str;
}


internal u8*
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
    _keyword_kind_t_count_,
} keyword_kind_t;
global_variable u8* interned_keywords[_keyword_kind_t_count_];
// @Volatile
global_variable u8* keywords_as_strings[_keyword_kind_t_count_] = {
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

internal void
interns_initilize()
{
    interns.allocated = _keyword_kind_t_count_;
    interns.data = xmalloc(sizeof(intern_t) * interns.allocated);
    foreach(i, _keyword_kind_t_count_)
        interned_keywords[i] = intern(keywords_as_strings[i]);
}


internal u8*
token_value(token_t token)
{
    return (
        token.kind > _START_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_ && 
        token.kind < _END_OF_TOKENS_WHO_STORE_A_ZERO_TERMINATED_STRING_IN_TOKEN_START_) ? token.start : strn(token.start, token.end);
}

#define IS_NUMBER (*c > 47 && *c < 58)
#define IS_DIGIT (*c > 47 && *c < 58 || *c == '.')
#define IS_ALPHA (*c > 64 && *c < 91 || *c > 96 && *c < 123 || *c == '_')

#define SKIP_WHITESPACE \
    while (*c == ' ' || *c == '\t' || *c == '\n') ++c;

#define GOTO_END_OF_NUMBER \
    while (IS_NUMBER || *c == '.') ++c;

#define GOTO_END_OF_ATOM \
    while (IS_ALPHA || IS_NUMBER) ++c;

#define GOTO_END_OF_LINE \
    while (*c != '\n') ++c;

internal token_t*
lex(u8* source)
{
    if (!interns.data)
        interns_initilize();

    u64 tokens_count = 0;
    u64 tokens_allocated = 8;

    token_t* tokens = xmalloc(sizeof(token_t) * tokens_allocated);

    // Our character pointer
    u8* c = source;

    // Token starts of as unknown
    token_kind_t kind = TOKEN_UNKNOWN;
    do
    {
        kind = TOKEN_UNKNOWN;

        // We skip all initial whitespace
        SKIP_WHITESPACE;

        // 'c' now points to the start of the character
        u8* start = c;
        u8* end = NULL;

        switch (*c)
        {   
            default:
                kind = (token_kind_t)(*c++);
                end = c;
                break;
            case '#':
            {
                kind = TOKEN_COMMENT;
                start = ++c;
                GOTO_END_OF_LINE;
                end = c;
            } break;

            case '\'':
            {
                kind = TOKEN_CHAR;
                start = ++c;
                if (*c == '\\') ++c;
                end = ++c;
                ++c;
            } break;
            case '"':
            {
                kind = TOKEN_STRING;
                ++c;
                start = c;
                while (*c != '"') ++c;
                end = c++;
            } break;

            case '0': FALLTHROUGH;
            case '1': FALLTHROUGH;
            case '2': FALLTHROUGH;
            case '3': FALLTHROUGH;
            case '4': FALLTHROUGH;
            case '5': FALLTHROUGH;
            case '6': FALLTHROUGH;
            case '7': FALLTHROUGH;
            case '8': FALLTHROUGH;
            case '9':
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

            // TOKEN_ATOM
            case '_': FALLTHROUGH;
            case 'A': FALLTHROUGH;
            case 'B': FALLTHROUGH;
            case 'C': FALLTHROUGH;
            case 'D': FALLTHROUGH;
            case 'E': FALLTHROUGH;
            case 'F': FALLTHROUGH;
            case 'G': FALLTHROUGH;
            case 'H': FALLTHROUGH;
            case 'I': FALLTHROUGH;
            case 'J': FALLTHROUGH;
            case 'K': FALLTHROUGH;
            case 'L': FALLTHROUGH;
            case 'M': FALLTHROUGH;
            case 'N': FALLTHROUGH;
            case 'O': FALLTHROUGH;
            case 'P': FALLTHROUGH;
            case 'Q': FALLTHROUGH;
            case 'R': FALLTHROUGH;
            case 'S': FALLTHROUGH;
            case 'T': FALLTHROUGH;
            case 'U': FALLTHROUGH;
            case 'V': FALLTHROUGH;
            case 'W': FALLTHROUGH;
            case 'X': FALLTHROUGH;
            case 'Y': FALLTHROUGH;
            case 'Z': FALLTHROUGH;
            case 'a': FALLTHROUGH;
            case 'b': FALLTHROUGH;
            case 'c': FALLTHROUGH;
            case 'd': FALLTHROUGH;
            case 'e': FALLTHROUGH;
            case 'f': FALLTHROUGH;
            case 'g': FALLTHROUGH;
            case 'h': FALLTHROUGH;
            case 'i': FALLTHROUGH;
            case 'j': FALLTHROUGH;
            case 'k': FALLTHROUGH;
            case 'l': FALLTHROUGH;
            case 'm': FALLTHROUGH;
            case 'n': FALLTHROUGH;
            case 'o': FALLTHROUGH;
            case 'p': FALLTHROUGH;
            case 'q': FALLTHROUGH;
            case 'r': FALLTHROUGH;
            case 's': FALLTHROUGH;
            case 't': FALLTHROUGH;
            case 'u': FALLTHROUGH;
            case 'v': FALLTHROUGH;
            case 'w': FALLTHROUGH;
            case 'x': FALLTHROUGH;
            case 'y': FALLTHROUGH;
            case 'z':
            {
                kind = TOKEN_ATOM;
                GOTO_END_OF_ATOM;
                end = c;

                // Intern the string and check if it's a keyword
                u8* value = intern_range(start, end);
                foreach(i, _keyword_kind_t_count_)
                {
                    // interned strings point to the same address
                    if (interned_keywords[i] == value)
                    {
                        kind = (token_kind_t)(i + _TOKEN_KEYWORDS_START_+1); // keywords start at _TOKEN_KEYWORDS_START_+1 @Volatile
                        break;
                    }
                }

                start = value;
            } break;
        }

        // Allocate more tokens if needed
        while (tokens_count >= tokens_allocated)
        {
            tokens_allocated *= 2; // allocate 2x more
            tokens = xrealloc(tokens, sizeof(token_t) * tokens_allocated);
        }

        // Add to token list
        tokens[tokens_count++] = (token_t)
        {
            .kind = kind,
            .start = start,
            .end = end
        };

    } while(kind); // jumps out on TOKEN_EOF(0)

    return tokens;
}

