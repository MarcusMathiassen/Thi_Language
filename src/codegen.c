#include "codegen.h"
#include "typedefs.h"
#include "ast.h"
#include "utility.h" // error warning info, etc
#include "values.h" // Value
#include "stretchy_buffer.h"  // sb_free

char* output = NULL;

char** lines = NULL;

static void emit_load(Value* value)
{
    // const char* reg = get_eax_reg(value->get_size())
    // switch (value->kind)
    // {
    //     case VALUE_INTEGER_LITERAL: emit("MOV %s, %d", reg, value->Int.value;
    // }
}

static void codegen_expr(Expr* expr)
{
    switch (expr->kind)
    {
        case EXPR_NONE: error("EXPR_NONE codegen not implemented");
        case EXPR_INT:
        {
            emit(output, "MOV RAX, %llu", expr->Int.val);
            break;
        }
        case EXPR_FLOAT: error("EXPR_FLOAT codegen not implemented");
        case EXPR_IDENT: error("EXPR_IDENT codegen not implemented");
        case EXPR_CALL: error("EXPR_CALL codegen not implemented");
        case EXPR_UNARY: error("EXPR_UNARY codegen not implemented");
        case EXPR_BINARY: error("EXPR_BINARY codegen not implemented");
        case EXPR_COMPOUND: error("EXPR_COMPOUND codegen not implemented");
        case EXPR_RET: 
            codegen_expr(expr->Ret.expr);
            emit(output, "RET");
            break;
        case EXPR_VAR_DECL: error("EXPR_VAR_DECL codegen not implemented");
        case EXPR_FUNC: 
        {
            const char* func_name = expr->Func.type->Func.name;
            // Value* function = make_value_function(func_name);
            emit(output, "%s:", func_name);
            codegen_expr(expr->Func.body);
        } break;
        case EXPR_IF: error("EXPR_IF codegen not implemented");
        case EXPR_FOR: error("EXPR_FOR codegen not implemented");
        case EXPR_BLOCK: 
        {
            u64 stmts_count = sb_count(expr->Block.stmts);
            for (u64 i = 0; i < stmts_count; ++i)
            {
                codegen_expr(expr->Block.stmts[i]);   
            }
        } break;
        case EXPR_WHILE: error("EXPR_WHILE codegen not implemented");
        case EXPR_GROUPING: error("EXPR_GROUPING codegen not implemented");
        default: error("unknown kind codegen not implemented");
    }
}

char* generate_code_from_ast(AST** ast)
{ 
    output = xmalloc(0);

    emit(output, "global main");
    emit(output, "section .text");
    
    // Codegen AST
    u64 ast_count = sb_count(ast);
    for (u64 i = 0; i < ast_count; ++i)
    {
        Expr* expr = ast[i]; 
        codegen_expr(expr);
    }
    return output;
}
