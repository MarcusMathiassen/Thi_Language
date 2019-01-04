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
    EXPR_MACRO,
    EXPR_NOTE,
    EXPR_INT,
    EXPR_FLOAT,
    EXPR_STRING,
    EXPR_IDENT,
    EXPR_CALL,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_COMPOUND,
    EXPR_RET,
    EXPR_VARIABLE_DECL,
    EXPR_VARIABLE_DECL_TYPE_INF,
    EXPR_STRUCT,
    EXPR_FUNCTION,
    EXPR_SUBSCRIPT,
    EXPR_IF,
    EXPR_FOR,
    EXPR_WHILE,
    EXPR_BREAK,
    EXPR_CONTINUE,
    EXPR_BLOCK,
    EXPR_GROUPING,
};

//------------------------------------------------------------------------------
//                          AST Expression Structures
//------------------------------------------------------------------------------

typedef struct
{
    char* variable_name;
    Expr* expr;
} AST_Subscript;
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
    u64 val;
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
    Expr* body;
} AST_Function;
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
    Expr* cond;
    Expr* then_body;
    Expr* else_body;
} AST_If;
typedef struct
{
    char* iterator_name;
    Expr* start;
    Expr* end;
    Expr* body;
} AST_For;
typedef struct
{
    Expr* cond;
    Expr* body;
} AST_While;
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
typedef struct
{
    char* name;
    Expr* value;
} AST_Variable_Decl_Type_Inf;

struct Expr
{
    Expr_Kind kind;
    union {
        AST_Subscript Subscript;
        AST_Macro Macro;
        AST_Note Note;
        AST_Grouping Grouping;
        AST_Block Block;
        AST_Ret Ret;
        AST_Int Int;
        AST_Float Float;
        AST_String String;
        AST_Function Function;
        AST_Struct Struct;
        AST_Ident Ident;
        AST_If If;
        AST_For For;
        AST_While While;
        AST_Call Call;
        AST_Unary Unary;
        AST_Binary Binary;
        AST_Variable_Decl Variable_Decl;
        AST_Variable_Decl_Type_Inf Variable_Decl_Type_Inf;
    };
};

Expr* make_expr_subscript(char* variable_name, Expr* expr);
Expr* make_expr_continue(void);
Expr* make_expr_break(void);
Expr* make_expr_macro(char* name, Expr* expr);
Expr* make_expr_note(Expr* expr);
Expr* make_expr_int(u64 value);
Expr* make_expr_float(f64 value);
Expr* make_expr_string(char* value);
Expr* make_expr_ident(char* ident);
Expr* make_expr_struct(Typespec* struct_t);
Expr* make_expr_call(char* callee, Expr** args);
Expr* make_expr_if(Expr* cond, Expr* then_body, Expr* else_body);
Expr* make_expr_for(char* iterator_name, Expr* start, Expr* end, Expr* body);
Expr* make_expr_while(Expr* cond, Expr* body);
Expr* make_expr_function(Typespec* func_t, Expr* body);
Expr* make_expr_unary(Token_Kind op, Expr* operand);
Expr* make_expr_binary(Token_Kind op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(Expr** stmts);
Expr* make_expr_ret(Expr* expr);
Expr* make_expr_grouping(Expr* expr);
Expr* make_expr_variable_decl(char* name, Typespec* type, Expr* value);
Expr* make_expr_variable_decl_type_inf(char* name, Expr* value);

char* ast_to_json(List ast);
void print_ast(List ast);
char* expr_to_str(Expr* expr);
char* expr_to_str_debug(Expr* expr);
char* expr_kind_to_str(Expr_Kind kind);

#endif
