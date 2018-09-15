#include "codegen.h"
#include "typedefs.h"
#include "ast.h"
#include "utility.h" // error warning info, etc
#include "stretchy_buffer.h"  // sb_free



char* output = NULL;

static void codegen_expr(Expr* expr)
{
    switch (expr->kind)
    {
        case EXPR_NONE: error("EXPR_NONE codegen not implemented");
        case EXPR_INT: error("EXPR_INT codegen not implemented");
        case EXPR_FLOAT: error("EXPR_FLOAT codegen not implemented");
        case EXPR_IDENT: error("EXPR_IDENT codegen not implemented");
        case EXPR_CALL: error("EXPR_CALL codegen not implemented");
        case EXPR_UNARY: error("EXPR_UNARY codegen not implemented");
        case EXPR_BINARY: error("EXPR_BINARY codegen not implemented");
        case EXPR_COMPOUND: error("EXPR_COMPOUND codegen not implemented");
        case EXPR_RET: error("EXPR_RET codegen not implemented");
        case EXPR_VAR_DECL: error("EXPR_VAR_DECL codegen not implemented");
        case EXPR_FUNC: error("EXPR_FUNC codegen not implemented");
        case EXPR_IF: error("EXPR_IF codegen not implemented");
        case EXPR_FOR: error("EXPR_FOR codegen not implemented");
        case EXPR_BLOCK: error("EXPR_BLOCK codegen not implemented");
        case EXPR_WHILE: error("EXPR_WHILE codegen not implemented");
        case EXPR_GROUPING: error("EXPR_GROUPING codegen not implemented");
        default: error("unknown kind codegen not implemented");
    }
}

char* generate_code_from_ast(AST** ast)
{ 

    output = NULL;
    int ast_count = sb_count(ast);
    for (int i = 0; i < ast_count; ++i)
    {
        Expr* expr = ast[i]; 
        codegen_expr(expr);
    }

    return output;
}
