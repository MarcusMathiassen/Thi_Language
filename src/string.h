#ifndef STRING_H
#define STRING_H

#include "typedefs.h"
typedef struct
{
    char* c_str;
    u64 len;
} string;

string make_string(const char* str);
void append_string(string* s, const char* str);
void free_string(string* s);

const char* str_intern_range(const char* start, const char* end);
const char* str_intern(const char* str);

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void);

#endif
