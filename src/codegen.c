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

Typespec* integer_literal_type = NULL;
Context ctx;
string output;
Stack scope_stack;
Value** functions = NULL;

Value* codegen_expr(Expr* expr);

void push_s(int reg)
{
    assert(reg >= 0 && reg <= 67);
    emit_s("PUSH %s", get_reg(reg));
    ctx.stack_index += 8;
}

void pop_s(int reg)
{
    assert(reg >= 0 && reg <= 67);
    emit_s("POP %s", get_reg(reg));
    ctx.stack_index -= 8;
    assert(ctx.stack_index >= 0);
}

void push(int reg) { emit(&output, "PUSH %s", get_reg(reg)); }

void pop(int reg) { emit(&output, "POP %s", get_reg(reg)); }

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
void pop_scope()
{
    stack_pop(&scope_stack);
    // Scope* scope = stack_pop(&scope_stack);

    // Free variables
    // for (u64 i = 0; i < scope->count; ++i)
    // {
    // u64 size = get_size_of_value(scope->local_variables[i]);
    // ctx.stack_index -= size;
    // ctx.current_function->Function.stack_allocated -= size;
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
    for (u64 i = 0; i < scope->count; ++i) {
        info("Scope index: %llu variable: %s", i, scope->local_variables[i]->Variable.name);
    }
}

/// Returns the value, or NULL if not found.
Value* get_variable_in_scope(Scope* scope, char* name)
{
    assert(scope);
    assert(name);
    u64 variable_count = scope->count;
    for (u64 i = 0; i < variable_count; ++i) {
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
    // warning("adding variable: %s %llu", variable->Variable.name, variable->Variable.stack_pos);
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
    u64 size = get_size_of_value(val);
    if (val->type->kind == TYPESPEC_ARRAY) {
        size = get_size_of_typespec(val->type->Array.type);
    }
    i32 reg_n = get_rax_reg_of_byte_size(size);
    i32 temp_reg_n = get_next_available_reg(&ctx, size);
    emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
    function_push_reg(ctx.current_function, temp_reg_n);
}

int pop_result_from_temporary_reg() { return function_pop_reg(ctx.current_function); }

int get_latest_used_temp_reg_in_function()
{
    u8* regs_used = ctx.current_function->Function.regs_used;
    u8 regs_count = ctx.current_function->Function.regs_used_count;
    return regs_used[regs_count];
}

void emit_store(Value* variable)
{
    assert(variable->kind == VALUE_VARIABLE);

    u64 size = get_size_of_value(variable);
    i32 reg_n = get_rax_reg_of_byte_size(size);
    u64 stack_pos = get_stack_pos_of_variable(variable);

    switch (variable->type->kind) {
    case TYPESPEC_ARRAY: {
        i32 temp_reg_n = get_push_or_popable_reg(pop_result_from_temporary_reg());
        size = get_size_of_typespec(variable->type->Array.type);
        emit_s("MOV [RBP-%d+%s*%d], %s ; emit_store on '%s'", stack_pos, get_reg(temp_reg_n), size, get_reg(reg_n),
               variable->Variable.name);
        break;
    }
    default: {
        emit_s("MOV [RBP-%d], %s", stack_pos, get_reg(reg_n));
        break;
    }
    }
}

void emit_load(Value* variable)
{
    assert(variable->kind == VALUE_VARIABLE);
    u64 size = get_size_of_value(variable);
    i32 reg_n = get_rax_reg_of_byte_size(size);
    u64 stack_pos = get_stack_pos_of_variable(variable);

    switch (variable->type->kind) {
    case TYPESPEC_ARRAY: {
        size = get_size_of_typespec(variable->type->Array.type);
        // reg_n = get_rax_reg_of_byte_size(size);
        emit_s("MOV %s, [RBP-%d+%s*%d] ; emit_load on '%s'", get_reg(reg_n), stack_pos, get_reg(reg_n), size,
               variable->Variable.name);
        break;
    }
    default: {
        emit_s("MOV %s, [RBP-%d]", get_reg(reg_n), stack_pos);
        break;
    }
    }
}

Value* codegen_function(Expr* expr)
{
    // char* func_name = expr->Function.type->Function.name;
    Value* function = make_value_function(expr->Function.type);
    ctx.current_function = function;

    start_codeblock(ctx.current_function, expr_to_str(expr));

    push_scope();

    sb_push(functions, function);

    u64 index = 0;
    u64 stack_before_func = ctx.stack_index;

    Arg* args = expr->Function.type->Function.args;
    i32 arg_count = sb_count(args);
    if (arg_count) info("Printing function parameters");

    for (int i = 0; i < arg_count; ++i) {
        Arg* arg = &args[i];

        u64 size = get_size_of_typespec(arg->type);
        u64 stack_pos = ctx.stack_index + size;
        Value* var = make_value_variable(arg->name, arg->type, stack_pos);

        add_variable(var);
        emit_s("MOV [RBP-%d], %s", stack_pos, get_reg(get_parameter_reg(index, size)));

        ctx.stack_index += size;
        ++index;
    }

    u64 stack_used = ctx.stack_index - stack_before_func;
    function->Function.stack_allocated += stack_used;
    ctx.stack_index = 0;

    codegen_expr(expr->Function.body);

    pop_scope();

    return function;
}

Value* codegen_int(Expr* expr)
{
    assert(expr->kind == EXPR_INT);
    Value* val = make_value_int(DEFAULT_INTEGER_BYTE_SIZE, integer_literal_type, expr->Int.val);
    i32 reg_n = get_rax_reg_of_byte_size(get_size_of_value(val));
    emit_s("MOV %s, %d", get_reg(reg_n), val->Int.value);
    return val;
}

Value* codegen_block(Expr* expr)
{
    push_scope();
    Expr** stmts = expr->Block.stmts;
    u64 stmts_count = sb_count(stmts);
    Value* last = NULL;
    for (u64 i = 0; i < stmts_count; ++i) {
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
    case THI_SYNTAX_POINTER: error("Unary '*' not implemented"); break;
    case TOKEN_BANG: {
        emit_s("CMP %s, 0", get_reg(reg_n));
        emit_s("SETE AL");
        break;
    }
    case TOKEN_PLUS: {
        // no nothing
    } break;
    case TOKEN_MINUS: {
        emit_s("NEG %s", get_reg(reg_n));
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
        Value* variable = codegen_expr(lhs);
        assert(variable->kind == VALUE_VARIABLE);
        Value* rhs_val = codegen_expr(rhs);
        emit_store(variable);
        return rhs_val;
    }
    case TOKEN_PLUS: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("ADD RAX, RCX");
        return lhs_v;
    }
    case TOKEN_MINUS: {
        codegen_expr(rhs);
        push_s(RAX);
        Value* lhs_v = codegen_expr(lhs);
        pop_s(RCX);
        emit_s("SUB RAX, RCX");
        return lhs_v;
    }
    case TOKEN_ASTERISK: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("IMUL RAX, RCX");
        return lhs_v;
    }
    case TOKEN_FWSLASH: {
        Value* rhs_v = codegen_expr(rhs);
        push_s(RAX);
        codegen_expr(lhs);
        pop_s(RCX);
        emit_s("CDQ");
        emit_s("IDIV RCX");
        return rhs_v;
    }

    case TOKEN_AND_AND: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, 0");
        emit_s("SETNE CL");
        emit_s("CMP RAX, 0");
        emit_s("SETNE AL");
        emit_s("AND CL, AL");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("OR RCX, RAX");
        emit_s("SETNE AL");
        return lhs_v;
    }

    case TOKEN_LT: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, RAX");
        emit_s("SETL AL");
        return lhs_v;
    }

    case TOKEN_GT: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, RAX");
        emit_s("SETG AL");
        return lhs_v;
    }

    case TOKEN_LT_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, RAX");
        emit_s("SETLE AL");
        return lhs_v;
    }

    case TOKEN_GT_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, RAX");
        emit_s("SETGE AL");
        return lhs_v;
    }

    case TOKEN_EQ_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, RAX");
        emit_s("SETE AL");
        return lhs_v;
    }

    case TOKEN_BANG_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        push_s(RAX);
        codegen_expr(rhs);
        pop_s(RCX);
        emit_s("CMP RCX, RAX");
        emit_s("SETNE AL");
        return lhs_v;
    }

    case TOKEN_PLUS_EQ: {
        Value* lhs_v = codegen_expr(lhs);
        if (lhs_v->kind != VALUE_VARIABLE) error("lhs of += must be a variable.");

        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(rhs_size);
        u64 stack_pos = get_stack_pos_of_variable(lhs_v);
        emit_s("ADD [RBP-%d], %s", stack_pos, get_reg(reg_n));

        return lhs_v;
    }

    case TOKEN_MINUS_EQ: {
        Value* rhs_v = codegen_expr(rhs);
        push_result_to_temporary_reg(rhs_v);

        Value* lhs_v = codegen_expr(lhs);
        if (lhs_v->kind != VALUE_VARIABLE) error("lhs of -= must be a variable.");
        i32 rhs_r = pop_result_from_temporary_reg();
        i32 lhs_r = get_rax_reg_of_byte_size(get_size_of_value(lhs_v));

        emit_s("SUB %s, %s", get_reg(lhs_r), get_reg(rhs_r));
        emit_store(lhs_v);
        return lhs_v;
    }

    case TOKEN_ASTERISK_EQ: {
        Value* variable = codegen_expr(lhs);
        if (variable->kind != VALUE_VARIABLE) error("lhs of += must be a variable.");
        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(rhs_size);
        u64 stack_pos = get_stack_pos_of_variable(variable);
        emit_s("IMUL %s, [RBP-%d]", get_reg(reg_n), stack_pos);
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

        emit_s("CDQ");
        emit_s("IDIV %s", get_reg(rhs_r));
        emit_store(lhs_v);
        return lhs_v;
    }

    case TOKEN_AT: error("at not implemented.");
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
        emit_s("CMP %s, 0", get_reg_fitting_value(codegen_expr(lhs)));
        emit_s("JE %s", ctx.temp_label0 = ctx_get_unique_label(&ctx));
        ctx.temp_label1 = ctx_get_unique_label(&ctx);
        Value* rhs_val = codegen_expr(rhs);
        emit_s("JMP %s", ctx.temp_label1);
        return rhs_val;
    }

    // ex. cond ? expr : expr
    // This comes before '?'
    case TOKEN_COLON: {
        codegen_expr(lhs); // '?' part
        emit_s("%s:", ctx.temp_label0);
        Value* rhs_val = codegen_expr(rhs);
        emit_s("%s:", ctx.temp_label1);
        ctx_pop_label(&ctx);
        return rhs_val;
    }
    }

    error("Codegen: Unhandled binary op %s", token_kind_to_str(op));
    return NULL;
}

Value* codegen_variable_decl_type_inf(Expr* expr)
{
    char* name = expr->Variable_Decl_Type_Inf.name;
    Expr* assignment_expr = expr->Variable_Decl_Type_Inf.value;

    Value* assignment_expr_value = codegen_expr(assignment_expr); // Any value this creates is stored in RAX

    Typespec* type = assignment_expr_value->type;
    u64 size_of_assigned_type = get_size_of_typespec(type);
    u64 variable_stack_pos = size_of_assigned_type + ctx.stack_index;
    Value* variable = make_value_variable(name, type, variable_stack_pos);
    add_variable(variable);

    emit_store(variable);
    ctx.stack_index += size_of_assigned_type;
    ctx.current_function->Function.stack_allocated += ctx.stack_index;

    return variable;
}

Value* codegen_variable_decl(Expr* expr)
{
    char* name = expr->Variable_Decl.name;
    Typespec* type = expr->Variable_Decl.type;
    Expr* assignment_expr = expr->Variable_Decl.value;

    if (assignment_expr) codegen_expr(assignment_expr); // Any value this creates is stored in RAX

    u64 type_size = get_size_of_typespec(type);
    u64 stack_pos = type_size + ctx.stack_index;
    Value* variable = make_value_variable(name, type, stack_pos);
    add_variable(variable);

    if (type->kind != TYPESPEC_ARRAY) emit_store(variable); // The variable is set to whatevers in RAX
    ctx.stack_index += type_size;
    ctx.current_function->Function.stack_allocated += ctx.stack_index;

    return variable;
}

Value* codegen_ret(Expr* expr)
{
    Expr* ret_expr = expr->Ret.expr;
    Value* ret_val = codegen_expr(ret_expr);

    // TODO maybe the return value is a struct? so youll have to push the struct by value

    // Pop off regs in reverse order
    // u8 regs_used_total = ctx.current_function->Function.regs_used_total;
    // if (regs_used_total) {
    //     for (int i = regs_used_total - 1; i >= 0; --i) {
    //         switch (i) {
    //         case 0: pop_s(R10); break;
    //         case 1: pop_s(R11); break;
    //         case 2: pop_s(R12); break;
    //         case 3: pop_s(R13); break;
    //         case 4: pop_s(R14); break;
    //         case 5: pop_s(R15); break;
    //         }
    //     }
    // }

    emit_s("LEAVE");
    emit_s("RET");

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

    Value** param_vals = NULL;

    for (int i = 0; i < arg_count; ++i) {
        Expr* arg = args[i];
        Value* val = codegen_expr(arg);
        sb_push(param_vals, val);
        push_s(RAX);
    }

    for (int i = arg_count - 1; i >= 0; --i) {
        Value* val = param_vals[i];
        i32 size = get_size_of_value(val);
        i32 reg_n = get_rax_reg_of_byte_size(size);
        i32 param_reg_n = get_parameter_reg(i, size);

        pop_s(RAX);
        emit_s("MOV %s, %s", get_reg(param_reg_n), get_reg(reg_n));
    }

    sb_free(param_vals);
    emit_s("CALL _%s", callee);
    return make_value_call(callee, func_t->Function.ret_type);
}

Value* codegen_while(Expr* expr)
{
    assert(expr->kind == EXPR_WHILE);
    Expr* condition = expr->While.cond;
    Expr* body = expr->While.body;

    ctx_push_label(&ctx);

    char* condition_label = ctx_get_unique_label(&ctx);
    char* continue_label = ctx_get_unique_label(&ctx);

    ctx_set_break_label(&ctx, continue_label);
    ctx_set_continue_label(&ctx, condition_label);

    // COND:
    emit_s("%s:", condition_label);

    // Compare the iterator to the end value
    codegen_expr(condition);
    emit_s("JE %s", continue_label);

    // BODY
    codegen_expr(body);
    emit_s("JMP %s", condition_label);

    // CONT:
    emit_s("%s:", continue_label);
    ctx_pop_label(&ctx);

    return NULL;
}
Value* codegen_for(Expr* expr)
{
    assert(expr->kind == EXPR_FOR);

    char* iterator_name = expr->For.iterator_name;
    Expr* start = expr->For.start;
    Expr* end = expr->For.end;
    Expr* body = expr->For.body;

    ctx_push_label(&ctx);
    char* condition_label = ctx_get_unique_label(&ctx);
    char* continue_label = ctx_get_unique_label(&ctx);
    char* inc_label = ctx_get_unique_label(&ctx);

    ctx_set_break_label(&ctx, continue_label);
    ctx_set_continue_label(&ctx, inc_label);

    // Setup the iterator variable with the start value.
    Value* start_val = codegen_expr(start);
    i32 type_size = get_size_of_value(start_val);
    i32 stack_pos = type_size + ctx.stack_index;
    Value* iterator_var = make_value_variable(iterator_name, start_val->type, stack_pos);
    add_variable(iterator_var);
    ctx.current_function->Function.stack_allocated += type_size;
    emit_store(iterator_var);
    ctx.stack_index += type_size;

    // COND:
    emit_s("%s:", condition_label);
    i32 res_reg = get_rax_reg_of_byte_size(type_size);

    // Compare the iterator to the end value
    codegen_expr(end);
    emit_s("CMP %s, [RBP-%d]", get_reg(res_reg), iterator_var->Variable.stack_pos);
    emit_s("JE %s", continue_label);

    // BODY
    codegen_expr(body);

    // INC:
    emit_s("%s:", inc_label);

    emit_load(iterator_var);
    emit_s("INC %s [RBP-%d]", get_op_size(type_size), iterator_var->Variable.stack_pos);
    emit_s("JMP %s", condition_label);

    // CONT:
    emit_s("%s:", continue_label);
    ctx_pop_label(&ctx);

    remove_variable(iterator_var);
    ctx.stack_index -= type_size;

    return NULL;
}

Value* codegen_if(Expr* expr)
{
    assert(expr->kind == EXPR_IF);

    Expr* condition = expr->If.cond;
    Expr* then_body = expr->If.then_body;
    Expr* else_body = expr->If.else_body;

    // COND:
    ctx_push_label(&ctx);
    char* continue_label = ctx_get_unique_label(&ctx);
    char* else_label = else_body ? ctx_get_unique_label(&ctx) : NULL;

    Value* condition_val = codegen_expr(condition);
    i32 condition_size = get_size_of_value(condition_val);
    i32 res_reg = get_rax_reg_of_byte_size(condition_size);

    emit_s("CMP %s, 0", get_reg(res_reg));
    emit_s("JE %s", else_body ? else_label : continue_label);

    // THEN
    codegen_expr(then_body);
    emit_s("JMP %s", continue_label);

    // ELSE:
    if (else_body) {
        emit_s("%s:", else_label);
        codegen_expr(else_body);
    }

    // CONT:
    emit_s("%s:", continue_label);

    ctx_pop_label(&ctx);

    return NULL;
}

Value* codegen_macro(Expr* expr)
{
    assert(expr->kind == EXPR_MACRO);
    return NULL;
}

Value* codegen_continue(Expr* expr)
{
    assert(expr->kind == EXPR_CONTINUE);
    emit_s("JMP %s", ctx.label_continue_to);
    return NULL;
}

Value* codegen_break(Expr* expr)
{
    assert(expr->kind == EXPR_BREAK);
    emit_s("JMP %s", ctx.label_break_to);
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

Value* codegen_subscript(Expr* expr)
{
    assert(expr->kind == EXPR_SUBSCRIPT);
    Value* result = codegen_expr(expr->Subscript.expr);
    assert(result->kind == VALUE_INT);
    Value* variable = get_variable(expr->Subscript.variable_name);
    push_result_to_temporary_reg(result);
    emit_load(variable);
    return variable;
}

Value* codegen_string(Expr* expr)
{
    assert(expr->kind == EXPR_STRING);
    char* val = expr->String.val;
    emit_s("MOV RAX, %s", val);
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

    // Typespec* struct_t = expr->Struct.type;

    /* Create the structure of the struct??

         int2 :: struct {
            x: i32
            y: i32
         }
    */
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
    case EXPR_SUBSCRIPT: return codegen_subscript(expr);
    case EXPR_CONTINUE: return codegen_continue(expr);
    case EXPR_BREAK: return codegen_break(expr);
    case EXPR_MACRO: return codegen_macro(expr);
    case EXPR_NOTE: return codegen_note(expr);
    case EXPR_INT: return codegen_int(expr);
    case EXPR_FLOAT: error("EXPR_FLOAT codegen not implemented");
    case EXPR_STRING: return codegen_string(expr);
    case EXPR_IDENT: return codegen_ident(expr);
    case EXPR_CALL: return codegen_call(expr);
    case EXPR_UNARY: return codegen_unary(expr);
    case EXPR_BINARY: return codegen_binary(expr);
    case EXPR_COMPOUND: error("EXPR_COMPOUND codegen not implemented");
    case EXPR_RET: return codegen_ret(expr);
    case EXPR_VARIABLE_DECL: return codegen_variable_decl(expr);
    case EXPR_VARIABLE_DECL_TYPE_INF: return codegen_variable_decl_type_inf(expr);
    case EXPR_IF: return codegen_if(expr);
    case EXPR_FOR: return codegen_for(expr);
    case EXPR_BLOCK: return codegen_block(expr);
    case EXPR_WHILE: return codegen_while(expr);
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
        emit(&output, strf("extern _%s", func_name));
    }

    emit(&output, "section .data");
    List* constant_string_list = get_constant_string_list();
    LIST_FOREACH(constant_string_list)
    {
        char* val = (char*)it->data;
        u64 len = xstrlen(val);
        emit(&output, strf("%s: db \"%s\", %llu", val, val, len));
    }

    emit(&output, "global _main");
    emit(&output, "section .text");

    LIST_FOREACH(ast) { codegen_expr((Expr*)it->data); }

    i32 func_count = sb_count(functions);
    for (int i = 0; i < func_count; ++i) {
        Value* func_v = functions[i];
        char* func_name = func_v->Function.name;

        emit(&output, "_%s:", func_name);

        push(RBP);
        emit(&output, "MOV RBP, RSP");

        u64 stack_allocated = func_v->Function.stack_allocated;
        // Allocate stack space
        if (stack_allocated) {
            emit(&output, "   SUB RSP, %llu", stack_allocated);
            info("function '%s' allocated %d bytes on the stack", func_name, stack_allocated);
        }

        // u8 regs_used_total = func_v->Function.regs_used_total;
        // for (u8 i = 0; i < regs_used_total; ++i) {
        //     switch (i) {
        //     case 0: push(R10); break;
        //     case 1: push(R11); break;
        //     case 2: push(R12); break;
        //     case 3: push(R13); break;
        //     case 4: push(R14); break;
        //     case 5: push(R15); break;
        //     }
        // }

        CodeBlock** codeblocks = func_v->Function.codeblocks;
        i32 cb_count = sb_count(codeblocks);
        for (int j = 0; j < cb_count; ++j) {
            if (codeblocks[j]->block.len) emit(&output, "%s", codeblocks[j]->block.c_str);
        }
    }

    /* Prints out a debug version of the output assembly */
    for (int i = 0; i < func_count; ++i) {
        Value* func_v = functions[i];
        function_print_debug(func_v);
    }

    ctx_free(&ctx);

    return output.c_str;
}
