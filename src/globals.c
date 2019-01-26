#include "globals.h"

#include <assert.h>   // assert
#include <string.h>   // strcmp
#include "list.h"     // List
#include "map.h"      // Map
#include "stack.h"    // Stack
#include "string.h"   // string
#include "utility.h"  // warning, error, xmalloc

bool detailed_print          = false;
bool debug_mode              = false;
bool enable_constant_folding = true;
bool optimize                = true;

List*  foreign_function_list;
List*  link_list;
Map*   symbol_map;
Map*   macro_map;
Stack* timer_stack;
List*  timers;

string output_name;
string source_file;
char*  previous_file;
string current_directory;
List*  file_list;
List*  type_list;

char* get_output_name(void) { return output_name.c_str; }
void  set_output_name(char* name) { output_name = make_string(name); }

void set_source_file(char* file_name) {
    previous_file = source_file.c_str;
    source_file   = make_string(file_name);
}
char* get_source_file(void) { return source_file.c_str; }
char* get_previous_source_file(void) { return previous_file; }
void  set_current_dir(char* dir_name) { current_directory = make_string(dir_name); }
char* get_current_dir() { return current_directory.c_str; }

List* get_file_list() { return file_list; }

void initilize_globals(void) {
    timer_stack           = make_stack();
    foreign_function_list = make_list();
    link_list             = make_list();
    timers                = make_list();
    file_list             = make_list();
    symbol_map            = make_map();
    macro_map             = make_map();
    type_list             = make_list();
}

void add_link(char* library_name) {
    assert(library_name);
    LIST_FOREACH(link_list) {
        char* l = (char*)it->data;
        if (strcmp(l, library_name) == 0) {
            return;
        }
    }
    list_append(link_list, library_name);
    info("added link: '%s'", library_name);
}

List* get_link_list(void) { return link_list; }

void print_symbol_map(void) {
    s64 count = symbol_map->size;
    info("symbol_map count: %d", count);
    for (s64 i = 0; i < count; ++i) {
        info("key %lld type %s", symbol_map->data[i].key, type_to_str(symbol_map->data[i].data));
    }
}

void add_foreign_function(char* name, Type* type) {
    assert(type);
    list_append(foreign_function_list, type);
    info("added extern function: '%s' of type '%s'", name, type_to_str(type));
}
List* get_foreign_function_list(void) { return foreign_function_list; }

Type* add_symbol(char* name, Type* type) {
    assert(name);
    assert(type);
    Type* t = map_set(symbol_map, name, type);
    if (!t) {
        error("symbol redecl: '%s'", name);
    }
    info("added symbol: '%s' of type '%s'", name, type_to_str(type));
    return t;
}

Type* set_symbol(char* name, Type* type) {
    assert(name);
    assert(type);
    Type* t = map_set_overwrite(symbol_map, name, type);
    info("set symbol: '%s' to '%s'", name, type_to_str(type));
    return t;
}

Type* get_symbol(char* name) {
    assert(name);
    Type* t = (Type*)map_get(symbol_map, name);
    if (!t) {
        warning("no symbol with name '%s'", name);
    }
    return t;
}

void add_macro_def(char* name, AST* expr) {
    assert(name);
    assert(expr);
    if (!map_set(macro_map, name, expr)) {
        warning("macro redecl: '%s'", name);
    }
    info("added macro: %s :: %s", name, ast_to_str(expr));
}

AST* get_macro_def(char* name) {
    assert(name);
    AST* expr = (AST*)map_get(macro_map, name);
    // if (!expr) {
    //     error("no macro with name '%s'", name);
    // }
    return expr;
}

List*  get_timers(void) { return timers; }
Timer* peek_timer(void) { return (Timer*)stack_peek(timer_stack); }
void   set_current_timers_time(f64 new_time) {
    Timer* tm = (Timer*)stack_peek(timer_stack);
    tm->ms    = new_time;
}

void push_timer(char* desc) {
    assert(desc);
    Timer* tm = xmalloc(sizeof(Timer));
    tm->ms    = get_time();
    tm->desc  = desc;
    stack_push(timer_stack, tm);
}

void pop_timer(void) {
    Timer* tm = (Timer*)stack_pop(timer_stack);
    tm->ms    = get_time() - tm->ms;
    list_append(timers, tm);
}
