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

#include "type.h"
 // TYPE_LIST_STARTING_ALLOC
#include "string.h"    // strf, string_append, string
#include "common.h"
#include "utility.h" // error
#include <string.h>  // strcmp
#include <stdlib.h> // malloc, realloc, calloc
//------------------------------------------------------------------------------
//                               type.c
//------------------------------------------------------------------------------

char* type_kind_to_str(Type_Kind kind) {
    TASSERT_KIND_IN_RANGE(TYPE, kind);
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("kind = %d", kind));
    case TYPE_UNRESOLVED: return "TYPE_UNRESOLVED";
    case TYPE_VOID:       return "TYPE_VOID";
    case TYPE_INT:        return "TYPE_INT";
    case TYPE_FLOAT:      return "TYPE_FLOAT";
    case TYPE_STRING:     return "TYPE_STRING";
    case TYPE_POINTER:    return "TYPE_POINTER";
    case TYPE_ARRAY:      return "TYPE_ARRAY";
    case TYPE_ENUM:       return "TYPE_ENUM";
    case TYPE_STRUCT:     return "TYPE_STRUCT";
    case TYPE_FUNCTION:   return "TYPE_FUNCTION";
    case TYPE_VAR_ARGS:   return "TYPE_VAR_ARGS";
    }
    UNREACHABLE;
    return NULL;
}

//------------------------------------------------------------------------------
//                               String Functions
//------------------------------------------------------------------------------

static void type_to_str_unresolved (string* s, Type* type);
static void type_to_str_void       (string* s, Type* type);
static void type_to_str_int        (string* s, Type* type);
static void type_to_str_float      (string* s, Type* type);
static void type_to_str_string     (string* s, Type* type);
static void type_to_str_pointer    (string* s, Type* type);
static void type_to_str_array      (string* s, Type* type);
static void type_to_str_enum       (string* s, Type* type);
static void type_to_str_struct     (string* s, Type* type);
static void type_to_str_union      (string* s, Type* type);
static void type_to_str_function   (string* s, Type* type);
static void type_to_str_var_args   (string* s, Type* type);

char* type_to_str(Type* type) {

    string* s = make_string("");

    if (!type) {
        string_append(s, "---");
        return string_data(s);
    }

    Type_Kind kind = type->kind;
    TASSERT_KIND_IN_RANGE(TYPE, kind);

    switch (kind) {
    ERROR_UNHANDLED_TYPE_KIND(kind);
    case TYPE_UNRESOLVED: type_to_str_unresolved (s, type); break;
    case TYPE_VOID:       type_to_str_void       (s, type); break;
    case TYPE_INT:        type_to_str_int        (s, type); break;
    case TYPE_FLOAT:      type_to_str_float      (s, type); break;
    case TYPE_STRING:     type_to_str_string     (s, type); break;
    case TYPE_POINTER:    type_to_str_pointer    (s, type); break;
    case TYPE_ARRAY:      type_to_str_array      (s, type); break;
    case TYPE_ENUM:       type_to_str_enum       (s, type); break;
    case TYPE_STRUCT:     type_to_str_struct     (s, type); break;
    case TYPE_UNION:      type_to_str_union      (s, type); break;
    case TYPE_FUNCTION:   type_to_str_function   (s, type); break;
    case TYPE_VAR_ARGS:   type_to_str_var_args   (s, type); break;
    }
    return string_data(s);
}

static void type_to_str_unresolved (string* s, Type* type) {
    string_append_f(s, "%s?", get_type_name(type));
}

static void type_to_str_void       (string* s, Type* type) {
    string_append(s, "void");
}

static void type_to_str_int        (string* s, Type* type) {
    string_append_f(s, type->Int.is_unsigned ? "u%d" : "s%d", type->Int.bytes * 8);
}

static void type_to_str_float      (string* s, Type* type) {
    string_append_f(s, "f%d", type->Float.bytes * 8);
}

static void type_to_str_string     (string* s, Type* type) {
    string_append_f(s, "\"\", %d", type->String.len);
}

static void type_to_str_pointer    (string* s, Type* type) {
    string_append(s, type_to_str(type->Pointer.pointee));
    string_append(s, "*");
}

static void type_to_str_array      (string* s, Type* type) {
    type_to_str(type->Array.type);
    string_append_f(s, "[%d]", type->Array.size);
}

static void type_to_str_enum       (string* s, Type* type) {
    string_append_f(s, "%s = { ", get_type_name(type));
    list_foreach(type_get_members(type)) {
        Type_Name_Pair* mem = it->data;
        if (mem->name)
            string_append_f(s, "%s %s", mem->name, get_type_name(mem->type));
        else
            string_append_f(s, "%s", get_type_name(mem->type));
        if (it->next) string_append(s, ", ");
    }
    string_append(s, " }");
}

static void type_to_str_struct     (string* s, Type* type) {
    string_append_f(s, "%s = { ", get_type_name(type));
    list_foreach(type_get_members(type)) {
        Type_Name_Pair* mem = it->data;
        if (mem->name)
            string_append_f(s, "%s %s", mem->name, get_type_name(mem->type));
        else
            string_append_f(s, "%s", get_type_name(mem->type));
        if (it->next) string_append(s, ", ");
    }
    string_append(s, " }");
}

static void type_to_str_union      (string* s, Type* type) {

}

static void type_to_str_function   (string* s, Type* type) {
    string_append(s, "(");
    list_foreach(type->Function.parameters) {
        Type_Name_Pair* param = it->data;
        if (param->name)
            string_append_f(s, "%s %s", param->name, get_type_name(param->type));
        else
            string_append_f(s, "%s", get_type_name(param->type));
        if (it->next) string_append(s, ", ");
    }
    string_append_f(s, ") %s", get_type_name(type->Function.return_type));
}

static void type_to_str_var_args   (string* s, Type* type) {
    string_append(s, "...");
}


//------------------------------------------------------------------------------
//                               Type Array Ref
//------------------------------------------------------------------------------


Type_Ref_List make_type_ref_list() {
    Type_Ref_List l;
    l.count = 0;
    l.allocated = TYPE_REF_LIST_STARTING_ALLOC;
    l.data = xmalloc(l.allocated * sizeof(Type*));
    return l;
}

void type_ref_list_append(Type_Ref_List* l, Type* t) {
    if (l->count == l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(*l->data));
    }
    l->data[l->count++] = t;
}


//------------------------------------------------------------------------------
//                               Type Helpers
//------------------------------------------------------------------------------

List* type_get_members(Type* type) {
    xassert(type);
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_ENUM: return type->Enum.members;
    case TYPE_STRUCT: return type->Struct.members;
    }
    UNREACHABLE;
    return NULL;
}

// @Cleanup(marcus) we're leaking memory here.
void type_replace(Type* a, Type* b) {
    xassert(a);
    xassert(b);
    debug("REPLACED %s WITH %s", give_unique_color(type_to_str(a)), give_unique_color(type_to_str(b)));
    *a = *b;
}

Type* get_underlying_type_if_any(Type* type) {
    xassert(type);
    switch (type->kind) {
    default: return type;
    case TYPE_POINTER: return type->Pointer.pointee;
    case TYPE_ARRAY: return type->Array.type;
    }
    UNREACHABLE;
    return NULL;
}

s64 get_size_of_underlying_type_if_any(Type* type) {
    xassert(type);
    switch (type->kind) {
    default: return get_size_of_type(type);
    case TYPE_POINTER: return get_size_of_type(type->Pointer.pointee);
    case TYPE_ARRAY: return get_size_of_type(type->Array.type);
    }
    UNREACHABLE;
    return 0;
}

bool is_same_type(Type* a, Type* b) {
    xassert(a);
    xassert(b);
    char* an = type_to_str(a);
    char* bn = type_to_str(b);
    return strcmp(an, bn) == 0;
}

char* get_type_name(Type* type) {
    if (!type) return "---";
    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_STRING:   // fallthrough
    case TYPE_INT:   // fallthrough
    case TYPE_FLOAT: // fallthrough
    case TYPE_POINTER:
    case TYPE_ARRAY:
    case TYPE_VOID: return type_to_str(type);
    case TYPE_FUNCTION: return type->Function.name;
    case TYPE_UNRESOLVED: return type->Unresolved.name;
    case TYPE_STRUCT: return type->Struct.name;
    case TYPE_ENUM: return type->Enum.name;
    case TYPE_VAR_ARGS: return "...";
    }
    UNREACHABLE;
    return NULL;
}

s64 get_size_of_type(Type* type) {
    xassert(type);

    switch (type->kind) {
        ERROR_UNHANDLED_TYPE_KIND(type->kind);
    case TYPE_VAR_ARGS: return 0;
    case TYPE_UNRESOLVED: return 0;
    case TYPE_VOID: return 0;
    case TYPE_INT: return type->Int.bytes;
    case TYPE_FLOAT: return type->Float.bytes;
    case TYPE_STRING: return type->String.len;
    case TYPE_POINTER: return DEFAULT_POINTER_SIZE;
    case TYPE_ENUM: return DEFAULT_ENUM_SIZE;
    case TYPE_ARRAY: return get_size_of_type(type->Array.type) * type->Array.size;
    case TYPE_STRUCT: {
        s64 accum_size = 0;
        list_foreach(type_get_members(type)) {
            Type_Name_Pair* mem = it->data;
            if (mem->type->kind == TYPE_FUNCTION) continue;
            accum_size += get_size_of_type(mem->type);
        }
        return accum_size;
    }
    case TYPE_FUNCTION: {
        s64 accum_size = 0;
        list_foreach(type->Function.parameters) {
            Type_Name_Pair* arg = it->data;
            accum_size += get_size_of_type(arg->type);
        }
        return accum_size;
    }
    }

    UNREACHABLE;
    return 0;
}

s64 get_offset_in_struct_to_field(Type* type, char* name) {
    xassert(type);
    xassert(type->kind == TYPE_STRUCT);
    s64 accum_size = 0;
    list_foreach(type_get_members(type)) {
        Type_Name_Pair* mem = it->data;
        if (strcmp(name, mem->name) == 0) return accum_size;
        accum_size += get_size_of_type(mem->type);
    }
    error("cant find field: %s", name);
    UNREACHABLE;
    return 0;
}

s64 type_function_get_arg_count(Type* type) {
    xassert(type);
    xassert(type->kind == TYPE_FUNCTION);
    return type->Function.parameters->count;
}

s64 type_array_get_count(Type* type) {
    xassert(type);
    xassert(type->kind == TYPE_ARRAY);
    return type->Array.size;
}


//------------------------------------------------------------------------------
//                               Type Maker Functions
//------------------------------------------------------------------------------

Type* make_type(Type_Kind kind, u32 flags) {
    Type* t = xmalloc(sizeof(Type));
    t->kind = kind;
    t->flags = flags;
    return t;
}

Type* make_type_void() {
    Type* t = make_type(TYPE_VOID, 0);
    return t;
}

Type* make_type_var_args() {
    Type* t = make_type(TYPE_VAR_ARGS, 0);
    return t;
}

Type* make_type_unresolved(char* name) {
    xassert(name);
    Type* t = make_type(TYPE_UNRESOLVED, 0);
    t->Unresolved.name = name;
    return t;
}

Type* make_type_array(Type* type, s64 size) {
    xassert(type);
    xassert(size > 0);
    Type* t = make_type(TYPE_ARRAY, 0);
    t->Array.type = type;
    t->Array.size = size;
    return t;
}

Type* make_type_int(s8 bytes, bool is_unsigned) {
    xassert(bytes > 0 && bytes < 9);
    xassert(is_unsigned == 1 || is_unsigned == 0);
    Type* t = make_type(TYPE_INT, 0);
    t->Int.bytes = bytes;
    t->Int.is_unsigned = is_unsigned;
    return t;
}

Type* make_type_float(s8 bytes) {
    xassert(bytes > 0 && bytes < 9);
    Type* t = make_type(TYPE_FLOAT, 0);
    t->Float.bytes = bytes;
    return t;
}

Type* make_type_string(s64 len) {
    xassert(len);
    Type* t = make_type(TYPE_STRING, 0);
    t->String.len = len;
    return t;
}

Type* make_type_pointer(Type* pointee) {
    xassert(pointee);
    Type* t = make_type(TYPE_POINTER, 0);
    t->Pointer.pointee = pointee;
    return t;
}

Type* make_type_enum(char* name, List* members) {
    xassert(name);
    xassert(members);
    Type* t = make_type(TYPE_ENUM, 0);
    t->name = name;
    t->Enum.name = name;
    t->Enum.members = members;
    return t;
}

Type* make_type_struct(char* name, List* members) {
    xassert(name);
    xassert(members);
    Type* t = make_type(TYPE_STRUCT, 0);
    t->name = name;
    t->Struct.name = name;
    t->Struct.members = members;
    return t;
}

Type* make_type_function(char* name, List* parameters, Type* return_type, u32 flags) {
    xassert(name);
    xassert(parameters);
    xassert(return_type);
    Type* t = make_type(TYPE_FUNCTION, flags);
    t->name = name;
    t->Function.name = name;
    t->Function.parameters = parameters;
    t->Function.return_type = return_type;
    return t;
}
