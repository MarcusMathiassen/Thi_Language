#include "typespec.h"

#include "stretchy_buffer.h" // sb_free
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

u64 get_size_of_typespec(Typespec *type) {
  switch (type->kind) {
  case TYPESPEC_INT:
    return type->Int.bits / 8;
  case TYPESPEC_FUNCTION: {
    u64 accum_size = 0;
    for (int i = 0; i < sb_count(type->Function.args); ++i) {
      accum_size += get_size_of_typespec(type->Function.args[i].type);
    }
    return accum_size;
  }
  default:
    error("not implemented kind %d", type->kind);
  }
  return 0;
}

char *typespec_to_str(Typespec *type) {
  switch (type->kind) {
  case TYPESPEC_INT:
    return strf((type->Int.is_unsigned ? "u"
                                       : "i"
                                         "%d"),
                type->Int.bits);
  case TYPESPEC_FUNCTION: {
    string *str = make_string(strf("%s :: (", type->Function.name));
    strf("func. name: %d", type->Function.name);
    Arg *args = type->Function.args;
    u64 arg_count = sb_count(args);
    if (arg_count)
      for (int i = 0; i < sb_count(args); ++i) {
        append_string(
            str, strf("%s: %s", args[i].name, typespec_to_str(args[i].type)));
      }

    append_string(str,
                  strf(") -> %s", typespec_to_str(type->Function.ret_type)));
    return str->c_str;
  }
  default:
    warning("not implemented kind %d", type->kind);
  }
  return NULL;
}

//------------------------------------------------------------------------------
//                               Type Maker Functions
//------------------------------------------------------------------------------

Typespec *make_typespec(Typespec_Kind kind) {
  Typespec *t = xmalloc(sizeof(Typespec));
  t->kind = kind;
  return t;
}

Typespec *make_typespec_int(i8 bits, bool is_unsigned) {
  assert(bits > 7 && bits < 65);
  assert(is_unsigned == 1 || is_unsigned == 0);
  Typespec *t = make_typespec(TYPESPEC_INT);
  t->Int.bits = bits;
  t->Int.is_unsigned = is_unsigned;
  return t;
}

Typespec *make_typespec_float(i8 bits) {
  assert(bits > 7 && bits < 65);
  Typespec *t = make_typespec(TYPESPEC_FLOAT);
  t->Float.bits = bits;
  return t;
}

Typespec *make_typespec_pointer(Typespec *pointee) {
  Typespec *t = make_typespec(TYPESPEC_FLOAT);
  t->Pointer.pointee = pointee;
  return t;
}

Typespec *make_typespec_struct(const char *name, Arg *members) {
  assert(name);
  Typespec *t = make_typespec(TYPESPEC_STRUCT);
  t->Struct.name = name;
  t->Struct.members = members;
  return t;
}

Typespec *make_typespec_function(const char *name, Arg *args,
                                 Typespec *ret_type) {
  assert(name);
  Typespec *t = make_typespec(TYPESPEC_FUNCTION);
  t->Function.name = name;
  t->Function.args = args;
  t->Function.ret_type = ret_type;
  return t;
}
