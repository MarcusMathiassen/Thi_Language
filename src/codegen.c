#define CODEGEN_TRACE debug("[%s:%s:%d]: %s",  __FILE__, __func__, __LINE__, give_unique_color(ast_kind_to_str(node->kind)));

typedef struct
{
    string* data;
    string* text;
} codegen_ctx_t;

typedef struct
{
} value_t;

internal value_t* codegen(codegen_ctx_t* ctx, ast_t* node);

internal void
emit(codegen_ctx_t* ctx, u8* fmt, ...)
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

internal value_t* _codegen_ast_comment       (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    return NULL;
}
internal value_t* _codegen_ast_atom          (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    return NULL;
}
internal value_t* _codegen_ast_literal       (codegen_ctx_t* ctx, ast_t* node)
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
internal value_t* _codegen_ast_grouping      (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    if (node->Grouping.expr) codegen(ctx, node->Grouping.expr);
    return NULL;
}
internal value_t* _codegen_ast_unary         (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    return NULL;
}
internal value_t* _codegen_ast_lambda        (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    ast_t* params = node->Lambda.params;
    ast_t* block = node->Lambda.block;

    codegen(ctx, block);

    return NULL;
}
internal value_t* _codegen_ast_named_lambda  (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    char* name = node->Named_Lambda.name;
    emit(ctx, "_%s:", name);

    emit(ctx, "mov rbp, rsp");
    codegen(ctx, node->Named_Lambda.lambda);
    emit(ctx, "ret");
    return NULL;
}
internal value_t* _codegen_ast_return        (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    codegen(ctx, node->Return.expr);
    emit(ctx, "jmp .end");
    return NULL;
}
internal value_t* _codegen_ast_variable_decl (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    return NULL;
}
internal value_t* _codegen_ast_module        (codegen_ctx_t* ctx, ast_t* node)
{
    CODEGEN_TRACE;
    list_foreach(node->Module.stmts)
    {
        codegen(ctx, it->data);
    }
    return NULL;
}

global_variable value_t* (*codegen_transitions[])(codegen_ctx_t*, ast_t*) =
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
    [AST_MODULE]        = _codegen_ast_module,
};


internal value_t* codegen(codegen_ctx_t* ctx, ast_t* node)
{
    xassert(ctx);
    ast_kind_t kind = node->kind;
    value_t* (*func)(codegen_ctx_t*, ast_t*) = (*codegen_transitions[kind]);
    tassert(func, "codegen missing callback for %s", kind);
    return (*func)(ctx, node);
}

internal u8*
to_x64(ast_t* ast)
{
    codegen_ctx_t ctx = (codegen_ctx_t)
    {
        .data = make_string(""),
        .text = make_string(""),
    };

    codegen(&ctx, ast);

    char* code = strf("%s%s", string_data(ctx.data), string_data(ctx.text));
    debug("%s", code);
    string_destroy(ctx.data);
    string_destroy(ctx.text);

    return code;
}
