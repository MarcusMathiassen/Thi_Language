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

#include "set.h"
#include "utility.h"
#include "map.h"

struct Set {
    Map* elements;
    u32 (*hashf)(void*);
};

Set* make_set(u32 (*hashf)(void*)) {
    Set* s = xmalloc(sizeof(Set));
    s->hashf = hashf;
    s->elements = make_map();
    return s;
}

void* set_add(Set* this, void* data) {
    u32 index = (this->hashf)(data);
    debug("%d", index);
    return map_set(this->elements, strf("%d", index), data);
}

void* set_is_element_of(Set* this, void* data) {
    u32 index = (this->hashf)(data);
    debug("%d", index);
    return map_get(this->elements, strf("%d", index));
}


typedef struct
{
    float val;
} Test_Type;

u32 crc32(void* data) {
    Test_Type* t = data;
    xassert(data);
    u32 hash = 5381;
    u32 c;
    while ((c = t->val++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

void set_tests() {

    // Set* s = make_set(crc32);
    
    // Test_Type t1;
    // t1.val = 3.43f;

    // Test_Type t2;
    // t2.val = 53.43f;

    // set_add(s, &t1);
    // set_add(s, &t1);
    // set_add(s, &t2);
    // set_add(s, &t2);

    // xassert(!set_is_element_of(s, &t1));

    // error("gg");
}
