#ifndef THI_H
#define THI_H

#include "ast.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "string.h"
#include "type.h"
#include "typedefs.h"

typedef struct {
    f64   ms;
    char* desc;
} Timer;

typedef struct {
    bool detailed_print;
    bool debug_mode;
    bool enable_constant_folding;
    bool optimize;

    s64 lines;
    s64 comments;

    List*         load_list;
    List*         extern_list;
    List*         link_list;
    Map*          symbol_map;
    Map*          macro_map;
    Stack*        timer_stack;
    List*         timer_list;
    Type_Ref_List unresolved_types;
    List*         variables_in_need_of_type_inference;

    string output_name;
    string source_file;
    char*  previous_file;
    string current_directory;
} Thi;

Thi            make_thi();
void           add_load(Thi* thi, char* loaded_file);
void           add_link(Thi* thi, char* library_name);
Type*          add_symbol(Thi* thi, char* name, Type* type);
Type*          get_symbol(Thi* thi, char* name);
void           set_current_directory(Thi* thi, char* dir_name);
char*          get_current_directory(Thi* thi);
List*          get_load_list(Thi* thi);
List*          get_link_list(Thi* thi);
char*          get_output_name(Thi* thi);
void           set_output_name(Thi* thi, char* name);
void           set_source_file(Thi* thi, char* file_name);
char*          get_source_file(Thi* thi);
char*          get_previous_source_file(Thi* thi);
void           print_symbol_map(Thi* thi);
void           push_timer(Thi* thi, char* desc);
void           pop_timer(Thi* thi);
List*          get_timers(Thi* thi);
#endif
