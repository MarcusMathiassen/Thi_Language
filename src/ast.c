#include "ast.h"
#include "globals.h"
#include "lexer.h"           // token_kind_to_str,
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

char* expr_kind_to_str(Expr_Kind kind)
{
    switch (kind) {
    case EXPR_ASM: return "EXPR_ASM";
    case EXPR_MACRO: return "EXPR_MACRO";
    case EXPR_NOTE: return "EXPR_NOTE";
    case EXPR_INT: return "EXPR_INT";
    case EXPR_FLOAT: return "EXPR_FLOAT";
    case EXPR_STRING: return "EXPR_STRING";
    case EXPR_IDENT: return "EXPR_IDENT";
    case EXPR_CALL: return "EXPR_CALL";
    case EXPR_UNARY: return "EXPR_UNARY";
    case EXPR_BINARY: return "EXPR_BINARY";
    case EXPR_VARIABLE_DECL: return "EXPR_VARIABLE_DECL";
    case EXPR_FUNCTION: return "EXPR_FUNCTION";
    case EXPR_STRUCT: return "EXPR_STRUCT";
    case EXPR_BLOCK: return "EXPR_BLOCK";
    case EXPR_GROUPING: return "EXPR_GROUPING";
    }
    return "print not implemented";
}

char* expr_to_str(Expr* expr)
{
    char* result = NULL;
    switch (expr->kind) {
    case EXPR_ASM: {
        result = strf("%s", expr->Asm.str);
    } break;
    case EXPR_MACRO: {
        result = strf("%s :: %s", expr->Macro.name, expr_to_str(expr->Macro.expr));
    } break;
    case EXPR_NOTE: {
        result = strf("$%s", expr_to_str(expr->Note.expr));
    } break;
    case EXPR_INT: {
        result = strf("%llu", expr->Int.val);
    } break;
    case EXPR_STRING: {
        result = strf("\"%s\"", expr->String.val);
    } break;
    case EXPR_IDENT: {
        result = strf("%s", expr->Ident.name);
    } break;
    case EXPR_UNARY: {
        result = strf("%s(%s)", token_kind_to_str(expr->Unary.op), expr_to_str(expr->Unary.operand));
    } break;
    case EXPR_BINARY: {
        result = strf("%s %s %s", expr_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op),
                      expr_to_str(expr->Binary.rhs));
    } break;
    case EXPR_VARIABLE_DECL: {
        result = strf(expr->Variable_Decl.value ? "%s: %s = %s" : "%s: %s", expr->Variable_Decl.name,
                      typespec_to_str(expr->Variable_Decl.type),
                      expr->Variable_Decl.value ? expr_to_str(expr->Variable_Decl.value) : "");
    } break;
    case EXPR_BLOCK: {
        string str = make_string("");
        LIST_FOREACH(expr->Block.stmts) {
            Expr* stmt = (Expr*)it->data;
            append_string_f(&str, "\t%s\n", expr_to_str(stmt));
        }
        result = str.c_str;
    } break;
    case EXPR_STRUCT: {
        result = strf("%s", typespec_to_str(expr->Struct.type));
    } break;
    case EXPR_FUNCTION: {
        string str =
            make_string_f("%s  {\n%s}", typespec_to_str(expr->Function.type), expr_to_str(expr->Function.body));
        result = str.c_str;
    } break;

    case EXPR_GROUPING: result = strf("(%s)", expr_to_str(expr->Grouping.expr)); break;
    case EXPR_CALL: {
        string str = make_string_f("%s", expr->Call.callee);
        result = str.c_str;
    } break;
    }
    assert(result);
    return result;
}

Typespec* get_inferred_type_of_expr(Expr* expr)
{
    switch(expr->kind)
    {
    case EXPR_MACRO: return get_inferred_type_of_expr(expr->Macro.expr);
    case EXPR_NOTE: return get_inferred_type_of_expr(expr->Note.expr);
    case EXPR_INT: return make_typespec_int(DEFAULT_INTEGER_BIT_SIZE, 0);
    case EXPR_FLOAT: error("get_inferred_type_of_expr EXPR_FLOAT not implemented");
    case EXPR_STRING: error("get_inferred_type_of_expr EXPR_STRING not implemented");
    case EXPR_IDENT: return get_symbol(expr->Ident.name);
    case EXPR_CALL: return get_symbol(expr->Call.callee);
    case EXPR_UNARY: return get_inferred_type_of_expr(expr->Unary.operand);
    case EXPR_BINARY: return get_inferred_type_of_expr(expr->Binary.rhs);
    case EXPR_VARIABLE_DECL: return expr->Variable_Decl.type;
    case EXPR_FUNCTION: return expr->Function.type->Function.ret_type;
    case EXPR_STRUCT: return expr->Struct.type;
    case EXPR_GROUPING: return get_inferred_type_of_expr(expr->Grouping.expr);
    // default: error("%s has no type", expr_kind_to_str(expr->kind));
    }
    return NULL;
}

void print_ast(List* ast)
{
    info("Printing AST..");
    LIST_FOREACH(ast) {
        info("%s", wrap_with_colored_parens(expr_to_str((Expr*)it->data)));
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

Expr* make_expr_asm(char* str)
{
    assert(str);
    Expr* e = make_expr(EXPR_ASM);
    e->Asm.str = str;
    return e;
}

Expr* make_expr_macro(char* name, Expr* expr)
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

Expr* make_expr_int(i64 value)
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

Expr* make_expr_string(char* value)
{
    Expr* e = make_expr(EXPR_STRING);
    e->String.val = value;
    return e;
}

Expr* make_expr_ident(char* ident)
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

Expr* make_expr_block(List* stmts)
{
    Expr* e = make_expr(EXPR_BLOCK);
    e->Block.stmts = stmts;
    return e;
}

Expr* make_expr_call(char* callee, List* args)
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

Expr* make_expr_variable_decl(char* name, Typespec* type, Expr* value)
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
