// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "value.h"
#include "register.h" // error, xmallox
#include "typedefs.h"
#include "utility.h" // error, xmallox
  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   //
#include <string.h>  // strncat,

//------------------------------------------------------------------------------
//                               value.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* value_kind_to_str(Value_Kind kind) {
    TASSERT_KIND_IN_RANGE(VALUE, kind);
    switch (kind) {
        ERROR_UNHANDLED_KIND(strf("kind = %d", kind));
    case VALUE_INT: return "VALUE_INT";
    case VALUE_FLOAT: return "VALUE_FLOAT";
    case VALUE_POINTER: return "VALUE_POINTER";
    case VALUE_STRING: return "VALUE_STRING";
    case VALUE_VARIABLE: return "VALUE_VARIABLE";
    case VALUE_GLOBAL_VARIABLE: return "VALUE_GLOBAL_VARIABLE";
    case VALUE_CALL: return "VALUE_CALL";
    case VALUE_FUNCTION: return "VALUE_FUNCTION";
    case VALUE_STRUCT: return "VALUE_STRUCT";
    case VALUE_LOAD_INST: return "VALUE_LOAD_INST";
    case VALUE_STORE_INST: return "VALUE_STORE_INST";
    }
    UNREACHABLE;
    return NULL;
}

char* value_to_str(Value* value) {
    UNFINISHED;
    return NULL;
}

char* get_value_name(Value* value) {
    xassert(value);
    Value_Kind kind = value->kind;
    TASSERT_KIND_IN_RANGE(VALUE, kind);
    switch (kind) {
        ERROR_UNHANDLED_VALUE_KIND(kind);
    // case VALUE_INT: return "VALUE_INT";
    // case VALUE_FLOAT: return "VALUE_FLOAT";
    // case VALUE_POINTER: return "VALUE_POINTER";
    // case VALUE_STRING: return "VALUE_STRING";
    case VALUE_VARIABLE: return value->Variable.name;
    case VALUE_GLOBAL_VARIABLE: return value->Global_Variable.name;
    case VALUE_CALL: return value->Call.callee;
    case VALUE_FUNCTION:
        return value->Function.name;
        // case VALUE_STRUCT:
        // case VALUE_LOAD_INST: return "VALUE_LOAD_INST";
        // case VALUE_STORE_INST: return "VALUE_STORE_INST";
    }
    UNREACHABLE;
    return NULL;
}

char* get_mem_loc(Value* value) {
    xassert(value);
    Value_Kind kind = value->kind;
    TASSERT_KIND_IN_RANGE(VALUE, kind);
    switch (kind) {
        ERROR_UNHANDLED_VALUE_KIND(kind);
    // case VALUE_INT: return "VALUE_INT";
    // case VALUE_FLOAT: return "VALUE_FLOAT";
    // case VALUE_POINTER: return "VALUE_POINTER";
    // case VALUE_STRING: return "VALUE_STRING";
    case VALUE_VARIABLE: return strf("[rbp-%lld]", value->Variable.stack_pos);
    case VALUE_GLOBAL_VARIABLE: {
        // @Audit
        if (value->type->kind == TYPE_POINTER)
            return strf("%s", value->Global_Variable.label);
        return strf("[rel %s]", value->Global_Variable.label);
    }
        // case VALUE_CALL: return value->Call.callee;
        // case VALUE_FUNCTION: return value->Function.name;
        // case VALUE_STRUCT:
        // case VALUE_LOAD_INST: return "VALUE_LOAD_INST";
        // case VALUE_STORE_INST: return "VALUE_STORE_INST";
    }
    UNREACHABLE;
    return NULL;
}

char* get_literal_value(Value* value) {
    xassert(value);
    Value_Kind kind = value->kind;
    TASSERT_KIND_IN_RANGE(VALUE, kind);
    switch (kind) {
    ERROR_UNHANDLED_VALUE_KIND(kind);
    case VALUE_INT: return strf("%lld", value->Int.value);
    case VALUE_FLOAT: return strf("%f", value->Float.value);
    case VALUE_STRING: return strf("`%s`", value->String.value);
    // case AST_ARRAY:
    // case AST_CHAR:
    }
    UNREACHABLE;
    return NULL;
}


s64 get_size_of_value(Value* value) {
    xassert(value);
    Value_Kind kind = value->kind;
    TASSERT_KIND_IN_RANGE(VALUE, kind);
    switch (kind) {
        ERROR_UNHANDLED_VALUE_KIND(kind);
    case VALUE_FLOAT: return get_size_of_type(value->type);
    case VALUE_INT: return get_size_of_type(value->type);
    case VALUE_STRING: return value->String.len;
    case VALUE_VARIABLE: return get_size_of_type(value->type);
    case VALUE_GLOBAL_VARIABLE: return get_size_of_type(value->type);
    case VALUE_FUNCTION: error("Asking for the size of a function? Why?");
    case VALUE_STRUCT: return get_size_of_type(value->type);
    case VALUE_CALL: return get_size_of_type(value->type);
    case VALUE_LOAD_INST: return get_size_of_type(value->LoadInst.variable->type);
    case VALUE_STORE_INST: return get_size_of_type(value->StoreInst.variable->type);
    }
    return get_size_of_type(value->type);
}

//------------------------------------------------------------------------------
//                               Value Maker Functions
//------------------------------------------------------------------------------

Value* make_value(Value_Kind kind) {
    Value* v = xmalloc(sizeof(Value));
    v->kind = kind;
    v->flags = 0;
    return v;
}

Value* make_value_load_inst(Value* variable, s64 offset) {
    xassert(variable);
    Value* v = make_value(VALUE_LOAD_INST);
    v->LoadInst.variable = variable;
    v->type = NULL;
    v->LoadInst.offset = offset;
    return v;
}

Value* make_value_store_inst(Value* variable, s64 offset) {
    xassert(variable);
    Value* v = make_value(VALUE_STORE_INST);
    v->StoreInst.variable = variable;
    v->type = NULL;
    v->StoreInst.offset = offset;
    return v;
}

Value* make_value_int(u8 bytes, Type* type, s64 value) {
    xassert(bytes > 0 && bytes < 9);
    xassert(type);
    Value* v = make_value(VALUE_INT);
    v->type = type;
    v->Int.bytes = bytes;
    v->Int.value = value;
    return v;
}

Value* make_value_float(Type* type, f64 value) {
    xassert(type);
    Value* v = make_value(VALUE_FLOAT);
    v->type = type;
    v->Float.value = value;
    return v;
}

Value* make_value_string(char* value, Type* type) {
    xassert(value);
    xassert(type);
    Value* v = make_value(VALUE_STRING);
    v->type = type;
    v->String.value = value;
    v->String.len = strlen(value);
    return v;
}

Value* make_value_variable(char* name, Type* type, s64 stack_pos) {
    xassert(name);
    xassert(type);
    xassert(stack_pos >= 0);
    Value* v = make_value(VALUE_VARIABLE);
    v->Variable.name = name;
    v->type = type;
    v->Variable.stack_pos = stack_pos;
    return v;
}

Value* make_value_global_variable(char* name, Type* type, char* label) {
    xassert(name);
    xassert(type);
    xassert(label);
    Value* v = make_value(VALUE_GLOBAL_VARIABLE);
    v->Global_Variable.name = name;
    v->type = type;
    v->Global_Variable.label = label;
    return v;
}

Value* make_value_call(char* callee, Type* type) {
    xassert(callee);
    xassert(type);
    Value* v = make_value(VALUE_CALL);
    v->type = type;
    v->Call.callee = callee;
    return v;
}

Value* make_value_function(Type* type) {
    xassert(type);
    xassert(type->kind == TYPE_FUNCTION);

    Value* v = make_value(VALUE_FUNCTION);
    v->type = type;
    v->Function.name = type->Function.name;
    v->Function.stack_allocated = 0;
    return v;
}
Value* make_value_struct(Type* type) {
    xassert(type);
    xassert(type->kind == TYPE_STRUCT);
    Value* v = make_value(VALUE_STRUCT);
    v->type = type;
    return v;
}

//------------------------------------------------------------------------------
//                               Value Helper Functions
//------------------------------------------------------------------------------

s64 get_stack_pos_of_variable(Value* variable) {
    switch (variable->kind) {
        ERROR_UNHANDLED_VALUE_KIND(variable->kind);
    case VALUE_LOAD_INST: return get_stack_pos_of_variable(variable->LoadInst.variable);
    case VALUE_VARIABLE: return variable->Variable.stack_pos;
    }
    return 0;
}
