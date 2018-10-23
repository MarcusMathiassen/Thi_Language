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

static Typespec* integer_literal_type = NULL;
static Context ctx;
static string output;
static Stack scope_stack;
static Value** functions = NULL;

static Value* codegen_expr(Expr* expr);

static char* get_op_size(i8 bytes)
{
    switch (bytes) {
    case 1: return "BYTE";
    case 2: return "WORD";
    case 4: return "DWORD";
    case 8: return "QWORD";
    }
    error("get_op_size unknown byte size: %d", bytes);
    return NULL;
}

static void push_scope()
{
    Scope* new_scope = make_scope(10);
    stack_push(&scope_stack, new_scope);
}
static void pop_scope() { stack_pop(&scope_stack); }

static void append_variable_to_scope(Scope* s, Value* value)
{
    assert(s);
    assert(value);
    if (s->alloc_count < s->count + 1) {
        s->alloc_count += 1;
        s->local_variables = xrealloc(s->local_variables, sizeof(Value*) * s->alloc_count);
    }
    s->local_variables[s->count++] = value;
}

static void print_scope(Scope* scope)
{
    info("Scope count %d", scope->count);
    info("Scope alloc_count %d", scope->alloc_count);
    for (u64 i = 0; i < scope->count; ++i) {
        info("Scope index: %llu variable: %s", i, scope->local_variables[i]->Variable.name);
    }
}

/// Returns the value, or NULL if not found.
static Value* get_variable_in_scope(Scope* scope, const char* name)
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

static void add_variable_to_scope(Scope* scope, Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);

    const char* name = variable->Variable.name;

    // Check for redeclaration
    Value* res = get_variable_in_scope(scope, name);
    if (res) error("variable %s already exists in current scope.", name);

    // info("Added variable %s", name);
    append_variable_to_scope(scope, variable);
    // print_scope(scope);
}

static void remove_variable_in_scope(Scope* scope, const char* name)
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

static Value* get_variable(const char* name)
{
    STACK_FOREACH(scope_stack)
    {
        Scope* scope = (Scope*)it->data;
        Value* res = get_variable_in_scope(scope, name);
        if (res) return res;
    }
    error("no variable with %s", name);
    return NULL;
}

static void add_variable(Value* variable)
{
    assert(variable);
    assert(variable->kind == VALUE_VARIABLE);
    // warning("adding variable: %s %llu", variable->Variable.name, variable->Variable.stack_pos);
    Scope* top = (Scope*)stack_peek(&scope_stack);
    add_variable_to_scope(top, variable);
}

static void remove_variable(Value* variable)
{
    STACK_FOREACH(scope_stack)
    {
        Scope* scope = (Scope*)it->data;
        remove_variable_in_scope(scope, variable->Variable.name);
        return;
    }
}

static void emit_store(Value* variable)
{
    start_codeblock(ctx.current_function, "emit_store");
    assert(variable->kind == VALUE_VARIABLE);
    u64 size = get_size_of_value(variable);
    int reg_n = get_rax_reg_of_byte_size(size);
    u64 stack_pos = get_stack_pos_of_variable(variable);
    emit_s("MOV [RSP-%d], %s", stack_pos, get_reg(reg_n));
}

static void emit_load(Value* value)
{
    start_codeblock(ctx.current_function, "emit_load");
    int reg_n = get_rax_reg_of_byte_size(get_size_of_value(value));
    switch (value->kind) {
    case VALUE_INT: {
        emit_s("MOV %s, %d", get_reg(reg_n), value->Int.value);
    } break;
    case VALUE_VARIABLE: {
        emit_s("MOV %s, [RSP-%d]", get_reg(reg_n), get_stack_pos_of_variable(value));
    } break;
    case VALUE_FUNCTION: {
        error("VALUE_FUNCTION EMIT_LOAD NOT IMPLEMENETED");
    } break;
    }
}

static Value* codegen_function(Expr* expr)
{
    // const char* func_name = expr->Function.type->Function.name;
    Value* function = make_value_function(expr->Function.type);
    ctx.current_function = function;

    push_scope();

    start_codeblock(ctx.current_function, "codegen_function");

    sb_push(functions, function);

    u64 index = 0;
    u64 stack_before_func = ctx.stack_index;

    Arg* args = expr->Function.type->Function.args;
    int arg_count = sb_count(args);
    if (arg_count) info("Printing function parameters");

    for (int i = 0; i < arg_count; ++i) {
        Arg* arg = &args[i];

        u64 size = get_size_of_typespec(arg->type);
        u64 stack_pos = ctx.stack_index + size;
        Value* var = make_value_variable(arg->name, arg->type, stack_pos);

        add_variable(var);
        emit_s("MOV [RSP-%d], %s", stack_pos, get_reg(get_parameter_reg(index, size)));

        ctx.stack_index += size;
        ++index;
    }

    u64 stack_used = ctx.stack_index - stack_before_func;
    function->Function.stack_allocated += stack_used;

    codegen_expr(expr->Function.body);

    pop_scope();

    return function;
}

static Value* codegen_ident(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_ident");
    const char* name = expr->Ident.name;
    Value* var = get_variable(name);
    assert(var);
    emit_load(var);
    return var;
}

static Value* codegen_int(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_int");
    Value* val = make_value_int(DEFAULT_INTEGER_BYTE_SIZE, integer_literal_type, expr->Int.val);
    emit_load(val);
    return val;
}

static Value* codegen_block(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_block");
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

static Value* codegen_unary(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_unary");
    Token_Kind op = expr->Unary.op;
    Expr* operand = expr->Unary.operand;

    Value* operand_val = codegen_expr(operand);
    int reg_n = get_rax_reg_of_byte_size(get_size_of_value(operand_val));
    Value* result = operand_val;

    switch (op) {
    case THI_SYNTAX_ADDRESS: error("AST_Unary '*' not implemented"); break;
    case THI_SYNTAX_POINTER: error("AST_Unary '&' not implemented"); break;
    case TOKEN_BANG: {
        emit_s("CMP %s, 0", get_reg(reg_n));
        emit_s("SETE AL");
        break;
    }
    case TOKEN_MINUS: {
        emit_s("NEG %s", get_reg(reg_n));
        break;
    }
    default: error("unhandled unary case: %c", token_kind_to_str(op)); break;
    }
    return result;
}

static Value* codegen_binary(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_binary");
    Token_Kind op = expr->Binary.op;
    Expr* lhs = expr->Binary.lhs;
    Expr* rhs = expr->Binary.rhs;

    switch (op) {
    case TOKEN_EQ: {
        Value* lhs_val = codegen_expr(lhs);
        if (lhs_val->kind != VALUE_VARIABLE) error("lhs of an assignment must be a variable.");
        Value* rhs_val = codegen_expr(rhs);
        Value* variable = get_variable(lhs->Variable_Decl.name);
        emit_store(variable);
        return rhs_val;
    }
    case TOKEN_PLUS: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int res_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(res_n));
        codegen_expr(rhs);
        emit_s("ADD %s, %s", get_reg(res_n), get_reg(temp_reg_n));
        return lhs_val;
    }
    case TOKEN_MINUS: {
        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        int temp_reg_n = get_next_available_reg(rhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        int reg_n = get_rax_reg_of_byte_size(rhs_size);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        Value* lhs_val = codegen_expr(lhs);
        emit_s("SUB %s, %s", get_reg(reg_n), get_reg(temp_reg_n));
        return lhs_val;
    }
    case TOKEN_ASTERISK: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int res_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(res_n));
        codegen_expr(rhs);
        emit_s("IMUL %s, %s", get_reg(res_n), get_reg(temp_reg_n));
        return lhs_val;
    }
    case TOKEN_FWSLASH: {
        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        int reg_n = get_rax_reg_of_byte_size(rhs_size);
        int temp_reg_n = get_next_available_reg(rhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(lhs);
        emit_s("CDQ");
        emit_s("IDIV %s", get_reg(temp_reg_n));
        return rhs_val;
    }

    case TOKEN_AND_AND: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        int temp_lower_byte = get_reg_as_another_size(temp_reg_n, 1);
        emit_s("CMP %s, 0", get_reg(temp_reg_n));
        emit_s("SETNE %s", get_reg(temp_lower_byte));
        emit_s("CMP %s, 0", get_reg(reg_n));
        emit_s("SETNE AL");
        emit_s("AND %s, AL", get_reg(temp_lower_byte));
        return lhs_val;
    }

    case TOKEN_PIPE_PIPE: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("OR %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETNE AL");
        return lhs_val;
    }

    case TOKEN_LT: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("CMP %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETL AL");
        return lhs_val;
    }

    case TOKEN_GT: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("CMP %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETG AL");
        return lhs_val;
    }

    case TOKEN_LT_EQ: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("CMP %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETLE AL");
        return lhs_val;
    }

    case TOKEN_GT_EQ: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("CMP %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETGE AL");
        return lhs_val;
    }

    case TOKEN_EQ_EQ: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("CMP %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETE AL");
        return lhs_val;
    }

    case TOKEN_BANG_EQ: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        int reg_n = get_rax_reg_of_byte_size(lhs_size);
        int temp_reg_n = get_next_available_reg(lhs_size);
        function_push_reg(ctx.current_function, temp_reg_n);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        codegen_expr(rhs);
        emit_s("CMP %s, %s", get_reg(temp_reg_n), get_reg(reg_n));
        emit_s("SETNE AL");
        return lhs_val;
    }

    case TOKEN_PLUS_EQ: {
        Value* variable = codegen_expr(lhs);
        if (variable->kind != VALUE_VARIABLE) error("lhs of += must be a variable.");

        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(rhs_size);
        u64 stack_pos = get_stack_pos_of_variable(variable);
        emit_s("ADD [RSP-%d], %s", stack_pos, get_reg(reg_n));

        return variable;
    }

    case TOKEN_MINUS_EQ: {
        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(rhs_size);

        int temp_reg_n = get_next_available_reg(rhs_size);
        // curr_func->add_used_reg(temp_reg.reg);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));

        Value* variable = codegen_expr(lhs);
        if (variable->kind != VALUE_VARIABLE) error("lhs of -= must be a variable.");

        emit_s("SUB %s, %s", get_reg(reg_n), get_reg(temp_reg_n));
        emit_store(variable);
        return variable;
    }

    case TOKEN_ASTERISK_EQ: {
        Value* variable = codegen_expr(lhs);
        if (variable->kind != VALUE_VARIABLE) error("lhs of += must be a variable.");

        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(rhs_size);
        u64 stack_pos = get_stack_pos_of_variable(variable);
        emit_s("IMUL %s, [RSP-%d]", get_reg(reg_n), stack_pos);
        emit_store(variable);
        return variable;
    }
    case TOKEN_PERCENT_EQ: error("percent_eq not implemented.");
    case TOKEN_PIPE_EQ: error("pipe_eq not implemented.");
    case TOKEN_HAT_EQ: error("hat_eq not implemented.");
    case TOKEN_BITWISE_LEFTSHIFT: error("bitwise_leftshift not implemented.");
    case TOKEN_BITWISE_RIGHTSHIFT: error("bitwise_rightshift not implemented.");
    case TOKEN_FWSLASH_EQ: {
        Value* rhs_val = codegen_expr(rhs);
        u64 rhs_size = get_size_of_value(rhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(rhs_size);

        int temp_reg_n = get_next_available_reg(rhs_size);
        // curr_func->add_used_reg(temp_reg.reg);
        emit_s("MOV %s, %s", get_reg(temp_reg_n), get_reg(reg_n));

        Value* variable = codegen_expr(lhs);
        if (variable->kind != VALUE_VARIABLE) error("lhs of /= must be a variable.");

        emit_s("CDQ");
        emit_s("IDIV %s", get_reg(temp_reg_n));
        emit_store(variable);
        return variable;
    }

    case TOKEN_AT: error("at not implemented.");
    case TOKEN_HAT: error("hat not implemented.");
    case TOKEN_PIPE: error("pipe not implemented.");

    // Ternary operator
    case TOKEN_QUESTION_MARK: {
        Value* lhs_val = codegen_expr(lhs);
        u64 lhs_size = get_size_of_value(lhs_val);
        u64 reg_n = get_rax_reg_of_byte_size(lhs_size);
        emit_s("CMP %s, 0", get_reg(reg_n));
        ctx_pop_label(&ctx);
        emit_s("JE %s", ctx_get_unique_label(&ctx, "E3"));
        Value* rhs_val = codegen_expr(rhs);
        emit_s("JMP %s", ctx_get_unique_label(&ctx, "CONTINUE"));
        return rhs_val;
    }
    case TOKEN_COLON: {
        ctx_push_label(&ctx);
        codegen_expr(lhs);
        emit_s("%s:", ctx_get_unique_label(&ctx, "E3"));
        Value* rhs_val = codegen_expr(rhs);
        emit_s("%s:", ctx_get_unique_label(&ctx, "CONTINUE"));
        return rhs_val;
    }
    }

    error("Codegen: Unhandled binary op %s", token_kind_to_str(op));
    return NULL;
}

static Value* codegen_variable_decl_type_inf(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_variable_decl_type_inf");
    const char* name = expr->Variable_Decl_Type_Inf.name;
    Expr* assignment_expr = expr->Variable_Decl_Type_Inf.value;

    Value* assign_expr_val = codegen_expr(assignment_expr); // Any value this creates is stored in RAX
    Typespec* type = assign_expr_val->type;
    u64 type_size = get_size_of_typespec(type);
    u64 stack_pos = type_size + ctx.stack_index;

    Value* variable = make_value_variable(name, type, stack_pos);
    add_variable(variable);
    emit_store(variable); // The variable is set to whatevers in RAX
    ctx.stack_index += type_size;

    ctx.current_function->Function.stack_allocated += ctx.stack_index;

    return variable;
}
static Value* codegen_variable_decl(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_variable_decl");
    const char* name = expr->Variable_Decl.name;
    Typespec* type = expr->Variable_Decl.type;
    Expr* assignment_expr = expr->Variable_Decl.value;

    if (assignment_expr) codegen_expr(assignment_expr); // Any value this creates is stored in RAX

    u64 type_size = get_size_of_typespec(type);
    u64 stack_pos = type_size + ctx.stack_index;
    Value* variable = make_value_variable(name, type, stack_pos);
    add_variable(variable);

    emit_store(variable); // The variable is set to whatevers in RAX
    ctx.stack_index += type_size;
    ctx.current_function->Function.stack_allocated += ctx.stack_index;

    return variable;
}

static Value* codegen_ret(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_ret");
    Expr* ret_expr = expr->Ret.expr;
    Value* ret_val = codegen_expr(ret_expr);

    // We have to pop off any regs used.
    // Also deallocate any stack used.

    // Pop off regs in reverse order
    u8* regs_used = ctx.current_function->Function.regs_used;
    u8 regs_count = ctx.current_function->Function.regs_used_count;
    if (regs_count)
        for (int i = regs_count - 1; i >= 0; --i) {
            int reg_n = get_push_or_popable_reg(regs_used[i]);
            emit_s("POP %s", get_reg(reg_n));
        }

    // Deallocate stack used by the function
    u64 stack_used = ctx.current_function->Function.stack_allocated;
    if (stack_used) {
        emit_s("ADD RSP, %llu", stack_used);
    }

    emit_s("RET");
    return ret_val;
}

static Value* codegen_call(Expr* expr)
{
    start_codeblock(ctx.current_function, "codegen_call");

    const char* callee = expr->Call.callee;
    Expr** args = expr->Call.args;

    Typespec* func_t = get_symbol(callee);

    // push the arguments in reverse order onto the stack
    int func_arg_count = typespec_function_get_arg_count(func_t);
    int arg_count = sb_count(args);

    if (func_arg_count != arg_count) error("wrong amount of parameters for call to function '%s'", callee);

    int bytes_to_remove = 0;
    Value** param_vals = NULL;

    for (int i = 0; i < arg_count; ++i) {
        Expr* arg = args[i];
        Value* val = codegen_expr(arg);
        sb_push(param_vals, val);
        emit_s("PUSH RAX");
    }

    for (int i = arg_count - 1; i >= 0; --i) {
        Value* val = param_vals[i];
        int size = get_size_of_value(val);
        int reg_n = get_rax_reg_of_byte_size(size);
        int param_reg_n = get_parameter_reg(i, size);

        emit_s("POP RAX");
        emit_s("MOV %s, %s", get_reg(param_reg_n), get_reg(reg_n));

        ctx.stack_index += size;
        bytes_to_remove += size;
    }

    sb_free(param_vals);

    emit_s("CALL %s", callee);
    return make_value_call(callee, func_t->Function.ret_type);
}

static Value* codegen_if(Expr* expr)
{
    assert(expr->kind == EXPR_IF);

    Expr* condition = expr->If.cond;
    Expr* then_body = expr->If.then_body;
    Expr* else_body = expr->If.else_body;

    // COND:
    ctx_push_label(&ctx);
    Value* condition_val = codegen_expr(condition);
    int condition_size = get_size_of_value(condition_val);
    int res_reg = get_rax_reg_of_byte_size(condition_size);

    emit_s("CMP %s, 0", get_reg(res_reg));
    emit_s("JE %s", else_body ? ctx_get_unique_label(&ctx, "ifelse") : ctx_get_unique_label(&ctx, "ifcont"));
    emit_s("JMP %s", ctx_get_unique_label(&ctx, "ifthen"));

    // THEN:
    emit_s("%s:", ctx_get_unique_label(&ctx, "ifthen"));
    codegen_expr(then_body);

    // ELSE:
    if (else_body) {
        emit_s("%s:", ctx_get_unique_label(&ctx, "ifelse"));
        codegen_expr(else_body);
    }

    emit_s("%s:", ctx_get_unique_label(&ctx, "ifcont"));
    ctx_pop_label(&ctx);

    return NULL;
}
static Value* codegen_note(Expr* expr)
{
    assert(expr->kind == EXPR_NOTE);
    start_codeblock(ctx.current_function, "codegen_note");
    Expr* int_expr = expr->Note.expr;
    assert(int_expr->kind == EXPR_INT);
    int integer_value = int_expr->Int.val;
    if (integer_value < 1) error("note parameters start at 1.");
    const char* name = ctx.current_function->type->Function.args[integer_value - 1].name;
    Value* var = get_variable(name);
    emit_load(var);
    return var;
}

// @Hotpath
static Value* codegen_expr(Expr* expr)
{
    info("Generating code for: %s", expr_to_str(expr));
    switch (expr->kind) {
    case EXPR_NOTE: return codegen_note(expr);
    case EXPR_INT: return codegen_int(expr);
    case EXPR_FLOAT: error("EXPR_FLOAT codegen not implemented");
    case EXPR_IDENT: return codegen_ident(expr);
    case EXPR_CALL: return codegen_call(expr);
    case EXPR_UNARY: return codegen_unary(expr);
    case EXPR_BINARY: return codegen_binary(expr);
    case EXPR_COMPOUND: error("EXPR_COMPOUND codegen not implemented");
    case EXPR_RET: return codegen_ret(expr);
    case EXPR_VARIABLE_DECL: return codegen_variable_decl(expr);
    case EXPR_VARIABLE_DECL_TYPE_INF: return codegen_variable_decl_type_inf(expr);
    case EXPR_FUNCTION: return codegen_function(expr);
    case EXPR_STRUCT: error("EXPR_STRUCT codegen not implemented");
    case EXPR_IF: return codegen_if(expr);
    case EXPR_FOR: error("EXPR_FOR codegen not implemented");
    case EXPR_BLOCK: return codegen_block(expr);
    case EXPR_WHILE: error("EXPR_WHILE codegen not implemented");
    case EXPR_GROUPING: return codegen_expr(expr->Grouping.expr);
    }
    return NULL;
}

char* generate_code_from_ast(AST** ast)
{
    info("Generating X64 Assembly from AST");

    integer_literal_type = make_typespec_int(DEFAULT_INTEGER_BIT_SIZE, false);

    ctx_init(&ctx);
    stack_init(&scope_stack);
    output = make_string("");

    emit(&output, "global main");
    emit(&output, "section .text");

    u64 ast_count = sb_count(ast);
    for (u64 i = 0; i < ast_count; ++i) {
        codegen_expr(ast[i]);
    }

    int func_count = sb_count(functions);
    for (int i = 0; i < func_count; ++i) {
        Value* func_v = functions[i];
        const char* func_name = func_v->Function.name;

        emit(&output, "%s:", func_name);

        u8* regs_used = func_v->Function.regs_used;
        u8 regs_count = func_v->Function.regs_used_count;
        for (u8 k = 0; k < regs_count; ++k) {
            int reg_n = get_push_or_popable_reg(regs_used[k]);
            emit(&output, "   PUSH %s", get_reg(reg_n));
        }

        u64 stack_allocated = func_v->Function.stack_allocated;
        // Allocate stack space
        if (stack_allocated) {
            emit(&output, "   SUB RSP, %llu", stack_allocated);
            info("function '%s' allocated %d bytes on the stack", func_name, stack_allocated);
        }

        CodeBlock** codeblocks = func_v->Function.codeblocks;
        int cb_count = sb_count(codeblocks);
        for (int j = 0; j < cb_count; ++j) {
            if (codeblocks[j]->block.len) emit(&output, "%s", codeblocks[j]->block.c_str);
        }
    }

    // for (int i = 0; i < func_count; ++i) {
    //     Value* func_v = functions[i];
    //     function_print_debug(func_v);
    // }

    return output.c_str;
}
