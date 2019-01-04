#ifndef GLOBALS_H
#define GLOBALS_H

#include "ast.h"    // expr
#include "list.h"   // List
#include "stack.h"  // Stack
#include "string.h" // string
#include "typedefs.h"
#include "typespec.h" // Typespec
extern bool detailed_print;
extern List file_list;
extern string* current_output;

#define DEFAULT_INTEGER_BYTE_SIZE 4
#define DEFAULT_INTEGER_BIT_SIZE DEFAULT_INTEGER_BYTE_SIZE * 8

void set_source_file(char* file_name);
char* get_previous_source_file(void);
char* get_source_file(void);

List* get_file_list(void);

void set_current_dir(char* dir_name);
char* get_current_dir();

void initilize_globals(void);
bool is_builtin_type(char* name);
void add_builtin_type(char* name, Typespec* type);
Typespec* get_builtin_type(char* name);

void add_symbol(char* name, Typespec* type);
Typespec* get_symbol(char* name);

void add_macro_def(char* name, Expr* expr);
Expr* get_macro_def(char* name);

void print_symbol_map(void);

void add_foreign_function(char* name, Typespec* type);
List* get_foreign_function_list(void);

void add_constant_string(char* name);
List* get_constant_string_list(void);

typedef struct
{
    f64 ms;
    char* desc;
} Timer;

void push_timer(char* desc);
void pop_timer(void);
Timer* peek_timer(void);
void set_current_timers_time(f64 new_time);
List* get_timers(void);

#endif
