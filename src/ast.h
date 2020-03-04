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

#include "lex.h"   // Token_Kind
#include "list.h"    // List
#include "string.h"  // string
#include "type.h"    // Type
#include "utility.h" // Loc_Info


//------------------------------------------------------------------------------
//                                  AST Structures
//------------------------------------------------------------------------------

typedef enum {
    BLOCK_LAST_EXPR_IS_IMPLICITLY_RETURNED = 1 << 0,
} BlockFlags;

typedef enum {
    AST_FLAG_GLOBAL_VARIABLE = 1 << 0,
    _AST_FLAG_COUNT_,
} AST_Flag;

typedef enum {
    UNARY_NOT = TOKEN_BANG ,
    UNARY_DEREFERENCE = THI_SYNTAX_POINTER ,
    UNARY_PLUS = TOKEN_PLUS ,
    UNARY_ADDRESSOF = THI_SYNTAX_ADDRESS ,
    UNARY_MINUS = TOKEN_MINUS ,
    UNARY_TOKEN_TILDE = TOKEN_TILDE ,
    UNARY_TOKEN_DOT = TOKEN_DOT ,
    UNARY_TOKEN_SIZEOF = TOKEN_SIZEOF ,
    UNARY_TOKEN_TYPEOF = TOKEN_TYPEOF ,
    UNARY_TOKEN_PLUS_PLUS = TOKEN_PLUS_PLUS ,
    UNARY_TOKEN_MINUS_MINUS = TOKEN_MINUS_MINUS ,
} Unary_Kind;

typedef enum {
    AST_LIST,
    AST_DEF,
    AST_COMMENT,
    AST_NOP,
    AST_SPACE_SEPARATED_IDENTIFIER_LIST,
    AST_COMMA_SEPARATED_LIST,
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
    AST_CHAR,
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
    AST_POST_INC_OR_DEC,
    AST_LITERAL,
    AST_ASM,
    AST_CAST,
    AST_EXPR_LIST,
    AST_TYPE,
    _AST_COUNT_
} AST_Kind;

typedef enum {
    LITERAL_CHAR,
    LITERAL_INTEGER,
    LITERAL_HEX,
    LITERAL_BINARY,
    LITERAL_FLOAT,
    LITERAL_STRING,
    _LITERAL_COUNT_,
} Literal_Kind;

typedef enum {
    LIST_STMT,
    LIST_EXPR,
    _LIST_COUNT_,
} List_Kind;

typedef struct AST AST;

typedef struct {
    s64 line;
    s64 col;
} Loc_Info;

struct AST {
    AST_Kind kind;
    Type* type;
    Loc_Info loc_info;
    List* edges;
    u32 flags;
    union {
        struct {
            List_Kind kind;
            Token_Kind delimitor;
            List* list;
        } List;
        struct {
            char*   name;
            AST*    stmt;
        } Def;
        struct {
            char* name;
            Type* type;
        } Type;
        struct
        {
            char* text;
        } Comment;
        struct
        {
            List* identifiers;
        } Space_Separated_Identifier_List;
        struct
        {
            List* nodes;
        } Comma_Separated_List;
        struct
        {
            char* name;
            List* top_level;
        } Module;
        struct
        {
            AST* cond;
            AST* cases;
            AST* default_case;
        } Switch;
        struct
        {
            AST* node;
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
            AST* module;
        } Load;
        struct
        {
            char* str;
        } Link;
        struct
        {
            AST* node;
        } Note;
        struct
        {
            AST* node;
        } Grouping;
        struct
        {
            List* stmts;
            u32 flags;
        } Block;
        struct
        {
            List* exprs;
        } ExpressionList;
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
            char val;
        } Char;
        struct
        {
            char* name;
            List* members;
        } Struct;
        struct
        {
            char* name;
            List* members;
        } Enum;
        struct
        {
            char* name;
            List* parameters;
            AST* body;
            List* defers;
        } Function;
        struct
        {
            char* name;
        } Ident;
        struct
        {
            List* args;
            char* callee;
        } Call;
        struct
        {
            Token_Kind op;
            AST* operand;
        } Unary;
        struct
        {
            Token_Kind op;
            AST* lhs;
            AST* rhs;
        } Binary;
        struct
        {
            char* name;
            Type* type;
            AST* value;
        } Variable_Decl;
        struct
        {
            AST* load;
            AST* sub;
        } Subscript;
        struct
        {
            AST* load;
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
            AST* node;
        } Return;
        struct
        {
            AST* node;
        } Break;
        struct
        {
            AST* node;
        } Continue;
        struct
        {
            AST* node;
        } Defer;
        struct
        {
            AST* node;
            AST* type_node;
        } As;
        struct
        {
            AST* node;
        } Sizeof;
        struct
        {
            AST* node;
        } Typeof;
        struct
        {
            Token_Kind op;
            AST* node;
        } Post_Inc_or_Dec;
        struct
        {
            Literal_Kind kind;
            union {
                struct { char  value; } Char;
                struct { s64   value; } Integer;
                struct { u64   value; } Hex;
                struct { u64   value; } Binary;
                struct { f64   value; } Float;
                struct { char* value; } String;
            };
        } Literal;
        struct
        {
            AST* block;
        } Asm;
        struct
        {
            AST* desired_type;
            AST* node;
        } Cast;
        struct
        {
            List* exprs;
        } Expr_List;
    };
};

AST* make_ast_list                            (Loc_Info loc_info, List_Kind kind, Token_Kind delimitor, List* list);
AST* make_ast_def                             (Loc_Info loc_info, char* name, AST* stmt);
AST* make_ast_comment                         (Loc_Info loc_info, char* text);
AST* make_ast_nop                             (Loc_Info loc_info);
AST* make_ast_module                          (Loc_Info loc_info, char* name, List* top_level);
AST* make_ast_extern                          (Loc_Info loc_info, Type* type);
AST* make_ast_load                            (Loc_Info loc_info, char* str, AST* module);
AST* make_ast_link                            (Loc_Info loc_info, char* str);
AST* make_ast_var_args                        (Loc_Info loc_info);
AST* make_ast_note                            (Loc_Info loc_info, AST* node);
AST* make_ast_int                             (Loc_Info loc_info, s64 value, Type* type);
AST* make_ast_float                           (Loc_Info loc_info, f64 value, Type* type);
AST* make_ast_string                          (Loc_Info loc_info, char* value);
AST* make_ast_char                            (Loc_Info loc_info, char value);
AST* make_ast_ident                           (Loc_Info loc_info, char* ident);
AST* make_ast_struct                          (Loc_Info loc_info, char* name, List* members);
AST* make_ast_enum                            (Loc_Info loc_info, char* name, List* members);
AST* make_ast_function                        (Loc_Info loc_info, char* name, List* parameters, Type* func_t, AST* body);
AST* make_ast_call                            (Loc_Info loc_info, char* callee, List* args);
AST* make_ast_unary                           (Loc_Info loc_info, Token_Kind op, AST* operand);
AST* make_ast_binary                          (Loc_Info loc_info, Token_Kind op, AST* lhs, AST* rhs);
AST* make_ast_block                           (Loc_Info loc_info, List* stmts);
AST* make_ast_grouping                        (Loc_Info loc_info, AST* node);
AST* make_ast_variable_decl                   (Loc_Info loc_info, char* name, Type* type, AST* value);
AST* make_ast_subscript                       (Loc_Info loc_info, AST* load, AST* sub);
AST* make_ast_field_access                    (Loc_Info loc_info, AST* load, char* field);
AST* make_ast_if                              (Loc_Info loc_info, AST* cond, AST* then_block, AST* else_block);
AST* make_ast_for                             (Loc_Info loc_info, AST* init, AST* cond, AST* step, AST* then_block);
AST* make_ast_while                           (Loc_Info loc_info, AST* cond, AST* then_block);
AST* make_ast_return                          (Loc_Info loc_info, AST* node);
AST* make_ast_defer                           (Loc_Info loc_info, AST* node);
AST* make_ast_as                              (Loc_Info loc_info, AST* node, AST* type_node);
AST* make_ast_sizeof                          (Loc_Info loc_info, AST* node);
AST* make_ast_typeof                          (Loc_Info loc_info, AST* node);
AST* make_ast_is                              (Loc_Info loc_info, AST* node, AST* body, bool has_fallthrough);
AST* make_ast_switch                          (Loc_Info loc_info, AST* if_statement);
AST* make_ast_fallthrough                     (Loc_Info loc_info);
AST* make_ast_break                           (Loc_Info loc_info);
AST* make_ast_continue                        (Loc_Info loc_info);
AST* make_ast_space_separated_identifier_list (Loc_Info loc_info, List* identifiers);
AST* make_ast_comma_separated_list            (Loc_Info loc_info, List* nodes);
AST* make_ast_post_inc_or_dec                 (Loc_Info loc_info, Token_Kind op, AST* node);

AST* make_ast_literal                         (Loc_Info loc_info, char* value);
AST* make_ast_literal_int                     (Loc_Info loc_info, s64 val);
AST* make_ast_literal_float                   (Loc_Info loc_info, f64 val);
AST* make_ast_literal_char                    (Loc_Info loc_info, char val);
AST* make_ast_literal_string                  (Loc_Info loc_info, char* val);

AST* make_ast_asm                             (Loc_Info loc_info, AST* block);
AST* make_ast_cast                            (Loc_Info loc_info, AST* desired_type, AST* node);
AST* make_ast_expr_list                       (Loc_Info loc_info, List* expr_list);
AST* make_ast_type                            (Loc_Info loc_info, char* name, Type* type);

typedef void* (*ast_callback)(void*, AST*);

AST* get_arg_from_func             (AST* func, s64 arg_index);
void ast_tests                     (void);
void* ast_visit                    (ast_callback func, void* ctx, AST* node);
AST* ast_clone                     (AST* a);
void ast_replace                   (AST* a, AST* b);
char* get_ast_name                 (AST* node);
char* ast_get_literal_value_as_str (AST* node);
char* get_ast_loc_str              (AST* node);
char* ast_to_src                   (AST* node);
char* ast_to_str                   (AST* node);
char* ast_kind_to_str              (AST_Kind kind);
char* literal_kind_to_str          (Literal_Kind kind);

typedef struct {
    AST** data;
    s64 count;
    s64 allocated;
} AST_Ref_List;

void ast_ref_list_destroy(AST_Ref_List* l);
AST_Ref_List* make_ast_ref_list(void);
void ast_ref_list_append(AST_Ref_List* l, AST* a);

#ifdef NDEBUG
#define ERROR_UNHANDLED_AST(x)
#define ERROR_UNHANDLED_AST_KIND(x)
#define ERROR_UNHANDLED_LITERAL_KIND(x)
#else
#define ERROR_UNHANDLED_AST(x) ERROR_UNHANDLED_KIND(ast_to_str(x))
#define ERROR_UNHANDLED_AST_KIND(x) ERROR_UNHANDLED_KIND(ast_kind_to_str(x))
#define ERROR_UNHANDLED_LITERAL_KIND(x) ERROR_UNHANDLED_KIND(literal_kind_to_str(x))
#endif

#endif
