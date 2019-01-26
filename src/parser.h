#ifndef PARSER_H
#define PARSER_H
#include "list.h" // List
#include "lexer.h"  // Token_Array
List* generate_ast_from_tokens(Token_Array tokens);
#endif
