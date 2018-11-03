#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "list.h" // List
List generate_ast_from_tokens(Token* tokens);
void generate_symbol_table_from_tokens(Token* tokens);

#endif
