#include "value.h"
#include "utility.h" // error, xmallox
#include <stdarg.h>  // va_list, va_start, va_end
#include <string.h>  // strncat,
#include <stdio.h>  // strncat,
#include "stretchy_buffer.h"  // sb_push
#include <assert.h>  // assert

//------------------------------------------------------------------------------
//                               value.c
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

u64 get_size_of_value(Value* value)
{
    assert(value);
    switch (value->kind)
    {
        case VALUE_INT: return value->Int.bytes;
        case VALUE_VARIABLE: return get_size_of_typespec(value->type);
        case VALUE_FUNCTION: error("Asking for the size of a function? Why?");
        case VALUE_BLOCK: error("Asking for the size of a function? Why?");
    }
    error("get_size_of_value no valid size found");
    return 0;
}


void debug_push_new_instr_group(const char* desc)
{
    error("debug_push_new_instr_group not implemented");
}

void emit(string* output, const char* fmt, ...)
{
    assert(output);
    
    va_list args;
    va_start(args, fmt);
    u64 str_len = vsnprintf(0, 0, fmt, args) + 1;  // strlen + 1 for '\n'
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);

    append_string(output, str);
    append_string(output, "\n");
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
Value* make_value_int(u8 bytes, u64 value)
{
    assert(bytes > 0 && bytes < 9);
    Value* v = make_value(VALUE_INT);
    v->type = NULL;
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
Value* make_value_function(const char* name)
{
    assert(name);
    Value* v = make_value(VALUE_FUNCTION);
    v->type = NULL;
    v->Function.name = name;
    v->Function.blocks = NULL;
    return v;
}
Value* make_value_block(Value* function, const char* name)
{
    assert(function);
    assert(name);
    Value* v = make_value(VALUE_BLOCK);
    v->type = NULL;
    v->Block.parent_function = function;
    v->Block.name = name;
    v->Block.lines = NULL;
    return v;
}

//------------------------------------------------------------------------------
//                               Value Helper Functions
//------------------------------------------------------------------------------

u64 get_stack_pos_of_variable(Value* variable)
{
    return variable->Variable.stack_pos;
}
