// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "type_checker.h"
#include "ast.h"
#include "constants.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert
#include <string.h> // strcmp

#define DEBUG_START \
    assert(node);   \
    // info("%s: %s", give_unique_color(ast_kind_to_str(node->kind)), wrap_with_colored_parens(ast_to_str(node)));

typedef struct {
    Map*  symbol_table;
    AST*  module;
    AST*  active_function;
    Type* expected_type;
} Typer_Context;

// clang-format off
Type* type_check_node           (Typer_Context* ctx, AST* node);
Type* type_check_module         (Typer_Context* ctx, AST* node);
Type* type_check_typeof         (Typer_Context* ctx, AST* node);
Type* type_check_sizeof         (Typer_Context* ctx, AST* node);
Type* type_check_switch         (Typer_Context* ctx, AST* node);
Type* type_check_extern         (Typer_Context* ctx, AST* node);
Type* type_check_struct         (Typer_Context* ctx, AST* node);
Type* type_check_enum           (Typer_Context* ctx, AST* node);
Type* type_check_function       (Typer_Context* ctx, AST* node);
Type* type_check_note           (Typer_Context* ctx, AST* node);
Type* type_check_int            (Typer_Context* ctx, AST* node);
Type* type_check_float          (Typer_Context* ctx, AST* node);
Type* type_check_string         (Typer_Context* ctx, AST* node);
Type* type_check_ident          (Typer_Context* ctx, AST* node);
Type* type_check_call           (Typer_Context* ctx, AST* node);
Type* type_check_unary          (Typer_Context* ctx, AST* node);
Type* type_check_binary         (Typer_Context* ctx, AST* node);
Type* type_check_variable_decl  (Typer_Context* ctx, AST* node);
Type* type_check_constant_decl  (Typer_Context* ctx, AST* node);
Type* type_check_block          (Typer_Context* ctx, AST* node);
Type* type_check_subscript      (Typer_Context* ctx, AST* node);
Type* type_check_field_access   (Typer_Context* ctx, AST* node);
Type* type_check_if             (Typer_Context* ctx, AST* node);
Type* type_check_for            (Typer_Context* ctx, AST* node);
Type* type_check_while          (Typer_Context* ctx, AST* node);
Type* type_check_return         (Typer_Context* ctx, AST* node);
Type* type_check_defer          (Typer_Context* ctx, AST* node);
Type* type_check_break          (Typer_Context* ctx, AST* node);
Type* type_check_continue       (Typer_Context* ctx, AST* node);
Type* type_check_as             (Typer_Context* ctx, AST* node);
Type* type_check_is             (Typer_Context* ctx, AST* node);
// clang-format on

static Type* get_symbol(Typer_Context* ctx, char* name) {
    return (Type*)map_get(ctx->symbol_table, name);
}

void type_checker(Map* symbol_table, AST* ast) {
    info("Type checking...");
    Typer_Context ctx;
    ctx.symbol_table = symbol_table;
    type_check_node(&ctx, ast);
}

Type* type_check_node(Typer_Context* ctx, AST* node) {
    if (!node) return NULL;
    DEBUG_START;
    Type* result = NULL;
    // clang-format off
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
    case AST_COMMENT:        result = NULL;                                                   break;
    case AST_NOP:            result = NULL;                                                   break;
    case AST_FALLTHROUGH:    result = NULL;                                                   break;
    case AST_LOAD:           result = NULL;                                                   break;
    case AST_LINK:           result = NULL;                                                   break;
    case AST_TYPEOF:         result = type_check_typeof        (ctx, node);                   break;
    case AST_SIZEOF:         result = type_check_sizeof        (ctx, node);                   break;
    case AST_SWITCH:         result = type_check_switch        (ctx, node);                   break;
    case AST_EXTERN:         result = type_check_extern        (ctx, node);                   break;
    case AST_STRUCT:         result = type_check_struct        (ctx, node);                   break;
    case AST_ENUM:           result = type_check_enum          (ctx, node);                   break;
    case AST_FUNCTION:       result = type_check_function      (ctx, node);                   break;
    case AST_NOTE:           result = type_check_note          (ctx, node);                   break;
    case AST_INT:            result = type_check_int           (ctx, node);                   break;
    case AST_FLOAT:          result = type_check_float         (ctx, node);                   break;
    case AST_STRING:         result = type_check_string        (ctx, node);                   break;
    case AST_IDENT:          result = type_check_ident         (ctx, node);                   break;
    case AST_CALL:           result = type_check_call          (ctx, node);                   break;
    case AST_UNARY:          result = type_check_unary         (ctx, node);                   break;
    case AST_BINARY:         result = type_check_binary        (ctx, node);                   break;
    case AST_VARIABLE_DECL:  result = type_check_variable_decl (ctx, node);                   break;
    case AST_CONSTANT_DECL:  result = type_check_constant_decl (ctx, node);                   break;
    case AST_BLOCK:          result = type_check_block         (ctx, node);                   break;
    case AST_GROUPING:       result = type_check_node          (ctx, node->Grouping.node);    break;
    case AST_SUBSCRIPT:      result = type_check_subscript     (ctx, node);                   break;
    case AST_FIELD_ACCESS:   result = type_check_field_access  (ctx, node);                   break;
    case AST_IF:             result = type_check_if            (ctx, node);                   break;
    case AST_FOR:            result = type_check_for           (ctx, node);                   break;
    case AST_WHILE:          result = type_check_while         (ctx, node);                   break;
    case AST_RETURN:         result = type_check_return        (ctx, node);                   break;
    case AST_DEFER:          result = type_check_defer         (ctx, node);                   break;
    case AST_BREAK:          result = type_check_break         (ctx, node);                   break;
    case AST_CONTINUE:       result = type_check_continue      (ctx, node);                   break;
    case AST_AS:             result = type_check_as            (ctx, node);                   break;
    case AST_IS:             result = type_check_is            (ctx, node);                   break;
    case AST_MODULE:         result = type_check_module        (ctx, node);                   break;
    }
    // clang-format on
    node->type = result;
    return result;
}

Type* type_check_sizeof(Typer_Context* ctx, AST* node) {
    type_check_node(ctx, node->Sizeof.node);
    return get_symbol(ctx, DEFAULT_BIG_INT_TYPE_AS_STRING);
}
Type* type_check_module(Typer_Context* ctx, AST* node) {
    ctx->module = node;
    return type_check_node(ctx, node->Module.top_level);
}

Type* type_check_typeof(Typer_Context* ctx, AST* node) {
    return type_check_node(ctx, node->Typeof.node);
}

Type* type_check_switch(Typer_Context* ctx, AST* node) {

    AST* cond         = node->Switch.cond;
    AST* cases        = node->Switch.cases;
    AST* default_case = node->Switch.default_case;

    // Make sure the resulting type is of type INT.
    type_check_node(ctx, cond);
    // if (cond_t->kind == TYPE_INT) {
    // error("%s is not a INT.", ast_to_str(cond), type_to_str(NULL, cond_t));
    // }

    // A switches type is the same as its cases return type if any.
    Type* a = type_check_node(ctx, cases);
    Type* b = type_check_node(ctx, default_case);

    if (!is_same_type(a, b)) {
        error("[type_missmatch] %s != %s", type_to_str(NULL, a), type_to_str(NULL, b));
    }

    return a;
}

Type* type_check_extern(Typer_Context* ctx, AST* node) {
    return node->Extern.type;
}
Type* type_check_struct(Typer_Context* ctx, AST* node) {
    LIST_FOREACH(type_get_members(node->type)) {
        type_check_node(ctx, it->data);
    }
    return node->type;
}
Type* type_check_enum(Typer_Context* ctx, AST* node) {
    LIST_FOREACH(type_get_members(node->type)) {
        type_check_node(ctx, it->data);
    }
    return node->type;
}
Type* type_check_function(Typer_Context* ctx, AST* node) {

    Type* func_type = node->Function.type;
    AST*  func_body = node->Function.body;
    List* args      = func_type->Function.parameters;

    if (func_type->Function.return_type->kind == TYPE_UNRESOLVED) {
        func_type->Function.return_type = map_get(ctx->symbol_table, get_type_name(func_type->Function.return_type));
    }

    ctx->active_function = node;

    LIST_FOREACH(args) {
        AST* arg = (AST*)it->data;
        type_check_node(ctx, arg);
    }

    type_check_node(ctx, func_body);
    return func_type;
}
Type* type_check_note(Typer_Context* ctx, AST* node) {
    return NULL;
}

Type* type_check_int(Typer_Context* ctx, AST* node) {
    return (Type*)map_get(ctx->symbol_table, DEFAULT_INT_TYPE_AS_STRING);
}

Type* type_check_float(Typer_Context* ctx, AST* node) {
    return (Type*)map_get(ctx->symbol_table, DEFAULT_FLOAT_TYPE_AS_STRING);
}
Type* type_check_string(Typer_Context* ctx, AST* node) {
    return make_type_pointer(make_type_int(8, 1));
}

Type* type_check_ident(Typer_Context* ctx, AST* node) {
    char* identifier = node->Ident.name;
    return get_symbol(ctx, identifier);
}
Type* type_check_call(Typer_Context* ctx, AST* node) {
    char* callee = node->Call.callee;
    List* args   = node->Call.args;
    Type* func_t = (Type*)map_get(ctx->symbol_table, callee);
    assert(func_t);
    assert(func_t->Function.return_type);
    // if (func_t->Function.return_type->kind == TYPE_UNRESOLVED) {
    //     func_t->Function.return_type = map_get(ctx->symbol_table, get_type_name(func_t->Function.return_type));
    // }
    // info(type_to_str(NULL, func_t));
    LIST_FOREACH(args) {
        type_check_node(ctx, it->data);
    }
    func_t->Function.return_type->flags = func_t->flags; // @HACK
    return func_t->Function.return_type;
}
Type* type_check_unary(Typer_Context* ctx, AST* node) {
    Token_Kind op      = node->Unary.op;
    AST*       operand = node->Unary.operand;

    Type* result_t = type_check_node(ctx, operand);
    switch (op) {
    default: break;
    case THI_SYNTAX_POINTER: {
        result_t = result_t->Pointer.pointee;
    } break;
    case THI_SYNTAX_ADDRESS: {
        result_t = make_type_pointer(result_t);
    } break;
    }

    return result_t;
}
Type* type_check_binary(Typer_Context* ctx, AST* node) {

    AST* lhs = node->Binary.lhs;
    AST* rhs = node->Binary.rhs;

    Type* a = type_check_node(ctx, lhs);
    Type* b = type_check_node(ctx, rhs);

    if (!is_same_type(a, b)) {
        // error("[type_missmatch] %s -> %s != %s ", ast_to_str(node), type_to_str(NULL, a), type_to_str(NULL, b));
    }

    // 'a' and 'b' are the same so just return any one of them
    return a;
}
Type* type_check_variable_decl(Typer_Context* ctx, AST* node) {
    char* variable_name   = node->Variable_Decl.name;
    Type* variable_type   = node->Variable_Decl.type;
    AST*  assignment_expr = node->Variable_Decl.value;

    ctx->expected_type = variable_type ? variable_type : NULL;

    Type* assigned_type = type_check_node(ctx, assignment_expr);

    // Make sure the set type and assigned type is the same
    if (variable_type && assigned_type && !is_same_type(variable_type, assigned_type)) {
        error("[type_missmatch] %s -> %s != %s ", ast_to_str(node), type_to_str(NULL, variable_type), type_to_str(NULL, assigned_type));
    }
    variable_type      = assigned_type ? assigned_type : variable_type;
    ctx->expected_type = variable_type;
    map_set_overwrite(ctx->symbol_table, variable_name, variable_type);
    return variable_type;
}

Type* type_check_constant_decl(Typer_Context* ctx, AST* node) {
    AST* value = node->Constant_Decl.value;
    return type_check_node(ctx, value);
}

Type* type_check_block(Typer_Context* ctx, AST* node) {

    List* stmts = node->Block.stmts;

    // Gather all returned types
    List* returned_nodes = make_list();
    LIST_FOREACH(stmts) {
        AST* statement = it->data;
        type_check_node(ctx, statement);
        if (statement->kind == AST_RETURN) {
            list_append(returned_nodes, statement);
        }
    }

    Type* result_t = NULL;
    // In case of the block has a return
    if (returned_nodes->count > 0) {

        // ..make sure they are all the same type
        AST*  a   = list_first(returned_nodes);
        Type* a_t = a->type;
        LIST_FOREACH(returned_nodes) {
            AST*  b   = it->data;
            Type* b_t = b->type;
            // ..raise an error if not

            if (!is_same_type(a_t, b_t)) {
                error("[%s] Type_Error. Differing return types in block.\n%s <- %s\n!=\n%s <- %s", LOCATION_OF_ast_to_str(ctx->module, node), type_to_str(NULL, a_t), ast_to_str(a), type_to_str(NULL, b_t), ast_to_str(b));
            }
        }
        result_t = a_t;
    }

    // We return the common type returned from any return statement in
    // the block.

    return result_t;
}

Type* type_check_subscript(Typer_Context* ctx, AST* node) {
    AST*  load = node->Subscript.load;
    AST*  sub  = node->Subscript.sub;
    Type* t    = type_check_node(ctx, load);
    type_check_node(ctx, sub);
    t = get_underlying_type_if_any(t);
    return t;
}

char* get_name_of_member(AST* mem) {
    char* result = NULL;
    // clang-format off
    switch (mem->kind) {
    case AST_IDENT:             result = mem->Ident.name;                   break;
    case AST_VARIABLE_DECL:     result = mem->Variable_Decl.name;           break;
    case AST_CONSTANT_DECL:     result = mem->Constant_Decl.name;           break;
    case AST_FUNCTION:          result = mem->Function.type->Function.name; break;
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(mem->kind));
        // clang-format on
    }
    return result;
}

Type* type_check_field_access(Typer_Context* ctx, AST* node) {
    // char* type_name  = node->Field_Access.load->Ident.name;
    char* field_name = node->Field_Access.field;
    // info("field: %s type: %s", field_name, type_name);

    Type* t = type_check_node(ctx, node->Field_Access.load);
    assert(t);

    // Type* t = map_get(ctx->symbol_table, type_name);

    // info("looking for %s in %s", field_name, type_to_str(NULL, t));
    Type* res = NULL;
    switch (t->kind) {
    case TYPE_ENUM: {
        LIST_FOREACH(t->Enum.members) {
            AST* mem = it->data;
            info_no_newline("on %s", ast_to_str(mem));
            if (strcmp(mem->Constant_Decl.name, field_name) == 0) {
                // info("found it!");
                ast_replace(node, mem->Constant_Decl.value);
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
                // info("found it!");
                // info("getting offset to '%s' in type '%s'", name, type_to_str(NULL, t));
                res = mem->type;
                break;
            }
        }
    } break;
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind))
    }
    // type_check_node(ctx, node);
    return res;
}

Type* type_check_if(Typer_Context* ctx, AST* node) {
    AST* cond       = node->If.cond;
    AST* then_block = node->If.then_block;
    AST* else_block = node->If.else_block;
    type_check_node(ctx, cond);
    ctx->expected_type = cond->type;
    type_check_node(ctx, then_block);
    type_check_node(ctx, else_block);
    return cond->type;
}
Type* type_check_for(Typer_Context* ctx, AST* node) {
    type_check_node(ctx, node->For.init);
    type_check_node(ctx, node->For.cond);
    type_check_node(ctx, node->For.step);
    type_check_node(ctx, node->For.then_block);
    return NULL;
}
Type* type_check_while(Typer_Context* ctx, AST* node) {
    type_check_node(ctx, node->While.cond);
    type_check_node(ctx, node->While.then_block);
    return NULL;
}

Type* type_check_return(Typer_Context* ctx, AST* node) {
    AST*  ret_node = node->Return.node;
    Type* t        = type_check_node(ctx, ret_node);
    return t;
}

Type* type_check_defer(Typer_Context* ctx, AST* node) {
    return type_check_node(ctx, node->Defer.node);
}
Type* type_check_break(Typer_Context* ctx, AST* node) {
    return type_check_node(ctx, node->Break.node);
}
Type* type_check_continue(Typer_Context* ctx, AST* node) {
    return type_check_node(ctx, node->Continue.node);
}
Type* type_check_as(Typer_Context* ctx, AST* node) {
    type_check_node(ctx, node->As.node);
    Type* t = type_check_node(ctx, node->As.type_node);
    return t;
}

Type* type_check_is(Typer_Context* ctx, AST* node) {
    type_check_node(ctx, node->Is.body);
    type_check_node(ctx, node->Is.node);
    UNFINISHED;
    return NULL;
}
