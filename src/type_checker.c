#include "type_checker.h"
#include "ast.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert

#define DEBUG_START                                                                                                    \
    info("%s: %s", give_unique_color(__func__), wrap_with_colored_parens(ast_to_str(expr)));                           \
    assert(expr);

Type* type_check_expr(AST* expr);

Type* type_check_switch(AST* expr);
Type* type_check_extern(AST* expr);
Type* type_check_struct(AST* expr);
Type* type_check_enum(AST* expr);
Type* type_check_function(AST* expr);
Type* type_check_note(AST* expr);
Type* type_check_int(AST* expr);
Type* type_check_float(AST* expr);
Type* type_check_string(AST* expr);
Type* type_check_ident(AST* expr);
Type* type_check_call(AST* expr);
Type* type_check_unary(AST* expr);
Type* type_check_binary(AST* expr);
Type* type_check_variable_decl(AST* expr);
Type* type_check_constant_decl(AST* expr);
Type* type_check_block(AST* expr);
Type* type_check_subscript(AST* expr);
Type* type_check_field_access(AST* expr);
Type* type_check_if(AST* expr);
Type* type_check_for(AST* expr);
Type* type_check_while(AST* expr);
Type* type_check_return(AST* expr);
Type* type_check_defer(AST* expr);
Type* type_check_break(AST* expr);
Type* type_check_continue(AST* expr);
Type* type_check_cast(AST* expr);

void type_checker(List* ast) {
    LIST_FOREACH(ast) { type_check_expr((AST*)it->data); }
    error("Type Checker END");
}

Type* type_check_expr(AST* expr) {
    switch (expr->kind) {
    case AST_FALLTHROUGH: return NULL;
    case AST_LOAD: return NULL;
    case AST_LINK: return NULL;
    case AST_SWITCH: return type_check_switch(expr);
    case AST_EXTERN: return type_check_extern(expr);
    case AST_STRUCT: return type_check_struct(expr);
    case AST_ENUM: return type_check_enum(expr);
    case AST_FUNCTION: return type_check_function(expr);
    case AST_NOTE: return type_check_note(expr);
    case AST_INT: return type_check_int(expr);
    case AST_FLOAT: return type_check_float(expr);
    case AST_STRING: return type_check_string(expr);
    case AST_IDENT: return type_check_ident(expr);
    case AST_CALL: return type_check_call(expr);
    case AST_UNARY: return type_check_unary(expr);
    case AST_BINARY: return type_check_binary(expr);
    case AST_VARIABLE_DECL: return type_check_variable_decl(expr);
    case AST_CONSTANT_DECL: return type_check_constant_decl(expr);
    case AST_BLOCK: return type_check_block(expr);
    case AST_GROUPING: return type_check_expr(expr->Grouping.expr);
    case AST_SUBSCRIPT: return type_check_subscript(expr);
    case AST_FIELD_ACCESS: return type_check_field_access(expr);
    case AST_IF: return type_check_if(expr);
    case AST_FOR: return type_check_for(expr);
    case AST_WHILE: return type_check_while(expr);
    case AST_RETURN: return type_check_return(expr);
    case AST_DEFER: return type_check_defer(expr);
    case AST_BREAK: return type_check_break(expr);
    case AST_CONTINUE: return type_check_continue(expr);
    case AST_CAST: return type_check_cast(expr);
    default: error("Unhandled %s case for kind '%s'", give_unique_color(__func__), ast_kind_to_str(expr->kind));
    }
    return NULL;
}
Type* type_check_switch(AST* expr) {
    DEBUG_START;

    AST* cond         = expr->Switch.cond;
    AST* cases        = expr->Switch.cases;
    AST* default_case = expr->Switch.default_case;

    type_check_expr(cond);
    type_check_expr(default_case);

    LIST_FOREACH(cases->Block.stmts) {
        AST* c = (AST*)it->data;
        type_check_expr(c->Is.body);
    }

    return NULL;
}
Type* type_check_extern(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_struct(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_enum(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_function(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_note(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_int(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_float(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_string(AST* expr) {
    DEBUG_START;
    Type* t = make_type_pointer(make_type_int(8, 1));
    return t;
}
Type* type_check_ident(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_call(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_unary(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_binary(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_variable_decl(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_constant_decl(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_block(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_subscript(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_field_access(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_if(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_for(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_while(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_return(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_defer(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_break(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_continue(AST* expr) {
    DEBUG_START;
    return NULL;
}
Type* type_check_cast(AST* expr) {
    DEBUG_START;
    return NULL;
}
