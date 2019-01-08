#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "list.h" // List
void generate_ast_from_tokens(List* ast, List* tokens);
void generate_symbol_table_from_tokens(List* ast, List* tokens);
void parse(List* ast, char* source_file);
#endif
