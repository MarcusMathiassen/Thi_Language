#ifndef AST_H
#define AST_H

#include "lexer.h"     // Token_Kind
#include "list.h"      // List
#include "typespec.h"  // Typespec

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------
typedef struct Expr    Expr;
typedef enum Expr_Kind Expr_Kind;

enum Expr_Kind {
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
    EXPR_GROUPING,
    EXPR_SUBSCRIPT,

    EXPR_CAST,

    EXPR_BLOCK,
    EXPR_STRUCT,
    EXPR_FUNCTION,
    EXPR_VARIABLE_DECL,
    EXPR_IF,
    EXPR_FOR,
    EXPR_WHILE,
    EXPR_RETURN,
    EXPR_DEFER,

    EXPR_BREAK,
    EXPR_CONTINUE,

    EXPR_COUNT
};

//------------------------------------------------------------------------------
//                          AST Expression Structures
//------------------------------------------------------------------------------
typedef struct {
    char* str;
} AST_Asm;

typedef struct {
    char* name;
    Expr* expr;
} AST_Macro;

typedef struct {
    Expr* expr;
} AST_Note;

typedef struct {
    Expr* expr;
} AST_Grouping;

typedef struct {
    List* stmts;
} AST_Block;

typedef struct {
    s64 val;
} AST_Int;

typedef struct {
    f64 val;
} AST_Float;

typedef struct {
    char* val;
} AST_String;

typedef struct {
    Typespec* type;
} AST_Struct;

typedef struct {
    Typespec* type;
    Expr*     body;
    List*     defers;
} AST_Function;

typedef struct {
    char* name;
} AST_Ident;

typedef struct {
    char* callee;
    List* args;
} AST_Call;

typedef struct {
    Token_Kind op;
    Expr*      operand;
} AST_Unary;

typedef struct {
    Token_Kind op;
    Expr*      lhs;
    Expr*      rhs;
} AST_Binary;

typedef struct {
    char*     name;
    Typespec* type;
    Expr*     value;
} AST_Variable_Decl;

typedef struct {
    Expr* load;
    Expr* sub;
} AST_Subscript;

typedef struct {
    Expr* cond;
    Expr* then_block;
    Expr* else_block;
} AST_If;

typedef struct {
    Expr* init;
    Expr* cond;
    Expr* step;
    Expr* then_block;
} AST_For;

typedef struct {
    Expr* cond;
    Expr* then_block;
} AST_While;

typedef struct {
    Expr* expr;
} AST_Return;

typedef struct {
    Expr* expr;
} AST_Break;
typedef struct {
    Expr* expr;
} AST_Continue;
typedef struct {
    Expr* expr;
} AST_Defer;
typedef struct {
    Typespec* type;
    Expr*     expr;
} AST_Cast;

struct Expr {
    Expr_Kind kind;
    Typespec* type;
    union {
        AST_Asm       Asm;
        AST_Macro     Macro;
        AST_Note      Note;
        AST_Grouping  Grouping;
        AST_Block     Block;
        AST_Int       Int;
        AST_Float     Float;
        AST_String    String;
        AST_Struct    Struct;
        AST_Function  Function;
        AST_Ident     Ident;
        AST_Call      Call;
        AST_Unary     Unary;
        AST_Binary    Binary;
        AST_Subscript Subscript;

        AST_Cast Cast;

        AST_Variable_Decl Variable_Decl;
        AST_If            If;
        AST_For           For;
        AST_While         While;
        AST_Return        Return;
        AST_Defer         Defer;

        AST_Break    Break;
        AST_Continue Continue;
    };
};

Expr* make_expr_asm(char* name);
Expr* make_expr_macro(char* name, Expr* expr);
Expr* make_expr_note(Expr* expr);
Expr* make_expr_int(s64 value);
Expr* make_expr_float(f64 value);
Expr* make_expr_string(char* value);
Expr* make_expr_ident(char* ident);
Expr* make_expr_struct(Typespec* struct_t);
Expr* make_expr_function(Typespec* func_t, Expr* body);
Expr* make_expr_call(char* callee, List* args);
Expr* make_expr_unary(Token_Kind op, Expr* operand);
Expr* make_expr_binary(Token_Kind op, Expr* lhs, Expr* rhs);
Expr* make_expr_block(List* stmts);
Expr* make_expr_grouping(Expr* expr);
Expr* make_expr_variable_decl(char* name, Typespec* type, Expr* value);
Expr* make_expr_subscript(Expr* load, Expr* sub);
Expr* make_expr_if(Expr* cond, Expr* then_block, Expr* else_block);
Expr* make_expr_for(Expr* init, Expr* cond, Expr* step, Expr* then_block);
Expr* make_expr_while(Expr* cond, Expr* then_block);
Expr* make_expr_return(Expr* expr);
Expr* make_expr_defer(Expr* expr);
Expr* make_expr_cast(Expr* expr, Typespec* type);

Expr* make_expr_break();
Expr* make_expr_continue();

Typespec* get_inferred_type_of_expr(Expr* expr);
Expr*     get_arg_from_func(Typespec* func_t, s64 arg_index);
Expr*     constant_fold_expr(Expr* expr);
void      print_ast(List* ast);

char* expr_to_json(Expr* expr);
char* expr_to_str(Expr* expr);
char* expr_kind_to_str(Expr_Kind kind);

#endif
