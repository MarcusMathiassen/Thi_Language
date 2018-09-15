#ifndef VALUES_H
#define VALUES_H

#include "types.h"
#include <stdarg.h>  // va_list, va_start, va_end

typedef struct Value Value;
typedef enum Value_Kind Value_Kind;


enum Value_Kind
{
    VALUE_INT,
    VALUE_RET,
    VALUE_FUNC,
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
        struct { } Int;
        struct { } Ret;
        struct { } Func;
        struct
        { 
            const char* name;
            CodeBlock* lines;
        } Block;
    };
};

void emit(char* output, const char* fmt, ...);
void debug_push_new_instr_group(const char* desc);
int get_size_of_value(Value* value);

#endif
