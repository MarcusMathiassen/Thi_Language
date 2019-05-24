// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "thi.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert
#include <string.h> // strcmp

Thi make_thi() {
    Thi thi;
    thi.lines                   = 0;
    thi.comments                = 0;
    thi.detailed_print          = false;
    thi.debug_mode              = false;
    thi.enable_constant_folding = true;
    thi.optimize                = true;
    thi.timer_list              = make_list();
    thi.symbol_map              = make_map();
    thi.macro_map               = make_map();
    thi.timer_stack             = make_stack();
    thi.output_name             = string_create("");
    thi.previous_file           = NULL;
    thi.input_file              = NULL;
    thi.source_file             = string_create("");
    thi.current_directory       = string_create("");

    thi.all_passes_for_all_kinds = make_map();

    thi.ast                                 = NULL;
    thi.links                               = make_list();
    thi.loads                               = make_list();
    thi.unresolved_types                    = make_type_ref_list();
    thi.externs                             = make_ast_ref_list();
    thi.calls                               = make_ast_ref_list();
    thi.subscripts                          = make_ast_ref_list();
    thi.variables_in_need_of_type_inference = make_ast_ref_list();
    thi.constants                           = make_ast_ref_list();
    thi.identifiers                         = make_ast_ref_list();
    thi.structs                             = make_ast_ref_list();
    thi.enums                               = make_ast_ref_list();
    thi.field_access                        = make_ast_ref_list();

    // AST_Kind
    map_set(thi.all_passes_for_all_kinds, "AST_COMMENT", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_NOP", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_SPACE_SEPARATED_IDENTIFIER_LIST", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_COMMA_SEPARATED_LIST", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_MODULE", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_IS", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_FALLTHROUGH", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_VAR_ARGS", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_EXTERN", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_LOAD", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_LINK", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_NOTE", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_INT", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_FLOAT", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_STRING", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_IDENT", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_CALL", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_UNARY", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_BINARY", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_GROUPING", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_SUBSCRIPT", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_FIELD_ACCESS", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_AS", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_BLOCK", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_STRUCT", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_ENUM", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_FUNCTION", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_VARIABLE_DECL", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_CONSTANT_DECL", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_IF", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_FOR", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_WHILE", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_RETURN", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_DEFER", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_BREAK", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_CONTINUE", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_TYPEOF", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_SIZEOF", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_SWITCH", make_list());
    map_set(thi.all_passes_for_all_kinds, "AST_COUNT", make_list());

    return thi;
}

List* thi_get_visitors_for_kind(Thi* thi, AST_Kind kind) {
    List* res = map_get(thi->all_passes_for_all_kinds, ast_kind_to_str(kind));
    if (!res) ERROR_UNHANDLED_KIND(ast_kind_to_str(kind));
    return res;
}

void thi_remove_all_passes(Thi* thi) {
    // AST_Kind
    map_set(thi->all_passes_for_all_kinds, "AST_COMMENT", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_NOP", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_SPACE_SEPARATED_IDENTIFIER_LIST", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_COMMA_SEPARATED_LIST", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_MODULE", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_IS", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_FALLTHROUGH", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_VAR_ARGS", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_EXTERN", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_LOAD", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_LINK", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_NOTE", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_INT", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_FLOAT", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_STRING", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_IDENT", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_CALL", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_UNARY", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_BINARY", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_GROUPING", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_SUBSCRIPT", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_FIELD_ACCESS", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_AS", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_BLOCK", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_STRUCT", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_ENUM", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_FUNCTION", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_VARIABLE_DECL", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_CONSTANT_DECL", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_IF", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_FOR", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_WHILE", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_RETURN", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_DEFER", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_BREAK", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_CONTINUE", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_TYPEOF", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_SIZEOF", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_SWITCH", make_list());
    map_set(thi->all_passes_for_all_kinds, "AST_COUNT", make_list());
}

void thi_install_pass(Thi* thi, PassDescriptor passDesc) {
    PassDescriptor* p = xmalloc(sizeof(PassDescriptor));
    *p                = passDesc;
    List* l           = map_get(thi->all_passes_for_all_kinds, ast_kind_to_str(passDesc.kind));
    list_append(l, p);
}

char* get_output_name(Thi* thi) {
    return string_data(thi->output_name);
}
void set_output_name(Thi* thi, char* name) {
    thi->output_name = string_create(name);
}
void set_source_file(Thi* thi, char* file_name) {
    thi->previous_file = string_data(thi->source_file);
    thi->source_file   = string_create(file_name);
}
char* get_source_file(Thi* thi) {
    return string_data(thi->source_file);
}
char* get_previous_source_file(Thi* thi) {
    return thi->previous_file;
}
void set_current_directory(Thi* thi, char* dir_name) {
    thi->current_directory = string_create(dir_name);
}
char* get_current_directory(Thi* thi) {
    return string_data(thi->current_directory);
}

List* get_load_list(Thi* thi) {
    return thi->loads;
}

void add_load(Thi* thi, char* loaded_file) {
    assert(loaded_file);
    LIST_FOREACH(thi->loads) {
        char* l = (char*)it->data;
        if (strcmp(l, loaded_file) == 0) {
            return;
        }
    }
    list_append(thi->loads, loaded_file);
    info("added load: %s", give_unique_color(loaded_file));
}

void add_link(Thi* thi, char* library_name) {
    assert(library_name);
    LIST_FOREACH(thi->links) {
        char* l = (char*)it->data;
        if (strcmp(l, library_name) == 0) {
            return;
        }
    }
    list_append(thi->links, library_name);
    info("added link: %s", give_unique_color(library_name));
}

List* get_link_list(Thi* thi) {
    return thi->links;
}

void print_symbol_map(Thi* thi) {
    s64 count = thi->symbol_map->size;
    info("symbol_map count: %d", count);
    for (s64 i = 0; i < count; ++i) {
        info("key %lld type %s", thi->symbol_map->data[i].key, type_to_str(NULL, thi->symbol_map->data[i].data));
    }
}

Type* add_symbol(Thi* thi, char* name, Type* type) {
    assert(name);
    assert(type);
    Type* t = map_set(thi->symbol_map, name, type);
    if (!t) {
        error("symbol redecl: '%s'", name);
    }
    info("added symbol: %s of type %s", give_unique_color(name), give_unique_color(type_to_str(NULL, type)));
    return t;
}

Type* get_symbol(Thi* thi, char* name) {
    assert(name);
    Type* t = (Type*)map_get(thi->symbol_map, name);
    if (!t) {
        warning("no symbol with name %s", give_unique_color(name));
    }
    return t;
}

List* get_timers(Thi* thi) {
    return thi->timer_list;
}

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
