#include "map.h"
#include "utility.h"
#include <assert.h>
#include <string.h>

#define INITIAL_SIZE 16

u32 hash_it(char* str);

typedef struct
{
    s32 id;
    float val;
} Test_Type;

void map_tests(void)
{
    Map* map = make_map();

    Test_Type t1;
    t1.id = 0;
    t1.val = 3.43f;

    Test_Type t2;
    t2.id = 1;
    t2.val = 6.41f;

    map_set(map, "t1", &t1);
    map_set(map, "t2", &t2);

    assert(((Test_Type*)map_get(map, "t1"))->val == 3.43f);
    assert(((Test_Type*)map_get(map, "t2"))->val == 6.41f);
}

void map_init(Map* map)
{
    map->table_size = INITIAL_SIZE;
    map->size = 0;
    map->data = NULL;
    map->data = xcalloc(INITIAL_SIZE, sizeof(Map_Element));
}

Map* make_map()
{
    Map* m = xmalloc(sizeof(Map));
    map_init(m);
    return m;
}

// Add a pointer to the hashmap with some key
int map_set(Map* map, char* key, void* value)
{
    assert(map);
    assert(key);

    s64 hash_val = hash_it(key);
    for (s64 i = 0; i < map->size; ++i) {
        if (map->data[i].key == hash_val) {
            return MAP_EXISTS;
        }
    }

    // Make sure there is space for it
    if (map->size >= map->table_size) {
        map->data = xrealloc(map->data, (map->table_size + INITIAL_SIZE) * sizeof(Map_Element));
    }

    map->data[map->size].key = hash_val;
    map->data[map->size++].data = value;

    return MAP_SUCCESS;
}

// Get your pointer out of the hashmap with a key
void* map_get(Map* map, char* key)
{
    assert(map);
    assert(key);
    s64 hash_val = hash_it(key);
    for (s64 i = 0; i < map->size; ++i) {
        if (map->data[i].key == hash_val) {
            return map->data[i].data;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
//                               Hash Function
//------------------------------------------------------------------------------

u32 hash_it(char* str)
{
    assert(str);
    u32 hash = 5381;
    s32 c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}
