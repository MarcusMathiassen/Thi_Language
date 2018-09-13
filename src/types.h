#ifndef TYPES_H
#define TYPES_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                               types.h
//------------------------------------------------------------------------------

typedef struct Type Type;
typedef struct Arg Arg;
typedef enum Type_Kind Type_Kind;

u64 get_size_of_type(Type* type);

void print_type(Type* type);

Type* make_type_int(i8 bits, bool is_unsigned);
Type* make_type_float(i8 bits);
Type* make_type_pointer(Type* pointee);
Type* make_type_struct(const char* name, Arg* members);
Type* make_type_func(const char* name, Arg* args, Type* ret_type);

enum Type_Kind {
    TYPE_NONE,

    TYPE_INT,
    TYPE_FLOAT,
    TYPE_POINTER,

    TYPE_ENUM,
    TYPE_STRUCT,
    TYPE_FUNC,
};

struct Arg {
    const char* name;
    Type* type;
};

struct Type {

    Type_Kind kind;

    union {
        struct { i8 bits; i8 is_unsigned;                     } Int;
        struct { i8 bits;                                     } Float;
        struct { Type* pointee;                               } Pointer;
        struct { const char* name; Arg* members;              } Struct;
        struct { const char* name; Arg* args; Type* ret_type; } Func;
    };
};

#endif
