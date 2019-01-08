#include "codegen.h"
#include "ast.h"
#include "context.h" // Context
#include "globals.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "string.h"          // string
#include "typedefs.h"
#include "utility.h" // error warning info, etc
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <string.h>  // strncat,
#include <stdlib.h>  // free, malloc

Typespec* integer_literal_type = NULL;
Context ctx;
string output;
Stack scope_stack;

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
    List* stmts = expr->Block.stmts;
    Value* last = NULL;
    LIST_FOREACH(stmts) {
        Expr* stmt = (Expr*)it->data;
        last = codegen_expr(stmt);
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
        rhs = make_expr_unary(TOKEN_MINUS, rhs);
        expr = make_expr_binary(TOKEN_PLUS, lhs, rhs);
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
        expr = make_expr_binary(TOKEN_FWSLASH, rhs, lhs);
        Value* variable = codegen_expr(expr);
        emit("MOV RAX, RDX");
        return variable;
    }
    case TOKEN_PIPE: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("OR CL, AL");
        return lhs_v;
    }
    case TOKEN_AND: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("AND CL, AL");
        return lhs_v;
    }
    case TOKEN_HAT: {
        Value* lhs_v = codegen_expr(lhs);
        push(RAX);
        codegen_expr(rhs);
        pop(RCX);
        emit("XOR CL, AL");
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
        emit("AND CL, AL");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        expr = make_expr_binary(TOKEN_PIPE, lhs, rhs);
        Value* v = codegen_expr(expr);
        emit("SETNE AL");
        return v;
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
    case TOKEN_LT_LT_EQ: {
        rhs = make_expr_binary(TOKEN_LT_LT, lhs, rhs);
        expr = make_expr_unary(TOKEN_EQ, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_GT_GT_EQ: {
        rhs = make_expr_binary(TOKEN_GT_GT, lhs, rhs);
        expr = make_expr_unary(TOKEN_EQ, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_BANG_EQ: {
        rhs = make_expr_binary(TOKEN_EQ_EQ, lhs, rhs);
        expr = make_expr_unary(TOKEN_BANG, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_PLUS_EQ: {
        rhs = make_expr_binary(TOKEN_PLUS, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_MINUS_EQ: {
        rhs = make_expr_binary(TOKEN_MINUS, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_ASTERISK_EQ: {
        rhs = make_expr_binary(TOKEN_ASTERISK, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_FWSLASH_EQ: {
        rhs = make_expr_binary(TOKEN_FWSLASH, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_PERCENT_EQ: {
        rhs = make_expr_binary(TOKEN_PERCENT, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_PIPE_EQ: {
        rhs = make_expr_binary(TOKEN_PIPE, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_HAT_EQ: {
        rhs = make_expr_binary(TOKEN_HAT, lhs, rhs);
        expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_expr(expr);
        return variable;
    }
    case TOKEN_QUESTION_MARK: {
        ctx_push_label(&ctx);
        emit("CMP %s, 0", get_reg_fitting_value(codegen_expr(lhs)));
        emit("JE %s", ctx.temp_label0 = ctx_get_unique_label(&ctx));
        ctx.temp_label1 = ctx_get_unique_label(&ctx);
        Value* rhs_val = codegen_expr(rhs);
        emit("JMP %s", ctx.temp_label1);
        return rhs_val;
    }
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

Value* codegen_call(Expr* expr)
{
    char* callee = expr->Call.callee;
    List* args = expr->Call.args;

    Typespec* func_t = get_symbol(callee);

    // push the arguments in reverse order onto the stack
    i32 func_arg_count = typespec_function_get_arg_count(func_t);
    i32 arg_count = args->count;

    if (func_arg_count != arg_count) error("wrong amount of parameters for call to function '%s'", callee);

    LIST_FOREACH(args) {
        Expr* arg = (Expr*)it->data;
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
    char* name = ((Arg*)list_at(ctx.current_function->type->Function.args, integer_value - 1))->name;
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

i64 get_all_alloca_in_block(Expr* block)
{
    i64 sum = 0;
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

Value* codegen_function(Expr* expr)
{
    assert(expr);
    assert(expr->kind == EXPR_FUNCTION);

    char* func_name = expr->Function.type->Function.name;
    Typespec* func_type = expr->Function.type;
    Expr* func_body = expr->Function.body;

    Value* function = make_value_function(func_type);
    ctx.current_function = function;

    push_scope();

    emit("_%s:", func_name);
    push(RBP);
    emit("MOV RBP, RSP");

    // Sum the params
    i64 sum = get_size_of_typespec(expr->Function.type);

    sum += get_all_alloca_in_block(func_body);

    i64 stack_allocated = sum;
    int padding = (X64_ASM_OSX_STACK_PADDING - (stack_allocated % X64_ASM_OSX_STACK_PADDING)) % X64_ASM_OSX_STACK_PADDING;
    if (stack_allocated + padding)
        emit("SUB RSP, %d; %d alloc, %d padding", stack_allocated + padding, stack_allocated, padding);

    emit(DEFAULT_FUNCTION_ENTRY_LABEL_NAME);

    ctx.stack_index = 0;

    List* args = func_type->Function.args;
    i64 i = 0;
    LIST_FOREACH(args) {
        Arg* arg = (Arg*)it->data;

        i64 size = get_size_of_typespec(arg->type);
        i64 stack_pos = ctx.stack_index + size;
        Value* var = make_value_variable(arg->name, arg->type, stack_pos);
        add_variable(var);

        char* param_reg = get_reg(get_parameter_reg(i, size));
        emit("MOV [RBP-%lld], %s", stack_pos, param_reg);

        i += 1;
    }

    codegen_expr(func_body);

    emit(DEFAULT_FUNCTION_END_LABEL_NAME);
    if (stack_allocated + padding)
        emit("ADD RSP, %d; %d alloc, %d padding", stack_allocated + padding, stack_allocated, padding);

    emit("LEAVE");
    emit("RET");

    pop_scope();

    return function;
}


// @Hotpath
Value* codegen_expr(Expr* expr)
{
    info("Generating code for: %s", wrap_with_colored_parens(expr_to_str(expr)));
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

    // Finaly, codegen the whole thing.
    LIST_FOREACH(ast) { codegen_expr((Expr*)it->data); }

    debug_info_color = RGB_WHITE;
    info("%s", output.c_str);
    debug_info_color = RGB_GRAY;

    ctx_free(&ctx);

    return output.c_str;
}
