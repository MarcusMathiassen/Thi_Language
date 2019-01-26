#ifndef AST_H
#define AST_H

#include "lexer.h"  // Token_Kind
#include "list.h"   // List
#include "type.h"   // Type

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------
typedef struct AST    AST;
typedef enum AST_Kind AST_Kind;

enum AST_Kind {

    AST_EXTERN,
    AST_LOAD,
    AST_LINK,

    AST_NOTE,
    AST_INT,
    AST_FLOAT,
    AST_STRING,
    AST_IDENT,
    AST_CALL,
    AST_UNARY,
    AST_BINARY,
    AST_GROUPING,
    AST_SUBSCRIPT,

    AST_CAST,

    AST_BLOCK,
    AST_STRUCT,
    AST_ENUM,
    AST_FUNCTION,

    AST_VARIABLE_DECL,
    AST_CONSTANT_DECL,

    AST_IF,
    AST_FOR,
    AST_WHILE,
    AST_RETURN,
    AST_DEFER,

    AST_BREAK,
    AST_CONTINUE,

    AST_COUNT
};

//------------------------------------------------------------------------------
//                                  AST Structures
//------------------------------------------------------------------------------
typedef struct {
    AST*  node;
} AST_Extern;

typedef struct {
    AST*  node;
} AST_Load;

typedef struct {
    AST* node;
} AST_Link;

typedef struct {
    AST* expr;
} AST_Note;

typedef struct {
    AST* expr;
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
    Type* type;
} AST_Struct;

typedef struct {
    Type* type;
} AST_Enum;

typedef struct {
    Type* type;
    AST*  body;
    List* defers;
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
    AST*       operand;
} AST_Unary;

typedef struct {
    Token_Kind op;
    AST*       lhs;
    AST*       rhs;
} AST_Binary;

typedef struct {
    char* name;
    Type* type;
    AST*  value;
} AST_Variable_Decl;

typedef struct {
    char* name;
    AST*  value;
} AST_Constant_Decl;

typedef struct {
    AST* load;
    AST* sub;
} AST_Subscript;

typedef struct {
    AST* cond;
    AST* then_block;
    AST* else_block;
} AST_If;

typedef struct {
    AST* init;
    AST* cond;
    AST* step;
    AST* then_block;
} AST_For;

typedef struct {
    AST* cond;
    AST* then_block;
} AST_While;

typedef struct {
    AST* expr;
} AST_Return;

typedef struct {
    AST* expr;
} AST_Break;
typedef struct {
    AST* expr;
} AST_Continue;
typedef struct {
    AST* expr;
} AST_Defer;
typedef struct {
    Type* type;
    AST*  expr;
} AST_Cast;

struct AST {
    AST_Kind kind;
    Type*    type;
    union {
        AST_Extern     Extern;
        AST_Load     Load;
        AST_Link     Link;

        AST_Note      Note;
        AST_Grouping  Grouping;
        AST_Block     Block;
        AST_Int       Int;
        AST_Float     Float;
        AST_String    String;
        AST_Struct    Struct;
        AST_Enum      Enum;
        AST_Function  Function;
        AST_Ident     Ident;
        AST_Call      Call;
        AST_Unary     Unary;
        AST_Binary    Binary;
        AST_Subscript Subscript;

        AST_Cast Cast;

        AST_Variable_Decl Variable_Decl;
        AST_Constant_Decl Constant_Decl;
        AST_If            If;
        AST_For           For;
        AST_While         While;
        AST_Return        Return;
        AST_Defer         Defer;

        AST_Break    Break;
        AST_Continue Continue;
    };
};

AST* make_ast_extern(AST* node);
AST* make_ast_load(AST* node);
AST* make_ast_link(AST* node);

AST* make_ast_note(AST* expr);
AST* make_ast_int(s64 value);
AST* make_ast_float(f64 value);
AST* make_ast_string(char* value);
AST* make_ast_ident(char* ident);
AST* make_ast_struct(Type* struct_t);
AST* make_ast_enum(Type* enum_t);
AST* make_ast_function(Type* func_t, AST* body);
AST* make_ast_call(char* callee, List* args);
AST* make_ast_unary(Token_Kind op, AST* operand);
AST* make_ast_binary(Token_Kind op, AST* lhs, AST* rhs);
AST* make_ast_block(List* stmts);
AST* make_ast_grouping(AST* expr);
AST* make_ast_variable_decl(char* name, Type* type, AST* value);
AST* make_ast_constant_decl(char* name, AST* value);
AST* make_ast_subscript(AST* load, AST* sub);
AST* make_ast_if(AST* cond, AST* then_block, AST* else_block);
AST* make_ast_for(AST* init, AST* cond, AST* step, AST* then_block);
AST* make_ast_while(AST* cond, AST* then_block);
AST* make_ast_return(AST* expr);
AST* make_ast_defer(AST* expr);
AST* make_ast_cast(AST* expr, Type* type);

AST* make_ast_break();
AST* make_ast_continue();

AST*  get_arg_from_func(Type* func_t, s64 arg_index);
AST*  constant_fold_expr(AST* expr);

void print_ast(List* ast);
void print_ast_json(List* ast);

char* ast_to_json(AST* expr);
char* ast_to_str(AST* expr);
char* ast_kind_to_str(AST_Kind kind);

#endif
