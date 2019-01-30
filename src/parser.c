#include "parser.h"
#include "ast.h" // AST, ast_make_*
#include "constants.h"
#include "lexer.h" // Token, Token_Kind, generate_tokens_from_source, token_array_get_info_of
#include "list.h" // List
#include "map.h" // Map
#include "type.h" // Type, make_typspec_*,
#include "typedefs.h" // s32 , s64, etc.
#include "utility.h" // info, error, warning, success, strf, get_file_content
#include <assert.h> // assert
#include <ctype.h> // atoll
#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h> // printf, vprintf
#include <stdlib.h> // xmalloc
#include <string.h> // strcmp
//------------------------------------------------------------------------------
//                              parser.c
//------------------------------------------------------------------------------

#define DEBUG_START info("%s: %s", __func__, token_to_str(pctx->curr_tok));

#define BIN_OP_COUNT 35
struct {
    Token_Kind kind;
    s32        p;
} binop_precedence[BIN_OP_COUNT] = {
    { TOKEN_OPEN_PAREN, 15 }, // ()
    { TOKEN_OPEN_BRACKET, 15 }, // []
    { TOKEN_DOT, 15 }, // .
    { TOKEN_ASTERISK, 13 }, // *
    { TOKEN_FWSLASH, 13 }, // /
    { TOKEN_PERCENT, 13 }, // %
    { TOKEN_PLUS, 12 }, // +
    { TOKEN_MINUS, 12 }, // -
    { TOKEN_LT_LT, 11 }, // <<
    { TOKEN_GT_GT, 11 }, // >>
    { TOKEN_LT, 10 }, // <
    { TOKEN_LT_EQ, 10 }, // <=
    { TOKEN_GT, 10 }, // >
    { TOKEN_GT_EQ, 10 }, // >=
    { TOKEN_EQ_EQ, 9 }, // ==
    { TOKEN_BANG_EQ, 9 }, // !=
    { TOKEN_AND, 8 }, // &
    { TOKEN_HAT, 7 }, // ^
    { TOKEN_PIPE, 6 }, // |
    { TOKEN_AND_AND, 5 }, // &&
    { TOKEN_PIPE_PIPE, 4 }, // ||
    { TOKEN_QUESTION_MARK, 3 }, // ?
    { TOKEN_COLON, 3 }, // :
    { TOKEN_EQ, 2 }, // =
    { TOKEN_PLUS_EQ, 2 }, // +=
    { TOKEN_MINUS_EQ, 2 }, // -=
    { TOKEN_ASTERISK_EQ, 2 }, // *=
    { TOKEN_FWSLASH_EQ, 2 }, // /=
    { TOKEN_PERCENT_EQ, 2 }, // %=
    { TOKEN_AND_EQ, 2 }, // &=
    { TOKEN_HAT_EQ, 2 }, // ^=
    { TOKEN_PIPE_EQ, 2 }, // |=
    { TOKEN_LT_LT_EQ, 2 }, // <<=
    { TOKEN_GT_GT_EQ, 2 }, // >>=
    // {TOKEN_COMMA, 1},          // ,
};

#define UNARY_OP_COUNT 7
Token_Kind unary_ops[UNARY_OP_COUNT] = {
    TOKEN_BANG,
    THI_SYNTAX_POINTER,
    TOKEN_PLUS,
    THI_SYNTAX_ADDRESS,
    TOKEN_MINUS,
    TOKEN_TILDE,
    TOKEN_DOT,
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

typedef struct {
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

Parser_Context make_parser_context()
{
    Parser_Context pctx;
    pctx.token_index        = 0;
    pctx.curr_tok.kind      = TOKEN_UNKNOWN;
    pctx.llast_if_statement = NULL;
    pctx.olast_if_statement = NULL;

    pctx.unresolved_types                    = make_type_ref_list();
    pctx.variables_in_need_of_type_inference = make_ast_ref_list();
    pctx.calls                               = make_ast_ref_list();
    pctx.constants                           = make_ast_ref_list();
    pctx.identifiers                         = make_ast_ref_list();
    pctx.structs                             = make_ast_ref_list();
    pctx.enums                               = make_ast_ref_list();
    pctx.externs                             = make_ast_ref_list();
    pctx.field_access                        = make_ast_ref_list();
    pctx.subscripts                        = make_ast_ref_list();
    pctx.loads                               = make_list();
    pctx.links                               = make_list();
    pctx.symbols                             = make_map();

    return pctx;
}

//------------------------------------------------------------------------------
//              Each construct of the language gets its own function
//------------------------------------------------------------------------------

AST* parse_statement(Parser_Context* pctx);
AST* parse_primary(Parser_Context* pctx);
AST* parse_identifier(Parser_Context* pctx);
AST* parse_expression_identifier(Parser_Context* pctx);
AST* parse_variable_decl(Parser_Context* pctx, char* ident);
AST* parse_constant_decl(Parser_Context* pctx, char* ident);
AST* parse_function_call(Parser_Context* pctx, char* ident);
AST* parse_block(Parser_Context* pctx);
AST* parse_return(Parser_Context* pctx);
AST* parse_note(Parser_Context* pctx);
AST* parse_expression(Parser_Context* pctx);
AST* parse_unary(Parser_Context* pctx);
AST* parse_binary(Parser_Context* pctx, s8 min_prec, AST* lhs);
AST* parse_integer(Parser_Context* pctx);
AST* parse_float(Parser_Context* pctx);
AST* parse_parens(Parser_Context* pctx);
AST* parse_defer(Parser_Context* pctx);
AST* parse_load(Parser_Context* pctx);
AST* parse_extern(Parser_Context* pctx);
AST* parse_link(Parser_Context* pctx);
AST* parse_if(Parser_Context* pctx);
AST* parse_is(Parser_Context* pctx);
AST* parse_fallthrough(Parser_Context* pctx);
AST* parse_dangling_else(Parser_Context* pctx);
AST* parse_for(Parser_Context* pctx);
AST* parse_while(Parser_Context* pctx);
AST* parse_break(Parser_Context* pctx);
AST* parse_continue(Parser_Context* pctx);
AST* parse_string(Parser_Context* pctx);
AST* parse_sizeof(Parser_Context* pctx);
AST* parse_cast(Parser_Context* pctx);
AST* parse_enum(Parser_Context* pctx);
AST* parse_struct(Parser_Context* pctx);

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

Type* parse_enum_signature(Parser_Context* pctx, char* enum_name);
Type* parse_struct_signature(Parser_Context* pctx, char* struct_name);
Type* parse_function_signature(Parser_Context* pctx, char* func_name);
Type* parse_extern_function_signature(Parser_Context* pctx, char* func_name);

s64   get_integer(Parser_Context* pctx);
f64   get_float(Parser_Context* pctx);
Type* get_type(Parser_Context* pctx);

void set_if_statement(Parser_Context* pctx, AST* if_statement);
void restore_if_statement(Parser_Context* pctx);
void set_dangling_else(Parser_Context* pctx, AST* else_block);

Token      next_tok(Parser_Context* pctx);
bool       tok_is_on_same_line(Parser_Context* pctx);
bool       tok_is(Parser_Context* pctx, Token_Kind kind);
int        get_tok_precedence(Parser_Context* pctx);
void       eat(Parser_Context* pctx);
void       eat_kind(Parser_Context* pctx, Token_Kind kind);
Token_Kind next_tok_kind(Parser_Context* pctx);
bool       next_tok_is_on_same_line(Parser_Context* pctx);

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

Parsed_File generate_ast_from_tokens(Token_Array tokens)
{
    info("Generating ast from tokens..");

    List* ast = make_list();

    Parser_Context pctx = make_parser_context();
    pctx.tokens         = tokens;

    eat(&pctx);
    while (!tok_is(&pctx, TOKEN_EOF)) {
        AST* stmt = parse_statement(&pctx);
        if (stmt) {
            list_append(ast, stmt);
        }
    }

    Parsed_File pf;
    pf.ast                                 = ast;
    pf.unresolved_types                    = pctx.unresolved_types;
    pf.variables_in_need_of_type_inference = pctx.variables_in_need_of_type_inference;
    pf.externs                             = pctx.externs;
    pf.loads                               = pctx.loads;
    pf.links                               = pctx.links;
    pf.enums                               = pctx.enums;
    pf.structs                             = pctx.structs;
    pf.field_access                        = pctx.field_access;
    pf.subscripts                        = pctx.subscripts;
    pf.calls                               = pctx.calls;
    pf.constants                           = pctx.constants;
    pf.identifiers                         = pctx.identifiers;
    pf.symbols                             = pctx.symbols;
    return pf;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

AST* parse_statement(Parser_Context* pctx)
{
    DEBUG_START;
    switch (pctx->curr_tok.kind) {
    case TOKEN_BLOCK_START: return parse_block(pctx);
    case TOKEN_ENUM: return parse_enum(pctx);
    case TOKEN_STRUCT: return parse_struct(pctx);
    case TOKEN_IDENTIFIER: return parse_expression(pctx);
    case TOKEN_RETURN: return parse_return(pctx);
    case TOKEN_BREAK: return parse_break(pctx);
    case TOKEN_CONTINUE: return parse_continue(pctx);
    case TOKEN_IF: return parse_if(pctx);
    case TOKEN_IS: return parse_is(pctx);
    case TOKEN_FALLTHROUGH: return parse_fallthrough(pctx);
    case TOKEN_ELSE: return parse_dangling_else(pctx);
    case TOKEN_DEFER: return parse_defer(pctx);
    case TOKEN_FOR: return parse_for(pctx);
    case TOKEN_WHILE: return parse_while(pctx);
    case TOKEN_EXTERN: return parse_extern(pctx);
    case TOKEN_LOAD: return parse_load(pctx);
    case TOKEN_LINK: return parse_link(pctx);
    default: error("Invalid statement token '%s'", token_to_str(pctx->curr_tok));
    }
    return NULL;
}

AST* parse_primary(Parser_Context* pctx)
{
    DEBUG_START;
    switch (pctx->curr_tok.kind) {
    case TOKEN_TRUE: eat(pctx); return make_ast_int(pctx->curr_tok, 1);
    case TOKEN_FALSE: eat(pctx); return make_ast_int(pctx->curr_tok, 0);
    case TOKEN_SIZEOF: return parse_sizeof(pctx);
    case TOKEN_CAST: return parse_cast(pctx);
    case TOKEN_IDENTIFIER: return parse_identifier(pctx);
    case TOKEN_DOLLAR_SIGN: return parse_note(pctx);
    case TOKEN_FLOAT: return parse_float(pctx);
    case TOKEN_CHAR: // FALLTHROUGH
    case TOKEN_HEX: // FALLTHROUGH
    case TOKEN_INTEGER: return parse_integer(pctx);
    case TOKEN_STRING: return parse_string(pctx);
    case TOKEN_OPEN_PAREN: return parse_parens(pctx);
    default: error("Invalid primary token '%s'", token_to_str(pctx->curr_tok));
    }
    return NULL;
}

AST* parse_identifier(Parser_Context* pctx)
{
    DEBUG_START;

    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);

    switch (pctx->curr_tok.kind) {
    case TOKEN_COLON: // fallthrough
    case TOKEN_COLON_EQ: return parse_variable_decl(pctx, ident);
    case TOKEN_COLON_COLON: return parse_constant_decl(pctx, ident);
    case TOKEN_OPEN_PAREN: return parse_function_call(pctx, ident);
    }

    AST* i = make_ast_ident(pctx->curr_tok, ident);
    ast_ref_list_append(&pctx->identifiers, i);

    return i;
}

AST* parse_expression_identifier(Parser_Context* pctx)
{
    DEBUG_START;

    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);

    switch (pctx->curr_tok.kind) {
    case TOKEN_OPEN_PAREN: return parse_function_call(pctx, ident);
    }

    AST* i = make_ast_ident(pctx->curr_tok, ident);
    ast_ref_list_append(&pctx->identifiers, i);

    return i;
}

AST* parse_struct(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_STRUCT);
    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    Type* type = parse_struct_signature(pctx, ident);
    AST*  s    = make_ast_struct(pctx->curr_tok, type);
    map_set(pctx->symbols, ident, type);
    ast_ref_list_append(&pctx->structs, s);
    return s;
}

AST* parse_enum(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_ENUM);
    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    Type* type = parse_enum_signature(pctx, ident);
    AST*  e    = make_ast_enum(pctx->curr_tok, type);
    map_set(pctx->symbols, ident, type);
    ast_ref_list_append(&pctx->enums, e);
    return e;
}

AST* parse_load(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_LOAD);
    char* str = strf("%s.thi", pctx->curr_tok.value);
    eat_kind(pctx, TOKEN_STRING);
    list_append(pctx->loads, str);
    return make_ast_load(pctx->curr_tok, str);
}

AST* parse_link(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_LINK);
    char* str = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_STRING);
    list_append(pctx->links, str);
    return make_ast_link(pctx->curr_tok, str);
}

AST* parse_extern(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_EXTERN);
    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    Type* type = parse_extern_function_signature(pctx, ident);
    AST*  e    = make_ast_extern(pctx->curr_tok, type);
    map_set(pctx->symbols, ident, type);
    ast_ref_list_append(&pctx->externs, e);
    return e;
}

AST* parse_continue(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_CONTINUE);
    return make_ast_continue(pctx->curr_tok);
}

AST* parse_break(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_BREAK);
    return make_ast_break(pctx->curr_tok);
}

AST* parse_while(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_WHILE);
    AST* cond       = parse_expression(pctx);
    AST* then_block = parse_block(pctx);
    return make_ast_while(pctx->curr_tok, cond, then_block);
}

AST* parse_for(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_FOR);
    AST* init = parse_statement(pctx);
    eat_kind(pctx, TOKEN_COMMA);
    AST* cond = parse_expression(pctx);
    eat_kind(pctx, TOKEN_COMMA);
    AST* step       = parse_expression(pctx);
    AST* then_block = parse_block(pctx);
    return make_ast_for(pctx->curr_tok, init, cond, step, then_block);
}

AST* parse_defer(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_DEFER);
    AST* block = parse_block(pctx);
    return make_ast_defer(pctx->curr_tok, block);
}

AST* parse_fallthrough(Parser_Context* pctx)
{
    DEBUG_START;
    Token t = pctx->curr_tok;
    eat_kind(pctx, TOKEN_FALLTHROUGH);
    return make_ast_fallthrough(t);
}
AST* parse_is(Parser_Context* pctx)
{
    DEBUG_START;
    Token t = pctx->curr_tok;
    eat_kind(pctx, TOKEN_IS);
    AST* expr = parse_expression(pctx);
    AST* body = parse_block(pctx);

    bool has_fallthrough = false;
    LIST_FOREACH(body->Block.stmts)
    {
        AST* stmt = (AST*)it->data;
        if (stmt->kind == AST_FALLTHROUGH) {
            has_fallthrough = true;
            break;
        }
    }

    return make_ast_is(t, expr, body, has_fallthrough);
}

AST* parse_dangling_else(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_ELSE);
    AST* else_block = parse_block(pctx);
    set_dangling_else(pctx, else_block);
    return NULL;
}

AST* parse_if(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_IF);
    AST* cond       = parse_expression(pctx);
    AST* then_block = parse_block(pctx);
    AST* else_block = NULL;
    if (tok_is(pctx, TOKEN_ELSE)) {
        eat_kind(pctx, TOKEN_ELSE);
        else_block = parse_block(pctx);
    }
    AST* if_statement = make_ast_if(pctx->curr_tok, cond, then_block, else_block);
    set_if_statement(pctx, if_statement);
    return if_statement;
}

AST* parse_sizeof(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_SIZEOF);
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Type* type = get_type(pctx);
    eat_kind(pctx, TOKEN_CLOSE_PAREN);
    return make_ast_sizeof(pctx->curr_tok, type);
}

AST* parse_string(Parser_Context* pctx)
{
    DEBUG_START;
    char* value = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_STRING);
    return make_ast_string(pctx->curr_tok, value);
}

AST* parse_block(Parser_Context* pctx)
{
    DEBUG_START;

    AST* block = NULL;

    // Is it a single statement?
    if (tok_is_on_same_line(pctx)) {
        List* stmts = make_list();
        AST*  stmt  = parse_statement(pctx);
        if (stmt) list_append(stmts, stmt);
        block = make_ast_block(pctx->curr_tok, stmts);
    }

    if (!block) {
        List* stmts = make_list();
        eat_kind(pctx, TOKEN_BLOCK_START);
        while (!tok_is(pctx, TOKEN_BLOCK_END)) {
            AST* stmt = parse_statement(pctx);
            if (stmt) {
                list_append(stmts, stmt);
            }
        }
        eat_kind(pctx, TOKEN_BLOCK_END);
        block = make_ast_block(pctx->curr_tok, stmts);
    }

    assert(block);

    restore_if_statement(pctx);

    return block;
}

AST* parse_return(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_RETURN);
    AST* expr = NULL;
    if (tok_is_on_same_line(pctx)) {
        expr = parse_expression(pctx);
    }
    return make_ast_return(pctx->curr_tok, expr);
}

AST* parse_function_call(Parser_Context* pctx, char* ident)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    List* args                   = make_list();
    bool  has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);
        AST* arg = parse_expression(pctx);
        list_append(args, arg);
        has_multiple_arguments = true;
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);
    AST* call = make_ast_call(pctx->curr_tok, ident, args);
    ast_ref_list_append(&pctx->calls, call);
    return call;
}

AST* parse_constant_decl(Parser_Context* pctx, char* ident)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_COLON_COLON);
    AST* value         = parse_expression(pctx);
    AST* constant_decl = make_ast_constant_decl(pctx->curr_tok, ident, value);
    ast_ref_list_append(&pctx->constants, constant_decl);
    return constant_decl;
}

AST* parse_variable_decl(Parser_Context* pctx, char* ident)
{
    DEBUG_START;

    char* variable_name  = ident;
    Type* variable_type  = NULL;
    AST*  variable_value = NULL;

    bool variable_needs_type_inference = false;

    switch (pctx->curr_tok.kind) {
    case TOKEN_COLON: {
        eat_kind(pctx, TOKEN_COLON);
        variable_type = get_type(pctx);
        if (tok_is(pctx, TOKEN_EQ)) {
            eat_kind(pctx, TOKEN_EQ);
            variable_value = parse_expression(pctx);
        }
    } break;

    case TOKEN_COLON_EQ: {
        eat_kind(pctx, TOKEN_COLON_EQ);
        variable_value                = parse_expression(pctx);
        variable_needs_type_inference = true;
    } break;
    }

    AST* var_decl = make_ast_variable_decl(pctx->curr_tok, variable_name, variable_type, variable_value);
    if (variable_needs_type_inference) {
        ast_ref_list_append(&pctx->variables_in_need_of_type_inference, var_decl);
    }
    return var_decl;
}

AST* parse_binary(Parser_Context* pctx, s8 expr_prec, AST* lhs)
{
    DEBUG_START;

    AST* expr = NULL;

    // If this is a binop, find its precedence.
    for (;;) {
        s32 tok_prec = get_tok_precedence(pctx);

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (tok_prec < expr_prec) {
            expr = lhs;
            break;
        }

        // Okay, we know this is a binop.
        Token_Kind binary_op_token = pctx->curr_tok.kind;
        eat(pctx); // eat binop

        // Parse the unary expression after the binary operator.
        AST* rhs = parse_unary(pctx);
        if (!rhs) {
            expr = NULL;
            break;
        }
        // If BinOp binds less tightly with rhs than the operator after rhs, let
        // the pending operator take rhs as its lhs.
        s32 next_prec = get_tok_precedence(pctx);
        if (tok_prec < next_prec) {
            rhs = parse_binary(pctx, tok_prec + 1, rhs);

            if (!rhs) {
                expr = NULL;
                break;
            }
        }

        // Merge LHS/rhs.
        lhs = make_ast_binary(pctx->curr_tok, binary_op_token, lhs, rhs);
    }

    return expr;
}

AST* read_subscript_expr(Parser_Context* pctx, AST* expr)
{
    eat_kind(pctx, TOKEN_OPEN_BRACKET);
    AST* sub = parse_expression(pctx);
    eat_kind(pctx, TOKEN_CLOSE_BRACKET);
    sub = make_ast_subscript(pctx->curr_tok, expr, sub);
    ast_ref_list_append(&pctx->subscripts, sub);
    return sub;
}

AST* read_field_access(Parser_Context* pctx, AST* expr)
{
    eat_kind(pctx, TOKEN_DOT);
    char* field_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    AST* field = make_ast_field_access(pctx->curr_tok, expr, field_name);
    ast_ref_list_append(&pctx->field_access, field);
    return field;
}

AST* parse_postfix_tail(Parser_Context* pctx, AST* primary_expr)
{
    DEBUG_START;
    if (!primary_expr) return NULL;
    while (1) {
        if (tok_is(pctx, TOKEN_OPEN_BRACKET)) {
            primary_expr = read_subscript_expr(pctx, primary_expr);
            continue;
        }
        if (tok_is(pctx, TOKEN_DOT)) {
            primary_expr = read_field_access(pctx, primary_expr);
            continue;
        }
        return primary_expr;
    }
}

AST* parse_postfix(Parser_Context* pctx)
{
    DEBUG_START;
    AST* primary_expr = parse_primary(pctx);
    return parse_postfix_tail(pctx, primary_expr);
}

AST* parse_cast(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_CAST);
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Type* type = get_type(pctx);
    eat_kind(pctx, TOKEN_COMMA);
    AST* expr = parse_expression(pctx);
    eat_kind(pctx, TOKEN_CLOSE_PAREN);
    return make_ast_cast(pctx->curr_tok, expr, type);
}

AST* parse_unary(Parser_Context* pctx)
{
    DEBUG_START;
    AST* unary = NULL;

    for (s8 i = 0; i < UNARY_OP_COUNT; ++i) {
        if (tok_is(pctx, unary_ops[i])) {
            Token_Kind op = pctx->curr_tok.kind;
            eat(pctx);

            AST* operand = parse_unary(pctx);
            if (operand) {
                unary = make_ast_unary(pctx->curr_tok, op, operand);
            }
        }
    }

    // If the current token is not an operator, it must be a primary expression.
    return unary ? unary : parse_postfix(pctx);
}

AST* parse_note(Parser_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_DOLLAR_SIGN);
    AST* expr = NULL;
    switch (pctx->curr_tok.kind) {
    case TOKEN_HEX: expr = parse_integer(pctx); break;
    case TOKEN_INTEGER: expr = parse_integer(pctx); break;
    default: error("unknown token when expecting a note");
    }
    return make_ast_note(pctx->curr_tok, expr);
}

AST* parse_expression(Parser_Context* pctx)
{
    DEBUG_START;
    AST* lhs  = parse_unary(pctx);
    AST* expr = NULL;
    if (lhs) {
        expr = parse_binary(pctx, 0, lhs);
    }
    return expr;
}

AST* parse_float(Parser_Context* pctx)
{
    DEBUG_START;
    AST* res = make_ast_float(pctx->curr_tok, get_float(pctx));
    return res;
}
AST* parse_integer(Parser_Context* pctx)
{
    DEBUG_START;
    AST* res = make_ast_int(pctx->curr_tok, get_integer(pctx));
    return res;
}

AST* parse_parens(Parser_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_OPEN_PAREN);
    AST* expr = parse_expression(pctx);
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    return make_ast_grouping(pctx->curr_tok, expr);
}

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

Type* parse_enum_signature(Parser_Context* pctx, char* enum_name)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_BLOCK_START);
    List* members = make_list();
    while (!tok_is(pctx, TOKEN_BLOCK_END)) {
        AST* expr = parse_identifier(pctx);
        if (expr) {
            list_append(members, expr);
        }
    }
    eat_kind(pctx, TOKEN_BLOCK_END);
    return make_type_enum(enum_name, members);
}

Type* parse_struct_signature(Parser_Context* pctx, char* struct_name)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_BLOCK_START);
    List* members = make_list();
    while (!tok_is(pctx, TOKEN_BLOCK_END)) {
        AST* expr = parse_statement(pctx);
        if (expr) {
            list_append(members, expr);
        }
    }
    eat_kind(pctx, TOKEN_BLOCK_END);
    return make_type_struct(struct_name, members);
}

Type* parse_extern_function_signature(Parser_Context* pctx, char* func_name)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    List* args                   = make_list();
    bool  has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);
        Type* type             = get_type(pctx);
        AST*  expr             = make_ast_variable_decl(pctx->curr_tok, NULL, type, NULL);
        has_multiple_arguments = true;
        list_append(args, expr);
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    Type* ret_type = NULL;
    if (tok_is_on_same_line(pctx) && tok_is(pctx, TOKEN_IDENTIFIER)) {
        ret_type = get_type(pctx);
    }

    return make_type_function(func_name, args, ret_type);
}

Type* parse_function_signature(Parser_Context* pctx, char* func_name)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    List* args                   = make_list();
    bool  has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);
        char* ident = pctx->curr_tok.value;
        eat_kind(pctx, TOKEN_IDENTIFIER);
        AST* expr              = parse_variable_decl(pctx, ident);
        has_multiple_arguments = true;
        list_append(args, expr);
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);
    return make_type_function(func_name, args, NULL);
}

Type* get_type(Parser_Context* pctx)
{
    DEBUG_START;

    char* type_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);

    Type* type = map_get(pctx->symbols, type_name);
    if (!type) {
        type       = make_type_unresolved(type_name);
        type->name = type_name;
        type_ref_list_append(&pctx->unresolved_types, type);
    }
    type->name = type_name;

    switch (pctx->curr_tok.kind) {
    case THI_SYNTAX_POINTER: {
        while (tok_is(pctx, THI_SYNTAX_POINTER)) {
            eat_kind(pctx, THI_SYNTAX_POINTER);
            type = make_type_pointer(type);
        }
    } break;
    case TOKEN_OPEN_BRACKET: {
        while (tok_is(pctx, TOKEN_OPEN_BRACKET)) {
            eat_kind(pctx, TOKEN_OPEN_BRACKET);
            s64 size = 0;
            if (tok_is(pctx, TOKEN_INTEGER) || tok_is(pctx, TOKEN_HEX)) {
                size = get_integer(pctx);
            }
            eat_kind(pctx, TOKEN_CLOSE_BRACKET);
            type = make_type_array(type, size);
        }
    } break;
    }

    return type;
}

s64 get_integer(Parser_Context* pctx)
{
    DEBUG_START;

    s64 value = 0;
    switch (pctx->curr_tok.kind) {
    case TOKEN_CHAR: {
        u8 c = pctx->curr_tok.value[0];
        if (c == '\\') {
            u8 c = pctx->curr_tok.value[1];
            switch (c) {
            case 'a': value = 7; break;
            case 'n': value = 10; break;
            case 't': value = 9; break;
            case '\\': value = 92; break;
            case '\'': value = 27; break;
            case '"': value = 22; break;
            }
        } else
            value = c;
        break;
    }
    case TOKEN_INTEGER: value = atoll(pctx->curr_tok.value); break;
    case TOKEN_HEX: value = strtoll(pctx->curr_tok.value, NULL, 0); break;
    default: error("not an integer.");
    }
    eat(pctx);

    return value;
}

f64 get_float(Parser_Context* pctx)
{
    DEBUG_START;

    f64 value = atof(pctx->curr_tok.value);
    eat_kind(pctx, TOKEN_FLOAT);

    return value;
}

int get_tok_precedence(Parser_Context* pctx)
{
    DEBUG_START;
    for (int i = 0; i < BIN_OP_COUNT; ++i)
        if (binop_precedence[i].kind == pctx->curr_tok.kind) return binop_precedence[i].p;
    return -1;
}

Token next_tok(Parser_Context* pctx)
{
    if (pctx->tokens.count < pctx->token_index + 1) {
        error("No next token. We're all out.");
    }
    return pctx->tokens.data[pctx->token_index];
}

Token_Kind next_tok_kind(Parser_Context* pctx)
{
    if (pctx->tokens.count < pctx->token_index + 1) {
        error("No next token. We're all out.");
    }
    Token_Kind kind = pctx->tokens.data[pctx->token_index].kind;
    return kind;
}

bool tok_is_on_same_line(Parser_Context* pctx)
{
    s64 l1 = pctx->curr_tok.line_pos;
    s64 l2 = pctx->prev_tok.line_pos;
    return l1 == l2;
}

bool next_tok_is_on_same_line(Parser_Context* pctx)
{
    Token t2 = next_tok(pctx);
    s64   l1 = pctx->curr_tok.line_pos;
    s64   l2 = t2.line_pos;
    return l1 == l2;
}
bool tok_is(Parser_Context* pctx, Token_Kind kind) { return pctx->curr_tok.kind == kind; }

void eat(Parser_Context* pctx)
{
    pctx->prev_tok = pctx->curr_tok;
    pctx->curr_tok = pctx->tokens.data[pctx->token_index];
    pctx->token_index += 1;
}

void eat_kind(Parser_Context* pctx, Token_Kind kind)
{
    Token_Kind tk = pctx->curr_tok.kind;
    if (tk != kind) {
        error("Expected '%s' got '%s'", token_kind_to_str(kind), token_kind_to_str(tk));
    }
    eat(pctx);
}

void set_if_statement(Parser_Context* pctx, AST* if_statement)
{
    pctx->olast_if_statement = pctx->llast_if_statement;
    pctx->llast_if_statement = if_statement;
}
void restore_if_statement(Parser_Context* pctx) { pctx->llast_if_statement = pctx->olast_if_statement; }
void set_dangling_else(Parser_Context* pctx, AST* else_block) { pctx->llast_if_statement->If.else_block = else_block; }
