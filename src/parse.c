#define TRACE debug("[%s:%d:%s]: %s : %s",  __FILE__, __LINE__, __func__, token_kind_to_str(ctx->tokens->kind), give_unique_color(token_value(*ctx->tokens)));

typedef enum
{
    AST_COMMENT,

    _AST_EXPRESSIONS_START_,

    AST_ATOM,
    AST_LITERAL,
    AST_GROUPING,
    AST_UNARY,
    AST_LAMBDA,

    _AST_EXPRESSIONS_END_,

    _AST_STATEMENTS_START_,
    
    AST_RETURN,

    _AST_STATEMENTS_END_,

} ast_kind_t;

typedef enum
{
    AST_LITERAL_INT,
    AST_LITERAL_FLOAT,
    AST_LITERAL_CHAR,
    AST_LITERAL_STRING,
} ast_literal_kind_t;

internal u8*
ast_kind_to_str(ast_kind_t kind)
{
    switch(kind)
    {
        case AST_COMMENT: return "AST_COMMENT";
        case AST_ATOM:    return "AST_ATOM";
        case AST_LITERAL: return "AST_LITERAL";
        case AST_GROUPING: return "AST_GROUPING";
        case AST_UNARY:   return "AST_UNARY";
        case AST_LAMBDA:  return "AST_LAMBDA";
        case AST_RETURN:  return "AST_RETURN";
    }
    UNREACHABLE;
    return NULL;
}

typedef struct ast_t ast_t;
struct ast_t
{
    ast_kind_t kind;
    union
    {
        struct
        {
            u8* text;
        } Comment;
        struct
        {
            u8* name;
        } Atom;
        struct
        {
            ast_literal_kind_t kind;
            union
            {
                struct { s64   value; } Int;
                struct { f64   value; } Float;
                struct { u8    value; } Char;
                struct { char* value; } String;
            };
        } Literal;
        struct
        {
            ast_t* expr;
        } Grouping;
        struct
        {
            token_kind_t kind;
            ast_t* expr;
        } Unary;
        struct
        {
            char* name; // optional
            ast_t* params; // optional
            ast_t* block;
        } Lambda;
        struct
        {
            ast_t* expr;
        } Return;
    };
};

inline internal ast_t*
make_ast(ast_kind_t kind)
{
    ast_t* a = xmalloc(sizeof(ast_t));
    a->kind = kind;
    return a;
}

typedef struct
{
    token_t* tokens;
} parse_ctx_t;

inline internal void
eat(parse_ctx_t* ctx)
{
    ++ctx->tokens;
}

inline internal token_kind_t
kind(parse_ctx_t* ctx)
{
    return ctx->tokens->kind;
}

inline internal void
match(parse_ctx_t* ctx, token_kind_t k)
{
    if (kind(ctx) != k)
    {
        error("Expected %s. Got %s", token_kind_to_str(k), token_kind_to_str(kind(ctx)));
        exit(1);
    }
    eat(ctx);
}

inline internal u8*
value(parse_ctx_t* ctx)
{
    return token_value(*ctx->tokens);
}

typedef enum
{
    ASSOC_LEFT_TO_RIGHT,
    ASSOC_RIGHT_TO_LEFT,
} assoc_t;

const global_variable struct
{
    s32 prec;
    assoc_t assoc;
} precedence_table[] =
{
    // [TOKEN_PLUS_PLUS]   = {1, ASSOC_LEFT_TO_RIGHT},
    // [TOKEN_MINUS_MINUS] = {1, ASSOC_LEFT_TO_RIGHT},
    [TOKEN_OPEN_PAREN]  = {1, ASSOC_LEFT_TO_RIGHT},
    [TOKEN_OPEN_BRACKET]  = {1, ASSOC_LEFT_TO_RIGHT},
    [TOKEN_DOT]  = {1, ASSOC_LEFT_TO_RIGHT},
    // [TOKEN_MINUS_GT]  = {1, ASSOC_LEFT_TO_RIGHT},
    // (type){list}    Compound literal(C99)
    // [TOKEN_PLUS_PLUS]  = {2, ASSOC_RIGHT_TO_LEFT},
    // [TOKEN_MINUS_MINUS]  = {2, ASSOC_RIGHT_TO_LEFT},
    [TOKEN_PLUS]  = {2, ASSOC_RIGHT_TO_LEFT},
    [TOKEN_MINUS]  = {2, ASSOC_RIGHT_TO_LEFT},

    [TOKEN_BANG]  = {2, ASSOC_RIGHT_TO_LEFT},
    [TOKEN_TILDE]  = {2, ASSOC_RIGHT_TO_LEFT},
    // [TOKEN_CAST]  = {2, ASSOC_RIGHT_TO_LEFT},
    [TOKEN_ASTERISK]  = {2, ASSOC_RIGHT_TO_LEFT},
    [TOKEN_AND]  = {2, ASSOC_RIGHT_TO_LEFT},
    // [TOKEN_SIZEOF]  = {2, ASSOC_RIGHT_TO_LEFT},
};


internal ast_t* parse_expression(parse_ctx_t* ctx);
internal ast_t* parse_statement(parse_ctx_t* ctx);

inline internal ast_t*
parse_atom(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_ATOM);
    node->Atom.name = value(ctx);
    eat(ctx);
    return node;
}
inline internal ast_t*
parse_literal(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_LITERAL);
    u8* val = value(ctx);

    switch (kind(ctx))
    {
        case TOKEN_INT:    node->Literal.Int.value = strtoll(val, NULL, 0); break;
        case TOKEN_FLOAT:  node->Literal.Float.value = atof(val);           break;
        case TOKEN_CHAR:   node->Literal.Char.value = atoll(val);           break;
        case TOKEN_STRING: node->Literal.String.value = val;                break;
    }
    eat(ctx);
    return node;
}
inline internal ast_t*
parse_unary(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_UNARY);
    token_kind_t k = kind(ctx);
    return node;
}

inline internal ast_t*
parse_lambda(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_LAMBDA);
    // eat(ctx);
    // node->Lambda.name = value(ctx);
    // eat(ctx);
    // node->Lambda.expr = parse_expression(ctx);
    return node;
}

inline internal ast_t*
parse_comment(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_COMMENT);
    node->Comment.text = value(ctx);
    eat(ctx);
    return node;
}

inline internal ast_t*
parse_grouping(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_GROUPING);
    eat(ctx);
    if (kind(ctx) != TOKEN_CLOSE_PAREN) 
        node->Grouping.expr = parse_expression(ctx);
    match(ctx, TOKEN_CLOSE_PAREN);
    return node;
}

inline internal ast_t*
parse_return(parse_ctx_t* ctx)
{
    TRACE;
    ast_t* node = make_ast(AST_RETURN);
    eat(ctx);
    node->Return.expr = parse_expression(ctx);
    return node;
}

internal ast_t*
parse_primary(parse_ctx_t* ctx)
{
    TRACE;
    switch(kind(ctx))
    {
        default: UNFINISHED;
        
        case TOKEN_COMMENT:       return parse_comment(ctx);
        case TOKEN_ATOM:          return parse_atom(ctx);
        case TOKEN_OPEN_PAREN:    return parse_grouping(ctx);
        
        case TOKEN_INT:    FALLTHROUGH;
        case TOKEN_FLOAT:  FALLTHROUGH;
        case TOKEN_CHAR:   FALLTHROUGH;
        case TOKEN_STRING: return parse_literal(ctx);
    }
    UNREACHABLE;
    return NULL;
}

internal ast_t*
parse_expression(parse_ctx_t* ctx)
{
    TRACE;
    switch(kind(ctx))
    {
        default: return parse_primary(ctx);
    }
    UNREACHABLE;
    return NULL;
}

internal ast_t*
parse_statement(parse_ctx_t* ctx)
{
    TRACE;
    switch(kind(ctx))
    {
        default: return parse_expression(ctx);
        case TOKEN_RETURN: return parse_return(ctx);
    }
    UNREACHABLE;
    return NULL;
}

internal ast_t**
parse(token_t* tokens)
{ 
    u64 ast_count = 0;
    u64 ast_allocated = 8;
    ast_t** ast = xmalloc(sizeof(ast_t*) * ast_allocated);

    parse_ctx_t ctx = (parse_ctx_t)
    {
        .tokens = tokens
    };

    while (kind(&ctx))
    {
        ast_t* node = parse_statement(&ctx);

        // Allocate more tokens if needed
        while (ast_count >= ast_allocated)
        {
            ast_allocated *= 2;
            ast = xrealloc(ast, sizeof(ast_t*) * ast_allocated);
        }

        debug("%s", ast_kind_to_str(node->kind));
        ast[ast_count++] = node;
    }

    return ast;
}
