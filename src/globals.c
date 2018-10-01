#include "globals.h"

#include "list.h"    // List
#include "map.h"     // Map
#include "stack.h"   // Stack
#include "utility.h" // warning, error, xmalloc
#include <assert.h>  // assert

static Map* symbol_map = NULL;
static Map* builtin_type_map = NULL;
static Stack* timer_stack = NULL;
static List* timers = NULL;

void initilize_globals(void)
{
    symbol_map = make_map();
    builtin_type_map = make_map();

    timers = make_list();
    timer_stack = make_stack();
}

void print_symbol_map(void)
{
    u64 count = symbol_map->size;
    info("symbol_map count: %d", count);
    for (u64 i = 0; i < count; ++i) {
        info("%s", typespec_to_str(symbol_map->data[i].data));
    }
}

bool is_builtin_type(const char* name)
{
    assert(name);
    if (map_get(builtin_type_map, name)) return true;
    return false;
}

void add_builtin_type(const char* name, Typespec* type)
{
    assert(name);
    assert(type);
    if (map_set(builtin_type_map, name, type) == MAP_EXISTS) {
        warning("type redecl: '%s'", name);
    }
    info("added builtin type: %s of type '%s'", name, typespec_to_str(type));
}

Typespec* get_builtin_type(const char* name)
{
    assert(name);
    Typespec* type = (Typespec*)map_get(builtin_type_map, name);
    if (!type) {
        error("no type with name '%s'", name);
    }
    return type;
}

void add_symbol(const char* name, Typespec* type)
{
    assert(name);
    assert(type);
    if (map_set(symbol_map, name, type) == MAP_EXISTS) {
        warning("symbol redecl: '%s'", name);
    }
    info("added symbol: '%s' of type '%s'", name, typespec_to_str(type));
}

Typespec* get_symbol(const char* name)
{
    assert(name);
    Typespec* type = (Typespec*)map_get(symbol_map, name);
    if (!type) {
        error("no symbol with name '%s'", name);
    }
    return type;
}

List* get_timers(void) { return timers; }

void push_timer(const char* desc)
{
    Timer* tm = xmalloc(sizeof(Timer));
    tm->ms = get_time();
    tm->desc = desc;
    stack_push(timer_stack, tm);
}

void pop_timer(void)
{
    Timer* tm = (Timer*)stack_pop(timer_stack);
    tm->ms = get_time() - tm->ms;
    list_append(timers, tm);
}
