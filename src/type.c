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

#include "ast.h"       // AST*
#include "constants.h" // TYPE_LIST_STARTING_ALLOC
#include "string.h"    // strf, string_append, string
#include "utility.h"   // error
#include <assert.h>    // assert
#include <string.h>    // strcmp

//------------------------------------------------------------------------------
//                               type.c
//------------------------------------------------------------------------------

char* type_kind_to_str(Type_Kind kind) {
    // clang-format off
    assert(kind < TYPE_COUNT);
    switch (kind) {
    default: ERROR_UNHANDLED_KIND(strf("%d", kind));
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
    case TYPE_COUNT:      return "TYPE_COUNT";
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

List* type_get_members(Type* type) {
    assert(type);
    // clang-format off
    switch(type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_ENUM:       return type->Enum.members;
    case TYPE_STRUCT:     return type->Struct.members;
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

void type_replace(Type* a, Type* b) {
    assert(a);
    assert(b);
    info("REPLACED %s WITH %s", give_unique_color(type_to_str(a)), give_unique_color(type_to_str(b)));
    *a = *b;
    // MEM_LEAK
}

Type* get_underlying_type_if_any(Type* type) {
    assert(type);
    // clang-format off
    switch (type->kind) {
    default:           return type;
    case TYPE_POINTER: return type->Pointer.pointee;
    case TYPE_ARRAY:   return type->Array.type;
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

s64 get_size_of_underlying_type_if_any(Type* type) {
    assert(type);
    // clang-format off
    switch (type->kind) {
    default:           return get_size_of_type(type);
    case TYPE_POINTER: return get_size_of_type(type->Pointer.pointee);
    case TYPE_ARRAY:   return get_size_of_type(type->Array.type);
    }
    // clang-format on
    UNREACHABLE;
    return 0;
}

bool is_same_type(Type* a, Type* b) {
    assert(a);
    assert(b);
    char* an = type_to_str(a);
    char* bn = type_to_str(b);
    return strcmp(an, bn) == 0;
}

char* get_type_name(Type* type) {
    if (!type) return "---";
    // clang-format off
    switch (type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_INT:        return "int";
    case TYPE_VOID:       return "void";
    case TYPE_UNRESOLVED: return type->Unresolved.name;
    case TYPE_POINTER: {
        Type* t = type->Pointer.pointee;
        while (t->kind == TYPE_POINTER) {
            t = type->Pointer.pointee;
        }
        return get_type_name(t);
    }
    case TYPE_STRUCT:   return type->Struct.name;
    case TYPE_ENUM:     return type->Enum.name;
    case TYPE_FUNCTION: return type->Function.name ? type->Function.name : "---";
    case TYPE_VAR_ARGS: return "TYPE_VAR_ARGS";
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

s64 get_size_of_type(Type* type) {
    assert(type);
    // clang-format off
    switch (type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_VAR_ARGS:   return 0;
    case TYPE_UNRESOLVED: return 0;
    case TYPE_VOID:       return 0;
    case TYPE_INT:        return type->Int.bytes;
    case TYPE_FLOAT:      return type->Float.bytes;
    case TYPE_STRING:     return type->String.len;
    case TYPE_POINTER:    return 8;
    case TYPE_ENUM:       return 8;
    case TYPE_ARRAY:      return get_size_of_type(type->Array.type) * type->Array.size;
    case TYPE_STRUCT: {
        s64 accum_size = 0;
        if (type->Struct.members) {
            LIST_FOREACH(type->Struct.members) {
                AST* mem = (AST*)it->data;
                if (mem->kind != AST_FUNCTION)
                    accum_size += get_size_of_type(mem->type);
            }
        }
        return accum_size;
    }
    case TYPE_FUNCTION: {
        s64 accum_size = 0;
        LIST_FOREACH(type->Function.parameters) {
            AST* arg = (AST*)it->data;
            accum_size += get_size_of_type(arg->type);
        }
        return accum_size;
    }
    }
    // clang-format on
    UNREACHABLE;
    return 0;
}

s64 get_offset_in_struct_to_field(Type* type, char* name) {
    assert(type);
    assert(type->kind == TYPE_STRUCT);
    s64 accum_size = 0;
    LIST_FOREACH(type_get_members(type)) {
        AST* mem = it->data;
        if (strcmp(name, mem->Variable_Decl.name) == 0) {
            return accum_size;
        }
        accum_size += get_size_of_type(mem->type);
    }
    error("cant find field: %s", name);
    UNREACHABLE;
    return 0;
}

s64 type_function_get_arg_count(Type* type) {
    assert(type);
    assert(type->kind == TYPE_FUNCTION);
    return type->Function.parameters->count;
}

s64 type_array_get_count(Type* type) {
    assert(type);
    assert(type->kind == TYPE_ARRAY);
    return type->Array.size;
}

char* type_to_str(Type* type) {
    if (!type) return "---";
    // clang-format off
    switch (type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_VAR_ARGS:   return "TYPE_VAR_ARGS";
    case TYPE_VOID:       return "void";
    case TYPE_UNRESOLVED: return strf("%s?", get_type_name(type));
    case TYPE_ARRAY:      return strf("%s[%d]", type_to_str(type->Array.type), type->Array.size);
    case TYPE_INT:        return strf(type->Int.is_unsigned ? "u%d" : "s%d", type->Int.bytes * 8);
    case TYPE_POINTER:    return strf("%s*", type_to_str(type->Pointer.pointee));
    case TYPE_FLOAT:      return strf("f%d", type->Float.bytes * 8);
    case TYPE_STRING:     return strf("\"\", %d", type->String.len);

    case TYPE_ENUM: // fallthrough
    case TYPE_STRUCT: {
        string s = string_create_f("%s = { ", get_type_name(type));
        LIST_FOREACH(type_get_members(type)) {
            AST* mem = it->data;
            string_append(&s, ast_to_str(mem));
            if (it->next) string_append(&s, ", ");
        }
        string_append(&s, " }");
        return string_data(&s);
    };

    case TYPE_FUNCTION: {
        string s = string_create_f("%s(", get_type_name(type));
        LIST_FOREACH(type->Function.parameters) {
            string_append(&s, ast_to_str(it->data));
            if (it->next) string_append(&s, ", ");
        }
        string_append_f(&s, ") %s", type_to_str(type->Function.return_type));
        return string_data(&s);
    }
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

char* type_to_json(Type* type) {
    if (!type) return "\"---\"";
    char* result = NULL;
    switch (type->kind) {
    default: ERROR_UNHANDLED_KIND(type_kind_to_str(type->kind));
    case TYPE_VAR_ARGS: return "TYPE_VAR_ARGS";
    case TYPE_VOID: return "void";
    case TYPE_UNRESOLVED: {
        result = strf("{\"%s\": {\"name\": \"%s\"}}",
                      type_kind_to_str(type->kind),
                      type->Unresolved.name);
    } break;
    case TYPE_ARRAY: {
        result = strf("{\"%s\": {\"type\": %s, \"size\": %d}}",
                      type_kind_to_str(type->kind),
                      type_to_json(type->Array.type),
                      type->Array.size);
    } break;
    case TYPE_INT: {
        result = strf("{\"%s\": {\"bytes\": %d, \"is_signed\":%s}}",
                      type_kind_to_str(type->kind),
                      type->Int.bytes,
                      type->Int.is_unsigned ? "true" : "false");
    } break;
    case TYPE_POINTER: {
        result =
            strf("{\"%s\": {\"pointee\": %s}}", type_kind_to_str(type->kind), type_to_json(type->Pointer.pointee));
    } break;
    case TYPE_FLOAT: {
        result = strf("{\"%s\": {\"bytes\": %d}}", type_kind_to_str(type->kind), type->Float.bytes);
    } break;
    case TYPE_STRING: {
        result = strf("{\"%s\": {\"len\": %d}}", type_kind_to_str(type->kind), type->String.len);
    } break;
    case TYPE_STRUCT: {
        return strf("{\"%s\": {\"name\": \"%s\"}}", type_kind_to_str(type->kind), type->Struct.name);
        string str = string_create("");
        string_append_f(&str, "{\"%s\": {\"name\": \"%s\", ", type_kind_to_str(type->kind), type->Struct.name);
        string_append(&str, "\"args\": [");
        s64 arg_count = type->Struct.members->count;
        s64 counter   = 0;
        LIST_FOREACH(type->Struct.members) {
            string_append(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) string_append(&str, ",");
            counter += 1;
        }
        string_append(&str, "]}}");
        result = str.c_str;
    } break;

    case TYPE_ENUM: {
        return strf("{\"%s\": {\"name\": \"%s\"}}", type_kind_to_str(type->kind), type->Enum.name);
        string str = string_create("");
        string_append_f(&str, "{\"%s\": {\"name\": \"%s\", ", type_kind_to_str(type->kind), type->Enum.name);
        string_append(&str, "\"args\": [");
        s64 arg_count = type->Enum.members->count;
        s64 counter   = 0;
        LIST_FOREACH(type->Enum.members) {
            string_append(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) string_append(&str, ",");
            counter += 1;
        }
        string_append(&str, "]}}");
        result = str.c_str;
    };

    case TYPE_FUNCTION: {
        string str = string_create("");
        string_append_f(&str, "{\"%s\": {\"name\": \"%s\", ", type_kind_to_str(type->kind), type->Enum.name);
        string_append(&str, "\"args\": [");
        s64 arg_count = type->Function.parameters->count;
        s64 counter   = 0;
        LIST_FOREACH(type->Function.parameters) {
            string_append(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) string_append(&str, ",");
            counter += 1;
        }
        // string_append(&str, "]}}");
        // warning("BEGIN");
        // warning("%s", type_to_json(type->Function.ret_type));
        // warning("ED");
        string_append(&str, strf("], \"ret_type\":%s}}", type_to_json(type->Function.return_type)));
        result = str.c_str;
    }
    }
    assert(result);
    return result;
}

Type_Ref_List
make_type_ref_list() {
    Type_Ref_List l;
    l.count     = 0;
    l.allocated = TYPE_REF_LIST_STARTING_ALLOC;
    l.data      = xmalloc(l.allocated * sizeof(Type*));
    return l;
}

void type_ref_list_append(Type_Ref_List* l, Type* t) {
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(Type*));
    }
    l->data[l->count] = t;
    l->count += 1;
}

//------------------------------------------------------------------------------
//                               Type Maker Functions
//------------------------------------------------------------------------------

Type* make_type(Type_Kind kind, u32 flags) {
    Type* t  = xmalloc(sizeof(Type));
    t->kind  = kind;
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
    assert(name);
    Type* t            = make_type(TYPE_UNRESOLVED, 0);
    t->Unresolved.name = name;
    return t;
}

Type* make_type_array(Type* type, s64 size) {
    assert(type);
    assert(size > 0);
    Type* t       = make_type(TYPE_ARRAY, 0);
    t->Array.type = type;
    t->Array.size = size;
    return t;
}

Type* make_type_int(s8 bytes, bool is_unsigned) {
    assert(bytes > 0 && bytes < 9);
    assert(is_unsigned == 1 || is_unsigned == 0);
    Type* t            = make_type(TYPE_INT, 0);
    t->Int.bytes       = bytes;
    t->Int.is_unsigned = is_unsigned;
    return t;
}

Type* make_type_float(s8 bytes) {
    assert(bytes > 0 && bytes < 9);
    Type* t        = make_type(TYPE_FLOAT, 0);
    t->Float.bytes = bytes;
    return t;
}

Type* make_type_string(s64 len) {
    assert(len);
    Type* t       = make_type(TYPE_STRING, 0);
    t->String.len = len;
    return t;
}

Type* make_type_pointer(Type* pointee) {
    assert(pointee);
    Type* t            = make_type(TYPE_POINTER, 0);
    t->Pointer.pointee = pointee;
    return t;
}

Type* make_type_enum(char* name, List* members) {
    assert(name);
    assert(members);
    Type* t         = make_type(TYPE_ENUM, 0);
    t->name         = name;
    t->Enum.name    = name;
    t->Enum.members = members;
    return t;
}

Type* make_type_struct(char* name, List* members) {
    assert(name);
    assert(members);
    Type* t           = make_type(TYPE_STRUCT, 0);
    t->name           = name;
    t->Struct.name    = name;
    t->Struct.members = members;
    return t;
}

Type* make_type_function(char* name, List* parameters, Type* return_type, u32 flags) {
    assert(name);
    Type* t                 = make_type(TYPE_FUNCTION, flags);
    t->name                 = name;
    t->Function.name        = name;
    t->Function.parameters  = parameters;
    t->Function.return_type = return_type;
    return t;
}
