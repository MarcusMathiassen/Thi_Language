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
//                               string.c
//------------------------------------------------------------------------------

#include "string.h"


#include "common.h"
#include "utility.h" // xmalloc, xrealloc
  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   // vsnprintf
#include <stdlib.h>  // free
#include <string.h>  // memcpy

string* make_string(char* str) {
    xassert(str);
    string* s = xmalloc(sizeof(string));
    s->len = 0;
    s->cap = STRING_STARTING_ALLOC;
    s->c_str = xmalloc(s->cap);
    string_append(s, str);
    return s;
}
string* make_string_f(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    char* tmp = vstrf(fmt, args);
    va_end(args);
    string* s = make_string(tmp);
    free(tmp);
    return s;
}
char* string_data(string* this) {
    xassert(this);
    xassert(this->c_str);
    return this->c_str;
}

void string_append(string* this, char* str) {
    xassert(this);
    s64 str_len = xstrlen(str);
    xassert(this->len <= this->cap);
    while (this->len + str_len + 1 >= this->cap) {
        this->cap *= STRING_GROWTH_FACTOR;
        this->c_str = xrealloc(this->c_str, this->cap * sizeof(*this->c_str));
    }
    memcpy(this->c_str + this->len, str, str_len);
    this->len += str_len;
    this->c_str[this->len] = '\0';
}

void string_append_f(string* this, char* fmt, ...) {
    xassert(this);
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    char* tmp = vstrf(fmt, args);
    va_end(args);
    string_append(this, tmp);
    free(tmp);
}
void string_destroy(string* this) {
    xassert(this);
    free(this->c_str);
    free(this);
}

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void) {
    // string test
    string* s = make_string("Hello");
    xassert(s->len == 5);
    tassert(strcmp(string_data(s), "Hello") == 0, "%s", string_data(s));
    string_append(s, ", Marcus Mathiasssen.");
    xassert(s->len == 26);
    xassert(strcmp(string_data(s), "Hello, Marcus Mathiasssen.") == 0);
    string_append(s, " It's nice to see you again. How are you?");
    xassert(s->len == 67);
    xassert(strcmp(string_data(s),
                  "Hello, Marcus Mathiasssen. It's nice to see you "
                  "again. How are you?") == 0);
}
