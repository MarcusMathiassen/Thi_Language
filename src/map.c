#define MAP_DEEMED_FULL_PERCENTAGE 0.75
#define MAP_TABLE_GROWTH_RATE 2
#define MAP_STARTING_TABLE_SIZE 16

typedef struct
{
    char* key;
    void* value;
} Map_Element;

typedef struct
{
    Map_Element* elements;
    s64 count;
    s64 table_size;
} Map;

#define map_foreach(map) for (Map_Element* it = map->elements; it != &(map->elements[map->table_size]); ++it) if (it->key)

INTERNAL Map* make_map(void);
INTERNAL void* map_set(Map* map, char* key, void* value);
INTERNAL void* map_get(Map* map, char* key);
INTERNAL void map_destroy(Map* map);
INTERNAL Map* make_map_with_initial_size(s64 initial_size);
INTERNAL s64 map_count(Map* map);

// djb2
INTERNAL u32 hash(char* str)
{
    xassert(str);
    u32 hash = 5381;
    s32 c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

INTERNAL inline Map_Element* find_slot_with_key(Map* map, char* key)
{
    xassert(map && key);
    u32 index = hash(key);
    Map_Element* probe;
    while ((probe = &map->elements[index++ % map->table_size])->key)
        if (strcmp(key, probe->key) == 0)
            break;
    return probe;
}

INTERNAL inline Map_Element* find_empty_slot(Map* map, char* key)
{
    xassert(map && key);
    u32 index = hash(key);
    Map_Element* probe;
    while ((probe = &map->elements[index++ % map->table_size])->key);
    return probe;
}

INTERNAL Map* make_map_with_initial_size(s64 initial_size)
{
    tassert(initial_size > 0, "initial_size = %lld", initial_size);
    Map* map = xmalloc(sizeof(Map));
    map->count = 0;
    map->table_size = initial_size;
    map->elements = xcalloc(map->table_size, sizeof(Map_Element));
    return map;
}

INTERNAL Map* make_map()
{
    return make_map_with_initial_size(MAP_STARTING_TABLE_SIZE);
}

INTERNAL void map_destroy(Map* map)
{
    free(map->elements);
    free(map);
    map = NULL;
}

INTERNAL s64 map_count(Map* map)
{
    return map->count;
}

inline INTERNAL void map_increase_table_size_and_rehash(Map* map, s64 new_table_size) {
    Map_Element* nelements = xcalloc(new_table_size, sizeof(Map_Element));
    map_foreach(map) {
        u32 index = hash(it->key);        
        Map_Element* probe;
        while ((probe = &nelements[index++ % new_table_size])->key);
        probe->key = it->key;
        probe->value = it->value;
    }
    free(map->elements);
    map->elements = nelements;
    map->table_size = new_table_size;
}

INTERNAL void* map_set(Map* map, char* key, void* value)
{
    xassert(map && key && value);
    if ((float)map->count / map->table_size > MAP_DEEMED_FULL_PERCENTAGE) {
        map_increase_table_size_and_rehash(map, map->table_size * MAP_TABLE_GROWTH_RATE);
    }
    Map_Element* slot = find_empty_slot(map, key);
    slot->key = key;
    slot->value = value;
    ++map->count;
    return value;
}

INTERNAL void* map_get(Map* map, char* key)
{
    xassert(map && key);
    Map_Element* slot = find_slot_with_key(map, key);
    return slot->value;
}
