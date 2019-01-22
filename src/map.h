#ifndef MAP_H
#define MAP_H

#include "typedefs.h"
#include "typespec.h"

//------------------------------------------------------------------------------
//                               Map
//------------------------------------------------------------------------------

typedef struct Map_Element {
    s64   key;
    void* data;
} Map_Element;

typedef struct Map {
    s64          table_size;
    s64          size;
    Map_Element* data;
} Map;

Map*  make_map();
void  map_tests(void);
void* map_get(Map* map, char* key);
void* map_set(Map* map, char* key, void* value);
void* map_set_overwrite(Map* map, char* key, void* value);
#endif
