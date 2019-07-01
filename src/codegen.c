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

// [1] System V Application Binary Interface AMD64 Architecture Processor Supplement (With LP64 and ILP32 Programming Models) Version 1.0

#include "codegen.h"
#include "ast.h" // AST*, ast_to_str

#include "parser.h"
#include "map.h" // Map
#include "lex.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "stack.h"
#include "common.h"
#include "utility.h" // error warning info, wrap_with_colored_parens
#include "value.h"   // Value, Scope
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <stdlib.h>  // free
#include <string.h>  // strcmp

#define DEBUG_START                                                              \
    xassert(ctx && node);                                                        \
    // debug("%s: %s", (char*)__func__, wrap_with_colored_parens(ast_to_str(node))); \
    // emit(ctx, "; %s", ast_to_str(node));


typedef struct
{
    AST* module;
    Map* symbols;
    Stack* scopes;

    s64 stack_pos;

    AST* current_function;
    Type* expected_type;

    List* data_list;

    bool inside_asm;
    
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
    Map* symbols;
} Scope;

char* class_kind_to_str(Class_Kind kind);
Class_Kind classify(Type* type);
List* classify_arguments(List* arguments);

AST* get_current_function(Codegen_Context* ctx);
s64 get_stack_pos(Codegen_Context* ctx);
char* get_o0(Codegen_Context* ctx);
char* get_o1(Codegen_Context* ctx);
char* get_l0(Codegen_Context* ctx);
char* get_l1(Codegen_Context* ctx);

char* get_ocontinue(Codegen_Context* ctx);
char* get_lcontinue(Codegen_Context* ctx);
char* get_obreak(Codegen_Context* ctx);
char* get_lbreak(Codegen_Context* ctx);
char* get_l_end(Codegen_Context* ctx);

void set_temp_labels                  (Codegen_Context* ctx, char* l0, char* l1);
void restore_temp_labels              (Codegen_Context* ctx);
void set_break_label                  (Codegen_Context* ctx, char* break_l);
void set_continue_label               (Codegen_Context* ctx, char* continue_l);
void restore_break_label              (Codegen_Context* ctx);
void restore_continue_label           (Codegen_Context* ctx);
void set_jump_labels                  (Codegen_Context* ctx, char* continue_l, char* break_l);
void restore_jump_labels              (Codegen_Context* ctx);
char* make_text_label                 (Codegen_Context* ctx);
char* make_data_label                 (Codegen_Context* ctx);
void reset_text_label_counter         (Codegen_Context* ctx);
void reset_stack                      (Codegen_Context* ctx);
void set_current_function_expr        (Codegen_Context* ctx, AST* func_expr);
void emit_no_tab                      (Codegen_Context* ctx, char* fmt, ...);
void emit_extern                      (Codegen_Context* ctx, char* fmt, ...);
char* emit_data                       (Codegen_Context* ctx, char* fmt, ...);
void emit                             (Codegen_Context* ctx, char* fmt, ...);
void push                             (Codegen_Context* ctx, int reg);
void pop                              (Codegen_Context* ctx, int reg);
void push_type                        (Codegen_Context* ctx, Type* type);
void pop_type                         (Codegen_Context* ctx, Type* type);
void pop_type_2                       (Codegen_Context* ctx, Type* type);
void push_scope                       (Codegen_Context* ctx);
void pop_scope                        (Codegen_Context* ctx);
char* get_result_reg_of_size          (Type* type, s8 size);
char* get_result_reg                  (Type* type);
char* get_result_reg_2                (Type* type);
char* get_op_size                     (s8 bytes);
char* get_db_op                       (Type* type);
char* get_move_op                     (Type* type);
void alloc_variable                   (Codegen_Context* ctx, Value* variable);
void dealloc_variable                 (Codegen_Context* ctx, Value* variable);
Value* get_variable_in_scope          (Scope* scope, char* name);
Value* get_variable                   (Codegen_Context* ctx, AST* ident);
void add_variable                     (Codegen_Context* ctx, Value* variable);
int align                             (int n, s32 m);
char* get_instr                       (Token_Kind op, Type* type);
char* emit_save_result                (Codegen_Context* ctx, Value* value);
char* get_next_available_reg_fitting  (Codegen_Context* ctx, Type* type);
s8 get_next_available_xmm_reg_fitting (Codegen_Context* ctx);
s8 get_next_available_rax_reg_fitting (Codegen_Context* ctx, s64 size);
s64 get_all_alloca_in_block           (AST* block);
void emit_cast                        (Codegen_Context* ctx, Value* variable, Type* desired_type);
void emit_cast_int_to_int             (Codegen_Context* ctx, char* reg, Type* type);
void emit_cast_float_to_int           (Codegen_Context* ctx, char* reg, Type* type);
void emit_store_deref                 (Codegen_Context* ctx, Value* variable);
void emit_store_r                     (Codegen_Context* ctx, Value* variable, Register_Kind reg);
void emit_store                       (Codegen_Context* ctx, Value* variable);
void emit_load                        (Codegen_Context* ctx, Value* variable);
void emit_jmp                         (Codegen_Context* ctx, char* label);

Codegen_Context make_codegen_context(void);

static Value* codegen                                        (Codegen_Context* ctx, AST* node);
static Value* codegen_comment                         (Codegen_Context* ctx, AST* node);
static Value* codegen_nop                             (Codegen_Context* ctx, AST* node);
static Value* codegen_space_separated_identifier_list (Codegen_Context* ctx, AST* node);
static Value* codegen_comma_separated_list            (Codegen_Context* ctx, AST* node);
static Value* codegen_module                          (Codegen_Context* ctx, AST* node);
static Value* codegen_is                              (Codegen_Context* ctx, AST* node);
static Value* codegen_fallthrough                     (Codegen_Context* ctx, AST* node);
static Value* codegen_var_args                        (Codegen_Context* ctx, AST* node);
static Value* codegen_extern                          (Codegen_Context* ctx, AST* node);
static Value* codegen_load                            (Codegen_Context* ctx, AST* node);
static Value* codegen_link                            (Codegen_Context* ctx, AST* node);
static Value* codegen_note                            (Codegen_Context* ctx, AST* node);
static Value* codegen_int                             (Codegen_Context* ctx, AST* node);
static Value* codegen_float                           (Codegen_Context* ctx, AST* node);
static Value* codegen_string                          (Codegen_Context* ctx, AST* node);
static Value* codegen_char                            (Codegen_Context* ctx, AST* node);
static Value* codegen_ident                           (Codegen_Context* ctx, AST* node);
static Value* codegen_call                            (Codegen_Context* ctx, AST* node);
static Value* codegen_unary                           (Codegen_Context* ctx, AST* node);
static Value* codegen_binary                          (Codegen_Context* ctx, AST* node);
static Value* codegen_grouping                        (Codegen_Context* ctx, AST* node);
static Value* codegen_subscript                       (Codegen_Context* ctx, AST* node);
static Value* codegen_field_access                    (Codegen_Context* ctx, AST* node);
static Value* codegen_as                              (Codegen_Context* ctx, AST* node);
static Value* codegen_block                           (Codegen_Context* ctx, AST* node);
static Value* codegen_struct                          (Codegen_Context* ctx, AST* node);
static Value* codegen_enum                            (Codegen_Context* ctx, AST* node);
static Value* codegen_function                        (Codegen_Context* ctx, AST* node);
static Value* codegen_variable_decl                   (Codegen_Context* ctx, AST* node);
static Value* codegen_if                              (Codegen_Context* ctx, AST* node);
static Value* codegen_for                             (Codegen_Context* ctx, AST* node);
static Value* codegen_while                           (Codegen_Context* ctx, AST* node);
static Value* codegen_return                          (Codegen_Context* ctx, AST* node);
static Value* codegen_defer                           (Codegen_Context* ctx, AST* node);
static Value* codegen_break                           (Codegen_Context* ctx, AST* node);
static Value* codegen_continue                        (Codegen_Context* ctx, AST* node);
static Value* codegen_typeof                          (Codegen_Context* ctx, AST* node);
static Value* codegen_sizeof                          (Codegen_Context* ctx, AST* node);
static Value* codegen_switch                          (Codegen_Context* ctx, AST* node);
static Value* codegen_post_inc_or_dec                 (Codegen_Context* ctx, AST* node);
static Value* codegen_literal                         (Codegen_Context* ctx, AST* node);
static Value* codegen_asm                             (Codegen_Context* ctx, AST* node);

static Value* (*codegen_transitions[])(Codegen_Context*, AST*) = {
    [AST_COMMENT]                         =  codegen_comment,
    [AST_NOP]                             =  codegen_nop,
    [AST_SPACE_SEPARATED_IDENTIFIER_LIST] =  codegen_space_separated_identifier_list,
    [AST_COMMA_SEPARATED_LIST]            =  codegen_comma_separated_list,
    [AST_MODULE]                          =  codegen_module,
    [AST_IS]                              =  codegen_is,
    [AST_FALLTHROUGH]                     =  codegen_fallthrough,
    [AST_VAR_ARGS]                        =  codegen_var_args,
    [AST_EXTERN]                          =  codegen_extern,
    [AST_LOAD]                            =  codegen_load,
    [AST_LINK]                            =  codegen_link,
    [AST_NOTE]                            =  codegen_note,
    [AST_INT]                             =  codegen_int,
    [AST_FLOAT]                           =  codegen_float,
    [AST_STRING]                          =  codegen_string,
    [AST_CHAR]                            =  codegen_char,
    [AST_IDENT]                           =  codegen_ident,
    [AST_CALL]                            =  codegen_call,
    [AST_UNARY]                           =  codegen_unary,
    [AST_BINARY]                          =  codegen_binary,
    [AST_GROUPING]                        =  codegen_grouping,
    [AST_SUBSCRIPT]                       =  codegen_subscript,
    [AST_FIELD_ACCESS]                    =  codegen_field_access,
    [AST_AS]                              =  codegen_as,
    [AST_BLOCK]                           =  codegen_block,
    [AST_STRUCT]                          =  codegen_struct,
    [AST_ENUM]                            =  codegen_enum,
    [AST_FUNCTION]                        =  codegen_function,
    [AST_VARIABLE_DECL]                   =  codegen_variable_decl,
    [AST_IF]                              =  codegen_if,
    [AST_FOR]                             =  codegen_for,
    [AST_WHILE]                           =  codegen_while,
    [AST_RETURN]                          =  codegen_return,
    [AST_DEFER]                           =  codegen_defer,
    [AST_BREAK]                           =  codegen_break,
    [AST_CONTINUE]                        =  codegen_continue,
    [AST_TYPEOF]                          =  codegen_typeof,
    [AST_SIZEOF]                          =  codegen_sizeof,
    [AST_SWITCH]                          =  codegen_switch,
    [AST_POST_INC_OR_DEC]                 =  codegen_post_inc_or_dec,
    [AST_LITERAL]                         =  codegen_literal,
    [AST_ASM]                             =  codegen_asm,
};

char* to_x64(AST* node) {
    push_timer((char*)__func__);
    Codegen_Context ctx = make_codegen_context();
    codegen(&ctx, node);

#ifdef __APPLE__
    char* x64 = strf("%s%sglobal _main\n%s", string_data(ctx.section_extern), string_data(ctx.section_data), string_data(ctx.section_text));
#elif __unix__
    char* x64 = strf("%s%sglobal main\n%s", string_data(ctx.section_extern), string_data(ctx.section_data), string_data(ctx.section_text));
#endif
    pop_timer();
    return x64;
}

// @Hotpath @Recursive
static Value* codegen(Codegen_Context* ctx, AST* node) {
    xassert(ctx);
    if (!node) return NULL;
    AST_Kind kind = node->kind;
    Value* (*func)(Codegen_Context*, AST*) = (*codegen_transitions[kind]);
    tassert(func, "codegen missing callback for %s", kind);
    return (*func)(ctx, node);
}

static Value* codegen_comment(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return NULL;
}

static Value* codegen_nop(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return NULL;
}

static Value* codegen_space_separated_identifier_list(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_comma_separated_list(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_module(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    list_foreach(node->Module.top_level) {
        codegen(ctx, it->data);
    }
    return NULL;
}

static Value* codegen_is(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_fallthrough(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_var_args(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_extern(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    char* func_name = node->Extern.type->Function.name;
    emit_extern(ctx, func_name);
    return NULL;
}

static Value* codegen_load(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return codegen(ctx, node->Load.module);
}

static Value* codegen_link(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return NULL;
}

static Value* codegen_note(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    s64 integer_value = node->Note.node->Int.val;
    if (integer_value < 1) error("note parameters start at 1.");
    AST* arg = get_arg_from_func(get_current_function(ctx), integer_value - 1);
    Value* var = get_variable(ctx, make_ast_ident(arg->loc_info, arg->Variable_Decl.name));
    emit_load(ctx, var);
    return var;
}

static Value* codegen_int(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    Value* val = make_value_int(DEFAULT_INT_BYTE_SIZE, node->type, node->Int.val);
    char* reg = get_result_reg(val->type);
    char* mov_op = get_move_op(val->type);
    emit(ctx, "%s %s, %d", mov_op, reg, val->Int.value);
    return val;
}

static Value* codegen_float(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    Value* val = make_value_float(node->type, node->Float.val);
    char* db_op = get_db_op(val->type);
    char* flabel = emit_data(ctx, "%s %f", db_op, val->Float.value);
    char* mov_op = get_move_op(val->type);
    char* reg = get_result_reg(val->type);
    emit(ctx, "%s %s, [rel %s]; float_ref", mov_op, reg, flabel);
    return val;
}

static Value* codegen_string(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    char* val = node->String.val;
    Type* t = make_type_pointer(make_type_int(8, 1));
    char* slabel = emit_data(ctx, "db `%s`, 0 ", val);
    char* mov_op = get_move_op(t);
    char* reg = get_result_reg(t);
    emit(ctx, "%s %s, %s; string_ref", mov_op, reg, slabel);
    return make_value_string(val, t);
}

static Value* codegen_char(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return codegen(ctx, make_ast_int(node->loc_info, node->Char.val, node->type));
}

static Value* codegen_ident(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    Value* var = get_variable(ctx, node);
    emit_load(ctx, var);
    return var;
}

static Value* codegen_call(Codegen_Context* ctx, AST* node) {
// From the System V Application Binary Interface Manual
// -- Passing
//  Once arguments are classified, the registers get assigned(in left-to-right order) for passing as follows:
//      1. If the class is MEMORY, pass the argument on the stack.
//      2. If the class is INTEGER, the next available register of the sequence %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used15.
//      3. If the class is SSE, the next available vector register is used, the registers are taken in the order from %xmm0 to %xmm7.
//      4. If the class is SSEUP, the eightbyte is passed in the next available eightbyte chunk of the last used vector register.
//      5. If the class is X87, X87UP or COMPLEX_X87, it is passed in memory.
//
//  When a value of type _Bool is returned or passed in a register or on the stack, bit 0 contains the truth value and bits 1 to 7 shall be zero.
//
//  If there are no registers available for any eightbyte of an argument, the whole argument is passed on the stack. If registers have already been assigned for
//  some eightbyte of such argument, the assignments get reverted.
//
//  Once registers are assigned, the arguments passed in memory are pushed on the stack in reversed (right-to-left) order.
//  For calls that may call functions that use varargs or stdargs (prototype-less calls or calls to functions containing ellipsis (...) in the declaration) %al is used
//  as hidden argument to specify the number of vector registers used. The contents of %al do not need to match exactly the number of registers, but must be an upper bound
//  on the number of vector registers used and is in the range 0-8 inclusive. When passing __m256 or __m512 arguments to functions that use varargs or stdargs, function
//  prototypes must be provided. Otherwise, the run-time behavior is undefined.
    DEBUG_START;

    char* callee = node->Call.callee;
    List* args = node->Call.args;
    Type* return_type = node->type;

    s8 class_integer_counter = 0; // used for getting the next available register
    s8 class_sse_counter = 0;     // used for getting the next available register

    List* values = make_list();
    list_foreach_reverse(args) {
        Value* v = codegen(ctx, it->data);
        push_type(ctx, v->type);
        list_append(values, v);
    }

    list_foreach_reverse(values) {
        Value* arg_v = it->data;
        Class_Kind class = classify(arg_v->type);

        s8 param_reg = -1;

        switch (class) {
        ERROR_UNHANDLED_CLASS_KIND(class);
        // case CLASS_MEMORY:          break;
        case CLASS_INTEGER: param_reg = get_parameter_reg_int(class_integer_counter++, 8); break;
        case CLASS_SSE:     param_reg = get_parameter_reg_float(class_sse_counter++);      break;
        // case CLASS_SSEUP:           break;
        // case CLASS_X87:             // fallthrough
        // case CLASS_X87UP:           // fallthrough
        // case CLASS_COMPLEX_X87:     break;
        }

        // char* mov_op = get_move_op(arg_v->type);
        // char* result_reg = get_result_reg_of_size(arg_v->type, 8);
        // emit(ctx, "%s %s, %s", mov_op, get_reg(param_reg), result_reg);

        // @FixMe!
        pop(ctx, param_reg);
    }

    if (node->type->flags & TYPE_FLAG_HAS_VAR_ARG) {
        emit(ctx, "mov al, %lld; var_arg_count", args->count);
    }

#ifdef __APPLE__
    emit(ctx, "call _%s", callee);
#elif __unix__
    emit(ctx, "call %s", callee);
#endif
    return make_value_call(callee, return_type);
}

static Value* codegen_unary(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    Token_Kind op = node->Unary.op;
    AST* operand = node->Unary.operand;

    Value* operand_val = codegen(ctx, operand);
    char* reg = get_result_reg(operand->type);
    Value* result = operand_val;

    switch (op) {
    ERROR_UNHANDLED_TOKEN_KIND(op);
    case TOKEN_MINUS_MINUS: {
        s64 size = 1;
        if (operand->type->kind == TYPE_POINTER)
            size = get_size_of_underlying_type_if_any(operand->type);
        result = codegen(ctx, make_ast_binary(node->loc_info, TOKEN_MINUS_EQ, operand, make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, 0))));
    } break;
    case TOKEN_PLUS_PLUS: {
        s64 size = 1;
        if (operand->type->kind == TYPE_POINTER)
            size = get_size_of_underlying_type_if_any(operand->type);
        result = codegen(ctx, make_ast_binary(node->loc_info, TOKEN_PLUS_EQ, operand, make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, 0))));
    } break;
    case THI_SYNTAX_ADDRESS: {
        s64 stack_pos = get_stack_pos_of_variable(operand_val);
        emit(ctx, "lea rax, [rbp-%lld]; addrsof", stack_pos);
    } break;
    case THI_SYNTAX_POINTER: {
        Type* t = operand->type;
        char* reg = get_result_reg(t);
        emit(ctx, "mov %s, [rax]; deref ", reg);
        // A deref expects an lvalue and returns an lvalue
    } break;
    case TOKEN_BANG: {
        emit(ctx, "cmp %s, 0", reg);
        emit(ctx, "sete al");
    } break;
    case TOKEN_PLUS: { // do nothing
    } break;
    case TOKEN_TILDE: {
        emit(ctx, "not al");
    } break;
    case TOKEN_DOT: {
    } break;
    case TOKEN_MINUS: {
        Type_Kind tk = operand->type->kind;
        switch (tk) {
            ERROR_UNHANDLED_TYPE_KIND(tk);
        case TYPE_INT:
            emit(ctx, "neg %s", reg);
            break;

        case TYPE_FLOAT:
            emit(ctx, "movd ecx, xmm0");
            emit(ctx, "xor ecx, 2147483648");
            emit(ctx, "movd xmm0, ecx");
            break;
        }
        break;
    }
    }
    return result;
}

static Value* codegen_binary(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    Token_Kind op = node->Binary.op;
    AST* lhs = node->Binary.lhs;
    AST* rhs = node->Binary.rhs;

    switch (op) {
    ERROR_UNHANDLED_TOKEN_KIND(op);
    case THI_SYNTAX_ASSIGNMENT: {

        Value* rhs_v = codegen(ctx, rhs);
        push_type(ctx, rhs_v->type);
        
        bool is_deref = false;
        if (lhs->kind == AST_UNARY) {
            lhs = lhs->Unary.operand;
            is_deref = true;
        }
        Value* lhs_v = codegen(ctx, lhs);
        
        pop_type_2(ctx, rhs_v->type);
        if (is_deref) {
            emit_store_deref(ctx, lhs_v);
        } else
            emit_store(ctx, lhs_v);
        emit_load(ctx, lhs_v);
        return lhs_v;
    }

    case TOKEN_PLUS: // fallthrough
    case TOKEN_MINUS: // fallthrough
    case TOKEN_ASTERISK: // fallthrough
    case TOKEN_FWSLASH: {
        Value* rhs_va = codegen(ctx, rhs);
        push_type(ctx, rhs->type);
        codegen(ctx, lhs);
        pop_type_2(ctx, rhs->type);
        Type* type = lhs->type; // both lhs and rhs are the same type
        char* instr = get_instr(op, type);
        char* op1 = get_result_reg(lhs->type);
        char* op2 = get_result_reg_2(rhs->type);
        emit(ctx, "%s %s, %s", instr, op1, op2);
        return rhs_va;
    }

    case TOKEN_PERCENT: {
        node = make_ast_binary(node->loc_info, TOKEN_FWSLASH, lhs, rhs);
        Value* variable = codegen(ctx, node);
        emit(ctx, "mov rax, rdx");
        return variable;
    }
    case TOKEN_PIPE: {
        Value* lhs_v = codegen(ctx, lhs);
        push(ctx, RAX);
        codegen(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "or al, cl");
        return lhs_v;
    }
    case TOKEN_AND: {
        Value* lhs_v = codegen(ctx, lhs);
        push(ctx, RAX);
        codegen(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "and al, cl");
        return lhs_v;
    }
    case TOKEN_HAT: {
        Value* lhs_v = codegen(ctx, lhs);
        push(ctx, RAX);
        codegen(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "xor al, cl");
        return lhs_v;
    }
    case TOKEN_LT_LT: {
        Value* lhs_v = codegen(ctx, lhs);
        xassert(rhs->kind == AST_INT);
        emit(ctx, "shl al, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_GT_GT: {
        Value* lhs_v = codegen(ctx, lhs);
        xassert(rhs->kind == AST_INT);
        emit(ctx, "shr al, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_AND_AND: {
        Value* lhs_v = codegen(ctx, lhs);
        push(ctx, RAX);
        codegen(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "cmp rax, 0");
        emit(ctx, "setne al");
        emit(ctx, "cmp rcx, 0");
        emit(ctx, "setne cl");
        emit(ctx, "and al, cl");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        node = make_ast_binary(node->loc_info, TOKEN_PIPE, lhs, rhs);
        Value* v = codegen(ctx, node);
        emit(ctx, "setne al");
        return v;
    }

    case TOKEN_LT:    // FALLTHROUGH
    case TOKEN_GT:    // FALLTHROUGH
    case TOKEN_LT_EQ: // FALLTHROUGH
    case TOKEN_GT_EQ: // FALLTHROUGH
    case TOKEN_EQ_EQ: // FALLTHROUGH
    case TOKEN_BANG_EQ: {
        Value* lhs_v = codegen(ctx, lhs);
        if (lhs_v->type->kind == TYPE_FLOAT) {
            push(ctx, XMM0);
            codegen(ctx, rhs);
            pop(ctx, XMM1);
            if (lhs_v->type->Float.bytes == 4) {
                emit(ctx, "ucomiss xmm1, xmm0");
            } else {
                emit(ctx, "ucomisd xmm1, xmm0");
            }
        } else {
            push(ctx, RAX);
            codegen(ctx, rhs);
            pop(ctx, RCX);
            emit(ctx, "cmp rcx, rax");
        }
        switch (op) {
            ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_LT:
            lhs_v->type->kind == TYPE_FLOAT ? emit(ctx, "setb al")
                                            : emit(ctx, "setl al");
            break;
        case TOKEN_GT: emit(ctx, "setg al"); break;
        case TOKEN_LT_EQ:
            lhs_v->type->kind == TYPE_FLOAT ? emit(ctx, "setna al")
                                            : emit(ctx, "setle al");
            break;
        case TOKEN_GT_EQ: emit(ctx, "setge al"); break;
        case TOKEN_EQ_EQ: emit(ctx, "sete al"); break;
        case TOKEN_BANG_EQ: emit(ctx, "setne al"); break;
        }
        return lhs_v;
    } break;

    case TOKEN_LT_LT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_LT_LT, lhs, rhs);
        node = make_ast_unary(node->loc_info, TOKEN_EQ, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_GT_GT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_GT_GT, lhs, rhs);
        node = make_ast_unary(node->loc_info, TOKEN_EQ, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_PLUS_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_PLUS, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_MINUS_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_MINUS, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_ASTERISK_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_ASTERISK, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_FWSLASH_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_FWSLASH, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_PERCENT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_PERCENT, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_PIPE_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_PIPE, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_HAT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_HAT, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen(ctx, node);
        return variable;
    }
    case TOKEN_QUESTION_MARK: {
        char* l0 = make_text_label(ctx);
        char* l1 = make_text_label(ctx);
        set_temp_labels(ctx, l0, l1);
        Value* lhs_v = codegen(ctx, lhs);
        char* reg = get_result_reg(lhs_v->type);
        emit(ctx, "cmp %s, 0", reg);
        emit(ctx, "je %s", l0);
        Value* rhs_v = codegen(ctx, rhs);
        emit(ctx, "jmp %s", l1);
        return rhs_v;
    }
    case TOKEN_COLON: {
        codegen(ctx, lhs); // '?' part
        emit(ctx, "%s:", get_l0(ctx));
        Value* rhs_v = codegen(ctx, rhs);
        emit(ctx, "%s:", get_l1(ctx));
        restore_temp_labels(ctx);
        return rhs_v;
    }
    }

    UNREACHABLE;
    return NULL;
}

static Value* codegen_grouping(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return codegen(ctx, node->Grouping.node);
}

static Value* codegen_subscript(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_field_access(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_as(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    AST* e = node->As.node;
    Type* t = node->As.type_node->type;
    Value* v = codegen(ctx, e);
    emit_cast(ctx, v, t);
    return v;
}

static Value* codegen_block(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    push_scope(ctx);
    List* stmts = node->Block.stmts;
    Value* last = NULL;
    list_foreach(stmts) {
        last = codegen(ctx, it->data);
    }
    pop_scope(ctx);
    return last;
}

static Value* codegen_struct(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return make_value_struct(node->type);
}

static Value* codegen_enum(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    xassert(node->kind == AST_ENUM);
    warning("enum incomplete?");
    return NULL;
}

static Value* codegen_function(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    set_current_function_expr(ctx, node);

    char* func_name = node->Function.name;
    AST* func_body = node->Function.body;

    push_scope(ctx);

#ifdef __APPLE__
    emit(ctx, "_%s:", func_name);
#elif __unix__
    emit(ctx, "%s:", func_name);
#endif
    push(ctx, RBP);
    emit(ctx, "mov rbp, rsp");

    // Figure out how much stack to allocate + padding
    s64 sum = get_size_of_type(node->type);
    sum += get_all_alloca_in_block(func_body);
    s64 stack_allocated = sum;
    s32 padding = X64_ASM_MACOS_STACK_PADDING - (stack_allocated % X64_ASM_MACOS_STACK_PADDING);
    if (stack_allocated + padding)
        emit(ctx, "sub rsp, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);

    // Emit our function label
    emit(ctx, "%s:", DEFAULT_FUNCTION_ENTRY_LABEL_NAME);

    // Make sure the stack starts at zero. @Note(marcus) this should not be needed. I believe it is handled automatically.
    reset_stack(ctx);

    List* args = node->Function.parameters;

    s8 class_integer_counter = 0;   // used for getting the next available register
    s8 class_sse_counter     = 0;   // used for getting the next available register

    List* classified_arguments = classify_arguments(args);
    list_foreach(classified_arguments) {

        ClassifiedArgument* ca = it->data;
        AST* arg = ca->argument;
        Class_Kind class = ca->class;

        Value* arg_v = codegen(ctx, arg);
        s64 size = get_size_of_value(arg_v);
        Register_Kind param_reg = -1;

        switch (class) {
        ERROR_UNHANDLED_CLASS_KIND(class);
        // case CLASS_MEMORY: break;
        case CLASS_INTEGER: param_reg = get_parameter_reg_int(class_integer_counter++, size); break;
        case CLASS_SSE:     param_reg = get_parameter_reg_float(class_sse_counter++);         break;
        // case CLASS_SSEUP:       break;
        // case CLASS_X87:         // fallthrough
        // case CLASS_X87UP:       // fallthrough
        // case CLASS_COMPLEX_X87: break;
        }

        emit_store_r(ctx, arg_v, param_reg);
    }

    push_scope(ctx);
    List* stmts = func_body->Block.stmts;
    list_foreach(stmts) {
        codegen(ctx, it->data);
    }

    emit(ctx, "%s:", DEFAULT_FUNCTION_END_LABEL_NAME);

    List* defers = get_current_function(ctx)->Function.defers;
    list_foreach_reverse(defers) {
        codegen(ctx, it->data);
    }
    pop_scope(ctx);

    if (stack_allocated + padding) {
        emit(ctx, "add rsp, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);
    }

    emit(ctx, "leave");
    emit(ctx, "ret");

    reset_text_label_counter(ctx);

    pop_scope(ctx);
    return NULL;
}

static Value* codegen_variable_decl(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    char* name = node->Variable_Decl.name;
    Type* type = node->type;
    AST* assignment_expr = node->Variable_Decl.value;
    bool is_global = node->flags & AST_FLAG_GLOBAL_VARIABLE;
    Value* variable = NULL;
    // Set the label for it..
    if (is_global) {
        char* initial_value = NULL;
        if (assignment_expr)
            initial_value = ast_get_literal_value_as_str(assignment_expr);
        else
            initial_value = "0";
        char* db_op = get_db_op(type);
        char* label = emit_data(ctx, "%s %s", db_op, initial_value);
        variable = make_value_global_variable(name, type, label);
    } else {
        s64 type_size = get_size_of_type(type);
        s64 stack_pos = type_size + get_stack_pos(ctx);
        debug("name: %s stack_pos: %d type_size: %d", name, stack_pos, type_size);
        variable = make_value_variable(name, type, stack_pos);
    }
    add_variable(ctx, variable);
    if (assignment_expr && !is_global) {
        codegen(ctx, make_ast_binary(node->loc_info, TOKEN_EQ, make_ast_ident(node->loc_info, name), assignment_expr));
    }
    return variable;
}

static Value* codegen_if(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    char* else_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    AST* cond = node->If.cond;
    AST* then_block = node->If.then_block;
    AST* else_block = node->If.else_block;

    codegen(ctx, cond);
    emit(ctx, "cmp al, 0");
    emit(ctx, "je %s", else_block ? else_l : end_l);
    codegen(ctx, then_block);
    emit(ctx, "jmp %s", end_l);
    emit(ctx, "%s:", else_l);
    if (else_block) {
        codegen(ctx, else_block);
    }
    emit(ctx, "%s:", end_l);

    return NULL;
}

static Value* codegen_for(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    push_scope(ctx);

    char* begin_l = make_text_label(ctx);
    char* mid_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    AST* init = node->For.init;
    AST* cond = node->For.cond;
    AST* step = node->For.step;
    AST* then_block = node->For.then_block;

    codegen(ctx, init);
    emit(ctx, "%s:", begin_l);
    codegen(ctx, cond);
    emit(ctx, "cmp al, 0");
    emit(ctx, "je %s", end_l);

    set_jump_labels(ctx, mid_l, end_l);
    codegen(ctx, then_block);
    restore_jump_labels(ctx);

    emit(ctx, "%s:", mid_l);
    codegen(ctx, step);
    emit(ctx, "jmp %s", begin_l);
    emit(ctx, "%s:", end_l);

    pop_scope(ctx);

    return NULL;
}

static Value* codegen_while(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    xassert(node->kind == AST_WHILE);

    char* begin_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    AST* cond = node->While.cond;
    AST* then_block = node->While.then_block;

    emit(ctx, "%s:", begin_l);
    codegen(ctx, cond);
    emit(ctx, "cmp al, 0");
    emit(ctx, "je %s", end_l);

    set_jump_labels(ctx, begin_l, end_l);
    codegen(ctx, then_block);
    restore_jump_labels(ctx);

    emit(ctx, "jmp %s", begin_l);
    emit(ctx, "%s:", end_l);

    return NULL;
}

static Value* codegen_return(Codegen_Context* ctx, AST* node) {
// From the System V Application Binary Interface Manual
// -- Returning of Values
//  The returning of values is done according to the following algorithm:
//      1. Classify the return type with the classification algorithm.
//      2. If the type has class MEMORY, then the caller provides space for the return value and passes the address of this storage in %rdi as if it were the first argument to the function.
//         In effect, this address becomes a “hidden” first ar- gument. This storage must not overlap any data visible to the callee through other names than this argument.
//         On return %rax will contain the address that has been passed in by the caller in %rdi.
//      3. If the class is INTEGER, the next available register of the sequence %rax, %rdx is used.
//      4. If the class is SSE, the next available vector register of the sequence %xmm0, %xmm1 is used.
//      5. If the class is SSEUP, the eightbyte is returned in the next available eightbyte chunk of the last used vector register.
    DEBUG_START;
    AST* ret_e = node->Return.node;
    Value* ret_v = NULL;
    if (ret_e) {
        Class_Kind class = classify(ret_e->type);
        ret_v = codegen(ctx, ret_e);
        s64 size = get_size_of_value(ret_v);
        s8 return_reg = -1;
        s8 class_integer_counter = 0;
        s8 class_sse_counter = 0;
        switch (class) {
        ERROR_UNHANDLED_CLASS_KIND(class);
        // case CLASS_MEMORY: break;
        case CLASS_INTEGER: return_reg = get_return_reg_int(class_integer_counter++, size); break;
        case CLASS_SSE:
            return_reg = get_return_reg_float(class_sse_counter++);
            break;
            // case CLASS_SSEUP: break;
        }
        char* mov_op = get_move_op(ret_v->type);
        char* result_reg = get_result_reg(ret_v->type);
        emit(ctx, "%s %s, %s", mov_op, get_reg(return_reg), result_reg);
    }
    emit(ctx, "jmp %s", DEFAULT_FUNCTION_END_LABEL_NAME);
    return ret_v;
}

static Value* codegen_defer(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    AST* defer_expr = node->Defer.node;
    list_append(get_current_function(ctx)->Function.defers, defer_expr);
    return NULL;
}

static Value* codegen_break(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    emit(ctx, "jmp %s", get_lbreak(ctx));
    return NULL;
}

static Value* codegen_continue(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    emit(ctx, "jmp %s", get_lcontinue(ctx));
    return NULL;
}

static Value* codegen_typeof(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_sizeof(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

static Value* codegen_switch(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    char* default_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    set_break_label(ctx, end_l);

    AST* cond = node->Switch.cond;
    AST* cases = node->Switch.cases;
    AST* default_case = node->Switch.default_case;

    List* labels = make_list();
    list_foreach(cases->Block.stmts) {
        AST* c = (AST*)it->data;

        char* l = make_text_label(ctx);
        list_append(labels, l);

        AST* case_cond = c->Is.node;
        codegen(ctx, make_ast_binary(c->loc_info, TOKEN_EQ_EQ, cond, case_cond));

        emit(ctx, "cmp al, 1");
        emit(ctx, "je %s", l);
    }

    if (default_case) {
        emit(ctx, "%s:", default_l);
        codegen(ctx, default_case);
        emit(ctx, "jmp %s", end_l);
    } else
        emit(ctx, "jmp %s", end_l);

    List_Node* label_it = labels->head;
    list_foreach(cases->Block.stmts) {
        AST* c = (AST*)it->data;

        char* l = (char*)label_it->data;
        emit(ctx, "%s:", l);
        codegen(ctx, c->Is.body);

        if (!c->Is.has_fallthrough) {
            emit(ctx, "jmp %s", end_l);
        }

        label_it = label_it->next;
    }

    emit(ctx, "%s:", end_l);

    restore_break_label(ctx);

    return NULL;
}

static Value* codegen_post_inc_or_dec(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    Value* v = codegen(ctx, node->Post_Inc_or_Dec.node);
    push_type(ctx, v->type);
    v = codegen(ctx, make_ast_unary(node->loc_info, node->Post_Inc_or_Dec.op, node->Post_Inc_or_Dec.node));
    pop_type(ctx, v->type);
    return v;
}

static Value* codegen_literal(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    UNFINISHED;
    return NULL;
}

// @Cleanup @Audit @Volatile @Ugly
static Value* codegen_asm(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    AST* block = node->Asm.block;

    list_foreach(block->Block.stmts) {
        AST* stmt = it->data;
        switch (stmt->kind) {
        ERROR_UNHANDLED_AST_KIND(stmt->kind);
        case AST_STRING: {
            char* ss = stmt->String.val;
            char* str = stmt->String.val;
            char c;
            debug("%s", str);
            char* substart =  NULL;
            char* subend =  NULL;
            bool has_sub  = false;
            while ((c = *str)) {
                if (c == '%') {
                    has_sub = true;
                    substart = str;
                    ++str;
                    while (*str == ' ') ++str; // skip whitespace

                    char* start = str;
                    while (*str != '\0' && *str != ',' && *str != ' ' && *str != '"') ++str;
                    char* end = str;
                    char* identifier = strn(start, end);

                    Value* var = get_variable(ctx, make_ast_ident(stmt->loc_info, identifier));
                    char* mem = get_mem_loc(var);
                    ss = mem;

                    subend = end;
                }
                ++str;
            }

            if (has_sub) {
                char* s1 = strn(stmt->String.val, substart);
                char* s2 = ss;
                char* s3 = strn(subend, str);
                emit(ctx, "%s%s%s", s1, s2, s3);
            } else emit(ctx, "%s", ss);
        } break;
        }

    }

    return NULL;
}

//------------------------------------------------------------------------------
//                              Codegen Helpers
//------------------------------------------------------------------------------


typedef enum {
    OPER_MEM,
    OPER_REG,
    OPER_IMM,
    _OPER_COUNT_,
} Operand_Kind;

typedef struct {
    Operand_Kind kind;
} Operand;

typedef enum {
    INSTRUCTION_MOV,
    INSTRUCTION_ADD,
    INSTRUCTION_SUB,
    INSTRUCTION_IMUL,
    INSTRUCTION_MUL,
    INSTRUCTION_IDIV,
    INSTRUCTION_DIV,
    INSTRUCTION_PUSH,
    INSTRUCTION_POP,
    INSTRUCTION_JMP,
    INSTRUCTION_JE,
    _INSTRUCTION_COUNT_,
} Instruction_Kind;

typedef struct {
    char* label;
    Instruction_Kind instr;
    Operand o1, o2;
    char* comment;
} NASM_Line;



typedef struct {
    char* label;
    char* str;
} Data;

AST* get_current_function(Codegen_Context* ctx) {
    return ctx->current_function;
}
s64 get_stack_pos(Codegen_Context* ctx) {
    return ctx->stack_pos;
}
char* get_o0(Codegen_Context* ctx) {
    return ctx->o0;
}
char* get_o1(Codegen_Context* ctx) {
    return ctx->o1;
}
char* get_l0(Codegen_Context* ctx) {
    return ctx->l0;
}
char* get_l1(Codegen_Context* ctx) {
    return ctx->l1;
}
char* get_ocontinue(Codegen_Context* ctx) {
    return ctx->ocontinue;
}
char* get_lcontinue(Codegen_Context* ctx) {
    return ctx->lcontinue;
}
char* get_obreak(Codegen_Context* ctx) {
    return ctx->obreak;
}
char* get_lbreak(Codegen_Context* ctx) {
    return ctx->lbreak;
}
char* get_l_end(Codegen_Context* ctx) {
    return ctx->l_end;
}

void push_scope(Codegen_Context* ctx) {
    Scope* top = stack_peek(ctx->scopes);
    Scope* s = xmalloc(sizeof(Scope));
    s->stack_pos = top->stack_pos;
    s->symbols = make_map();
    stack_push(ctx->scopes, s);
}
void pop_scope(Codegen_Context* ctx) {
    Scope* s = stack_pop(ctx->scopes);
    ctx->stack_pos = s->stack_pos;
}
static void add_variable_to_current_scope(Codegen_Context* ctx, Value* variable) {
    debug("Adding variable %s of %s to scope", get_value_name(variable), value_kind_to_str(variable->kind));
    Scope* top = stack_peek(ctx->scopes);
    if (variable->kind != VALUE_GLOBAL_VARIABLE)
        top->stack_pos += get_size_of_value(variable);
    map_set(top->symbols, get_value_name(variable), variable);
}

Codegen_Context make_codegen_context() {
    Codegen_Context ctx;
    
    ctx.scopes = make_stack();
    Scope* s = xmalloc(sizeof(Scope));
    s->stack_pos = 0;
    s->symbols = make_map();
    stack_push(ctx.scopes,  s);

    ctx.data_list = make_list();
    ctx.current_function = NULL;
    ctx.expected_type = NULL;
    ctx.inside_asm = false;
    ctx.section_extern = make_string("");
    ctx.section_text = make_string("");
    ctx.section_data = make_string("");
    ctx.stack_pos = 0;
    ctx.text_label_counter = 0;
    ctx.data_label_counter = 0;
    ctx.ocontinue = NULL;
    ctx.lcontinue = NULL;
    ctx.obreak = NULL;
    ctx.lbreak = NULL;
    ctx.l_end = NULL;
    ctx.l0 = NULL;
    ctx.l1 = NULL;
    ctx.o0 = NULL;
    ctx.o1 = NULL;
    ctx.next_available_xmm_reg_counter = 0;
    ctx.next_available_rax_reg_counter = 0;

    string_append(ctx.section_data, "section .data\n");
    emit_no_tab(&ctx, "section .text");

    return ctx;
}

void emit_no_tab(Codegen_Context* ctx, char* fmt, ...) {
    xassert(ctx);
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // xstrlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    string_append_f(ctx->section_text, "%s\n", str);

    free(str);
}

void emit_extern(Codegen_Context* ctx, char* fmt, ...) {
    xassert(ctx);
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // xstrlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    string_append_f(ctx->section_extern, "extern _%s\n", str);

    free(str);
}


char* emit_data(Codegen_Context* ctx, char* fmt, ...) {
    xassert(ctx);
    
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // xstrlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    char* label = NULL;

    // @Todo: this is an optimization. It does not belong here.
    // Removes duplicate data entries
    
    // list_foreach(ctx->data_list) {
    //     Data* s = it->data;
    //     if (strcmp(s->str, str) == 0) {
    //         label = s->label;
    //         break;
    //     }
    // }
    //
    if (!label) {
        label = make_data_label(ctx);
        Data* d = xmalloc(sizeof(Data));
        d->label = label;
        d->str = str;
        list_append(ctx->data_list, d);
        string_append_f(ctx->section_data, "\t%s: %s\n", label, str);
    }

    return label;
}

void emit(Codegen_Context* ctx, char* fmt, ...) {
    xassert(ctx);
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // xstrlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    bool is_label = false;
    foreach(i, str_len) {
        if (str[i] == ':') {
            is_label = true;
            break;
        }
    }

    if (is_label) {
        string_append_f(ctx->section_text, "%s\n", str);
    } else {
        string_append_f(ctx->section_text, "\t\t%s\n", str);
    }

    free(str);
}

void push_type(Codegen_Context* ctx, Type* type) {
    xassert(type);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_ARRAY: {
        // Push each element in the array
        s64 size = type->Array.size;
        Type* contained_type = type->Array.type;
        foreach(i, size) {
            push_type(ctx, contained_type);
        }
        break;
    }
    case TYPE_ENUM: // fallthrough
    case TYPE_STRUCT: {
        // Push each member recursivly onto the stack
        list_foreach(type_get_members(type)) {
            Type_Name_Pair* member = it->data;
            push_type(ctx, member->type);
        }
        break;
    }
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: push(ctx, RAX); break;
    case TYPE_FLOAT: push(ctx, XMM0); break;
    }
}

void pop_type_2(Codegen_Context* ctx, Type* type) {
    xassert(type);
    switch (type->kind) {
    ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_ARRAY:   // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_INT: pop(ctx, RCX); break;
    case TYPE_FLOAT: pop(ctx, XMM1); break;
    }
}
void pop_type(Codegen_Context* ctx, Type* type) {
    xassert(ctx);
    xassert(type);
    switch (type->kind) {
    ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_ARRAY: {
        // Push each element in the array
        s64 size = type->Array.size;
        Type* contained_type = type->Array.type;
        foreach(i, size) {
            pop_type(ctx, contained_type);
        }
        break;
    }
    case TYPE_ENUM: // fallthrough
    case TYPE_STRUCT: {
        // Push each member recursivly onto the stack
        list_foreach(type_get_members(type)) {
            Type_Name_Pair* member = it->data;
            pop_type(ctx, member->type);
        }
        break;
    }
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: pop(ctx, RAX); break;
    case TYPE_FLOAT: pop(ctx, XMM0); break;
    }
}

void push(Codegen_Context* ctx, int reg) {
    xassert(ctx);
    tassert(reg >= 0 && reg <= _REGISTER_COUNT_, "reg = %d", reg);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "sub rsp, 8");
        emit(ctx, "movsd [rsp], %s", r);
    } else {
        emit(ctx, "push %s", r);
    }
    ctx->stack_pos += 8;
}

void pop(Codegen_Context* ctx, int reg) {
    xassert(ctx);
    tassert(reg >= 0 && reg <= _REGISTER_COUNT_, "reg = %d", reg);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "movsd %s, [rsp]", r);
        emit(ctx, "add rsp, 8");
    } else {
        emit(ctx, "pop %s", r);
    }
    ctx->stack_pos -= 8;
    xassert(ctx->stack_pos >= 0);
}

char* get_op_size(s8 bytes) {
    tassert(bytes >= 1 && bytes <= 8, "bytes = %d", bytes);
    switch (bytes) {
    case 1: return "byte";
    case 2: return "word";
    case 4: return "dword";
    case 8: return "qword";
    default: return "qword";
    }
    UNREACHABLE;
    return NULL;
}

char* get_result_reg_2(Type* type) {
    xassert(type);
    s64 size = get_size_of_type(type);
    tassert(size >= 1 && size <= 8, "size = %d", size);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    // case TYPE_ARRAY:   // return get_reg(RCX);
    // case TYPE_STRUCT:  // fallthrough
    // case TYPE_ENUM:    // fallthrough
    // case TYPE_VOID:    // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: return get_reg(get_rax_reg_of_byte_size(size, 'c'));
    case TYPE_FLOAT: return "xmm1";
    }
    UNREACHABLE;
    return NULL;
}

char* get_result_reg_of_size(Type* type, s8 size) {
    xassert(type);
    tassert(size >= 1 && size <= 8, "size = %d", size);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    // case TYPE_ARRAY:   // return get_reg(RCX);
    // case TYPE_STRUCT:  // fallthrough
    // case TYPE_ENUM:    // fallthrough
    // case TYPE_VOID:    // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: return get_reg(get_rax_reg_of_byte_size(size, 'a'));
    case TYPE_FLOAT: return "xmm0";
    }
    UNREACHABLE;
    return NULL;
}

char* get_result_reg(Type* type) {
    xassert(type);
    s64 size = get_size_of_type(type);
    tassert(size >= 1 && size <= 8, "size = %d", size);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    // case TYPE_ARRAY:   // return get_reg(RCX);
    // case TYPE_STRUCT:  // fallthrough
    // case TYPE_ENUM:    // fallthrough
    // case TYPE_VOID:    // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: return get_reg(get_rax_reg_of_byte_size(size, 'a'));
    case TYPE_FLOAT: return "xmm0";
    }
    UNREACHABLE;
    return NULL;
}

char* get_db_op(Type* type) {
    xassert(type);
    s64 size = get_size_of_type(type);
    tassert(size >= 1 && size <= 8, "size = %d", size);
    switch (size) {
    case 1: return "db";
    case 2: return "dw";
    case 4: return "dd";
    case 8: return "dq";
    }
    UNREACHABLE;
    return NULL;
}

char* get_move_op(Type* type) {
    xassert(type);
    s64 size = get_size_of_type(type);
    tassert(size >= 1 && size <= 8, "size = %d", size);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    // case TYPE_ARRAY:   // return "lea";
    // case TYPE_STRUCT:  // fallthrough
    // case TYPE_ENUM:    // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: return "mov";
    case TYPE_FLOAT:
        switch (size) {
        case 4: return "movss";
        case 8: return "movsd";
        }
    }
    UNREACHABLE;
    return NULL;
}

void alloc_variable(Codegen_Context* ctx, Value* variable) {
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    debug("Allocating variable '%s', type '%s', size '%lld' ",
         variable->Variable.name,
         get_type_name(variable->type),
         size);
    ctx->stack_pos += size;
}

void dealloc_variable(Codegen_Context* ctx, Value* variable) {
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    debug("Deallocating variable '%s', type '%s', size '%lld' ",
         variable->Variable.name,
         get_type_name(variable->type),
         size);
    ctx->stack_pos -= size;
    xassert(ctx->stack_pos >= 0);
}

Value* get_variable(Codegen_Context* ctx, AST* ident) {
    xassert(ctx && ident);
    char* name = ident->Ident.name;
    stack_foreach(ctx->scopes) {
        Scope* scope = it->data;
        Value* v;
        if ((v = map_get(scope->symbols, name))) 
            return v;
    }
    error("no variable with name '%s'", name);
    UNREACHABLE;
    return NULL;

}

void add_variable(Codegen_Context* ctx, Value* variable) {
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE || variable->kind == VALUE_GLOBAL_VARIABLE);
    if (variable->kind != VALUE_GLOBAL_VARIABLE) {
        alloc_variable(ctx, variable);
    }
    add_variable_to_current_scope(ctx, variable);
}

int align(int n, s32 m) {
    return (m - (n % m)) % m;
}

void emit_cast_float_to_int(Codegen_Context* ctx, char* reg, Type* type) {
    xassert(ctx);
    xassert(reg);
    xassert(type);
    xassert(type->kind == TYPE_INT);
    bool usig = type->Int.is_unsigned;
    s8 type_size = get_size_of_type(type);
    switch (type_size) {
    case 4: emit(ctx, "cvttss2si %s, xmm0", reg); break;
    case 8: emit(ctx, "cvttsd2si %s, xmm0", reg); break;
    }
    if (usig) {
        emit_cast_int_to_int(ctx, reg, make_type_int(type_size, usig));
    }
}

void emit_cast_int_to_int(Codegen_Context* ctx, char* reg, Type* type) {
    xassert(ctx);
    xassert(reg);
    xassert(type);
    xassert(type->kind == TYPE_INT);
    bool usig = type->Int.is_unsigned;
    s64 type_size = get_size_of_type(type);
    switch (type_size) {
    case 1:
        usig ? emit(ctx, "movzbq %s, al", reg)
             : emit(ctx, "movsbq %s, al", reg);
        break;
    case 2:
        usig ? emit(ctx, "movzwq %s, ax", reg)
             : emit(ctx, "movswq %s, ax", reg);
        break;
    case 4:
        usig ? emit(ctx, "mov %s, %s", reg, reg) : 
               emit(ctx, "cltq");
        break;
        // case 8: // fallthrough
    }
}

void emit_cast(Codegen_Context* ctx, Value* variable, Type* desired_type) {
    xassert(ctx);
    xassert(variable);
    xassert(desired_type);
    Type* type = variable->type;
    char* reg = get_result_reg(type);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_INT: {
        if (desired_type->kind == TYPE_INT) {
            emit_cast_int_to_int(ctx, reg, desired_type);
        }
    } break;
    case TYPE_FLOAT: {
        if (desired_type->kind == TYPE_INT) {
            emit_cast_float_to_int(ctx, reg, desired_type);
        }
    } break;
    }
}

void emit_store_r(Codegen_Context* ctx, Value* variable, Register_Kind reg) {
    xassert(ctx);
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE);
    tassert(reg >= 0 && reg <= _REGISTER_COUNT_, "reg = %d", reg);
    char* mem = get_mem_loc(variable);
    char* reg_c = get_reg(reg);
    char* mov_op = get_move_op(variable->type);
    emit(ctx, "%s %s, %s; store_r %s", mov_op, mem, reg_c, (variable->Variable.name));
}

void emit_store_deref(Codegen_Context* ctx, Value* variable) {
    xassert(ctx);
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE);
    char* reg = get_result_reg_2(variable->type);
    char* mov_op = get_move_op(variable->type);
    emit(ctx, "%s [rax], %s; store %s", mov_op, reg, (variable->Variable.name));
}

void emit_store(Codegen_Context* ctx, Value* variable) {
    xassert(ctx);
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE || variable->kind == VALUE_GLOBAL_VARIABLE);
    char* mem = get_mem_loc(variable);
    char* reg = get_result_reg_2(variable->type);
    char* mov_op = get_move_op(variable->type);
    emit(ctx, "%s %s, %s; store %s", mov_op, mem, reg, get_value_name(variable));
}

void emit_load(Codegen_Context* ctx, Value* variable) {
    xassert(ctx);
    xassert(variable);
    xassert(variable->kind == VALUE_VARIABLE || variable->kind == VALUE_GLOBAL_VARIABLE);
    char* mem = get_mem_loc(variable);
    char* reg = get_result_reg(variable->type);
    char* mov_op = get_move_op(variable->type);
    emit(ctx, "%s %s, %s; load %s", mov_op, reg, mem, get_value_name(variable));
}

void set_break_label(Codegen_Context* ctx, char* break_l) {
    xassert(ctx);
    ctx->obreak = ctx->lbreak;
    ctx->lbreak = break_l;
}
void restore_break_label(Codegen_Context* ctx) {
    xassert(ctx);
    ctx->lbreak = ctx->obreak;
}

void set_continue_label(Codegen_Context* ctx, char* continue_l) {
    xassert(ctx);
    xassert(continue_l);
    ctx->ocontinue = ctx->lcontinue;
    ctx->lcontinue = continue_l;
}
void restore_continue_label(Codegen_Context* ctx) {
    xassert(ctx);
    ctx->lcontinue = ctx->ocontinue;
}

void set_jump_labels(Codegen_Context* ctx, char* continue_l, char* break_l) {
    xassert(ctx);
    set_continue_label(ctx, continue_l);
    set_break_label(ctx, break_l);
}

void restore_jump_labels(Codegen_Context* ctx) {
    xassert(ctx);
    restore_continue_label(ctx);
    restore_break_label(ctx);
}

void set_temp_labels(Codegen_Context* ctx, char* l0, char* l1) {
    xassert(ctx);
    xassert(l0);
    xassert(l1);
    ctx->o0 = ctx->l0;
    ctx->o1 = ctx->l1;
    ctx->l0 = l0;
    ctx->l1 = l1;
}
void restore_temp_labels(Codegen_Context* ctx) {
    xassert(ctx);
    ctx->l0 = ctx->o0;
    ctx->l1 = ctx->o1;
}
char* make_text_label(Codegen_Context* ctx) {
    xassert(ctx);
    char* l = strf(".l%d", ctx->text_label_counter);
    ctx->text_label_counter += 1;
    return l;
}

char* make_data_label(Codegen_Context* ctx) {
    xassert(ctx);
    char* l = strf("d%d", ctx->data_label_counter);
    ctx->data_label_counter += 1;
    return l;
}

void reset_text_label_counter(Codegen_Context* ctx) {
    xassert(ctx);
    ctx->text_label_counter = 0;
}
void reset_stack(Codegen_Context* ctx) {
    xassert(ctx);
    ctx->stack_pos = 0;
}
void set_current_function_expr(Codegen_Context* ctx, AST* func_expr) {
    xassert(ctx);
    ctx->current_function = func_expr;
}

char* emit_save_result(Codegen_Context* ctx, Value* value) {
    xassert(ctx);
    xassert(value);
    char* mov_op = get_move_op(value->type);
    char* reg = get_next_available_reg_fitting(ctx, value->type);
    char* result_reg = get_result_reg(value->type);
    emit(ctx, "%s %s, %s", mov_op, reg, result_reg);
    return reg;
}

char* get_instr(Token_Kind op, Type* type) {
    xassert(type);
    char* inst = NULL;
    switch (type->kind) {
    ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: {
        bool usig = type->Int.is_unsigned;
        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_PLUS:     inst = "add";                   break;
        case TOKEN_MINUS:    inst = "sub";                   break;
        case TOKEN_ASTERISK: inst = "imul";                  break;
        case TOKEN_FWSLASH:  inst = (usig ? "div" : "idiv"); break;
        }
    } break;
    case TYPE_FLOAT: {
        s64 size = get_size_of_type(type);
        tassert(size == 4 || size == 8, "size = %d", size);
        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_PLUS:     inst = (size == 8 ? "addsd" : "addss"); break;
        case TOKEN_MINUS:    inst = (size == 8 ? "subsd" : "subss"); break;
        case TOKEN_ASTERISK: inst = (size == 8 ? "mulsd" : "mulss"); break;
        case TOKEN_FWSLASH:  inst = (size == 8 ? "divsd" : "divss"); break;
        }
    } break;
    }
    return inst;
}

char* get_next_available_reg_fitting(Codegen_Context* ctx, Type* type) {
    s64 size = get_size_of_type(type);
    s8 r = -1;
    switch (type->kind) {
    ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_ARRAY:   // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_INT:     r = get_next_available_rax_reg_fitting(ctx, size);  break;
    case TYPE_FLOAT:   r = get_next_available_xmm_reg_fitting(ctx);        break;
    }
    return get_reg(r);
}

s8 get_next_available_xmm_reg_fitting(Codegen_Context* ctx) {
    s8 res = -1;
    switch (ctx->next_available_xmm_reg_counter) {
    case 0: res = XMM8; break;
    case 1: res = XMM9; break;
    case 2: res = XMM10; break;
    case 3: res = XMM11; break;
    case 4: res = XMM12; break;
    case 5: res = XMM13; break;
    case 6: res = XMM14; break;
    case 7: res = XMM15; break;
    }

    if (ctx->next_available_xmm_reg_counter == 7)
        ctx->next_available_xmm_reg_counter = 0;
    ++ctx->next_available_xmm_reg_counter;
    return res;
}

s8 get_next_available_rax_reg_fitting(Codegen_Context* ctx, s64 size) {
    s8 res = -1;
    switch (ctx->next_available_rax_reg_counter) {
    case 0:
        switch (size) {
        case 8: res = R10; break;
        case 4: res = R10D; break;
        case 2: res = R10W; break;
        case 1: res = R10B; break;
        }
        break;
    case 1:
        switch (size) {
        case 8: res = R11; break;
        case 4: res = R11D; break;
        case 2: res = R11W; break;
        case 1: res = R11B; break;
        }
        break;
    case 2:
        switch (size) {
        case 8: res = R12; break;
        case 4: res = R12D; break;
        case 2: res = R12W; break;
        case 1: res = R12B; break;
        }
        break;
    case 3:
        switch (size) {
        case 8: res = R13; break;
        case 4: res = R13D; break;
        case 2: res = R13W; break;
        case 1: res = R13B; break;
        }
        break;
    case 4:
        switch (size) {
        case 8: res = R14; break;
        case 4: res = R14D; break;
        case 2: res = R14W; break;
        case 1: res = R14B; break;
        }
        break;
    case 5:
        switch (size) {
        case 8: res = R15; break;
        case 4: res = R15D; break;
        case 2: res = R15W; break;
        case 1: res = R15B; break;
        }
        break;
    }

    if (ctx->next_available_rax_reg_counter == 5)
        ctx->next_available_rax_reg_counter = 0;
    ++ctx->next_available_rax_reg_counter;
    return res;
}

//------------------------------------------------------------------------------
//                             instructions
//------------------------------------------------------------------------------

void emit_jmp(Codegen_Context* ctx, char* label) {
    xassert(ctx);
    xassert(label);
    emit(ctx, "jmp %s", label);
}

void emit_je(Codegen_Context* ctx, char* label) {
    xassert(ctx);
    xassert(label);
    emit(ctx, "je %s", label);
}

void emit_push(Codegen_Context* ctx, s8 reg) {
    xassert(ctx);
    xassert(reg >= 0 && reg <= _REGISTER_COUNT_);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "sub rsp, 8");
        emit(ctx, "movss [rsp], %s", r);
    } else {
        emit(ctx, "push %s", r);
    }
    ctx->stack_pos += 8;
}

void emit_pop(Codegen_Context* ctx, s8 reg) {
    xassert(ctx);
    xassert(reg >= 0 && reg <= _REGISTER_COUNT_);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "movss %s, [rsp]", r);
        emit(ctx, "add rsp, 8");
    } else {
        emit(ctx, "pop %s", r);
    }
    ctx->stack_pos -= 8;
    xassert(ctx->stack_pos >= 0);
}

bool is_reg8(s8 reg) {
    return (get_size_of_reg(reg) == 1) && (reg < XMM0);
}
bool is_reg16(s8 reg) {
    return (get_size_of_reg(reg) == 2) && (reg < XMM0);
}
bool is_reg32(s8 reg) {
    return (get_size_of_reg(reg) == 4) && (reg < XMM0);
}
bool is_reg64(s8 reg) {
    return (get_size_of_reg(reg) == 8) && (reg < XMM0);
}

void emit_lea_reg64_mem(Codegen_Context* ctx, s8 reg64, char* mem) {
    xassert(ctx);
    xassert(is_reg64(reg64));
    xassert(mem);
    char* r = get_reg(reg64);
    emit(ctx, "lea %s, %s", r, mem);
}

void* visitor_get_all_alloca_in_block(void* sum, AST* node) {
    xassert(sum);
    xassert(node);
    if (node->kind == AST_VARIABLE_DECL) {
        *((s64*)sum) += get_size_of_type(node->type);
    }
    return NULL;
}

// @Audit
s64 get_all_alloca_in_block(AST* block) {
    xassert(block);
    s64 sum = 0;
    ast_visit(visitor_get_all_alloca_in_block, &sum, block);
    return sum;
}

// From the System V Application Binary Interface Manual
// -- Classification
//  The size of each argument gets rounded up to eightbytes.
//  The basic types are assigned their natural classes:
//      - Arguments of types (signed and unsigned) _Bool, char, short, int, long,long long, and pointers are in the INTEGER class.
//      - Arguments of types float, double, _Decimal32, _Decimal64 and __m64 are in class SSE.
//      - Arguments of types __float128, _Decimal128 and __m128 are split into two halves. The least significant ones belong to class SSE, the most significant one to class SSEUP.
//      - Arguments of type __m256 are split into four eightbyte chunks. The least significant one belongs to class SSE and all the others to class SSEUP.
//      - Arguments of type __m512 are split into eight eightbyte chunks. The least significant one belongs to class SSE and all the others to class SSEUP.
//      - The 64-bit mantissa of arguments of type long double
//      - The 64-bit mantissa of arguments of type long double belongs to class X87, the 16-bit exponent plus 6 bytes of padding belongs to class X87UP.
//      - Arguments of type __int128 offer the same operations as INTEGERs, yet they do not fit into one general purpose register but require two registers.
//              For classification purposes __int128 is treated as if it were implemented as:  typedef struct { long low, high; } __int128;
//              with the exception that arguments of type __int128 that are stored in memory must be aligned on a 16-byte boundary.
//      - Arguments of complex T where T is one of the types float or double are treated as if they are implemented as:    struct complexT { T real; T imag; };
//      - A variable of type complex long double is classified as type COM- PLEX_X87.
//
//  The classification of aggregate (structures and arrays) and union types works as follows:
//      1. If the size of an object is larger than eight eightbytes, or it contains un- aligned fields, it has class MEMORY.
//
//      2. If a C++ object is non-trivial for the purpose of calls, as specified in the C++ ABI 13, it is passed by invisible reference (the object is replaced in the parameter list by a pointer that has class INTEGER).
//      3. If the size of the aggregate exceeds a single eightbyte, each is classified separately. Each eightbyte gets initialized to class NO_CLASS.
//      4. Each field of an object is classified recursively so that always two fields are considered. The resulting class is calculated according to the classes of the fields in the eightbyte:
//
//              (a) If both classes are equal, this is the resulting class.
//              (b) If one of the classes is NO_CLASS, the resulting class is the other class.
//              (c) If one of the classes is MEMORY, the result is the MEMORY class.
//              (d) If one of the classes is INTEGER, the result is the INTEGER.
//              (e) If one of the classes is X87, X87UP, COMPLEX_X87 class, MEM- ORY is used as class.
//              (f) Otherwise class SSE is used.
//
//      5. Then a post merger cleanup is done:
//
//              (a) If one of the classes is MEMORY, the whole argument is passed in memory.
//              (b) If X87UP is not preceded by X87, the whole argument is passed in memory.
//              (c) If the size of the aggregate exceeds two eightbytes and the first eight- byte isn’t SSE or any other eightbyte isn’t SSEUP, the whole argument is passed in memory.
//              (d) If SSEUP is not preceded by SSE or SSEUP, it is converted to SSE.
//
Class_Kind classify(Type* type) {
    xassert(type);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
        case TYPE_INT:      // fallthrough
        case TYPE_POINTER:  return CLASS_INTEGER;
        case TYPE_FLOAT:    return CLASS_SSE;
        case TYPE_STRUCT:   // fallthrough
        case TYPE_ARRAY:    // fallthrough
        case TYPE_UNION: {
            s64 size = get_size_of_type(type);
            bool is_aligned = type->flags & TYPE_FLAG_IS_ALIGNED;
            if (size > 8 || !is_aligned) {
                return CLASS_MEMORY;
            }
        }
    }
    return CLASS_NO_CLASS;
}

List* classify_arguments(List* arguments) {
    xassert(arguments);
    List* classified_argument_list = make_list();
    list_foreach(arguments) {
        AST* arg = it->data;
        xassert(arg);
        ClassifiedArgument* ca = xmalloc(sizeof(ClassifiedArgument));
        ca->class = classify(arg->type);
        ca->argument = arg;
        list_append(classified_argument_list, ca);
    }
    return classified_argument_list;
}

char* class_kind_to_str(Class_Kind kind) {
    TASSERT_KIND_IN_RANGE(CLASS, kind);
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("kind = %d", kind));
    case CLASS_INTEGER:     return "CLASS_INTEGER";
    case CLASS_SSE:         return "CLASS_SSE";
    case CLASS_SSEUP:       return "CLASS_SSEUP";
    case CLASS_X87:         return "CLASS_X87";
    case CLASS_X87UP:       return "CLASS_X87UP";
    case CLASS_COMPLEX_X87: return "CLASS_COMPLEX_X87";
    case CLASS_NO_CLASS:    return "CLASS_NO_CLASS";
    case CLASS_MEMORY:      return "CLASS_MEMORY";
    }
    UNREACHABLE;
    return NULL;
}
