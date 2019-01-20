#ifndef GLOBALS_H
#define GLOBALS_H

#include "ast.h"    // expr
#include "list.h"   // List
#include "stack.h"  // Stack
#include "string.h" // string
#include "typedefs.h"
#include "typespec.h" // Typespec

#define RESET "\033[0m"
#define GRAY "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RGB_GRAY "\033[38;2;110;110;110;m"
#define RGB_WHITE "\033[38;2;255;255;255;m"

extern bool debug_mode;
extern bool optimize;
extern bool enable_constant_folding;

extern bool detailed_print;
extern List* file_list;

#define X64_ASM_OSX_STACK_PADDING 16

#define DEFAULT_FUNCTION_ENTRY_LABEL_NAME ".BEGIN"
#define DEFAULT_FUNCTION_END_LABEL_NAME ".END"

#define DEFAULT_INT_BYTE_SIZE 8
#define DEFAULT_FLOAT_BYTE_SIZE 4

#define INTERN_ARRAY_STARTING_ALLOC  1
#define TOKEN_ARRAY_STARTING_ALLOC   100

#define LINES_ABOVE_AND_BELOW_TO_SHOW_ON_ERROR 2

void set_source_file(char* file_name);
char* get_previous_source_file(void);
char* get_source_file(void);

char* get_output_name(void);
void set_output_name(char* name);

List* get_file_list(void);
List* get_link_list(void);
void add_link(char* library_name);

void set_current_dir(char* dir_name);
char* get_current_dir();

void initilize_globals(void);
bool is_builtin_type(char* name);
void add_builtin_type(char* name, Typespec* type);
Typespec* get_builtin_type(char* name);

Typespec* add_symbol(char* name, Typespec* type);
Typespec* set_symbol(char* name, Typespec* type);
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
