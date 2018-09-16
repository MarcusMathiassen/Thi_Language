#ifndef AST_H
#define AST_H

#include "types.h"
#include "lexer.h" // Token_Kind

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------

typedef struct Expr Expr;
typedef struct Expr AST;

typedef enum Expr_Kind Expr_Kind;

const char* expr_kind_to_str(Expr_Kind kind);
void print_expr(Expr* expr);
void print_ast(AST** ast);

Expr* make_expr_note(Expr* expr);
Expr* make_expr_int(int value);
Expr* make_expr_float(float value);
Expr* make_expr_ident(const char* ident);
Expr* make_expr_func(Type* func_t, Expr* body);
Expr* make_expr_unary(char op, Expr* operand);
Expr* make_expr_binary(char op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(Expr** stmts);
Expr* make_expr_ret(Expr* expr);
Expr* make_expr_grouping(Expr* expr);

enum Expr_Kind
{
    EXPR_NOTE,
    EXPR_INT,
    EXPR_FLOAT,
    EXPR_IDENT,
    EXPR_CALL,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_COMPOUND,
    EXPR_RET,
    EXPR_VAR_DECL,
    EXPR_FUNC,
    EXPR_IF,
    EXPR_FOR,
    EXPR_BLOCK,
    EXPR_WHILE,
    EXPR_GROUPING,
};

//------------------------------------------------------------------------------
//                          AST Expression Structures                    
//------------------------------------------------------------------------------

struct Expr
{
    Expr_Kind kind;
    union
    {
        struct { Expr* expr;                                } Note;
        struct { Expr* expr;                                } Grouping;
        struct { Expr** stmts;                              } Block;
        struct { Expr* expr;                                } Ret;
        struct { u64 val;                                   } Int;
        struct { f64 val;                                   } Float;
        struct { Type*  type; Expr* body;                   } Func;
        struct { const char* name;                          } Ident;
        struct { Token_Kind op; Expr* operand;              } Unary;
        struct { Token_Kind op; Expr* lhs; Expr* rhs;       } Binary;
        struct { const char* name; Type* type; Expr* value; } Variable_Decl;
    };
};

#endif
