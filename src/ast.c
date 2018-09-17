#include "ast.h"

#include <stdlib.h>             // malloc,
#include <stdio.h>              // sprintf,
#include "stretchy_buffer.h"    // sb_push
#include "utility.h"            // info, success, error, warning
#include "lexer.h"              // token_kind_to_str,
#include <assert.h>             // assert

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

const char* expr_kind_to_str(Expr_Kind kind)
{
    switch (kind)
    {
        case EXPR_NOTE:      return "EXPR_NOTE";
        case EXPR_INT:       return "EXPR_INT";
        case EXPR_FLOAT:     return "EXPR_FLOAT";
        case EXPR_IDENT:     return "EXPR_IDENT";
        case EXPR_CALL:      return "EXPR_CALL";
        case EXPR_UNARY:     return "EXPR_UNARY";
        case EXPR_BINARY:    return "EXPR_BINARY";
        case EXPR_COMPOUND:  return "EXPR_COMPOUND";
        case EXPR_RET:       return "EXPR_RET";
        case EXPR_VAR_DECL:  return "EXPR_VAR_DECL";
        case EXPR_FUNC:      return "EXPR_FUNC";
        case EXPR_STRUCT:    return "EXPR_STRUCT";
        case EXPR_IF:        return "EXPR_IF";
        case EXPR_FOR:       return "EXPR_FOR";
        case EXPR_BLOCK:     return "EXPR_BLOCK";
        case EXPR_WHILE:     return "EXPR_WHILE";
        case EXPR_GROUPING:  return "EXPR_GROUPING";
    }
    return "print not implemented";
}

void print_expr(Expr* expr)
{
    info(expr_kind_to_str(expr->kind));
    switch (expr->kind)
    {
        case EXPR_NOTE: print_expr(expr->Note.expr); return;
        case EXPR_INT: info("%lld", expr->Int.val); return;
        case EXPR_FLOAT: info("%f", expr->Float.val); return;
        case EXPR_IDENT: info("%s", expr->Ident.name); return;
        case EXPR_CALL: warning("EXPR_CALL not print implemented."); return;
        case EXPR_UNARY: info("%c", expr->Unary.op); return;
        case EXPR_BINARY: info("%s", token_kind_to_str(expr->Binary.op)); return;
        case EXPR_COMPOUND: warning("EXPR_COMPOUND not print implemented."); return;
        case EXPR_RET: print_expr(expr->Ret.expr); return;
        case EXPR_VAR_DECL: warning("EXPR_VAR_DECL not print implemented."); return;
        case EXPR_FUNC: 
            print_type(expr->Func.type);
            print_expr(expr->Func.body);
            return;        
        case EXPR_STRUCT:
            warning("EXPR_STRUCT not print implemented."); return;
        case EXPR_IF: warning("EXPR_IF not print implemented."); return;
        case EXPR_FOR: warning("EXPR_FOR not print implemented."); return;
        case EXPR_BLOCK:
            for (int i = 0; i < sb_count(expr->Block.stmts); ++i)
            {
                print_expr(expr->Block.stmts[i]);   
            }
            return;
        case EXPR_WHILE: warning("EXPR_WHILE not print implemented."); return;
        case EXPR_GROUPING: warning("EXPR_GROUPING not print implemented."); return;
    }
    warning("print not implemented %s", expr_kind_to_str(expr->kind));
}

void print_ast(AST** ast)
{
    for (int i = 0; i < sb_count(ast); ++i)
    {
        print_expr(ast[i]);
    }
}

//------------------------------------------------------------------------------
//                               Expr Maker Functions
//------------------------------------------------------------------------------

Expr* make_expr(Expr_Kind kind)
{
    Expr* e = xmalloc(sizeof(Expr));
    e->kind = kind;
    return e;
}
Expr* make_expr_note(Expr* expr)
{
    assert(expr);
    Expr* e = make_expr(EXPR_NOTE);
    e->Note.expr = expr;
    return e;
}
Expr* make_expr_int(u64 value)
{
    Expr* e = make_expr(EXPR_INT);
    e->Int.val = value;
    return e;
}

Expr* make_expr_float(f64 value)
{
    Expr* e = make_expr(EXPR_FLOAT);
    e->Float.val = value;
    return e;
}

Expr* make_expr_ident(const char* ident)
{
    assert(ident);
    Expr* e = make_expr(EXPR_IDENT);
    e->Ident.name = ident;
    return e;
}
Expr* make_expr_struct(Typespec* struct_t)
{
    assert(struct_t);
    Expr* e = make_expr(EXPR_STRUCT);
    e->Struct.type = struct_t;
    return e;
}
Expr* make_expr_func(Typespec* func_t, Expr* body)
{
    assert(func_t);
    assert(body);
    Expr* e = make_expr(EXPR_FUNC);
    e->Func.type = func_t;
    e->Func.body = body;
    return e;
}

Expr* make_expr_binary(Token_Kind op, Expr* lhs, Expr* rhs)
{
    assert(op != TOKEN_UNKNOWN);
    assert(lhs);
    assert(rhs);
    Expr* e = make_expr(EXPR_BINARY);
    e->Binary.op = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

Expr* make_expr_unary(Token_Kind op, Expr* operand)
{
    assert(op != TOKEN_UNKNOWN);
    assert(operand);
    Expr* e = make_expr(EXPR_UNARY);
    e->Unary.op = op;
    e->Unary.operand = operand;
    return e;
}

Expr* make_expr_block(Expr** stmts)
{
    Expr* e = make_expr(EXPR_BLOCK);
    e->Block.stmts = stmts;
    return e;
}

Expr* make_expr_grouping(Expr* expr)
{
    assert(expr);
    Expr* e = make_expr(EXPR_GROUPING);
    e->Grouping.expr = expr;
    return e;
}

Expr* make_expr_ret(Expr* expr)
{
    assert(expr);
    Expr* e = make_expr(EXPR_RET);
    e->Ret.expr = expr;
    return e;
}
