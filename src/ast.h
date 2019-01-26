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

    AST_SIZEOF,

    AST_COUNT
};

//------------------------------------------------------------------------------
//                                  AST Structures
//------------------------------------------------------------------------------

struct AST {
    AST_Kind kind;
    Type*    type;
    union {
        struct {
            Type* type;
        } Extern;

        struct {
            char* str;
        } Load;

        struct {
            char* str;
        } Link;

        struct {
            AST* expr;
        } Note;

        struct {
            AST* expr;
        } Grouping;

        struct {
            List* stmts;
        } Block;

        struct {
            s64 val;
        } Int;

        struct {
            f64 val;
        } Float;

        struct {
            char* val;
        } String;

        struct {
            Type* type;
        } Struct;

        struct {
            Type* type;
        } Enum;

        struct {
            Type* type;
            AST*  body;
            List* defers;
        } Function;

        struct {
            char* name;
        } Ident;

        struct {
            char* callee;
            List* args;
        } Call;

        struct {
            Token_Kind op;
            AST*       operand;
        } Unary;

        struct {
            Token_Kind op;
            AST*       lhs;
            AST*       rhs;
        } Binary;

        struct {
            char* name;
            Type* type;
            AST*  value;
        } Variable_Decl;

        struct {
            char* name;
            AST*  value;
        } Constant_Decl;

        struct {
            AST* load;
            AST* sub;
        } Subscript;

        struct {
            AST* cond;
            AST* then_block;
            AST* else_block;
        } If;

        struct {
            AST* init;
            AST* cond;
            AST* step;
            AST* then_block;
        } For;

        struct {
            AST* cond;
            AST* then_block;
        } While;

        struct {
            AST* expr;
        } Return;

        struct {
            AST* expr;
        } Break;

        struct {
            AST* expr;
        } Continue;

        struct {
            AST* expr;
        } Defer;

        struct {
            Type* type;
            AST*  expr;
        } Cast;

        struct {
            Type* type;
        } Sizeof;
    };
};

AST* make_ast_extern(Type* type);
AST* make_ast_load(char* str);
AST* make_ast_link(char* str);

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
AST* make_ast_sizeof(Type* type);

AST* make_ast_break();
AST* make_ast_continue();

AST* get_arg_from_func(Type* func_t, s64 arg_index);

void print_ast(List* ast);
void print_ast_json(List* ast);

char* ast_to_json(AST* expr);
char* ast_to_str(AST* expr);
char* ast_kind_to_str(AST_Kind kind);

typedef struct {
    AST** data;
    s64   count;
    s64   allocated;
} AST_Ref_List;

AST_Ref_List make_ast_ref_list();
void         ast_ref_list_append(AST_Ref_List* l, AST* a);

#endif
