#include "types.h"

#include "utility.h" // error
#include <stdlib.h>  // malloc
#include "stretchy_buffer.h"  // sb_free


//------------------------------------------------------------------------------
//                               types.c
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

u64 get_size_of_type(Type* type) {
    error("get_size_of_type not impl.");
    return 0;
}

void print_type(Type* type)
{
    switch (type->kind)
    {
        case TYPE_INT: info(type->Int.is_unsigned ? "u" : "i" "%d",  type->Int.bits); break;
        case TYPE_FLOAT: info("f%d", type->Float.bits); break;
        // case TYPE_POINTER: info("ptr: %d", type->Int.val); break;
        // case TYPE_ENUM: info("enum: %d", type->Int.val); break;
        case TYPE_STRUCT: {
            info("struct. name %s", type->Struct.name); 
            for (int i = 0; i < sb_count(type->Struct.members); ++i)
            {
                info(type->Struct.members[i].name);
                print_type(type->Struct.members[i].type);
            }
        } break;
        case TYPE_FUNC: {
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

Type* make_type_int(i8 bits, bool is_unsigned) {
    Type* temp = (Type*)xmalloc(sizeof(Type));
    temp->kind = TYPE_INT;
    temp->Int.bits = bits;
    temp->Int.is_unsigned = is_unsigned;
    return temp;
}

Type* make_type_float(i8 bits) {
    Type* temp = (Type*)xmalloc(sizeof(Type));
    temp->kind = TYPE_FLOAT;
    temp->Float.bits = bits;
    return temp;
}

Type* make_type_pointer(Type* pointee) {
    Type* temp = (Type*)xmalloc(sizeof(Type));
    temp->kind = TYPE_POINTER;
    temp->Pointer.pointee = pointee;
    return temp;
}

Type* make_type_struct(const char* name, Arg* members) {
    Type* temp = (Type*)xmalloc(sizeof(Type));
    temp->kind = TYPE_STRUCT;
    temp->Struct.name = name;
    temp->Struct.members = members;
    return temp;

}

Type* make_type_func(const char* name, Arg* args, Type* ret_type) {
    Type* temp = (Type*)xmalloc(sizeof(Type));
    temp->kind = TYPE_FUNC;
    temp->Func.name = name;
    temp->Func.args = args;
    temp->Func.ret_type = ret_type;
    return temp;
}
