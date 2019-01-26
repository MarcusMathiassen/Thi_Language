#include "thi.h"
#include <assert.h>  // assert
#include <string.h>  // strcmp
#include "type.h"
#include "utility.h"

Thi make_thi() {
    Thi thi;
    thi.lines                               = 0;
    thi.comments                            = 0;
    thi.detailed_print                      = false;
    thi.debug_mode                          = false;
    thi.enable_constant_folding             = true;
    thi.optimize                            = true;
    thi.extern_list                         = make_list();
    thi.link_list                           = make_list();
    thi.load_list                           = make_list();
    thi.timer_list                          = make_list();
    thi.symbol_map                          = make_map();
    thi.macro_map                           = make_map();
    thi.timer_stack                         = make_stack();
    thi.output_name                         = make_string("");
    thi.previous_file                       = NULL;
    thi.source_file                         = make_string("");
    thi.current_directory                   = make_string("");
    thi.unresolved_types                    = make_type_ref_list();
    thi.variables_in_need_of_type_inference = make_list();
    return thi;
}

char* get_output_name(Thi* thi) { return thi->output_name.c_str; }
void  set_output_name(Thi* thi, char* name) { thi->output_name = make_string(name); }

void set_source_file(Thi* thi, char* file_name) {
    thi->previous_file = thi->source_file.c_str;
    thi->source_file   = make_string(file_name);
}
char* get_source_file(Thi* thi) { return thi->source_file.c_str; }
char* get_previous_source_file(Thi* thi) { return thi->previous_file; }
void  set_current_directory(Thi* thi, char* dir_name) { thi->current_directory = make_string(dir_name); }
char* get_current_directory(Thi* thi) { return thi->current_directory.c_str; }

List* get_load_list(Thi* thi) { return thi->load_list; }

void add_load(Thi* thi, char* loaded_file) {
    assert(loaded_file);
    LIST_FOREACH(thi->load_list) {
        char* l = (char*)it->data;
        if (strcmp(l, loaded_file) == 0) {
            return;
        }
    }
    list_append(thi->load_list, loaded_file);
    info("added load: '%s'", loaded_file);
}

void add_link(Thi* thi, char* library_name) {
    assert(library_name);
    LIST_FOREACH(thi->link_list) {
        char* l = (char*)it->data;
        if (strcmp(l, library_name) == 0) {
            return;
        }
    }
    list_append(thi->link_list, library_name);
    info("added link: '%s'", library_name);
}

List* get_link_list(Thi* thi) { return thi->link_list; }

void print_symbol_map(Thi* thi) {
    s64 count = thi->symbol_map->size;
    info("symbol_map count: %d", count);
    for (s64 i = 0; i < count; ++i) {
        info("key %lld type %s", thi->symbol_map->data[i].key, type_to_str(thi->symbol_map->data[i].data));
    }
}

Type* add_symbol(Thi* thi, char* name, Type* type) {
    assert(name);
    assert(type);
    Type* t = map_set(thi->symbol_map, name, type);
    if (!t) {
        error("symbol redecl: '%s'", name);
    }
    info("added symbol: '%s' of type '%s'", name, type_to_str(type));
    return t;
}

Type* get_symbol(Thi* thi, char* name) {
    assert(name);
    Type* t = (Type*)map_get(thi->symbol_map, name);
    if (!t) {
        warning("no symbol with name '%s'", name);
    }
    return t;
}

List* get_timers(Thi* thi) { return thi->timer_list; }

void push_timer(Thi* thi, char* desc) {
    assert(desc);
    Timer* tm = xmalloc(sizeof(Timer));
    tm->ms    = get_time();
    tm->desc  = desc;
    stack_push(thi->timer_stack, tm);
}

void pop_timer(Thi* thi) {
    Timer* tm = (Timer*)stack_pop(thi->timer_stack);
    tm->ms    = get_time() - tm->ms;
    list_append(thi->timer_list, tm);
}
