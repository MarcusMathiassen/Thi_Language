#ifndef TYPESPEC_H
#define TYPESPEC_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                               typespec.h
//------------------------------------------------------------------------------

typedef struct Typespec Typespec;
typedef struct Arg Arg;
typedef enum Typespec_Kind Typespec_Kind;

u64 get_size_of_typespec(Typespec* Typespec);
char* typespec_to_str(Typespec* type);

Typespec* make_typespec_int(i8 bits, bool is_unsigned);
Typespec* make_typespec_float(i8 bits);
Typespec* make_typespec_string(u64 len);
Typespec* make_typespec_pointer(Typespec* pointee);
Typespec* make_typespec_array(Typespec* type, int size);
Typespec* make_typespec_struct(char* name, Arg* members);
Typespec* make_typespec_enum(char* name, char** members);
Typespec* make_typespec_function(char* name, Arg* args, Typespec* ret_type);

u64 typespec_function_get_arg_count(Typespec* type);
u64 typespec_array_get_count(Typespec* type);

enum Typespec_Kind
{
    TYPESPEC_INT,
    TYPESPEC_FLOAT,
    TYPESPEC_STRING,
    TYPESPEC_POINTER,
    TYPESPEC_ARRAY,

    TYPESPEC_ENUM,
    TYPESPEC_STRUCT,
    TYPESPEC_FUNCTION,
};

struct Arg
{
    char* name;
    Typespec* type;
};

struct Typespec
{
    Typespec_Kind kind;
    union {
        struct
        {
            i8 bits;
            i8 is_unsigned;
        } Int;
        struct
        {
            i8 bits;
        } Float;
        struct
        {
            u64 len;
        } String;
        struct
        {
            Typespec* pointee;
        } Pointer;
        struct
        {
            Typespec* type;
            int size;
        } Array;
        struct
        {
            char* name;
            char** members;
        } Enum;
        struct
        {
            char* name;
            Arg* members;
        } Struct;
        struct
        {
            char* name;
            Arg* args;
            Typespec* ret_type;
        } Function;
    };
};

#endif
