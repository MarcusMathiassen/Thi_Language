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
//                               map.c
//------------------------------------------------------------------------------

#include "map.h"
#include "constants.h"
#include "utility.h"

#include <string.h>
#include <stdlib.h> // free

typedef struct
{
    s32 id;
    float val;
} Test_Type;

void map_tests(void) {

    Map* map = make_map();

    Test_Type t1;
    t1.id = 0;
    t1.val = 3.43f;

    Test_Type t2;
    t2.id = 1;
    t2.val = 6.41f;

    map_set(map, "t1", &t1);
    map_set(map, "t2", &t2);
    map_set(map, "t3", &t2);
    map_set(map, "t4", &t2);
    map_set(map, "t5", &t2);
    map_set(map, "t6", &t2);
    map_set(map, "t7", &t2);
    map_set(map, "t8", &t2);
    map_set(map, "t9", &t2);
    map_set(map, "t10", &t2);
    map_set(map, "t11", &t2);
    map_set(map, "t12", &t2);
    map_set(map, "t13", &t2);

    xassert(((Test_Type*)map_get(map, "t1"))->val == 3.43f);
    xassert(((Test_Type*)map_get(map, "t2"))->val == 6.41f);

    map_foreach(map) {
        char* key = it->key;
        Test_Type* t =  it->value;
        info("key: %s, value: %f", key, t->val);
    }   
}

static u32 hash(char* str) {
    xassert(str);
    u32 hash = 5381;
    s32 c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

static inline Map_Element* find_slot_with_key(Map* map, char* key) {
    xassert(map && key);
    u32 index = hash(key);
    Map_Element* probe = NULL;
    while ((probe = &map->elements[index++ % map->table_size])->key)
        if (strcmp(key, probe->key) == 0)
            break;
    return probe;
}

static inline Map_Element* find_empty_slot(Map* map, char* key) {
    xassert(map && key);
    u32 index = hash(key);
    Map_Element* probe = NULL;
    while ((probe = &map->elements[index++ % map->table_size])->key)
        tassert(strcmp(key, probe->key) != 0, "key %s already exists in map %zu", key, map);
    return probe;
}

Map* make_map_with_initial_size(s64 initial_size) {
    tassert(initial_size > 0, "initial_size = %lld", initial_size);
    Map* map = xmalloc(sizeof(Map));
    map->count = 0;
    map->table_size = initial_size;
    map->elements = xcalloc(map->table_size, map->table_size * sizeof(Map_Element));
    return map;
}

Map* make_map() {
    return make_map_with_initial_size(DEFAULT_MAP_STARTING_TABLE_SIZE);
}

void map_destroy(Map* map) {
    free(map->elements);
    free(map);
}

s64 map_count(Map* map) {
    return map->count;
}

void* p_map_set(Map* map, char* key, void* value) {
    xassert(map && key && value);
    if ((float)map->count / map->table_size > 0.75f) {
        s64 last_table_size = map->table_size;
        map->table_size <<= 1;
        Map* nmap = make_map_with_initial_size(map->table_size);
        foreach(k, last_table_size) {
            Map_Element* probe = &map->elements[k];
            if (probe->key) 
                map_set(nmap, probe->key, probe->value);
        }
        free(map->elements);
        xassert(map_count(map) == map_count(nmap));
        *map = *nmap;
    }
    Map_Element* slot = find_empty_slot(map, key);
    slot->key = key;
    slot->value = value;
    ++map->count;
    return value;
}
void* p_map_get(Map* map, char* key) {
    xassert(map && key);
    Map_Element* slot = find_slot_with_key(map, key);
    tassert(slot->value, "key %s value was NULL.", key);
    return slot->value;
}
void* d_map_set(Map* map, char* key, void* value, char* file, char* func, int line) {
    xassert(map && key && value);
    if ((float)map->count / map->table_size > 0.75f) {
        s64 last_table_size = map->table_size;
        map->table_size <<= 1;
        Map* nmap = make_map_with_initial_size(map->table_size);
        foreach(k, last_table_size) {
            Map_Element* probe = &map->elements[k];
            if (probe->key) 
                map_set(nmap, probe->key, probe->value);
        }
        free(map->elements);
        xassert(map_count(map) == map_count(nmap));
        *map = *nmap;
    }

    u32 index = hash(key);
    Map_Element* slot = NULL;
    while ((slot = &map->elements[index++ % map->table_size])->key);
        // tassert(strcmp(key, slot->key) != 0, "[%s:%s:%d] key %s already exists in map %zu", file, func, line, key, key, map);

    slot->key = key;
    slot->value = value;
    ++map->count;
    return value;
}

void* d_map_get(Map* map, char* key, char* file, char* func, int line) {
    xassert(map && key);
    Map_Element* slot = find_slot_with_key(map, key);
    // tassert(slot->value, "[%s:%s:%d]key %s value was NULL.", file, func, line, key);
    return slot->value;
}
