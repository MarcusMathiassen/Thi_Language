#include "codegen.h"
#include "ast.h"
#include "globals.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "string.h" // string
#include "typedefs.h"
#include "utility.h" // error warning info, etc
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //

typedef struct {
    Expr* current_function;

    string output;

    Stack stack;

    s64 stack_index;

    char* temp_label0;
    char* temp_label1;

    s64   text_label_counter;
    s64   data_label_counter;
    char* ocontinue;
    char* lcontinue;
    char* obreak;
    char* lbreak;
    char* l_end;
} Codegen_Context;

Codegen_Context make_codegen_context() {
    Codegen_Context cctx;
    cctx.current_function   = NULL;
    cctx.output             = make_string("");
    cctx.stack_index        = 0;
    cctx.temp_label0        = NULL;
    cctx.temp_label1        = NULL;
    cctx.text_label_counter = 0;
    cctx.data_label_counter = 0;
    cctx.ocontinue          = NULL;
    cctx.lcontinue          = NULL;
    cctx.obreak             = NULL;
    cctx.lbreak             = NULL;
    cctx.l_end              = NULL;
    return cctx;
}

void ctx_set_jump_labels(Codegen_Context* cctx, char* continue_l, char* break_l) {
    cctx->ocontinue = cctx->lcontinue;
    cctx->obreak    = cctx->lbreak;
    cctx->lcontinue = continue_l;
    cctx->lbreak    = break_l;
}

void ctx_restore_jump_labels(Codegen_Context* cctx) {
    cctx->lcontinue = cctx->ocontinue;
    cctx->lbreak    = cctx->obreak;
}

char* ctx_make_text_label(Codegen_Context* cctx) {
    char* l = strf(".L%d", cctx->text_label_counter);
    cctx->text_label_counter += 1;
    return l;
}

char* ctx_make_data_label(Codegen_Context* cctx) {
    char* l = strf("D%d", cctx->data_label_counter);
    cctx->data_label_counter += 1;
    return l;
}
void ctx_reset_text_label_counter(Codegen_Context* cctx) { cctx->text_label_counter = 0; }

Typespec*       integer_literal_type = NULL;
Codegen_Context cctx;
string          section_text, section_data;
Stack           scope_stack;

#define DEBUG_START info("%s: %s", __func__, wrap_with_colored_parens(expr_to_str(expr)));

Value* codegen_expr(Expr* expr);

void emit_no_tab(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    append_string(&section_text, str);
    append_string(&section_text, "\n");
}

void emit_data(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    append_string(&section_data, strf("\t%s", str));
    append_string(&section_data, "\n");
}
void emit(char* fmt, ...) {
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

    if (is_label)
        append_string(&section_text, strf("%s", str));
    else
        append_string(&section_text, strf("\t%s", str));
    append_string(&section_text, "\n");
}

void push(int reg) {
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    emit("PUSH %s", get_reg(reg));
    cctx.stack_index += 8;
}

void pop(int reg) {
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    emit("POP %s", get_reg(reg));
    cctx.stack_index -= 8;
    assert(cctx.stack_index >= 0);
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

void alloc_variable(Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    info("Allocating variable '%s', type '%s', size '%lld' ", variable->Variable.name, typespec_to_str(variable->type),
         size);
    cctx.stack_index += size;
}

void dealloc_variable(Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 size = get_size_of_value(variable);
    info("Deallocating variable '%s', type '%s', size '%lld' ", variable->Variable.name,
         typespec_to_str(variable->type), size);
    cctx.stack_index -= size;
    assert(cctx.stack_index >= 0);
}

void push_scope() {
    Scope* new_scope = make_scope();
    stack_push(&scope_stack, new_scope);
}
void pop_scope() {
    Scope* scope = (Scope*)stack_pop(&scope_stack);
    LIST_FOREACH(scope->local_variables) {
        Value* v = (Value*)it->data;
        dealloc_variable(v);
    }
}
void print_scope(Scope* scope) {
    assert(scope);
    info("Scope count %d", scope->local_variables->count);
    s32 index = 0;
    LIST_FOREACH(scope->local_variables) {
        Value* v = (Value*)it->data;
        info("Scope index: %lld variable: %s", index, v->Variable.name);
        index += 1;
    }
}

/// Returns the value, or NULL if not found.
Value* get_variable_in_scope(Scope* scope, char* name) {
    assert(scope);
    assert(name);
    LIST_FOREACH(scope->local_variables) {
        Value* v = (Value*)it->data;
        if (v->Variable.name == name) return v;
    }
    return NULL;
}

Value* get_variable(char* name) {
    assert(name);
    STACK_FOREACH(scope_stack) {
        Scope* scope = (Scope*)it->data;
        Value* res   = get_variable_in_scope(scope, name);
        if (res) return res;
    }
    error("no variable with name '%s'", name);
    return NULL;
}

void add_variable(Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    alloc_variable(variable);
    info("Adding variable: '%s' to scope", variable->Variable.name);
    Scope* top = (Scope*)stack_peek(&scope_stack);
    list_append(top->local_variables, variable);
}

int align(int n, s32 m) { return (m - (n % m)) % m; }

void emit_store_r(Value* variable, s64 reg) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    char* reg_c     = get_reg(reg);
    switch (variable->type->kind) {
    case TYPESPEC_STRUCT:
    case TYPESPEC_POINTER:
    case TYPESPEC_ARRAY: emit("MOV [RAX], %s; store", reg_c); break;
    default: emit("MOV [RBP-%lld], %s; store", stack_pos, reg_c); break;
    }
}
void emit_store(Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64 stack_pos = get_stack_pos_of_variable(variable);
    switch (variable->type->kind) {
    // case TYPESPEC_STRUCT:
    // case TYPESPEC_POINTER:
    // case TYPESPEC_ARRAY: emit("MOV [RAX], RCX; store"); break;
    default: emit("MOV [RBP-%lld], RCX; store", stack_pos); break;
    }
}

void emit_load(Value* variable) {
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    s64   stack_pos = get_stack_pos_of_variable(variable);
    s64   size      = get_size_of_value(variable);
    char* reg       = get_reg(get_rax_reg_of_byte_size(size));
    char* mov_size  = get_op_size(size);
    switch (variable->type->kind) {
    // case TYPESPEC_POINTER:
    // case TYPESPEC_ARRAY: emit("LEA RAX, [RBP-%lld]; load_ref", stack_pos); break;
    default: emit("MOV %s, %s [RBP-%lld]; load", reg, mov_size, stack_pos); break;
    }
}

Value* codegen_unary(Expr* expr) {
    DEBUG_START;
    Token_Kind op      = expr->Unary.op;
    Expr*      operand = expr->Unary.operand;

    Value* operand_val = codegen_expr(operand);
    s32    reg_n       = get_rax_reg_of_byte_size(get_size_of_value(operand_val));
    Value* result      = operand_val;

    char* reg = get_reg(reg_n);

    switch (op) {
    case THI_SYNTAX_ADDRESS: {
        // Operand must be an identifier
        s64 stack_pos = get_stack_pos_of_variable(operand_val);
        emit("LEA RAX, [RSP-%lld]; addrsof", stack_pos);
    } break;
    case THI_SYNTAX_POINTER: {
        Typespec* t = operand_val->type->Array.type;
        switch (t->kind) {
        case TYPESPEC_ARRAY:
        case TYPESPEC_POINTER: emit("LEA RAX, [RAX]; deref"); break;
        default: emit("MOV RAX, [RAX]; deref"); break;
        }
    } break;
    case TOKEN_BANG: {
        emit("CMP %s, 0", reg);
        emit("SETE AL");
    } break;
    case TOKEN_PLUS: { // no nothing
    } break;
    case TOKEN_TILDE: {
        emit("NOT AL");
    } break;
    case TOKEN_MINUS: {
        emit("NEG %s", reg);
        break;
    }
    default: error("unhandled unary case: %c", token_kind_to_str(op)); break;
    }
    return result;
}

Value* codegen_binary(Expr* expr) {
    DEBUG_START;

    Token_Kind op  = expr->Binary.op;
    Expr*      lhs = expr->Binary.lhs;
    Expr*      rhs = expr->Binary.rhs;

    switch (op) {
    // Field access
    case TOKEN_DOT: {
        error("FIELD ACCESS NOT IMPLEMENTED");
        Value* variable = get_variable(lhs->Ident.name);

        assert(variable->kind == VALUE_VARIABLE);
        assert(variable->type->kind == TYPESPEC_STRUCT);

        s64 offset = get_offset_in_struct_to_field(variable->type, rhs->Ident.name);
        // if (offset == 0) { emit_load(variable); return variable; }

        lhs = make_expr_unary(THI_SYNTAX_ADDRESS, lhs);
        lhs = make_expr_binary(TOKEN_PLUS, lhs, make_expr_int(offset));
        lhs = make_expr_unary(THI_SYNTAX_POINTER, lhs);

        return codegen_expr(lhs);
    }
    case THI_SYNTAX_ASSIGNMENT: {
        codegen_expr(rhs);
        push(RAX);
        Value* variable = NULL;
        push(RAX);

        // @HACK: i dont like specific cases like this.
        // if (lhs->kind == EXPR_UNARY) {
        // variable = codegen_expr(lhs->Unary.operand);
        // } else {
        variable = codegen_expr(lhs);
        // }

        pop(RCX);
        emit_store(variable);
        pop(RAX);
        return variable;
    }
    case TOKEN_PLUS: {
        codegen_expr(rhs);
        push(RAX);
        Value* lhs_v = codegen_expr(lhs);
        pop(RCX);
        emit("ADD RAX, RCX");
        return lhs_v;
    }
    case TOKEN_MINUS: {
        rhs             = make_expr_unary(TOKEN_MINUS, rhs);
        expr            = make_expr_binary(TOKEN_PLUS, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_ASTERISK: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("IMUL RAX, RCX");
        return lhs_v;
    }
    case TOKEN_FWSLASH: {
        Value* rhs_v = codegen_expr(rhs);
        push(RAX);
        codegen_expr(lhs);
        pop(RCX);
        emit("CDQ");
        emit("IDIV RCX");
        return rhs_v;
    }
    case TOKEN_PERCENT: {
        expr            = make_expr_binary(TOKEN_FWSLASH, lhs, rhs);
        Value* variable = codegen_expr(expr);
        emit("MOV RAX, RDX");
        return variable;
    }
    case TOKEN_PIPE: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("OR AL, CL");
        return lhs_v;
    }
    case TOKEN_AND: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("AND AL, CL");
        return lhs_v;
    }
    case TOKEN_HAT: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("XOR AL, CL");
        return lhs_v;
    }
    case TOKEN_LT_LT: {
        Value* lhs_v = codegen_expr(lhs);
        assert(rhs->kind == EXPR_INT);
        emit("SHL AL, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_GT_GT: {
        Value* lhs_v = codegen_expr(lhs);
        assert(rhs->kind == EXPR_INT);
        emit("SHR AL, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_AND_AND: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RAX, 0");
        emit("SETNE AL");
        emit("CMP RCX, 0");
        emit("SETNE CL");
        emit("AND AL, CL");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        expr     = make_expr_binary(TOKEN_PIPE, lhs, rhs);
        Value* v = codegen_expr(expr);
        emit("SETNE AL");
        return v;
    }

    case TOKEN_LT:    // FALLTHROUGH
    case TOKEN_GT:    // FALLTHROUGH
    case TOKEN_LT_EQ: // FALLTHROUGH
    case TOKEN_GT_EQ: // FALLTHROUGH
    case TOKEN_EQ_EQ: // FALLTHROUGH
    case TOKEN_BANG_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        switch (op) {
        case TOKEN_LT: emit("SETL AL"); break;
        case TOKEN_GT: emit("SETG AL"); break;
        case TOKEN_LT_EQ: emit("SETLE AL"); break;
        case TOKEN_GT_EQ: emit("SETGE AL"); break;
        case TOKEN_EQ_EQ: emit("SETE AL"); break;
        case TOKEN_BANG_EQ: emit("SETNE AL"); break;
        }
        return lhs_v;
    } break;

    case TOKEN_LT_LT_EQ: {
        rhs             = make_expr_binary(TOKEN_LT_LT, lhs, rhs);
        expr            = make_expr_unary(TOKEN_EQ, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_GT_GT_EQ: {
        rhs             = make_expr_binary(TOKEN_GT_GT, lhs, rhs);
        expr            = make_expr_unary(TOKEN_EQ, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_PLUS_EQ: {
        rhs             = make_expr_binary(TOKEN_PLUS, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_MINUS_EQ: {
        rhs             = make_expr_binary(TOKEN_MINUS, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_ASTERISK_EQ: {
        rhs             = make_expr_binary(TOKEN_ASTERISK, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_FWSLASH_EQ: {
        rhs             = make_expr_binary(TOKEN_FWSLASH, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_PERCENT_EQ: {
        rhs             = make_expr_binary(TOKEN_PERCENT, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_PIPE_EQ: {
        rhs             = make_expr_binary(TOKEN_PIPE, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_HAT_EQ: {
        rhs             = make_expr_binary(TOKEN_HAT, lhs, rhs);
        expr            = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_QUESTION_MARK: {
        emit("CMP %s, 0", get_reg_fitting_value(codegen_expr(lhs)));
        cctx.temp_label0 = ctx_make_text_label(&cctx);
        emit("JE %s", cctx.temp_label0);
        cctx.temp_label1 = ctx_make_text_label(&cctx);
        Value* rhs_val   = codegen_expr(rhs);
        emit("JMP %s", cctx.temp_label1);
        return rhs_val;
    }
    case TOKEN_COLON: {
        codegen_expr(lhs); // '?' part
        emit("%s:", cctx.temp_label0);
        Value* rhs_val = codegen_expr(rhs);
        emit("%s:", cctx.temp_label1);
        return rhs_val;
    }
    }

    error("Codegen: Unhandled binary op %s", token_kind_to_str(op));
    return NULL;
}

Value* codegen_variable_decl(Expr* expr) {
    DEBUG_START;
    assert(expr);
    assert(expr->kind == EXPR_VARIABLE_DECL);
    char*     name            = expr->Variable_Decl.name;
    Typespec* type            = expr->Variable_Decl.type;
    Expr*     assignment_expr = expr->Variable_Decl.value;

    s64 type_size = get_size_of_typespec(type);
    s64 stack_pos = type_size + cctx.stack_index;

    Value* variable = make_value_variable(name, type, stack_pos);
    add_variable(variable);

    if (assignment_expr)
        if (type->kind != TYPESPEC_ARRAY || type->kind != TYPESPEC_POINTER || type->kind != TYPESPEC_STRING)
            codegen_expr(make_expr_binary(TOKEN_EQ, make_expr_ident(name), assignment_expr));

    return variable;
}

Value* codegen_call(Expr* expr) {
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
    LIST_FOREACH(args) {
        Expr* arg = (Expr*)it->data;
        codegen_expr(arg);
        push(RAX);
    }

    // %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
    for (int i = arg_count - 1; i >= 0; --i) {
        switch (i) {
        case 0: pop(RDI); break;
        case 1: pop(RSI); break;
        case 2: pop(RDX); break;
        case 3: pop(RCX); break;
        case 4: pop(R8); break;
        case 5: pop(R9); break;
        }
    }

    emit("CALL _%s", callee);

    Typespec* ret_type = func_t->Function.ret_type;
    if (!ret_type) ret_type = make_typespec_int(1, true);

    return make_value_call(callee, ret_type);
}

Value* codegen_int(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_INT);
    Value* val   = make_value_int(DEFAULT_INT_BYTE_SIZE, integer_literal_type, expr->Int.val);
    s32    reg_n = get_rax_reg_of_byte_size(DEFAULT_INT_BYTE_SIZE);
    emit("MOV %s, %d", get_reg(reg_n), val->Int.value);
    return val;
}

Value* codegen_block(Expr* expr) {
    DEBUG_START;
    push_scope();
    List*  stmts = expr->Block.stmts;
    Value* last  = NULL;
    LIST_FOREACH(stmts) {
        Expr* stmt = (Expr*)it->data;
        last       = codegen_expr(stmt);
    }
    pop_scope();
    return last;
}

Value* codegen_macro(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_MACRO);
    return NULL;
}

Value* codegen_ident(Expr* expr) {
    assert(expr->kind == EXPR_IDENT);
    DEBUG_START;
    char* name       = expr->Ident.name;
    Expr* macro_expr = get_macro_def(name);
    if (macro_expr) {
        return codegen_expr(macro_expr);
    }
    Value* var = get_variable(name);
    emit_load(var);
    return var;
}

Value* codegen_subscript(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_SUBSCRIPT);
    Expr* load = expr->Subscript.load;
    Expr* sub  = expr->Subscript.sub;

    Value* variable = codegen_expr(load); // ADDRESS OF 'C' is in 'RAX'
    s64    size     = get_size_of_value(variable);

    sub     = make_expr_binary(TOKEN_ASTERISK, make_expr_int(size), sub);
    Expr* t = make_expr_binary(TOKEN_PLUS, load, sub);
    t       = make_expr_unary(THI_SYNTAX_POINTER, t);
    return codegen_expr(t);
}

Value* codegen_string(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_STRING);
    char*     val    = expr->String.val;
    Typespec* t      = make_typespec_pointer(make_typespec_int(8, 1));
    char*     slabel = ctx_make_data_label(&cctx);
    emit_data("%s db `%s`, 0 ", slabel, val);
    emit("MOV RAX, %s; string_ref", slabel);
    return make_value_string(val, t);
}

Value* codegen_note(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_NOTE);
    Expr* int_expr = expr->Note.expr;
    assert(int_expr->kind == EXPR_INT);
    s32 integer_value = int_expr->Int.val;
    if (integer_value < 1) error("note parameters start at 1.");

    Expr*  arg  = get_arg_from_func(cctx.current_function->Function.type, integer_value - 1);
    char*  name = arg->Variable_Decl.name;
    Value* var  = get_variable(name);

    emit_load(var);

    return var;
}

Value* codegen_if(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_IF);

    char* else_l = ctx_make_text_label(&cctx);
    char* end_l  = ctx_make_text_label(&cctx);

    Expr* cond       = expr->If.cond;
    Expr* then_block = expr->If.then_block;
    Expr* else_block = expr->If.else_block;

    codegen_expr(cond);
    emit("CMP AL, 0");
    emit("JE %s", else_block ? else_l : end_l);
    codegen_expr(then_block);
    emit("JMP %s", end_l);
    emit("%s:", else_l);
    if (else_block) {
        codegen_expr(else_block);
    }
    emit("%s:", end_l);

    return NULL;
}

Value* codegen_for(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_FOR);

    char* begin_l = ctx_make_text_label(&cctx);
    char* mid_l   = ctx_make_text_label(&cctx);
    char* end_l   = ctx_make_text_label(&cctx);

    Expr* init       = expr->For.init;
    Expr* cond       = expr->For.cond;
    Expr* step       = expr->For.step;
    Expr* then_block = expr->For.then_block;

    codegen_expr(init);
    emit("%s:", begin_l);
    codegen_expr(cond);
    emit("CMP AL, 0");
    emit("JE %s", end_l);

    ctx_set_jump_labels(&cctx, mid_l, end_l);
    codegen_expr(then_block);
    ctx_restore_jump_labels(&cctx);

    emit("%s:", mid_l);
    codegen_expr(step);
    emit("JMP %s", begin_l);
    emit("%s:", end_l);

    return NULL;
}

Value* codegen_while(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_WHILE);

    char* begin_l = ctx_make_text_label(&cctx);
    char* end_l   = ctx_make_text_label(&cctx);

    Expr* cond       = expr->While.cond;
    Expr* then_block = expr->While.then_block;

    emit("%s:", begin_l);
    codegen_expr(cond);
    emit("CMP AL, 0");
    emit("JE %s", end_l);

    ctx_set_jump_labels(&cctx, begin_l, end_l);
    codegen_expr(then_block);
    ctx_restore_jump_labels(&cctx);

    emit("JMP %s", begin_l);
    emit("%s:", end_l);

    return NULL;
}

Value* codegen_defer(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_DEFER);
    Expr* defer_expr = expr->Defer.expr;
    list_append(cctx.current_function->Function.defers, defer_expr);
    return NULL;
}

Value* codegen_return(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_RETURN);

    Expr* ret_expr = expr->Return.expr;

    char* label  = ctx_make_text_label(&cctx);
    char* label2 = ctx_make_text_label(&cctx);

    emit("%s:", label2);
    List* defers = cctx.current_function->Function.defers;
    LIST_FOREACH_REVERSE(defers) {
        Expr* defer_expr = (Expr*)it->data;
        codegen_expr(defer_expr);
    }
    emit("JMP %s", label);
    emit("%s:", label);
    if (ret_expr) {
        codegen_expr(ret_expr);
    }
    emit("JMP %s", DEFAULT_FUNCTION_END_LABEL_NAME);

    return NULL;
}

Value* codegen_break(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_BREAK);
    emit("JMP %s", cctx.lbreak);
    return NULL;
}
Value* codegen_continue(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_CONTINUE);
    emit("JMP %s", cctx.lcontinue);
    return NULL;
}

Value* codegen_struct(Expr* expr) {
    DEBUG_START;
    assert(expr->kind == EXPR_STRUCT);
    warning("struct incomplete?");
    return make_value_struct(expr->Struct.type);
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

Value* codegen_function(Expr* expr) {
    DEBUG_START;
    assert(expr);
    assert(expr->kind == EXPR_FUNCTION);

    cctx.current_function = expr;

    char*     func_name = expr->Function.type->Function.name;
    Typespec* func_type = expr->Function.type;
    Expr*     func_body = expr->Function.body;

    push_scope();

    emit("_%s:", func_name);
    push(RBP);
    emit("MOV RBP, RSP");

    // Sum the params
    s64 sum = get_size_of_typespec(expr->Function.type);

    sum += get_all_alloca_in_block(func_body);

    s64 stack_allocated = sum;
    s32 padding =
        (X64_ASM_OSX_STACK_PADDING - (stack_allocated % X64_ASM_OSX_STACK_PADDING)) % X64_ASM_OSX_STACK_PADDING;
    if (stack_allocated + padding)
        emit("SUB RSP, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);

    emit("%s:", DEFAULT_FUNCTION_ENTRY_LABEL_NAME);

    cctx.stack_index = 0;

    List* args = func_type->Function.args;
    s64   i    = 0;
    LIST_FOREACH(args) {
        Expr*  arg       = (Expr*)it->data;
        Value* v         = codegen_expr(arg);
        s64    size      = get_size_of_value(v);
        s64    param_reg = get_parameter_reg(i, size);
        emit_store_r(v, param_reg);
        i += 1;
    }

    codegen_expr(func_body);

    emit("%s:", DEFAULT_FUNCTION_END_LABEL_NAME);

    if (stack_allocated + padding)
        emit("ADD RSP, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);

    emit("LEAVE");
    emit("RET");

    ctx_reset_text_label_counter(&cctx);

    pop_scope();

    return NULL;
}

// @Hotpath
Value* codegen_expr(Expr* expr) {
    switch (expr->kind) {
    case EXPR_ASM: emit("%s", expr->Asm.str); return NULL;
    case EXPR_MACRO: return codegen_macro(expr);
    case EXPR_STRUCT: return codegen_struct(expr);
    case EXPR_FUNCTION: return codegen_function(expr);
    case EXPR_NOTE: return codegen_note(expr);
    case EXPR_INT: return codegen_int(expr);
    case EXPR_FLOAT: error("EXPR_FLOAT codegen not implemented");
    case EXPR_STRING: return codegen_string(expr);
    case EXPR_IDENT: return codegen_ident(expr);
    case EXPR_CALL: return codegen_call(expr);
    case EXPR_UNARY: return codegen_unary(expr);
    case EXPR_BINARY: return codegen_binary(expr);
    case EXPR_VARIABLE_DECL: return codegen_variable_decl(expr);
    case EXPR_BLOCK: return codegen_block(expr);
    case EXPR_GROUPING: return codegen_expr(expr->Grouping.expr);
    case EXPR_SUBSCRIPT: return codegen_subscript(expr);
    case EXPR_IF: return codegen_if(expr);
    case EXPR_FOR: return codegen_for(expr);
    case EXPR_WHILE: return codegen_while(expr);
    case EXPR_RETURN: return codegen_return(expr);
    case EXPR_DEFER: return codegen_defer(expr);
    case EXPR_BREAK: return codegen_break(expr);
    case EXPR_CONTINUE: return codegen_continue(expr);
    default: error("Unhandled codegen_expr case for kind '%s'", expr_kind_to_str(expr->kind));
    }
    return NULL;
}

char* generate_code_from_ast(List* ast) {
    info("Generating X64 Assembly from AST");

    integer_literal_type = make_typespec_int(DEFAULT_INT_BYTE_SIZE, false);

    cctx = make_codegen_context();
    stack_init(&scope_stack);

    section_text = make_string("");
    section_data = make_string("");

    List* foreign_function_list = get_foreign_function_list();
    LIST_FOREACH(foreign_function_list) {
        char* func_name = ((Typespec*)it->data)->Function.name;
        append_string(&section_data, strf("extern _%s\n", func_name));
    }

    append_string(&section_data, "section .data\n");
    emit_no_tab("section .text");

    // codegen the whole thing.
    LIST_FOREACH(ast) { codegen_expr((Expr*)it->data); }

    char* output = strf("%sglobal _main\n%s", section_data.c_str, section_text.c_str);

    info("%s", output);

    return output;
}
