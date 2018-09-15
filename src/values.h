#ifndef VALUES_H
#define VALUES_H

#include "types.h"
#include <stdarg.h>  // va_list, va_start, va_end

typedef struct Value Value;
typedef enum Value_Kind Value_Kind;


enum Value_Kind
{
    VALUE_INT,
    VALUE_FUNCTION,
    VALUE_BLOCK,
};

typedef struct CodeBlock
{
    const char* color;
    char** lines;
} CodeBlock;

struct Value
{
    Value_Kind kind;
    Type* type;
    union
    {
        struct { u8 bytes; u64 value; } Int;
        struct
        {
            const char* name;
            Value* blocks;
        } Function;
        struct
        { 
            Value* parent_function;
            const char* name;
            CodeBlock* lines;
        } Block;
    };
};

Value make_value_int(u8 bytes, u64 value);
Value make_value_function(const char* name);
Value make_value_block(Value* function, const char* name);

void emit(char* output, const char* fmt, ...);
void debug_push_new_instr_group(const char* desc);
int get_size_of_value(Value* value);

#endif
