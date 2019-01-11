#include "parser.h"

#include "lexer.h"           // Token, Token_Kind
#include "string.h"
#include "typedefs.h" // s32, s64, etc.
#include "typespec.h" // Typespec
#include "register.h" // get_reg, get_parameter_reg
#include "utility.h"  // info, error, warning, success
#include <assert.h>   // assert
#include <ctype.h>    // atoll
#include <string.h>   // strcmp
#include <stdlib.h>   // xmalloc

#include "globals.h" // add_symbol

#define BIN_OP_COUNT 51
struct
{
    Token_Kind kind;
    s32 p;
} binop_precedence[BIN_OP_COUNT] = {

    {TOKEN_PLUS_PLUS, 100},   // ++
    {TOKEN_MINUS_MINUS, 100}, // --
    {TOKEN_OPEN_BRACKET, 100}, // []
    {TOKEN_DOT, 100},          // .
    {TOKEN_OPEN_PAREN, 100},   // ()
    {TOKEN_RIGHT_ARROW, 100},   // ->

    {TOKEN_BANG, 85},     // !
    {TOKEN_TILDE, 85},     // ~
    {THI_SYNTAX_POINTER, 85}, // @
    {THI_SYNTAX_ADDRESS, 85}, // &

    {TOKEN_ASTERISK, 80}, // *
    {TOKEN_FWSLASH, 80},  // /
    {TOKEN_PERCENT, 80},  // %

    {TOKEN_MINUS, 70}, // -
    {TOKEN_PLUS, 70},  // +

    {TOKEN_LT_LT, 65},    // <<
    {TOKEN_GT_GT, 65},    // >>

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
    {TOKEN_COLON, 15},         // :

    {TOKEN_EQ, 10},          // =
    {TOKEN_PLUS_EQ, 10},     // +=
    {TOKEN_MINUS_EQ, 10},    // -=
    {TOKEN_ASTERISK_EQ, 10}, // *=
    {TOKEN_FWSLASH_EQ, 10},  // /=
    {TOKEN_PERCENT_EQ, 10},  // %=
    {TOKEN_AND_EQ, 10},      // &=
    {TOKEN_HAT_EQ, 10},      // ^=
    {TOKEN_PIPE_EQ, 10},     // |=

    {TOKEN_LT_LT_EQ, 10},  // <<=
    {TOKEN_GT_GT_EQ, 10}, // >>=
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

typedef struct
{
    List* g_tokens;
    List* ast_list_ptr;
    s64 token_index;
    Token curr_tok;
    Token top_tok;

    Expr* active_func;
    char* ocontinue;
    char* lcontinue;
    char* obreak;
    char* lbreak;
    char* l_end;
} Parse_Context;

bool tok_is(Parse_Context* pctx, Token_Kind kind);
int get_tok_precedence(Parse_Context* pctx);
void add_new_symbol(Parse_Context* pctx);
void eat(Parse_Context* pctx);
void eat_kind(Parse_Context* pctx, Token_Kind kind);
Expr* get_definition(Parse_Context* pctx, char* ident);
Expr* get_variable_declaration(Parse_Context* pctx, char* ident);
Expr* get_function_call(Parse_Context* pctx, char* ident);

Expr* parse_top_level(Parse_Context* pctx);
Expr* parse_statement(Parse_Context* pctx);
Expr* parse_primary(Parse_Context* pctx);
Expr* parse_identifier(Parse_Context* pctx);
Expr* parse_subscript(Parse_Context* pctx);
Expr* parse_block(Parse_Context* pctx);
Expr* parse_return(Parse_Context* pctx);
Expr* parse_note(Parse_Context* pctx);
Expr* parse_expression(Parse_Context* pctx);
Expr* parse_unary(Parse_Context* pctx);
Expr* parse_binary(Parse_Context* pctx, int expr_prec, Expr* lhs);
Expr* parse_integer(Parse_Context* pctx);
Expr* parse_parens(Parse_Context* pctx);
Expr* parse_defer(Parse_Context* pctx);
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

s64 get_integer(Parse_Context* pctx);
f64 get_float(Parse_Context* pctx);
Typespec* get_type(Parse_Context* pctx);

#define DEBUG_START info("%s: %s", __func__, pctx->curr_tok.value);

#define DEBUG_STATEMENT_START                                                                                          \
    info("%s: %s", __func__, pctx->curr_tok.value);                                                                    \
    debug_info_color = get_next_color();

#define DEBUG_STATEMENT_END debug_info_color = get_previous_color();

#define SET_ACTIVE_FUNC(func) pctx->active_func = func
#define GET_ACTIVE_FUNC pctx->active_func

#define SET_JUMP_LABELS(cont, brk)\
    pctx->ocontinue = pctx->lcontinue;\
    pctx->obreak = pctx->lbreak; \
    pctx->lcontinue = cont;\
    pctx->lbreak = brk;

#define RESTORE_JUMP_LABELS \
    pctx->lcontinue = pctx->ocontinue;\
    pctx->lbreak = pctx->obreak;


//------------------------------------------------------------------------------
//                               Public Functions
//------------------------------------------------------------------------------

void give_type_to_all_nodes(Expr* expr)
{
    switch(expr->kind)
    {
    case EXPR_MACRO:  give_type_to_all_nodes(expr->Macro.expr); break;
    case EXPR_NOTE:  give_type_to_all_nodes(expr->Note.expr); break;
    case EXPR_UNARY:  give_type_to_all_nodes(expr->Unary.operand); break;
    case EXPR_BINARY:  give_type_to_all_nodes(expr->Binary.lhs); give_type_to_all_nodes(expr->Binary.rhs); break;
    case EXPR_GROUPING:  give_type_to_all_nodes(expr->Grouping.expr); break;
    case EXPR_BLOCK: {
        List* stmts = expr->Block.stmts;
        LIST_FOREACH(stmts) {
            Expr* stmt = (Expr*)it->data;
            give_type_to_all_nodes(stmt);
        }
    } break;
    }

    expr->type = get_inferred_type_of_expr(expr);
    if (expr->type) 
        warning("%s | %s", expr_to_str(expr), typespec_to_str(expr->type));
}

void parse(List* ast, char* source_file)
{
    char* last_file = get_source_file();
    char* last_dir = get_current_dir();

    list_append(&file_list, source_file);

    push_timer(source_file);
    List* tokens = generate_tokens_from_source(source_file);
    generate_symbol_table_from_tokens(ast, tokens);
    generate_ast_from_tokens(ast, tokens);

    // Give each node a type
    LIST_FOREACH(ast) { 
        Expr* expr = (Expr*)it->data; 
        give_type_to_all_nodes(expr);
    }
    

    pop_timer();

    // print_tokens(tokens);
    print_ast(ast);

    set_source_file(last_file);
    set_current_dir(last_dir);

    debug_info_color = RGB_GRAY;
}

void generate_ast_from_tokens(List* ast, List* tokens)
{
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
            list_append(ast, stmt);
        }
    }
}

void generate_symbol_table_from_tokens(List* ast, List* tokens)
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
            string file = make_string(strf("%s%s", get_current_dir(), pctx.curr_tok.value));
            bool file_already_parsed = false;
            LIST_FOREACH(get_file_list())
            {
                char* f = (char*)it->data;
                if (strcmp(f, file.c_str) == 0) {
                    file_already_parsed = true;
                }
            }
            if (!file_already_parsed) parse(ast, file.c_str);
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

        default: eat(&pctx);
        }
    }
}

//------------------------------------------------------------------------------
//                               Private Functions
//------------------------------------------------------------------------------

static int label_counter = 0;
void reset_label_counter()
{
    label_counter = 0;
}
char* make_label()
{
    return strf(".L%d", label_counter++);
}

Expr* parse_top_level(Parse_Context* pctx)
{
    DEBUG_START;

    Expr* expr = NULL;

    pctx->top_tok = pctx->curr_tok;
    switch (pctx->curr_tok.kind) {
    case TOKEN_FOREIGN: {
        eat_kind(pctx, TOKEN_FOREIGN);
        eat_kind(pctx, TOKEN_IDENTIFIER);
        eat_kind(pctx, TOKEN_COLON_COLON);
        skip_function_signature(pctx);
    } break;
    case TOKEN_IDENTIFIER: expr = parse_statement(pctx); break;
    default: warning("unknown toplevel token '%s'", pctx->curr_tok.value); eat(pctx);
    }

    return expr;
}

Expr* parse_statement(Parse_Context* pctx)
{
    DEBUG_STATEMENT_START;
    Expr* statement = NULL;
    switch (pctx->curr_tok.kind) {
    case TOKEN_IDENTIFIER: statement = parse_expression(pctx); break;
    case TOKEN_RETURN: statement = parse_return(pctx); break;
    case TOKEN_OPEN_PAREN: statement = parse_parens(pctx); break;
    case TOKEN_OPEN_BRACE: statement = parse_block(pctx); break;
    case TOKEN_BREAK: statement = parse_break(pctx); break;
    case TOKEN_CONTINUE: statement = parse_continue(pctx); break;
    case TOKEN_IF: statement = parse_if(pctx); break;
    case TOKEN_DEFER: statement = parse_defer(pctx); break;
    case TOKEN_FOR: statement = parse_for(pctx); break;
    case TOKEN_WHILE: statement = parse_while(pctx); break;
    default:
        statement =
            parse_expression(pctx); // error("Unhandled token '%s' was not a valid statement", pctx->curr_tok.value);
    }
    DEBUG_STATEMENT_END;
    return statement;
}

Expr* parse_primary(Parse_Context* pctx)
{
    DEBUG_START;
    switch (pctx->curr_tok.kind) {
    case TOKEN_IDENTIFIER: return parse_identifier(pctx);
    case TOKEN_DOLLAR_SIGN: return parse_note(pctx);
    case TOKEN_CHAR: // fallthrough
    case TOKEN_HEX: // fallthrough
    case TOKEN_INTEGER: return parse_integer(pctx);
    case TOKEN_STRING: return parse_string(pctx);
    case TOKEN_OPEN_PAREN: return parse_parens(pctx);
    case TOKEN_OPEN_BRACE: return parse_block(pctx);
    default: error("unhandled primary '%s'", pctx->curr_tok.value);
    }
    return NULL;
}

Expr* parse_identifier(Parse_Context* pctx)
{
    DEBUG_START;
    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    switch (pctx->curr_tok.kind) {
    case TOKEN_COLON_COLON: return get_definition(pctx, ident);
    case TOKEN_COLON: // fallthrough
    case TOKEN_COLON_EQ: return get_variable_declaration(pctx, ident);
    case TOKEN_OPEN_PAREN: return get_function_call(pctx, ident);
    }

    return make_expr_ident(ident);
}

Expr* parse_continue(Parse_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_CONTINUE);
    return make_expr_asm(strf("JMP %s", pctx->lcontinue));
}

Expr* parse_break(Parse_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_BREAK);
    return make_expr_asm(strf("JMP %s", pctx->lbreak));
}
Expr* parse_while(Parse_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_WHILE);

    List* stmts = make_list();

    char* begin = make_label();
    char* end = make_label();

    Expr* cond = parse_expression(pctx);

    SET_JUMP_LABELS(begin, end);
    Expr* body = parse_block(pctx);
    RESTORE_JUMP_LABELS;

    list_append(stmts, make_expr_asm(strf("%s:", begin)));
    list_append(stmts, cond);
    list_append(stmts, make_expr_asm(strf("JE %s", end)));
    list_append(stmts, body);
    list_append(stmts, make_expr_asm(strf("JMP %s", begin)));
    list_append(stmts, make_expr_asm(strf("%s:", end)));

    return make_expr_block(stmts);
}

Expr* parse_for(Parse_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_FOR);

    char* begin = make_label();
    char* mid = make_label();
    char* end = make_label();

    char* iterator_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    eat_kind(pctx, TOKEN_COLON);
    Expr* start_expr = parse_expression(pctx);

    Typespec* type = get_inferred_type_of_expr(start_expr);
    Expr* variable = make_expr_variable_decl(iterator_name, type, start_expr);
    add_symbol(iterator_name, type);
    
    eat_kind(pctx, TOKEN_DOT_DOT);
    Expr* end_expr = parse_expression(pctx);

    SET_JUMP_LABELS(mid, end);
    Expr* body = parse_block(pctx);
    RESTORE_JUMP_LABELS;

    Expr* iterator = make_expr_ident(iterator_name);
    Expr* cond = make_expr_binary(TOKEN_LT_EQ,  iterator, end_expr);
    Expr* step = make_expr_binary(TOKEN_PLUS_EQ, iterator, make_expr_int(1));

    List* stmts = make_list();
    list_append(stmts, variable);
    list_append(stmts, make_expr_asm(strf("%s:", begin)));
    list_append(stmts, cond);
    list_append(stmts, iterator);
    list_append(stmts, make_expr_asm(strf("JGE %s", end)));
    list_append(stmts, body);
    list_append(stmts, make_expr_asm(strf("%s:", mid)));
    list_append(stmts, step);
    list_append(stmts, make_expr_asm(strf("JMP %s", begin)));
    list_append(stmts, make_expr_asm(strf("%s:", end)));

    return make_expr_block(stmts);
}

Expr* parse_defer(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_DEFER);
    Expr* block = parse_block(pctx);
    List* defers = GET_ACTIVE_FUNC->Function.defers;
    list_append(defers, block);
    return NULL;
}

Expr* parse_if(Parse_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_IF);

    List* stmts = make_list();

    char* else_l = make_label();
    char* end_l = make_label();

    Expr* cond = parse_expression(pctx);
    Expr* body = parse_block(pctx);
    Expr* else_b = NULL;
    if (tok_is(pctx, TOKEN_ELSE)) {
        eat_kind(pctx, TOKEN_ELSE);
        else_b = parse_block(pctx);
    }

    list_append(stmts, cond);
    list_append(stmts, make_expr_asm(strf("JNE %s", else_b ? else_l : end_l)));
    list_append(stmts, body);
    list_append(stmts, make_expr_asm(strf("JMP %s", end_l)));
    list_append(stmts, make_expr_asm(strf("%s:", else_l)));
    if (else_b) list_append(stmts, else_b);
    list_append(stmts, make_expr_asm(strf("%s:", end_l)));

    return make_expr_block(stmts);
}

Expr* parse_string(Parse_Context* pctx)
{
    DEBUG_START;
    char* value = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_STRING);
    return make_expr_string(value);
}

Expr* parse_block(Parse_Context* pctx)
{
    DEBUG_START;

    List* stmts = make_list();

    eat_kind(pctx, TOKEN_OPEN_BRACE);
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE)) {
        Expr* stmt = parse_statement(pctx);
        if (stmt) { 
            list_append(stmts, stmt); 
        }
    }

    eat_kind(pctx, TOKEN_CLOSE_BRACE);
    return make_expr_block(stmts);
}

Expr* parse_return(Parse_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_RETURN);

    List* stmts = make_list();
    list_append(stmts, parse_expression(pctx));
    list_append(stmts, make_expr_asm("JMP .END"));

    return make_expr_block(stmts);
}

Expr* get_function_call(Parse_Context* pctx, char* ident)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_OPEN_PAREN);

    List* args = make_list();

    bool has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);
        Expr* arg = parse_expression(pctx);
        if (arg)
            list_append(args, arg);
        else
            error("Invalid expression in function call %s", ident);
        has_multiple_arguments = true;
    }
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    return make_expr_call(ident, args);
}

Expr* get_variable_declaration(Parse_Context* pctx, char* ident)
{
    DEBUG_START;

    char*       variable_name = ident;
    Typespec*   variable_type = NULL;
    Expr*       variable_value = NULL;

    switch(pctx->curr_tok.kind) {
    case TOKEN_COLON: 
    {   
        eat_kind(pctx, TOKEN_COLON);
        variable_type = get_type(pctx);
        if (tok_is(pctx, TOKEN_EQ)) {
            eat_kind(pctx, TOKEN_EQ);
            variable_value = parse_expression(pctx);
        }
    } break;
    case TOKEN_COLON_EQ:
    {
        // We need to infer the type based on the assignment expr
        eat_kind(pctx, TOKEN_COLON_EQ);
        variable_value = parse_expression(pctx);
        variable_type = get_inferred_type_of_expr(variable_value);
    } break;
    }

    add_symbol(ident, variable_type);

    return make_expr_variable_decl(variable_name, variable_type, variable_value);
}

Expr* parse_binary(Parse_Context* pctx, int expr_prec, Expr* lhs)
{
    DEBUG_START;

    Expr* expr = NULL;

    // If this is a binop, find its precedence.
    while (1) {
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

        // Parser the unary expression after the binary operator.
        Expr* rhs = parse_unary(pctx);
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
        lhs = make_expr_binary(binary_op_token, lhs, rhs);
    }

    return expr;
}

Expr* read_subscript_expr(Parse_Context* pctx, Expr* expr)
{
    eat_kind(pctx, TOKEN_OPEN_BRACKET);
    Expr* sub = parse_expression(pctx);
    if (!sub) error("subscription expected");
    eat_kind(pctx, TOKEN_CLOSE_BRACKET);
    s64 size = get_size_of_underlying_typespec(get_inferred_type_of_expr(expr));
    sub = make_expr_binary(TOKEN_ASTERISK, make_expr_int(size), sub);
    Expr* t = make_expr_binary(TOKEN_PLUS, expr, sub);
    return make_expr_unary(THI_SYNTAX_POINTER, t);
}

Expr* parse_postfix_tail(Parse_Context* pctx, Expr* primary_expr)
{
    DEBUG_START;
    if (!primary_expr) return NULL;
    while (1) {
        if (tok_is(pctx, TOKEN_OPEN_BRACKET)) {
            primary_expr = read_subscript_expr(pctx, primary_expr);
            continue;
        }
        return primary_expr;
    }
}

Expr* parse_postfix(Parse_Context* pctx)
{
    DEBUG_START;
    Expr* primary_expr = parse_primary(pctx);
    return parse_postfix_tail(pctx, primary_expr);
}

Expr* parse_unary(Parse_Context* pctx)
{
    DEBUG_START;
    Expr* unary_expr = NULL;
    if (tok_is(pctx, TOKEN_BANG) || tok_is(pctx, THI_SYNTAX_POINTER) || tok_is(pctx, TOKEN_MINUS) ||
        tok_is(pctx, TOKEN_PLUS) || tok_is(pctx, THI_SYNTAX_ADDRESS) || tok_is(pctx, TOKEN_TILDE)) {
        Token_Kind op = pctx->curr_tok.kind;
        eat(pctx);

        Expr* operand = parse_unary(pctx);
        if (operand) {
            unary_expr = make_expr_unary(op, operand);
        }
    }

    // If the current token is not an operator, it must be a primary expression.
    return unary_expr ? unary_expr : parse_postfix(pctx);
}

Expr* parse_note(Parse_Context* pctx)
{
    DEBUG_START;
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
    DEBUG_START;
    Expr* lhs = parse_unary(pctx);
    Expr* expr = NULL;
    if (lhs) {
        expr = parse_binary(pctx, 0, lhs);
    }
    return expr;
}

Expr* parse_integer(Parse_Context* pctx)
{
    DEBUG_START;

    Expr* res = make_expr_int(get_integer(pctx));

    return res;
}

Expr* parse_parens(Parse_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Expr* expr = parse_expression(pctx);
    eat_kind(pctx, TOKEN_CLOSE_PAREN);

    return make_expr_grouping(expr);
}
//------------------------------------------------------------------------------
//                               Type Utilty Functions
//------------------------------------------------------------------------------

s64 get_integer(Parse_Context* pctx)
{
    DEBUG_START;

    s64 value = 0;
    switch (pctx->curr_tok.kind) {
    case TOKEN_CHAR: {
        char c = pctx->curr_tok.value[0];
        if (c == '\\')
        {
            char c = pctx->curr_tok.value[1];
            switch (c)
            {
                case 'a':  value = 7; break;
                case 'n':  value = 10; break;
                case 't':  value = 9; break;
                case '\\': value = 92; break;
                case '\'': value =  27; break;
                case '"':  value = 22; break;
            }
        } else value = c; break;
    }
    case TOKEN_INTEGER: value = atoll(pctx->curr_tok.value); break;
    case TOKEN_HEX: value = strtoll(pctx->curr_tok.value, NULL, 0); break;
    default: error("not an integer.");
    }
    eat(pctx);

    return value;
}

f64 get_float(Parse_Context* pctx)
{
    DEBUG_START;

    f64 value = atof(pctx->curr_tok.value);
    eat_kind(pctx, TOKEN_FLOAT);

    return value;
}

void skip_type(Parse_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_IDENTIFIER);

    switch (pctx->curr_tok.kind) {
    case THI_SYNTAX_POINTER: {
        while (tok_is(pctx, THI_SYNTAX_POINTER)) {
            eat_kind(pctx, THI_SYNTAX_POINTER);
        }
    } break;
    case TOKEN_OPEN_BRACKET: {
        eat_kind(pctx, TOKEN_OPEN_BRACKET);
        if (tok_is(pctx, TOKEN_INTEGER) || 
            tok_is(pctx, TOKEN_HEX) ||
            tok_is(pctx, TOKEN_CHAR)) {
            get_integer(pctx);
        }
        eat_kind(pctx, TOKEN_CLOSE_BRACKET);
    } break;
    }
}

// Returns NULL if the current token is not a type.
Typespec* get_type(Parse_Context* pctx)
{
    DEBUG_START;

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
        s64 size = 0;
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
    DEBUG_START;

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
    DEBUG_START;

    eat_kind(pctx, TOKEN_STRUCT);
    eat_kind(pctx, TOKEN_OPEN_BRACE);

    List* members = make_list();

    while (!tok_is(pctx, TOKEN_CLOSE_BRACE)) {
        Arg* member = xmalloc(sizeof(Arg));
        member->name = pctx->curr_tok.value;
        eat_kind(pctx, TOKEN_IDENTIFIER);
        eat_kind(pctx, TOKEN_COLON);
        member->type = get_type(pctx);
        list_append(members, member);
    }
    eat_kind(pctx, TOKEN_CLOSE_BRACE);

    return make_typespec_struct(struct_name, members);
}

Typespec* parse_function_signature(Parse_Context* pctx, char* func_name)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_OPEN_PAREN);

    List* args = make_list();

    bool has_multiple_arguments = false;
    while (!tok_is(pctx, TOKEN_CLOSE_PAREN)) {
        if (has_multiple_arguments) eat_kind(pctx, TOKEN_COMMA);

        Arg* arg = xmalloc(sizeof(Arg));
        arg->name = pctx->curr_tok.value;
        arg->type = NULL;

        // foreign's dont have named parameters
        if (pctx->top_tok.kind == TOKEN_FOREIGN) {
            arg->name = NULL;
            arg->type = get_type(pctx);
        } else {
            eat_kind(pctx, TOKEN_IDENTIFIER);
            eat_kind(pctx, TOKEN_COLON);
            arg->type = get_type(pctx);
        }

        has_multiple_arguments = true;
        list_append(args, arg);
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
    DEBUG_START;

    eat_kind(pctx, TOKEN_COLON_COLON);
    switch (pctx->curr_tok.kind) {
    case TOKEN_ENUM: {
        skip_enum_signature(pctx);
        // return make_expr_enum(get_symbol(ident));
        // return NULL;
        break;
    }
    case TOKEN_STRUCT: {
        skip_struct_signature(pctx);
        return make_expr_struct(get_symbol(ident));
    }
    case TOKEN_OPEN_PAREN: {
        skip_function_signature(pctx);
        Expr* func = make_expr_function(get_symbol(ident), NULL);
        SET_ACTIVE_FUNC(func);
        Expr* body = parse_block(pctx);
        func->Function.body = body;
        reset_label_counter();
        return func;
    }
    default: {
        // Macro def
        parse_expression(pctx);
        // return NULL;
        // return make_expr_macro(ident, parse_expression(pctx));
        break;
    }
    }

    return NULL;
}

int get_tok_precedence(Parse_Context* pctx)
{
    DEBUG_START;
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

void eat(Parse_Context* pctx) { pctx->curr_tok = *(Token*)list_at(pctx->g_tokens, pctx->token_index++); }

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
    DEBUG_START;
    if (tok_is(pctx, TOKEN_OPEN_BRACE)) {
        skip_block(pctx);
        return;
    }
    parse_statement(pctx);
}

void skip_block(Parse_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    s64 counter = 1;
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
    DEBUG_START;
    if (tok_is(pctx, TOKEN_IDENTIFIER)) skip_type(pctx);
    eat_kind(pctx, TOKEN_OPEN_BRACE);
    while (!tok_is(pctx, TOKEN_CLOSE_BRACE))
        eat_kind(pctx, TOKEN_IDENTIFIER);
    eat(pctx);
}

void skip_struct_signature(Parse_Context* pctx)
{
    DEBUG_START;
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
    DEBUG_START;
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
    DEBUG_START;
    /*
        Can this please also look for symbols inside toplevel functions??
        Kinda want to define macros and structs inside other constructs
        you know.
    */

    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    if (tok_is(pctx, TOKEN_COLON_COLON)) {
        eat_kind(pctx, TOKEN_COLON_COLON);
        switch (pctx->curr_tok.kind) {
        case TOKEN_ENUM: {
            Typespec* type = parse_enum_signature(pctx, ident);
            add_symbol(ident, type);
            break;
        }
        case TOKEN_STRUCT: {
            Typespec* type = parse_struct_signature(pctx, ident);
            add_symbol(ident, type);
            break;
        }
        case TOKEN_OPEN_PAREN: {
            Typespec* type = parse_function_signature(pctx, ident);
            skip_statement_body(pctx);
            add_symbol(ident, type);
            break;
        }
        default: {
            Expr* expr = parse_expression(pctx);
            add_macro_def(ident, expr);
            break;
        }
        }
    }
}
