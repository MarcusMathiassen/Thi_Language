#include "values.h"
#include "utility.h" // error, xmallox
#include <stdarg.h>  // va_list, va_start, va_end
#include <string.h>  // strncat,
#include <stdio.h>  // strncat,
#include "stretchy_buffer.h"  // sb_push

int get_size_of_value(Value* value)
{
    switch (value->kind)
    {
        case VALUE_INT: return value->Int.bytes;
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

void emit(char* output, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    u64 str_len = 1 + vsnprintf(0, NULL, fmt, args);
    va_end(args);
    char* str = xmalloc(str_len);

    va_start(args, fmt);
    vsnprintf(str, str_len, fmt, args);
    va_end(args);
    
    u64 out_len = xstrlen(output); 
    memcpy(output+out_len, str, str_len + 1); // copy the str into ours
    output[out_len + str_len - 1] = '\n'; // append a newline
}

Value* make_value_int(u8 bytes, u64 value)
{
    Value* temp = xmalloc(sizeof(Value));
    temp->kind = VALUE_INT;
    temp->type = NULL;
    temp->Int.bytes = bytes;
    temp->Int.value = value;
    return temp;
}

Value* make_value_function(const char* name)
{
    Value* temp = xmalloc(sizeof(Value));
    temp->kind = VALUE_FUNCTION;
    temp->type = NULL;
    temp->Function.name = name;
    temp->Function.blocks = NULL;
    return temp;
}

Value* make_value_block(Value* function, const char* name)
{
    Value* temp = xmalloc(sizeof(Value));
    temp->kind = VALUE_BLOCK;
    temp->type = NULL;
    temp->Block.parent_function = function;
    temp->Block.name = name;
    temp->Block.lines = NULL;
    return temp;
}
