#include    "parser.h"

#include    "typedefs.h"    // s32, s64, etc.
#include    "lexer.h"       // Token, Token_Kind, Lexify, Lex
#include    "typespec.h"    // Typespec, make_typspec_*, 
#include    "utility.h"     // info, error, warning, success, strf, get_file_content
#include    <assert.h>      // assert
#include    <ctype.h>       // atoll
#include    <stdlib.h>      // xmalloc
#include    <string.h>      // strcmp
#include    "globals.h"     // add_symbol

#define BIN_OP_COUNT 51
struct {
    Token_Kind kind;
    s32 p;
} binop_precedence[BIN_OP_COUNT] = {

    {TOKEN_PLUS_PLUS,           100},   // ++
    {TOKEN_MINUS_MINUS,         100},   // --
    {TOKEN_OPEN_BRACKET,        100},   // []
    {TOKEN_DOT,                 100},   // .
    {TOKEN_OPEN_PAREN,          100},   // ()
    {TOKEN_RIGHT_ARROW,         100},   // ->
    {TOKEN_BANG,                85},    // !
    {TOKEN_TILDE,               85},    // ~
    {THI_SYNTAX_POINTER,        85},    // @
    {THI_SYNTAX_ADDRESS,        85},    // &
    {TOKEN_ASTERISK,            80},    // *
    {TOKEN_FWSLASH,             80},    // /
    {TOKEN_PERCENT,             80},    // %
    {TOKEN_MINUS,               70},    // -
    {TOKEN_PLUS,                70},    // +
    {TOKEN_LT_LT,               65},    // <<
    {TOKEN_GT_GT,               65},    // >>
    {TOKEN_LT,                  60},    // <
    {TOKEN_GT,                  60},    // >
    {TOKEN_GT_EQ,               60},    // >=
    {TOKEN_LT_EQ,               60},    // <=
    {TOKEN_EQ_EQ,               50},    // ==
    {TOKEN_BANG_EQ,             50},    // !=
    {TOKEN_AND_AND,             48},    // &&
    {TOKEN_PIPE_PIPE,           47},    // ||
    {TOKEN_AT,                  40},    // @
    {TOKEN_AND,                 40},    // &
    {TOKEN_HAT,                 30},    // ^
    {TOKEN_PIPE,                20},    // |
    {TOKEN_QUESTION_MARK,       15},    // ?
    {TOKEN_COLON,               15},    // :
    {TOKEN_EQ,                  10},    // =
    {TOKEN_PLUS_EQ,             10},    // +=
    {TOKEN_MINUS_EQ,            10},    // -=
    {TOKEN_ASTERISK_EQ,         10},    // *=
    {TOKEN_FWSLASH_EQ,          10},    // /=
    {TOKEN_PERCENT_EQ,          10},    // %=
    {TOKEN_AND_EQ,              10},    // &=
    {TOKEN_HAT_EQ,              10},    // ^=
    {TOKEN_PIPE_EQ,             10},    // |=
    {TOKEN_LT_LT_EQ,            10},    // <<=
    {TOKEN_GT_GT_EQ,            10},    // >>=
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

typedef struct
{
    Token_List token_list;
    List* ast;
    u64 token_index;
    Token curr_tok;
    Token top_tok;
    char* source_file;
    char* source;

    u64 label_counter;

    Expr* active_func;
    char* ocontinue;
    char* lcontinue;
    char* obreak;
    char* lbreak;
    char* l_end;
} Parse_Context;

//------------------------------------------------------------------------------
//              Each construct of the language gets its own function
//------------------------------------------------------------------------------

void                recursively_fill_ast           (List* ast, Parse_Context* pctx);

Expr*               parse_top_level                (Parse_Context* pctx);
Expr*               parse_statement                (Parse_Context* pctx);
Expr*               parse_primary                  (Parse_Context* pctx);
Expr*               parse_identifier               (Parse_Context* pctx);
Expr*               parse_variable_decl            (Parse_Context* pctx);
Expr*               parse_function_call            (Parse_Context* pctx);
Expr*               parse_subscript                (Parse_Context* pctx);
Expr*               parse_block                    (Parse_Context* pctx);
Expr*               parse_return                   (Parse_Context* pctx);
Expr*               parse_note                     (Parse_Context* pctx);
Expr*               parse_expression               (Parse_Context* pctx);
Expr*               parse_unary                    (Parse_Context* pctx);
Expr*               parse_binary                   (Parse_Context* pctx, int expr_prec, Expr* lhs);
Expr*               parse_integer                  (Parse_Context* pctx);
Expr*               parse_parens                   (Parse_Context* pctx);
Expr*               parse_defer                    (Parse_Context* pctx);
Expr*               parse_load                     (Parse_Context* pctx);
Expr*               parse_extern                   (Parse_Context* pctx);
Expr*               parse_link                     (Parse_Context* pctx);
Expr*               parse_if                       (Parse_Context* pctx);
Expr*               parse_for                      (Parse_Context* pctx);
Expr*               parse_while                    (Parse_Context* pctx);
Expr*               parse_break                    (Parse_Context* pctx);
Expr*               parse_continue                 (Parse_Context* pctx);
Expr*               parse_string                   (Parse_Context* pctx);
Expr*               parse_sizeof                   (Parse_Context* pctx);
Expr*               parse_def                      (Parse_Context* pctx);
Expr*               parse_macro                    (Parse_Context* pctx);
Expr*               parse_type                     (Parse_Context* pctx);

//------------------------------------------------------------------------------
//                               Helpers
//------------------------------------------------------------------------------

Typespec*           parse_type_signature           (Parse_Context* pctx, char* struct_name);
Typespec*           parse_function_signature       (Parse_Context* pctx, char* func_name);

s64                 get_integer                    (Parse_Context* pctx);
f64                 get_float                      (Parse_Context* pctx);
Typespec*           get_type                       (Parse_Context* pctx);

bool                tok_is                         (Parse_Context* pctx, Token_Kind kind);
int                 get_tok_precedence             (Parse_Context* pctx);
void                eat                            (Parse_Context* pctx);
void                eat_kind                       (Parse_Context* pctx, Token_Kind kind);
Token_Kind          next_tok_kind                  (Parse_Context* pctx);

void                reset_label_counter            (Parse_Context* pctx);
char*               make_label                     (Parse_Context* pctx);


#define DEBUG_START info("%s: %s", __func__, pctx->curr_tok.value);

#define SET_ACTIVE_FUNC(func) pctx->active_func = func
#define GET_ACTIVE_FUNC pctx->active_func

#define SET_JUMP_LABELS(cont, brk)                                                                                     \
    pctx->ocontinue = pctx->lcontinue;                                                                                 \
    pctx->obreak = pctx->lbreak;                                                                                       \
    pctx->lcontinue = cont;                                                                                            \
    pctx->lbreak = brk;

#define RESTORE_JUMP_LABELS                                                                                            \
    pctx->lcontinue = pctx->ocontinue;                                                                                 \
    pctx->lbreak = pctx->obreak;

//------------------------------------------------------------------------------
//                               Public 
//------------------------------------------------------------------------------

void give_type_to_all_nodes(Expr* expr)
{
    switch (expr->kind) {
    case EXPR_MACRO:    give_type_to_all_nodes(expr->Macro.expr);       break;
    case EXPR_NOTE:     give_type_to_all_nodes(expr->Note.expr);        break;
    case EXPR_UNARY:    give_type_to_all_nodes(expr->Unary.operand);    break;
    case EXPR_BINARY: {
        give_type_to_all_nodes(expr->Binary.lhs);
        give_type_to_all_nodes(expr->Binary.rhs);
    } break;
    case EXPR_GROUPING: give_type_to_all_nodes(expr->Grouping.expr);    break;
    case EXPR_BLOCK: {
        List* stmts = expr->Block.stmts;
        LIST_FOREACH(stmts) {
            Expr* stmt = (Expr*)it->data;
            give_type_to_all_nodes(stmt);
        }
    } break;
    }

    expr->type = get_inferred_type_of_expr(expr);
    if (expr->type) info("%s | %s", expr_to_str(expr), typespec_to_str(expr->type));
}

void parse(List* ast, char* source_file)
{
    // We need to set some state
    char* last_file = get_source_file();
    char* last_dir  = get_current_dir();
    char* dir       = get_file_directory(source_file);
    
    set_source_file(source_file);
    set_current_dir(dir);

    list_append(file_list, source_file);

    push_timer(source_file);

    char*  source =  get_file_content(source_file);
    Lex    lex    =  lexify(source);

    print_tokens(lex.token_list);

    Parse_Context pctx;
    pctx.token_list = lex.token_list;
    pctx.token_index = 0;
    pctx.top_tok.kind = TOKEN_UNKNOWN;
    pctx.curr_tok.kind = TOKEN_UNKNOWN;
    pctx.ast = ast;
    pctx.source_file = source_file;
    pctx.source = source;
    pctx.label_counter = 0;

    recursively_fill_ast(ast, &pctx);

    print_symbol_map();
    print_ast(ast);

    pop_timer();

    set_source_file(last_file);
    set_current_dir(last_dir);

    debug_info_color = RGB_GRAY;
}

//------------------------------------------------------------------------------
//                               Private
//------------------------------------------------------------------------------

void recursively_fill_ast(List* ast, Parse_Context* pctx)
{
    info("Generating ast from tokens..");
    eat(pctx);
    while (!tok_is(pctx, TOKEN_EOF)) {
        Expr* stmt = parse_top_level(pctx);
        if (stmt) {
            list_append(ast, stmt);
        }
    }
}

Expr* parse_top_level(Parse_Context* pctx)
{
    DEBUG_START;
    pctx->top_tok = pctx->curr_tok;
    switch (pctx->curr_tok.kind) {
        case TOKEN_IDENTIFIER:              return parse_macro(pctx);
        case TOKEN_TYPE:                    return parse_type(pctx);
        case TOKEN_DEF:                     return parse_def(pctx);
        case TOKEN_EXTERN:                  return parse_extern(pctx);
        case TOKEN_LOAD:                    return parse_load(pctx);
        case TOKEN_LINK:                    return parse_link(pctx);
        default: eat(pctx);                 break;
    }
    return NULL;
}

Expr* parse_statement(Parse_Context* pctx)
{
    DEBUG_START;
    switch (pctx->curr_tok.kind) {
        case TOKEN_DEF:                    return parse_def(pctx);
        case TOKEN_TYPE:                   return parse_type(pctx);
        case THI_SYNTAX_POINTER:           return parse_unary(pctx);
        case TOKEN_IDENTIFIER:             return parse_expression(pctx);
        case TOKEN_RET:                    return parse_return(pctx);
        case TOKEN_BREAK:                  return parse_break(pctx);
        case TOKEN_CONTINUE:               return parse_continue(pctx);
        case TOKEN_IF:                     return parse_if(pctx);
        case TOKEN_DEFER:                  return parse_defer(pctx);
        case TOKEN_FOR:                    return parse_for(pctx);
        case TOKEN_WHILE:                  return parse_while(pctx);
        case TOKEN_EXTERN:                 return parse_extern(pctx);
        case TOKEN_LOAD:                   return parse_load(pctx);
        case TOKEN_LINK:                   return parse_link(pctx);
        default:
            warning("Unhandled token '%s' was not a valid statement", pctx->curr_tok.value);
            eat(pctx);
            break;
    }
    return NULL;
}

Expr* parse_primary(Parse_Context* pctx)
{
    DEBUG_START;
    switch (pctx->curr_tok.kind) {
        case TOKEN_TRUE: eat(pctx);            return make_expr_int(1);
        case TOKEN_FALSE: eat(pctx);           return make_expr_int(0);
        case TOKEN_SIZEOF:                     return parse_sizeof(pctx);
        case TOKEN_IDENTIFIER:                 return parse_identifier(pctx);
        case TOKEN_DOLLAR_SIGN:                return parse_note(pctx);
        case TOKEN_CHAR: // FALLTHROUGH
        case TOKEN_HEX:  // FALLTHROUGH
        case TOKEN_INTEGER:                    return parse_integer(pctx);
        case TOKEN_STRING:                     return parse_string(pctx);
        case TOKEN_OPEN_PAREN:                 return parse_parens(pctx);
        default: error("unhandled primary '%s'", pctx->curr_tok.value);
    }
    return NULL;
}

Expr* parse_identifier(Parse_Context* pctx)
{
    DEBUG_START;
    switch (next_tok_kind(pctx)) {
        case TOKEN_COLON:                      // FALLTHROUGH
        case TOKEN_COLON_EQ:                   return parse_variable_decl(pctx);
        case TOKEN_OPEN_PAREN:                 return parse_function_call(pctx);
        case TOKEN_COLON_COLON:                return parse_macro(pctx);
    }
    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    return make_expr_ident(ident);
}

Expr* parse_load(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_LOAD);
    string file = make_string(strf("%s%s", get_current_dir(), pctx->curr_tok.value));
    bool file_already_parsed = false;
    LIST_FOREACH(get_file_list())
    {
        char* f = (char*)it->data;
        if (strcmp(f, file.c_str) == 0) {
            file_already_parsed = true;
        }
    }
    if (!file_already_parsed) parse(pctx->ast, file.c_str);
    eat_kind(pctx, TOKEN_STRING);
    return NULL;
}

Expr* parse_link(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_LINK);
    char* lib_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_STRING);
    add_link(lib_name);
    return NULL;
}

Expr* parse_extern(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_EXTERN);
    char* func_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    Typespec* func_t = parse_function_signature(pctx, func_name);
    add_foreign_function(func_name, func_t);
    add_symbol(func_name, func_t);
    return NULL;
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
    char* begin = make_label(pctx);
    char* end = make_label(pctx);
    Expr* cond = parse_expression(pctx);
    SET_JUMP_LABELS(begin, end);
    Expr* body = parse_block(pctx);
    RESTORE_JUMP_LABELS;
    list_append(stmts, make_expr_asm(strf("%s:", begin)));
    list_append(stmts, cond);
    list_append(stmts, make_expr_asm("CMP AL, 0"));
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

    char* begin = make_label(pctx);
    char* mid = make_label(pctx);
    char* end = make_label(pctx);

    Expr* init = parse_statement(pctx);
    eat_kind(pctx, TOKEN_COMMA);
    Expr* cond = parse_expression(pctx);
    eat_kind(pctx, TOKEN_COMMA);
    Expr* inc = parse_expression(pctx);

    SET_JUMP_LABELS(mid, end);
    Expr* body = parse_block(pctx);
    RESTORE_JUMP_LABELS;

    List* stmts = make_list();
    list_append(stmts, init);
    list_append(stmts, make_expr_asm(strf("%s:", begin)));
    list_append(stmts, cond);
    list_append(stmts, make_expr_asm("CMP AL, 0"));
    list_append(stmts, make_expr_asm(strf("JE %s", end)));
    list_append(stmts, body);
    list_append(stmts, make_expr_asm(strf("%s:", mid)));
    list_append(stmts, inc);
    list_append(stmts, make_expr_asm(strf("JMP %s", begin)));
    list_append(stmts, make_expr_asm(strf("%s:", end)));

    return make_expr_block(stmts);
}

Expr* parse_defer(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_DEFER);
    Expr* block = parse_block(pctx);
    List* defers = pctx->active_func->Function.defers;
    list_append(defers, block);
    return NULL;
}

Expr* parse_if(Parse_Context* pctx)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_IF);

    List* stmts = make_list();

    char* else_l = make_label(pctx);
    char* end_l = make_label(pctx);

    Expr* cond = parse_expression(pctx);
    Expr* body = parse_block(pctx);
    Expr* else_b = NULL;
    if (tok_is(pctx, TOKEN_ELSE)) {
        eat_kind(pctx, TOKEN_ELSE);
        else_b = parse_block(pctx);
    }

    list_append(stmts, cond);
    list_append(stmts, make_expr_asm(strf("CMP AL, 0")));
    list_append(stmts, make_expr_asm(strf("JE %s", else_b ? else_l : end_l)));
    list_append(stmts, body);
    list_append(stmts, make_expr_asm(strf("JMP %s", end_l)));
    list_append(stmts, make_expr_asm(strf("%s:", else_l)));
    if (else_b) list_append(stmts, else_b);
    list_append(stmts, make_expr_asm(strf("%s:", end_l)));

    return make_expr_block(stmts);
}

Expr* parse_sizeof(Parse_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_SIZEOF);
    eat_kind(pctx, TOKEN_OPEN_PAREN);
    Typespec* t = get_type(pctx);
    eat_kind(pctx, TOKEN_CLOSE_PAREN);
    s64 size = get_size_of_typespec(t);
    return make_expr_int(size);
}

Expr* parse_macro(Parse_Context* pctx)
{
    char* identifier = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    eat_kind(pctx, TOKEN_COLON_COLON);
    Expr* expr = parse_expression(pctx);
    add_macro_def(identifier, expr);
    return NULL;
}

Expr* parse_def(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_DEF);
    char* identifier = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    Typespec* type = parse_function_signature(pctx, identifier);
    Expr* func = make_expr_function(type, NULL);
    add_symbol(identifier, type);
    SET_ACTIVE_FUNC(func);
    Expr* body = parse_block(pctx);
    func->Function.body = body;
    reset_label_counter(pctx);
    list_append(pctx->ast, func);
    return NULL;
}

Expr* parse_type(Parse_Context* pctx)
{
    eat_kind(pctx, TOKEN_TYPE);
    char* identifier = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);
    Typespec* type = parse_type_signature(pctx, identifier);
    add_symbol(identifier, type);
    return NULL;
}

Expr* parse_string(Parse_Context* pctx)
{
    DEBUG_START;
    char* value = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_STRING);
    Expr* expr = make_expr_string(value);
    expr = make_expr_unary(THI_SYNTAX_POINTER, expr);
    return expr;
}

Expr* parse_block(Parse_Context* pctx)
{
    DEBUG_START;

    // Is it a single statement?
    if (tok_is(pctx, TOKEN_DO)) {
        eat_kind(pctx, TOKEN_DO);
        return parse_statement(pctx);
    }

    // else it's a block
    eat_kind(pctx, TOKEN_BLOCK_START);
    List* stmts = make_list();
    while (!tok_is(pctx, TOKEN_BLOCK_END)) {
        Expr* stmt = parse_statement(pctx);
        if (stmt) {
            list_append(stmts, stmt);
        }
    }

    eat_kind(pctx, TOKEN_BLOCK_END);
    return make_expr_block(stmts);
}

Expr* parse_return(Parse_Context* pctx)
{
    DEBUG_START;
    eat_kind(pctx, TOKEN_RET);

    bool returns_something = false;

    // If the current function does not return anything
    if (GET_ACTIVE_FUNC->Function.type->Function.ret_type) {
        returns_something = true;
    }

    List* stmts = make_list();
    char* label = make_label(pctx);
    char* label2 = make_label(pctx);

    // @SLOW: we're reverseing a list using a stack
    //        every time we return. We should just have the
    //        defers be a stack in the first place.
    //        Basically, DONT REVERSE THE LIST ON EVERY RETURN.

    list_append(stmts, make_expr_asm(strf("%s:", label2)));
    list_append_content_of_in_reverse(stmts, pctx->active_func->Function.defers);
    list_append(stmts, make_expr_asm(strf("JMP %s", label)));

    list_append(stmts, make_expr_asm(strf("%s:", label)));
    if (returns_something) list_append(stmts, parse_expression(pctx));
    list_append(stmts, make_expr_asm(strf("JMP %s", DEFAULT_FUNCTION_END_LABEL_NAME)));

    return make_expr_block(stmts);
}

Expr* parse_function_call(Parse_Context* pctx)
{
    DEBUG_START;

    char* ident = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);

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

Expr* parse_variable_decl(Parse_Context* pctx)
{
    DEBUG_START;

    char* variable_name = pctx->curr_tok.value;
    Typespec* variable_type = NULL;
    Expr* variable_value = NULL;

    eat_kind(pctx, TOKEN_IDENTIFIER);

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
            // We need to infer the type based on the assignment expr
            eat_kind(pctx, TOKEN_COLON_EQ);
            variable_value = parse_expression(pctx);
            variable_type = get_inferred_type_of_expr(variable_value);
        } break;
    }

    add_symbol(variable_name, variable_type);

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

Typespec* active_type = NULL;
Expr* read_field_access_expr(Parse_Context* pctx, Expr* expr)
{
    warning("expr: %s", expr_to_str(expr));

    eat_kind(pctx, TOKEN_DOT);
    char* field_name = pctx->curr_tok.value;
    eat_kind(pctx, TOKEN_IDENTIFIER);

    Typespec* type = get_inferred_type_of_expr(expr);
    active_type = type;
    s64 offset = get_offset_in_struct_to_field(type, field_name);

    expr = make_expr_unary(THI_SYNTAX_ADDRESS, expr);
    expr = make_expr_binary(TOKEN_PLUS, expr, make_expr_int(offset));

    return make_expr_unary(THI_SYNTAX_POINTER, expr);
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

    // return make_expr_subscript(expr)
    return make_expr_unary(THI_SYNTAX_POINTER, t);
}

Expr* parse_postfix_tail(Parse_Context* pctx, Expr* primary_expr)
{
    DEBUG_START;
    if (!primary_expr) return NULL;
    while (1) {
        if (tok_is(pctx, TOKEN_DOT)) {
            primary_expr = read_field_access_expr(pctx, primary_expr);
            continue;
        }
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
//                               Helpers
//------------------------------------------------------------------------------

Typespec* parse_enum_signature(Parse_Context* pctx, char* name)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_ENUM);
    eat_kind(pctx, TOKEN_BLOCK_START);
    while (!tok_is(pctx, TOKEN_BLOCK_END)) {
        info("  enum member: %s", pctx->curr_tok.value);
        eat_kind(pctx, TOKEN_IDENTIFIER);
    }
    eat_kind(pctx, TOKEN_BLOCK_END);

    return make_typespec_enum(name, NULL);
}

Typespec* parse_type_signature(Parse_Context* pctx, char* struct_name)
{
    DEBUG_START;

    eat_kind(pctx, TOKEN_BLOCK_START);

    List* members = make_list();

    while (!tok_is(pctx, TOKEN_BLOCK_END)) {
        Arg* member = xmalloc(sizeof(Arg));
        member->name = pctx->curr_tok.value;
        eat_kind(pctx, TOKEN_IDENTIFIER);
        eat_kind(pctx, TOKEN_COLON);
        member->type = get_type(pctx);
        list_append(members, member);
    }
    eat_kind(pctx, TOKEN_BLOCK_END);

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
        if (pctx->top_tok.kind == TOKEN_EXTERN) {
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

    // If the type is not yet known, make a stub for it
    if (!type) {
        type = make_typespec_struct(type_name, NULL);
    }

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

    return type;
}


s64 get_integer(Parse_Context* pctx)
{
    DEBUG_START;

    s64 value = 0;
    switch (pctx->curr_tok.kind) {
    case TOKEN_CHAR: {
        char c = pctx->curr_tok.value[0];
        if (c == '\\') {
            char c = pctx->curr_tok.value[1];
            switch (c) {
                case 'a':   value = 7;  break;
                case 'n':   value = 10; break;
                case 't':   value = 9;  break;
                case '\\':  value = 92; break;
                case '\'':  value = 27; break;
                case '"':   value = 22; break;
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

f64 get_float(Parse_Context* pctx)
{
    DEBUG_START;

    f64 value = atof(pctx->curr_tok.value);
    eat_kind(pctx, TOKEN_FLOAT);

    return value;
}

int get_tok_precedence(Parse_Context* pctx)
{
    DEBUG_START;
    for (int i = 0; i < BIN_OP_COUNT; ++i)
        if (binop_precedence[i].kind == pctx->curr_tok.kind) return binop_precedence[i].p;
    return -1;
}

Token_Kind next_tok_kind(Parse_Context* pctx)
{
    if (pctx->token_list.count < pctx->token_index + 1) {
        error("No next token. We're all out.");
    }
    Token_Kind kind = pctx->token_list.data[pctx->token_index].kind;
    return kind;
}

bool tok_is(Parse_Context* pctx, Token_Kind kind)
{
    if (pctx->curr_tok.kind == kind) {
        return true;
    }
    return false;
}

void eat(Parse_Context* pctx) { pctx->curr_tok = pctx->token_list.data[pctx->token_index++]; }

void eat_kind(Parse_Context* pctx, Token_Kind kind)
{
    Token t = pctx->curr_tok;
    if (t.kind != kind) {

        char* start = t.line_start;
        char* end = start;

        // u8 lines_up = 0;
        // while (lines_up != LINES_ABOVE_AND_BELOW_TO_SHOW_ON_ERROR && *start != '\0') {
        //     if (*start == '\n') ++lines_up;
        //     --start;
        // }

        // u8 lines_down = 0;
        // while (lines_down != LINES_ABOVE_AND_BELOW_TO_SHOW_ON_ERROR && *end != '\0') {
        //     if (*end == '\n') ++lines_down;
        //     ++end;
        // }

        while (*end != '\n')
            ++end;
        s64 len = end - start;
        char* str = xmalloc(len + 1);
        memcpy(str, start, len);
        str[len] = 0;

        error("%s:%llu,%llu: Syntax Error: expected '%s' got '%s'\n%s", pctx->source_file, t.line_pos, t.col_pos,
              token_kind_to_str(kind), token_kind_to_str(t.kind), str);
    }
    eat(pctx);
}

void reset_label_counter(Parse_Context* pctx) { 
    pctx->label_counter = 0; 
}

char* make_label(Parse_Context* pctx) { 
    char* c = strf(".L%d", pctx->label_counter);
    pctx->label_counter += 1;
    return c;
}
