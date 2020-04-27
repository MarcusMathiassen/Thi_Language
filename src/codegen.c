#define CODEGEN_TRACE debug("[%s:%s:%d]: %s",  __FILE__, __func__, __LINE__, give_unique_color(ast_kind_to_str(node->kind)));

typedef struct
{
    string* data;
    string* text;
} Codegen_Context;

typedef struct
{
} Value;

INTERNAL Value* codegen(Codegen_Context* ctx, AST* node);

INTERNAL void
emit(Codegen_Context* ctx, u8* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char* tmp = vstrf(fmt, args);
    va_end(args);

    // we dont tab labels
    s64 len = strlen(tmp);
    bool is_label = false;
    foreach(i, len)
    {
        if (tmp[i] == ':')
        {
            is_label = true;
            break;
        }
    }
    string_append_f(ctx->text, is_label ? "%s\n" : "\t%s\n", tmp);
}

INTERNAL Value* _codegen_ast_comment       (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    return NULL;
}
INTERNAL Value* _codegen_ast_atom          (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    return NULL;
}
INTERNAL Value* _codegen_ast_literal       (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    switch(node->Literal.kind)
    {
        case AST_LITERAL_INT:
        {
            emit(ctx, "mov rax, %llu", node->Literal.Int.value);
        } break;
        case AST_LITERAL_FLOAT:
        {
            emit(ctx, "%f", node->Literal.Float.value);
        } break;
        case AST_LITERAL_CHAR:
        {
            emit(ctx, "%d", node->Literal.Char.value);
        } break;
        case AST_LITERAL_STRING:
        {
            emit(ctx, "%s", node->Literal.String.value);
        } break;
    }
    return NULL;
}
INTERNAL Value* _codegen_ast_grouping      (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    if (node->Grouping.expr) codegen(ctx, node->Grouping.expr);
    return NULL;
}
INTERNAL Value* _codegen_ast_unary         (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    return NULL;
}
INTERNAL Value* _codegen_ast_lambda        (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    AST* params = node->Lambda.params;
    AST* block = node->Lambda.block;

    codegen(ctx, block);

    return NULL;
}
INTERNAL Value* _codegen_ast_named_lambda  (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    char* name = node->Named_Lambda.name;
    emit(ctx, "_%s:", name);

    codegen(ctx, node->Named_Lambda.lambda);
    return NULL;
}
INTERNAL Value* _codegen_ast_return        (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    codegen(ctx, node->Return.expr);
    emit(ctx, "ret");
    return NULL;
}
INTERNAL Value* _codegen_ast_variable_decl (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    return NULL;
}
INTERNAL Value* _codegen_ast_module        (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    list_foreach(node->Module.stmts)
    {
        codegen(ctx, it->data);
    }
    return NULL;
}

INTERNAL Value* _codegen_ast_block        (Codegen_Context* ctx, AST* node)
{
    CODEGEN_TRACE;
    list_foreach(node->Block.stmts)
    {
        codegen(ctx, it->data);
    }
    return NULL;
}

GLOBAL_VARIABLE Value* (*codegen_transitions[])(Codegen_Context*, AST*) =
{
    [AST_COMMENT]       = _codegen_ast_comment,
    [AST_ATOM]          = _codegen_ast_atom,
    [AST_LITERAL]       = _codegen_ast_literal,
    [AST_GROUPING]      = _codegen_ast_grouping,
    [AST_UNARY]         = _codegen_ast_unary,
    [AST_LAMBDA]        = _codegen_ast_lambda,
    [AST_NAMED_LAMBDA]  = _codegen_ast_named_lambda,
    [AST_RETURN]        = _codegen_ast_return,
    [AST_VARIABLE_DECL] = _codegen_ast_variable_decl,
    [AST_BLOCK]         = _codegen_ast_block,
    [AST_MODULE]        = _codegen_ast_module,
};


INTERNAL Value* codegen(Codegen_Context* ctx, AST* node)
{
    xassert(ctx);
    AST_Kind kind = node->kind;
    Value* (*func)(Codegen_Context*, AST*) = (*codegen_transitions[kind]);
    tassert(func, "codegen missing callback for %s", ast_kind_to_str(kind));
    return (*func)(ctx, node);
}

INTERNAL u8*
to_x64(AST* ast)
{
    Codegen_Context ctx = (Codegen_Context)
    {
        .data = make_string(""),
        .text = make_string(""),
    };

    string_append(ctx.data, "section .data\n");
    string_append(ctx.text, "section .text\n");
    string_append(ctx.text, "global _main\n");

    codegen(&ctx, ast);

    char* code = strf("%s%s", string_data(ctx.data), string_data(ctx.text));
    debug("%s", code);

    string_destroy(ctx.data);
    string_destroy(ctx.text);

    return code;
}
