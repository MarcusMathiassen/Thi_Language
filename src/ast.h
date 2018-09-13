#ifndef AST_H
#define AST_H

#include "types.h"

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------

typedef struct Expr Expr;
typedef struct Expr AST;

typedef enum Expr_Kind Expr_Kind;
typedef enum Stmt_Kind Stmt_Kind;
typedef enum Decl_Kind Decl_Kind;

void print_expr(Expr* expr);
void print_ast(AST** ast);

Expr* make_expr_int(int value);
Expr* make_expr_float(float value);
Expr* make_expr_ident(const char* ident);
Expr* make_expr_func(Type* func_t, Expr* body);
Expr* make_expr_unary(char op, Expr* operand);
Expr* make_expr_binary(char op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(Expr** stmts);
Expr* make_expr_ret(Expr* expr);
Expr* make_expr_grouping(Expr* expr);

enum Decl_Kind {
    DECL_NONE,
    DECL_VAR,
    DECL_FUNC,
};

enum Stmt_Kind {
    STMT_NONE,

    STMT_RET,
    STMT_IF,
    STMT_FOR,
    STMT_WHILE,
    STMT_VAR_DECL,
};

enum Expr_Kind {
    EXPR_NONE,

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

struct Expr {

    Expr_Kind kind;

    union {
        struct { Expr* expr;                                } Grouping;
        struct { Expr** stmts;                              } Block;
        struct { Expr* expr;                                } Ret;
        struct { u64 val;                                   } Int;
        struct { f64 val;                                   } Float;
        struct { Type*  type; Expr* body;                   } Func;
        struct { const char* name;                          } Ident;
        struct { char op; Expr* operand;                    } Unary;
        struct { char op; Expr* lhs; Expr* rhs;             } Binary;
        struct { const char* name; Type* type; Expr* value; } Variable_Decl;
    };
};

#endif
