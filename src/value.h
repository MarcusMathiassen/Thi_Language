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

#ifndef VALUES_H
#define VALUES_H

#include "list.h"   // list
#include "string.h" // string
#include "type.h"   // Type
#include "typedefs.h"

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
    _VALUE_COUNT_,
} Value_Kind;

//------------------------------------------------------------------------------
//                          Value Structures
//------------------------------------------------------------------------------

struct Value {
    Value_Kind kind;
    Type* type;
    union {
        struct
        {
            char* name;
            s64 stack_pos;
        } Variable;
        struct
        {
            char* name;
            s64 stack_allocated;
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
            u8 bytes;
            s64 value;
        } Int;
        struct
        {
            char* value;
            s64 len;
        } String;
        struct
        {
            char* callee;
        } Call;
        struct
        {
            Value* variable;
            s64 offset;
        } LoadInst;
        struct
        {
            Value* variable;
            s64 offset;
        } StoreInst;
    };
};

char* value_to_str(Value* value);
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

#ifdef NDEBUG
#define ERROR_UNHANDLED_VALUE(x)
#define ERROR_UNHANDLED_VALUE_KIND(x)
#else
#define ERROR_UNHANDLED_VALUE(x) ERROR_UNHANDLED_KIND(value_to_str(x))
#define ERROR_UNHANDLED_VALUE_KIND(x) ERROR_UNHANDLED_KIND(value_kind_to_str(x))
#endif

//------------------------------------------------------------------------------
//                               Scope
//------------------------------------------------------------------------------
typedef struct
{
    List* local_variables;
} Scope;
Scope*
make_scope(void);

#endif
