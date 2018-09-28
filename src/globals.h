#ifndef GLOBALS_H
#define GLOBALS_H

#include "typedefs.h"
#include "typespec.h" // Typespec

#define DEFAULT_INTEGER_BYTE_SIZE 4
#define DEFAULT_INTEGER_BIT_SIZE DEFAULT_INTEGER_BYTE_SIZE * 8

void init_maps(void);
bool is_builtin_type(const char* name);
void add_builtin_type(const char* name, Typespec* type);
Typespec* get_builtin_type(const char* name);
void add_symbol(const char* name, Typespec* type);
Typespec* get_symbol(const char* name);
void print_symbol_map(void);
void init_symbol_map(void);

#endif
