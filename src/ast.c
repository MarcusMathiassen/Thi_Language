#include "ast.h"

#include <stdlib.h>           // malloc,
#include "stretchy_buffer.h"  // sb_push
#include "utility.h"          // info, success, error, warning

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------
static Expr* alloc_expr();

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

void print_expr(Expr* expr) {
    switch (expr->kind) {
        case EXPR_NONE: warning("EXPR_NONE print not implemented"); break;
        case EXPR_INT: info("EXPR_INT: %lld", expr->Int.val); break;
        case EXPR_FLOAT: info("EXPR_FLOAT: %f", expr->Float.val); break;
        case EXPR_WHILE: warning("EXPR_WHILE print not implemented"); break;
        case EXPR_FOR: warning("EXPR_FOR print not implemented"); break;
        case EXPR_IF: warning("EXPR_IF print not implemented"); break;
        case EXPR_COMPOUND: warning("EXPR_COMPOUND print not implemented"); break;
        case EXPR_CALL: warning("EXPR_CALL print not implemented"); break;
        case EXPR_VAR_DECL: warning("EXPR_VAR_DECL print not implemented"); break;
        case EXPR_IDENT: info("EXPR_IDENT: %s", expr->Ident.name); break;
        case EXPR_FUNC: info("EXPR_FUNC: %s", expr->Func.type->Func.name); break;
        case EXPR_UNARY: info("EXPR_UNARY: %c", expr->Unary.op); break;
        case EXPR_BINARY: info("EXPR_BINARY: %c", expr->Binary.op); break;
        default: warning("print not implemented");
    }
}

void print_ast(AST** ast) {
    for (int i = 0; i < sb_count(ast); ++i) {
        print_expr(ast[i]);
    }
}

//------------------------------------------------------------------------------
//                               Expr Maker Functions
//------------------------------------------------------------------------------

Expr* make_expr_int(int value) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_INT;
    temp->Int.val = value;
    return temp;
}

Expr* make_expr_float(float value) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_FLOAT;
    temp->Float.val = value;
    return temp;
}

Expr* make_expr_ident(const char* ident) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_IDENT;
    temp->Ident.name = ident;
    return temp;
}

Expr* make_expr_func(Type* func_t, Expr* body) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_FUNC;
    temp->Func.type = func_t;
    temp->Func.body = body;
    return temp;
}

Expr* make_expr_binary(char op, Expr* lhs, Expr* rhs) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_BINARY;
    temp->Binary.op = op;
    temp->Binary.lhs = lhs;
    temp->Binary.rhs = rhs;
    return temp;
}

Expr* make_expr_unary(char op, Expr* operand) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_UNARY;
    temp->Unary.op = op;
    temp->Unary.operand = operand;
    return temp;
}

Expr* make_expr_block(Expr** stmts) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_BLOCK;
    temp->Block.stmts = stmts;
    return temp;
}

Expr* make_expr_grouping(Expr* expr) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_GROUPING;
    temp->Grouping.expr = expr;
    return temp;
}
Expr* make_expr_ret(Expr* expr) {
    Expr* temp = alloc_expr();
    temp->kind = EXPR_RET;
    temp->Ret.expr = expr;
    return temp;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

static Expr* alloc_expr() {
    Expr* temp = (Expr*)malloc(sizeof(Expr));
    if (!temp) error("Out of memory trying to allocate expression.");
    return temp;
}
