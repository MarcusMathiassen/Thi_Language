#ifndef VALUES_H
#define VALUES_H

#include "list.h"   // list
#include "string.h" // string
#include "type.h"   // Type

typedef struct Value Value;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_POINTER,
    VALUE_STRING,
    VALUE_VARIABLE,
    VALUE_CALL,
    VALUE_FUNCTION,
    VALUE_STRUCT,
    VALUE_LOAD_INST,
    VALUE_STORE_INST,
} Value_Kind;

//------------------------------------------------------------------------------
//                          Value Structures
//------------------------------------------------------------------------------

struct Value {
    Value_Kind kind;
    Type*      type;
    union {
        struct
        {
            char* name;
            s64   stack_pos;
        } Variable;
        struct
        {
            char* name;
            s64   stack_allocated;
        } Function;
        struct
        {
        } Struct;
        struct
        {
            f64 value;
        } Float;
        struct
        {
            u8  bytes;
            s64 value;
        } Int;
        struct
        {
            char* value;
            s64   len;
        } String;
        struct
        {
            char* callee;
        } Call;
        struct
        {
            Value* variable;
            s64    offset;
        } LoadInst;
        struct
        {
            Value* variable;
            s64    offset;
        } StoreInst;
    };
};

char* value_kind_to_str(Value_Kind kind);

Value*
make_value_load_inst(Value* variable, s64 offset);
Value*
make_value_store_inst(Value* variable, s64 offset);
Value*
make_value_int(u8 bytes, Type* type, s64 value);
Value*
make_value_float(Type* type, f64 value);
Value*
make_value_string(char* value, Type* type);
Value*
make_value_variable(char* name, Type* type, s64 stack_pos);
Value*
make_value_call(char* callee, Type* type);
Value*
make_value_function(Type* type);
Value*
make_value_struct(Type* type);

s64 get_size_of_value(Value* value);
s64 get_stack_pos_of_variable(Value* variable);

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------
typedef struct
{
    List* local_variables;
} Scope;
Scope*
make_scope();

#endif
