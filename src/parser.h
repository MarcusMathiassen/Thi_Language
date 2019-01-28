#ifndef PARSER_H
#define PARSER_H
#include "lexer.h" // Token_Array
#include "list.h" // List
#include "type.h" // Type_Ref_List
typedef struct {
    List*         ast;
    Type_Ref_List unresolved_types;
} Parsed_File;

Parsed_File generate_ast_from_tokens(Token_Array tokens);

#endif
