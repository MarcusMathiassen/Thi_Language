#include "type.h"

#include "ast.h"       // AST*
#include "constants.h" // TYPE_LIST_STARTING_ALLOC
#include "string.h"    // strf, append_string, string
#include "utility.h"   // error
#include <assert.h>    // assert
#include <stdlib.h>    // xmalloc
#include <string.h>    // strcmp

//------------------------------------------------------------------------------
//                               type.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* type_kind_to_str(Type_Kind kind) {
    switch (kind) {
    case TYPE_UNRESOLVED: return "TYPE_UNRESOLVED";
    case TYPE_VOID: return "TYPE_VOID";
    case TYPE_INT: return "TYPE_INT";
    case TYPE_FLOAT: return "TYPE_FLOAT";
    case TYPE_STRING: return "TYPE_STRING";
    case TYPE_POINTER: return "TYPE_POINTER";
    case TYPE_ARRAY: return "TYPE_ARRAY";
    case TYPE_ENUM: return "TYPE_ENUM";
    case TYPE_STRUCT: return "TYPE_STRUCT";
    case TYPE_FUNCTION: return "TYPE_FUNCTION";
    case TYPE_VAR_ARGS: return "TYPE_VAR_ARGS";
    default: warning("not implemented type_kind_to_str kind %d", kind);
    }
    error("xxx");
    return "";
}

s64 get_size_of_underlying_type(Type* type) {
    switch (type->kind) {
    case TYPE_POINTER: return get_size_of_type(type->Pointer.pointee);
    case TYPE_ARRAY: return get_size_of_type(type->Array.type);
    default: return get_size_of_type(type);
    }
    return 0;
}

bool is_same_type(Type* a, Type* b) {
    char* an = get_type_name(a);
    char* bn = get_type_name(b);
    warning("%s %s", an, bn);
    return strcmp(an, bn) == 0;
}

char* get_type_name(Type* type) {
    if (!type) return "---";
    switch (type->kind) {
    default: error("unhandled case: %s", type_kind_to_str(type->kind));
    case TYPE_VOID: return "void";
    case TYPE_UNRESOLVED: return type->Unresolved.name;
    case TYPE_POINTER: {
        Type* t = type->Pointer.pointee;
        while (t->kind == TYPE_POINTER) {
            warning("%s", type_to_str(t));
            t = type->Pointer.pointee;
        }
        return get_type_name(t);
    }
    case TYPE_STRUCT: return type->Struct.name;
    case TYPE_ENUM: return type->Enum.name;
    case TYPE_FUNCTION: return type->Function.name;
    case TYPE_VAR_ARGS: return "TYPE_VAR_ARGS";
    }
    return NULL;
}

s64 get_size_of_type(Type* type) {
    switch (type->kind) {
    case TYPE_VAR_ARGS: return 0;
    case TYPE_UNRESOLVED: return 0;
    case TYPE_VOID: return 0;
    case TYPE_INT: return type->Int.bytes;
    case TYPE_FLOAT: return type->Float.bytes;
    case TYPE_STRING: return type->String.len;
    case TYPE_POINTER: return 8;
    case TYPE_ARRAY: return get_size_of_type(type->Array.type) * type->Array.size;
    case TYPE_STRUCT: {
        s64 accum_size = 0;
        if (type->Struct.members) {
            LIST_FOREACH(type->Struct.members) {
                AST* mem = (AST*)it->data;
                if (mem->kind != AST_FUNCTION) accum_size += get_size_of_type(mem->Variable_Decl.type);
            }
        }
        return accum_size;
    }
    case TYPE_ENUM: return 8;
    case TYPE_FUNCTION: {
        s64 accum_size = 0;
        LIST_FOREACH(type->Function.args) {
            AST* arg = (AST*)it->data;
            accum_size += get_size_of_type(arg->Variable_Decl.type);
        }
        return accum_size;
    }
    default: warning("get_size_of_type kind %s not implemented.", type_kind_to_str(type->kind));
    }
    return 0;
}

s64 get_offset_in_struct_to_field(Type* type, char* name) {
    assert(type);
    assert(type->kind == TYPE_STRUCT);
    s64 accum_size = 0;
    LIST_FOREACH(type->Struct.members) {
        AST* mem = (AST*)it->data;
        if (strcmp(name, mem->Variable_Decl.name) == 0) {
            return accum_size;
        }
        accum_size += get_size_of_type(mem->Variable_Decl.type);
    }
    error("cant find field: %s", name);
    return -1;
}

s64 type_function_get_arg_count(Type* type) {
    assert(type);
    assert(type->kind == TYPE_FUNCTION);
    return type->Function.args->count;
}

s64 type_array_get_count(Type* type) {
    assert(type);
    assert(type->kind == TYPE_ARRAY);
    return type->Array.size;
}

char* type_to_str(Type* type) {
    if (!type) return "NULL";
    // warning("type_to_str: %s", type_kind_to_str(type->kind));
    switch (type->kind) {
    case TYPE_VAR_ARGS: return "TYPE_VAR_ARGS";
    case TYPE_VOID: return "void";
    case TYPE_UNRESOLVED: return strf("PLACEHOLDER(%s)", type->Unresolved.name);
    case TYPE_ARRAY: return strf("%s[%d]", type_to_str(type->Array.type), type->Array.size);
    case TYPE_INT: return strf(type->Int.is_unsigned ? "u%d" : "s%d", type->Int.bytes * 8);
    case TYPE_POINTER: return strf("%s*", type_to_str(type->Pointer.pointee));
    case TYPE_FLOAT: return strf("f%d", type->Float.bytes * 8);
    case TYPE_STRING: return strf("\"\", %d", type->String.len);
    case TYPE_STRUCT: {
        char* s = strf("%s\n", type->Struct.name);
        LIST_FOREACH(type->Struct.members) {
            AST* mem = (AST*)it->data;
            s        = strf("%s\t%s", s, strf("%s\n", ast_to_str(mem)));
        }
        return s;
    };

    case TYPE_ENUM: {
        char* s = strf("%s\n", type->Enum.name);
        LIST_FOREACH(type->Enum.members) {
            AST* mem = (AST*)it->data;
            s        = strf("%s\t%s", s, strf("%s\n", ast_to_str(mem)));
        }
        return s;
    };

    case TYPE_FUNCTION: {
        string str       = make_string(type->Function.name);
        s64    arg_count = type->Function.args->count;
        s64    arg_index = 0;
        append_string(&str, "(");
        LIST_FOREACH(type->Function.args) {
            append_string(&str, ast_to_str(it->data));
            if (arg_index != arg_count - 1) {
                append_string(&str, ", ");
            }
            arg_index += 1;
        }
        append_string(&str, ")");
        if (type->Function.ret_type) append_string_f(&str, " %s", type_to_str(type->Function.ret_type));
        return str.c_str;
    }
    default: warning("type_to_str not implemented kind %d", type_kind_to_str(type->kind));
    }
    return NULL;
}

char* type_to_json(Type* type) {
    if (!type) return "\"---\"";
    // error("type_to_json got null");
    // warning("type_to_json: %s", type_kind_to_str(type->kind));
    char* result = NULL;
    switch (type->kind) {
    case TYPE_VAR_ARGS: return "TYPE_VAR_ARGS";
    case TYPE_VOID: return "void";
    case TYPE_UNRESOLVED: {
        result = strf("{\"%s\": {\"name\": \"%s\"}}", type_kind_to_str(type->kind), type->Unresolved.name);
    } break;
    case TYPE_ARRAY: {
        result = strf("{\"%s\": {\"type\": %s, \"size\": %d}}", type_kind_to_str(type->kind),
                      type_to_json(type->Array.type), type->Array.size);
    } break;
    case TYPE_INT: {
        result = strf("{\"%s\": {\"bytes\": %d, \"is_signed\":%s}}", type_kind_to_str(type->kind), type->Int.bytes,
                      type->Int.is_unsigned ? "true" : "false");
    } break;
    case TYPE_POINTER: {
        result = strf("{\"%s\": {\"pointee\": %s}}", type_kind_to_str(type->kind), type_to_json(type->Pointer.pointee));
    } break;
    case TYPE_FLOAT: {
        result = strf("{\"%s\": {\"bytes\": %d}}", type_kind_to_str(type->kind), type->Float.bytes);
    } break;
    case TYPE_STRING: {
        result = strf("{\"%s\": {\"len\": %d}}", type_kind_to_str(type->kind), type->String.len);
    } break;
    case TYPE_STRUCT: {
        string str = make_string("");
        append_string_f(&str, "{\"%s\": {\"name\": \"%s\", ", type_kind_to_str(type->kind), type->Struct.name);
        append_string(&str, "\"args\": [");
        s64 arg_count = type->Struct.members->count;
        s64 counter   = 0;
        LIST_FOREACH(type->Struct.members) {
            append_string(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) append_string(&str, ",");
            counter += 1;
        }
        append_string(&str, "]}}");
        result = str.c_str;
    } break;

    case TYPE_ENUM: {
        string str = make_string("");
        append_string_f(&str, "{\"%s\": {\"name\": \"%s\", ", type_kind_to_str(type->kind), type->Enum.name);
        append_string(&str, "\"args\": [");
        s64 arg_count = type->Enum.members->count;
        s64 counter   = 0;
        LIST_FOREACH(type->Enum.members) {
            append_string(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) append_string(&str, ",");
            counter += 1;
        }
        append_string(&str, "]}}");
        result = str.c_str;
    };

    case TYPE_FUNCTION: {
        string str = make_string("");
        append_string_f(&str, "{\"%s\": {\"name\": \"%s\", ", type_kind_to_str(type->kind), type->Enum.name);
        append_string(&str, "\"args\": [");
        s64 arg_count = type->Function.args->count;
        s64 counter   = 0;
        LIST_FOREACH(type->Function.args) {
            append_string(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) append_string(&str, ",");
            counter += 1;
        }
        // append_string(&str, "]}}");
        // warning("BEGIN");
        // warning("%s", type_to_json(type->Function.ret_type));
        // warning("ED");
        append_string(&str, strf("], \"ret_type\":%s}}", type_to_json(type->Function.ret_type)));
        result = str.c_str;
    }
    default: warning("type_to_json not implemented kind %d", type_kind_to_str(type->kind));
    }
    assert(result);
    return result;
}

Type_Ref_List make_type_ref_list() {
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

Type* make_type(Type_Kind kind) {
    Type* t = xmalloc(sizeof(Type));
    t->kind = kind;
    return t;
}

Type* make_type_void() {
    Type* t = make_type(TYPE_VOID);
    return t;
}

Type* make_type_unresolved(char* name) {
    assert(name);
    Type* t            = make_type(TYPE_UNRESOLVED);
    t->Unresolved.name = name;
    return t;
}

Type* make_type_array(Type* type, s32 size) {
    assert(type);
    assert(size > 0);
    Type* t       = make_type(TYPE_ARRAY);
    t->Array.type = type;
    t->Array.size = size;
    return t;
}

Type* make_type_int(s8 bytes, bool is_unsigned) {
    assert(bytes > 0 && bytes < 9);
    assert(is_unsigned == 1 || is_unsigned == 0);
    Type* t            = make_type(TYPE_INT);
    t->Int.bytes       = bytes;
    t->Int.is_unsigned = is_unsigned;
    return t;
}

Type* make_type_float(s8 bytes) {
    assert(bytes > 0 && bytes < 9);
    Type* t        = make_type(TYPE_FLOAT);
    t->Float.bytes = bytes;
    return t;
}

Type* make_type_string(s64 len) {
    assert(len);
    Type* t       = make_type(TYPE_STRING);
    t->String.len = len;
    return t;
}

Type* make_type_pointer(Type* pointee) {
    Type* t            = make_type(TYPE_POINTER);
    t->Pointer.pointee = pointee;
    return t;
}

Type* make_type_enum(char* name, List* members) {
    assert(name);
    Type* t         = make_type(TYPE_ENUM);
    t->name         = name;
    t->Enum.name    = name;
    t->Enum.members = members;
    return t;
}

Type* make_type_struct(char* name, List* members) {
    assert(name);
    Type* t           = make_type(TYPE_STRUCT);
    t->name           = name;
    t->Struct.name    = name;
    t->Struct.members = members;
    return t;
}

Type* make_type_function(char* name, List* args, Type* ret_type, bool has_var_arg) {
    assert(name);
    Type* t                 = make_type(TYPE_FUNCTION);
    t->name                 = name;
    t->Function.name        = name;
    t->Function.args        = args;
    t->Function.ret_type    = ret_type;
    t->Function.has_var_arg = has_var_arg;
    return t;
}
Type* make_type_var_args() {
    Type* t = make_type(TYPE_VAR_ARGS);
    return t;
}
