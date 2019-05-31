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

#ifndef CODEGEN_UTILITY_H
#define CODEGEN_UTILITY_H
#include "typedefs.h"

#include "ast.h"    // AST
#include "lexer.h"  // Token_Kind
#include "stack.h"  // Stack
#include "map.h"  // Map
#include "string.h" // string
#include "type.h"   // Type
#include "value.h"  // Value

typedef enum {
    CLASS_INTEGER, // This class consists of integral types that fit into one of the general purpose registers.
    CLASS_SSE,     // The class consists of types that fit into a vector register.
    CLASS_SSEUP,   // The class consists of types that fit into a vector register and can be passed and returned in the upper bytes of it.
    CLASS_X87,
    CLASS_X87UP,       // These classes consists of types that will be returned via the x87 FPU.
    CLASS_COMPLEX_X87, // This class consists of types that will be returned via the x87 FPU.
    CLASS_NO_CLASS,    // This class is used as initializer in the algorithms. It will be used for padding and empty structures and unions.
    CLASS_MEMORY,      // This class consists of types that will be passed and returned in memory via the stack.
    _CLASS_COUNT_,
} Class_Kind;

typedef struct {
    Class_Kind class;
    AST* argument;
} ClassifiedArgument;

#ifdef NDEBUG
#define ERROR_UNHANDLED_CLASS_KIND(x)
#else
#define ERROR_UNHANDLED_CLASS_KIND(x) ERROR_UNHANDLED_KIND(class_kind_to_str(x))
#endif

typedef struct {
    u64 stack_pos;
    List* local_variables;
} Scope;

typedef struct
{
    AST* module;
    Map* symbols;
    Stack* scopes;

    s64 stack_pos;

    AST* current_function;
    Type* expected_type;
    
    string* section_text;
    string* section_data;
    string* section_extern;

    s64 text_label_counter;
    s64 data_label_counter;

    char* o0;
    char* o1;
    char* l0;
    char* l1;

    char* ocontinue;
    char* lcontinue;
    char* obreak;
    char* lbreak;
    char* l_end;

    s8 next_available_xmm_reg_counter;
    s8 next_available_rax_reg_counter;
} Codegen_Context;

char* class_kind_to_str(Class_Kind kind);
Class_Kind classify(Type* type);
List* classify_arguments(List* arguments);

Codegen_Context make_codegen_context(void);
void set_temp_labels(Codegen_Context* ctx, char* l0, char* l1);
void restore_temp_labels(Codegen_Context* ctx);
void set_break_label(Codegen_Context* ctx, char* break_l);
void set_continue_label(Codegen_Context* ctx, char* continue_l);
void restore_break_label(Codegen_Context* ctx);
void restore_continue_label(Codegen_Context* ctx);
void set_jump_labels(Codegen_Context* ctx, char* continue_l, char* break_l);
void restore_jump_labels(Codegen_Context* ctx);
char* make_text_label(Codegen_Context* ctx);
char* make_data_label(Codegen_Context* ctx);
void reset_text_label_counter(Codegen_Context* ctx);
void reset_stack(Codegen_Context* ctx);
void set_current_function_expr(Codegen_Context* ctx, AST* func_expr);

void emit_no_tab(Codegen_Context* ctx, char* fmt, ...);
void emit_extern(Codegen_Context* ctx, char* fmt, ...);
void emit_data(Codegen_Context* ctx, char* fmt, ...);
void emit(Codegen_Context* ctx, char* fmt, ...);

void push(Codegen_Context* ctx, int reg);
void pop(Codegen_Context* ctx, int reg);
void push_type(Codegen_Context* ctx, Type* type);
void pop_type(Codegen_Context* ctx, Type* type);
void pop_type_2(Codegen_Context* ctx, Type* type);
void push_scope(Codegen_Context* ctx);
void pop_scope(Codegen_Context* ctx);
char* get_result_reg_of_size(Type* type, s8 size);
char* get_result_reg(Type* type);
char* get_result_reg_2(Type* type);
char* get_op_size(s8 bytes);
char* get_db_op(Type* type);
char* get_move_op(Type* type);
void alloc_variable(Codegen_Context* ctx, Value* variable);
void dealloc_variable(Codegen_Context* ctx, Value* variable);
Value* get_variable_in_scope(Scope* scope, char* name);
Value* get_variable(Codegen_Context* ctx, AST* ident);
void add_variable(Codegen_Context* ctx, Value* variable);
int align(int n, s32 m);
char* get_instr(Token_Kind op, Type* type);

char* emit_save_result(Codegen_Context* ctx, Value* value);
char* get_next_available_reg_fitting(Codegen_Context* ctx, Type* type);
s8 get_next_available_xmm_reg_fitting(Codegen_Context* ctx);
s8 get_next_available_rax_reg_fitting(Codegen_Context* ctx, s64 size);

s64 get_all_alloca_in_block(AST* block);

void emit_cast(Codegen_Context* ctx, Value* variable, Type* desired_type);
void emit_cast_int_to_int(Codegen_Context* ctx, char* reg, Type* type);
void emit_cast_float_to_int(Codegen_Context* ctx, char* reg, Type* type);
void emit_store_deref(Codegen_Context* ctx, Value* variable);
void emit_store_r(Codegen_Context* ctx, Value* variable, s64 reg);
void emit_store(Codegen_Context* ctx, Value* variable);
void emit_load(Codegen_Context* ctx, Value* variable);
void emit_jmp(Codegen_Context* ctx, char* label);
#endif
