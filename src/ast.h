#ifndef AST_H
#define AST_H

#include "types.h" // Type
#include "lexer.h" // Token_Kind

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------

typedef struct Expr Expr;
typedef struct Expr AST;
typedef enum Expr_Kind Expr_Kind;

char* expr_to_str(char* str, Expr* expr);
const char* expr_kind_to_str(Expr_Kind kind);
void print_expr(Expr* expr);
void print_ast(AST** ast);

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
    EXPR_STRUCT,
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
        struct { Type*  type;                               } Struct;
        struct { const char* name;                          } Ident;
        struct { Token_Kind op; Expr* operand;              } Unary;
        struct { Token_Kind op; Expr* lhs; Expr* rhs;       } Binary;
        struct { const char* name; Type* type; Expr* value; } Variable_Decl;
    };
};

Expr* make_expr_note(Expr* expr);
Expr* make_expr_int(u64 value);
Expr* make_expr_float(f64 value);
Expr* make_expr_ident(const char* ident);
Expr* make_expr_struct(Type* struct_t);
Expr* make_expr_func(Type* func_t, Expr* body);
Expr* make_expr_unary(char op, Expr* operand);
Expr* make_expr_binary(char op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(Expr** stmts);
Expr* make_expr_ret(Expr* expr);
Expr* make_expr_grouping(Expr* expr);




typedef struct AST_Node AST_Node;
typedef struct AST_List AST_List;

struct AST_Node
{
    Expr* expr;
    AST_Node* next;
};

struct AST_List
{
    AST_Node* head;
};

/*
    AST_Node head;
    head = NULL;
    head = malloc(sizeof(AST_Node));
    head->expr = expr;
    head->next = malloc(sizeof(AST_Node));
    head->next->next = NULL;
*/

#endif
