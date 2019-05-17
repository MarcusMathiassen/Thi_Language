#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H
#include "ast.h" // AST
#include "map.h"  // Map
void type_checker(Map* symbol_map, AST* ast);
#endif
