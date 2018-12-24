#include "globals.h"

#include "list.h"    // List
#include "map.h"     // Map
#include "stack.h"   // Stack
#include "string.h"  // string
#include "utility.h" // warning, error, xmalloc
#include <assert.h>  // assert

bool detailed_print = true;
string* current_output;

List foreign_function_list;
Map symbol_map;
Map macro_map;
Map builtin_type_map;
Stack timer_stack;
List timers;

string source_file;
string current_directory;

void set_source_file(const char* file_name) { source_file = make_string(file_name); }
char* get_source_file() { return source_file.c_str; }
void set_current_dir(const char* dir_name) { current_directory = make_string(dir_name); }
char* get_current_dir() { return current_directory.c_str; }

void initilize_globals(void)
{
    list_init(&foreign_function_list);
    map_init(&symbol_map);
    map_init(&macro_map);
    map_init(&builtin_type_map);
    list_init(&timers);
    stack_init(&timer_stack);
}

void print_symbol_map(void)
{
    u64 count = symbol_map.size;
    info("symbol_map count: %d", count);
    for (u64 i = 0; i < count; ++i) {
        info("%s", typespec_to_str(symbol_map.data[i].data));
    }
}

bool is_builtin_type(const char* name)
{
    assert(name);
    if (map_get(&builtin_type_map, name)) return true;
    return false;
}

void add_builtin_type(const char* name, Typespec* type)
{
    assert(name);
    assert(type);
    if (map_set(&builtin_type_map, name, type) == MAP_EXISTS) {
        warning("type redecl: '%s'", name);
    }
    info("added builtin type: %s of type '%s'", name, typespec_to_str(type));
}

Typespec* get_builtin_type(const char* name)
{
    assert(name);
    Typespec* type = (Typespec*)map_get(&builtin_type_map, name);
    if (!type) {
        error("no type with name '%s'", name);
    }
    return type;
}

void add_foreign_function(const char* name, Typespec* type)
{
    assert(type);
    list_append(&foreign_function_list, type);
    info("added foreign function: '%s' of type '%s'", name, typespec_to_str(type));
}
List get_foreign_function_list(void) { return foreign_function_list; }

void add_symbol(const char* name, Typespec* type)
{
    assert(name);
    assert(type);
    if (map_set(&symbol_map, name, type) == MAP_EXISTS) {
        warning("symbol redecl: '%s'", name);
    }
    info("added symbol: '%s' of type '%s'", name, typespec_to_str(type));
}

Typespec* get_symbol(const char* name)
{
    assert(name);
    Typespec* type = (Typespec*)map_get(&symbol_map, name);
    if (!type) {
        error("no symbol with name '%s'", name);
    }
    return type;
}

void add_macro_def(const char* name, Expr* expr)
{
    assert(name);
    assert(expr);
    if (map_set(&macro_map, name, expr) == MAP_EXISTS) {
        warning("macro redecl: '%s'", name);
    }
    info("added macro: '%s' with expr '%s'", name, expr_to_str(expr));
}

Expr* get_macro_def(const char* name)
{
    assert(name);
    Expr* expr = (Expr*)map_get(&macro_map, name);
    // if (!expr) {
    // error("no macro with name '%s'", name);
    // }
    return expr;
}

List get_timers(void) { return timers; }

void push_timer(const char* desc)
{
    Timer* tm = xmalloc(sizeof(Timer));
    tm->ms = get_time();
    tm->desc = desc;
    stack_push(&timer_stack, tm);
}

void pop_timer(void)
{
    Timer* tm = (Timer*)stack_pop(&timer_stack);
    tm->ms = get_time() - tm->ms;
    list_append(&timers, tm);
}
