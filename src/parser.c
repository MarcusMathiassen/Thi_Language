#include "parser.h"

#include <assert.h>           // assert
#include <ctype.h>            // atoll
#include "lexer.h"            // Token, Token_Kind
#include "stretchy_buffer.h"  // sb_push
#include "typedefs.h"         // i32, i64, etc.
#include "types.h"            // Type
#include "utility.h"          // info, error, warning, success

#include "globals.h"          // add_symbol

#define BIN_OP_COUNT 46
static struct { Token_Kind kind; int p; }  binop_precedence[BIN_OP_COUNT] = {

    { TOKEN_DOT,           100 }, // .
    { TOKEN_OPEN_PAREN,    100 }, // ()
    { TOKEN_OPEN_BRACKET,  100 }, // []

    { TOKEN_PLUS_PLUS,    90 }, // ++
    { TOKEN_MINUS_MINUS,  90 }, // --

    { TOKEN_ASTERISK,  80 }, // *
    { TOKEN_FWSLASH,   80 }, // /

    { TOKEN_MINUS,  70 }, // -
    { TOKEN_PLUS,   70 }, // +

    { TOKEN_LESS,           60 }, // <
    { TOKEN_GREATER,        60 }, // >
    { TOKEN_GREATER_EQUAL,  60 }, // >=
    { TOKEN_LESS_EQUAL,     60 }, // <=

    { TOKEN_EQUAL_EQUAL,  50 }, // ==
    { TOKEN_BANG_EQUAL,   50 }, // !=

    { TOKEN_AND_AND,    48 }, // &&
    { TOKEN_PIPE_PIPE,  47 }, // ||

    { TOKEN_AT,        40 }, // @
    { TOKEN_AND,       40 }, // &
    { TOKEN_HAT,       30 }, // ^
    { TOKEN_PIPE,      20 }, // |

    { TOKEN_EQUAL,           10 }, // =
    { TOKEN_PLUS_EQUAL,      10 }, // +=
    { TOKEN_MINUS_EQUAL,     10 }, // -=
    { TOKEN_ASTERISK_EQUAL,  10 }, // *=
    { TOKEN_FWSLASH_EQUAL,   10 }, // /=
    { TOKEN_PERCENT_EQUAL,   10 }, // %=
    { TOKEN_AND_EQUAL,       10 }, // &=
    { TOKEN_HAT_EQUAL,       10 }, // ^=
    { TOKEN_PIPE_EQUAL,      10 }, // |=

    { TOKEN_BITWISE_LEFTSHIFT,   10 }, // <<=
    { TOKEN_BITWISE_RIGHTSHIFT,  10 }, // >>=

    // Ternary operator
    { TOKEN_QUESTION_MARK,  5 }, // >>=
    { TOKEN_COLON,          5 }, // >>= 
};

//------------------------------------------------------------------------------
//                               Parser
//------------------------------------------------------------------------------

static bool tok_is(Token_Kind kind);
static int get_tok_precedence();
static void add_new_symbol();
static void eat();
static void eat_kind(Token_Kind kind);
static Expr* get_definition(const char* ident);
static Expr* parse_top_level();
static Expr* parse_statement();
static Expr* parse_primary();
static Expr* parse_identifier();
static Expr* parse_block();
static Expr* parse_ret();
static Expr* parse_expression();
static Expr* parse_unary();
static Expr* parse_binary(int expr_prec, Expr* lhs);
static Expr* parse_integer();
static Expr* parse_parens();

static Type* parse_struct_signature(const char* struct_name);
static Type* parse_function_signature(const char* func_name);

static void skip_block();
static void skip_enum_signature();
static void skip_struct_signature();
static void skip_function_signature();

//-----------------
// Global variables
//-----------------
static Token* g_tokens = NULL;
static u64 token_index = 0;
static Token curr_tok;
static Token top_tok;

//------------------------------------------------------------------------------
//                               Public Functions
//------------------------------------------------------------------------------

AST** generate_ast_from_tokens(Token* tokens) {
    success("Generating AST from Tokens");

    g_tokens = tokens;
    token_index = 0;
    top_tok.kind = TOKEN_UNKNOWN;
    curr_tok.kind = TOKEN_UNKNOWN;
    eat();
    
    Expr** ast = NULL;
    while (!tok_is(TOKEN_EOF)) {
        Expr* stmt = parse_top_level();
        if (stmt) {
            sb_push(ast, stmt);
        }
        print_token(curr_tok);
    }

    return ast;
}

void generate_symbol_table_from_tokens(Token* tokens)
{
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
                error("TOKEN_LOAD parser not implemented.");
            } break;

            case TOKEN_FOREIGN: {
                error("TOKEN_LOAD parser not implemented.");
                // parse_and_add_foreign_function(current_token.value);
            } break;
        }
    }
}

//------------------------------------------------------------------------------
//                               Private Functions
//------------------------------------------------------------------------------

static Expr* parse_top_level()
{
    top_tok = curr_tok;
    switch (curr_tok.kind)
    {
        case TOKEN_FOREIGN:
            error("MISSING IMPLEENTATI FOR FOREIGN TOP LEVEL");
            // eat();
            // skip_function_signature();
            // break;
        case TOKEN_IDENTIFIER: return parse_statement();
    }
    
    return NULL;
}

static Expr* parse_statement()
{  
    switch (curr_tok.kind)
    {
        case TOKEN_IDENTIFIER:        return parse_expression();
        // case TOKEN_PRINT:             return parse_print();
        // case TOKEN_BREAK:             return parse_break();
        // case TOKEN_CONTINUE:          return parse_continue();
        case TOKEN_RETURN:            return parse_ret();
        // case TOKEN_IF_STATEMENT:      return parse_if();
        // case TOKEN_FOR_LOOP:          return parse_for();
        // case TOKEN_WHILE_LOOP:        return parse_while();
        // case TOKEN_REPEAT:            return parse_repeat();
        case TOKEN_OPEN_BRACE:        return parse_block();
        case TOKEN_OPEN_PAREN:        return parse_parens();
    }
    return NULL;
}

static Expr* parse_primary()
{
    warning("parse_primary: %s",  token_kind_to_str(curr_tok.kind));
    switch (curr_tok.kind)
    {
        case TOKEN_IDENTIFIER:     return parse_identifier();
        // case TOKEN_DOLLAR_SIGN:    return parse_note();
        // case TOKEN_HEX:            return parse_hex();
        case TOKEN_INTEGER:        return parse_integer();
        // case TOKEN_FLOATING_POINT: return parse_float();
        // case TOKEN_STRING:         return parse_string();
        case TOKEN_OPEN_PAREN:     return parse_parens();
        case TOKEN_OPEN_BRACE:     return parse_block();
        // case TOKEN_OPEN_BRACKET:   return parse_array();
        // case TOKEN_TRUE:           return parse_constant();
        // case TOKEN_FALSE:          return parse_constant();
        // case TOKEN_NIL:            return parse_nil();
        // case TOKEN_CAST:           return parse_cast();
        // case TOKEN_MALLOC:         return parse_malloc();
        // case TOKEN_SIZEOF:         return parse_sizeof();
    }
    return NULL;
}

static Expr* parse_identifier()
{
    warning("parse_identifier");
    const char* ident = curr_tok.value;
    eat();
    switch (curr_tok.kind)
    {
        case TOKEN_COLON_COLON:   return get_definition(ident);
        // case TOKEN_COLON_EQUAL:   return get_variable_typeinferred();
        // case TOKEN_COLON:         return get_variable_declaration();
        // case TOKEN_OPEN_PAREN:    return get_function_call();
        // case TOKEN_OPEN_BRACKET:  return get_subscript_access();
    }
    return make_expr_ident(ident);
}

static Expr* parse_block() {
    warning("parse_block");
    Expr** statements = NULL;
    eat();
    while (!tok_is(TOKEN_CLOSE_BRACE))
    {
        Expr* stmt = parse_statement();
        if (stmt) sb_push(statements, stmt);
    }
    eat(TOKEN_CLOSE_BRACE);
    return make_expr_block(statements);
}

static Expr* parse_ret() {
    warning("parse_ret");
    eat();
    print_token(curr_tok);
    Expr* exp = parse_expression();
    return make_expr_ret(exp);
}

static Expr* parse_binary(int expr_prec, Expr* lhs) {
    warning("parse_binary");
    // If this is a binop, find its precedence.
    while (1) {
        const int tok_prec = get_tok_precedence();

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (tok_prec < expr_prec) return lhs;

        // Okay, we know this is a binop.
        Token_Kind binary_op_token = curr_tok.kind;
        eat();  // eat binop

        // Parser the unary expression after the binary operator.
        Expr* rhs = parse_unary();
        if (!rhs) {
            warning("parse_binary rhs returned nullptr");
            return NULL;
        }
        // If BinOp binds less tightly with rhs than the operator after rhs, let
        // the pending operator take rhs as its lhs.
        int next_prec = get_tok_precedence();
        if (tok_prec < next_prec) {
            rhs = parse_binary(tok_prec + 1, rhs);

            if (!rhs) {
                warning("parseeee_binary rhs returned nullptr");
                return NULL;
            }
        }

        // Merge LHS/rhs.
        lhs = make_expr_binary(binary_op_token, lhs, rhs);
    }
}

static Expr* parse_unary() {
    warning("parse_unary");
    if (tok_is(TOKEN_BANG) || tok_is(THI_SYNTAX_POINTER) ||
        tok_is(TOKEN_MINUS) || tok_is(THI_SYNTAX_ADDRESS))
    {
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

static Expr* parse_expression() {
    warning("parse_expression");

    Expr* lhs = parse_unary();
    if (lhs) {
        return parse_binary(0, lhs);
    }
    warning("parse_unary returned NULL");
    return NULL;
}

static Expr* parse_integer() {
    warning("parse_integer");
    Expr* res = make_expr_int(atoll(curr_tok.value));
    eat(TOKEN_INTEGER);
    return res;
}

static Expr* parse_parens() {
    warning("parse_parens");
    eat_kind(TOKEN_OPEN_PAREN);
    Expr* exp = parse_expression();
    if (!exp) {
        warning("parse_parens returned nullptr");
        return NULL;
    }
    eat_kind(TOKEN_CLOSE_PAREN);
    return make_expr_grouping(exp);
}
//------------------------------------------------------------------------------
//                               Type Utilty Functions
//------------------------------------------------------------------------------

static i64 get_integer() {
    i64 value = 0;
    switch (curr_tok.kind) {
        case TOKEN_INTEGER:   value = atoll(curr_tok.value); break;
        case TOKEN_HEX:       value = strtoll(curr_tok.value, NULL, 0); break;
        default: error("not an integer."); break;
    }
    eat();
    return value;
}

static f64 get_float() {
    f64 value = atof(curr_tok.value);
    eat();
    return value;
}

static Type* get_type() {
    const char* type_name = curr_tok.value;
    eat_kind(TOKEN_IDENTIFIER);
    Type* type = NULL;

    if (is_builtin_type(type_name))
        type = get_builtin_type(type_name);
    else
        type = get_symbol(type_name);

    // Is a pointer or array?
    if (tok_is(THI_SYNTAX_POINTER))
    {
        eat_kind(THI_SYNTAX_POINTER);
        return make_type_pointer(type);
    } else if (tok_is(TOKEN_OPEN_BRACKET))
    {
        eat_kind(TOKEN_OPEN_BRACKET);
        i64 size = 0;
        if (tok_is(TOKEN_INTEGER) || tok_is(TOKEN_HEX))
            size = get_integer();
        eat_kind(TOKEN_CLOSE_BRACKET);
        // return make_type_array(type, size);
        // return new Type_Array(type, size);
    }

    if (!type)
        error("no type found for type '%s'", type_name);

    return type;
}

//------------------------------------------------------------------------------
//                               Parsing Utility Functions
//------------------------------------------------------------------------------

static Type* parse_struct_signature(const char* struct_name) {
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
    eat();
    return make_type_struct(struct_name, members);
}

static Type* parse_function_signature(const char* func_name) {

    eat_kind(TOKEN_OPEN_PAREN);

    Arg* args = NULL;

    bool has_multiple_arguments = false;

    while (!tok_is(TOKEN_CLOSE_PAREN)) {

        if (has_multiple_arguments)
            eat_kind(TOKEN_COMMA);

        Arg arg;
        arg.name = curr_tok.value;
        arg.type = NULL;

        // foreign's dont have named parameters
        if (top_tok.kind == TOKEN_FOREIGN) {
            arg.type = get_type();
        } else {
            eat_kind(TOKEN_IDENTIFIER);
            eat_kind(TOKEN_COLON);
            arg.type = get_type();
        }

        has_multiple_arguments = true;

        sb_push(args, arg);
    }

    eat();

    Type* ret_type = NULL;

    if (tok_is(TOKEN_RIGHT_ARROW)) {
        eat();
        ret_type = get_type();
    }

    return make_type_func(func_name, args, ret_type);
}

static Expr* get_definition(const char* ident) {
    warning("get_definition");
    eat(TOKEN_COLON_COLON);
    switch (curr_tok.kind)
    {
        // case TOKEN_ENUM:
        // {
        //     eat();
        //     skip_enum_signature();
        //     return get_symbol(ident);
        // }
        // case TOKEN_STRUCT:
        // {
        //     eat();
        //     skip_struct_signature();
        //     return get_symbol(ident);
        // }
        case TOKEN_OPEN_PAREN:
        {
            skip_function_signature();
            Expr* body = parse_block();
            return make_expr_func(get_symbol(ident), body);
        }
        // default: return get_constant(ident); 
    }

    error("GET DEF RETURNING NULL");
    return NULL;
}

static int get_tok_precedence() {
    for (int i = 0; i < BIN_OP_COUNT; ++i)
    {
        if (binop_precedence[i].kind == curr_tok.kind)
        {   
            if (binop_precedence[i].p <= 0) {
                return -1;
            } else {
                info("token: %s is bin op: %s", curr_tok.value, token_kind_to_str(binop_precedence[i].kind));
                return binop_precedence[i].p;
            }
        }
    }
    error("GET TOK PREC NO BIN OP FOUND");
    return -1;
}

static bool tok_is(Token_Kind kind) {
    if (curr_tok.kind == kind) {
        return true;
    }
    return false;
}

static void eat()
{
    curr_tok = g_tokens[token_index++];
}

static void eat_kind(Token_Kind kind) {
    if (curr_tok.kind == kind) {
        eat();
    } else {
        error("expected '%s'", token_kind_to_str(kind));
    }
}

//------------------------------------------------------------------------------
//                               Skip Functions
//------------------------------------------------------------------------------

static void skip_block() {
    eat_kind(TOKEN_OPEN_BRACE);
    i64 counter = 1;
    while (true) {
        eat();
        if (tok_is(TOKEN_OPEN_BRACE))
            ++counter;
        if (tok_is(TOKEN_CLOSE_BRACE))
            --counter;
        if (counter == 0)
            break;
    }
    eat_kind(TOKEN_CLOSE_BRACE);
}

static void skip_enum_signature() {
    if (tok_is(TOKEN_IDENTIFIER))
        get_type();
    eat_kind(TOKEN_OPEN_BRACE);
    while (!tok_is(TOKEN_CLOSE_BRACE))
        eat_kind(TOKEN_IDENTIFIER);
    eat();
}

static void skip_struct_signature() {
    eat_kind(TOKEN_STRUCT);
    eat_kind(TOKEN_OPEN_BRACE);
    while (!tok_is(TOKEN_CLOSE_BRACE)) {
        eat_kind(TOKEN_IDENTIFIER);
        eat_kind(TOKEN_COLON);
        get_type();
    }
    eat();
}

static void skip_function_signature() {
    eat();
    bool has_multiple_arguments = false;
    while (!tok_is(TOKEN_CLOSE_PAREN))
    {
        if (has_multiple_arguments)
            eat_kind(TOKEN_COMMA);
        eat_kind(TOKEN_IDENTIFIER);
        eat_kind(TOKEN_COLON);
        get_type();
        has_multiple_arguments = true;
    }
    eat();
    if (tok_is(TOKEN_RIGHT_ARROW))
    {
        eat();
        get_type();
    }
}

//------------------------------------------------------------------------------
//                               Order-independence Functions
//------------------------------------------------------------------------------

static void add_new_symbol()
{
    const char*  ident = curr_tok.value;
    eat_kind(TOKEN_IDENTIFIER);
    if (tok_is(TOKEN_COLON_COLON))
    {
        eat();
        switch (curr_tok.kind)
        {
            case TOKEN_ENUM:
            {
                eat();
                // auto enum_signature = parse_enum_signature(ident);
                // add_enum(new AST_Enum(enum_signature));
                return;
            }
            case TOKEN_STRUCT:
            {
                Type* type = parse_struct_signature(ident);
                add_symbol(ident, type);
                return;
            }
            case TOKEN_OPEN_PAREN:
            {
                Type* type = parse_function_signature(ident);
                skip_block();
                add_symbol(ident, type);
                return;
            }
            default:
            {
                // eat();
                // auto expr = parse_expression();
                // add_constant(new AST_Constant_Variable(ident, expr));
                return;
            }
        }
    }
}
