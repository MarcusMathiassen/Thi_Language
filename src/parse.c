#define TRACE debug("[%s:%s:%d]: %s : %s",  __FILE__, __func__, __LINE__, token_kind_to_str(ctx->curr.kind), give_unique_color(token_value(ctx->curr)))

typedef enum
{
    AST_COMMENT,
    AST_ATOM,
    AST_LITERAL,
    AST_GROUPING,
    AST_UNARY,
    AST_LAMBDA,
    AST_NAMED_LAMBDA,
    AST_RETURN,
    AST_VARIABLE_DECL,
    AST_BLOCK,
    AST_MODULE,
    _AST_Kind_count_
} AST_Kind;

typedef enum
{
    AST_LITERAL_INT,
    AST_LITERAL_FLOAT,
    AST_LITERAL_CHAR,
    AST_LITERAL_STRING,
    _AST_Literal_Kind_count_
} AST_Literal_Kind;

INTERNAL u8*
ast_kind_to_str(AST_Kind kind)
{
    ASSERT_KIND_IN_RANGE(AST_Kind, kind);
    switch(kind)
    {
        ERROR_UNHANDLED_KIND(strf("%d", kind));
        case AST_COMMENT: return "AST_COMMENT";
        case AST_ATOM:    return "AST_ATOM";
        case AST_LITERAL: return "AST_LITERAL";
        case AST_GROUPING: return "AST_GROUPING";
        case AST_UNARY:   return "AST_UNARY";
        case AST_LAMBDA:  return "AST_LAMBDA";
        case AST_NAMED_LAMBDA:  return "AST_NAMED_LAMBDA";
        case AST_RETURN:  return "AST_RETURN";
        case AST_VARIABLE_DECL:  return "AST_VARIABLE_DECL";
        case AST_BLOCK:  return "AST_BLOCK";
        case AST_MODULE:  return "AST_MODULE";
    }
    UNREACHABLE;
    return NULL;
}

typedef struct AST AST;
struct AST
{
    AST_Kind kind;
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
            AST_Literal_Kind kind;
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
            AST* expr;
        } Grouping;
        struct
        {
            Token_Kind kind;
            AST* expr;
        } Unary;
        struct
        {
            AST* params;
            AST* block;
        } Lambda;
        struct
        {
            char* name;
            AST* lambda; 
        } Named_Lambda;
        struct
        {
            AST* expr;
        } Return;
        struct
        {
            char* name;
            AST* type; // optional
            AST* value; // optional
        } Variable_Decl;
        struct
        {
            List* stmts;
        } Block;
        struct
        {
            char* name;
            List* stmts;
        } Module;

    };
};

inline INTERNAL AST*
make_ast(AST_Kind kind)
{
    AST* a = xmalloc(sizeof(AST));
    a->kind = kind;
    return a;
}

typedef struct
{
    u8* file;
    u8* source;
    
    Token prev;
    Token curr;


    // These are for indents
    s64 previous_indentation_level;
    s64 current_indentation_level;

    s64 comments;
    s64 lines;


    // sometimes we have to buffer tokens
    Stack* buffered_tokens;

} Parse_Context;

inline INTERNAL void
eat(Parse_Context* ctx)
{
    //
    // Look for indentation
    //

    bool newline_hit = false;
    u8* position_of_newline = ctx->source;

    Token t;

    // Keep getting tokens if they are newlines or comments

    u8* start_of_token = ctx->source;
    while (true)
    {
        t = get_token(&ctx->source);

        if (t.kind == TOKEN_NEWLINE)
        {
            newline_hit = true;
            position_of_newline = t.start+1;
            ++ctx->lines;
        }
        else if (t.kind == TOKEN_COMMENT)
        {
            ++ctx->comments;
            break;
        }
        else
        {
            // If we hit a newline, we may have to emit indent or dedent token
            if (newline_hit)
            {
                start_of_token = ctx->source;
                #define NUM_SPACES_THAT_COUNT_AS_INDENT 4

                u64 col = t.col;
                // warning("%llu - %llu = %llu", start_of_token, position_of_newline, col);

                // Update indentation
                ctx->current_indentation_level = col;

                // warning("prev_indent_lvl: %llu, curr_indent_lvl: %llu", ctx->previous_indentation_level, ctx->current_indentation_level);
                if (ctx->current_indentation_level > ctx->previous_indentation_level) {
                    ctx->previous_indentation_level = ctx->current_indentation_level;
                    warning("indent");
                    // Set the token
                    t = (Token)
                    {
                        .kind = TOKEN_INDENT,
                        .start = t.start-1,
                        .end = t.start
                    };
                } else while (ctx->current_indentation_level < ctx->previous_indentation_level) {
                    ctx->previous_indentation_level -= NUM_SPACES_THAT_COUNT_AS_INDENT;
                    warning("dedent");
                    // Set the token
                    t = (Token)
                    {
                        .kind = TOKEN_DEDENT,
                        .start = t.start-1,
                        .end = t.start
                    };
                }
            }
            break;
        }
    }

    ctx->prev = ctx->curr;
    ctx->curr = t; 
}

inline INTERNAL Token_Kind
kind(Parse_Context* ctx)
{
    return ctx->curr.kind;
}

inline INTERNAL void
match(Parse_Context* ctx, Token_Kind k)
{
    if (kind(ctx) != k)
    {
        error("Expected %s. Got %s", token_kind_to_str(k), token_kind_to_str(kind(ctx)));
        exit(1);
    }
    eat(ctx);
}

inline INTERNAL u8*
value(Parse_Context* ctx)
{
    return token_value(ctx->curr);
}

typedef enum
{
    ASSOC_LEFT_TO_RIGHT,
    ASSOC_RIGHT_TO_LEFT,
} assoc_t;

const GLOBAL_VARIABLE struct
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

INTERNAL AST* parse_expression(Parse_Context* ctx);
INTERNAL AST* parse_statement(Parse_Context* ctx);

INTERNAL List* parse_delimited_list(Parse_Context* ctx, AST*(*parse_func)(Parse_Context* ctx), Token_Kind delimitor)
{
    List* list = make_list();
    while (kind(ctx) != delimitor)
    {
        AST* node = (*parse_func)(ctx);
        list_append(list, node);
    }
    return list;
}

inline INTERNAL AST*
parse_atom(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_ATOM);
    node->Atom.name = value(ctx);
    eat(ctx);
    switch(kind(ctx))
    {
        case TOKEN_ATOM:
        {
            AST* decl = make_ast(AST_VARIABLE_DECL);
            node->Variable_Decl.name = node->Atom.name;
            node->Variable_Decl.type = parse_expression(ctx);
            return decl;
        } break;
        case TOKEN_OPEN_PAREN:
        {
            AST* named_lambda = make_ast(AST_NAMED_LAMBDA);
            named_lambda->Named_Lambda.name = node->Atom.name;
            named_lambda->Named_Lambda.lambda = parse_expression(ctx);
            return named_lambda;
        } break;
    }
    return node;
}
inline INTERNAL AST*
parse_literal(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_LITERAL);
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

inline INTERNAL AST*
parse_unary(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_UNARY);
    Token_Kind k = kind(ctx);
    return node;
}

inline INTERNAL AST*
parse_lambda(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_LAMBDA);
    // eat(ctx);
    // node->Lambda.name = value(ctx);
    // eat(ctx);
    // node->Lambda.expr = parse_expression(ctx);
    return node;
}

inline INTERNAL AST*
parse_comment(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_COMMENT);
    node->Comment.text = value(ctx);
    eat(ctx);
    return node;
}

inline INTERNAL AST*
parse_grouping(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_GROUPING);
    eat(ctx);
    if (kind(ctx) != TOKEN_CLOSE_PAREN) 
        node->Grouping.expr = parse_expression(ctx);
    match(ctx, TOKEN_CLOSE_PAREN);
    return node;
}

inline INTERNAL AST*
parse_return(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_RETURN);
    eat(ctx);
    node->Return.expr = parse_expression(ctx);
    return node;
}
inline INTERNAL AST*
parse_block(Parse_Context* ctx)
{
    TRACE;
    AST* node = make_ast(AST_BLOCK);
    eat(ctx);
    node->Block.stmts = parse_delimited_list(ctx, parse_statement, TOKEN_DEDENT);
    eat(ctx);
    return node;
}

INTERNAL AST*
parse_primary(Parse_Context* ctx)
{
    TRACE;
    switch(kind(ctx))
    {
        ERROR_UNHANDLED_KIND(strf("%d", kind));
        case TOKEN_COMMENT:       return parse_comment(ctx);
        case TOKEN_ATOM:          return parse_atom(ctx);
        case TOKEN_OPEN_PAREN:    return parse_grouping(ctx);
        case TOKEN_INT:           FALLTHROUGH;
        case TOKEN_FLOAT:         FALLTHROUGH;
        case TOKEN_CHAR:          FALLTHROUGH;
        case TOKEN_STRING:        return parse_literal(ctx);
    }
    UNREACHABLE;
    return NULL;
}

INTERNAL AST*
parse_expression(Parse_Context* ctx)
{
    TRACE;
    switch(kind(ctx))
    {
        default: return parse_primary(ctx);
    }
    UNREACHABLE;
    return NULL;
}

INTERNAL AST*
parse_statement(Parse_Context* ctx)
{
    TRACE;
    switch(kind(ctx))
    {
        default: return parse_expression(ctx);
        case TOKEN_RETURN: return parse_return(ctx);
        case TOKEN_INDENT: return parse_block(ctx);
    }
    UNREACHABLE;
    return NULL;
}

INTERNAL AST*
parse(u8* file)
{ 
    u8* source = get_file_content(file);

    push_timer(file);

    Parse_Context ctx = (Parse_Context)
    {
        .file = file,
        .source = source,
        .current_indentation_level = 0,
        .previous_indentation_level = 0,
        .lines = 0,
        .comments = 0,
    };

    eat(&ctx); // prime the first token

    List* stmts = make_list();
    while (kind(&ctx))
    {
        AST* stmt = parse_statement(&ctx);
        list_append(stmts, stmt);
    }

    AST* module = make_ast(AST_MODULE);
    module->Module.name = file;
    module->Module.stmts = stmts;

    debug("%lld lines and %lld comments", ctx.lines, ctx.comments);

    pop_timer();

    return module;
}
