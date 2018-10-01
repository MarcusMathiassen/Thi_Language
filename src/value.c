#include "value.h"
#include "register.h"        // error, xmallox
#include "stretchy_buffer.h" // sb_push
#include "typedefs.h"
#include "utility.h" // error, xmallox
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   // strncat,
#include <string.h>  // strncat,

//------------------------------------------------------------------------------
//                               value.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

static string* current_output = NULL;

u64 get_size_of_value(Value* value)
{
    assert(value);
    switch (value->kind) {
    case VALUE_INT: return value->Int.bytes;
    case VALUE_VARIABLE: return get_size_of_typespec(value->type);
    case VALUE_FUNCTION: error("Asking for the size of a function? Why?");
    }
    error("get_size_of_value no valid size found");
    return 0;
}

void emit_s(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    u64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    append_string(current_output, "   ");
    append_string(current_output, str);
    append_string(current_output, "\n");
}

void emit(string* output, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    u64 str_len = vsnprintf(0, 0, fmt, args) + 1; // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    append_string(output, str);
    append_string(output, "\n");
}

void start_codeblock(Value* function, const char* desc)
{
    assert(function);
    assert(function->kind == VALUE_FUNCTION);
    assert(desc);

    CodeBlock* cb = xmalloc(sizeof(CodeBlock));
    cb->desc = make_string(desc);
    cb->block = make_string("");
    cb->color = get_unique_color();
    sb_push(function->Function.codeblocks, cb);
    current_output = cb->block;
}

//------------------------------------------------------------------------------
//                               Value Maker Functions
//------------------------------------------------------------------------------

Value* make_value(Value_Kind kind)
{
    Value* v = xmalloc(sizeof(Value));
    v->kind = kind;
    return v;
}
Value* make_value_int(u8 bytes, Typespec* type, u64 value)
{
    assert(bytes > 0 && bytes < 9);
    assert(type);
    Value* v = make_value(VALUE_INT);
    v->type = type;
    v->Int.bytes = bytes;
    v->Int.value = value;
    return v;
}

Value* make_value_variable(const char* name, Typespec* type, u64 stack_pos)
{
    assert(name);
    assert(type);
    assert(stack_pos >= 0);
    Value* v = make_value(VALUE_VARIABLE);
    v->Variable.name = name;
    v->type = type;
    v->Variable.stack_pos = stack_pos;
    return v;
}

Value* make_value_call(const char* callee, Typespec* type)
{
    assert(callee);
    assert(type);
    Value* v = make_value(VALUE_CALL);
    v->type = type;
    v->Call.callee = callee;
    return v;
}
Value* make_value_function(Typespec* type)
{
    assert(type);
    assert(type->kind == TYPESPEC_FUNCTION);

    Value* v = make_value(VALUE_FUNCTION);
    v->type = type;
    v->Function.name = type->Function.name;
    v->Function.codeblocks = NULL;
    v->Function.stack_allocated = 0;
    v->Function.regs_used_count = 0;
    return v;
}

//------------------------------------------------------------------------------
//                               Value Helper Functions
//------------------------------------------------------------------------------

u64 get_stack_pos_of_variable(Value* variable) { return variable->Variable.stack_pos; }

void function_print_debug(Value* function)
{
    assert(function);
    assert(function->kind == VALUE_FUNCTION);

    CodeBlock** codeblocks = function->Function.codeblocks;
    int cb_count = sb_count(codeblocks);

    const char* cb_0_c = codeblocks[0]->color;
    info("%s%s: \033[00m", cb_0_c, function->Function.name);
    // Save off any scrap regs used
    u8* regs_used = function->Function.regs_used;
    u8 regs_count = function->Function.regs_used_count;
    for (u8 k = 0; k < regs_count; ++k) {
        int reg_n = get_push_or_popable_reg(regs_used[k]);
        info("   %sPUSH %s\033[00m", cb_0_c, get_reg(reg_n));
    }

    // Allocate stack space
    u64 stack_allocated = function->Function.stack_allocated;
    if (stack_allocated) info("   %sSUB RSP, %d\033[00m", cb_0_c, stack_allocated);

    for (int j = 0; j < cb_count; ++j) {
        CodeBlock* cb = codeblocks[j];
        const char* cb_c = cb->color;

        // Print line
        if (cb->block->len) {
            const char* line = cb->block->c_str;
            info("%s%s\033[00m", cb_c, line);
        }
    }
}

void function_get_stack_used(Value* function)
{
    assert(function);
    assert(function->kind == VALUE_FUNCTION);
}

void function_push_reg(Value* function, u64 reg_n)
{
    assert(function);
    assert(function->kind == VALUE_FUNCTION);
    u64 count = function->Function.regs_used_count;
    function->Function.regs_used[count] = reg_n;
    ++function->Function.regs_used_count;
}

u64 function_pop_reg(Value* function)
{
    assert(function);
    assert(function->kind == VALUE_FUNCTION);
    u64 count = function->Function.regs_used_count--;
    return function->Function.regs_used[count];
}

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------

Scope* make_scope(u64 pre_allocated_variable_count)
{
    Scope* s = xmalloc(sizeof(Scope));
    s->local_variables = xmalloc(sizeof(Value*) * pre_allocated_variable_count);
    s->count = 0;
    s->alloc_count = pre_allocated_variable_count;
    return s;
}
