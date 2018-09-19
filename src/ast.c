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

const char *expr_kind_to_str(Expr_Kind kind) {
  switch (kind) {
  case EXPR_NOTE:
    return "EXPR_NOTE";
  case EXPR_INT:
    return "EXPR_INT";
  case EXPR_FLOAT:
    return "EXPR_FLOAT";
  case EXPR_IDENT:
    return "EXPR_IDENT";
  case EXPR_CALL:
    return "EXPR_CALL";
  case EXPR_UNARY:
    return "EXPR_UNARY";
  case EXPR_BINARY:
    return "EXPR_BINARY";
  case EXPR_COMPOUND:
    return "EXPR_COMPOUND";
  case EXPR_RET:
    return "EXPR_RET";
  case EXPR_VARIABLE_DECL:
    return "EXPR_VARIABLE_DECL";
  case EXPR_VARIABLE_DECL_TYPE_INF:
    return "EXPR_VARIABLE_DECL_TYPE_INF";
  case EXPR_FUNCTION:
    return "EXPR_FUNCTION";
  case EXPR_STRUCT:
    return "EXPR_STRUCT";
  case EXPR_IF:
    return "EXPR_IF";
  case EXPR_FOR:
    return "EXPR_FOR";
  case EXPR_BLOCK:
    return "EXPR_BLOCK";
  case EXPR_WHILE:
    return "EXPR_WHILE";
  case EXPR_GROUPING:
    return "EXPR_GROUPING";
  }
  return "print not implemented";
}

char *expr_to_str(Expr *expr) {
  char *result = NULL;
  switch (expr->kind) {
  case EXPR_INT: {
    result = strf("%lld", expr->Int.val);
  } break;
  case EXPR_IDENT: {
    result = strf("%s", expr->Ident.name);
  } break;
  case EXPR_UNARY: {
    result = strf("%s%s", token_kind_to_str(expr->Unary.op),
                  expr_to_str(expr->Unary.operand));
  } break;
  case EXPR_BINARY: {
    result =
        strf("%s %s %s", expr_to_str(expr->Binary.lhs),
             token_kind_to_str(expr->Binary.op), expr_to_str(expr->Binary.rhs));
  } break;
  case EXPR_RET: {
    result = strf("ret %s", expr_to_str(expr->Ret.expr));
  } break;
  case EXPR_VARIABLE_DECL: {
    result = strf(
        expr->Variable_Decl.value ? "%s: %s = %s" : "%s: %s",
        expr->Variable_Decl.name, typespec_to_str(expr->Variable_Decl.type),
        expr->Variable_Decl.value ? expr_to_str(expr->Variable_Decl.value)
                                  : "");
  } break;
  case EXPR_VARIABLE_DECL_TYPE_INF: {
    result = strf("%s := %s", expr->Variable_Decl_Type_Inf.name,
                  expr_to_str(expr->Variable_Decl_Type_Inf.value));
  } break;

  case EXPR_BLOCK: {
    string *str = make_string("", 50);
    for (int i = 0; i < sb_count(expr->Block.stmts); ++i) {
      append_string(str, strf("\t%s\n", expr_to_str(expr->Block.stmts[i])));
    }
    result = str->data;
  } break;

  case EXPR_FUNCTION: {
    string *str =
        make_string(strf("%s  {\n%s}", typespec_to_str(expr->Function.type),
                         expr_to_str(expr->Function.body)),
                    50);
    result = str->data;
  } break;

  case EXPR_GROUPING:
    result = strf("(%s)", expr_to_str(expr->Grouping.expr));
    break;
  }
  assert(result);
  return wrap_with_colored_parens(result);
}

void print_ast(AST **ast) {
  info("Printing AST..");
  for (int i = 0; i < sb_count(ast); ++i) {
    info("%s", expr_to_str(ast[i]));
  }
}

//------------------------------------------------------------------------------
//                               Expr Maker Functions
//------------------------------------------------------------------------------

Expr *make_expr(Expr_Kind kind) {
  Expr *e = xmalloc(sizeof(Expr));
  e->kind = kind;
  return e;
}
Expr *make_expr_note(Expr *expr) {
  assert(expr);
  Expr *e = make_expr(EXPR_NOTE);
  e->Note.expr = expr;
  return e;
}
Expr *make_expr_int(u64 value) {
  Expr *e = make_expr(EXPR_INT);
  e->Int.val = value;
  return e;
}

Expr *make_expr_float(f64 value) {
  Expr *e = make_expr(EXPR_FLOAT);
  e->Float.val = value;
  return e;
}

Expr *make_expr_ident(const char *ident) {
  assert(ident);
  Expr *e = make_expr(EXPR_IDENT);
  e->Ident.name = ident;
  return e;
}
Expr *make_expr_struct(Typespec *struct_t) {
  assert(struct_t);
  Expr *e = make_expr(EXPR_STRUCT);
  e->Struct.type = struct_t;
  return e;
}
Expr *make_expr_function(Typespec *func_t, Expr *body) {
  assert(func_t);
  assert(func_t->kind == TYPESPEC_FUNCTION);
  assert(body);
  assert(body->kind == EXPR_BLOCK);
  Expr *e = make_expr(EXPR_FUNCTION);
  e->Function.type = func_t;
  e->Function.body = body;
  return e;
}

Expr *make_expr_binary(Token_Kind op, Expr *lhs, Expr *rhs) {
  assert(op != TOKEN_UNKNOWN);
  assert(lhs);
  assert(rhs);
  Expr *e = make_expr(EXPR_BINARY);
  e->Binary.op = op;
  e->Binary.lhs = lhs;
  e->Binary.rhs = rhs;
  return e;
}

Expr *make_expr_unary(Token_Kind op, Expr *operand) {
  assert(op != TOKEN_UNKNOWN);
  assert(operand);
  Expr *e = make_expr(EXPR_UNARY);
  e->Unary.op = op;
  e->Unary.operand = operand;
  return e;
}

Expr *make_expr_block(Expr **stmts) {
  Expr *e = make_expr(EXPR_BLOCK);
  e->Block.stmts = stmts;
  return e;
}

Expr *make_expr_grouping(Expr *expr) {
  assert(expr);
  Expr *e = make_expr(EXPR_GROUPING);
  e->Grouping.expr = expr;
  return e;
}

Expr *make_expr_ret(Expr *expr) {
  assert(expr);
  Expr *e = make_expr(EXPR_RET);
  e->Ret.expr = expr;
  return e;
}

Expr *make_expr_variable_decl(const char *name, Typespec *type, Expr *value) {
  assert(name);
  assert(type);
  // Value can be NULL if the decl doesnt have an initial value.
  Expr *e = make_expr(EXPR_VARIABLE_DECL);
  e->Variable_Decl.name = name;
  e->Variable_Decl.type = type;
  e->Variable_Decl.value = value;
  return e;
}
Expr *make_expr_variable_decl_type_inf(const char *name, Expr *value) {
  assert(name);
  assert(value);
  Expr *e = make_expr(EXPR_VARIABLE_DECL_TYPE_INF);
  e->Variable_Decl_Type_Inf.name = name;
  e->Variable_Decl_Type_Inf.value = value;
  return e;
}
