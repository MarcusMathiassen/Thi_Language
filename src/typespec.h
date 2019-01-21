#ifndef TYPESPEC_H
#define TYPESPEC_H

#include "list.h"
#include "typedefs.h"

//------------------------------------------------------------------------------
//                               typespec.h
//------------------------------------------------------------------------------

typedef struct Typespec    Typespec;
typedef struct Arg         Arg;
typedef enum Typespec_Kind Typespec_Kind;

s64 get_offset_in_struct_to_field(Typespec* type, char* name);
s64 get_size_of_underlying_typespec(Typespec* type);
s64 get_size_of_typespec(Typespec* Typespec);

char* typespec_to_str(Typespec* type);

Typespec* make_typespec_int(s8 bytes, bool is_unsigned);
Typespec* make_typespec_float(s8 bytes);
Typespec* make_typespec_string(s64 len);
Typespec* make_typespec_pointer(Typespec* pointee);
Typespec* make_typespec_array(Typespec* type, s32 size);
Typespec* make_typespec_struct(char* name, List* members);
Typespec* make_typespec_enum(char* name, List* members);
Typespec* make_typespec_function(char* name, List* args, Typespec* ret_type);

s64 typespec_function_get_arg_count(Typespec* type);
s64 typespec_array_get_count(Typespec* type);

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
    char*     name;
    Typespec* type;
};

struct Typespec
{
    Typespec_Kind kind;
    union {
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
            Typespec* pointee;
        } Pointer;
        struct
        {
            Typespec* type;
            s32       size;
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
            char*     name;
            List*     args;
            Typespec* ret_type;
        } Function;
    };
};

#endif
