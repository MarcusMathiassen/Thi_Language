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
    Token* tokens;

    char* file;
    char* dir;

    List* loads;

    Token curr_tok;
    Token prev_tok;

    AST* llast_if_statement; // used for dangling else
    AST* olast_if_statement; // used for dangling else

} Parser_Context;

Parser_Context make_parser_context(void);
Type*          get_type(Parser_Context* ctx);
s64            get_integer(Parser_Context* ctx);
f64            get_float(Parser_Context* ctx);
int            get_tok_precedence(Parser_Context* ctx);
Token          next_tok(Parser_Context* ctx);
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
