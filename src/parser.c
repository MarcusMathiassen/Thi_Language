#include "parser.h"

#include "lexer.h"           // Token, Token_Kind
#include "stretchy_buffer.h" // sb_push
#include "string.h"
#include "typedefs.h" // i32, i64, etc.
#include "typespec.h" // Typespec
#include "utility.h"  // info, error, warning, success
#include <assert.h>   // assert
#include <ctype.h>    // atoll

#include "globals.h" // add_symbol

#define BIN_OP_COUNT 47
struct
{
    Token_Kind kind;
    i32 p;
} binop_precedence[BIN_OP_COUNT] = {
    {TOKEN_DOT, 100},          // .
    {TOKEN_OPEN_PAREN, 100},   // ()
    {TOKEN_OPEN_BRACKET, 100}, // []

    {TOKEN_PLUS_PLUS, 90},   // ++
    {TOKEN_MINUS_MINUS, 90}, // --

    {TOKEN_BANG, 80},     // !
    {TOKEN_ASTERISK, 80}, // *
    {TOKEN_FWSLASH, 80},  // /

    {TOKEN_MINUS, 70}, // -
    {TOKEN_PLUS, 70},  // +

    {TOKEN_LT, 60},    // <
    {TOKEN_GT, 60},    // >
    {TOKEN_GT_EQ, 60}, // >=
    {TOKEN_LT_EQ, 60}, // <=

    {TOKEN_EQ_EQ, 50},   // ==
    {TOKEN_BANG_EQ, 50}, // !=

    {TOKEN_AND_AND, 48},   // &&
    {TOKEN_PIPE_PIPE, 47}, // ||

    {TOKEN_AT, 40},   // @
    {TOKEN_AND, 40},  // &
    {TOKEN_HAT, 30},  // ^
    {TOKEN_PIPE, 20}, // |

    {TOKEN_QUESTION_MARK, 15}, // ?
    {TOKEN_COLON, 15}, // :

    {TOKEN_EQ, 10},          // =
    {TOKEN_PLUS_EQ, 10},     // +=
    {TOKEN_MINUS_EQ, 10},    // -=
    {TOKEN_ASTERISK_EQ, 10}, // *=
    {TOKEN_FWSLASH_EQ, 10},  // /=
    {TOKEN_PERCENT_EQ, 10},  // %=
    {TOKEN_AND_EQ, 10},      // &=
    {TOKEN_HAT_EQ, 10},      // ^=
    {TOKEN_PIPE_EQ, 10},     // |=

    {TOKEN_BITWISE_LEFTSHIFT, 10},  // <<=
    {TOKEN_BITWISE_RIGHTSHIFT, 10}, // >>=
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

typedef struct 
{   
    Token* g_tokens;
    List* ast_list_ptr;
    u64 token_index;
    Token curr_tok;
    Token top_tok;
} Parse_Context;

bool tok_is(Parse_Context* pctx, Token_Kind kind);
int get_tok_precedence(Parse_Context* pctx);
void add_new_symbol(Parse_Context* pctx);
void eat(Parse_Context* pctx);
void eat_kind(Parse_Context* pctx, Token_Kind kind);
Expr* get_definition(Parse_Context* pctx, char* ident);
Expr* get_variable_declaration(Parse_Context* pctx, char* ident);
Expr* get_variable_typeinferred(Parse_Context* pctx, char* ident);
Expr* get_function_call(Parse_Context* pctx, char* ident);
Expr* get_subscript(Parse_Context* pctx, char* ident);

Expr* parse_top_level(Parse_Context* pctx);
Expr* parse_statement(Parse_Context* pctx);
Expr* parse_primary(Parse_Context* pctx);
Expr* parse_identifier(Parse_Context* pctx);
Expr* parse_block(Parse_Context* pctx);
Expr* parse_ret(Parse_Context* pctx);
Expr* parse_note(Parse_Context* pctx);
Expr* parse_expression(Parse_Context* pctx);
Expr* parse_unary(Parse_Context* pctx);
Expr* parse_binary(Parse_Context* pctx, int expr_prec, Expr* lhs);
Expr* parse_integer(Parse_Context* pctx);
Expr* parse_parens(Parse_Context* pctx);
Expr* parse_if(Parse_Context* pctx);
Expr* parse_for(Parse_Context* pctx);
Expr* parse_while(Parse_Context* pctx);
Expr* parse_break(Parse_Context* pctx);
Expr* parse_continue(Parse_Context* pctx);
Expr* parse_string(Parse_Context* pctx);

Typespec* parse_struct_signature(Parse_Context* pctx, char* struct_name);
Typespec* parse_function_signature(Parse_Context* pctx, char* func_name);

void skip_statement_body(Parse_Context* pctx);
void skip_block(Parse_Context* pctx);
void skip_enum_signature(Parse_Context* pctx);
void skip_struct_signature(Parse_Context* pctx);
void skip_function_signature(Parse_Context* pctx);

void skip_type(Parse_Context* pctx);

u64 get_integer(Parse_Context* pctx);
f64 get_float(Parse_Context* pctx);
Typespec* get_type(Parse_Context* pctx);

//------------------------------------------------------------------------------
//                               Public Functions
//------------------------------------------------------------------------------

void generate_ast_from_tokens(List* ast, Token* tokens)
{
    info("Generating AST from Tokens..");

    Parse_Context pctx;
    pctx.g_tokens = tokens;
    pctx.token_index = 0;
    pctx.top_tok.kind = TOKEN_UNKNOWN;
    pctx.curr_tok.kind = TOKEN_UNKNOWN;
    pctx.ast_list_ptr = ast;

    eat(&pctx);

    while (!tok_is(&pctx, TOKEN_EOF)) {
        Expr* stmt = parse_top_level(&pctx);
        if (stmt) {
            info("%s", expr_to_str(stmt));
            list_append(ast, stmt);
        }
    }
}

void generate_symbol_table_from_tokens(List* ast, Token* tokens)
{
    info("Generating symbol table..");
    Parse_Context pctx;
    pctx.g_tokens = tokens;
    pctx.token_index = 0;
    pctx.top_tok.kind = TOKEN_UNKNOWN;
    pctx.curr_tok.kind = TOKEN_UNKNOWN;
    pctx.ast_list_ptr = ast;

    eat(&pctx);

    while (!tok_is(&pctx, TOKEN_EOF)) {
        pctx.top_tok = pctx.curr_tok;
        switch (pctx.curr_tok.kind) {

        case TOKEN_IDENTIFIER: {
            add_new_symbol(&pctx);
        } break;


        case TOKEN_LOAD: {

            eat_kind(&pctx, TOKEN_LOAD);

            // // Set the current directory to the directory of the added file
            string file = make_string(strf("%s%s", get_current_dir(), pctx.curr_tok.value));
            char* dir = get_file_directory(file.c_str);
            set_current_dir(dir);

            success("Parsing: '%s'", file.c_str);
            char* source = get_file_content(file.c_str);

            push_timer(file.c_str);
            Token* tokense = generate_tokens_from_source(source);

            generate_symbol_table_from_tokens(ast, tokense);
            generate_ast_from_tokens(ast, tokense);
            pop_timer();

            success("Continuing on: '%s'", get_source_file());

            eat_kind(&pctx, TOKEN_STRING);
        } break;

        case TOKEN_FOREIGN: {
            eat_kind(&pctx, TOKEN_FOREIGN);
            char* func_name = pctx.curr_tok.value;
            eat_kind(&pctx, TOKEN_IDENTIFIER);
            eat_kind(&pctx, TOKEN_COLON_COLON);
            Typespec* func_t = parse_function_signature(&pctx, func_name);
            add_foreign_function(func_name, func_t);
            add_symbol(func_name, func_t);
        } break;
        }
    }
}

//------------------------------------------------------------------------------
//                               Private Functions
//------------------------------------------------------------------------------

Expr* parse_top_level(Parse_Context* pctx)
{
    info("parse_top_level");
    pctx->top_tok = pctx->curr_tok;
    switch (pctx->curr_tok.kind) {
    case TOKEN_FOREIGN: {
        eat_kind(pctx, TOKEN_FOREIGN);
        eat_kind(pctx, TOKEN_IDENTIFIER);
        eat_kind(pctx, TOKEN_COLON_COLON);
        skip_function_signature(pctx);
    } break;
    case TOKEN_IDENTIFIER: return parse_statement(pctx);
    default: warning("unknown toplevel token '%s'", pctx->curr_tok.value); eat(pctx);
    }
    return NULL;
}

Expr* parse_statement(Parse_Context* pctx)
{
    info("parse_statement");
    switch (pctx->curr_tok.kind) {
    case TOKEN_IDENTIFIER: return parse_expression(pctx);
    case TOKEN_RETURN: return parse_ret(pctx);
    case TOKEN_OPEN_BRACE: return parse_block(pctx);
    case TOKEN_OPEN_PAREN: return parse_parens(pctx);
    // case TOKEN_PRINT:             return parse_print();
    case TOKEN_BREAK: return parse_break(pctx);
    case TOKEN_CONTINUE: return parse_continue(pctx);
    case TOKEN_IF: return parse_if(pctx);
    case TOKEN_FOR: return parse_for(pctx);
    case TOKEN_WHILE: return parse_while(pctx);
    default: error("Unhandled token '%s' was not a valid statement", pctx->curr_tok.value);
    }
    return NULL;
}

Expr* parse_continue(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_CONTINUE);
    return make_expr_continue();
}

Expr* parse_break(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_BREAK);
    return make_expr_break();
}
Expr* parse_while(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_WHILE);
    Expr* cond = parse_expression(pctx);
    if (!cond) {
        error("missing cond value in while loop");
    }
    Expr* body = parse_block(pctx);
    return make_expr_while(cond, body);
}
Expr* parse_for(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_FOR);
    char* iterator_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    eat_kind(pctx, TOKEN_COLON);
    Expr* start = parse_expression(pctx);
    if (!start) {
        error("missing start value in for loop");
    }
    eat_kind(pctx, TOKEN_DOT_DOT);
    Expr* end = parse_expression(pctx);
    if (!end) error("missing end value in for loop");
    Expr* body = parse_block(pctx);
    return make_expr_for(iterator_name, start, end, body);
}

Expr* parse_if(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_IF);
    Expr* cond = parse_expression(pctx);
    Expr* then_body = parse_block(pctx);
    Expr* else_body = NULL;
    if (tok_is(pctx, TOKEN_ELSE)) {
        eat_kind(pctx, TOKEN_ELSE);
        else_body = parse_block(pctx);
    }
    return make_expr_if(cond, then_body, else_body);
}

Expr* parse_primary(Parse_Context* pctx)
{
    info("parse_primary");

    switch (pctx->curr_tok.kind) {
    case TOKEN_IDENTIFIER: return parse_identifier(pctx);
    case TOKEN_DOLLAR_SIGN: return parse_note(pctx);

    case TOKEN_HEX: // fallthrough
    case TOKEN_INTEGER: return parse_integer(pctx);
    // case TOKEN_FLOATING_POINT: return parse_float();

    case TOKEN_STRING:         return parse_string(pctx);
    case TOKEN_OPEN_PAREN: return parse_parens(pctx);
    case TOKEN_OPEN_BRACE:
        return parse_block(pctx);
        // case TOKEN_TRUE:           return parse_constant();
        // case TOKEN_FALSE:          return parse_constant();
        // case TOKEN_NIL:            return parse_nil();
        // case TOKEN_CAST:           return parse_cast();
        // case TOKEN_MALLOC:         return parse_malloc();
        // case TOKEN_SIZEOF:         return parse_sizeof();
    }
    return NULL;
}

Expr* parse_identifier(Parse_Context* pctx)
{
    info("parse_identifier");

    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    switch (pctx->curr_tok.kind) {
    case TOKEN_COLON_COLON:     return get_definition(pctx, ident);
    case TOKEN_COLON_EQ:        return get_variable_typeinferred(pctx, ident);
    case TOKEN_COLON:           return get_variable_declaration(pctx, ident);
    case TOKEN_OPEN_PAREN:      return get_function_call(pctx, ident);
    case TOKEN_OPEN_BRACKET:    return get_subscript(pctx, ident);
    }
    return make_expr_ident(ident);
}

Expr* parse_string(Parse_Context* pctx)
{
    char* value = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_STRING);
    add_constant_string(value);
    return make_expr_string(value);
}

Expr* parse_block(Parse_Context* pctx)
{
    Expr** statements = NULL;
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE)) {
        Expr* stmt = parse_statement(pctx);
        if (stmt) sb_push(statements, stmt);
    }
    eat_kind(pctx, TOKEN_CLOSE_BRACE);
    return make_expr_block(statements);
}

Expr* parse_ret(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_RETURN);
    Expr* exp = parse_expression(pctx);
    return make_expr_ret(exp);
}

Expr* get_subscript(Parse_Context* pctx, char* ident)
{
    info("get_subscript");
    eat_kind(pctx, TOKEN_OPEN_BRACKET);
    Expr* expr = parse_expression(pctx);
    eat_kind(pctx, TOKEN_CLOSE_BRACKET);
    return make_expr_subscript(ident, expr);
}

Expr* get_function_call(Parse_Context* pctx, char* ident)
{
    info("get_function_call");
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Expr** args = NULL;

    bool has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);
        Expr* arg = parse_expression(pctx);
        if (arg)
            sb_push(args, arg);
        else
            error("Invalid expression in function call %s", ident);
        has_multiple_arguments = true;
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    return make_expr_call(ident, args);
}

Expr* get_variable_typeinferred(Parse_Context* pctx, char* ident)
{
    eat_kind(pctx, TOKEN_COLON_EQ);
    Expr* assignment_expr = parse_expression(pctx);
    return make_expr_variable_decl_type_inf(ident, assignment_expr);
}
Expr* get_variable_declaration(Parse_Context* pctx, char* ident)
{
    eat_kind(pctx, TOKEN_COLON);
    Typespec* variable_type = get_type(pctx);
    Expr* assignment_expr = NULL;
    if (tok_is(pctx, TOKEN_EQ)) {
        eat_kind(pctx, TOKEN_EQ);
        assignment_expr = parse_expression(pctx);
    }
    return make_expr_variable_decl(ident, variable_type, assignment_expr);
}

Expr* parse_binary(Parse_Context* pctx, int expr_prec, Expr* lhs)
{
    // If this is a binop, find its precedence.
    while (1) {
        const i32 tok_prec = get_tok_precedence(pctx);

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (tok_prec < expr_prec) return lhs;

        // Okay, we know this is a binop.
        Token_Kind binary_op_token = pctx->curr_tok.kind;
        eat(pctx); // eat binop

        // Parser the unary expression after the binary operator.
        Expr* rhs = parse_unary(pctx);
        if (!rhs) {
            return NULL;
        }
        // If BinOp binds less tightly with rhs than the operator after rhs, let
        // the pending operator take rhs as its lhs.
        i32 next_prec = get_tok_precedence(pctx);
        if (tok_prec < next_prec) {
            rhs = parse_binary(pctx, tok_prec + 1, rhs);

            if (!rhs) {
                return NULL;
            }
        }

        // Merge LHS/rhs.
        lhs = make_expr_binary(binary_op_token, lhs, rhs);
    }
}

Expr* parse_unary(Parse_Context* pctx)
{
    if (tok_is(pctx, TOKEN_BANG) || tok_is(pctx, THI_SYNTAX_POINTER) || tok_is(pctx, TOKEN_MINUS) || tok_is(pctx, THI_SYNTAX_ADDRESS)) {
        Token_Kind op = pctx->curr_tok.kind;
        eat(pctx);

        Expr* operand = parse_unary(pctx);
        if (operand) {
            return make_expr_unary(op, operand);
        }
    }

    // If the current token is not an operator, it must be a primary expression.
    return parse_primary(pctx);
}

Expr* parse_note(Parse_Context* pctx)
{
    info("parse_note");

    eat_kind(pctx, TOKEN_DOLLAR_SIGN);
    Expr* expr = NULL;
    switch (pctx->curr_tok.kind) {
    case TOKEN_HEX: expr = parse_integer(pctx); break;
    case TOKEN_INTEGER: expr = parse_integer(pctx); break;
    default: error("unknown token when expecting a note");
    }
    return make_expr_note(expr);
}

Expr* parse_expression(Parse_Context* pctx)
{
    Expr* lhs = parse_unary(pctx);
    if (lhs) {
        return parse_binary(pctx, 0, lhs);
    }
    return NULL;
}

Expr* parse_integer(Parse_Context* pctx)
{
    Expr* res = make_expr_int(get_integer(pctx));
    return res;
}

Expr* parse_parens(Parse_Context* pctx)
{
    info("parse_parens");
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Expr* exp = parse_expression(pctx);
    eat_kind(pctx, TOKEN_CLOSE_PAREN);
    if (!exp) { return NULL; }
    return make_expr_grouping(exp);
}
//------------------------------------------------------------------------------
//                               Type Utilty Functions
//------------------------------------------------------------------------------

u64 get_integer(Parse_Context* pctx)
{
    info("get_integer: %s", pctx->curr_tok.value);

    u64 value = 0;
    switch (pctx->curr_tok.kind) {
    case TOKEN_INTEGER: value = atoll(pctx->curr_tok.value); break;
    case TOKEN_HEX: value = strtoll(pctx->curr_tok.value, NULL, 0); break;
    default: error("not an integer."); break;
    }
    eat(pctx);
    return value;
}

f64 get_float(Parse_Context* pctx)
{
    info("get_float: %s", pctx->curr_tok.value);
    f64 value = atof(pctx->curr_tok.value);
    eat_kind(pctx, TOKEN_FLOAT);
    return value;
}

void skip_type(Parse_Context* pctx)
{
    info("skip_type: %s", pctx->curr_tok.value);

    eat_kind(pctx, TOKEN_IDENTIFIER);

    switch (pctx->curr_tok.kind) {
    case THI_SYNTAX_POINTER: {
        while (tok_is(pctx, THI_SYNTAX_POINTER)) {
            eat_kind(pctx, THI_SYNTAX_POINTER);
        }
    } break;
    case TOKEN_OPEN_BRACKET: {
        eat_kind(pctx, TOKEN_OPEN_BRACKET);
        if (tok_is(pctx, TOKEN_INTEGER) || tok_is(pctx, TOKEN_HEX)) {
            get_integer(pctx);
        }
        eat_kind(pctx, TOKEN_CLOSE_BRACKET);
    } break;
    }
}

// Returns NULL if the current token is not a type.
Typespec* get_type(Parse_Context* pctx)
{
    info("get_type: %s", pctx->curr_tok.value);

    char* type_name = pctx->curr_tok.value;

    eat_kind(pctx, TOKEN_IDENTIFIER);
    Typespec* type = NULL;

    if (is_builtin_type(type_name))
        type = get_builtin_type(type_name);
    else
        type = get_symbol(type_name);

    switch (pctx->curr_tok.kind) {
    case THI_SYNTAX_POINTER: {
        while (tok_is(pctx, THI_SYNTAX_POINTER)) {
            eat_kind(pctx, THI_SYNTAX_POINTER);
            type = make_typespec_pointer(type);
        }
    } break;
    case TOKEN_OPEN_BRACKET: {
        eat_kind(pctx, TOKEN_OPEN_BRACKET);
        i64 size = 0;
        if (tok_is(pctx, TOKEN_INTEGER) || tok_is(pctx, TOKEN_HEX)) {
            size = get_integer(pctx);
        }
        eat_kind(pctx, TOKEN_CLOSE_BRACKET);
        type = make_typespec_array(type, size);
    } break;
    }

    if (!type) warning("no type found for type '%s'", type_name);

    return type;
}

//------------------------------------------------------------------------------
//                               Parsing Utility Functions
//------------------------------------------------------------------------------

Typespec* parse_enum_signature(Parse_Context* pctx, char* name)
{
    info("Parsing enum: %s", name);
    eat_kind(pctx, TOKEN_ENUM);
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE)) {
        info("  enum member: %s", pctx->curr_tok.value);
        eat_kind(pctx, TOKEN_IDENTIFIER);
    }
    eat_kind(pctx, TOKEN_CLOSE_BRACE);

    return make_typespec_enum(name, NULL);
}

Typespec* parse_struct_signature(Parse_Context* pctx, char* struct_name)
{
    eat_kind(pctx, TOKEN_STRUCT);
    eat_kind(pctx, TOKEN_OPEN_BRACE);

    Arg* members = NULL;
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE)) {
        Arg member;
        member.name = pctx->curr_tok.value;
        eat_kind(pctx, TOKEN_IDENTIFIER);
        eat_kind(pctx, TOKEN_COLON);
        member.type = get_type(pctx);
        sb_push(members, member);
    }
    eat_kind(pctx, TOKEN_CLOSE_BRACE);
    return make_typespec_struct(struct_name, members);
}

Typespec* parse_function_signature(Parse_Context* pctx, char* func_name)
{
    info("parse_function_signature");
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Arg* args = NULL;
    bool has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);

        Arg arg;
        arg.name = pctx->curr_tok.value;
        arg.type = NULL;

        // foreign's dont have named parameters
        if (pctx->top_tok.kind == TOKEN_FOREIGN) {
            arg.name = NULL;
            arg.type = get_type(pctx);
        } else {
            eat_kind(pctx, TOKEN_IDENTIFIER);
            eat_kind(pctx, TOKEN_COLON);
            arg.type = get_type(pctx);
        }

        has_multiple_arguments = true;
        sb_push(args, arg);
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    Typespec* ret_type = NULL;
    if (tok_is(pctx, TOKEN_RIGHT_ARROW)) {
        eat_kind(pctx, TOKEN_RIGHT_ARROW);
        ret_type = get_type(pctx);
    }

    return make_typespec_function(func_name, args, ret_type);
}

Expr* get_definition(Parse_Context* pctx, char* ident)
{
    eat_kind(pctx, TOKEN_COLON_COLON);
    switch (pctx->curr_tok.kind) {
    case TOKEN_ENUM: {
        eat_kind(pctx, TOKEN_ENUM);
        skip_enum_signature(pctx);
        // return make_expr_enum(get_symbol(ident));
        return NULL;
    }
    case TOKEN_STRUCT: {
        eat_kind(pctx, TOKEN_STRUCT);
        skip_struct_signature(pctx);
        return make_expr_struct(get_symbol(ident));
    }
    case TOKEN_OPEN_PAREN: {
        skip_function_signature(pctx);
        Expr* body = parse_block(pctx);
        return make_expr_function(get_symbol(ident), body);
    }
    default: {
        // Macro def
        parse_expression(pctx);
        return NULL;
        // return make_expr_macro(ident, parse_expression(pctx));
    }
    }

    error("GET DEF RETURNING NULL");
    return NULL;
}

int get_tok_precedence(Parse_Context* pctx)
{
    for (int i = 0; i < BIN_OP_COUNT; ++i)
        if (binop_precedence[i].kind == pctx->curr_tok.kind) return binop_precedence[i].p;
    return -1;
}

bool tok_is(Parse_Context* pctx, Token_Kind kind)
{
    if (pctx->curr_tok.kind == kind) {
        return true;
    }
    return false;
}

void eat(Parse_Context* pctx) { 
        pctx->curr_tok = pctx->g_tokens[pctx->token_index++];
}

void eat_kind(Parse_Context* pctx, Token_Kind kind)
{
    if (pctx->curr_tok.kind != kind) {
        error("expected '%s' got '%s'", token_kind_to_str(kind), token_kind_to_str(pctx->curr_tok.kind));
    }
    eat(pctx);
}

//------------------------------------------------------------------------------
//                               Skip Functions
//------------------------------------------------------------------------------

void skip_statement_body(Parse_Context* pctx)
{
    if(tok_is(pctx, TOKEN_OPEN_BRACE))
    {
        skip_block(pctx);
        return;
    }
    parse_statement(pctx);
}

void skip_block(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    i64 counter = 1;
    while (true) {
        eat(pctx);
        if (tok_is(pctx, TOKEN_OPEN_BRACE)) ++counter;
        if (tok_is(pctx, TOKEN_CLOSE_BRACE)) --counter;
        if (counter == 0) break;
    }
    eat_kind(pctx, TOKEN_CLOSE_BRACE);
}

void skip_enum_signature(Parse_Context* pctx)
{
    if (tok_is(pctx, TOKEN_IDENTIFIER)) skip_type(pctx);
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE))
        eat_kind(pctx, TOKEN_IDENTIFIER);
    eat(pctx);
}

void skip_struct_signature(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_STRUCT);
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE)) {
        eat_kind(pctx, TOKEN_IDENTIFIER);
        eat_kind(pctx, TOKEN_COLON);
        skip_type(pctx);
    }
    eat_kind(pctx, TOKEN_CLOSE_BRACE);
}

void skip_function_signature(Parse_Context* pctx)
{
    info("skip_function_signature");
    eat(pctx);
    bool has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);
        if (pctx->top_tok.kind == TOKEN_FOREIGN) {
            skip_type(pctx);
        } else {
            eat_kind(pctx, TOKEN_IDENTIFIER);
            eat_kind(pctx, TOKEN_COLON);
            skip_type(pctx);
        }
        has_multiple_arguments = true;
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    if (tok_is(pctx, TOKEN_RIGHT_ARROW)) {
        eat_kind(pctx, TOKEN_RIGHT_ARROW);
        skip_type(pctx);
    }

}

//------------------------------------------------------------------------------
//                               Order-independence Functions
//------------------------------------------------------------------------------

void add_new_symbol(Parse_Context* pctx)
{
    info("add_new_symbol");

    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    if(tok_is(pctx, TOKEN_COLON_COLON))
    {
        eat_kind(pctx, TOKEN_COLON_COLON);
        switch (pctx->curr_tok.kind) {
        case TOKEN_ENUM: {
            Typespec* type = parse_enum_signature(pctx, ident);
            add_symbol(ident, type);
            return;
        }
        case TOKEN_STRUCT: {
            Typespec* type = parse_struct_signature(pctx, ident);
            add_symbol(ident, type);
            return;
        }
        case TOKEN_OPEN_PAREN: {
            Typespec* type = parse_function_signature(pctx, ident);
            skip_statement_body(pctx);
            add_symbol(ident, type);
            return;
        }
        default: {
            Expr* expr = parse_expression(pctx);
            add_macro_def(ident, expr);
            return;
        }
        }
    }
}
