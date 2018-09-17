#include "string.h"

#include "typedefs.h"
#include "utility.h" // xmalloc, xrealloc, xstrlen
#include <assert.h> // assert
#include <stdlib.h>

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
