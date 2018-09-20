#ifndef VALUES_H
#define VALUES_H

#include "string.h"   // string
#include "typespec.h" // Typespec

typedef struct Value Value;
typedef enum Value_Kind Value_Kind;

enum Value_Kind {
    VALUE_INT,
    VALUE_VARIABLE,
    VALUE_FUNCTION,
};

typedef struct CodeBlock {
    string* block;
    const char* color;
} CodeBlock;

CodeBlock* make_codeblock(const char* desc, const char* color);
void add_lines_to_codeblock(CodeBlock* cb, const char* line);

//------------------------------------------------------------------------------
//                          Value Structures
//------------------------------------------------------------------------------

typedef struct {
    const char* name;
    u64 stack_pos;
} Variable;

typedef struct {
    const char* name;
    u64 stack_allocated;

    // An array of all scrap regs used by the function.
    // These are pushed at the start of the function and popped at the end.
    u8 regs_used[5];
    u8 regs_used_count;
} Function;

typedef struct {
    u8 bytes;
    u64 value;
} Int;

struct Value {
    Value_Kind kind;
    Typespec* type;
    union {
        Int Int;
        Variable Variable;
        Function Function;
    };
};

Value* make_value_int(u8 bytes, Typespec* type, u64 value);
Value* make_value_variable(const char* name, Typespec* type, u64 stack_pos);
Value* make_value_function(Typespec* type);

u64 get_size_of_value(Value* value);
u64 get_stack_pos_of_variable(Value* variable);

void function_get_stack_used(Value* function);
void function_push_reg(Value* function, u64 reg_n);
u64 function_pop_reg(Value* function);

void emit(string* output, const char* fmt, ...);

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------
typedef struct {
    Value** local_variables;
    u64 count;
    u64 alloc_count;
} Scope;

Scope* make_scope(u64 pre_allocated_variable_count);

#endif
