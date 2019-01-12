#include "ast.h"
#include "globals.h"
#include "lexer.h"   // token_kind_to_str,
#include "string.h"  // strf, append_string, string
#include "utility.h" // info, success, error, warning
#include <assert.h>  // assert
#include <stdio.h>   // sprintf,
#include <stdlib.h>  // xmalloc,

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
    switch (expr->kind) {
    case EXPR_ASM: {
        return strf("%s", expr->Asm.str);
    }
    case EXPR_MACRO: {
        return strf("%s :: %s", expr->Macro.name, expr_to_str(expr->Macro.expr));
    }
    case EXPR_NOTE: {
        return strf("$%s", expr_to_str(expr->Note.expr));
    }
    case EXPR_INT: {
        return strf("%lld", expr->Int.val);
    }
    case EXPR_STRING: {
        return strf("\"%s\"", expr->String.val);
    }
    case EXPR_IDENT: {
        return strf("%s", expr->Ident.name);
    }
    case EXPR_UNARY: {
        return strf("%s(%s)", token_kind_to_str(expr->Unary.op), expr_to_str(expr->Unary.operand));
    }
    case EXPR_BINARY: {
        return strf("%s %s %s", expr_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op),
                    expr_to_str(expr->Binary.rhs));
    }
    case EXPR_VARIABLE_DECL: {
        return strf(expr->Variable_Decl.value ? "%s: %s = %s" : "%s: %s", expr->Variable_Decl.name,
                    typespec_to_str(expr->Variable_Decl.type),
                    expr->Variable_Decl.value ? expr_to_str(expr->Variable_Decl.value) : "");
    }
    case EXPR_BLOCK: {
        string str = make_string("");
        LIST_FOREACH(expr->Block.stmts)
        {
            Expr* stmt = (Expr*)it->data;
            append_string_f(&str, "%s\n", expr_to_str(stmt));
        }
        return str.c_str;
    }
    case EXPR_STRUCT: {
        return strf("%s", typespec_to_str(expr->Struct.type));
    }
    case EXPR_FUNCTION: {
        string str = make_string_f("%s {\n%s}", typespec_to_str(expr->Function.type), expr_to_str(expr->Function.body));
        return str.c_str;
    }
    case EXPR_GROUPING: return strf("(%s)", expr_to_str(expr->Grouping.expr));
    case EXPR_CALL: {
        string str = make_string(expr->Call.callee);
        s64 count = expr->Call.args->count;
        s64 index = 0;
        append_string(&str, "(");
        LIST_FOREACH(expr->Call.args)
        {
            Expr* arg = (Expr*)it->data;
            append_string(&str, expr_to_str(arg));
            if (index++ != count - 1) append_string(&str, ", ");
        }
        append_string(&str, ")");
        return str.c_str;
    }
    }

    error("%s: unhandled case %s", __func__, expr_kind_to_str(expr->kind));
    return NULL;
}

Typespec* get_inferred_type_of_expr(Expr* expr)
{
    switch (expr->kind) {
    case EXPR_MACRO: return get_inferred_type_of_expr(expr->Macro.expr);
    case EXPR_NOTE: return get_inferred_type_of_expr(expr->Note.expr);
    case EXPR_INT: return make_typespec_int(DEFAULT_INT_BYTE_SIZE, 0);
    case EXPR_FLOAT: error("get_inferred_type_of_expr EXPR_FLOAT not implemented");
    case EXPR_STRING: return make_typespec_pointer(make_typespec_int(8, 1));
    case EXPR_IDENT: return get_symbol(expr->Ident.name);
    case EXPR_CALL: return get_symbol(expr->Call.callee)->Function.ret_type;
    case EXPR_UNARY: return get_inferred_type_of_expr(expr->Unary.operand);
    case EXPR_BINARY: return get_inferred_type_of_expr(expr->Binary.rhs);
    case EXPR_VARIABLE_DECL: return expr->Variable_Decl.type;
    case EXPR_FUNCTION: return expr->Function.type->Function.ret_type;
    case EXPR_STRUCT: return expr->Struct.type;
    case EXPR_GROUPING: return get_inferred_type_of_expr(expr->Grouping.expr);
    default: error("%s has no type", expr_kind_to_str(expr->kind));
    }
    return NULL;
}
void print_ast(List* ast)
{
    info("Printing AST..");
    LIST_FOREACH(ast) { info("%s", wrap_with_colored_parens(expr_to_str((Expr*)it->data))); }
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

Expr* make_expr_int(s64 value)
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
    Expr* e = make_expr(EXPR_FUNCTION);
    e->Function.type = func_t;
    e->Function.body = body;
    e->Function.defers = make_list();
    return e;
}

Expr* constant_fold_expr(Expr* expr)
{
    Expr* lhs = expr->Binary.lhs;
    Expr* rhs = expr->Binary.rhs;

    switch (lhs->kind) {
        // case EXPR_GROUPING: lhs = constant_fold_expr(lhs->Grouping.expr); break;
        case EXPR_BINARY: lhs = constant_fold_expr(lhs); break;
    }

    switch (rhs->kind) {
        // case EXPR_GROUPING: rhs = constant_fold_expr(rhs->Grouping.expr); break;
        case EXPR_BINARY: rhs = constant_fold_expr(rhs); break;
    }

    if (lhs->kind == EXPR_INT && rhs->kind == EXPR_INT) {

        Token_Kind op = expr->Binary.op;
        s64 lhs_v = lhs->Int.val;
        s64 rhs_v = rhs->Int.val;

        s64 value = 0;

        switch(op) {
            case TOKEN_EQ_EQ: value = (lhs_v == rhs_v); break;
            case TOKEN_BANG_EQ: value = (lhs_v != rhs_v); break;
            case TOKEN_PLUS: value = (lhs_v + rhs_v); break;
            case TOKEN_MINUS: value = (lhs_v - rhs_v); break;
            case TOKEN_ASTERISK: value = (lhs_v * rhs_v); break;
            case TOKEN_FWSLASH: value = (lhs_v / rhs_v); break;
            case TOKEN_AND: value = (lhs_v & rhs_v); break;
            case TOKEN_PIPE: value = (lhs_v | rhs_v); break;
            case TOKEN_LT: value = (lhs_v < rhs_v); break;
            case TOKEN_GT: value = (lhs_v > rhs_v); break;
            case TOKEN_GT_GT: value = (lhs_v >> rhs_v); break;
            case TOKEN_LT_LT: value = (lhs_v << rhs_v); break;
            case TOKEN_PERCENT: value = (lhs_v % rhs_v); break;
            case TOKEN_HAT: value = (lhs_v ^ rhs_v); break;
            case TOKEN_AND_AND: value = (lhs_v && rhs_v); break;
            case TOKEN_PIPE_PIPE: value = (lhs_v || rhs_v); break;
            default: error("Unhandled constant_fold_expr case: %s", token_kind_to_str(op));
        }
        info("folded %s into %lld", expr_to_str(expr), value);
        return make_expr_int(value);
    }

    return expr;
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

    if (should_constant_fold_binary_expressions)
        e = constant_fold_expr(e);

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
