#include "codegen.h"
#include "ast.h"
#include "context.h" // Context
#include "globals.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "stretchy_buffer.h" // sb_free
#include "string.h"          // string
#include "typedefs.h"
#include "utility.h" // error warning info, etc
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <string.h>  // strncat,

Typespec* integer_literal_type = NULL;
Context ctx;
string output;
Stack scope_stack;
Value** functions = NULL;

Value* codegen_expr(Expr* expr);

void emit_no_tab(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    i64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    append_string(&output, str);
    append_string(&output, "\n");
}

void emit(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    i64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    bool is_label = false;

    for (int i = 0; i < str_len; ++i) {
        if (str[i] == ':')  {
            is_label = true;
            break;
        }
    }

    if (is_label)
        append_string(&output, strf("%s", str));
    else 
        append_string(&output, strf("\t%s", str));
    append_string(&output, "\n");
}

void push(int reg)
{
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    emit("PUSH %s", get_reg(reg));
    ctx.stack_index += 8;
}

void pop(int reg)
{
    assert(reg >= 0 && reg <= TOTAL_REG_COUNT);
    emit("POP %s", get_reg(reg));
    ctx.stack_index -= 8;
    // assert(ctx.stack_index >= 0);
}

char* get_op_size(i8 bytes)
{
    switch (bytes) {
    case 1: return "BYTE";
    case 2: return "WORD";
    case 4: return "DWORD";
    case 8: return "QWORD";
    default: error("get_op_size unknown byte size: %d", bytes);
    }
    return NULL;
}

void push_scope()
{
    Scope* new_scope = make_scope(10);
    stack_push(&scope_stack, new_scope);
}

void alloc_variable(Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    i64 size = get_size_of_value(variable);
    info("Allocating variable '%s' of size %lld", variable->Variable.name, size);
    ctx.stack_index += size;
}

void dealloc_variable(Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    i64 size = get_size_of_value(variable);
    info("Deallocating variable '%s' of size %lld", variable->Variable.name, size);
    ctx.stack_index -= size;
    assert(ctx.stack_index >= 0);
}

void pop_scope()
{
    stack_pop(&scope_stack);
    // Scope* scope = (Scope*)stack_pop(&scope_stack);
    // for (i64 i = 0; i < scope->count; ++i) {
    //     dealloc_variable(scope->local_variables[i]);
    // }
}

void append_variable_to_scope(Scope* s, Value* value)
{
    assert(s);
    assert(value);
    if (s->alloc_count < s->count + 1) {
        s->alloc_count += 1;
        s->local_variables = xrealloc(s->local_variables, sizeof(Value*) * s->alloc_count);
    }
    s->local_variables[s->count++] = value;
}

void print_scope(Scope* scope)
{
    info("Scope count %d", scope->count);
    info("Scope alloc_count %d", scope->alloc_count);
    for (i64 i = 0; i < scope->count; ++i) {
        info("Scope index: %lld variable: %s", i, scope->local_variables[i]->Variable.name);
    }
}

/// Returns the value, or NULL if not found.
Value* get_variable_in_scope(Scope* scope, char* name)
{
    assert(scope);
    assert(name);
    i64 variable_count = scope->count;
    for (i64 i = 0; i < variable_count; ++i) {
        Value* v = scope->local_variables[i];
        if (v->Variable.name == name) return v;
    }

    return NULL;
}

void add_variable_to_scope(Scope* scope, Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);

    char* name = variable->Variable.name;

    // Check for redeclaration
    Value* res = get_variable_in_scope(scope, name);
    if (res) error("variable %s already exists in current scope.", name);

    // info("Added variable %s", name);
    append_variable_to_scope(scope, variable);
    // print_scope(scope);
}

void remove_variable_in_scope(Scope* scope, char* name)
{
    Value* v = get_variable_in_scope(scope, name);
    if (v) {
        info("removing value: %s", name);
        // Swap this element with the last element;
        Value* temp = scope->local_variables[scope->count];
        scope->local_variables[scope->count] = v;
        v = temp;
        free(v);
        --scope->count;
        return;
    }
    error("Trying to remove unknown variable: %s", name);
}

Value* get_variable(char* name)
{
    STACK_FOREACH(scope_stack)
    {
        Scope* scope = (Scope*)it->data;
        Value* res = get_variable_in_scope(scope, name);
        if (res) return res;
    }
    error("no variable with name '%s'", name);
    return NULL;
}

void add_variable(Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    alloc_variable(variable);
    info("Adding variable: '%s' to scope", variable->Variable.name);
    Scope* top = (Scope*)stack_peek(&scope_stack);
    add_variable_to_scope(top, variable);
}

void remove_variable(Value* variable)
{
    STACK_FOREACH(scope_stack)
    {
        Scope* scope = (Scope*)it->data;
        remove_variable_in_scope(scope, variable->Variable.name);
        return;
    }
}

void push_result_to_temporary_reg(Value* val)
{
    i64 size = get_size_of_value(val);
    if (val->type->kind == TYPESPEC_ARRAY) {
        size = get_size_of_typespec(val->type->Array.type);
    }
    i32 reg_n = get_rax_reg_of_byte_size(size);
    i32 temp_reg_n = get_next_available_reg(&ctx, size);
    emit("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
    function_push_reg(ctx.current_function, temp_reg_n);
}

int pop_result_from_temporary_reg() { return function_pop_reg(ctx.current_function); }

int get_latest_used_temp_reg_in_function()
{
    u8* regs_used = ctx.current_function->Function.regs_used;
    u8 regs_count = ctx.current_function->Function.regs_used_count;
    return regs_used[regs_count];
}

int align(int n, int m) {
    return (m - (n % m)) % m;
}

void emit_store(Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);

    switch (variable->type->kind) {
    case TYPESPEC_POINTER: // fallthrough
    case TYPESPEC_ARRAY: { 
        emit("MOV [RAX], RCX; store1");
    } break;

    default: {
        i64 size = get_size_of_value(variable);
        i32 reg_n = get_rax_reg_of_byte_size(size);
        i64 stack_pos = get_stack_pos_of_variable(variable);
        emit("MOV [RBP-%d], %s; store", stack_pos, get_reg(reg_n));
    } break;
    }
}

void emit_load(Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);

    i64 size = get_size_of_value(variable);
    i32 reg_n = get_rax_reg_of_byte_size(size);
    i64 stack_pos = get_stack_pos_of_variable(variable);

    switch (variable->type->kind) {
    case TYPESPEC_POINTER:
    case TYPESPEC_ARRAY: {
        emit("LEA RAX, [RBP-%d]; load", stack_pos);
    } break;
    default: {
        emit("MOV %s, [RBP-%d]; load", get_reg(reg_n), stack_pos);
    } break;
    }
}

Value* codegen_function(Expr* expr)
{
    assert(expr);
    assert(expr->kind == EXPR_FUNCTION);

    Value* function = make_value_function(expr->Function.type);
    ctx.current_function = function;

    start_codeblock(ctx.current_function, expr_to_str(expr));

    push_scope();

    sb_push(functions, function);

    emit("_%s:", expr->Function.type->Function.name);
    push(RBP);
    emit("MOV RBP, RSP");

    // Allocate stack space. ex variables etc.
    i64 stack_allocated = expr->Function.stack_allocated;
    int padding = align(stack_allocated, 16);
    ctx.current_function->Function.stack_allocated = stack_allocated + padding;
    if (stack_allocated + padding)
        emit("SUB RSP, %d; %d alloc, %d padding", stack_allocated + padding, stack_allocated, padding);

    ctx.stack_index = 0;
    i64 index = 0;

    Arg* args = expr->Function.type->Function.args;
    i32 arg_count = sb_count(args);

    for (int i = 0; i < arg_count; ++i) {
        Arg* arg = &args[i];

        i64 size = get_size_of_typespec(arg->type);
        i64 stack_pos = ctx.stack_index + size;
        Value* var = make_value_variable(arg->name, arg->type, stack_pos);
        add_variable(var);

        char* param_reg = get_reg(get_parameter_reg(index, size));
        emit("MOV [RBP-%lld], %s", stack_pos, param_reg);

        ++index;
    }

    codegen_expr(expr->Function.body);

    pop_scope();

    return function;
}

Value* codegen_int(Expr* expr)
{
    assert(expr->kind == EXPR_INT);
    Value* val = make_value_int(DEFAULT_INTEGER_BYTE_SIZE, integer_literal_type, expr->Int.val);
    i32 reg_n = get_rax_reg_of_byte_size(get_size_of_value(val));
    emit("MOV %s, %d", get_reg(reg_n), val->Int.value);
    return val;
}

Value* codegen_block(Expr* expr)
{
    push_scope();
    Expr** stmts = expr->Block.stmts;
    i64 stmts_count = sb_count(stmts);
    Value* last = NULL;
    for (i64 i = 0; i < stmts_count; ++i) {
        last = codegen_expr(stmts[i]);
    }
    pop_scope();
    return last;
}

Value* codegen_unary(Expr* expr)
{
    Token_Kind op = expr->Unary.op;
    Expr* operand = expr->Unary.operand;

    Value* operand_val = codegen_expr(operand);
    i32 reg_n = get_rax_reg_of_byte_size(get_size_of_value(operand_val));
    Value* result = operand_val;

    switch (op) {
    case THI_SYNTAX_ADDRESS: error("Unary '&' not implemented"); break;
    case THI_SYNTAX_POINTER: {
        assert(operand_val->kind == VALUE_VARIABLE);
        assert( operand_val->type->kind == TYPESPEC_ARRAY ||
                operand_val->type->kind == TYPESPEC_POINTER);
        Typespec* t = operand_val->type->Array.type;
        if (t->kind == TYPESPEC_ARRAY)
        {
            emit("LEA RAX, [RAX]; deref");
        } else {
            emit("MOV RAX, [RAX]; deref");
        }
    } break;
    case TOKEN_BANG: {
        emit("CMP %s, 0", get_reg(reg_n));
        emit("SETE AL");
    } break;
    case TOKEN_PLUS: { // no nothing
    } break;
    case TOKEN_MINUS: {
        emit("NEG %s", get_reg(reg_n));
        break;
    }
    default: error("unhandled unary case: %c", token_kind_to_str(op)); break;
    }
    return result;
}

Value* codegen_binary(Expr* expr)
{
    Token_Kind op = expr->Binary.op;
    Expr* lhs = expr->Binary.lhs;
    Expr* rhs = expr->Binary.rhs;

    switch (op) {

    // Field access
    case TOKEN_DOT: {
        Value* variable = codegen_expr(lhs);
        assert(variable->kind == VALUE_VARIABLE);
        assert(variable->type->kind == TYPESPEC_STRUCT);
        emit_load(variable);
        return variable;
    }
    case THI_SYNTAX_ASSIGNMENT: {
        codegen_expr(rhs);
        push(RAX);
        Value* variable;
        if (lhs->kind == EXPR_UNARY) { 
            variable = codegen_expr(lhs->Unary.operand);
        } else {
            variable = get_variable(lhs->Ident.name);
        }
        pop(RCX);
        emit_store(variable);
        return variable;
    }
    case TOKEN_PLUS: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("ADD RAX, RCX");
        return lhs_v;
    }
    case TOKEN_MINUS: {
        codegen_expr(rhs);
        push(RAX);
        Value* lhs_v = codegen_expr(lhs);
        pop(RCX);
        emit("SUB RAX, RCX");
        return lhs_v;
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

    case TOKEN_AND_AND: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, 0");
        emit("SETNE CL");
        emit("CMP RAX, 0");
        emit("SETNE AL");
        emit("AND CL, AL");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("OR RCX, RAX");
        emit("SETNE AL");
        return lhs_v;
    }

    case TOKEN_LT: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        emit("SETL AL");
        return lhs_v;
    }

    case TOKEN_GT: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        emit("SETG AL");
        return lhs_v;
    }

    case TOKEN_LT_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        emit("SETLE AL");
        return lhs_v;
    }

    case TOKEN_GT_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        emit("SETGE AL");
        return lhs_v;
    }

    case TOKEN_EQ_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        emit("SETE AL");
        return lhs_v;
    }

    case TOKEN_BANG_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("CMP RCX, RAX");
        emit("SETNE AL");
        return lhs_v;
    }

    case TOKEN_PLUS_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        if (lhs_v->kind != VALUE_VARIABLE) error("lhs of += must be a variable.");

        Value* rhs_val = codegen_expr(rhs);
        i64 rhs_size = get_size_of_value(rhs_val);
        i64 reg_n = get_rax_reg_of_byte_size(rhs_size);
        i64 stack_pos = get_stack_pos_of_variable(lhs_v);
        emit("ADD [RBP-%d], %s", stack_pos, get_reg(reg_n));
        return lhs_v;
    }

    case TOKEN_MINUS_EQ: {
        Value* rhs_v = codegen_expr(rhs);
        push_result_to_temporary_reg(rhs_v);

        Value* lhs_v = codegen_expr(lhs);
        if (lhs_v->kind != VALUE_VARIABLE) error("lhs of -= must be a variable.");
        i32 rhs_r = pop_result_from_temporary_reg();
        i32 lhs_r = get_rax_reg_of_byte_size(get_size_of_value(lhs_v));

        emit("SUB %s, %s", get_reg(lhs_r), get_reg(rhs_r));
        emit_store(lhs_v);
        return lhs_v;
    }

    case TOKEN_ASTERISK_EQ: {
        if (lhs->kind == EXPR_UNARY) lhs = lhs->Unary.operand;
        rhs = make_expr_binary(TOKEN_ASTERISK, lhs, rhs);
        codegen_expr(rhs);
        push(RAX);
        Value* variable = codegen_expr(lhs);
        pop(RAX);
        emit_store(variable);
        return variable;
    }
    case TOKEN_PERCENT_EQ: error("percent_eq not implemented.");
    case TOKEN_PIPE_EQ: error("pipe_eq not implemented.");
    case TOKEN_HAT_EQ: error("hat_eq not implemented.");
    case TOKEN_BITWISE_LEFTSHIFT: error("bitwise_leftshift not implemented.");
    case TOKEN_BITWISE_RIGHTSHIFT: error("bitwise_rightshift not implemented.");
    case TOKEN_FWSLASH_EQ: {
        Value* rhs_v = codegen_expr(rhs);
        push_result_to_temporary_reg(rhs_v);

        Value* lhs_v = codegen_expr(lhs);
        if (lhs_v->kind != VALUE_VARIABLE) error("lhs of /= must be a variable.");

        i32 rhs_r = pop_result_from_temporary_reg();

        emit("CDQ");
        emit("IDIV %s", get_reg(rhs_r));
        emit_store(lhs_v);
        return lhs_v;
    }

    case TOKEN_HAT: error("hat not implemented.");
    case TOKEN_PIPE:
        error("pipe not implemented.");

        /* Ternary operator */
        /*
            // CODEGEN LHS
            cmp     edi, dword ptr [rbp - 24]
            jne     L01

            // CODEGEN RHS
            mov     eax, dword ptr [rbp - 8]
            mov     dword ptr [rbp - 28], eax # 4-byte Spill

            // JMP TO CONTINUE
            jmp     CONTINUE
    L01:
            mov     eax, 1
            mov     dword ptr [rbp - 28], eax # 4-byte Spill
            jmp     CONTINUE
    CONTINUE:
            mov     eax, dword ptr [rbp - 28] # 4-byte Reload
            mov     dword ptr [rbp - 20], eax
            mov     eax, dword ptr [rbp -
        */

    case TOKEN_QUESTION_MARK: {
        ctx_push_label(&ctx);
        emit("CMP %s, 0", get_reg_fitting_value(codegen_expr(lhs)));
        emit("JE %s", ctx.temp_label0 = ctx_get_unique_label(&ctx));
        ctx.temp_label1 = ctx_get_unique_label(&ctx);
        Value* rhs_val = codegen_expr(rhs);
        emit("JMP %s", ctx.temp_label1);
        return rhs_val;
    }

    // ex. cond ? expr : expr
    // This comes before '?'
    case TOKEN_COLON: {
        codegen_expr(lhs); // '?' part
        emit("%s:", ctx.temp_label0);
        Value* rhs_val = codegen_expr(rhs);
        emit("%s:", ctx.temp_label1);
        ctx_pop_label(&ctx);
        return rhs_val;
    }
    }

    error("Codegen: Unhandled binary op %s", token_kind_to_str(op));
    return NULL;
}

Value* codegen_variable_decl(Expr* expr)
{
    assert(expr);
    assert(expr->kind == EXPR_VARIABLE_DECL);
    char* name = expr->Variable_Decl.name;
    Typespec* type = expr->Variable_Decl.type;
    Expr* assignment_expr = expr->Variable_Decl.value;

    if (assignment_expr) codegen_expr(assignment_expr); // Any value this creates is stored in RAX

    i64 type_size = get_size_of_typespec(type);
    i64 stack_pos = type_size + ctx.stack_index;
    Value* variable = make_value_variable(name, type, stack_pos);
    add_variable(variable);

    if (assignment_expr && type->kind != TYPESPEC_ARRAY)
        emit_store(variable);

    return variable;
}

Value* codegen_ret(Expr* expr)
{
    Value* ret_val = codegen_expr(expr->Ret.expr);

    // Deallocate any stack used.
    i64 stack_allocated = ctx.current_function->Function.stack_allocated;
    if (stack_allocated) emit("ADD RSP, %lld", stack_allocated);

    pop(RBP);
    emit("RET");

    return ret_val;
}

Value* codegen_call(Expr* expr)
{
    char* callee = expr->Call.callee;
    Expr** args = expr->Call.args;

    Typespec* func_t = get_symbol(callee);

    // push the arguments in reverse order onto the stack
    i32 func_arg_count = typespec_function_get_arg_count(func_t);
    i32 arg_count = sb_count(args);

    if (func_arg_count != arg_count) error("wrong amount of parameters for call to function '%s'", callee);

    for (int i = 0; i < arg_count; ++i) {
        Expr* arg = args[i];
        codegen_expr(arg);
        push(RAX);
    }

    // %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
    for (int i = arg_count-1; i >= 0; --i) {
        switch(i)
        {
            case 0: pop(RDI); break;
            case 1: pop(RSI); break;
            case 2: pop(RDX); break; 
            case 3: pop(RCX); break;
            case 4: pop(R8);  break;
            case 5: pop(R9);  break;
        }
    }

    emit("CALL _%s", callee);

    return make_value_call(callee, func_t->Function.ret_type);
}

Value* codegen_macro(Expr* expr)
{
    assert(expr->kind == EXPR_MACRO);
    return NULL;
}

Value* codegen_ident(Expr* expr)
{
    char* name = expr->Ident.name;
    Expr* macro_expr = get_macro_def(name);
    if (macro_expr) {
        return codegen_expr(macro_expr);
    }
    Value* var = get_variable(name);
    emit_load(var);
    return var;
}

Value* codegen_string(Expr* expr)
{
    assert(expr->kind == EXPR_STRING);
    char* val = expr->String.val;
    emit("MOV RAX, %s", val);
    Typespec* t = make_typespec_string(xstrlen(val));
    return make_value_string(val, t);
}

Value* codegen_note(Expr* expr)
{
    assert(expr->kind == EXPR_NOTE);
    Expr* int_expr = expr->Note.expr;
    assert(int_expr->kind == EXPR_INT);
    i32 integer_value = int_expr->Int.val;
    if (integer_value < 1) error("note parameters start at 1.");
    char* name = ctx.current_function->type->Function.args[integer_value - 1].name;
    Value* var = get_variable(name);
    emit_load(var);
    return var;
}

Value* codegen_struct(Expr* expr)
{
    assert(expr->kind == EXPR_STRUCT);
    warning("struct incomplete?");
    return NULL;
}

// @Hotpath
Value* codegen_expr(Expr* expr)
{
    info("Generating code for: %s", expr_to_str_debug(expr));
    if (expr->kind == EXPR_FUNCTION) return codegen_function(expr);
    if (expr->kind == EXPR_STRUCT) return codegen_struct(expr);
    start_codeblock(ctx.current_function, expr_to_str(expr));
    switch (expr->kind) {
    case EXPR_ASM: emit("%s", expr->Asm.str); return NULL;
    case EXPR_MACRO: return codegen_macro(expr);
    case EXPR_NOTE: return codegen_note(expr);
    case EXPR_INT: return codegen_int(expr);
    case EXPR_FLOAT: error("EXPR_FLOAT codegen not implemented");
    case EXPR_STRING: return codegen_string(expr);
    case EXPR_IDENT: return codegen_ident(expr);
    case EXPR_CALL: return codegen_call(expr);
    case EXPR_UNARY: return codegen_unary(expr);
    case EXPR_BINARY: return codegen_binary(expr);
    case EXPR_RET: return codegen_ret(expr);
    case EXPR_VARIABLE_DECL: return codegen_variable_decl(expr);
    case EXPR_BLOCK: return codegen_block(expr);
    case EXPR_GROUPING: return codegen_expr(expr->Grouping.expr);
    }
    return NULL;
}

char* generate_code_from_ast(List* ast)
{
    info("Generating X64 Assembly from AST");

    integer_literal_type = make_typespec_int(DEFAULT_INTEGER_BIT_SIZE, false);

    ctx_init(&ctx);
    stack_init(&scope_stack);
    output = make_string("");

    List* foreign_function_list = get_foreign_function_list();
    LIST_FOREACH(foreign_function_list)
    {
        char* func_name = ((Typespec*)it->data)->Function.name;
        emit_no_tab(strf("extern _%s", func_name));
    }

    emit_no_tab("section .data");
    List* constant_string_list = get_constant_string_list();
    LIST_FOREACH(constant_string_list)
    {
        char* val = (char*)it->data;
        i64 len = xstrlen(val);
        emit_no_tab(strf("%s: db \"%s\", %lld", val, val, len));
    }

    emit_no_tab("global _main");
    emit_no_tab("section .text\n");

    // Set each EXPR_FUNCTION's stack allocated.
    // gather all allocations
    LIST_FOREACH(ast) { 
        Expr* expr = (Expr*)it->data; 
        if (expr->kind == EXPR_FUNCTION)
        {
            i64 sum = 0;

            // Sum the params
            sum += get_size_of_typespec(expr->Function.type);

            Expr** block = expr->Function.body->Block.stmts;
            i32 block_count = sb_count(block);
            for (int i = 0; i < block_count; ++i)
            {
                Expr* b_expr = block[i];
                switch (b_expr->kind)
                {
                    case EXPR_VARIABLE_DECL: sum += get_size_of_typespec(b_expr->Variable_Decl.type); break;
                }
            }
            expr->Function.stack_allocated = sum;
        }
    }

    // Finaly, codegen the whole thing.
    LIST_FOREACH(ast) { codegen_expr((Expr*)it->data); }

    debug_info_color = RGB_WHITE;
    info("%s", output.c_str);
    debug_info_color = RGB_GRAY;

    ctx_free(&ctx);

    return output.c_str;
}
