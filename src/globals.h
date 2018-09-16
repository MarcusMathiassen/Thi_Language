#ifndef GLOBALS_H
#define GLOBALS_H

#include "map.h"        // Type_Map
#include "types.h"      // Type
#include "typedefs.h"

#define DEFAULT_INTEGER_BYTE_SIZE 4
#define DEFAULT_INTEGER_BIT_SIZE DEFAULT_INTEGER_BYTE_SIZE * 8

void init_maps();
bool is_builtin_type(const char* name);
void add_builtin_type(const char* name, Type* type);
Type* get_builtin_type(const char* name);
void add_symbol(const char* name, Type* type);
Type* get_symbol(const char* name);
void print_symbol_map();
void init_symbol_map();

#endif
