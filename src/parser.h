#ifndef PARSER_H
#define PARSER_H
#include "ast.h" // AST_Ref_List
#include "lexer.h" // Token_Array
#include "list.h" // List
#include "type.h" // Type_Ref_List
typedef struct {
    List*         ast;
    Type_Ref_List unresolved_types;
    AST_Ref_List  calls;
    AST_Ref_List  variables_in_need_of_type_inference;
    AST_Ref_List  constants;
    AST_Ref_List  identifiers;
    AST_Ref_List  structs;
    AST_Ref_List  enums;
    AST_Ref_List  externs;
    List*         loads;
    List*         links;
} Parsed_File;

Parsed_File generate_ast_from_tokens(Token_Array tokens);

#endif
