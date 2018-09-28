#include "globals.h"

#include "map.h" // Map
#include "utility.h" // warning, error
#include <assert.h>  // assert

static Map* symbol_map = NULL;
static Map* builtin_type_map = NULL;

void init_maps(void) {
    symbol_map = make_map();
    builtin_type_map = make_map();
}

void print_symbol_map(void) {
    u64 count = symbol_map->size;
    info("symbol_map count: %d", count);
    for (u64 i = 0; i < count; ++i) {
        info("%s", typespec_to_str(symbol_map->data[i].data));
    }
}

bool is_builtin_type(const char* name) {
    assert(name);
    if (map_get(builtin_type_map, name))
        return true;
    return false;
}
void add_builtin_type(const char* name, Typespec* type) {
    assert(name);
    assert(type);
    if (map_set(builtin_type_map, name, type) == MAP_EXISTS) {
        warning("type redecl: '%s'", name);
    }
    info("added builtin type: %s", name);
}

Typespec* get_builtin_type(const char* name) {
    assert(name);
    Typespec* type = (Typespec*)map_get(builtin_type_map, name);
    if (!type) {
        error("no type with name '%s'", name);
    }
    return type;
}

void add_symbol(const char* name, Typespec* type) {
    assert(name);
    assert(type);
    if (map_set(symbol_map, name, type) == MAP_EXISTS) {
        warning("symbol redecl: '%s'", name);
    }
    info("added symbol: %s", name);
}

Typespec* get_symbol(const char* name) {
    assert(name);
    Typespec* type = (Typespec*)map_get(symbol_map, name);
    if (!type) {
        error("no symbol with name '%s'", name);
    }
    return type;
}
