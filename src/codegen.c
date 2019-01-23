#include "codegen.h"
#include "ast.h" // Expr, expr_to_str
#include "globals.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "string.h" // string
#include "typedefs.h"
#include "utility.h" // error warning info, wrap_with_colored_parens
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <stdlib.h>  // free

//------------------------------------------------------------------------------
//                              codegen.c
//------------------------------------------------------------------------------

#define DEBUG_START info("%s: %s", __func__, wrap_with_colored_parens(expr_to_str(expr)));

typedef struct {
    List*  ast;
    List*  foreign_function_list;
    Expr*  current_function;
    Stack* scope_stack;
    s64    stack_index;
    string section_text;
    string section_data;
    char*  temp_label0;
    char*  temp_label1;
    s64    text_label_counter;
    s64    data_label_counter;
    char*  ocontinue;
    char*  lcontinue;
    char*  obreak;
    char*  lbreak;
    char*  l_end;

    s8 next_available_xmm_reg_counter;
    s8 next_available_rax_reg_counter;
} Codegen_Context;

//------------------------------------------------------------------------------
//                               Codegen Functions
//------------------------------------------------------------------------------

Codegen_Context make_codegen_context();
char*           compile(Codegen_Context* ctx);
void            set_jump_labels(Codegen_Context* ctx, char* continue_l, char* break_l);
void            restore_jump_labels(Codegen_Context* ctx);
char*           make_text_label(Codegen_Context* ctx);
char*           make_data_label(Codegen_Context* ctx);
void            reset_text_label_counter(Codegen_Context* ctx);
void            reset_stack(Codegen_Context* ctx);
void            set_current_function_expr(Codegen_Context* ctx, Expr* func_expr);

void   emit_no_tab(Codegen_Context* ctx, char* fmt, ...);
void   emit_data(Codegen_Context* ctx, char* fmt, ...);
void   emit(Codegen_Context* ctx, char* fmt, ...);
void   push(Codegen_Context* ctx, int reg);
void   pop(Codegen_Context* ctx, int reg);
void   push_type(Codegen_Context* ctx, Typespec* type);
void   pop_type(Codegen_Context* ctx, Typespec* type);
void   push_scope(Codegen_Context* ctx);
void   pop_scope(Codegen_Context* ctx);
char*  get_result_reg(Typespec* type);
char*  get_op_size(s8 bytes);
char*  get_db_op(Typespec* type);
char*  get_move_op(Typespec* type);
void   alloc_variable(Codegen_Context* ctx, Value* variable);
void   dealloc_variable(Codegen_Context* ctx, Value* variable);
Value* get_variable_in_scope(Scope* scope, char* name);
Value* get_variable(Codegen_Context* ctx, char* name);
void   add_variable(Codegen_Context* ctx, Value* variable);
int    align(int n, s32 m);
char*  get_instr(Token_Kind op, Typespec* type);

char* emit_save_result(Codegen_Context* ctx, Value* value);
char* get_next_available_reg_fitting(Codegen_Context* ctx, Typespec* type);
s8    get_next_available_xmm_reg_fitting(Codegen_Context* ctx);
s8    get_next_available_rax_reg_fitting(Codegen_Context* ctx, s64 size);

s64 get_all_alloca_in_block(Expr* block);

void   emit_cast(Codegen_Context* ctx, Value* variable, Typespec* desired_type);
void   emit_cast_int_to_int(Codegen_Context* ctx, char* reg, Typespec* type);
void   emit_cast_float_to_int(Codegen_Context* ctx, char* reg, Typespec* type);
void   emit_store_r(Codegen_Context* ctx, Value* variable, s64 reg);
void   emit_store(Codegen_Context* ctx, Value* variable);
void   emit_load(Codegen_Context* ctx, Value* variable);
Value* codegen_unary(Codegen_Context* ctx, Expr* expr);
Value* codegen_binary(Codegen_Context* ctx, Expr* expr);
Value* codegen_variable_decl(Codegen_Context* ctx, Expr* expr);
Value* codegen_call(Codegen_Context* ctx, Expr* expr);
Value* codegen_float(Codegen_Context* ctx, Expr* expr);
Value* codegen_int(Codegen_Context* ctx, Expr* expr);
Value* codegen_block(Codegen_Context* ctx, Expr* expr);
Value* codegen_macro(Codegen_Context* ctx, Expr* expr);
Value* codegen_ident(Codegen_Context* ctx, Expr* expr);
Value* codegen_subscript(Codegen_Context* ctx, Expr* expr);
Value* codegen_string(Codegen_Context* ctx, Expr* expr);
Value* codegen_note(Codegen_Context* ctx, Expr* expr);
Value* codegen_if(Codegen_Context* ctx, Expr* expr);
Value* codegen_for(Codegen_Context* ctx, Expr* expr);
Value* codegen_while(Codegen_Context* ctx, Expr* expr);
Value* codegen_defer(Codegen_Context* ctx, Expr* expr);
Value* codegen_return(Codegen_Context* ctx, Expr* expr);
Value* codegen_break(Codegen_Context* ctx, Expr* expr);
Value* codegen_continue(Codegen_Context* ctx, Expr* expr);
Value* codegen_struct(Codegen_Context* ctx, Expr* expr);
Value* codegen_function(Codegen_Context* ctx, Expr* expr);
Value* codegen_cast(Codegen_Context* ctx, Expr* expr);
Value* codegen_expr(Codegen_Context* ctx, Expr* expr);

char* generate_X64_from_ast(List* ast);

// @Hotpath
Value* codegen_expr(Codegen_Context* ctx, Expr* expr) {
    switch (expr->kind) {
    case EXPR_ASM: emit(ctx, "%s", expr->Asm.str); return NULL;
    case EXPR_MACRO: return codegen_macro(ctx, expr);
    case EXPR_STRUCT: return codegen_struct(ctx, expr);
    case EXPR_FUNCTION: return codegen_function(ctx, expr);
    case EXPR_NOTE: return codegen_note(ctx, expr);
    case EXPR_INT: return codegen_int(ctx, expr);
    case EXPR_FLOAT: return codegen_float(ctx, expr);
    case EXPR_STRING: return codegen_string(ctx, expr);
    case EXPR_IDENT: return codegen_ident(ctx, expr);
    case EXPR_CALL: return codegen_call(ctx, expr);
    case EXPR_UNARY: return codegen_unary(ctx, expr);
    case EXPR_BINARY: return codegen_binary(ctx, expr);
    case EXPR_VARIABLE_DECL: return codegen_variable_decl(ctx, expr);
    case EXPR_BLOCK: return codegen_block(ctx, expr);
    case EXPR_GROUPING: return codegen_expr(ctx, expr->Grouping.expr);
    case EXPR_SUBSCRIPT: return codegen_subscript(ctx, expr);
    case EXPR_IF: return codegen_if(ctx, expr);
    case EXPR_FOR: return codegen_for(ctx, expr);
    case EXPR_WHILE: return codegen_while(ctx, expr);
    case EXPR_RETURN: return codegen_return(ctx, expr);
    case EXPR_DEFER: return codegen_defer(ctx, expr);
    case EXPR_BREAK: return codegen_break(ctx, expr);
    case EXPR_CONTINUE: return codegen_continue(ctx, expr);
    case EXPR_CAST: return codegen_cast(ctx, expr);
    default: error("Unhandled codegen_expr case for kind '%s'", expr_kind_to_str(expr->kind));
    }
    return NULL;
}

char* generate_X64_from_ast(List* ast) {
    info("Generating X64 Assembly from AST");

    Codegen_Context ctx       = make_codegen_context();
    ctx.ast                   = ast;
    ctx.foreign_function_list = get_foreign_function_list();

    char* output = compile(&ctx);
    info("%s", output);

    return output;
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

    append_string_f(&ctx->section_text, "%s\n", str);

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

    append_string_f(&ctx->section_data, "\t%s\n", str);

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
        append_string_f(&ctx->section_text, "%s\n", str);
    } else {
        append_string_f(&ctx->section_text, "\t%s\n", str);
    }

    free(str);
}

void push_type(Codegen_Context* ctx, Typespec* type) {
    assert(type);
    switch (type->kind) {
    case TYPESPEC_ARRAY:   // fallthrough
    case TYPESPEC_POINTER: // fallthrough
    case TYPESPEC_INT: push(ctx, RAX); break;
    case TYPESPEC_FLOAT: push(ctx, XMM0); break;
    default: error("Unhandled push_type %s", typespec_to_str(type));
    }
}

void pop_type(Codegen_Context* ctx, Typespec* type) {
    assert(type);
    switch (type->kind) {
    case TYPESPEC_ARRAY:   // fallthrough
    case TYPESPEC_POINTER: // fallthrough
    case TYPESPEC_INT: pop(ctx, RAX); break;
    case TYPESPEC_FLOAT: pop(ctx, XMM0); break;
    default: error("Unhandled pop_type %s", typespec_to_str(type));
    }
}

void push(Codegen_Context* ctx, int reg) {
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "SUB RSP, 8");
        emit(ctx, "MOVSS [RSP], %s", r);
    } else {
        emit(ctx, "PUSH %s", r);
    }
    ctx->stack_index += 8;
}

void pop(Codegen_Context* ctx, int reg) {
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    char* r = get_reg(reg);
    if (reg >= XMM0 && reg <= XMM7) {
        emit(ctx, "MOVSS %s, DWORD [RSP]", r);
        emit(ctx, "ADD RSP, 8");
    } else {
        emit(ctx, "POP %s", r);
    }
    ctx->stack_index -= 8;
    assert(ctx->stack_index >= 0);
}

char* get_op_size(s8 bytes) {
    assert(bytes >= 0);
    switch (bytes) {
    case 1: return "BYTE";
    case 2: return "WORD";
    case 4: return "DWORD";
    case 8:
        return "QWORD";
        // default: error("get_op_size unknown byte size: %d", bytes);
    }
    return "QWORD";
    ;
}

char* get_result_reg(Typespec* type) {
    assert(type);
    s64 bytes = get_size_of_typespec(type);
    switch (type->kind) {
    case TYPESPEC_FLOAT:
        switch (bytes) {
        case 4: return "XMM0";
        case 8: return "XMM0";
        }
    case TYPESPEC_POINTER: // fallthrough
    case TYPESPEC_STRUCT:  // fallthrough
    case TYPESPEC_ARRAY:   // fallthrough
    case TYPESPEC_INT: return get_reg(get_rax_reg_of_byte_size(bytes));
    default: error("get_move_op unhandled case: %s", typespec_kind_to_str(type->kind));
    }
    return NULL;
}

char* get_db_op(Typespec* type) {
    assert(type);
    s64 bytes = get_size_of_typespec(type);
    switch (bytes) {
    case 1: return "DB";
    case 2: return "DW";
    case 4: return "DD";
    case 8: return "DQ";
    }
    return NULL;
}
char* get_move_op(Typespec* type) {
    assert(type);
    s64 bytes = get_size_of_typespec(type);
    switch (type->kind) {
    case TYPESPEC_FLOAT:
        switch (bytes) {
        case 4: return "MOVSS";
        case 8: return "MOVSD";
        }
    case TYPESPEC_POINTER: // fallthrough
    case TYPESPEC_STRUCT:  // fallthrough
    case TYPESPEC_ARRAY:   // fallthrough
    case TYPESPEC_INT: return "MOV";
    default: error("get_move_op unhandled case: %s", typespec_kind_to_str(type->kind));
    }
    return NULL;
}

void alloc_variable(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    info("Allocating variable '%s', type '%s', size '%lld' ", variable->Variable.name, typespec_to_str(variable->type),
         size);
    ctx->stack_index += size;
}

void dealloc_variable(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    info("Deallocating variable '%s', type '%s', size '%lld' ", variable->Variable.name,
         typespec_to_str(variable->type), size);
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

Value* get_variable_in_scope(Scope* scope, char* name) {
    assert(scope);
    assert(name);
    LIST_FOREACH(scope->local_variables) {
        Value* v = (Value*)it->data;
        if (v->Variable.name == name) return v;
    }
    return NULL;
}

Value* get_variable(Codegen_Context* ctx, char* name) {
    assert(name);
    STACK_FOREACH(ctx->scope_stack) {
        Scope* scope = (Scope*)it->data;
        Value* res   = get_variable_in_scope(scope, name);
        if (res) return res;
    }
    error("no variable with name '%s'", name);
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

int align(int n, s32 m) { return (m - (n % m)) % m; }

void emit_cast_float_to_int(Codegen_Context* ctx, char* reg, Typespec* type) {
    assert(type->kind == TYPESPEC_INT);
    bool usig      = type->Int.is_unsigned;
    s8   type_size = get_size_of_typespec(type);
    switch (type_size) {
    case 4: emit(ctx, "CVTTSS2SI %s, XMM0", reg); break;
    case 8: emit(ctx, "CVTTSD2SI %s, XMM0", reg); break;
    }
    if (usig) {
        emit_cast_int_to_int(ctx, reg, make_typespec_int(type_size, usig));
    }
}

void emit_cast_int_to_int(Codegen_Context* ctx, char* reg, Typespec* type) {
    assert(type->kind == TYPESPEC_INT);
    bool usig      = type->Int.is_unsigned;
    s8   type_size = get_size_of_typespec(type);
    switch (type_size) {
    case 1: usig ? emit(ctx, "MOVZBQ %s, al", reg) : emit(ctx, "MOVSBQ %s, al", reg); break;
    case 2: usig ? emit(ctx, "MOVZWQ %s, ax", reg) : emit(ctx, "MOVSWQ %s, ax", reg); break;
    case 4:
        usig ? emit(ctx, "MOV %s, %s", reg, reg) : emit(ctx, "CLTQ");
        break;
        // case 8: // fallthrough
    }
}

void emit_cast(Codegen_Context* ctx, Value* variable, Typespec* desired_type) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    assert(desired_type);

    s64   variable_size = get_size_of_value(variable);
    char* reg           = get_reg(get_rax_reg_of_byte_size(variable_size));

    switch (variable->type->kind) {
    case TYPESPEC_INT: {
        switch (desired_type->kind) {
        case TYPESPEC_INT: emit_cast_int_to_int(ctx, reg, desired_type); break;
        }
    } break;
    case TYPESPEC_FLOAT: {
        switch (desired_type->kind) {
        case TYPESPEC_INT: emit_cast_float_to_int(ctx, reg, desired_type); break;
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
    emit(ctx, "%s [RBP-%lld], %s; store", mov_op, stack_pos, reg_c);
}
void emit_store(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    char* reg       = get_result_reg(variable->type);
    char* mov_op    = get_move_op(variable->type);
    emit(ctx, "%s [RBP-%lld], %s; store", mov_op, stack_pos, reg);
}

void emit_load(Codegen_Context* ctx, Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    s64   size      = get_size_of_value(variable);
    char* mov_size  = get_op_size(size);
    char* reg       = get_result_reg(variable->type);
    char* mov_op    = get_move_op(variable->type);
    emit(ctx, "%s %s, %s [RBP-%lld]; load", mov_op, reg, mov_size, stack_pos);
}

Value* codegen_unary(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    Token_Kind op      = expr->Unary.op;
    Expr*      operand = expr->Unary.operand;

    Value* operand_val  = codegen_expr(ctx, operand);
    s64    operand_size = get_size_of_value(operand_val);

    char*  reg    = get_result_reg(operand_val->type);
    Value* result = operand_val;

    switch (op) {
    case THI_SYNTAX_ADDRESS: {
        // Operand must be an identifier
        s64 stack_pos = get_stack_pos_of_variable(operand_val);
        emit(ctx, "LEA RAX, [RSP-%lld]; addrsof", stack_pos);
    } break;
    case THI_SYNTAX_POINTER: {
        Typespec* t = operand_val->type->Array.type;
        switch (t->kind) {
        case TYPESPEC_ARRAY:
        case TYPESPEC_POINTER: emit(ctx, "LEA RAX, [RAX]; deref"); break;
        default: emit(ctx, "MOV RAX, [RAX]; deref"); break;
        }
    } break;
    case TOKEN_BANG: {
        emit(ctx, "CMP %s, 0", reg);
        emit(ctx, "SETE AL");
    } break;
    case TOKEN_PLUS: { // no nothing
    } break;
    case TOKEN_TILDE: {
        emit(ctx, "NOT AL");
    } break;
    case TOKEN_MINUS: {
        switch (operand_val->type->kind) {
        case TYPESPEC_INT: emit(ctx, "NEG %s", reg); break;
        case TYPESPEC_FLOAT: {
            emit(ctx, "MOVD ECX, XMM0");
            emit(ctx, "XOR ECX, 2147483648");
            emit(ctx, "MOVD XMM0, ECX");
        } break;
        }
        break;
    }
    default: error("unhandled unary case: %c", token_kind_to_str(op)); break;
    }
    return result;
}

Value* codegen_binary(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;

    Token_Kind op  = expr->Binary.op;
    Expr*      lhs = expr->Binary.lhs;
    Expr*      rhs = expr->Binary.rhs;

    switch (op) {
    // Field access
    case TOKEN_DOT: {
        error("FIELD ACCESS NOT IMPLEMENTED");
        Value* variable = get_variable(ctx, lhs->Ident.name);

        assert(variable->kind == VALUE_VARIABLE);
        assert(variable->type->kind == TYPESPEC_STRUCT);

        s64 offset = get_offset_in_struct_to_field(variable->type, rhs->Ident.name);
        // if (offset == 0) { emit_load(variable); return variable; }

        lhs = make_expr_unary(THI_SYNTAX_ADDRESS, lhs);
        lhs = make_expr_binary(TOKEN_PLUS, lhs, make_expr_int(offset));
        lhs = make_expr_unary(THI_SYNTAX_POINTER, lhs);

        return codegen_expr(ctx, lhs);
    }
    case THI_SYNTAX_ASSIGNMENT: {
        Value* rhs_v = codegen_expr(ctx, rhs);
        push_type(ctx, rhs_v->type);
        Value* variable = codegen_expr(ctx, lhs);
        pop_type(ctx, rhs_v->type);
        emit_store(ctx, variable);
        return variable;
    }
    case TOKEN_PLUS: {
        Value* rhs_v = codegen_expr(ctx, rhs);
        if (rhs_v->type->kind == TYPESPEC_FLOAT) {
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, XMM0);
            Value* lhs_v = codegen_expr(ctx, lhs);
            pop(ctx, XMM1);
            emit(ctx, "%s XMM0, XMM1", inst);
            return lhs_v;
        } else {
            push(ctx, RAX);
            Value* lhs_v = codegen_expr(ctx, lhs);
            pop(ctx, RCX);
            emit(ctx, "ADD RAX, RCX");
            return lhs_v;
        }
    }
    case TOKEN_MINUS: {
        Value* rhs_v = codegen_expr(ctx, rhs);
        if (rhs_v->type->kind == TYPESPEC_FLOAT) {
            Value* rhs_v = codegen_expr(ctx, rhs);
            char*  inst  = get_instr(op, rhs_v->type);
            push(ctx, XMM0);
            Value* lhs_v = codegen_expr(ctx, lhs);
            pop(ctx, XMM1);
            emit(ctx, "%s XMM0, XMM1", inst);
            return lhs_v;
        } else {
            push(ctx, RAX);
            Value* lhs_v = codegen_expr(ctx, lhs);
            pop(ctx, RCX);
            emit(ctx, "SUB RAX, RCX");
            return lhs_v;
        }
    }
    case TOKEN_ASTERISK: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        if (lhs_v->type->kind == TYPESPEC_FLOAT) {
            char* inst = get_instr(op, lhs_v->type);
            push(ctx, XMM0);
            codegen_expr(ctx, rhs);
            pop(ctx, XMM1);
            emit(ctx, "%s XMM0, XMM1", inst);
            return lhs_v;
        } else {
            push(ctx, RAX);
            codegen_expr(ctx, rhs);
            pop(ctx, RCX);
            emit(ctx, "IMUL RAX, RCX");
            return lhs_v;
        }
    }
    case TOKEN_FWSLASH: {
        Value* rhs_v = codegen_expr(ctx, rhs);
        if (rhs_v->type->kind == TYPESPEC_FLOAT) {
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, XMM0);
            codegen_expr(ctx, lhs);
            pop(ctx, XMM1);
            emit(ctx, "%s XMM1", inst);
            return rhs_v;
        } else {
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, RAX);
            codegen_expr(ctx, lhs);
            pop(ctx, RCX);
            emit(ctx, "CDQ");
            emit(ctx, "%s RCX", inst);
            return rhs_v;
        }
    }

    case TOKEN_PERCENT: {
        expr            = make_expr_binary(TOKEN_FWSLASH, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        emit(ctx, "MOV RAX, RDX");
        return variable;
    }
    case TOKEN_PIPE: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        push(ctx, RAX);
        codegen_expr(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "OR AL, CL");
        return lhs_v;
    }
    case TOKEN_AND: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        push(ctx, RAX);
        codegen_expr(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "AND AL, CL");
        return lhs_v;
    }
    case TOKEN_HAT: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        push(ctx, RAX);
        codegen_expr(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "XOR AL, CL");
        return lhs_v;
    }
    case TOKEN_LT_LT: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        assert(rhs->kind == EXPR_INT);
        emit(ctx, "SHL AL, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_GT_GT: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        assert(rhs->kind == EXPR_INT);
        emit(ctx, "SHR AL, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_AND_AND: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        push(ctx, RAX);
        codegen_expr(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "CMP RAX, 0");
        emit(ctx, "SETNE AL");
        emit(ctx, "CMP RCX, 0");
        emit(ctx, "SETNE CL");
        emit(ctx, "AND AL, CL");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        expr     = make_expr_binary(TOKEN_PIPE, lhs, rhs);
        Value* v = codegen_expr(ctx, expr);
        emit(ctx, "SETNE AL");
        return v;
    }

    case TOKEN_LT:    // FALLTHROUGH
    case TOKEN_GT:    // FALLTHROUGH
    case TOKEN_LT_EQ: // FALLTHROUGH
    case TOKEN_GT_EQ: // FALLTHROUGH
    case TOKEN_EQ_EQ: // FALLTHROUGH
    case TOKEN_BANG_EQ: {
        Value* lhs_v = codegen_expr(ctx, lhs);
        if (lhs_v->type->kind == TYPESPEC_FLOAT) {
            push(ctx, XMM0);
            codegen_expr(ctx, rhs);
            pop(ctx, XMM1);
            if (lhs_v->type->Float.bytes == 4) {
                emit(ctx, "UCOMISS XMM1, XMM0");
            } else {
                emit(ctx, "UCOMISD XMM1, XMM0");
            }
        } else {
            push(ctx, RAX);
            codegen_expr(ctx, rhs);
            pop(ctx, RCX);
            emit(ctx, "CMP RCX, RAX");
        }
        switch (op) {
        case TOKEN_LT: lhs_v->type->kind == TYPESPEC_FLOAT ? emit(ctx, "SETB AL") : emit(ctx, "SETL AL"); break;
        case TOKEN_GT: emit(ctx, "SETG AL"); break;
        case TOKEN_LT_EQ: lhs_v->type->kind == TYPESPEC_FLOAT ? emit(ctx, "SETNA AL") : emit(ctx, "SETLE AL"); break;
        case TOKEN_GT_EQ: emit(ctx, "SETGE AL"); break;
        case TOKEN_EQ_EQ: emit(ctx, "SETE AL"); break;
        case TOKEN_BANG_EQ: emit(ctx, "SETNE AL"); break;
        }
        return lhs_v;
    } break;

    case TOKEN_LT_LT_EQ: {
        rhs             = make_expr_binary(TOKEN_LT_LT, lhs, rhs);
        expr            = make_expr_unary(TOKEN_EQ, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_GT_GT_EQ: {
        rhs             = make_expr_binary(TOKEN_GT_GT, lhs, rhs);
        expr            = make_expr_unary(TOKEN_EQ, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_PLUS_EQ: {
        rhs             = make_expr_binary(TOKEN_PLUS, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_MINUS_EQ: {
        rhs             = make_expr_binary(TOKEN_MINUS, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_ASTERISK_EQ: {
        rhs             = make_expr_binary(TOKEN_ASTERISK, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_FWSLASH_EQ: {
        rhs             = make_expr_binary(TOKEN_FWSLASH, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_PERCENT_EQ: {
        rhs             = make_expr_binary(TOKEN_PERCENT, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_PIPE_EQ: {
        rhs             = make_expr_binary(TOKEN_PIPE, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_HAT_EQ: {
        rhs             = make_expr_binary(TOKEN_HAT, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(ctx, expr);
        return variable;
    }
    case TOKEN_QUESTION_MARK: {
        emit(ctx, "CMP %s, 0", get_reg_fitting_value(codegen_expr(ctx, lhs)));
        ctx->temp_label0 = make_text_label(ctx);
        emit(ctx, "JE %s", ctx->temp_label0);
        ctx->temp_label1 = make_text_label(ctx);
        Value* rhs_val   = codegen_expr(ctx, rhs);
        emit(ctx, "JMP %s", ctx->temp_label1);
        return rhs_val;
    }
    case TOKEN_COLON: {
        codegen_expr(ctx, lhs); // '?' part
        emit(ctx, "%s:", ctx->temp_label0);
        Value* rhs_val = codegen_expr(ctx, rhs);
        emit(ctx, "%s:", ctx->temp_label1);
        return rhs_val;
    }
    }

    error("Codegen: Unhandled binary op %s", token_kind_to_str(op));
    return NULL;
}

Value* codegen_variable_decl(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr);
    assert(expr->kind == EXPR_VARIABLE_DECL);
    char*     name            = expr->Variable_Decl.name;
    Typespec* type            = expr->Variable_Decl.type;
    Expr*     assignment_expr = expr->Variable_Decl.value;

    s64 type_size = get_size_of_typespec(type);
    s64 stack_pos = type_size + ctx->stack_index;

    Value* variable = make_value_variable(name, type, stack_pos);
    add_variable(ctx, variable);

    if (assignment_expr)
        if (type->kind != TYPESPEC_ARRAY || type->kind != TYPESPEC_POINTER || type->kind != TYPESPEC_STRING)
            codegen_expr(ctx, make_expr_binary(TOKEN_EQ, make_expr_ident(name), assignment_expr));

    return variable;
}

Value* codegen_call(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    char* callee = expr->Call.callee;
    List* args   = expr->Call.args;

    Typespec* func_t = get_symbol(callee);

    // push the arguments in reverse order onto the stack
    s32 func_arg_count = typespec_function_get_arg_count(func_t);
    s32 arg_count      = args->count;

    if (func_arg_count != arg_count) error("wrong amount of parameters for call to function '%s'", callee);

    // Check if we really need all arguments.
    // If the callee function has any default argument we can skip them

    List* arg_values        = make_list();
    s8    int_arg_counter   = 0;
    s8    float_arg_counter = 0;

    LIST_FOREACH_REVERSE(args) {
        Expr*  arg = (Expr*)it->data;
        Value* v   = codegen_expr(ctx, arg);
        push_type(ctx, v->type);
        list_append(arg_values, v);
    }

    LIST_FOREACH(arg_values) {
        Value* v = (Value*)it->data;
        switch (v->type->kind) {
        case TYPESPEC_ARRAY:   // fallthrough
        case TYPESPEC_POINTER: // fallthrough
        case TYPESPEC_INT: {
            switch (int_arg_counter) {
            case 0: pop(ctx, RDI); break;
            case 1: pop(ctx, RSI); break;
            case 2: pop(ctx, RDX); break;
            case 3: pop(ctx, RCX); break;
            case 4: pop(ctx, R8); break;
            case 5: pop(ctx, R9); break;
            }
            int_arg_counter += 1;
        } break;
        case TYPESPEC_FLOAT: {
            switch (float_arg_counter) {
            case 0: pop(ctx, XMM0); break;
            case 1: pop(ctx, XMM1); break;
            case 2: pop(ctx, XMM2); break;
            case 3: pop(ctx, XMM3); break;
            case 4: pop(ctx, XMM4); break;
            case 5: pop(ctx, XMM5); break;
            case 6: pop(ctx, XMM6); break;
            case 7: pop(ctx, XMM7); break;
            }
            float_arg_counter += 1;
        } break;
        case TYPESPEC_STRUCT: error("undhandled");
        }
    }
    emit(ctx, "CALL _%s", callee);

    Typespec* ret_type = func_t->Function.ret_type;
    if (!ret_type) ret_type = make_typespec_int(1, true);

    return make_value_call(callee, ret_type);
}

Value* codegen_float(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_FLOAT);
    Value* val    = make_value_float(make_typespec_float(DEFAULT_FLOAT_BYTE_SIZE), expr->Float.val);
    char*  flabel = make_data_label(ctx);
    char*  db_op  = get_db_op(val->type);
    emit_data(ctx, "%s %s %f", flabel, db_op, expr->Float.val);
    emit(ctx, "MOV RAX, %s; float_ref", flabel);
    char* mov_op = get_move_op(val->type);
    emit(ctx, "%s XMM0, [RAX]; float_ref", mov_op);
    return val;
}

Value* codegen_int(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_INT);
    Value* val   = make_value_int(DEFAULT_INT_BYTE_SIZE, make_typespec_int(DEFAULT_INT_BYTE_SIZE, 0), expr->Int.val);
    s32    reg_n = get_rax_reg_of_byte_size(DEFAULT_INT_BYTE_SIZE);
    emit(ctx, "MOV %s, %d", get_reg(reg_n), val->Int.value);
    return val;
}

Value* codegen_block(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    push_scope(ctx);
    List*  stmts = expr->Block.stmts;
    Value* last  = NULL;
    LIST_FOREACH(stmts) {
        Expr* stmt = (Expr*)it->data;
        last       = codegen_expr(ctx, stmt);
    }
    pop_scope(ctx);
    return last;
}

Value* codegen_macro(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_MACRO);
    return NULL;
}

Value* codegen_ident(Codegen_Context* ctx, Expr* expr) {
    assert(expr->kind == EXPR_IDENT);
    DEBUG_START;
    char* name       = expr->Ident.name;
    Expr* macro_expr = get_macro_def(name);
    if (macro_expr) {
        return codegen_expr(ctx, macro_expr);
    }
    Value* var = get_variable(ctx, name);
    emit_load(ctx, var);
    return var;
}

Value* codegen_subscript(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_SUBSCRIPT);
    Expr* load = expr->Subscript.load;
    Expr* sub  = expr->Subscript.sub;

    Value* variable = codegen_expr(ctx, load); // ADDRESS OF 'C' is in 'RAX'
    s64    size     = get_size_of_value(variable);

    sub     = make_expr_binary(TOKEN_ASTERISK, make_expr_int(size), sub);
    Expr* t = make_expr_binary(TOKEN_PLUS, load, sub);
    t       = make_expr_unary(THI_SYNTAX_POINTER, t);
    return codegen_expr(ctx, t);
}

Value* codegen_string(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_STRING);
    char*     val    = expr->String.val;
    Typespec* t      = make_typespec_pointer(make_typespec_int(8, 1));
    char*     slabel = make_data_label(ctx);
    char*     db_op  = get_db_op(t);
    emit_data(ctx, "%s %s `%s`, 0 ", slabel, db_op, val);
    emit(ctx, "MOV RAX, %s; string_ref", slabel);
    return make_value_string(val, t);
}

Value* codegen_note(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_NOTE);
    Expr* int_expr = expr->Note.expr;
    assert(int_expr->kind == EXPR_INT);
    s32 integer_value = int_expr->Int.val;
    if (integer_value < 1) error("note parameters start at 1.");

    Expr*  arg  = get_arg_from_func(ctx->current_function->Function.type, integer_value - 1);
    char*  name = arg->Variable_Decl.name;
    Value* var  = get_variable(ctx, name);

    emit_load(ctx, var);

    return var;
}

Value* codegen_if(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_IF);

    char* else_l = make_text_label(ctx);
    char* end_l  = make_text_label(ctx);

    Expr* cond       = expr->If.cond;
    Expr* then_block = expr->If.then_block;
    Expr* else_block = expr->If.else_block;

    codegen_expr(ctx, cond);
    emit(ctx, "CMP AL, 0");
    emit(ctx, "JE %s", else_block ? else_l : end_l);
    codegen_expr(ctx, then_block);
    emit(ctx, "JMP %s", end_l);
    emit(ctx, "%s:", else_l);
    if (else_block) {
        codegen_expr(ctx, else_block);
    }
    emit(ctx, "%s:", end_l);

    return NULL;
}

Value* codegen_for(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_FOR);

    char* begin_l = make_text_label(ctx);
    char* mid_l   = make_text_label(ctx);
    char* end_l   = make_text_label(ctx);

    Expr* init       = expr->For.init;
    Expr* cond       = expr->For.cond;
    Expr* step       = expr->For.step;
    Expr* then_block = expr->For.then_block;

    codegen_expr(ctx, init);
    emit(ctx, "%s:", begin_l);
    codegen_expr(ctx, cond);
    emit(ctx, "CMP AL, 0");
    emit(ctx, "JE %s", end_l);

    set_jump_labels(ctx, mid_l, end_l);
    codegen_expr(ctx, then_block);
    restore_jump_labels(ctx);

    emit(ctx, "%s:", mid_l);
    codegen_expr(ctx, step);
    emit(ctx, "JMP %s", begin_l);
    emit(ctx, "%s:", end_l);

    return NULL;
}

Value* codegen_while(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_WHILE);

    char* begin_l = make_text_label(ctx);
    char* end_l   = make_text_label(ctx);

    Expr* cond       = expr->While.cond;
    Expr* then_block = expr->While.then_block;

    emit(ctx, "%s:", begin_l);
    codegen_expr(ctx, cond);
    emit(ctx, "CMP AL, 0");
    emit(ctx, "JE %s", end_l);

    set_jump_labels(ctx, begin_l, end_l);
    codegen_expr(ctx, then_block);
    restore_jump_labels(ctx);

    emit(ctx, "JMP %s", begin_l);
    emit(ctx, "%s:", end_l);

    return NULL;
}

Value* codegen_defer(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_DEFER);
    Expr* defer_expr = expr->Defer.expr;
    list_append(ctx->current_function->Function.defers, defer_expr);
    return NULL;
}

Value* codegen_return(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_RETURN);

    Expr* ret_expr = expr->Return.expr;

    char* label  = make_text_label(ctx);
    char* label2 = make_text_label(ctx);

    emit(ctx, "%s:", label2);
    List* defers = ctx->current_function->Function.defers;
    LIST_FOREACH_REVERSE(defers) {
        Expr* defer_expr = (Expr*)it->data;
        codegen_expr(ctx, defer_expr);
    }
    emit(ctx, "JMP %s", label);
    emit(ctx, "%s:", label);
    if (ret_expr) {
        codegen_expr(ctx, ret_expr);
    }
    emit(ctx, "JMP %s", DEFAULT_FUNCTION_END_LABEL_NAME);

    return NULL;
}

Value* codegen_break(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_BREAK);
    emit(ctx, "JMP %s", ctx->lbreak);
    return NULL;
}

Value* codegen_cast(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_CAST);
    Expr*     e = expr->Cast.expr;
    Typespec* t = expr->Cast.type;
    Value*    v = codegen_expr(ctx, e);
    emit_cast(ctx, v, t);
    return v;
}

Value* codegen_continue(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_CONTINUE);
    emit(ctx, "JMP %s", ctx->lcontinue);
    return NULL;
}

Value* codegen_struct(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_STRUCT);
    warning("struct incomplete?");
    return make_value_struct(expr->Struct.type);
}

Value* codegen_function(Codegen_Context* ctx, Expr* expr) {
    DEBUG_START;
    assert(expr);
    assert(expr->kind == EXPR_FUNCTION);

    set_current_function_expr(ctx, expr);

    char*     func_name = expr->Function.type->Function.name;
    Typespec* func_type = expr->Function.type;
    Expr*     func_body = expr->Function.body;

    push_scope(ctx);

    emit(ctx, "_%s:", func_name);
    push(ctx, RBP);
    emit(ctx, "MOV RBP, RSP");

    // Sum the params
    s64 sum = get_size_of_typespec(expr->Function.type);

    sum += get_all_alloca_in_block(func_body);

    s64 stack_allocated = sum;
    s32 padding =
        (X64_ASM_OSX_STACK_PADDING - (stack_allocated % X64_ASM_OSX_STACK_PADDING)) % X64_ASM_OSX_STACK_PADDING;
    if (stack_allocated + padding)
        emit(ctx, "SUB RSP, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);

    emit(ctx, "%s:", DEFAULT_FUNCTION_ENTRY_LABEL_NAME);

    reset_stack(ctx);

    List* args = func_type->Function.args;
    s64   i    = 0;
    s8    int_arg_counter   = 0;
    s8    float_arg_counter = 0;
    LIST_FOREACH(args) {
        Expr*  arg       = (Expr*)it->data;
        Value* v         = codegen_expr(ctx, arg);
        s64    size      = get_size_of_value(v);

        s8    param_reg = -1;

        switch (v->type->kind) {
        case TYPESPEC_ARRAY:   // fallthrough
        case TYPESPEC_POINTER: // fallthrough
        case TYPESPEC_INT: {
            switch (int_arg_counter) {
            case 0: param_reg = RDI; break;
            case 1: param_reg = RSI; break;
            case 2: param_reg = RDX; break;
            case 3: param_reg = RCX; break;
            case 4: param_reg = R8; break;
            case 5: param_reg = R9; break;
            }
            int_arg_counter += 1;
        } break;
        case TYPESPEC_FLOAT: {
            switch (float_arg_counter) {
            case 0: param_reg = XMM0; break;
            case 1: param_reg = XMM1; break;
            case 2: param_reg = XMM2; break;
            case 3: param_reg = XMM3; break;
            case 4: param_reg = XMM4; break;
            case 5: param_reg = XMM5; break;
            case 6: param_reg = XMM6; break;
            case 7: param_reg = XMM7; break;
            }
            float_arg_counter += 1;
        } break;
        case TYPESPEC_STRUCT: error("undhandled");
        }

        emit_store_r(ctx, v, param_reg);
        i += 1;
    }

    codegen_expr(ctx, func_body);

    emit(ctx, "%s:", DEFAULT_FUNCTION_END_LABEL_NAME);

    if (stack_allocated + padding)
        emit(ctx, "ADD RSP, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);

    emit(ctx, "LEAVE");
    emit(ctx, "RET");

    reset_text_label_counter(ctx);

    pop_scope(ctx);

    return NULL;
}

s64 get_all_alloca_in_block(Expr* block) {
    s64   sum   = 0;
    List* stmts = block->Block.stmts;
    LIST_FOREACH(stmts) {
        Expr* stmt = (Expr*)it->data;
        switch (stmt->kind) {
        case EXPR_VARIABLE_DECL: sum += get_size_of_typespec(stmt->Variable_Decl.type); break;
        case EXPR_BLOCK: sum += get_all_alloca_in_block(stmt); break;
        }
    }
    return sum;
}

Codegen_Context make_codegen_context() {
    Codegen_Context ctx;
    ctx.scope_stack                    = make_stack();
    ctx.current_function               = NULL;
    ctx.section_text                   = make_string("");
    ctx.section_data                   = make_string("");
    ctx.stack_index                    = 0;
    ctx.temp_label0                    = NULL;
    ctx.temp_label1                    = NULL;
    ctx.text_label_counter             = 0;
    ctx.data_label_counter             = 0;
    ctx.ocontinue                      = NULL;
    ctx.lcontinue                      = NULL;
    ctx.obreak                         = NULL;
    ctx.lbreak                         = NULL;
    ctx.l_end                          = NULL;
    ctx.next_available_xmm_reg_counter = 0;
    ctx.next_available_rax_reg_counter = 0;
    return ctx;
}

char* compile(Codegen_Context* ctx) {
    LIST_FOREACH(ctx->foreign_function_list) {
        char* func_name = ((Typespec*)it->data)->Function.name;
        append_string_f(&ctx->section_data, "extern _%s\n", func_name);
    }
    append_string(&ctx->section_data, "section .data\n");
    emit_no_tab(ctx, "section .text");
    LIST_FOREACH(ctx->ast) { codegen_expr(ctx, (Expr*)it->data); }
    char* output = strf("%sglobal _main\n%s", ctx->section_data.c_str, ctx->section_text.c_str);
    return output;
}

void set_jump_labels(Codegen_Context* ctx, char* continue_l, char* break_l) {
    ctx->ocontinue = ctx->lcontinue;
    ctx->obreak    = ctx->lbreak;
    ctx->lcontinue = continue_l;
    ctx->lbreak    = break_l;
}

void restore_jump_labels(Codegen_Context* ctx) {
    ctx->lcontinue = ctx->ocontinue;
    ctx->lbreak    = ctx->obreak;
}

char* make_text_label(Codegen_Context* ctx) {
    char* l = strf(".L%d", ctx->text_label_counter);
    ctx->text_label_counter += 1;
    return l;
}

char* make_data_label(Codegen_Context* ctx) {
    char* l = strf("D%d", ctx->data_label_counter);
    ctx->data_label_counter += 1;
    return l;
}

void reset_text_label_counter(Codegen_Context* ctx) { ctx->text_label_counter = 0; }
void reset_stack(Codegen_Context* ctx) { ctx->stack_index = 0; }
void set_current_function_expr(Codegen_Context* ctx, Expr* func_expr) { ctx->current_function = func_expr; }

char* emit_save_result(Codegen_Context* ctx, Value* value) {
    char* mov_op     = get_move_op(value->type);
    char* reg        = get_next_available_reg_fitting(ctx, value->type);
    char* result_reg = get_result_reg(value->type);
    emit(ctx, "%s %s, %s", mov_op, reg, result_reg);
    return reg;
}

char* get_instr(Token_Kind op, Typespec* type) {
    char* inst = NULL;
    switch (type->kind) {
    case TYPESPEC_INT: {
        bool usig = type->Int.is_unsigned;
        switch (op) {
        case TOKEN_PLUS: inst = "ADD"; break;
        case TOKEN_MINUS: inst = "SUB"; break;
        case TOKEN_ASTERISK: inst = "IMUL"; break;
        case TOKEN_FWSLASH: inst = (usig ? "DIV" : "IDIV"); break;
        }
    } break;
    case TYPESPEC_FLOAT: {
        s64 size = get_size_of_typespec(type);
        switch (op) {
        case TOKEN_PLUS: inst = (size == 8 ? "ADDSD" : "ADDSS"); break;
        case TOKEN_MINUS: inst = (size == 8 ? "SUBSD" : "SUBSS"); break;
        case TOKEN_ASTERISK: inst = (size == 8 ? "MULSD" : "MULSS"); break;
        case TOKEN_FWSLASH: inst = (size == 8 ? "DIVSD" : "DIVSS"); break;
        }
    } break;
    }
    return inst;
}
char* get_next_available_reg_fitting(Codegen_Context* ctx, Typespec* type) {
    s64 size = get_size_of_typespec(type);
    s8  r    = -1;
    switch (type->kind) {
    case TYPESPEC_ARRAY:   // fallthrough
    case TYPESPEC_POINTER: // fallthrough
    case TYPESPEC_INT: r = get_next_available_rax_reg_fitting(ctx, size); break;
    case TYPESPEC_FLOAT: r = get_next_available_xmm_reg_fitting(ctx); break;
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

    if (ctx->next_available_xmm_reg_counter == 7) ctx->next_available_xmm_reg_counter = 0;
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

    if (ctx->next_available_rax_reg_counter == 5) ctx->next_available_rax_reg_counter = 0;
    ++ctx->next_available_rax_reg_counter;
    return res;
}
