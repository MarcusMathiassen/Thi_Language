#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"  // Token_Array
#include "list.h"   // List
#include "map.h"    // Map
#include "type.h"   // Type_Ref_List

typedef struct {
    List*         ast;
    Type_Ref_List unresolved_types;
    List*         variables_in_need_of_type_inference;
    List*         extern_list;
    List*         load_list;
    List*         link_list;
    Map*          symbol_map;
} Parsed_File;

Parsed_File generate_ast_from_tokens(Token_Array tokens);

#endif
