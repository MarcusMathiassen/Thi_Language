#ifndef PARSER_UTILITY_H
#define PARSER_UTILITY_H

#include "ast.h"
#include "lexer.h"
#include "list.h"
#include "map.h"
#include "type.h"
#include "typedefs.h"

typedef struct
{
    Token_Array tokens;
    s64         token_index;

    Type_Ref_List unresolved_types;
    AST_Ref_List  calls;
    AST_Ref_List  variables_in_need_of_type_inference;
    AST_Ref_List  constants;
    AST_Ref_List  identifiers;
    AST_Ref_List  structs;
    AST_Ref_List  enums;
    AST_Ref_List  externs;
    AST_Ref_List  field_access;
    AST_Ref_List  subscripts;
    List*         loads;
    List*         links;

    Map* symbols;

    Token curr_tok;
    Token prev_tok;

    AST* llast_if_statement; // used for dangling else
    AST* olast_if_statement; // used for dangling else

} Parser_Context;

Parser_Context
make_parser_context();
Type* get_type(Parser_Context* ctx);
s64   get_integer(Parser_Context* ctx);
f64   get_float(Parser_Context* ctx);
int   get_tok_precedence(Parser_Context* ctx);
Token next_tok(Parser_Context* ctx);
Token_Kind
next_tok_kind(Parser_Context* ctx);
bool tok_is_on_same_line(Parser_Context* ctx);
bool next_tok_is_on_same_line(Parser_Context* ctx);
bool tok_is(Parser_Context* ctx, Token_Kind kind);
void eat(Parser_Context* ctx);
void eat_kind(Parser_Context* ctx, Token_Kind kind);
void set_if_statement(Parser_Context* ctx, AST* if_statement);
void restore_if_statement(Parser_Context* ctx);
void set_dangling_else(Parser_Context* ctx, AST* else_block);

#endif
