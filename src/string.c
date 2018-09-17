#include "string.h"

#include "typedefs.h"
#include "utility.h" // xmalloc, xrealloc, xstrlen
#include <assert.h> // assert
#include <string.h> // memcpy
#include <stdlib.h> // free
#include "stretchy_buffer.h"  // sb_push, sb_count

string* make_string(const char* str, u64 initial_size)
{
    assert(str);
    assert(initial_size != 0);
    string* s = xmalloc(sizeof(string));
    u64 str_len = xstrlen(str);
    if (str_len > initial_size)
        initial_size = str_len;
    s->data = xmalloc(initial_size);
    s->data_len = str_len;
    memcpy(s->data, str, str_len);
    s->alloc_len = initial_size;
    s->data[str_len] = 0;
    return s;
}
void append_string(string* s, const char* str)
{
    assert(s);
    assert(str);
    u64 str_len = xstrlen(str);
    // allocate more space if needed
    if (s->alloc_len < s->data_len + str_len)
    {
        s->alloc_len += str_len;
        s->data = xrealloc(s->data, s->alloc_len);
    }
    // Copy over the data
    memcpy(s->data+s->data_len, str, str_len);
    s->data_len += str_len;
    s->data[s->data_len] = 0;
}
void free_string(string* s)
{
    free(s->data);
}


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
    for (int i = 0; i < sb_count(interns); ++i)
    {
        if (interns[i].len == len && strncmp(interns[i].str, start, len) == 0)
        {
            return interns[i].str;
        }
    }
    char* str = xmalloc(len + 1);
    interns_mem_alloc_size += len+1;
    memcpy(str, start, len);
    str[len] = 0;
    sb_push(interns, ((Intern_Str) { len, str }));
    return str;
}

const char* str_intern(const char* str)
{
    return str_intern_range(str, str + strlen(str));
}

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void string_tests(void)
{
     // string test
    string* s = make_string("Hello", 50);
    assert(s->data_len == 5);
    assert(s->alloc_len == 50);
    assert(strcmp(s->data, "Hello") == 0);
    append_string(s, ", Marcus Mathiasssen.");
    assert(s->data_len == 26);
    assert(s->alloc_len == 50);
    assert(strcmp(s->data, "Hello, Marcus Mathiasssen.") == 0);
    append_string(s, " It's nice to see you again. How are you?");
    assert(s->data_len == 67);
    assert(s->alloc_len == 91);
    assert(strcmp(s->data, "Hello, Marcus Mathiasssen. It's nice to see you again. How are you?") == 0);
}
