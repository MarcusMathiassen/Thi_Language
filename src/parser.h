#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "list.h" // List
void parse(List* ast, char* source_file);
void parser_tests(void);
#endif
