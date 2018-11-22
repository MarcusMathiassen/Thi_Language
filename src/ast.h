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

    EXPR_BREAK,
    EXPR_CONTINUE,

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
    union {
        struct
        {
            const char* variable_name;
            Expr* expr;
        } Subscript;
        struct
        {
            const char* name;
            Expr* expr;
        } Macro;
        struct
        {
            Expr* expr;
        } Note;
        struct
        {
            Expr* expr;
        } Grouping;
        struct
        {
            Expr** stmts;
        } Block;
        struct
        {
            Expr* expr;
        } Ret;
        struct
        {
            u64 val;
        } Int;
        struct
        {
            f64 val;
        } Float;
        struct
        {
            Typespec* type;
            Expr* body;
        } Function;
        struct
        {
            Typespec* type;
        } Struct;
        struct
        {
            const char* name;
        } Ident;
        struct
        {
            Expr* cond;
            Expr* then_body;
            Expr* else_body;
        } If;
        struct
        {
            const char* iterator_name;
            Expr* start;
            Expr* end;
            Expr* body;
        } For;
        struct
        {
            Expr* cond;
            Expr* body;
        } While;
        struct
        {
            const char* callee;
            Expr** args;
        } Call;
        struct
        {
            Token_Kind op;
            Expr* operand;
        } Unary;
        struct
        {
            Token_Kind op;
            Expr* lhs;
            Expr* rhs;
        } Binary;
        struct
        {
            const char* name;
            Typespec* type;
            Expr* value;
        } Variable_Decl;
        struct
        {
            const char* name;
            Expr* value;
        } Variable_Decl_Type_Inf;
    };
};

Expr* make_expr_subscript(const char* variable_name, Expr* expr);
Expr* make_expr_continue(void);
Expr* make_expr_break(void);
Expr* make_expr_macro(const char* name, Expr* expr);
Expr* make_expr_note(Expr* expr);
Expr* make_expr_int(u64 value);
Expr* make_expr_float(f64 value);
Expr* make_expr_ident(const char* ident);
Expr* make_expr_struct(Typespec* struct_t);
Expr* make_expr_call(const char* callee, Expr** args);
Expr* make_expr_if(Expr* cond, Expr* then_body, Expr* else_body);
Expr* make_expr_for(const char* iterator_name, Expr* start, Expr* end, Expr* body);
Expr* make_expr_while(Expr* cond, Expr* body);
Expr* make_expr_function(Typespec* func_t, Expr* body);
Expr* make_expr_unary(Token_Kind op, Expr* operand);
Expr* make_expr_binary(Token_Kind op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(Expr** stmts);
Expr* make_expr_ret(Expr* expr);
Expr* make_expr_grouping(Expr* expr);
Expr* make_expr_variable_decl(const char* name, Typespec* type, Expr* value);
Expr* make_expr_variable_decl_type_inf(const char* name, Expr* value);

char* ast_to_json(List ast);
void print_ast(List ast);
char* expr_to_str(Expr* expr);
const char* expr_kind_to_str(Expr_Kind kind);

#endif
