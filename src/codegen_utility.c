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

#include "codegen_utility.h"

#include "ast.h" // AST*, ast_to_str
#include "constants.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "stack.h"  // make_stack
#include "string.h" // string
#include "typedefs.h"
#include "utility.h" // error warning info, wrap_with_colored_parens
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <stdlib.h>  // free
#include <string.h>  // strcmp

Codegen_Context
make_codegen_context() {
    Codegen_Context ctx;
    ctx.scope_stack                    = make_stack();
    ctx.current_function               = NULL;
    ctx.expected_type                  = NULL;
    ctx.section_extern                 = string_create("");
    ctx.section_text                   = string_create("");
    ctx.section_data                   = string_create("");
    ctx.stack_index                    = 0;
    ctx.text_label_counter             = 0;
    ctx.data_label_counter             = 0;
    ctx.ocontinue                      = NULL;
    ctx.lcontinue                      = NULL;
    ctx.obreak                         = NULL;
    ctx.lbreak                         = NULL;
    ctx.l_end                          = NULL;
    ctx.l0                             = NULL;
    ctx.l1                             = NULL;
    ctx.o0                             = NULL;
    ctx.o1                             = NULL;
    ctx.next_available_xmm_reg_counter = 0;
    ctx.next_available_rax_reg_counter = 0;
    return ctx;
}

void emit_no_tab(Codegen_Context* ctx, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    string_append_f(ctx->section_text, "%s\n", str);

    free(str);
}

void emit_extern(Codegen_Context* ctx, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    string_append_f(ctx->section_extern, "extern _%s\n", str);

    free(str);
}

void emit_data(Codegen_Context* ctx, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    string_append_f(ctx->section_data, "\t%s\n", str);

    free(str);
}

void emit(Codegen_Context* ctx, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    bool is_label = false;

    for (int i = 0; i < str_len; ++i) {
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
    assert(type);
    switch (type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_ARRAY:   // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_INT: push(ctx, RAX); break;
    case TYPE_FLOAT: push(ctx, XMM0); break;
    }
}

void pop_type_2(Codegen_Context* ctx, Type* type) {
    assert(type);
    switch (type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_ARRAY:   // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_INT: pop(ctx, RCX); break;
    case TYPE_FLOAT: pop(ctx, XMM1); break;
    }
}
void pop_type(Codegen_Context* ctx, Type* type) {
    assert(type);
    switch (type->kind) {
    case TYPE_ARRAY:   // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_INT: pop(ctx, RAX); break;
    case TYPE_FLOAT: pop(ctx, XMM0); break;
    default: error("Unhandled pop_type %s", type_to_str(NULL, type));
    }
}

void push(Codegen_Context* ctx, int reg) {
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "sub rsp, 8");
        emit(ctx, "movsd [rsp], %s", r);
    } else {
        emit(ctx, "push %s", r);
    }
    ctx->stack_index += 8;
}

void pop(Codegen_Context* ctx, int reg) {
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "movsd %s, [rsp]", r);
        emit(ctx, "add rsp, 8");
    } else {
        emit(ctx, "pop %s", r);
    }
    ctx->stack_index -= 8;
    assert(ctx->stack_index >= 0);
}

char* get_op_size(s8 bytes) {
    assert(bytes >= 0);
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
    assert(type);
    s64 bytes = get_size_of_type(type);
    switch (type->kind) {
    default:
        error("get_result_reg unhandled case: %s",
              type_kind_to_str(type->kind));
    case TYPE_FLOAT:
        switch (bytes) {
        case 4: return "xmm1";
        case 8: return "xmm1";
        }
    case TYPE_POINTER: // fallthrough
    case TYPE_ARRAY:   // return get_reg(RCX);
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_VOID:    // fallthrough
    case TYPE_INT: return get_reg(get_rax_reg_of_byte_size(bytes, 'c'));
    }
    UNREACHABLE;
    return NULL;
}

char* get_result_reg(Type* type) {
    assert(type);
    s64 bytes = get_size_of_type(type);
    switch (type->kind) {
    case TYPE_FLOAT:
        switch (bytes) {
        case 4: return "xmm0";
        case 8: return "xmm0";
        }
    case TYPE_POINTER: // fallthrough
    case TYPE_ARRAY:   // return get_reg(RCX);
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_VOID:    // fallthrough
    case TYPE_INT: return get_reg(get_rax_reg_of_byte_size(bytes, 'a'));
    default:
        error("get_result_reg unhandled case: %s",
              type_kind_to_str(type->kind));
    }

    UNREACHABLE;
    return NULL;
}

char* get_db_op(Type* type) {
    assert(type);
    s64 bytes = get_size_of_type(type);
    switch (bytes) {
    case 1: return "db";
    case 2: return "dw";
    case 4: return "dd";
    case 8: return "dq";
    }
    UNREACHABLE;
    return NULL;
}
char* get_move_op(Type* type) {
    assert(type);
    s64 bytes = get_size_of_type(type);
    switch (type->kind) {
    default:
        error("get_move_op unhandled case: %s", type_kind_to_str(type->kind));
    case TYPE_FLOAT:
        switch (bytes) {
        case 4: return "movss";
        case 8: return "movsd";
        }
    case TYPE_POINTER: // fallthrough
    case TYPE_ARRAY:   // return "lea";
    case TYPE_STRUCT:  // fallthrough
    case TYPE_ENUM:    // fallthrough
    case TYPE_INT: return "mov";
    }
    UNREACHABLE;
    return NULL;
}

void alloc_variable(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    info("Allocating variable '%s', type '%s', size '%lld' ",
         variable->Variable.name,
         get_type_name(variable->type),
         size);
    ctx->stack_index += size;
}

void dealloc_variable(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    info("Deallocating variable '%s', type '%s', size '%lld' ",
         variable->Variable.name,
         get_type_name(variable->type),
         size);
    ctx->stack_index -= size;
    assert(ctx->stack_index >= 0);
}

void push_scope(Codegen_Context* ctx) {
    Scope* new_scope = make_scope();
    stack_push(ctx->scope_stack, new_scope);
}

void pop_scope(Codegen_Context* ctx) {
    Scope* scope = (Scope*)stack_pop(ctx->scope_stack);
    LIST_FOREACH(scope->local_variables) {
        Value* v = (Value*)it->data;
        dealloc_variable(ctx, v);
    }
}

Value*
get_variable_in_scope(Scope* scope, char* name) {
    assert(scope);
    assert(name);
    LIST_FOREACH(scope->local_variables) {
        Value* v = (Value*)it->data;
        if (v->Variable.name == name) return v;
    }
    return NULL;
}

Value*
get_variable(Codegen_Context* ctx, AST* ident) {
    assert(ident);
    STACK_FOREACH(ctx->scope_stack) {
        Scope* scope = (Scope*)it->data;
        Value* res   = get_variable_in_scope(scope, ident->Ident.name);
        if (res) return res;
    }
    error("no variable with name '%s'", ident->Ident.name);
    return NULL;
}

void add_variable(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    alloc_variable(ctx, variable);
    info("Adding variable: '%s' to scope", variable->Variable.name);
    Scope* top = (Scope*)stack_peek(ctx->scope_stack);
    list_append(top->local_variables, variable);
}

int align(int n, s32 m) {
    return (m - (n % m)) % m;
}

void emit_cast_float_to_int(Codegen_Context* ctx, char* reg, Type* type) {
    assert(type->kind == TYPE_INT);
    bool usig      = type->Int.is_unsigned;
    s8   type_size = get_size_of_type(type);
    switch (type_size) {
    case 4: emit(ctx, "cvttss2si %s, xmm0", reg); break;
    case 8: emit(ctx, "cvttsd2si %s, xmm0", reg); break;
    }
    if (usig) {
        emit_cast_int_to_int(ctx, reg, make_type_int(type_size, usig));
    }
}

void emit_cast_int_to_int(Codegen_Context* ctx, char* reg, Type* type) {
    assert(type->kind == TYPE_INT);
    bool usig      = type->Int.is_unsigned;
    s8   type_size = get_size_of_type(type);
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
        usig ? emit(ctx, "mov %s, %s", reg, reg) : emit(ctx, "cltq");
        break;
        // case 8: // fallthrough
    }
}

void emit_cast(Codegen_Context* ctx, Value* variable, Type* desired_type) {
    assert(variable);
    assert(desired_type);

    char* reg = get_result_reg(variable->type);

    switch (variable->type->kind) {
    default: error("unexpected type case in emit_cast");
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

void emit_store_r(Codegen_Context* ctx, Value* variable, s64 reg) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    char* reg_c     = get_reg(reg);
    char* mov_op    = get_move_op(variable->type);

    switch(variable->type->kind)
    {
        // case TYPE_POINTER:
        // case TYPE_STRUCT:
        //     emit(ctx, "%s [rax], %s; store_r %s", mov_op, reg_c, (variable->Variable.name));
        //     break;
        default: 
            emit(ctx, "%s [rbp-%lld], %s; store_r %s at %lld", mov_op, stack_pos, reg_c, (variable->Variable.name), stack_pos);
            break;
    }
}


void emit_store_deref(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    char* reg       = get_result_reg_2(variable->type);
    char* mov_op    = get_move_op(variable->type);
    emit(ctx, "%s [rax], %s; store %s", mov_op, reg, (variable->Variable.name));
} 

void emit_store(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    char* reg       = get_result_reg_2(variable->type);
    char* mov_op    = get_move_op(variable->type);
    switch(variable->type->kind) {
    case TYPE_POINTER:
    case TYPE_STRUCT:
        emit(ctx, "%s [rax], %s; store %s of type '%s'", mov_op, reg, (variable->Variable.name), get_type_name(variable->type));
        break;
    default:
        emit(ctx, "%s [rbp-%lld], %s; store %s of type '%s' at %lld", mov_op, stack_pos, reg, (variable->Variable.name), get_type_name(variable->type), stack_pos);
        break;
    }
} 

void emit_load(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    char* reg       = get_result_reg(variable->type);
    char* mov_op    = get_move_op(variable->type);
    emit(ctx, "%s %s, [rbp-%lld]; load %s of type '%s' from %lld", mov_op, reg, stack_pos, variable->Variable.name, get_type_name(variable->type), stack_pos);
}

void set_break_label(Codegen_Context* ctx, char* break_l) {
    ctx->obreak = ctx->lbreak;
    ctx->lbreak = break_l;
}
void restore_break_label(Codegen_Context* ctx) {
    ctx->lbreak = ctx->obreak;
}

void set_continue_label(Codegen_Context* ctx, char* continue_l) {
    ctx->ocontinue = ctx->lcontinue;
    ctx->lcontinue = continue_l;
}
void restore_continue_label(Codegen_Context* ctx) {
    ctx->lcontinue = ctx->ocontinue;
}

void set_jump_labels(Codegen_Context* ctx, char* continue_l, char* break_l) {
    set_continue_label(ctx, continue_l);
    set_break_label(ctx, break_l);
}

void restore_jump_labels(Codegen_Context* ctx) {
    restore_continue_label(ctx);
    restore_break_label(ctx);
}

void set_temp_labels(Codegen_Context* ctx, char* l0, char* l1) {
    ctx->o0 = ctx->l0;
    ctx->o1 = ctx->l1;
    ctx->l0 = l0;
    ctx->l1 = l1;
}
void restore_temp_labels(Codegen_Context* ctx) {
    ctx->l0 = ctx->o0;
    ctx->l1 = ctx->o1;
}
char* make_text_label(Codegen_Context* ctx) {
    char* l = strf(".l%d", ctx->text_label_counter);
    ctx->text_label_counter += 1;
    return l;
}

char* make_data_label(Codegen_Context* ctx) {
    char* l = strf("d%d", ctx->data_label_counter);
    ctx->data_label_counter += 1;
    return l;
}

void reset_text_label_counter(Codegen_Context* ctx) {
    ctx->text_label_counter = 0;
}
void reset_stack(Codegen_Context* ctx) {
    ctx->stack_index = 0;
}
void set_current_function_expr(Codegen_Context* ctx, AST* func_expr) {
    ctx->current_function = func_expr;
}

char* emit_save_result(Codegen_Context* ctx, Value* value) {
    char* mov_op     = get_move_op(value->type);
    char* reg        = get_next_available_reg_fitting(ctx, value->type);
    char* result_reg = get_result_reg(value->type);
    emit(ctx, "%s %s, %s", mov_op, reg, result_reg);
    return reg;
}

char* get_instr(Token_Kind op, Type* type) {
    char* inst = NULL;
    switch (type->kind) {
    default: error("unhandled case: %s, %s", __func__, __LINE__);
    case TYPE_INT: {
        bool usig = type->Int.is_unsigned;
        switch (op) {
        default: error("unhandled case: %s, %s", __func__, __LINE__);
        case TOKEN_PLUS: inst = "add"; break;
        case TOKEN_MINUS: inst = "sub"; break;
        case TOKEN_ASTERISK: inst = "imul"; break;
        case TOKEN_FWSLASH: inst = (usig ? "div" : "idiv"); break;
        }
    } break;
    case TYPE_FLOAT: {
        s64 size = get_size_of_type(type);
        switch (op) {
        default: error("unhandled case: %s, %s", __func__, __LINE__);
        case TOKEN_PLUS: inst = (size == 8 ? "addsd" : "addss"); break;
        case TOKEN_MINUS: inst = (size == 8 ? "subsd" : "subss"); break;
        case TOKEN_ASTERISK: inst = (size == 8 ? "mulsd" : "mulss"); break;
        case TOKEN_FWSLASH: inst = (size == 8 ? "divsd" : "divss"); break;
        }
    } break;
    }
    return inst;
}

char* get_next_available_reg_fitting(Codegen_Context* ctx, Type* type) {
    s64 size = get_size_of_type(type);
    s8  r    = -1;
    switch (type->kind) {
    case TYPE_ARRAY:   // fallthrough
    case TYPE_POINTER: // fallthrough
    case TYPE_INT: r = get_next_available_rax_reg_fitting(ctx, size); break;
    case TYPE_FLOAT: r = get_next_available_xmm_reg_fitting(ctx); break;
    default: error("Unhandled get_next_available_reg_fitting");
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
    assert(ctx);
    assert(label);
    emit(ctx, "jmp %s", label);
}

void emit_je(Codegen_Context* ctx, char* label) {
    assert(ctx);
    assert(label);
    emit(ctx, "je %s", label);
}

void emit_push(Codegen_Context* ctx, s8 reg) {
    assert(ctx);
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "sub rsp, 8");
        emit(ctx, "movss [rsp], %s", r);
    } else {
        emit(ctx, "push %s", r);
    }
    ctx->stack_index += 8;
}

void emit_pop(Codegen_Context* ctx, s8 reg) {
    assert(ctx);
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "movss %s, [rsp]", r);
        emit(ctx, "add rsp, 8");

    } else {
        emit(ctx, "pop %s", r);
    }
    ctx->stack_index -= 8;
    assert(ctx->stack_index >= 0);
}

bool is_reg8(s8 reg) {
    return (get_size_of_reg(reg) == 1) && (reg < XMM_REG_START);
}
bool is_reg16(s8 reg) {
    return (get_size_of_reg(reg) == 2) && (reg < XMM_REG_START);
}
bool is_reg32(s8 reg) {
    return (get_size_of_reg(reg) == 4) && (reg < XMM_REG_START);
}
bool is_reg64(s8 reg) {
    return (get_size_of_reg(reg) == 8) && (reg < XMM_REG_START);
}

void emit_lea_reg64_mem(Codegen_Context* ctx, s8 reg64, char* mem) {
    assert(ctx);
    assert(is_reg64(reg64));
    assert(mem);
    char* r = get_reg(reg64);
    emit(ctx, "lea %s, %s", r, mem);
}

void visitor_get_all_alloca_in_block(void* sum, AST* node) {
    if (node->kind == AST_VARIABLE_DECL) {
        *((s64*)sum) += get_size_of_type(node->type);
    }
}

s64 get_all_alloca_in_block(AST* block) {
    s64 sum = 0;
    ast_visit(visitor_get_all_alloca_in_block, &sum, block);
    return sum;
}
