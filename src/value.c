#include "value.h"
#include "register.h" // error, xmallox
#include "typedefs.h"
#include "utility.h" // error, xmallox
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <string.h>  // strncat,

//------------------------------------------------------------------------------
//                               value.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

s64 get_size_of_value(Value* value) {
    assert(value);
    switch (value->kind) {
    case VALUE_FLOAT: return get_size_of_type(value->type);
    case VALUE_INT: return get_size_of_type(value->type);
    case VALUE_STRING: return value->String.len;
    case VALUE_VARIABLE: return get_size_of_type(value->type);
    case VALUE_FUNCTION: error("Asking for the size of a function? Why?");
    case VALUE_STRUCT: return get_size_of_type(value->type);
    case VALUE_CALL: return get_size_of_type(value->type);
    case VALUE_LOAD_INST: return get_size_of_type(value->LoadInst.variable->type);
    case VALUE_STORE_INST: return get_size_of_type(value->StoreInst.variable->type);
    default: error("get_size_of_value: unhandled case %d", value->kind);
    }
    return get_size_of_type(value->type);
}

//------------------------------------------------------------------------------
//                               Value Maker Functions
//------------------------------------------------------------------------------

Value* make_value(Value_Kind kind) {
    Value* v = xmalloc(sizeof(Value));
    v->kind  = kind;
    return v;
}

Value* make_value_load_inst(Value* variable, s64 offset) {
    assert(variable);
    Value* v             = make_value(VALUE_LOAD_INST);
    v->LoadInst.variable = variable;
    v->type              = NULL;
    v->LoadInst.offset   = offset;
    return v;
}

Value* make_value_store_inst(Value* variable, s64 offset) {
    assert(variable);
    Value* v              = make_value(VALUE_STORE_INST);
    v->StoreInst.variable = variable;
    v->type               = NULL;
    v->StoreInst.offset   = offset;
    return v;
}

Value* make_value_int(u8 bytes, Type* type, s64 value) {
    assert(bytes > 0 && bytes < 9);
    assert(type);
    Value* v     = make_value(VALUE_INT);
    v->type      = type;
    v->Int.bytes = bytes;
    v->Int.value = value;
    return v;
}

Value* make_value_float(Type* type, f64 value) {
    assert(type);
    Value* v       = make_value(VALUE_FLOAT);
    v->type        = type;
    v->Float.value = value;
    return v;
}

Value* make_value_string(char* value, Type* type) {
    assert(value);
    assert(type);
    Value* v        = make_value(VALUE_STRING);
    v->type         = type;
    v->String.value = value;
    v->String.len   = strlen(value);
    return v;
}

Value* make_value_variable(char* name, Type* type, s64 stack_pos) {
    assert(name);
    assert(type);
    assert(stack_pos >= 0);
    Value* v              = make_value(VALUE_VARIABLE);
    v->Variable.name      = name;
    v->type               = type;
    v->Variable.stack_pos = stack_pos;
    return v;
}

Value* make_value_call(char* callee, Type* type) {
    assert(callee);
    assert(type);
    Value* v       = make_value(VALUE_CALL);
    v->type        = type;
    v->Call.callee = callee;
    return v;
}
Value* make_value_function(Type* type) {
    assert(type);
    assert(type->kind == TYPE_FUNCTION);

    Value* v                    = make_value(VALUE_FUNCTION);
    v->type                     = type;
    v->Function.name            = type->Function.name;
    v->Function.stack_allocated = 0;
    return v;
}
Value* make_value_struct(Type* type) {
    assert(type);
    assert(type->kind == TYPE_STRUCT);
    Value* v = make_value(VALUE_STRUCT);
    v->type  = type;
    return v;
}

//------------------------------------------------------------------------------
//                               Value Helper Functions
//------------------------------------------------------------------------------

s64 get_stack_pos_of_variable(Value* variable) {
    switch (variable->kind) {
    case VALUE_LOAD_INST: return get_stack_pos_of_variable(variable->LoadInst.variable);
    case VALUE_VARIABLE: return variable->Variable.stack_pos;
    }
    return 0;
}

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------

Scope* make_scope() {
    Scope* s           = xmalloc(sizeof(Scope));
    s->local_variables = make_list();
    return s;
}
