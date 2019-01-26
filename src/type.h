#ifndef TYPE_H
#define TYPE_H

#include "list.h"
#include "typedefs.h"

//------------------------------------------------------------------------------
//                               type.h
//------------------------------------------------------------------------------

typedef struct Type    Type;
typedef struct Arg     Arg;
typedef enum Type_Kind Type_Kind;

s64   get_offset_in_struct_to_field(Type* type, char* name);
s64   get_size_of_underlying_type(Type* type);
s64   get_size_of_type(Type* Type);
char* get_type_name(Type* Type);

char* type_to_str(Type* type);
char* type_kind_to_str(Type_Kind kind);

Type* make_type_unresolved(char* name);
Type* make_type_void();
Type* make_type_int(s8 bytes, bool is_unsigned);
Type* make_type_float(s8 bytes);
Type* make_type_string(s64 len);
Type* make_type_pointer(Type* pointee);
Type* make_type_array(Type* type, s32 size);
Type* make_type_struct(char* name, List* members);
Type* make_type_enum(char* name, List* members);
Type* make_type_function(char* name, List* args, Type* ret_type);

s64 type_function_get_arg_count(Type* type);
s64 type_array_get_count(Type* type);

enum Type_Kind {
    TYPE_UNRESOLVED,

    TYPE_VOID,

    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_ENUM,
    TYPE_STRUCT,
    TYPE_FUNCTION,

    TYPE_COUNT,
};

struct Arg {
    char* name;
    Type* type;
};

struct Type {
    char*     name;
    Type_Kind kind;
    union {
        struct {
            char* name;
        } Unresolved;
        struct {
            s8 bytes;
            s8 is_unsigned;
        } Int;
        struct {
            s8 bytes;
        } Float;
        struct {
            s64 len;
        } String;
        struct {
            Type* pointee;
        } Pointer;
        struct {
            Type* type;
            s32   size;
        } Array;
        struct {
            char* name;
            List* members;
        } Enum;
        struct {
            char* name;
            List* members;
        } Struct;
        struct {
            char* name;
            List* args;
            Type* ret_type;
        } Function;
    };
};

typedef struct {
    Type** data;
    s64    count;
    s64    allocated;
} Type_Ref_List;

Type_Ref_List make_type_ref_list();
void          type_ref_list_append(Type_Ref_List* l, Type* t);

#endif
