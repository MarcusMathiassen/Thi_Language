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

typedef struct Map_Element
{
    s64 key;
    void* data;
} Map_Element;

typedef struct Map
{
    s64 table_size;
    s64 size;
    Map_Element* data;
} Map;

Map* make_map();
void map_tests(void);
void* map_get(Map* map, char* key);
int map_set(Map* map, char* key, void* value);

#endif
