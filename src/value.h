#ifndef VALUES_H
#define VALUES_H

#include "list.h"   // list
#include "string.h" // string
#include "type.h"   // Type

typedef struct Value    Value;
typedef enum Value_Kind Value_Kind;

enum Value_Kind {
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
};

//------------------------------------------------------------------------------
//                          Value Structures
//------------------------------------------------------------------------------

typedef struct {
    char *name;
    s64   stack_pos;
} Value_Variable;

typedef struct {
    char *name;
    s64   stack_allocated;
} Value_Function;

typedef struct {
} Value_Struct;
typedef struct {
    f64 value;
} Value_Float;
typedef struct {
    u8  bytes;
    s64 value;
} Value_Int;
typedef struct {
    char *value;
    s64   len;
} Value_String;
typedef struct {
    char *callee;
} Value_Call;
typedef struct {
    Value *variable;
    s64    offset;
} Value_LoadInst;
typedef struct {
    Value *variable;
    s64    offset;
} Value_StoreInst;

struct Value {
    Value_Kind kind;
    Type *     type;
    union {
        Value_Int       Int;
        Value_Float     Float;
        Value_String    String;
        Value_Variable  Variable;
        Value_Call      Call;
        Value_Struct    Struct;
        Value_Function  Function;
        Value_LoadInst  LoadInst;
        Value_StoreInst StoreInst;
    };
};

char *value_kind_to_str(Value_Kind kind);

Value *make_value_load_inst(Value *variable, s64 offset);
Value *make_value_store_inst(Value *variable, s64 offset);
Value *make_value_int(u8 bytes, Type *type, s64 value);
Value *make_value_float(Type *type, f64 value);
Value *make_value_string(char *value, Type *type);
Value *make_value_variable(char *name, Type *type, s64 stack_pos);
Value *make_value_call(char *callee, Type *type);
Value *make_value_function(Type *type);
Value *make_value_struct(Type *type);

s64 get_size_of_value(Value *value);
s64 get_stack_pos_of_variable(Value *variable);

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------
typedef struct {
    List *local_variables;
} Scope;
Scope *make_scope();

#endif
