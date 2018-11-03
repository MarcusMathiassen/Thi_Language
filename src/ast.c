#include "ast.h"
#include "lexer.h"           // token_kind_to_str,
#include "stretchy_buffer.h" // sb_push
#include "string.h"          // strf, append_string, string
#include "utility.h"         // info, success, error, warning
#include <assert.h>          // assert
#include <stdio.h>           // sprintf,
#include <stdlib.h>          // malloc,

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

const char* expr_kind_to_str(Expr_Kind kind)
{
    switch (kind) {
    case EXPR_MACRO: return "EXPR_MACRO";
    case EXPR_NOTE: return "EXPR_NOTE";
    case EXPR_INT: return "EXPR_INT";
    case EXPR_FLOAT: return "EXPR_FLOAT";
    case EXPR_IDENT: return "EXPR_IDENT";
    case EXPR_CALL: return "EXPR_CALL";
    case EXPR_UNARY: return "EXPR_UNARY";
    case EXPR_BINARY: return "EXPR_BINARY";
    case EXPR_COMPOUND: return "EXPR_COMPOUND";
    case EXPR_RET: return "EXPR_RET";
    case EXPR_VARIABLE_DECL: return "EXPR_VARIABLE_DECL";
    case EXPR_VARIABLE_DECL_TYPE_INF: return "EXPR_VARIABLE_DECL_TYPE_INF";
    case EXPR_FUNCTION: return "EXPR_FUNCTION";
    case EXPR_STRUCT: return "EXPR_STRUCT";
    case EXPR_IF: return "EXPR_IF";
    case EXPR_FOR: return "EXPR_FOR";
    case EXPR_BLOCK: return "EXPR_BLOCK";
    case EXPR_WHILE: return "EXPR_WHILE";
    case EXPR_GROUPING: return "EXPR_GROUPING";
    }
    return "print not implemented";
}

char* expr_to_str(Expr* expr)
{
    char* result = NULL;
    switch (expr->kind) {
    case EXPR_MACRO: {
        result = strf("%s :: %s", expr->Macro.name, expr_to_str(expr->Macro.expr));
    } break;
    case EXPR_NOTE: {
        result = strf("$%s", expr_to_str(expr->Note.expr));
    } break;
    case EXPR_INT: {
        result = strf("%lld", expr->Int.val);
    } break;
    case EXPR_IDENT: {
        result = strf("%s", expr->Ident.name);
    } break;
    case EXPR_UNARY: {
        result = strf("%s%s", token_kind_to_str(expr->Unary.op), expr_to_str(expr->Unary.operand));
    } break;
    case EXPR_BINARY: {
        result = strf("%s %s %s", expr_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op),
                      expr_to_str(expr->Binary.rhs));
    } break;
    case EXPR_RET: {
        result = strf("ret %s", expr_to_str(expr->Ret.expr));
    } break;
    case EXPR_VARIABLE_DECL: {
        result = strf(expr->Variable_Decl.value ? "%s: %s = %s" : "%s: %s", expr->Variable_Decl.name,
                      typespec_to_str(expr->Variable_Decl.type),
                      expr->Variable_Decl.value ? expr_to_str(expr->Variable_Decl.value) : "");
    } break;
    case EXPR_VARIABLE_DECL_TYPE_INF: {
        result = strf("%s := %s", expr->Variable_Decl_Type_Inf.name, expr_to_str(expr->Variable_Decl_Type_Inf.value));
    } break;

    case EXPR_BLOCK: {
        string str = make_string("");
        for (int i = 0; i < sb_count(expr->Block.stmts); ++i) {
            append_string(&str, strf("\t%s\n", expr_to_str(expr->Block.stmts[i])));
        }
        result = str.c_str;
    } break;

    case EXPR_FUNCTION: {
        string str =
            make_string(strf("%s  {\n%s}", typespec_to_str(expr->Function.type), expr_to_str(expr->Function.body)));
        result = str.c_str;
    } break;

    case EXPR_GROUPING: result = strf("(%s)", expr_to_str(expr->Grouping.expr)); break;
    case EXPR_WHILE: {
        result = strf("while %s {\n\t%s }", expr_to_str(expr->While.cond), expr_to_str(expr->While.body));
    } break;
    case EXPR_FOR: {
        string str = make_string(strf("for %s: %s..%s {\n\t%s }", expr->For.iterator_name, expr_to_str(expr->For.start),
                                      expr_to_str(expr->For.end), expr_to_str(expr->For.body)));
        result = str.c_str;
    } break;
    case EXPR_IF: {
        string str = make_string(strf("if %s {\n\t%s }", expr_to_str(expr->If.cond), expr_to_str(expr->If.then_body)));
        if (expr->If.else_body) append_string(&str, strf("\t%s\n", expr_to_str(expr->If.else_body)));
        result = str.c_str;
    } break;
    case EXPR_CALL: {
        string str = make_string(strf("%s", expr->Call.callee));
        result = str.c_str;
    } break;
    }
    assert(result);
    return wrap_with_colored_parens(result);
}

void print_ast(List ast)
{
    info("Printing AST..");
    LIST_FOREACH(ast) { info("%s", expr_to_str((Expr*)it->data)); }
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

Expr* make_expr_macro(const char* name, Expr* expr)
{
    assert(name);
    assert(expr);
    Expr* e = make_expr(EXPR_MACRO);
    e->Macro.name = name;
    e->Macro.expr = expr;
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
Expr* make_expr_function(Typespec* func_t, Expr* body)
{
    assert(func_t);
    assert(func_t->kind == TYPESPEC_FUNCTION);
    assert(body);
    assert(body->kind == EXPR_BLOCK);
    Expr* e = make_expr(EXPR_FUNCTION);
    e->Function.type = func_t;
    e->Function.body = body;
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

Expr* make_expr_while(Expr* cond, Expr* body)
{
    assert(cond);
    assert(body);
    Expr* e = make_expr(EXPR_WHILE);
    e->While.cond = cond;
    e->While.body = body;
    return e;
}
Expr* make_expr_for(const char* iterator_name, Expr* start, Expr* end, Expr* body)
{
    assert(iterator_name);
    assert(start);
    assert(end);
    assert(body);
    Expr* e = make_expr(EXPR_FOR);
    e->For.iterator_name = iterator_name;
    e->For.start = start;
    e->For.end = end;
    e->For.body = body;
    return e;
}
Expr* make_expr_if(Expr* cond, Expr* then_body, Expr* else_body)
{
    assert(cond);
    assert(then_body);
    Expr* e = make_expr(EXPR_IF);
    e->If.cond = cond;
    e->If.then_body = then_body;
    e->If.else_body = else_body;
    return e;
}

Expr* make_expr_call(const char* callee, Expr** args)
{
    assert(callee);
    assert(args);
    Expr* e = make_expr(EXPR_CALL);
    e->Call.callee = callee;
    e->Call.args = args;
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

Expr* make_expr_variable_decl(const char* name, Typespec* type, Expr* value)
{
    assert(name);
    assert(type);
    // Value can be NULL if the decl doesnt have an initial value.
    Expr* e = make_expr(EXPR_VARIABLE_DECL);
    e->Variable_Decl.name = name;
    e->Variable_Decl.type = type;
    e->Variable_Decl.value = value;
    return e;
}

Expr* make_expr_variable_decl_type_inf(const char* name, Expr* value)
{
    assert(name);
    assert(value);
    Expr* e = make_expr(EXPR_VARIABLE_DECL_TYPE_INF);
    e->Variable_Decl_Type_Inf.name = name;
    e->Variable_Decl_Type_Inf.value = value;
    return e;
}
