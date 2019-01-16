#include "globals.h"

#include "list.h"    // List
#include "map.h"     // Map
#include "stack.h"   // Stack
#include "string.h"  // string
#include "utility.h" // warning, error, xmalloc
#include <assert.h>  // assert

bool detailed_print = false;
bool debug_mode = false;
bool enable_constant_folding = true;
bool optimize = true;

string* current_output;

List* foreign_function_list;
List* constant_string_list;
Map* symbol_map;
Map* macro_map;
Map* builtin_type_map;
Stack* timer_stack;
List* timers;

char* debug_success_color = GREEN;
char* debug_info_color = RGB_GRAY;
char* debug_warning_color = YELLOW;
char* debug_error_color = RED;

string output_name;
string source_file;
char* previous_file;
string current_directory;
List* file_list;

char* get_output_name(void)
{
    return output_name.c_str;
}
void set_output_name(char* name)
{
    output_name = make_string(name);
}

void set_source_file(char* file_name)
{
    previous_file = source_file.c_str;
    source_file = make_string(file_name);
}
char* get_source_file(void) { return source_file.c_str; }
char* get_previous_source_file(void) { return previous_file; }
void set_current_dir(char* dir_name) { current_directory = make_string(dir_name); }
char* get_current_dir() { return current_directory.c_str; }

List* get_file_list() { return file_list; }

void initilize_globals(void)
{
    timer_stack = make_stack();
    foreign_function_list = make_list();
    constant_string_list = make_list();
    timers = make_list();
    file_list = make_list();
    symbol_map = make_map();
    macro_map = make_map();
    builtin_type_map = make_map();
}

void print_symbol_map(void)
{
    s64 count = symbol_map->size;
    success("symbol_map count: %d", count);
    for (s64 i = 0; i < count; ++i) {
        success("%s", typespec_to_str(symbol_map->data[i].data));
    }
}

bool is_builtin_type(char* name)
{
    assert(name);
    if (map_get(builtin_type_map, name)) return true;
    return false;
}

void add_builtin_type(char* name, Typespec* type)
{
    assert(name);
    assert(type);
    if (!map_set(builtin_type_map, name, type)) {
        warning("type redecl: '%s'", name);
    }
    success("added builtin type: %s of type '%s'", name, typespec_to_str(type));
}

Typespec* get_builtin_type(char* name)
{
    assert(name);
    Typespec* type = (Typespec*)map_get(builtin_type_map, name);
    if (!type) {
        error("no type with name '%s'", name);
    }
    return type;
}

void add_foreign_function(char* name, Typespec* type)
{
    assert(type);
    list_append(foreign_function_list, type);
    success("added extern function: '%s' of type '%s'", name, typespec_to_str(type));
}
List* get_foreign_function_list(void) { return foreign_function_list; }

void add_constant_string(char* name)
{
    assert(name);
    list_append(constant_string_list, name);
}
List* get_constant_string_list(void) { return constant_string_list; }

Typespec* add_symbol(char* name, Typespec* type)
{
    assert(name);
    assert(type);
    Typespec* t = map_set(symbol_map, name, type);
    if (!t) {
        error("symbol redecl: '%s'", name);
    }
    success("added symbol: '%s' of type '%s'", name, typespec_to_str(type));
    return t;
}

Typespec* set_symbol(char* name, Typespec* type)
{
    assert(name);
    assert(type);
    Typespec* t = map_set_overwrite(symbol_map, name, type);
    success("set symbol: '%s' to '%s'", name, typespec_to_str(type));
    return t;
}

Typespec* get_symbol(char* name)
{
    assert(name);
    Typespec* t = (Typespec*)map_get(symbol_map, name);
    if (!t) {
        warning("no symbol with name '%s'", name);
    }
    return t;
}

void add_macro_def(char* name, Expr* expr)
{
    assert(name);
    assert(expr);
    if (!map_set(macro_map, name, expr)) {
        warning("macro redecl: '%s'", name);
    }
    success("added macro: '%s' with expr '%s'", name, expr_to_str(expr));
}

Expr* get_macro_def(char* name)
{
    assert(name);
    Expr* expr = (Expr*)map_get(macro_map, name);
    // if (!expr) {
    //     error("no macro with name '%s'", name);
    // }
    return expr;
}

List* get_timers(void) { return timers; }
Timer* peek_timer(void) { return (Timer*)stack_peek(timer_stack); }
void set_current_timers_time(f64 new_time)
{
    Timer* tm = (Timer*)stack_peek(timer_stack);
    tm->ms = new_time;
}

void push_timer(char* desc)
{
    assert(desc);
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
