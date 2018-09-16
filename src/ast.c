#include "ast.h"

#include <stdlib.h>           // malloc,
#include "stretchy_buffer.h"  // sb_push
#include "utility.h"          // info, success, error, warning
#include "lexer.h"          // token_kind_to_str,

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------
static Expr* alloc_expr();

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

const char* expr_kind_to_str(Expr_Kind kind)
{
    switch (kind)
    {
        case EXPR_NOTE: return "EXPR_NOTE";
        case EXPR_INT: return "EXPR_INT";
        case EXPR_FLOAT: return "EXPR_FLOAT";
        case EXPR_IDENT: return "EXPR_IDENT";
        case EXPR_CALL: return "EXPR_CALL";
        case EXPR_UNARY: return "EXPR_UNARY";
        case EXPR_BINARY: return "EXPR_BINARY";
        case EXPR_COMPOUND: return "EXPR_COMPOUND";
        case EXPR_RET: return "EXPR_RET";
        case EXPR_VAR_DECL: return "EXPR_VAR_DECL";
        case EXPR_FUNC: return "EXPR_FUNC";
        case EXPR_IF: return "EXPR_IF";
        case EXPR_FOR: return "EXPR_FOR";
        case EXPR_BLOCK: return "EXPR_BLOCK";
        case EXPR_WHILE: return "EXPR_WHILE";
        case EXPR_GROUPING: return "EXPR_GROUPING";
    }
    return "print not implemented";
}

char* expr_to_str(char* str, Expr* expr)
{
    char* buffer = xmalloc(1000);

    switch (expr->kind)
    {
        case EXPR_INT: sprintf(buffer, "%llu", expr->Int.val); break;
        case EXPR_UNARY: 
        {
            sprintf(buffer, "%s%s",
            token_kind_to_str(expr->Unary.op),
            expr_to_str(buffer, expr->Unary.operand));
        } break;
        case EXPR_BINARY: 
        {
            sprintf(buffer, "%s %s %s",
                expr_to_str(buffer, expr->Binary.lhs), 
                token_kind_to_str(expr->Binary.op),
                expr_to_str(buffer, expr->Binary.rhs));
        } break;
        case EXPR_RET: 
            sprintf(buffer, "ret %s", expr_to_str(buffer, expr->Ret.expr));
            break;
        case EXPR_FUNC: 
        {
            sprintf(buffer, "%s :: () ", expr->Func.type->Func.name);
            expr_to_str(buffer, expr->Func.body);
        } break;
        case EXPR_BLOCK:
        {
            for (int i = 0; i < sb_count(expr->Block.stmts); ++i)
            {
                expr_to_str(buffer, expr->Block.stmts[i]);
            }
            sprintf(buffer, "{\n  %s\n}");
        } break;
        case EXPR_GROUPING:
        {
            sprintf(buffer, "%s", expr_to_str(buffer, expr->Grouping.expr));
        } break;
    }

    u64 str_len = strlen(str);
    u64 buffer_len = strlen(buffer);
    u64 new_len = str_len + buffer_len;
    str = xrealloc(str, new_len);
    strncat(str, buffer, buffer_len);

    return buffer;
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
    char* str = xmalloc(0);
    for (int i = 0; i < sb_count(ast); ++i) {
        // print_expr(ast[i]);
        const char* out = expr_to_str(str, ast[i]);
        info(out);
        free(str);
        free(out);
    }
}

//------------------------------------------------------------------------------
//                               Expr Maker Functions
//------------------------------------------------------------------------------

Expr* make_expr_note(Expr* expr)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_NOTE;
    temp->Note.expr = expr;
    return temp;
}
Expr* make_expr_int(int value)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_INT;
    temp->Int.val = value;
    return temp;
}

Expr* make_expr_float(float value)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_FLOAT;
    temp->Float.val = value;
    return temp;
}

Expr* make_expr_ident(const char* ident)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_IDENT;
    temp->Ident.name = ident;
    return temp;
}

Expr* make_expr_func(Type* func_t, Expr* body)

{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_FUNC;
    temp->Func.type = func_t;
    temp->Func.body = body;
    return temp;
}

Expr* make_expr_binary(char op, Expr* lhs, Expr* rhs)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_BINARY;
    temp->Binary.op = op;
    temp->Binary.lhs = lhs;
    temp->Binary.rhs = rhs;
    return temp;
}

Expr* make_expr_unary(char op, Expr* operand)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_UNARY;
    temp->Unary.op = op;
    temp->Unary.operand = operand;
    return temp;
}

Expr* make_expr_block(Expr** stmts)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_BLOCK;
    temp->Block.stmts = stmts;
    return temp;
}

Expr* make_expr_grouping(Expr* expr)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_GROUPING;
    temp->Grouping.expr = expr;
    return temp;
}

Expr* make_expr_ret(Expr* expr)
{
    Expr* temp = alloc_expr();
    temp->kind = EXPR_RET;
    temp->Ret.expr = expr;
    return temp;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

static Expr* alloc_expr()
{
    Expr* temp = (Expr*)malloc(sizeof(Expr));
    if (!temp) error("Out of memory trying to allocate expression.");
    return temp;
}
