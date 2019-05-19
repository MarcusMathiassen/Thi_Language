// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------

#ifndef AST_H
#define AST_H

#include "lexer.h" // Token_Kind
#include "list.h"  // List
#include "type.h"  // Type
#include "cst.h"  // CST

typedef enum {
    AST_MODULE,
    AST_IS,
    AST_FALLTHROUGH,
    AST_VAR_ARGS,
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
    AST_FIELD_ACCESS,
    AST_AS,
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
    AST_TYPEOF,
    AST_SIZEOF,
    AST_SWITCH,
    AST_COUNT
} AST_Kind;

//------------------------------------------------------------------------------
//                                  AST Structures
//------------------------------------------------------------------------------

typedef struct AST AST;

struct AST {
    AST_Kind kind;
    Type*    type;
    Token    t;
    union {
        struct
        {
            char* name;
            AST*  top_level;
        } Module;
        struct
        {
            AST* cond;
            AST* cases;
            AST* default_case;
        } Switch;
        struct
        {
            AST* expr;
            AST* body;
            bool has_fallthrough;
        } Is;
        struct
        {
            Type* type;
        } Extern;

        struct
        {
            char* str;
        } Load;

        struct
        {
            char* str;
        } Link;

        struct
        {
            AST* expr;
        } Note;

        struct
        {
            AST* expr;
        } Grouping;

        struct
        {
            List* stmts;
        } Block;

        struct
        {
            s64 val;
        } Int;

        struct
        {
            f64 val;
        } Float;

        struct
        {
            char* val;
        } String;

        struct
        {
            Type* type;
        } Struct;
        struct
        {
            Type* type;
        } Enum;

        struct
        {
            Type* type;
            AST*  body;
            List* defers;
        } Function;

        struct
        {
            char* name;
        } Ident;

        struct
        {
            char* callee;
            List* args;
        } Call;

        struct
        {
            Token_Kind op;
            AST*       operand;
        } Unary;

        struct
        {
            Token_Kind op;
            AST*       lhs;
            AST*       rhs;
        } Binary;

        struct
        {
            char* name;
            Type* type;
            AST*  value;
        } Variable_Decl;

        struct
        {
            char* name;
            AST*  value;
        } Constant_Decl;

        struct
        {
            AST* load;
            AST* sub;
        } Subscript;

        struct
        {
            AST*  load;
            char* field;
        } Field_Access;

        struct
        {
            AST* cond;
            AST* then_block;
            AST* else_block;
        } If;

        struct
        {
            AST* init;
            AST* cond;
            AST* step;
            AST* then_block;
        } For;

        struct
        {
            AST* cond;
            AST* then_block;
        } While;

        struct
        {
            AST* expr;
        } Return;

        struct
        {
            AST* expr;
        } Break;

        struct
        {
            AST* expr;
        } Continue;

        struct
        {
            AST* expr;
        } Defer;

        struct
        {
            AST* expr;
            AST* type_expr;
        } As;

        struct
        {
            AST* expr;
        } Sizeof;
        struct
        {
            AST* expr;
        } Typeof;
    };
};

List* generate_ast_from_cst(List* top_level_cst);

// clang-format off
AST* make_ast_module        (Token t, char* name, AST* top_level);
AST* make_ast_extern        (Token t, Type* type);
AST* make_ast_load          (Token t, char* str);
AST* make_ast_link          (Token t, char* str);
AST* make_ast_var_args      (Token t);
AST* make_ast_note          (Token t, AST* expr);
AST* make_ast_int           (Token t, s64 value);
AST* make_ast_float         (Token t, f64 value);
AST* make_ast_string        (Token t, char* value);
AST* make_ast_ident         (Token t, char* ident);
AST* make_ast_struct        (Token t, Type* struct_t);
AST* make_ast_enum          (Token t, Type* enum_t);
AST* make_ast_function      (Token t, Type* func_t, AST* body);
AST* make_ast_call          (Token t, char* callee, List* args);
AST* make_ast_unary         (Token t, Token_Kind op, AST* operand);
AST* make_ast_binary        (Token t, Token_Kind op, AST* lhs, AST* rhs);
AST* make_ast_block         (Token t, List* stmts);
AST* make_ast_grouping      (Token t, AST* expr);
AST* make_ast_variable_decl (Token t, char* name, Type* type, AST* value);
AST* make_ast_constant_decl (Token t, char* name, AST* value);
AST* make_ast_subscript     (Token t, AST* load, AST* sub);
AST* make_ast_field_access  (Token t, AST* load, char* field);
AST* make_ast_if            (Token t, AST* cond, AST* then_block, AST* else_block);
AST* make_ast_for           (Token t, AST* init, AST* cond, AST* step, AST* then_block);
AST* make_ast_while         (Token t, AST* cond, AST* then_block);
AST* make_ast_return        (Token t, AST* expr);
AST* make_ast_defer         (Token t, AST* expr);
AST* make_ast_as            (Token t, AST* expr, AST* type_expr);
AST* make_ast_sizeof        (Token t, AST* expr);
AST* make_ast_typeof        (Token t, AST* expr);
AST* make_ast_is            (Token t, AST* expr, AST* body, bool has_fallthrough);
AST* make_ast_switch        (Token t, AST* if_statement);
AST* make_ast_fallthrough   (Token t);
AST* make_ast_break         (Token t);
AST* make_ast_continue      (Token t);

AST*  get_arg_from_func (Type* func_t, s64 arg_index);
void ast_tests(void);
void  ast_visit         (void (*func)(void*, AST*), void* ctx, AST* expr);
void  ast_replace       (AST* a, AST* b);
char* ast_to_json       (AST* expr);
char* ast_to_str        (AST* expr);
char* ast_kind_to_str   (AST_Kind kind);
// clang-format on

typedef struct {
    AST** data;
    s64   count;
    s64   allocated;
} AST_Ref_List;

AST_Ref_List make_ast_ref_list(void);
void         ast_ref_list_append(AST_Ref_List* l, AST* a);

#endif
