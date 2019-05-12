#include "type_checker.h"
#include "ast.h"
#include "constants.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert
#include <string.h> // strcmp

#define UNFINISHED                                                                                                     \
    error("[UNFINISHED] %s: %s", give_unique_color((char *)__func__), wrap_with_colored_parens(ast_to_str(expr)));

#define DEBUG_START                                                                                                    \
    info("%s: %s", give_unique_color((char *)__func__), wrap_with_colored_parens(ast_to_str(expr)));                   \
    assert(expr);

typedef struct {
    Map * symbol_table;
    AST * active_function;
    Type *expected_type;
} Typer_Context;

Type *type_check_expr(Typer_Context *ctx, AST *expr);

Type *type_check_switch(Typer_Context *ctx, AST *expr);
Type *type_check_extern(Typer_Context *ctx, AST *expr);
Type *type_check_struct(Typer_Context *ctx, AST *expr);
Type *type_check_enum(Typer_Context *ctx, AST *expr);
Type *type_check_function(Typer_Context *ctx, AST *expr);
Type *type_check_note(Typer_Context *ctx, AST *expr);
Type *type_check_int(Typer_Context *ctx, AST *expr);
Type *type_check_float(Typer_Context *ctx, AST *expr);
Type *type_check_string(Typer_Context *ctx, AST *expr);
Type *type_check_ident(Typer_Context *ctx, AST *expr);
Type *type_check_call(Typer_Context *ctx, AST *expr);
Type *type_check_unary(Typer_Context *ctx, AST *expr);
Type *type_check_binary(Typer_Context *ctx, AST *expr);
Type *type_check_variable_decl(Typer_Context *ctx, AST *expr);
Type *type_check_constant_decl(Typer_Context *ctx, AST *expr);
Type *type_check_block(Typer_Context *ctx, AST *expr);
Type *type_check_subscript(Typer_Context *ctx, AST *expr);
Type *type_check_field_access(Typer_Context *ctx, AST *expr);
Type *type_check_if(Typer_Context *ctx, AST *expr);
Type *type_check_for(Typer_Context *ctx, AST *expr);
Type *type_check_while(Typer_Context *ctx, AST *expr);
Type *type_check_return(Typer_Context *ctx, AST *expr);
Type *type_check_defer(Typer_Context *ctx, AST *expr);
Type *type_check_break(Typer_Context *ctx, AST *expr);
Type *type_check_continue(Typer_Context *ctx, AST *expr);
Type *type_check_cast(Typer_Context *ctx, AST *expr);
Type *type_check_is(Typer_Context *ctx, AST *expr);

void type_checker(Map *symbol_table, List *ast) {
    info("Type checking...");

    Typer_Context ctx;
    ctx.symbol_table = symbol_table;

    LIST_FOREACH(ast) {
        type_check_expr(&ctx, (AST *)it->data);
    }
    // print_ast(ast);
    // error("Type Checker DEBUG ,,, ,, ,,, ,,");
}

Type *type_check_expr(Typer_Context *ctx, AST *expr) {
    if (!expr) return NULL;
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
    case AST_CAST: return type_check_cast(ctx, expr);
    case AST_IS: return type_check_is(ctx, expr);
    default: error("Unhandled %s case for kind '%s'", give_unique_color((char *)__func__), ast_to_str(expr));
    }
    return NULL;
}
Type *type_check_switch(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->Switch.cond);
    type_check_expr(ctx, expr->Switch.cases);
    type_check_expr(ctx, expr->Switch.default_case);
    return NULL;
}
Type *type_check_extern(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    expr->type = expr->Extern.type;
    return NULL;
}
Type *type_check_struct(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    LIST_FOREACH(expr->Struct.type->Struct.members) {
        type_check_expr(ctx, it->data);
    }
    expr->type = expr->Struct.type;
    return NULL;
}
Type *type_check_enum(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    LIST_FOREACH(expr->Enum.type->Enum.members) {
        type_check_expr(ctx, it->data);
    }
    expr->type = expr->Enum.type;
    return NULL;
}
Type *type_check_function(Typer_Context *ctx, AST *expr) {
    DEBUG_START;

    Type *func_type = expr->Function.type;
    AST * func_body = expr->Function.body;
    List *args      = func_type->Function.args;

    ctx->active_function = expr;

    LIST_FOREACH(args) {
        AST *arg = (AST *)it->data;
        type_check_expr(ctx, arg);
    }

    type_check_expr(ctx, func_body);

    expr->type->Function.ret_type = expr->type->Function.ret_type;

    ctx->active_function = NULL;

    return NULL;
}
Type *type_check_note(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    return NULL;
}
Type *type_check_int(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    Type *t    = (Type *)map_get(ctx->symbol_table, DEFAULT_INT_TYPE_AS_STRING);
    expr->type = t;
    return t;
}
Type *type_check_float(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    Type *t    = (Type *)map_get(ctx->symbol_table, DEFAULT_FLOAT_TYPE_AS_STRING);
    expr->type = t;
    return t;
}
Type *type_check_string(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    Type *t    = make_type_pointer(make_type_int(8, 1));
    expr->type = t;
    return t;
}
Type *type_check_ident(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    Type *t    = (Type *)map_get(ctx->symbol_table, expr->Ident.name);
    expr->type = t;
    return t;
}
Type *type_check_call(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    char *callee = expr->Call.callee;
    List *args   = expr->Call.args;

    Type *func_t = (Type *)map_get(ctx->symbol_table, callee);

    LIST_FOREACH(args) {
        type_check_expr(ctx, it->data);
    }

    expr->type = func_t;

    // temp
    /* Typecheck the args */
    assert(args);

    return func_t;
}
Type *type_check_unary(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    Token_Kind op      = expr->Unary.op;
    AST *      operand = expr->Unary.operand;

    switch (op) {
    case TOKEN_DOT: {
        if (ctx->expected_type->kind == TYPE_ENUM) {
            Type *enum_t = ctx->expected_type;
            assert(operand->kind == AST_IDENT);
            LIST_FOREACH(enum_t->Enum.members) {
                AST *mem = (AST *)it->data;
                if (strcmp(operand->Ident.name, mem->Constant_Decl.name) == 0) {
                    *expr = *make_ast_binary(mem->t, TOKEN_DOT, make_ast_ident(mem->t, enum_t->Enum.name), operand);
                }
            }
        }
    } break;
    default: type_check_expr(ctx, operand); break;
    }

    return NULL;
}
Type *type_check_binary(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    AST *lhs = expr->Binary.lhs;
    AST *rhs = expr->Binary.rhs;
    type_check_expr(ctx, lhs);
    type_check_expr(ctx, rhs);

    // We assume they are the same type
    expr->type = lhs->type;

    return NULL;
}
Type *type_check_variable_decl(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    char *name            = expr->Variable_Decl.name;
    Type *type            = expr->Variable_Decl.type;
    AST * assignment_expr = expr->Variable_Decl.value;
    assert(name);

    // if the variable has no assigned type. We infer it based on the
    // assignment expression.
    if (!type) {
        // It must have an assignment expression.
        assert(assignment_expr);
        type_check_expr(ctx, assignment_expr);
        type = assignment_expr->type;
    } else {
        ctx->expected_type = type;
        if (assignment_expr) {
            type_check_expr(ctx, assignment_expr);
            // assert(type->kind == assignment_expr->type->kind);
        }
    }
    ctx->expected_type = type;
    expr->type         = type;

    map_set_overwrite(ctx->symbol_table, name, type);

    return NULL;
}
Type *type_check_constant_decl(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->Constant_Decl.value);
    expr->type = expr->Constant_Decl.value->type;
    return NULL;
}
Type *type_check_block(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    List *stmts = expr->Block.stmts;
    LIST_FOREACH(stmts) {
        AST *stmt = (AST *)it->data;
        type_check_expr(ctx, stmt);
    }
    return NULL;
}
Type *type_check_subscript(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}
Type *type_check_field_access(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    char *type_name  = expr->Field_Access.load->Ident.name;
    char *field_name = expr->Field_Access.field;
    warning("field: %s type: %s", field_name, type_name);

    Type *t = map_get(ctx->symbol_table, type_name);

    warning("looking for %s", field_name);

    LIST_FOREACH(t->Enum.members) {
        AST *mem = it->data;
        warning("on %s", ast_to_str(mem));
        type_check_expr(ctx, mem);
        if (strcmp(mem->Constant_Decl.name, field_name) == 0) {
            *expr      = *mem->Constant_Decl.value;
            expr->type = mem->type;
        }
    }

    return NULL;
}
Type *type_check_if(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    AST *cond       = expr->If.cond;
    AST *then_block = expr->If.then_block;
    AST *else_block = expr->If.else_block;
    type_check_expr(ctx, cond);
    ctx->expected_type = cond->type;
    type_check_expr(ctx, then_block);
    type_check_expr(ctx, else_block);
    return NULL;
}
Type *type_check_for(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->For.init);
    type_check_expr(ctx, expr->For.cond);
    type_check_expr(ctx, expr->For.step);
    type_check_expr(ctx, expr->For.then_block);
    return NULL;
}
Type *type_check_while(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->While.cond);
    type_check_expr(ctx, expr->While.then_block);
    return NULL;
}
Type *type_check_return(Typer_Context *ctx, AST *expr) {
    DEBUG_START;

    AST *ret_expr = expr->Return.expr;
    type_check_expr(ctx, ret_expr);
    expr->type = ret_expr->type;

    assert(ctx->active_function);

    ctx->active_function->Function.type->Function.ret_type = expr->type;
    ctx->active_function->Function.body->type              = expr->type;
    return NULL;
}
Type *type_check_defer(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->Defer.expr);
    return NULL;
}
Type *type_check_break(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->Break.expr);
    return NULL;
}
Type *type_check_continue(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->Continue.expr);
    return NULL;
}
Type *type_check_cast(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

Type *type_check_is(Typer_Context *ctx, AST *expr) {
    DEBUG_START;
    type_check_expr(ctx, expr->Is.body);
    type_check_expr(ctx, expr->Is.expr);
    warning("BODY ::: %s", ast_to_str(expr->Is.body));
    warning("EXPR ::: %s", ast_to_str(expr->Is.expr));
    return NULL;
}
