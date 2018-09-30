#include "map.h"
#include "utility.h"
#include <assert.h>
#include <string.h>

#define INITIAL_SIZE 16

static u32 hash_it(const char* str);

typedef struct
{
    int id;
    float val;
} Test_Type;

void map_tests(void)
{

    Map* test_map = make_map();

    Test_Type* marcus = xmalloc(sizeof(Test_Type));
    marcus->id = 0;
    marcus->val = 3.43f;

    Test_Type* aylin = xmalloc(sizeof(Test_Type));
    aylin->id = 1;
    aylin->val = 6.41f;

    map_set(test_map, "marcus", marcus);
    map_set(test_map, "aylin", aylin);
    assert(((Test_Type*)map_get(test_map, "marcus"))->val == 3.43f);
    assert(((Test_Type*)map_get(test_map, "aylin"))->val == 6.41f);
}

Map* make_map(void)
{
    Map* map = xmalloc(sizeof(Map));
    map->data = NULL;
    map->data = xcalloc(INITIAL_SIZE, sizeof(Map_Element));
    map->table_size = INITIAL_SIZE;
    map->size = 0;
    return map;
}

// Add a pointer to the hashmap with some key
int map_set(Map* map, const char* key, void* value)
{
    assert(map);
    assert(key);

    u64 hash_val = hash_it(key);
    for (u64 i = 0; i < map->size; ++i) {
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
void* map_get(Map* map, const char* key)
{
    assert(map);
    assert(key);
    u64 hash_val = hash_it(key);
    for (u64 i = 0; i < map->size; ++i) {
        if (map->data[i].key == hash_val) {
            return map->data[i].data;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
//                               Hash Function
//------------------------------------------------------------------------------

static u32 hash_it(const char* str)
{
    assert(str);
    u32 hash = 5381;
    i32 c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}
