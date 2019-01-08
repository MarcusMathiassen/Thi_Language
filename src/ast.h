#ifndef AST_H
#define AST_H

#include "lexer.h"    // Token_Kind
#include "list.h"     // list
#include "typespec.h" // Typespec

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------
typedef struct Expr Expr;
typedef struct Expr AST;
typedef enum Expr_Kind Expr_Kind;

enum Expr_Kind
{
    EXPR_ASM,

    EXPR_MACRO,
    EXPR_NOTE,
    EXPR_INT,
    EXPR_FLOAT,
    EXPR_STRING,
    EXPR_IDENT,
    EXPR_CALL,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_VARIABLE_DECL,
    EXPR_STRUCT,
    EXPR_BLOCK,
    EXPR_GROUPING,
};

//------------------------------------------------------------------------------
//                          AST Expression Structures
//------------------------------------------------------------------------------
typedef struct
{
    char* str;
} AST_Asm;
typedef struct
{
    char* name;
    Expr* expr;
} AST_Macro;
typedef struct
{
    Expr* expr;
} AST_Note;
typedef struct
{
    Expr* expr;
} AST_Grouping;
typedef struct
{
    Expr** stmts;
} AST_Block;
typedef struct
{
    Expr* expr;
} AST_Ret;
typedef struct
{
    i64 val;
} AST_Int;
typedef struct
{
    f64 val;
} AST_Float;
typedef struct
{
    char* val;
} AST_String;
typedef struct
{
    Typespec* type;
} AST_Struct;
typedef struct
{
    char* name;
} AST_Ident;
typedef struct
{
    char* callee;
    Expr** args;
} AST_Call;
typedef struct
{
    Token_Kind op;
    Expr* operand;
} AST_Unary;
typedef struct
{
    Token_Kind op;
    Expr* lhs;
    Expr* rhs;
} AST_Binary;
typedef struct
{
    char* name;
    Typespec* type;
    Expr* value;
} AST_Variable_Decl;

struct Expr
{
    Expr_Kind kind;
    Typespec* type;
    union {
        AST_Asm Asm;

        AST_Macro Macro;
        AST_Note Note;
        AST_Grouping Grouping;
        AST_Block Block;
        AST_Ret Ret;
        AST_Int Int;
        AST_Float Float;
        AST_String String;
        AST_Struct Struct;
        AST_Ident Ident;
        AST_Call Call;
        AST_Unary Unary;
        AST_Binary Binary;
        AST_Variable_Decl Variable_Decl;
    };
};

Expr* make_expr_asm(char* name);

Expr* make_expr_macro(char* name, Expr* expr);
Expr* make_expr_note(Expr* expr);
Expr* make_expr_int(i64 value);
Expr* make_expr_float(f64 value);
Expr* make_expr_string(char* value);
Expr* make_expr_ident(char* ident);
Expr* make_expr_struct(Typespec* struct_t);
Expr* make_expr_call(char* callee, Expr** args);
Expr* make_expr_unary(Token_Kind op, Expr* operand);
Expr* make_expr_binary(Token_Kind op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(Expr** stmts);
Expr* make_expr_ret(Expr* expr);
Expr* make_expr_grouping(Expr* expr);
Expr* make_expr_variable_decl(char* name, Typespec* type, Expr* value);

Typespec* get_inferred_type_of_expr(Expr* expr);

char* ast_to_json(List* ast);
void print_ast(List* ast);
char* expr_to_str(Expr* expr);
char* expr_to_str_debug(Expr* expr);
char* expr_kind_to_str(Expr_Kind kind);

#endif
