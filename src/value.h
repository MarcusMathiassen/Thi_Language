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

//------------------------------------------------------------------------------
//                          Value Structures
//------------------------------------------------------------------------------

typedef struct
{
    char* name;
    i64 stack_pos;
} Value_Variable;

typedef struct
{
    char* name;
    i64 stack_allocated;
} Value_Function;

typedef struct
{
    u8 bytes;
    i64 value;
} Value_Int;
typedef struct
{
    char* value;
    i64 len;
} Value_String;
typedef struct
{
    char* callee;
} Value_Call;
typedef struct
{
    Value* variable;
    i64 offset;
} Value_LoadInst;
typedef struct
{
    Value* variable;
    i64 offset;
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

Value* make_value_load_inst(Value* variable, i64 offset);
Value* make_value_store_inst(Value* variable, i64 offset);

Value* make_value_int(u8 bytes, Typespec* type, i64 value);
Value* make_value_string(char* value, Typespec* type);
Value* make_value_variable(char* name, Typespec* type, i64 stack_pos);
Value* make_value_call(char* callee, Typespec* type);
Value* make_value_function(Typespec* type);

i64 get_size_of_value(Value* value);
i64 get_stack_pos_of_variable(Value* variable);

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------
typedef struct
{
    Value** local_variables;
    i64 count;
    i64 alloc_count;
} Scope;

Scope* make_scope(i64 pre_allocated_variable_count);

#endif
