#include "typespec.h"

#include "utility.h" // error
#include <stdlib.h>  // malloc
#include <assert.h>  // assert
#include "stretchy_buffer.h"  // sb_free


//------------------------------------------------------------------------------
//                               typespec.c
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

u64 get_size_of_typespec(Typespec* type)
{
    switch (type->kind)
    {
        case TYPESPEC_INT: return type->Int.bits / 8;
        case TYPESPEC_FUNC:
        {
            u64 accum_size = 0;
            for (int i = 0; i < sb_count(type->Func.args); ++i)
            {
                accum_size += get_size_of_typespec(type->Func.args[i].type);
            }
            return accum_size;
        }
        default: error("not implemented kind %d", type->kind);
    }
    return 0;
}

void print_type(Typespec* type)
{
    switch (type->kind)
    {
        case TYPESPEC_INT: info(type->Int.is_unsigned ? "u" : "i" "%d",  type->Int.bits); break;
        case TYPESPEC_FLOAT: info("f%d", type->Float.bits); break;
        // case TYPESPEC_POINTER: info("ptr: %d", type->Int.val); break;
        // case TYPESPEC_ENUM: info("enum: %d", type->Int.val); break;
        case TYPESPEC_STRUCT: {
            info("struct. name %s", type->Struct.name); 
            for (int i = 0; i < sb_count(type->Struct.members); ++i)
            {
                info(type->Struct.members[i].name);
                print_type(type->Struct.members[i].type);
            }
        } break;
        case TYPESPEC_FUNC: {
            info("func. name: %d", type->Func.name);
            for (int i = 0; i < sb_count(type->Func.args); ++i)
            {
                info(type->Func.args[i].name);
                print_type(type->Func.args[i].type);
            }
            print_type(type->Func.ret_type); 
        } break;
        default: warning("not implemented kind %d", type->kind);
    }
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

Typespec* make_typespec_int(i8 bits, bool is_unsigned)
{
    assert(bits > 7 && bits < 65);
    assert(is_unsigned = 1 || is_unsigned == 0);
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

Typespec* make_typespec_struct(const char* name, Arg* members)
{
    assert(name);
    Typespec* t = make_typespec(TYPESPEC_STRUCT);
    t->Struct.name = name;
    t->Struct.members = members;
    return t;
}

Typespec* make_typespec_func(const char* name, Arg* args, Typespec* ret_type)
{
    assert(name);
    Typespec* t = make_typespec(TYPESPEC_FUNC);
    t->Func.name = name;
    t->Func.args = args;
    t->Func.ret_type = ret_type;
    return t;
}
