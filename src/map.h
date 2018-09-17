#ifndef MAP_H
#define MAP_H

#include "typedefs.h"
#include "types.h"

//------------------------------------------------------------------------------
//                               Map
//------------------------------------------------------------------------------

typedef Type* any_t;

typedef struct Map Map;
typedef Map Type_Map;
typedef struct Map_Element Map_Element;

#define MAP_EXISTS 1
#define MAP_SUCCESS 0
#define MAP_FAILED -1

struct Map_Element {
    u64 key;
    any_t data;
};
struct Map {
    u64 table_size;
    u64 size;
    Map_Element* data;
};

void map_tests(void);
void map_init(Map* map);
any_t map_get(Map* map, const char* key);
int map_set(Map* map, const char* key, any_t value);

#endif
