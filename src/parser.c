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
    int p;
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

    // Ternary operator
    {TOKEN_QUESTION_MARK, 5}, // >>=
    {TOKEN_COLON, 5},         // >>=
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

bool tok_is(Token_Kind kind);
int get_tok_precedence(void);
void add_new_symbol(void);
void eat(void);
void eat_kind(Token_Kind kind);
Expr* get_definition(const char* ident);
Expr* get_variable_declaration(const char* ident);
Expr* get_variable_typeinferred(const char* ident);
Expr* get_function_call(const char* ident);
Expr* get_subscript(const char* ident);

Expr* parse_top_level(void);
Expr* parse_statement(void);
Expr* parse_primary(void);
Expr* parse_identifier(void);
Expr* parse_block(void);
Expr* parse_ret(void);
Expr* parse_note(void);
Expr* parse_expression(void);
Expr* parse_unary(void);
Expr* parse_binary(int expr_prec, Expr* lhs);
Expr* parse_integer(void);
Expr* parse_parens(void);
Expr* parse_if(void);
Expr* parse_for(void);
Expr* parse_while(void);
Expr* parse_break(void);
Expr* parse_continue(void);

Typespec* parse_struct_signature(const char* struct_name);
Typespec* parse_function_signature(const char* func_name);

void skip_statement_body(void);
void skip_block(void);
void skip_enum_signature(void);
void skip_struct_signature(void);
void skip_function_signature(void);

void skip_type(void);

u64 get_integer(void);
f64 get_float(void);
Typespec* get_type(void);

//-----------------
// Global variables
//-----------------
Token* g_tokens = NULL;
u64 token_index = 0;
Token curr_tok;
Token top_tok;

//------------------------------------------------------------------------------
//                               Public Functions
//------------------------------------------------------------------------------

List generate_ast_from_tokens(Token* tokens)
{
    info("Generating AST from Tokens..");
    g_tokens = tokens;
    token_index = 0;
    top_tok.kind = TOKEN_UNKNOWN;
    curr_tok.kind = TOKEN_UNKNOWN;
    eat();

    List ast_list;
    list_init(&ast_list);

    while (!tok_is(TOKEN_EOF)) {
        Expr* stmt = parse_top_level();
        if (stmt) {
            info("%s", expr_to_str(stmt));
            list_append(&ast_list, stmt);
        }
    }

    return ast_list;
}
void generate_symbol_table_from_tokens(Token* tokens)
{
    info("Generating symbol table..");
    g_tokens = tokens;
    token_index = 0;
    top_tok.kind = TOKEN_UNKNOWN;
    curr_tok.kind = TOKEN_UNKNOWN;
    eat();

    while (!tok_is(TOKEN_EOF)) {
        top_tok = curr_tok;
        switch (curr_tok.kind) {

        case TOKEN_IDENTIFIER: {
            add_new_symbol();
        } break;

        case TOKEN_LOAD: {
            // eat_kind(TOKEN_LOAD);
            // // Set the current directory to the directory of the added file
            // string file = make_string(strf("%s%s", get_current_dir(), curr_tok.value));
            // const char* dir = get_file_directory(file.c_str);
            // set_current_dir(dir);

            // // add_file(file);
            // success("Parsing: '%s'", file.c_str);
            // char* source = get_file_content(file.c_str);
            // warning("%s", source);
            // Token* tokense = generate_tokens_from_source(source);
            // assert(tokense);

            // // Save off current progress
            // int s_token_index = token_index;
            // Token_Kind s_k = curr_tok.kind;

            // generate_symbol_table_from_tokens(tokense);
            // success("Continuing on: '%s'", get_source_file());

            // // Load the saved off progress from previous file
            // token_index = s_token_index;
            // top_tok.kind = TOKEN_UNKNOWN;
            // g_tokens = tokens;
            // curr_tok.kind = s_k;

            // eat_kind(TOKEN_STRING);
            error("TOKEN_LOAD parser not implemented.");
        } break;

        case TOKEN_FOREIGN: {
            eat_kind(TOKEN_FOREIGN);
            const char* func_name = curr_tok.value;
            eat_kind(TOKEN_IDENTIFIER);
            eat_kind(TOKEN_COLON_COLON);
            Typespec* func_t = parse_function_signature(func_name);
            add_foreign_function(func_name, func_t);
            add_symbol(func_name, func_t);
        } break;
        }
    }
}

//------------------------------------------------------------------------------
//                               Private Functions
//------------------------------------------------------------------------------

Expr* parse_top_level(void)
{
    info("parse_top_level");
    top_tok = curr_tok;
    switch (curr_tok.kind) {
    case TOKEN_FOREIGN: {
        eat_kind(TOKEN_FOREIGN);
        eat_kind(TOKEN_IDENTIFIER);
        eat_kind(TOKEN_COLON_COLON);
        skip_function_signature();
    } break;
    case TOKEN_IDENTIFIER: return parse_statement();
    default: warning("unknown toplevel token '%s'", curr_tok.value); eat();
    }
    return NULL;
}

Expr* parse_statement(void)
{
    info("parse_statement");
    switch (curr_tok.kind) {
    case TOKEN_IDENTIFIER: return parse_expression();
    case TOKEN_RETURN: return parse_ret();
    case TOKEN_OPEN_BRACE: return parse_block();
    case TOKEN_OPEN_PAREN: return parse_parens();
    // case TOKEN_PRINT:             return parse_print();
    case TOKEN_BREAK: return parse_break();
    case TOKEN_CONTINUE: return parse_continue();
    case TOKEN_IF: return parse_if();
    case TOKEN_FOR: return parse_for();
    case TOKEN_WHILE: return parse_while();
    default: error("Unhandled token '%s' was not a valid statement", curr_tok.value);
    }
    return NULL;
}

Expr* parse_continue(void)
{
    eat_kind(TOKEN_CONTINUE);
    return make_expr_continue();
}

Expr* parse_break(void)
{
    eat_kind(TOKEN_BREAK);
    return make_expr_break();
}
Expr* parse_while(void)
{
    eat_kind(TOKEN_WHILE);
    Expr* cond = parse_expression();
    if (!cond) {
        error("missing cond value in while loop");
    }
    Expr* body = parse_block();
    return make_expr_while(cond, body);
}
Expr* parse_for(void)
{
    eat_kind(TOKEN_FOR);
    const char* iterator_name = curr_tok.value;
    eat_kind(TOKEN_IDENTIFIER);
    eat_kind(TOKEN_COLON);
    Expr* start = parse_expression();
    if (!start) {
        error("missing start value in for loop");
    }
    eat_kind(TOKEN_DOT_DOT);
    Expr* end = parse_expression();
    if (!end) error("missing end value in for loop");
    Expr* body = parse_block();
    return make_expr_for(iterator_name, start, end, body);
}

Expr* parse_if(void)
{
    eat_kind(TOKEN_IF);
    Expr* cond = parse_expression();
    Expr* then_body = parse_block();
    Expr* else_body = NULL;
    if (tok_is(TOKEN_ELSE)) {
        eat_kind(TOKEN_ELSE);
        else_body = parse_block();
    }
    return make_expr_if(cond, then_body, else_body);
}

Expr* parse_primary()
{
    info("parse_primary");

    switch (curr_tok.kind) {
    case TOKEN_IDENTIFIER: return parse_identifier();
    case TOKEN_DOLLAR_SIGN: return parse_note();

    case TOKEN_HEX: // fallthrough
    case TOKEN_INTEGER: return parse_integer();
    // case TOKEN_FLOATING_POINT: return parse_float();

    // case TOKEN_STRING:         return parse_string();
    case TOKEN_OPEN_PAREN: return parse_parens();
    case TOKEN_OPEN_BRACE:
        return parse_block();
        // case TOKEN_TRUE:           return parse_constant();
        // case TOKEN_FALSE:          return parse_constant();
        // case TOKEN_NIL:            return parse_nil();
        // case TOKEN_CAST:           return parse_cast();
        // case TOKEN_MALLOC:         return parse_malloc();
        // case TOKEN_SIZEOF:         return parse_sizeof();
    }
    return NULL;
}

Expr* parse_identifier()
{
    info("parse_identifier");

    const char* ident = curr_tok.value;
    eat_kind(TOKEN_IDENTIFIER);
    switch (curr_tok.kind) {
    case TOKEN_COLON_COLON:     return get_definition(ident);
    case TOKEN_COLON_EQ:        return get_variable_typeinferred(ident);
    case TOKEN_COLON:           return get_variable_declaration(ident);
    case TOKEN_OPEN_PAREN:      return get_function_call(ident);
    case TOKEN_OPEN_BRACKET:    return get_subscript(ident);
    }
    return make_expr_ident(ident);
}

Expr* parse_block()
{
    Expr** statements = NULL;
    eat_kind(TOKEN_OPEN_BRACE);
    while (!tok_is(TOKEN_CLOSE_BRACE)) {
        Expr* stmt = parse_statement();
        if (stmt) sb_push(statements, stmt);
    }
    eat_kind(TOKEN_CLOSE_BRACE);
    return make_expr_block(statements);
}

Expr* parse_ret()
{
    eat_kind(TOKEN_RETURN);
    Expr* exp = parse_expression();
    return make_expr_ret(exp);
}

Expr* get_subscript(const char* ident)
{
    info("get_subscript");
    eat_kind(TOKEN_OPEN_BRACKET);
    Expr* expr = parse_expression();
    eat_kind(TOKEN_CLOSE_BRACKET);
    return make_expr_subscript(ident, expr);
}

Expr* get_function_call(const char* ident)
{
    info("get_function_call");
    eat_kind(TOKEN_OPEN_PAREN);
    Expr** args = NULL;

    bool has_multiple_arguments = false;
    while (!tok_is(TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(TOKEN_COMMA);
        Expr* arg = parse_expression();
        if (arg)
            sb_push(args, arg);
        else
            error("Invalid expression in function call %s", ident);
        has_multiple_arguments = true;
    }
    eat_kind(TOKEN_CLOSE_PAREN);

    return make_expr_call(ident, args);
}

Expr* get_variable_typeinferred(const char* ident)
{
    eat_kind(TOKEN_COLON_EQ);
    Expr* assignment_expr = parse_expression();
    return make_expr_variable_decl_type_inf(ident, assignment_expr);
}
Expr* get_variable_declaration(const char* ident)
{
    eat_kind(TOKEN_COLON);
    Typespec* variable_type = get_type();
    Expr* assignment_expr = NULL;
    if (tok_is(TOKEN_EQ)) {
        eat_kind(TOKEN_EQ);
        assignment_expr = parse_expression();
    }
    return make_expr_variable_decl(ident, variable_type, assignment_expr);
}

Expr* parse_binary(int expr_prec, Expr* lhs)
{
    // If this is a binop, find its precedence.
    while (1) {
        const int tok_prec = get_tok_precedence();

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (tok_prec < expr_prec) return lhs;

        // Okay, we know this is a binop.
        Token_Kind binary_op_token = curr_tok.kind;
        eat(); // eat binop

        // Parser the unary expression after the binary operator.
        Expr* rhs = parse_unary();
        if (!rhs) {
            return NULL;
        }
        // If BinOp binds less tightly with rhs than the operator after rhs, let
        // the pending operator take rhs as its lhs.
        int next_prec = get_tok_precedence();
        if (tok_prec < next_prec) {
            rhs = parse_binary(tok_prec + 1, rhs);

            if (!rhs) {
                return NULL;
            }
        }

        // Merge LHS/rhs.
        lhs = make_expr_binary(binary_op_token, lhs, rhs);
    }
}

Expr* parse_unary()
{
    if (tok_is(TOKEN_BANG) || tok_is(THI_SYNTAX_POINTER) || tok_is(TOKEN_MINUS) || tok_is(THI_SYNTAX_ADDRESS)) {
        Token_Kind op = curr_tok.kind;
        eat();

        Expr* operand = parse_unary();
        if (operand) {
            return make_expr_unary(op, operand);
        }
    }

    // If the current token is not an operator, it must be a primary expression.
    return parse_primary();
}

Expr* parse_note()
{
    info("parse_note");

    eat_kind(TOKEN_DOLLAR_SIGN);
    Expr* expr = NULL;
    switch (curr_tok.kind) {
    case TOKEN_HEX: expr = parse_integer(); break;
    case TOKEN_INTEGER: expr = parse_integer(); break;
    default: error("unknown token when expecting a note");
    }
    return make_expr_note(expr);
}

Expr* parse_expression()
{
    Expr* lhs = parse_unary();
    if (lhs) {
        return parse_binary(0, lhs);
    }
    return NULL;
}

Expr* parse_integer()
{
    Expr* res = make_expr_int(get_integer());
    return res;
}

Expr* parse_parens()
{
    info("parse_parens");
    eat_kind(TOKEN_OPEN_PAREN);
    Expr* exp = parse_expression();
    eat_kind(TOKEN_CLOSE_PAREN);
    if (!exp) { return NULL; }
    return make_expr_grouping(exp);
}
//------------------------------------------------------------------------------
//                               Type Utilty Functions
//------------------------------------------------------------------------------

u64 get_integer(void)
{
    info("get_integer: %s", curr_tok.value);

    u64 value = 0;
    switch (curr_tok.kind) {
    case TOKEN_INTEGER: value = atoll(curr_tok.value); break;
    case TOKEN_HEX: value = strtoll(curr_tok.value, NULL, 0); break;
    default: error("not an integer."); break;
    }
    eat();
    return value;
}

f64 get_float(void)
{
    info("get_float: %s", curr_tok.value);
    f64 value = atof(curr_tok.value);
    eat_kind(TOKEN_FLOAT);
    return value;
}

void skip_type(void)
{
    info("skip_type: %s", curr_tok.value);

    eat_kind(TOKEN_IDENTIFIER);

    // Is a pointer or array?
    if (tok_is(THI_SYNTAX_POINTER)) {
        eat_kind(THI_SYNTAX_POINTER);
        return;
    } else if (tok_is(TOKEN_OPEN_BRACKET)) {
        eat_kind(TOKEN_OPEN_BRACKET);
        if (tok_is(TOKEN_INTEGER) || tok_is(TOKEN_HEX)) get_integer();
        eat_kind(TOKEN_CLOSE_BRACKET);
        // return make_typespec_array(type, size);
        // return new Type_Array(type, size);
    }
    return;
}

// Returns NULL if the current token is not a type.
Typespec* get_type(void)
{
    info("get_type: %s", curr_tok.value);

    const char* type_name = curr_tok.value;

    eat_kind(TOKEN_IDENTIFIER);
    Typespec* type = NULL;

    if (is_builtin_type(type_name))
        type = get_builtin_type(type_name);
    else
        type = get_symbol(type_name);

    switch (curr_tok.kind) {
    case THI_SYNTAX_POINTER: {
        while (tok_is(THI_SYNTAX_POINTER)) {
            eat_kind(THI_SYNTAX_POINTER);
            type = make_typespec_pointer(type);
        }
    } break;
    case TOKEN_OPEN_BRACKET: {
        eat_kind(TOKEN_OPEN_BRACKET);
        i64 size = 0;
        if (tok_is(TOKEN_INTEGER) || tok_is(TOKEN_HEX)) {
            size = get_integer();
        }
        eat_kind(TOKEN_CLOSE_BRACKET);
        type = make_typespec_array(type, size);
    } break;
    }

    if (!type) warning("no type found for type '%s'", type_name);

    return type;
}

//------------------------------------------------------------------------------
//                               Parsing Utility Functions
//------------------------------------------------------------------------------

Typespec* parse_enum_signature(const char* name)
{
    info("Parsing enum: %s", name);
    eat_kind(TOKEN_ENUM);
    eat_kind(TOKEN_OPEN_BRACE);
    while (!tok_is(TOKEN_CLOSE_BRACE)) {
        info("  enum member: %s", curr_tok.value);
        eat_kind(TOKEN_IDENTIFIER);
    }
    eat_kind(TOKEN_CLOSE_BRACE);

    return make_typespec_enum(name, NULL);
}

Typespec* parse_struct_signature(const char* struct_name)
{
    eat_kind(TOKEN_STRUCT);
    eat_kind(TOKEN_OPEN_BRACE);

    Arg* members = NULL;
    while (!tok_is(TOKEN_CLOSE_BRACE)) {
        Arg member;
        member.name = curr_tok.value;
        eat_kind(TOKEN_IDENTIFIER);
        eat_kind(TOKEN_COLON);
        member.type = get_type();
        sb_push(members, member);
    }
    eat_kind(TOKEN_CLOSE_BRACE);
    return make_typespec_struct(struct_name, members);
}

Typespec* parse_function_signature(const char* func_name)
{
    info("parse_function_signature");
    eat_kind(TOKEN_OPEN_PAREN);
    Arg* args = NULL;
    bool has_multiple_arguments = false;
    while (!tok_is(TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(TOKEN_COMMA);

        Arg arg;
        arg.name = curr_tok.value;
        arg.type = NULL;

        // foreign's dont have named parameters
        if (top_tok.kind == TOKEN_FOREIGN) {
            arg.name = NULL;
            arg.type = get_type();
        } else {
            eat_kind(TOKEN_IDENTIFIER);
            eat_kind(TOKEN_COLON);
            arg.type = get_type();
        }

        has_multiple_arguments = true;
        sb_push(args, arg);
    }
    eat_kind(TOKEN_CLOSE_PAREN);

    Typespec* ret_type = NULL;
    if (tok_is(TOKEN_RIGHT_ARROW)) {
        eat_kind(TOKEN_RIGHT_ARROW);
        ret_type = get_type();
    }

    return make_typespec_function(func_name, args, ret_type);
}

Expr* get_definition(const char* ident)
{
    eat_kind(TOKEN_COLON_COLON);
    switch (curr_tok.kind) {
    case TOKEN_ENUM: {
        eat_kind(TOKEN_ENUM);
        skip_enum_signature();
        // return make_expr_enum(get_symbol(ident));
        return NULL;
    }
    case TOKEN_STRUCT: {
        eat_kind(TOKEN_STRUCT);
        skip_struct_signature();
        return make_expr_struct(get_symbol(ident));
    }
    case TOKEN_OPEN_PAREN: {
        skip_function_signature();
        Expr* body = parse_block();
        return make_expr_function(get_symbol(ident), body);
    }
    default: {
        // Macro def
        return make_expr_macro(ident, parse_expression());
    }
    }

    error("GET DEF RETURNING NULL");
    return NULL;
}

int get_tok_precedence(void)
{
    for (int i = 0; i < BIN_OP_COUNT; ++i)
        if (binop_precedence[i].kind == curr_tok.kind) return binop_precedence[i].p;
    return -1;
}

bool tok_is(Token_Kind kind)
{
    if (curr_tok.kind == kind) {
        return true;
    }
    return false;
}

void eat(void) { curr_tok = g_tokens[token_index++]; }

void eat_kind(Token_Kind kind)
{
    if (curr_tok.kind != kind) {
        error("expected '%s' got '%s'", token_kind_to_str(kind), token_kind_to_str(curr_tok.kind));
    }
    eat();
}

//------------------------------------------------------------------------------
//                               Skip Functions
//------------------------------------------------------------------------------

void skip_statement_body(void)
{
    if(tok_is(TOKEN_OPEN_BRACE))
    {
        skip_block();
        return;
    }
    parse_statement();
}
void skip_block(void)
{
    eat_kind(TOKEN_OPEN_BRACE);
    i64 counter = 1;
    while (true) {
        eat();
        if (tok_is(TOKEN_OPEN_BRACE)) ++counter;
        if (tok_is(TOKEN_CLOSE_BRACE)) --counter;
        if (counter == 0) break;
    }
    eat_kind(TOKEN_CLOSE_BRACE);
}

void skip_enum_signature(void)
{
    if (tok_is(TOKEN_IDENTIFIER)) skip_type();
    eat_kind(TOKEN_OPEN_BRACE);
    while (!tok_is(TOKEN_CLOSE_BRACE))
        eat_kind(TOKEN_IDENTIFIER);
    eat();
}

void skip_struct_signature(void)
{
    eat_kind(TOKEN_STRUCT);
    eat_kind(TOKEN_OPEN_BRACE);
    while (!tok_is(TOKEN_CLOSE_BRACE)) {
        eat_kind(TOKEN_IDENTIFIER);
        eat_kind(TOKEN_COLON);
        skip_type();
    }
    eat_kind(TOKEN_CLOSE_BRACE);
}

void skip_function_signature(void)
{
    info("skip_function_signature");
    eat();
    bool has_multiple_arguments = false;
    while (!tok_is(TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(TOKEN_COMMA);
        if (top_tok.kind == TOKEN_FOREIGN) {
            eat();
        } else {
            eat_kind(TOKEN_IDENTIFIER);
            eat_kind(TOKEN_COLON);
            skip_type();
        }
        has_multiple_arguments = true;
    }
    eat_kind(TOKEN_CLOSE_PAREN);

    if (tok_is(TOKEN_RIGHT_ARROW)) {
        eat_kind(TOKEN_RIGHT_ARROW);
        eat_kind(TOKEN_IDENTIFIER);
    }

}

//------------------------------------------------------------------------------
//                               Order-independence Functions
//------------------------------------------------------------------------------

void add_new_symbol(void)
{
    info("add_new_symbol");

    const char* ident = curr_tok.value;
    eat_kind(TOKEN_IDENTIFIER);
    if(tok_is(TOKEN_COLON_COLON))
    {
        eat_kind(TOKEN_COLON_COLON);
        switch (curr_tok.kind) {
        case TOKEN_ENUM: {
            Typespec* type = parse_enum_signature(ident);
            add_symbol(ident, type);
            return;
        }
        case TOKEN_STRUCT: {
            Typespec* type = parse_struct_signature(ident);
            add_symbol(ident, type);
            return;
        }
        case TOKEN_OPEN_PAREN: {
            Typespec* type = parse_function_signature(ident);
            skip_statement_body();
            add_symbol(ident, type);
            return;
        }
        default: {
            Expr* expr = parse_expression();
            add_macro_def(ident, expr);
            return;
        }
        }
    }
}
