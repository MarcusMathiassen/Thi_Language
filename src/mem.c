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

#include "mem.h"
#include "utility.h" // error
#include <stdlib.h> // malloc

static u8* memory;
static u64 allocated;
static u64 used;

static u64 find_free_slot(u64 for_bytes);

typedef struct {
    u32 used;
    u32 allocated;
} Header;

//--------------------------------
// Public API
// -------------------------------

void mem_init(u64 initial_size)
{
    memory = malloc(initial_size);
    allocated = initial_size;
}

void* mem_alloc(u64 bytes)
{
    u64 slot = find_free_slot(bytes);
    void* ptr = &memory[slot];
    ptr += sizeof(Header);
    used += bytes;
    return ptr;
}

void mem_free(void* ptr)
{
    Header* head = ptr - sizeof(Header);
    used -= head->used;
}

void mem_test()
{
    mem_init(kilobytes(1));

    int* ints = mem_alloc(sizeof(int) * 10);
    foreach(i, 10) {
        ints[i] = i;
        warning("%d", ints[i]);
    }

    mem_free(ints);
    error("[arena::no_more_space] allocated: %llu used: %llu", allocated, used);
}

//--------------------------------
// Private API
// -------------------------------

static u64 find_free_slot(u64 for_bytes)
{
    if (used+for_bytes >= allocated) {
        error("[arena::no_more_space] allocated: %llu used: %llu requested: %llu", allocated, used, for_bytes);
    }
    return used + for_bytes;
}
