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

#include "sema.h"
#include "ast.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "constants.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert
#include <string.h> // strcmp

#define DEBUG_START                                                                                             \
    assert(node); \
    info("%s: %s", give_unique_color(ast_kind_to_str(node->kind)), wrap_with_colored_parens(ast_to_str(NULL, node)));

typedef struct {
    AST*  module;
    Stack* scopes;
} Sema_Context;

void sema_check_node(Sema_Context* ctx, AST* node);

void semantic_analysis(AST* ast) {
    info("Semantic Analysis...");
    Sema_Context ctx;
    ctx.scopes = make_stack();
    stack_push(ctx.scopes, make_list());
    sema_check_node(&ctx, ast);
}

static AST* get_variable_in_scope(Sema_Context* ctx, char* name) {
    assert(ctx);
    assert(name);
    List* variables = stack_peek(ctx->scopes);
    LIST_FOREACH(variables) {
        AST* v = it->data;
        if (v->Variable_Decl.name == name) return v;
    }
    return NULL;
}

#define SCOPE_START stack_push(ctx->scopes, stack_peek(ctx->scopes));
#define SCOPE_END stack_pop(ctx->scopes);

void sema_check_node(Sema_Context* ctx, AST* node) {
    assert(ctx);
    if (!node) return;
    DEBUG_START;
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
        case AST_SPACE_SEPARATED_IDENTIFIER_LIST:
        LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
            sema_check_node(ctx, it->data);
        }
        break;
    case AST_COMMA_SEPARATED_LIST:
        LIST_FOREACH(node->Comma_Separated_List.nodes) {
            sema_check_node(ctx, it->data);
        }
        break;
    // clang-format off
    case AST_MODULE: ctx->module = node; sema_check_node(ctx, node->Module.top_level); break;
    case AST_TYPEOF: sema_check_node(ctx, node->Typeof.node);      break;
    case AST_SIZEOF: sema_check_node(ctx, node->Sizeof.node);      break;
    case AST_NOTE:   sema_check_node(ctx, node->Note.node);        break;
    
    case AST_FALLTHROUGH: break;
    case AST_LOAD:        break;
    case AST_LINK:        break;
    case AST_INT:         break;
    case AST_FLOAT:       break;
    case AST_STRING:      break;
    case AST_IDENT: {
        // AST* var = get_variable_in_scope(ctx, node->Ident.name);
        // if (!var) {
        //     error("[%s:%d:%d] undefined identifier %s", ctx->module->Module.name, node->loc_info.line_pos, node->loc_info.col_pos, ast_to_str(NULL, node));
        // }
        // break;
    }
    case AST_STRUCT:      break;
        // clang-format on

    case AST_UNARY:
        sema_check_node(ctx, node->Unary.operand);
        break;
    case AST_BINARY: {
        sema_check_node(ctx, node->Binary.lhs);
        sema_check_node(ctx, node->Binary.rhs);
        AST* rhs = node->Binary.rhs;
        AST* lhs = node->Binary.lhs;
        Token_Kind op = node->Binary.op;
        if (op == THI_SYNTAX_ASSIGNMENT && lhs->kind == AST_IDENT) {
            // Look for it in the current scope and any parent scope.
            AST* var = get_variable_in_scope(ctx, lhs->Ident.name);
            if (!var) {
                ast_replace(node, make_ast_variable_decl(lhs->loc_info, lhs->Ident.name, NULL, rhs));
            }
        }
        break;
    }
    case AST_VARIABLE_DECL:
        list_append(stack_peek(ctx->scopes), node);
        sema_check_node(ctx, node->Variable_Decl.value);
        break;
    case AST_CONSTANT_DECL:
        list_append(stack_peek(ctx->scopes), node);
        sema_check_node(ctx, node->Constant_Decl.value);
        break;
    case AST_ENUM: break;
    case AST_GROUPING:
        sema_check_node(ctx, node->Grouping.node);
        break;
    case AST_SUBSCRIPT:
        sema_check_node(ctx, node->Subscript.load);
        sema_check_node(ctx, node->Subscript.sub);
        break;
    case AST_FIELD_ACCESS:
        sema_check_node(ctx, node->Field_Access.load);
        break;
    case AST_IF:
        sema_check_node(ctx, node->If.cond);
        sema_check_node(ctx, node->If.then_block);
        sema_check_node(ctx, node->If.else_block);
        break;
    case AST_FOR:
        SCOPE_START;
        sema_check_node(ctx, node->For.init);
        sema_check_node(ctx, node->For.cond);
        sema_check_node(ctx, node->For.step);
        sema_check_node(ctx, node->For.then_block);
        SCOPE_END;
        break;
    case AST_WHILE:
        sema_check_node(ctx, node->While.cond);
        sema_check_node(ctx, node->While.then_block);
        break;
    case AST_RETURN:
        sema_check_node(ctx, node->Return.node);
        break;
    case AST_DEFER:
        sema_check_node(ctx, node->Defer.node);
        break;
    case AST_BREAK:
        sema_check_node(ctx, node->Break.node);
        break;
    case AST_CONTINUE:
        sema_check_node(ctx, node->Continue.node);
        break;
    case AST_AS:
        sema_check_node(ctx, node->As.node);
        sema_check_node(ctx, node->As.type_node);
        break;
    case AST_IS:
        sema_check_node(ctx, node->Is.node);
        sema_check_node(ctx, node->Is.body);
        break;
    case AST_SWITCH:
        sema_check_node(ctx, node->Switch.cond);
        sema_check_node(ctx, node->Switch.cases);
        sema_check_node(ctx, node->Switch.default_case);
        break;
    case AST_EXTERN:
        LIST_FOREACH(node->Extern.type->Function.parameters) {
            sema_check_node(ctx, it->data);
        }
        break;
    case AST_FUNCTION:
        sema_check_node(ctx, node->Function.body);
        LIST_FOREACH(node->Function.type->Function.parameters) {
            sema_check_node(ctx, it->data);
        }
        LIST_FOREACH(node->Function.defers) {
            sema_check_node(ctx, it->data);
        }
        break;
    case AST_CALL:
        LIST_FOREACH(node->Call.args) {
            sema_check_node(ctx, it->data);
        }
        break;
    case AST_BLOCK:
        SCOPE_START;
        LIST_FOREACH(node->Block.stmts) {
            sema_check_node(ctx, it->data);
        }
        SCOPE_END;
        break;
    }
}
