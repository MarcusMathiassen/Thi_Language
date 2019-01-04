#ifndef VALUES_H
#define VALUES_H

#include "string.h"   // string
#include "typespec.h" // Typespec

typedef struct Value Value;
typedef enum Value_Kind Value_Kind;

enum Value_Kind
{
    VALUE_INT,
    VALUE_STRING,
    VALUE_VARIABLE,
    VALUE_CALL,
    VALUE_FUNCTION,
    VALUE_LOAD_INST,
    VALUE_STORE_INST,
};

typedef struct CodeBlock
{
    string desc;
    string block;
    char* color;
} CodeBlock;

void add_lines_to_codeblock(CodeBlock* cb, char* line);

void start_codeblock(Value* function, char* desc);

//------------------------------------------------------------------------------
//                          Value Structures
//------------------------------------------------------------------------------

typedef struct
{
    char* name;
    u64 stack_pos;
} Value_Variable;

typedef struct
{
    char* name;
    u64 stack_allocated;
    CodeBlock** codeblocks;

    /* There is a maximum of 6 scratch registers available
       for temporary use inside a function. These must be
       pushed to the stack at the prolog and popped off before
       the function returns.
    */
    u8 regs_used[6];
    u8 regs_used_count;
    u8 regs_used_total;
} Value_Function;

typedef struct
{
    u8 bytes;
    u64 value;
} Value_Int;
typedef struct
{
    char* value;
    u64 len;
} Value_String;
typedef struct
{
    char* callee;
} Value_Call;
typedef struct
{
    Value* variable;
    u64 offset;
} Value_LoadInst;
typedef struct
{
    Value* variable;
    u64 offset;
} Value_StoreInst;

struct Value
{
    Value_Kind kind;
    Typespec* type;
    union {
        Value_Int Int;
        Value_String String;
        Value_Variable Variable;
        Value_Call Call;
        Value_Function Function;
        Value_LoadInst LoadInst;
        Value_StoreInst StoreInst;
    };
};

Value* make_value_load_inst(Value* variable, u64 offset);
Value* make_value_store_inst(Value* variable, u64 offset);

Value* make_value_int(u8 bytes, Typespec* type, u64 value);
Value* make_value_string(char* value, Typespec* type);
Value* make_value_variable(char* name, Typespec* type, u64 stack_pos);
Value* make_value_call(char* callee, Typespec* type);
Value* make_value_function(Typespec* type);

u64 get_size_of_value(Value* value);
u64 get_stack_pos_of_variable(Value* variable);

void function_print_debug(Value* function);
void function_get_stack_used(Value* function);
void function_push_reg(Value* function, u64 reg_n);
u64 function_pop_reg(Value* function);

void emit_s(char* fmt, ...);
void emit(string* output, char* fmt, ...);

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------
typedef struct
{
    Value** local_variables;
    u64 count;
    u64 alloc_count;
} Scope;

Scope* make_scope(u64 pre_allocated_variable_count);

#endif
