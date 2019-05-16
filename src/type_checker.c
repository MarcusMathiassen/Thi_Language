#include "type_checker.h"
#include "ast.h"
#include "constants.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert
#include <string.h> // strcmp

#define UNFINISHED \
    error("[UNFINISHED] %s: %s", give_unique_color((char*)__func__), wrap_with_colored_parens(ast_to_str(expr)));

// #define DEBUG_START                                                                                            \
//     info_no_newline("%s: %s", give_unique_color((char*)__func__), wrap_with_colored_parens(ast_to_str(expr))); \
    // assert(expr);

#define DEBUG_START \
    info("%s: %s", give_unique_color(ast_kind_to_str(expr->kind)), wrap_with_colored_parens(ast_to_str(expr))); \
    assert(expr);

#define DEBUG_END \
    // info(" -> %s", give_unique_color(type_to_str(expr->type))); \
    // assert(expr->type);

typedef struct
{
    Map*  symbol_table;
    AST*  active_function;
    Type* expected_type;
} Typer_Context;

Type* type_check_expr(Typer_Context* ctx, AST* expr);

Type* type_check_typeof(Typer_Context* ctx, AST* expr);
Type* type_check_sizeof(Typer_Context* ctx, AST* expr);
Type* type_check_switch(Typer_Context* ctx, AST* expr);
Type* type_check_extern(Typer_Context* ctx, AST* expr);
Type* type_check_struct(Typer_Context* ctx, AST* expr);
Type* type_check_enum(Typer_Context* ctx, AST* expr);
Type* type_check_function(Typer_Context* ctx, AST* expr);
Type* type_check_note(Typer_Context* ctx, AST* expr);
Type* type_check_int(Typer_Context* ctx, AST* expr);
Type* type_check_float(Typer_Context* ctx, AST* expr);
Type* type_check_string(Typer_Context* ctx, AST* expr);
Type* type_check_ident(Typer_Context* ctx, AST* expr);
Type* type_check_call(Typer_Context* ctx, AST* expr);
Type* type_check_unary(Typer_Context* ctx, AST* expr);
Type* type_check_binary(Typer_Context* ctx, AST* expr);
Type* type_check_variable_decl(Typer_Context* ctx, AST* expr);
Type* type_check_constant_decl(Typer_Context* ctx, AST* expr);
Type* type_check_block(Typer_Context* ctx, AST* expr);
Type* type_check_subscript(Typer_Context* ctx, AST* expr);
Type* type_check_field_access(Typer_Context* ctx, AST* expr);
Type* type_check_if(Typer_Context* ctx, AST* expr);
Type* type_check_for(Typer_Context* ctx, AST* expr);
Type* type_check_while(Typer_Context* ctx, AST* expr);
Type* type_check_return(Typer_Context* ctx, AST* expr);
Type* type_check_defer(Typer_Context* ctx, AST* expr);
Type* type_check_break(Typer_Context* ctx, AST* expr);
Type* type_check_continue(Typer_Context* ctx, AST* expr);
Type* type_check_as(Typer_Context* ctx, AST* expr);
Type* type_check_is(Typer_Context* ctx, AST* expr);

void type_checker(Map* symbol_table, List* ast) {
    info("Type checking...");

    Typer_Context ctx;
    ctx.symbol_table = symbol_table;

    LIST_FOREACH(ast) {
        type_check_expr(&ctx, (AST*)it->data);
    }
    // print_ast(ast);
    // error("Type Checker DEBUG ,,, ,, ,,, ,,");
}

Type* type_check_expr(Typer_Context* ctx, AST* expr) {
    if (!expr) return NULL;

    DEBUG_START;

    // This nodes type
    Type* t = NULL;
    // clang-format off
    switch (expr->kind) {
    case AST_FALLTHROUGH:    t = NULL;                                       break;
    case AST_LOAD:           t = NULL;                                       break;
    case AST_LINK:           t = NULL;                                       break;
    case AST_TYPEOF:         t = type_check_typeof(ctx, expr);               break;
    case AST_SIZEOF:         t = type_check_sizeof(ctx, expr);               break;
    case AST_SWITCH:         t = type_check_switch(ctx, expr);               break;
    case AST_EXTERN:         t = type_check_extern(ctx, expr);               break;
    case AST_STRUCT:         t = type_check_struct(ctx, expr);               break;
    case AST_ENUM:           t = type_check_enum(ctx, expr);                 break;
    case AST_FUNCTION:       t = type_check_function(ctx, expr);             break;
    case AST_NOTE:           t = type_check_note(ctx, expr);                 break;
    case AST_INT:            t = type_check_int(ctx, expr);                  break;
    case AST_FLOAT:          t = type_check_float(ctx, expr);                break;
    case AST_STRING:         t = type_check_string(ctx, expr);               break;
    case AST_IDENT:          t = type_check_ident(ctx, expr);                break;
    case AST_CALL:           t = type_check_call(ctx, expr);                 break;
    case AST_UNARY:          t = type_check_unary(ctx, expr);                break;
    case AST_BINARY:         t = type_check_binary(ctx, expr);               break;
    case AST_VARIABLE_DECL:  t = type_check_variable_decl(ctx, expr);        break;
    case AST_CONSTANT_DECL:  t = type_check_constant_decl(ctx, expr);        break;
    case AST_BLOCK:          t = type_check_block(ctx, expr);                break;
    case AST_GROUPING:       t = type_check_expr(ctx, expr->Grouping.expr);  break;
    case AST_SUBSCRIPT:      t = type_check_subscript(ctx, expr);            break;
    case AST_FIELD_ACCESS:   t = type_check_field_access(ctx, expr);         break;
    case AST_IF:             t = type_check_if(ctx, expr);                   break;
    case AST_FOR:            t = type_check_for(ctx, expr);                  break;
    case AST_WHILE:          t = type_check_while(ctx, expr);                break;
    case AST_RETURN:         t = type_check_return(ctx, expr);               break;
    case AST_DEFER:          t = type_check_defer(ctx, expr);                break;
    case AST_BREAK:          t = type_check_break(ctx, expr);                break;
    case AST_CONTINUE:       t = type_check_continue(ctx, expr);             break;
    case AST_AS:             t = type_check_as(ctx, expr);                   break;
    case AST_IS:             t = type_check_is(ctx, expr);                   break;
    default:
        error("Unhandled %s case for kind '%s'", give_unique_color((char*)__func__), ast_to_str(expr));
    }
    // clang-format on

    expr->type = t;

    DEBUG_END;

    return t;
}

Type* type_check_sizeof(Typer_Context* ctx, AST* expr) {
    type_check_expr(ctx, expr->Sizeof.expr);
    return (Type*)map_get(ctx->symbol_table, DEFAULT_BIG_INT_TYPE_AS_STRING);
}

Type* type_check_typeof(Typer_Context* ctx, AST* expr) {
    return type_check_expr(ctx, expr->Typeof.expr);
}

Type* type_check_switch(Typer_Context* ctx, AST* expr) {

    AST* cond         = expr->Switch.cond;
    AST* cases        = expr->Switch.cases;
    AST* default_case = expr->Switch.default_case;

    // Make sure the resulting type is of type INT.
    Type* cond_t = type_check_expr(ctx, cond);
    if (cond_t->kind == TYPE_INT) {
        error("%s is not a INT.", ast_to_str(cond), type_to_str(cond_t));
    }

    // A switches type is the same as its cases return type if any.
    Type* a = type_check_expr(ctx, cases);
    Type* b = type_check_expr(ctx, default_case);

    if (!is_same_type(a, b)) {
        error("[type_missmatch] %s != %s", type_to_str(a), type_to_str(b));
    }

    return a;
}

Type* type_check_extern(Typer_Context* ctx, AST* expr) {
    return expr->Extern.type;
}
Type* type_check_struct(Typer_Context* ctx, AST* expr) {
    LIST_FOREACH(expr->Struct.type->Struct.members) {
        type_check_expr(ctx, it->data);
    }
    return expr->Struct.type;
}
Type* type_check_enum(Typer_Context* ctx, AST* expr) {
    LIST_FOREACH(expr->Enum.type->Enum.members) {
        type_check_expr(ctx, it->data);
    }
    return expr->Enum.type;
}
Type* type_check_function(Typer_Context* ctx, AST* expr) {

    Type* func_type = expr->Function.type;
    AST*  func_body = expr->Function.body;
    List* args      = func_type->Function.args;

    ctx->active_function = expr;

    LIST_FOREACH(args) {
        AST* arg = (AST*)it->data;
        type_check_expr(ctx, arg);
    }

    type_check_expr(ctx, func_body);

    expr->type->Function.ret_type = expr->type->Function.ret_type;
    func_body->type               = expr->type->Function.ret_type;

    ctx->active_function = NULL;

    return func_type;
}
Type* type_check_note(Typer_Context* ctx, AST* expr) {
    return NULL;
}

Type* type_check_int(Typer_Context* ctx, AST* expr) {
    return (Type*)map_get(ctx->symbol_table, DEFAULT_INT_TYPE_AS_STRING);
}

Type* type_check_float(Typer_Context* ctx, AST* expr) {
    return (Type*)map_get(ctx->symbol_table, DEFAULT_FLOAT_TYPE_AS_STRING);
}
Type* type_check_string(Typer_Context* ctx, AST* expr) {
    return make_type_pointer(make_type_int(8, 1));
}
Type* type_check_ident(Typer_Context* ctx, AST* expr) {
    return (Type*)map_get(ctx->symbol_table, expr->Ident.name);
}
Type* type_check_call(Typer_Context* ctx, AST* expr) {
    char* callee = expr->Call.callee;
    List* args   = expr->Call.args;
    Type* func_t = (Type*)map_get(ctx->symbol_table, callee);
    LIST_FOREACH(args) {
        type_check_expr(ctx, it->data);
    }
    return func_t;
}
Type* type_check_unary(Typer_Context* ctx, AST* expr) {
    Token_Kind op      = expr->Unary.op;
    AST*       operand = expr->Unary.operand;

    Type* res = NULL;
    switch (op) {
    case TOKEN_DOT: {
        if (ctx->expected_type->kind == TYPE_ENUM) {
            Type* enum_t = ctx->expected_type;
            res          = enum_t;
            assert(operand->kind == AST_IDENT);
            LIST_FOREACH(enum_t->Enum.members) {
                AST* mem = (AST*)it->data;
                if (strcmp(operand->Ident.name, mem->Constant_Decl.name) == 0) {
                    *expr = *make_ast_binary(
                        mem->t, TOKEN_DOT, make_ast_ident(mem->t, enum_t->Enum.name), operand);
                }
            }
        }
    } break;
    default: res = type_check_expr(ctx, operand); break;
    }

    return res;
}
Type* type_check_binary(Typer_Context* ctx, AST* expr) {

    AST* lhs = expr->Binary.lhs;
    AST* rhs = expr->Binary.rhs;

    Type* a = type_check_expr(ctx, lhs);
    Type* b = type_check_expr(ctx, rhs);

    if (!is_same_type(a, b)) {
        error("[type_missmatch] %s -> %s != %s ", ast_to_str(expr), type_to_str(a), type_to_str(b));
    }

    return lhs->type;
}
Type* type_check_variable_decl(Typer_Context* ctx, AST* expr) {
    char* name            = expr->Variable_Decl.name;
    Type* type            = expr->Variable_Decl.type;
    AST*  assignment_expr = expr->Variable_Decl.value;
    assert(name);

    // if the variable has no assigned type. We infer it based on the
    // assignment expression.
    if (!type) {
        // It must have an assignment expression.
        assert(assignment_expr);
        type_check_expr(ctx, assignment_expr);
        type = assignment_expr->type;
    } else {
        ctx->expected_type = type;
        if (assignment_expr) {
            type_check_expr(ctx, assignment_expr);
            // assert(type->kind == assignment_expr->type->kind);
        }
    }
    ctx->expected_type = type;

    map_set_overwrite(ctx->symbol_table, name, type);

    return type;
}
Type* type_check_constant_decl(Typer_Context* ctx, AST* expr) {
    return type_check_expr(ctx, expr->Constant_Decl.value);
}
Type* type_check_block(Typer_Context* ctx, AST* expr) {
    List* stmts = expr->Block.stmts;
    LIST_FOREACH(stmts) {
        type_check_expr(ctx, it->data);
    }
    return NULL;
}
Type* type_check_subscript(Typer_Context* ctx, AST* expr) {

    AST* load = expr->Subscript.load;
    AST* sub = expr->Subscript.sub;

    warning("load: %s", ast_to_str(load));
    warning("sub: %s", ast_to_str(sub));

    Type * t = type_check_expr(ctx, load);
    type_check_expr(ctx, sub);

    warning("type: %s", type_to_str(t));
    warning("type size: %d", get_size_of_type(t));

    t = get_underlying_type(t);

    warning("array type: %s", type_to_str(t));
    warning("array type size: %d", get_size_of_type(t));

    info_no_newline("transformed %s into ", give_unique_color(ast_to_str(expr)));

    // Get the size of the underlaying type
    s64 u_size = get_size_of_type(t);

    // Compute the offset
    expr = make_ast_binary(expr->t, TOKEN_ASTERISK, sub, make_ast_int(expr->t, u_size));
    // Get the memory location of the load
    AST* stack_ref = make_ast_unary(expr->t, THI_SYNTAX_ADDRESS, load);
    // Add the offset and memory address together
    expr = make_ast_binary(expr->t, TOKEN_PLUS, stack_ref, expr);
    // Load the resulting location by dereferencing the memory address
    expr = make_ast_unary(expr->t, THI_SYNTAX_POINTER, expr); 

    // ex. array: Foo[5]
    //     array[4].i[5]
    // trfm -> *(&(array) + sub * sizeof(array[0]))
    // turns it into this ->  *(&v + 0)

    info("%s -> %s", give_unique_color(ast_to_str(expr)), type_to_str(t));

    return t;
}

char* get_name_of_member(AST* mem) {
    switch (mem->kind) {
    case AST_IDENT: return mem->Ident.name;
    case AST_VARIABLE_DECL: return mem->Variable_Decl.name;
    case AST_CONSTANT_DECL: return mem->Constant_Decl.name;
    case AST_FUNCTION: return mem->Function.type->Function.name;
    default: error("unhandled %s case %d", give_unique_color((char*)__func__), ast_kind_to_str(mem->kind));
    }
}

Type* type_check_field_access(Typer_Context* ctx, AST* expr) {
    char* type_name  = expr->Field_Access.load->Ident.name;
    char* field_name = expr->Field_Access.field;
    info("field: %s type: %s", field_name, type_name);

    Type* t = type_check_expr(ctx, expr->Field_Access.load);

    // Type* t = map_get(ctx->symbol_table, type_name);

    info("looking for %s in %s", field_name, type_to_str(t));
    Type* res = NULL;
    switch (t->kind) {
    case TYPE_ENUM: {
        LIST_FOREACH(t->Enum.members) {
            AST* mem = it->data;
            info_no_newline("on %s", ast_to_str(mem));
            if (strcmp(mem->Constant_Decl.name, field_name) == 0) {
                info("found it!");
                ast_replace(expr, mem->Constant_Decl.value);
                res = mem->type;
                break;
            }
        }
    } break;
    case TYPE_STRUCT: {
        LIST_FOREACH(t->Struct.members) {
            AST* mem = it->data;
            info_no_newline("on %s", ast_to_str(mem));
            char* name = get_name_of_member(mem);
            if (strcmp(name, field_name) == 0) {
                info("found it!");
                info("getting offset to '%s' in type '%s'", name, type_to_str(t));
                s64 offset = get_offset_in_struct_to_field(t, field_name);

                AST* stack_ref = make_ast_unary(expr->t, THI_SYNTAX_ADDRESS, make_ast_ident(expr->t, type_name));
                expr           = make_ast_binary(expr->t, TOKEN_PLUS, stack_ref, make_ast_int(expr->t, offset));
                expr           = make_ast_unary(expr->t, THI_SYNTAX_POINTER, expr);
                //                turns it into this ->  *(&v + 0)
                res = mem->type;
                break;
            }
        }
    } break;
    }

    // type_check_expr(ctx, expr);

    return res;
}
Type* type_check_if(Typer_Context* ctx, AST* expr) {
    AST* cond       = expr->If.cond;
    AST* then_block = expr->If.then_block;
    AST* else_block = expr->If.else_block;
    type_check_expr(ctx, cond);
    ctx->expected_type = cond->type;
    Type* t            = type_check_expr(ctx, then_block);
    type_check_expr(ctx, else_block);
    return cond->type;
}
Type* type_check_for(Typer_Context* ctx, AST* expr) {
    type_check_expr(ctx, expr->For.init);
    type_check_expr(ctx, expr->For.cond);
    type_check_expr(ctx, expr->For.step);
    type_check_expr(ctx, expr->For.then_block);
    return NULL;
}
Type* type_check_while(Typer_Context* ctx, AST* expr) {
    type_check_expr(ctx, expr->While.cond);
    type_check_expr(ctx, expr->While.then_block);
    return NULL;
}

Type* type_check_return(Typer_Context* ctx, AST* expr) {
    AST*  ret_expr = expr->Return.expr;
    Type* t        = type_check_expr(ctx, ret_expr);
    assert(ctx->active_function);
    ctx->active_function->Function.type->Function.ret_type = t;
    ctx->active_function->Function.body->type              = t;
    return t;
}

Type* type_check_defer(Typer_Context* ctx, AST* expr) {
    return type_check_expr(ctx, expr->Defer.expr);
    ;
}
Type* type_check_break(Typer_Context* ctx, AST* expr) {
    return type_check_expr(ctx, expr->Break.expr);
}
Type* type_check_continue(Typer_Context* ctx, AST* expr) {
    return type_check_expr(ctx, expr->Continue.expr);
    ;
}
Type* type_check_as(Typer_Context* ctx, AST* expr) {
    type_check_expr(ctx, expr->As.expr);
    Type* t = type_check_expr(ctx, expr->As.type_expr);
    return t;
}

Type* type_check_is(Typer_Context* ctx, AST* expr) {
    type_check_expr(ctx, expr->Is.body);
    type_check_expr(ctx, expr->Is.expr);
    UNFINISHED;
    return NULL;
}
