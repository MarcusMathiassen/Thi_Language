#ifndef MAP_H
#define MAP_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                               map.h
//------------------------------------------------------------------------------

typedef struct Map         Map;
typedef struct Map_Element Map_Element;

struct Map_Element {
    s64   key;
    void* data;
};

struct Map {
    s64          table_size;
    s64          size;
    Map_Element* data;
};

Map*  make_map(void);
void  map_tests(void);
void* map_get(Map* map, char* key);
void* map_set(Map* map, char* key, void* value);
void* map_set_overwrite(Map* map, char* key, void* value);
#endif
