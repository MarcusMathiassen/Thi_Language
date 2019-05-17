#include "string.h"

#include "list.h"
#include "typedefs.h"
#include "utility.h" // xmalloc, xrealloc
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   // printf, vprintf
#include <stdlib.h>  // free
#include <string.h>  // memcpy

string
make_string(char* str) {
    assert(str);
    string s;
    s64    str_len = strlen(str);
    s.c_str        = xmalloc(str_len + 1);
    s.len          = str_len;
    memcpy(s.c_str, str, str_len);
    s.c_str[s.len] = 0;
    return s;
}

string
make_string_f(char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);

    string s;
    s.c_str = xmalloc(n);
    s.len   = n;

    va_start(args, fmt);
    vsnprintf(s.c_str, n, fmt, args);
    va_end(args);

    return s;
}

void append_string(string* s, char* str) {
    assert(s);
    assert(str);
    s64 str_len = strlen(str);
    assert(str_len != 0);
    s->c_str = xrealloc(s->c_str, s->len + str_len + 1);
    memcpy(s->c_str + s->len, str, str_len);
    s->len += str_len;
    s->c_str[s->len] = 0;
}

void append_string_f(string* s, char* fmt, ...) {
    assert(s);
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);

    char* str = xmalloc(n);

    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);

    append_string(s, str);

    free(str);
}

void free_string(string* s) {
    free(s->c_str);
}

string string_create(char* str) {
    assert(str);
    string s;
    s64    str_len = strlen(str);
    s.c_str        = xmalloc(str_len + 1);
    s.len          = str_len;
    memcpy(s.c_str, str, str_len);
    s.c_str[s.len] = 0;
    return s;
}
string string_create_f(char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);
    string s;
    s.c_str = xmalloc(n);
    s.len   = n;
    va_start(args, fmt);
    vsnprintf(s.c_str, n, fmt, args);
    va_end(args);
    return s;
}
char* string_data(string* this) {
    assert(this);
    assert(this->c_str);
    return this->c_str;
}
void string_append(string* this, char* str) {
    assert(this);
    assert(str);
    s64 str_len = strlen(str);
    assert(str_len != 0);

    char* c_str = this->c_str;
    s64   len   = this->len;

    c_str = xrealloc(c_str, len + str_len + 1);
    memcpy(c_str + len, str, str_len);
    len += str_len;
    c_str[len] = 0;

    this->c_str = c_str;
    this->len   = len;
}
void string_append_f(string* this, char* fmt, ...) {
    assert(this);
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);
    char* str = xmalloc(n);
    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);
    append_string(this, str);
    free(str);
}
void string_destroy(string* this) {
    assert(this);
    free(this->c_str);
    free(this);
}

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void) {
    // string test
    string s = make_string("Hello");
    assert(s.len == 5);
    assert(strcmp(s.c_str, "Hello") == 0);
    append_string(&s, ", Marcus Mathiasssen.");
    assert(s.len == 26);
    assert(strcmp(s.c_str, "Hello, Marcus Mathiasssen.") == 0);
    append_string(&s, " It's nice to see you again. How are you?");
    assert(s.len == 67);
    assert(strcmp(s.c_str,
                  "Hello, Marcus Mathiasssen. It's nice to see you "
                  "again. How are you?") == 0);
}
