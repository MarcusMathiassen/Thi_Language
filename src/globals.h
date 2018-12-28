#ifndef GLOBALS_H
#define GLOBALS_H

#include "ast.h"  // expr
#include "list.h" // List
#include "string.h" // string
#include "typedefs.h"
#include "typespec.h" // Typespec
extern bool detailed_print;
extern string* current_output;

#define DEFAULT_INTEGER_BYTE_SIZE 4
#define DEFAULT_INTEGER_BIT_SIZE DEFAULT_INTEGER_BYTE_SIZE * 8

void set_source_file(char* file_name);
char* get_source_file(void);

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
List get_foreign_function_list(void);

void add_constant_string(char* name);
List get_constant_string_list(void);

void push_timer(char* desc);
void pop_timer(void);
struct List get_timers(void);

#endif
