#include "string.h"

#include "list.h"
#include "typedefs.h"
#include "utility.h" // xmalloc, xrealloc
#include <assert.h>  // assert
#include <stdarg.h>  // va_list, va_start, va_end
#include <stdio.h>   // printf, vprintf
#include <stdlib.h>  // free
#include <string.h>  // memcpy

string make_string(char* str) {
    assert(str);
    string s;
    s64    str_len = strlen(str);
    s.c_str        = xmalloc(str_len + 1);
    s.len          = str_len;
    memcpy(s.c_str, str, str_len);
    s.c_str[s.len] = 0;
    return s;
}

string make_string_f(char* fmt, ...) {
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

void free_string(string* s) { free(s->c_str); }

//------------------------------------------------------------------------------
//                              Intern String
//------------------------------------------------------------------------------

typedef struct Intern_Str {
    s64   len;
    char* str;
} Intern_Str;

List* interns;
void  init_interns_list() { interns = make_list(); }
char* str_intern_range(char* start, char* end) {
    s64 len = end - start;
    LIST_FOREACH(interns) {
        Intern_Str* intern = (Intern_Str*)it->data;
        if (intern->len == len && strncmp(intern->str, start, len) == 0) {
            return intern->str;
        }
    }
    char* str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = 0;

    Intern_Str* intern = xmalloc(sizeof(Intern_Str));
    intern->len        = len;
    intern->str        = str;
    list_append(interns, intern);
    return str;
}
char* str_intern(char* str) { return str_intern_range(str, str + strlen(str)); }

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
    assert(strcmp(s.c_str, "Hello, Marcus Mathiasssen. It's nice to see you "
                           "again. How are you?") == 0);
}
