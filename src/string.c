#include "string.h"

#include "stretchy_buffer.h" // sb_push, sb_count
#include "typedefs.h"
#include "utility.h" // xmalloc, xrealloc, xstrlen
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   // printf, vprintf
#include <stdlib.h>  // free
#include <string.h>  // memcpy

string make_string(const char* str)
{
    assert(str);
    string s;
    u64 str_len = strlen(str);
    s.c_str = xmalloc(str_len + 1);
    s.len = str_len;
    memcpy(s.c_str, str, str_len);
    s.c_str[s.len] = 0;
    return s;
}

string make_string_f(const char* fmt, ...)
{
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    u64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);

    string s;
    s.c_str = xmalloc(n);
    s.len = n;

    va_start(args, fmt);
    vsnprintf(s.c_str, n, fmt, args);
    va_end(args);

    return s;
}

void append_string_f(string* s, const char* fmt, ...)
{
    assert(s);
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    u64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);

    char* temp = xmalloc(n);

    va_start(args, fmt);
    vsnprintf(temp, n, fmt, args);
    va_end(args);

    assert(n != 0);
    s->c_str = xrealloc(s->c_str, s->len + n + 1);
    memcpy(s->c_str + s->len, temp, n);
    s->len += n;
    s->c_str[s->len] = 0;

    free(temp);
}
void append_string(string* s, const char* str)
{
    assert(s);
    assert(str);
    u64 str_len = xstrlen(str);
    assert(str_len != 0);
    s->c_str = xrealloc(s->c_str, s->len + str_len + 1);
    memcpy(s->c_str + s->len, str, str_len);
    s->len += str_len;
    s->c_str[s->len] = 0;
}

void free_string(string* s) { free(s->c_str); }

//------------------------------------------------------------------------------
//                              Intern String
//------------------------------------------------------------------------------

typedef struct Intern_Str
{
    u64 len;
    const char* str;
} Intern_Str;

static Intern_Str* interns;
static u64 interns_mem_alloc_size = 0;
const char* str_intern_range(const char* start, const char* end)
{
    u64 len = end - start;
    for (int i = 0; i < sb_count(interns); ++i) {
        if (interns[i].len == len && strncmp(interns[i].str, start, len) == 0) {
            return interns[i].str;
        }
    }
    char* str = xmalloc(len + 1);
    interns_mem_alloc_size += len + 1;
    memcpy(str, start, len);
    str[len] = 0;
    sb_push(interns, ((Intern_Str){len, str}));
    return str;
}

const char* str_intern(const char* str) { return str_intern_range(str, str + strlen(str)); }

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void)
{
    // string test
    string s = make_string("Hello");
    assert(s.len == 5);
    assert(strcmp(s.c_str, "Hello") == 0);
    append_string(&s, ", Marcus Mathiasssen.");
    assert(s.len == 26);
    assert(strcmp(s.c_str, "Hello, Marcus Mathiasssen.") == 0);
    append_string(&s, " It's nice to see you again. How are you?");
    assert(s.len == 67);
    assert(strcmp(s.c_str, "Hello, Marcus Mathiasssen. It's nice to see you "
                           "again. How are you?") == 0);
}
