#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "list.h" // List
void parse(List* ast, char* source_file);
void give_type_to_all_nodes(Expr* expr);
#endif
