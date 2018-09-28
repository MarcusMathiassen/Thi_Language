#ifndef MAP_H
#define MAP_H

#include "typedefs.h"
#include "typespec.h"

//------------------------------------------------------------------------------
//                               Map
//------------------------------------------------------------------------------

#define MAP_EXISTS 1
#define MAP_SUCCESS 0
#define MAP_FAILED -1

typedef struct Map_Element {
    u64 key;
    void* data;
} Map_Element;

typedef struct Map {
    u64 table_size;
    u64 size;
    Map_Element* data;
} Map;

Map* make_map(void);
void map_tests(void);
void* map_get(Map* map, const char* key);
int map_set(Map* map, const char* key, void* value);

#endif
