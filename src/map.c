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

#include "map.h"
#include "constants.h"
#include "utility.h"
#include <assert.h>
#include <string.h>

static u32 hash(char* str) {
    assert(str);
    u32 hash = 5381;
    s32 c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

typedef struct
{
    s32 id;
    float val;
} Test_Type;

static Map map_create_with_custom_size(s64 initial_size) {
    Map map;
    map.count = 0;
    map.table_size = initial_size;
    map.elements = xcalloc(map.table_size, map.table_size * sizeof(Map_Element));
    return map;
}

Map map_create() {
    Map map;
    map.count = 0;
    map.table_size = DEFAULT_MAP_STARTING_TABLE_SIZE;
    map.elements = xcalloc(map.table_size, map.table_size * sizeof(Map_Element));
    return map;
}

Map* make_map() {
    Map* map = xmalloc(sizeof(Map));
    map->count = 0;
    map->table_size = DEFAULT_MAP_STARTING_TABLE_SIZE;
    map->elements = xcalloc(map->table_size, map->table_size * sizeof(Map_Element));
    return map;
}

void map_tests(void) {

    Map map = map_create();

    Test_Type t1;
    t1.id = 0;
    t1.val = 3.43f;

    Test_Type t2;
    t2.id = 1;
    t2.val = 6.41f;

    map_set(&map, "t1", &t1);
    map_set(&map, "t2", &t2);
    map_set(&map, "t3", &t2);
    map_set(&map, "t4", &t2);
    map_set(&map, "t5", &t2);
    map_set(&map, "t6", &t2);
    map_set(&map, "t7", &t2);
    map_set(&map, "t8", &t2);
    map_set(&map, "t9", &t2);
    map_set(&map, "t10", &t2);
    map_set(&map, "t11", &t2);
    map_set(&map, "t12", &t2);
    map_set(&map, "t13", &t2);

    assert(((Test_Type*)map_get(&map, "t1"))->val == 3.43f);
    assert(((Test_Type*)map_get(&map, "t2"))->val == 6.41f);

    // error("gerg");
}

// Get your pointer out of the hashmap with a key
void* p_map_get(Map* map, char* key) {
    assert(map && key);
    u32 index = hash(key);
    Map_Element* probe = NULL;
    u32 i = 0;
    while (i < map->table_size && (probe = &map->elements[(index + i++) % map->table_size])->key) {
        // @Todo(marcus): when we've made sure to intern all keys, we can swap this with a pointer comparison
        if (strcmp(key, probe->key) == 0) break;
    }
    if (i == map->table_size) error("no element with key %s", key);
    tassert(probe->value, "key %s value was NULL.", key);
    return probe->value;
}

void* p_map_set(Map* map, char* key, void* value) {
    assert(map && key && value);

    // We make sure to keep the load under 75%
    if ((float)map->count / map->table_size > 0.75) {
        s64 last_table_size = map->table_size;
        map->table_size <<= 1;
        // info("map_set -- allocated %d more space", map->table_size);
        Map nmap = map_create_with_custom_size(map->table_size);
        for (int k = 0; k < last_table_size; ++k) {
            Map_Element* probe = &map->elements[k];
            if (probe->key) {
                map_set(&nmap, probe->key, probe->value);
            }
        }
        free(map->elements);
        assert(map->count == nmap.count);
        *map = nmap;
    }

    u32 index = hash(key);
    // info("\nmap_set -- key: %s, value: %zu, hash: %d:", key, value, index);

    // Look for an open slot..
    u32 i = 0; // we hold an iterator to make sure we'
    Map_Element* probe = NULL;
    while (i != map->table_size && (probe = &map->elements[(index + i++) % map->table_size])->key) {
        // info("on key: %s, value: %zu", probe->key, probe->value);
        if (strcmp(key, probe->key) == 0) {
            error("key %s already exists in map %zu", key, map);
        }
    }

    // We've found our open slot. Drop it right in.
    probe->key = key;
    probe->value = value;

    // for (u32 j = 0; j < map->table_size; ++j) {
    //     Map_Element* probe = &map->elements[j];
    //     info("%d: key: %s, value: %s", j, probe->key ? ucolor(probe->key) : probe->key, ucolor(strf("%zu", probe->value)));
    // }
    // info("");

    ++map->count;

    return value;
}

s64 p_map_count(Map* map) {
    return map->count;
}
s64 d_map_count(Map* map, char* file, char* func, int line) {
    tassert(map, "%s:%s:%d", file, func, line);
    return p_map_count(map);
}

void* d_map_get(Map* map, char* key, char* file, char* func, int line) {
    tassert(map && key, "%s:%s:%d", file, func, line);
    return p_map_get(map, key);
}
void* d_map_set(Map* map, char* key, void* value, char* file, char* func, int line) {
    tassert(map && key && value, "%s:%s:%d", file, func, line);
    return p_map_set(map, key, value);
}
