#ifndef PARSER_H
#define PARSER_H
#include "ast.h"  // AST
#include "parser_utility.h"  // Parser_Context
AST* parse(Parser_Context* ctx, char* file);
#endif
