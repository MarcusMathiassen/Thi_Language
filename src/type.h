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

#ifndef TYPE_H
#define TYPE_H

#include "list.h"   // List
#include "string.h" // String_Context
#include "typedefs.h"

//------------------------------------------------------------------------------
//                               type.h
//------------------------------------------------------------------------------

typedef struct Type Type;

typedef enum {
    _TYPE_NONE_,
    TYPE_UNRESOLVED,
    TYPE_VOID,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_ENUM,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_FUNCTION,
    TYPE_VAR_ARGS,
    _TYPE_COUNT_,
} Type_Kind;

typedef enum {
    TYPE_FLAG_HAS_VAR_ARG = 1 << 0,
    TYPE_FLAG_IMPLICIT_RETURN = 1 << 1,
    TYPE_FLAG_IN_NEED_OF_TYPE_INFERENCE = 1 << 2,
    TYPE_FLAG_QUICK_LAMBDA = 1 << 3,
    TYPE_FLAG_IS_ALIGNED = 1 << 4,
} Type_Flag;

typedef struct {
    char* name;
    Type* type;
} Type_Name_Pair;

struct Type {
    Type_Kind kind;
    u32 flags;
    char* name;
    union {
        struct
        {
        } Var_Args;
        struct
        {
            char* name;
        } Unresolved;
        struct
        {
            s8 bytes;
            s8 is_unsigned;
        } Int;
        struct
        {
            s8 bytes;
        } Float;
        struct
        {
            s64 len;
        } String;
        struct
        {
            Type* pointee;
        } Pointer;
        struct
        {
            Type* type;
            s64 size;
        } Array;
        struct
        {
            char* name;
            List* members;
        } Enum;
        struct
        {
            char* name;
            List* members;
        } Struct;
        struct
        {
            char* name;
            List* parameters;
            Type* return_type;
        } Function;
    };
};

typedef struct
{
    Type** data;
    s64 count;
    s64 allocated;
} Type_Ref_List;

Type_Ref_List make_type_ref_list(void);
void type_ref_list_append(Type_Ref_List* l, Type* t);

bool is_same_type(Type* a, Type* b);

List* type_get_members(Type* type);
s64 get_offset_in_struct_to_field(Type* type, char* name);
s64 get_size_of_underlying_type_if_any(Type* type);
Type* get_underlying_type_if_any(Type* type);
s64 get_size_of_type(Type* Type);
char* get_type_name(Type* Type);

char* type_to_str(Type* type);
char* type_to_str_r(String_Context* ctx, Type* type);
char* type_to_json(Type* type);
char* type_kind_to_str(Type_Kind kind);
void type_replace(Type* a, Type* b);

Type* make_type_unresolved(char* name);
Type* make_type_void(void);
Type* make_type_int(s8 bytes, bool is_unsigned);
Type* make_type_float(s8 bytes);
Type* make_type_string(s64 len);
Type* make_type_pointer(Type* pointee);
Type* make_type_array(Type* type, s64 size);
Type* make_type_struct(char* name, List* members);
Type* make_type_enum(char* name, List* members);
Type* make_type_function(char* name, List* parameters, Type* return_type, u32 flags);
Type* make_type_var_args(void);

s64 type_function_get_arg_count(Type* type);
s64 type_array_get_count(Type* type);

#define ERROR_UNHANDLED_TYPE ERROR_UNHANDLED_KIND(type_to_str(type->kind))
#define ERROR_UNHANDLED_TYPE_KIND ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind))

#endif
