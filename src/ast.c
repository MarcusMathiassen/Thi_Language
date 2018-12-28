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

char* expr_kind_to_str(Expr_Kind kind)
{
    switch (kind) {
    case EXPR_SUBSCRIPT: return "EXPR_SUBSCRIPT";
    case EXPR_CONTINUE: return "EXPR_CONTINUE";
    case EXPR_BREAK: return "EXPR_BREAK";
    case EXPR_MACRO: return "EXPR_MACRO";
    case EXPR_NOTE: return "EXPR_NOTE";
    case EXPR_INT: return "EXPR_INT";
    case EXPR_FLOAT: return "EXPR_FLOAT";
    case EXPR_STRING: return "EXPR_STRING";
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
    case EXPR_SUBSCRIPT: { result = strf("%s[%s]", expr->Subscript.variable_name, expr_to_str(expr->Subscript.expr)); } break;
    case EXPR_CONTINUE: { result = "continue"; } break;
    case EXPR_BREAK: { result = "break"; } break;
    case EXPR_MACRO: { result = strf("%s :: %s", expr->Macro.name, expr_to_str(expr->Macro.expr)); } break;
    case EXPR_NOTE: { result = strf("$%s", expr_to_str(expr->Note.expr)); } break;
    case EXPR_INT: { result = strf("%lld", expr->Int.val); } break;
    case EXPR_STRING: { result = strf("\"%s\"", expr->String.val); } break;
    case EXPR_IDENT: { result = strf("%s", expr->Ident.name); } break;
    case EXPR_UNARY: { result = strf("%s%s", token_kind_to_str(expr->Unary.op), expr_to_str(expr->Unary.operand)); } break;
    case EXPR_BINARY: { result = strf("%s %s %s", expr_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op), expr_to_str(expr->Binary.rhs)); } break;
    case EXPR_RET: { result = strf("ret %s", expr_to_str(expr->Ret.expr)); } break;
    case EXPR_VARIABLE_DECL: { result = strf(expr->Variable_Decl.value ? "%s: %s = %s" : "%s: %s", expr->Variable_Decl.name, typespec_to_str(expr->Variable_Decl.type), expr->Variable_Decl.value ? expr_to_str(expr->Variable_Decl.value) : ""); } break;
    case EXPR_VARIABLE_DECL_TYPE_INF: { result = strf("%s := %s", expr->Variable_Decl_Type_Inf.name, expr_to_str(expr->Variable_Decl_Type_Inf.value)); } break;
    case EXPR_BLOCK: {
        string str = make_string("");
        for (int i = 0; i < sb_count(expr->Block.stmts); ++i) {
            append_string_f(&str, "\t%s\n", expr_to_str(expr->Block.stmts[i]));
        }
        result = str.c_str;
    } break;

    case EXPR_FUNCTION: {
        string str =
            make_string_f("%s  {\n%s}", typespec_to_str(expr->Function.type), expr_to_str(expr->Function.body));
        result = str.c_str;
    } break;

    case EXPR_GROUPING: result = strf("(%s)", expr_to_str(expr->Grouping.expr)); break;
    case EXPR_WHILE: { result = strf("while %s {\n\t%s }", expr_to_str(expr->While.cond), expr_to_str(expr->While.body));
    } break;    
    case EXPR_FOR: {
        string str = make_string_f("for %s: %s..%s {\n\t%s }", expr->For.iterator_name, expr_to_str(expr->For.start),
                                   expr_to_str(expr->For.end), expr_to_str(expr->For.body));
        result = str.c_str;
    } break;
    case EXPR_IF: {
        string str = make_string_f("if %s {\n\t%s }", expr_to_str(expr->If.cond), expr_to_str(expr->If.then_body));
        if (expr->If.else_body) append_string_f(&str, "\t%s\n", expr_to_str(expr->If.else_body));
        result = str.c_str;
    } break;
    case EXPR_CALL: {
        string str = make_string_f("%s", expr->Call.callee);
        result = str.c_str;
    } break;
    }
    assert(result);
    return result;
}

char* expr_to_str_debug(Expr* expr)
{
    char* result = NULL;
    switch (expr->kind) {
    case EXPR_SUBSCRIPT: {
        result = strf("%s[%s]", expr->Subscript.variable_name, expr_to_str(expr->Subscript.expr));
    } break;
    case EXPR_CONTINUE: { 
        result = "continue"; 
    } break;
    case EXPR_BREAK: {
        result = "break";
    } break;
    case EXPR_MACRO: {
        result = strf("%s :: %s", expr->Macro.name, expr_to_str(expr->Macro.expr));
    } break;
    case EXPR_NOTE: {
        result = strf("$%s", expr_to_str(expr->Note.expr));
    } break;
    case EXPR_INT: {
        result = strf("%lld", expr->Int.val);
    } break;
    case EXPR_STRING: {
        result = strf("\"%s\"", expr->String.val);
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
            append_string_f(&str, "\t%s\n", expr_to_str(expr->Block.stmts[i]));
        }
        result = str.c_str;
    } break;

    case EXPR_FUNCTION: {
        string str =
            make_string_f("%s  {\n%s}", typespec_to_str(expr->Function.type), expr_to_str(expr->Function.body));
        result = str.c_str;
    } break;

    case EXPR_GROUPING: result = strf("(%s)", expr_to_str(expr->Grouping.expr)); break;
    case EXPR_WHILE: {
        result = strf("while %s {\n\t%s }", expr_to_str(expr->While.cond), expr_to_str(expr->While.body));
    } break;
    case EXPR_FOR: {
        string str = make_string_f("for %s: %s..%s {\n\t%s }", expr->For.iterator_name, expr_to_str(expr->For.start),
                                   expr_to_str(expr->For.end), expr_to_str(expr->For.body));
        result = str.c_str;
    } break;
    case EXPR_IF: {
        string str = make_string_f("if %s {\n\t%s }", expr_to_str(expr->If.cond), expr_to_str(expr->If.then_body));
        if (expr->If.else_body) append_string_f(&str, "\t%s\n", expr_to_str(expr->If.else_body));
        result = str.c_str;
    } break;
    case EXPR_CALL: {
        string str = make_string_f("%s", expr->Call.callee);
        result = str.c_str;
    } break;
    }
    assert(result);
    return wrap_with_colored_parens(result);
}

char* expr_to_json(Expr* expr)
{
    char* result = NULL;
    switch (expr->kind) {
    case EXPR_SUBSCRIPT: {
        result = strf("{\"%s\": {\"ident\": \"%s\", \"expr\": %s}}", expr_kind_to_str(expr->kind),
                      expr->Subscript.variable_name, expr_to_str(expr->Subscript.expr));
    } break;
    case EXPR_CONTINUE: {
        result = strf("{\"%s\": {%s}}", expr_kind_to_str(expr->kind), "continue");
    } break;
    case EXPR_BREAK: {
        result = strf("{\"%s\": {%s}}", expr_kind_to_str(expr->kind), "break");
    } break;
    case EXPR_MACRO: {
        result = strf("{\"%s\": {\"name\": \"%s\", \"expr\": \"%s\"}}", expr_kind_to_str(expr->kind), expr->Macro.name,
                      expr_to_json(expr));
    } break;
    case EXPR_NOTE: {
        result = strf("{\"%s\": {\"note\":\"%s\"}}", expr_kind_to_str(expr->kind), expr_to_json(expr->Note.expr));
    } break;
    case EXPR_INT: {
        result = strf("{\"%s\": {\"value\": %lld}}", expr_kind_to_str(expr->kind), expr->Int.val);
    } break;
    case EXPR_STRING: {
        result = strf("{\"%s\": {\"value\": \"%s\"}}", expr_kind_to_str(expr->kind), expr->String.val);
    } break;
    case EXPR_IDENT: {
        result = strf("{\"%s\": {\"ident\": \"%s\"}}", expr_kind_to_str(expr->kind), expr->Ident.name);
    } break;
    case EXPR_UNARY: {
        result = strf("{\"%s\": {\"op\": \"%s\", \"expr\": \"%s\"}}", expr_kind_to_str(expr->kind),
                      token_kind_to_str(expr->Unary.op), expr_to_json(expr->Unary.operand));
    } break;
    case EXPR_BINARY: {
        result =
            strf("{\"%s\": {\"op\": \"%s\", \"lhs\": %s, \"rhs\": %s}}", expr_kind_to_str(expr->kind),
                 token_kind_to_str(expr->Binary.op), expr_to_json(expr->Binary.lhs), expr_to_json(expr->Binary.rhs));
    } break;
    case EXPR_RET: {
        result = strf("{\"%s\": {\"expr\": %s}}", expr_kind_to_str(expr->kind), expr_to_json(expr->Ret.expr));
    } break;
    case EXPR_VARIABLE_DECL: {
        result = strf("{\"%s\": {\"name\": \"%s\", \"type\": \"%s\", \"value\": \"%s\"}}", expr_kind_to_str(expr->kind),
                      expr->Variable_Decl.name, typespec_to_str(expr->Variable_Decl.type),
                      expr->Variable_Decl.value ? expr_to_json(expr->Variable_Decl.value) : "");
    } break;
    case EXPR_VARIABLE_DECL_TYPE_INF: {
        result = strf("{\"%s\": {\"name\": \"%s\", \"value\": %s}}", expr_kind_to_str(expr->kind),
                      expr->Variable_Decl_Type_Inf.name, expr_to_json(expr->Variable_Decl_Type_Inf.value));
    } break;
    case EXPR_BLOCK: {
        int block_count = sb_count(expr->Block.stmts);
        string str = make_string("{\"EXPR_BLOCK\": [");
        for (int i = 0; i < block_count; ++i) {
            append_string(&str, expr_to_json(expr->Block.stmts[i]));
            if (i != block_count - 1) append_string(&str, ", ");
        }
        append_string(&str, "]}");
        result = str.c_str;
    } break;
    case EXPR_FUNCTION: {
        result = strf("{\"%s\": {\"signature\": \"%s\", \"body\": %s }}", expr_kind_to_str(expr->kind),
                      typespec_to_str(expr->Function.type), expr_to_json(expr->Function.body));
    } break;
    case EXPR_GROUPING: {
        result = strf("{\"%s\": {\"expr\": %s}}", expr_kind_to_str(expr->kind), expr_to_json(expr->Grouping.expr));
    } break;
    case EXPR_WHILE: {
        result = strf("{\"%s\": {\"cond\": %s, \"body\": %s}}", expr_kind_to_str(expr->kind),
                      expr_to_json(expr->While.cond), expr_to_json(expr->While.body));
    } break;
    case EXPR_FOR: {
        result = strf("{\"%s\": {\"iterator_name\": %s, \"start\": %s, \"end\": %s, \"body\": %s }}",
                      expr_kind_to_str(expr->kind), expr->For.iterator_name, expr_to_json(expr->For.start),
                      expr_to_json(expr->For.end), expr_to_json(expr->For.body));
    } break;
    case EXPR_IF: {
        result = strf("{\"%s\": {\"cond\": %s, \"cond\": %s, \"cond\": %s }}", expr_kind_to_str(expr->kind),
                      expr_to_json(expr->If.cond), expr_to_json(expr->If.then_body),
                      expr->If.else_body ? expr_to_json(expr->If.else_body) : "null");
    } break;
    case EXPR_CALL: {
        string str = make_string("");
        append_string_f(&str, "{\"%s\": {\"callee\": %s, ", expr_kind_to_str(expr->kind), expr->Call.callee);
        append_string(&str, "\"args\": [");
        int arg_count = sb_count(expr->Call.args);
        for (int i = 0; i < arg_count; ++i) {
            append_string(&str, expr_to_json(expr->Call.args[i]));
            if (i != arg_count - 1) append_string(&str, ", ");
        }
        append_string(&str, "]}}");
        result = str.c_str;
    } break;
    }
    assert(result);
    return result;
}
void print_ast(List ast)
{
    info("Printing AST..");
    LIST_FOREACH(ast) { info("%s", expr_to_str_debug((Expr*)it->data)); }
}

char* ast_to_json(List ast)
{
    info("Printing AST as JSON..");
    string json = make_string("");
    LIST_FOREACH(ast)
    {
        Expr* expr = (Expr*)it->data;
        append_string_f(&json, "%s", expr_to_json(expr));
    }
    return json.c_str;
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

Expr* make_expr_subscript(char* variable_name, Expr* expr)
{
    assert(variable_name);
    assert(expr);
    Expr* e = make_expr(EXPR_SUBSCRIPT);
    e->Subscript.variable_name = variable_name;
    e->Subscript.expr = expr;
    return e;
}

Expr* make_expr_continue(void)
{
    Expr* e = make_expr(EXPR_CONTINUE);
    return e;
}

Expr* make_expr_break(void)
{
    Expr* e = make_expr(EXPR_BREAK);
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

Expr* make_expr_for(char* iterator_name, Expr* start, Expr* end, Expr* body)
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

Expr* make_expr_call(char* callee, Expr** args)
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

Expr* make_expr_variable_decl_type_inf(char* name, Expr* value)
{
    assert(name);
    assert(value);
    Expr* e = make_expr(EXPR_VARIABLE_DECL_TYPE_INF);
    e->Variable_Decl_Type_Inf.name = name;
    e->Variable_Decl_Type_Inf.value = value;
    return e;
}
