#ifndef STRING_H
#define STRING_H

#include "typedefs.h"
typedef struct
{
    char* c_str;
    s64   len;
} string;

string
make_string(char* str);
string
make_string_f(char* fmt, ...);
void append_string(string* s, char* str);
void append_string_f(string* s, char* fmt, ...);
void free_string(string* s);

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void);

#endif
