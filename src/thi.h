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

#ifndef THI_H
#define THI_H

#include "ast.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "string.h"
#include "type.h"
#include "typedefs.h"

typedef struct
{
    f64 ms;
    char* desc;
} Timer;

typedef enum {
    PASS_SAFE,
    PASS_UNSAFE
} PassKind;

typedef struct {
    char* description;
    AST_Kind kind;
    PassKind passKind;
    void (*visitor_func)(void*, AST*);
    void* visitor_arg;
} PassDescriptor;

typedef struct
{
    bool detailed_print;
    bool debug_mode;
    bool enable_constant_folding;
    bool optimize;

    s64 lines;
    s64 comments;

    Map* all_passes_for_all_kinds;

    Type_Ref_List unresolved_types;
    AST_Ref_List calls;
    AST_Ref_List variables_in_need_of_type_inference;
    AST_Ref_List constants;
    AST_Ref_List identifiers;
    AST_Ref_List structs;
    AST_Ref_List enums;
    AST_Ref_List field_access;
    AST_Ref_List externs;
    AST_Ref_List subscripts;
    List* loads;
    List* links;

    AST* ast;
    Map* symbol_map;
    Map* macro_map;
    Stack* timer_stack;
    List* timer_list;

    string* output_name;
    string* source_file;
    char* input_file;
    char* previous_file;
    string* current_directory;
} Thi;

Thi make_thi(void);
void thi_remove_all_passes(Thi* thi);
List* thi_get_visitors_for_kind(Thi* thi, AST_Kind kind);
void thi_install_pass(Thi* thi, PassDescriptor passDesc);
void add_load(Thi* thi, char* loaded_file);
void add_link(Thi* thi, char* library_name);
Type* add_symbol(Thi* thi, char* name, Type* type);
Type* get_symbol(Thi* thi, char* name);
void set_current_directory(Thi* thi, char* dir_name);
char* get_current_directory(Thi* thi);
List* get_load_list(Thi* thi);
List* get_link_list(Thi* thi);
char* get_output_name(Thi* thi);
void set_output_name(Thi* thi, char* name);
void set_source_file(Thi* thi, char* file_name);
char* get_source_file(Thi* thi);
char* get_previous_source_file(Thi* thi);
void print_symbol_map(Thi* thi);
void push_timer(Thi* thi, char* desc);
void pop_timer(Thi* thi);
List* get_timers(Thi* thi);
#endif
