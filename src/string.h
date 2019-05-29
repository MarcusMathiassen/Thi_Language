// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

//------------------------------------------------------------------------------
//                               string.h
//------------------------------------------------------------------------------

#ifndef STRING_H
#define STRING_H
#include "typedefs.h"
#include "utility.h"

typedef struct {
    char* c_str;
    s64 len;
    s64 cap;
} string;

typedef struct {
    string* str;
    bool with_newlines;
    Loc_Info last;
    u64 indentation_level;
} String_Context;

string* string_create(char* str);
string* string_create_f(char* fmt, ...);
void string_destroy(string* this);
char* string_data(string* this);
void string_append(string* this, char* str);
void string_append_f(string* this, char* fmt, ...);
void string_tests(void);

#endif
