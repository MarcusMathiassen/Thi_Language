#ifndef STRING_H
#define STRING_H

#include "typedefs.h"
typedef struct
{
    char* c_str;
    i64 len;
} string;

string make_string(char* str);
string make_string_f(char* fmt, ...);
void append_string(string* s, char* str);
void append_string_f(string* s, char* fmt, ...);
void free_string(string* s);

char* str_intern_range(char* start, char* end);
char* str_intern(char* str);

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void);

#endif
