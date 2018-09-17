#ifndef VALUES_H
#define VALUES_H

#include "typespec.h" // Typespec
#include "string.h" // string

typedef struct Value Value;
typedef enum Value_Kind Value_Kind;


enum Value_Kind
{
    VALUE_INT,
    VALUE_VARIABLE,
    VALUE_FUNCTION,
    VALUE_BLOCK,
};

typedef struct CodeBlock
{
    const char* color;
    char** lines;
} CodeBlock;

CodeBlock* make_codeblock(const char* desc, const char* color);
void add_lines_to_codeblock(CodeBlock* cb, const char* line);

//------------------------------------------------------------------------------
//                          Value Structures                    
//------------------------------------------------------------------------------

typedef struct
{
    Value* parent_function;
    const char* name;
    CodeBlock* lines;
} Block;

typedef struct
{
    const char* name;
    u64 stack_pos;
} Variable;


typedef struct
{
    const char* name;
    Value* blocks;
} Function;

typedef struct
{
    u8 bytes;
    u64 value;
} Int;

struct Value
{
    Value_Kind kind;
    Typespec* type;
    union
    {
        Int Int;
        Variable Variable;
        Function Function;
        Block Block;
    };
};

Value* make_value_int(u8 bytes, Typespec* type, u64 value);
Value* make_value_variable(const char* name, Typespec* type, u64 stack_pos);
Value* make_value_function(const char* name);
Value* make_value_block(Value* function, const char* name);
u64 get_stack_pos_of_variable(Value* variable);

void emit(string* output, const char* fmt, ...);
void debug_push_new_instr_group(const char* desc);
u64 get_size_of_value(Value* value);

#endif
