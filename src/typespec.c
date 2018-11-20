#include "typespec.h"

#include "stretchy_buffer.h" // sb_free, sb_count
#include "string.h"          // strf, append_string, string
#include "utility.h"         // error
#include <assert.h>          // assert
#include <stdlib.h>          // malloc

//------------------------------------------------------------------------------
//                               typespec.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

const char* typespec_kind_to_str(Typespec_Kind kind)
{
    switch (kind) {
    case TYPESPEC_INT: return "TYPESPEC_INT";
    case TYPESPEC_FLOAT: return "TYPESPEC_FLOAT";
    case TYPESPEC_POINTER: return "TYPESPEC_POINTER";
    case TYPESPEC_ARRAY: return "TYPESPEC_ARRAY";
    case TYPESPEC_ENUM: return "TYPESPEC_ENUM";
    case TYPESPEC_STRUCT: return "TYPESPEC_STRUCT";
    case TYPESPEC_FUNCTION: return "TYPESPEC_FUNCTION";
    default: error("not implemented typespec_kind_to_str kind %d", kind);
    }
    return "";
}

u64 get_size_of_typespec(Typespec* type)
{
    switch (type->kind) {
    case TYPESPEC_INT: return type->Int.bits / 8;
    case TYPESPEC_FUNCTION: {
        u64 accum_size = 0;
        for (int i = 0; i < sb_count(type->Function.args); ++i) {
            accum_size += get_size_of_typespec(type->Function.args[i].type);
        }
        return accum_size;
    }
    default: error("get_size_of_typespec kind %s not implemented.", typespec_kind_to_str(type->kind));
    }
    return 0;
}

u64 typespec_function_get_arg_count(Typespec* type)
{
    assert(type);
    assert(type->kind == TYPESPEC_FUNCTION);
    return sb_count(type->Function.args);
}

char* typespec_to_json(Typespec* type)
{
    switch (type->kind) {
    case TYPESPEC_ARRAY: return strf("%s[%d]", typespec_to_str(type->Array.type), type->Array.size);
    case TYPESPEC_INT: return strf(type->Int.is_unsigned ? "u%d" : "i%d", type->Int.bits);
    case TYPESPEC_FLOAT: return strf("f%d", type->Float.bits);
    case TYPESPEC_STRUCT: {
        string str = make_string(strf("%s :: {\n", type->Struct.name));
        Arg* args = type->Struct.members;
        int arg_count = sb_count(args);
        if (arg_count) {
            for (int i = 0; i < arg_count; ++i) {
                append_string(&str, strf("%s: %s", args[i].name, typespec_to_str(args[i].type)));
                if (i != arg_count - 1) append_string(&str, ", ");
            }
        }
        append_string(&str, "}\n");
        return str.c_str;
    };
    case TYPESPEC_ENUM: {
        string str = make_string(strf("%s :: enum {", type->Enum.name));
        const char** args = type->Enum.members;
        int arg_count = sb_count(args);
        if (arg_count) {
            for (int i = 0; i < arg_count; ++i) {
                append_string(&str, strf("%s", args[i]));
                if (i != arg_count - 1) append_string(&str, ", ");
            }
        }
        return str.c_str;
    };

    case TYPESPEC_FUNCTION: {
        string str = make_string(strf("%s :: (", type->Function.name));
        strf("func. name: %d", type->Function.name);
        Arg* args = type->Function.args;
        int arg_count = sb_count(args);
        if (arg_count) {
            for (int i = 0; i < arg_count; ++i) {
                if (args[i].name)
                    append_string(&str, strf("%s: %s", args[i].name, typespec_to_str(args[i].type)));
                else
                    append_string(&str, strf("%s", typespec_to_str(args[i].type)));
                if (i != arg_count - 1) append_string(&str, ", ");
            }
        }
        if (type->Function.ret_type) append_string(&str, strf(") -> %s", typespec_to_str(type->Function.ret_type)));
        return str.c_str;
    }
    default: warning("typespec_to_json not implemented kind %d", typespec_kind_to_str(type->kind));
    }
    return NULL;
}

char* typespec_to_str(Typespec* type)
{
    switch (type->kind) {
    case TYPESPEC_ARRAY: return strf("%s[%d]", typespec_to_str(type->Array.type), type->Array.size);
    case TYPESPEC_INT: return strf(type->Int.is_unsigned ? "u%d" : "i%d", type->Int.bits);
    case TYPESPEC_FLOAT: return strf("f%d", type->Float.bits);
    case TYPESPEC_STRUCT: {
        string str = make_string(strf("%s :: {\n", type->Struct.name));
        Arg* args = type->Struct.members;
        int arg_count = sb_count(args);
        if (arg_count) {
            for (int i = 0; i < arg_count; ++i) {
                append_string(&str, strf("%s: %s", args[i].name, typespec_to_str(args[i].type)));
                if (i != arg_count - 1) append_string(&str, ", ");
            }
        }
        append_string(&str, "}\n");
        return str.c_str;
    };
    case TYPESPEC_ENUM: {
        string str = make_string(strf("%s :: enum {", type->Enum.name));
        const char** args = type->Enum.members;
        int arg_count = sb_count(args);
        if (arg_count) {
            for (int i = 0; i < arg_count; ++i) {
                append_string(&str, strf("%s", args[i]));
                if (i != arg_count - 1) append_string(&str, ", ");
            }
        }
        return str.c_str;
    };

    case TYPESPEC_FUNCTION: {
        string str = make_string(strf("%s :: (", type->Function.name));
        strf("func. name: %d", type->Function.name);
        Arg* args = type->Function.args;
        int arg_count = sb_count(args);
        if (arg_count) {
            for (int i = 0; i < arg_count; ++i) {
                if (args[i].name)
                    append_string(&str, strf("%s: %s", args[i].name, typespec_to_str(args[i].type)));
                else
                    append_string(&str, strf("%s", typespec_to_str(args[i].type)));
                if (i != arg_count - 1) append_string(&str, ", ");
            }
        }
        if (type->Function.ret_type) append_string(&str, strf(") -> %s", typespec_to_str(type->Function.ret_type)));
        return str.c_str;
    }
    default: warning("not implemented kind %d", type->kind);
    }
    return NULL;
}

//------------------------------------------------------------------------------
//                               Type Maker Functions
//------------------------------------------------------------------------------

Typespec* make_typespec(Typespec_Kind kind)
{
    Typespec* t = xmalloc(sizeof(Typespec));
    t->kind = kind;
    return t;
}

Typespec* make_typespec_array(Typespec* type, int size)
{
    assert(type);
    assert(size > 0);
    Typespec* t = make_typespec(TYPESPEC_ARRAY);
    t->Array.type = type;
    t->Array.size = size;
    return t;

}

Typespec* make_typespec_int(i8 bits, bool is_unsigned)
{
    assert(bits > 7 && bits < 65);
    assert(is_unsigned == 1 || is_unsigned == 0);
    Typespec* t = make_typespec(TYPESPEC_INT);
    t->Int.bits = bits;
    t->Int.is_unsigned = is_unsigned;
    return t;
}

Typespec* make_typespec_float(i8 bits)
{
    assert(bits > 7 && bits < 65);
    Typespec* t = make_typespec(TYPESPEC_FLOAT);
    t->Float.bits = bits;
    return t;
}

Typespec* make_typespec_pointer(Typespec* pointee)
{
    Typespec* t = make_typespec(TYPESPEC_FLOAT);
    t->Pointer.pointee = pointee;
    return t;
}

Typespec* make_typespec_enum(const char* name, const char** members)
{
    assert(name);
    Typespec* t = make_typespec(TYPESPEC_ENUM);
    t->Enum.name = name;
    t->Enum.members = members;
    return t;
}

Typespec* make_typespec_struct(const char* name, Arg* members)
{
    assert(name);
    Typespec* t = make_typespec(TYPESPEC_STRUCT);
    t->Struct.name = name;
    t->Struct.members = members;
    return t;
}

Typespec* make_typespec_function(const char* name, Arg* args, Typespec* ret_type)
{
    assert(name);
    Typespec* t = make_typespec(TYPESPEC_FUNCTION);
    t->Function.name = name;
    t->Function.args = args;
    t->Function.ret_type = ret_type;
    return t;
}
