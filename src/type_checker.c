#include "type_checker.h"
#include "ast.h"
#include "constants.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert

#define DEBUG_START                                                                                                    \
    info("%s: %s", give_unique_color((char*)__func__), wrap_with_colored_parens(ast_to_str(expr)));                    \
    assert(expr);

typedef struct {
    Map* symbol_table;
    AST* active_block;
} Typer_Context;

Type* type_check_expr(Typer_Context* ctx, AST* expr);

Type* type_check_switch(Typer_Context* ctx, AST* expr);
Type* type_check_extern(Typer_Context* ctx, AST* expr);
Type* type_check_struct(Typer_Context* ctx, AST* expr);
Type* type_check_enum(Typer_Context* ctx, AST* expr);
Type* type_check_function(Typer_Context* ctx, AST* expr);
Type* type_check_note(Typer_Context* ctx, AST* expr);
Type* type_check_int(Typer_Context* ctx, AST* expr);
Type* type_check_float(Typer_Context* ctx, AST* expr);
Type* type_check_string(Typer_Context* ctx, AST* expr);
Type* type_check_ident(Typer_Context* ctx, AST* expr);
Type* type_check_call(Typer_Context* ctx, AST* expr);
Type* type_check_unary(Typer_Context* ctx, AST* expr);
Type* type_check_binary(Typer_Context* ctx, AST* expr);
Type* type_check_variable_decl(Typer_Context* ctx, AST* expr);
Type* type_check_constant_decl(Typer_Context* ctx, AST* expr);
Type* type_check_block(Typer_Context* ctx, AST* expr);
Type* type_check_subscript(Typer_Context* ctx, AST* expr);
Type* type_check_field_access(Typer_Context* ctx, AST* expr);
Type* type_check_if(Typer_Context* ctx, AST* expr);
Type* type_check_for(Typer_Context* ctx, AST* expr);
Type* type_check_while(Typer_Context* ctx, AST* expr);
Type* type_check_return(Typer_Context* ctx, AST* expr);
Type* type_check_defer(Typer_Context* ctx, AST* expr);
Type* type_check_break(Typer_Context* ctx, AST* expr);
Type* type_check_continue(Typer_Context* ctx, AST* expr);
Type* type_check_cast(Typer_Context* ctx, AST* expr);

void type_checker(Map* symbol_table, List* ast) {
    info("Type checking...");

    Typer_Context ctx;
    ctx.symbol_table = symbol_table;

    LIST_FOREACH(ast) { type_check_expr(&ctx, (AST*)it->data); }

    print_ast(ast);
    error("Type Checker DEBUG ,,, ,, ,,, ,,");
}

Type* type_check_expr(Typer_Context* ctx, AST* expr) {
    if (!expr) error("expr is null");
    switch (expr->kind) {
    case AST_FALLTHROUGH: return NULL;
    case AST_LOAD: return NULL;
    case AST_LINK: return NULL;
    case AST_SWITCH: return type_check_switch(ctx, expr);
    case AST_EXTERN: return type_check_extern(ctx, expr);
    case AST_STRUCT: return type_check_struct(ctx, expr);
    case AST_ENUM: return type_check_enum(ctx, expr);
    case AST_FUNCTION: return type_check_function(ctx, expr);
    case AST_NOTE: return type_check_note(ctx, expr);
    case AST_INT: return type_check_int(ctx, expr);
    case AST_FLOAT: return type_check_float(ctx, expr);
    case AST_STRING: return type_check_string(ctx, expr);
    case AST_IDENT: return type_check_ident(ctx, expr);
    case AST_CALL: return type_check_call(ctx, expr);
    case AST_UNARY: return type_check_unary(ctx, expr);
    case AST_BINARY: return type_check_binary(ctx, expr);
    case AST_VARIABLE_DECL: return type_check_variable_decl(ctx, expr);
    case AST_CONSTANT_DECL: return type_check_constant_decl(ctx, expr);
    case AST_BLOCK: return type_check_block(ctx, expr);
    case AST_GROUPING: return type_check_expr(ctx, expr->Grouping.expr);
    case AST_SUBSCRIPT: return type_check_subscript(ctx, expr);
    case AST_FIELD_ACCESS: return type_check_field_access(ctx, expr);
    case AST_IF: return type_check_if(ctx, expr);
    case AST_FOR: return type_check_for(ctx, expr);
    case AST_WHILE: return type_check_while(ctx, expr);
    case AST_RETURN: return type_check_return(ctx, expr);
    case AST_DEFER: return type_check_defer(ctx, expr);
    case AST_BREAK: return type_check_break(ctx, expr);
    case AST_CONTINUE: return type_check_continue(ctx, expr);
    case AST_IS: return NULL;
    case AST_VAR_ARGS: return NULL;
    case AST_SIZEOF: return NULL;
    case AST_CAST: return type_check_cast(ctx, expr);
    default: error("Unhandled %s case for kind '%s'", give_unique_color((char*)__func__), ast_kind_to_str(expr->kind));
    }
    return NULL;
}
Type* type_check_switch(Typer_Context* ctx, AST* expr) {
    DEBUG_START;

    AST* cond         = expr->Switch.cond;
    AST* cases        = expr->Switch.cases;
    AST* default_case = expr->Switch.default_case;

    type_check_expr(ctx, cond);
    type_check_expr(ctx, default_case);

    LIST_FOREACH(cases->Block.stmts) {
        AST* c = (AST*)it->data;
        type_check_expr(ctx, c->Is.body);
    }

    return NULL;
}
Type* type_check_extern(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_struct(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_enum(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_function(Typer_Context* ctx, AST* expr) {
    DEBUG_START;

    Type* func_type = expr->Function.type;
    AST*  func_body = expr->Function.body;
    List* args      = func_type->Function.args;

    LIST_FOREACH(args) {
        AST* arg = (AST*)it->data;
        type_check_expr(ctx, arg);
    }

    type_check_expr(ctx, func_body);
    func_type->Function.ret_type = func_body->type;

    return NULL;
}
Type* type_check_note(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_int(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    expr->type = (Type*)map_get(ctx->symbol_table, DEFAULT_INT_TYPE_AS_STRING);
    return NULL;
}
Type* type_check_float(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    expr->type = (Type*)map_get(ctx->symbol_table, DEFAULT_FLOAT_TYPE_AS_STRING);
    return NULL;
}
Type* type_check_string(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    Type* t = make_type_pointer(make_type_int(8, 1));
    return t;
}
Type* type_check_ident(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    expr->type = (Type*)map_get(ctx->symbol_table, expr->Ident.name);
    warning("identifier: %s type: %s", ast_to_str(expr), type_to_str(expr->type));
    return NULL;
}
Type* type_check_call(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    char* callee   = expr->Call.callee;
    List* args     = expr->Call.args;
    Type* ret_type = expr->type;

    Type* func_t = (Type*)map_get(ctx->symbol_table, callee);

    expr->type = func_t->Function.ret_type;

    assert(callee);
    assert(args);
    assert(ret_type);

    return NULL;
}
Type* type_check_unary(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_binary(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    AST* lhs = expr->Binary.lhs;
    AST* rhs = expr->Binary.rhs;
    type_check_expr(ctx, lhs);
    type_check_expr(ctx, rhs);

    // We assume they are the same type
    expr->type = lhs->type;

    return NULL;
}
Type* type_check_variable_decl(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    char* name            = expr->Variable_Decl.name;
    Type* type            = expr->Variable_Decl.type;
    AST*  assignment_expr = expr->Variable_Decl.value;
    assert(name);

    // if the variable has no assigned type. We infer it based on the
    // assignment expression.
    if (!type) {
        // It must have an assignment expression.
        assert(assignment_expr);
        type_check_expr(ctx, assignment_expr);
        type = assignment_expr->type;
    } else {
        if (assignment_expr) {
            type_check_expr(ctx, assignment_expr);
            // assert(type->kind == assignment_expr->type->kind);
        }
    }
    expr->type = type;
    return NULL;
}
Type* type_check_constant_decl(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_block(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    ctx->active_block = expr;
    List* stmts       = expr->Block.stmts;
    LIST_FOREACH(stmts) {
        AST* stmt = (AST*)it->data;
        type_check_expr(ctx, stmt);
    }
    ctx->active_block = NULL;
    return NULL;
}
Type* type_check_subscript(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_field_access(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_if(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_for(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_while(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_return(Typer_Context* ctx, AST* expr) {
    DEBUG_START;

    AST* ret_expr = expr->Return.expr;
    type_check_expr(ctx, ret_expr);
    expr->type = ret_expr->type;

    assert(ctx->active_block);
    ctx->active_block->type = expr->type;
    return NULL;
}
Type* type_check_defer(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_break(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_continue(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_cast(Typer_Context* ctx, AST* expr) {
    DEBUG_START;
    return NULL;
}
