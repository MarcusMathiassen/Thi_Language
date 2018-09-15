#include "codegen.h"
#include "typedefs.h"
#include "ast.h"
#include "globals.h"
#include "utility.h" // error warning info, etc
#include "values.h" // Value
#include "stretchy_buffer.h"  // sb_free

char* output = NULL;
char** lines = NULL;

int stack_index = 0;

static Value codegen_expr(Expr* expr);

static void emit_load(Value* value)
{
    // const char* reg = get_eax_reg(value->get_size())
    // switch (value->kind)
    // {
    //     case VALUE_INTEGER_LITERAL: emit("MOV %s, %d", reg, value->Int.value;
    // }
}

static void codegen_function(Expr* expr)
{
    const char* func_name = expr->Func.type->Func.name;
    info("Generating function: %s", func_name);

    Value function = make_value_function(func_name);
    Value block = make_value_block(&function, "entry");


    emit(output, "%s:", func_name);
    codegen_expr(expr->Func.body);

    // Allocate stack for parameters
    int index = 0;
    int stack_before_func = stack_index;

    Arg* args = expr->Func.type->Func.args;
    int arg_count = sb_count(args);
    if (arg_count) info("Printing function parameters");
    for (int i = 0; i < arg_count; ++i)
    {
        Arg arg = args[i];
        info("Parameter %d name: %s", i, arg.name);
        print_type(arg.type);

        int size = get_size_of_type(arg.type);
        info("size: %d", size);
    }
}

static Value codegen_ident(Expr* expr)
{
    const char* name = expr->Ident.name;

    // Value* var = get_variable(name)
    // emit_load(var);

    // return var;
    error("EXPR_IDENT codegen not implemented");
}
static Value codegen_int(Expr* expr)
{
    u64 value = expr->Int.val;
    emit(output, "MOV RAX, %llu", value);
    return make_value_int(DEFAULT_INTEGER_BIT_SIZE, value);
}

static void codegen_block(Expr* expr)
{
    Expr** stmts = expr->Block.stmts;
    u64 stmts_count = sb_count(stmts);
    for (u64 i = 0; i < stmts_count; ++i)
    {
        codegen_expr(stmts[i]);
    }
}
static void codegen_ret(Expr* expr)
{
    Expr* ret_expr = expr->Ret.expr;
    codegen_expr(ret_expr);
    emit(output, "RET");
}

static Value codegen_expr(Expr* expr)
{
    info("Generating code for: %s", expr_kind_to_str(expr->kind));
    switch (expr->kind)
    {
        case EXPR_INT:      return codegen_int(expr); break;
        case EXPR_FLOAT:    error("EXPR_FLOAT codegen not implemented");
        case EXPR_IDENT:    return codegen_ident(expr); break;
        case EXPR_CALL:     error("EXPR_CALL codegen not implemented");
        case EXPR_UNARY:    error("EXPR_UNARY codegen not implemented");
        case EXPR_BINARY:   error("EXPR_BINARY codegen not implemented");
        case EXPR_COMPOUND: error("EXPR_COMPOUND codegen not implemented");
        case EXPR_RET:      codegen_ret(expr); break;
        case EXPR_VAR_DECL: error("EXPR_VAR_DECL codegen not implemented");
        case EXPR_FUNC:     codegen_function(expr); break;
        case EXPR_IF:       error("EXPR_IF codegen not implemented");
        case EXPR_FOR:      error("EXPR_FOR codegen not implemented");
        case EXPR_BLOCK:    codegen_block(expr); break;
        case EXPR_WHILE:    error("EXPR_WHILE codegen not implemented");
        case EXPR_GROUPING: error("EXPR_GROUPING codegen not implemented");
        
        default:            error("unknown kind codegen not implemented");
    }

    Value t;
    return t;
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
        codegen_expr(ast[i]);
    }
    return output;
}
