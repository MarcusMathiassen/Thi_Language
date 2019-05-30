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
#include "constants.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "type.h"
#include "utility.h"
#include <assert.h> // assert
#include <string.h> // strcmp

#define DEBUG_START \
    assert(node);   \
    info("%s: %s", give_unique_color(ast_kind_to_str(node->kind)), wrap_with_colored_parens(ast_to_str(node)));

typedef struct {
    AST* module;
    Map* symbols;
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

static AST* get_symbol_in_scope(Sema_Context* ctx, char* name) {
    assert(ctx);
    assert(name);
    info("looking for %s", ucolor(name));
    STACK_FOREACH(ctx->scopes) {
        List* symbols = it->data;

        // @Debug(marcus): remove this later
        // info_no_newline("..scope change. listing scope.. ");
        // LIST_FOREACH_REVERSE(symbols) {
        // AST* v = it->data;
        // info_no_newline("%s ", ucolor(get_ast_name(v)));
        // }

        // info(".listing done.");
        LIST_FOREACH_REVERSE(symbols) {
            AST* v = it->data;
            // info("..on %s", ucolor(get_ast_name(v)));
            if (strcmp(get_ast_name(v), name) == 0) {
                // info(" <- found.\n");
                return v;
            }
        }
    }
    return NULL;
}

#define SCOPE_START stack_push(ctx->scopes, make_list())
#define SCOPE_END stack_pop(ctx->scopes)

void add_node_to_scope(Sema_Context* ctx, AST* node) {
    list_append(stack_peek(ctx->scopes), node);
    info("added %s to scope.", ucolor(get_ast_name(node)));
}

void add_all_decls_in_module(Sema_Context* ctx, AST* node) {
    LIST_FOREACH(node->Module.top_level) {
        AST* decl = it->data;
        switch (decl->kind) {
        default: error("[%s:%s] illegal top level construct %s", get_ast_name(ctx->module), get_ast_loc_str(decl), ucolor(ast_to_str(decl)));
        case AST_COMMENT: break;
        case AST_LINK: break;
        case AST_LOAD:
            add_all_decls_in_module(ctx, decl->Load.module);
            break;
        case AST_EXTERN:        // fallthrough
        case AST_FUNCTION:      // fallthrough
        case AST_CONSTANT_DECL: // fallthrough
        case AST_VARIABLE_DECL: // fallthrough
        case AST_ENUM:          // fallthrough
        case AST_STRUCT:
            add_node_to_scope(ctx, decl);
            break;
        }
    }
}
void sema_check_node(Sema_Context* ctx, AST* node) {
    assert(ctx);
    if (!node) return;
    DEBUG_START;
    Type* node_t = node->type;
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
    case AST_MODULE:
        ctx->module = node;
        // To support unordered decls we need to find all before we jump into scopes.
        // So we do a preliminary pass to find all decls and add them to the scope.
        add_all_decls_in_module(ctx, node);
        LIST_FOREACH(node->Module.top_level) {
            sema_check_node(ctx, it->data);
        }
        break;

    case AST_TYPEOF:
        sema_check_node(ctx, node->Typeof.node);
        node_t = node->Typeof.node->type;
        break;

    case AST_SIZEOF:
        sema_check_node(ctx, node->Sizeof.node);
        node_t = node->Sizeof.node->type;
        break;
    case AST_NOTE: sema_check_node(ctx, node->Note.node); break;

    case AST_COMMENT: break;
    case AST_NOP: break;
    case AST_FALLTHROUGH: break;
    case AST_LOAD:
        sema_check_node(ctx, node->Load.module);
        break;
    case AST_LINK: break;
    case AST_INT: break;
    case AST_FLOAT: break;
    case AST_STRING: break;
    case AST_CHAR: break;
    case AST_IDENT: {
        AST* var = get_symbol_in_scope(ctx, node->Ident.name);
        if (!var) {
            error("[%s:%s] undefined identifier %s", get_ast_name(ctx->module), get_ast_loc_str(node), ucolor(ast_to_str(node)));
        }
        // Since the variable must have been defined before this. It must also have a type.
        assert(var->type);
        node_t = var->type;
        break;
    }
    case AST_STRUCT:
        break;
    case AST_UNARY: {
        Token_Kind op = node->Unary.op;
        AST* operand = node->Unary.operand;

        sema_check_node(ctx, operand);
        assert(operand->type);

        node_t = operand->type;

        switch (op) {
        default: break;
        case THI_SYNTAX_POINTER:
            node_t = node_t->Pointer.pointee;
            break;
        case THI_SYNTAX_ADDRESS:
            node_t = make_type_pointer(node_t);
            break;
        }
        break;
    }
    case AST_BINARY: {
        Token_Kind op = node->Binary.op;
        AST* rhs = node->Binary.rhs;
        AST* lhs = node->Binary.lhs;
        sema_check_node(ctx, rhs);
        bool replaced = false;
        if (op == THI_SYNTAX_ASSIGNMENT && lhs->kind == AST_IDENT) {
            // Look for it in the current scope and any parent scope.
            AST* var = get_symbol_in_scope(ctx, lhs->Ident.name);
            if (!var) {
                replaced = true;
                ast_replace(node, make_ast_variable_decl(lhs->loc_info, lhs->Ident.name, rhs->type, rhs));
                add_node_to_scope(ctx, node);
            }
        }
        if (!replaced) sema_check_node(ctx, lhs);
        node_t = rhs->type;
        break;
    }
    case AST_VARIABLE_DECL: {
        Type* type = node->Variable_Decl.type;
        AST* value = node->Variable_Decl.value;
        add_node_to_scope(ctx, node);
        sema_check_node(ctx, value);
        node_t = type;
        break;
    }
    case AST_CONSTANT_DECL: {
        AST* value = node->Constant_Decl.value;
        add_node_to_scope(ctx, node);
        sema_check_node(ctx, value);
        node_t = value->type;
        break;
    }
    case AST_ENUM: break;
    case AST_GROUPING: {
        AST* group = node->Grouping.node;
        sema_check_node(ctx, group);
        node_t = group->type;
        break;
    }
    case AST_SUBSCRIPT: {
        AST* load = node->Subscript.load;
        AST* sub = node->Subscript.sub;
        sema_check_node(ctx, load);
        sema_check_node(ctx, sub);
        node_t = get_underlying_type_if_any(load->type);
        break;
    }

    case AST_FIELD_ACCESS: {
        AST* load =  node->Field_Access.load;
        char* field_name = node->Field_Access.field;
        sema_check_node(ctx, load);
        switch (load->type->kind) {
        default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
        case TYPE_STRUCT: {
            LIST_FOREACH(load->type->Struct.members) {
                Type_Name_Pair* mem = it->data;
                info_no_newline("on %s ", mem->name);
                if (strcmp(mem->name, field_name) == 0) {
                    info_no_newline("FOUND -> %s of %s\n", mem->name, type_to_str(mem->type));
                    node_t =  mem->type;
                    break;
                }
            }
            break;
        } 
        }
        break;
    }

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
        // Typeify
        node_t = node->Return.node->type;
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
        add_node_to_scope(ctx, node);
        // @Todo(marcus): we need to check the parameters if they make sense.
        // LIST_FOREACH(node->Extern.type->Function.parameters) {
        //     sema_check_node(ctx, it->data);
        // }
        break;
    case AST_FUNCTION:
        add_node_to_scope(ctx, node);
        SCOPE_START;
        LIST_FOREACH(node->Function.parameters) {
            sema_check_node(ctx, it->data);
        }
        sema_check_node(ctx, node->Function.body);
        LIST_FOREACH(node->Function.defers) {
            sema_check_node(ctx, it->data);
        }
        SCOPE_END;
        // info_no_newline("scopes: %d -> ", ctx->scopes->count);
        // List* vars = (List*)stack_peek(ctx->scopes);
        // LIST_FOREACH_REVERSE(vars) {
        // AST* v = it->data;
        // info_no_newline("%s ", ucolor(v->Variable_Decl.name));
        // }
        // info("");
        break;

    case AST_CALL: {
        char* callee = node->Call.callee;
        AST* callee_f = get_symbol_in_scope(ctx, callee);
        if (!callee_f) error("no function in scope with name %s", ucolor(callee));
        LIST_FOREACH(node->Call.args) {
            AST* arg = it->data;
            sema_check_node(ctx, arg);
        }

        // Typeify
        Type* callee_t = callee_f->type;
        callee_t->Function.return_type->flags = callee_t->flags; // @HACK
        node_t = callee_t->Function.return_type;
        break;
    }

    case AST_BLOCK:
        SCOPE_START;
        List* stmts = node->Block.stmts;

        LIST_FOREACH(stmts) {
            sema_check_node(ctx, it->data);
        }

        List* returned_nodes = make_list();
        LIST_FOREACH(stmts) {
            AST* stmt = it->data;
            if (stmt->kind == AST_RETURN) {
                list_append(returned_nodes, stmt);
            }
        }

        // In case of the block has a return
        if (returned_nodes->count > 0) {

            // ..make sure they are all the same type
            AST* a = list_first(returned_nodes);
            Type* a_t = a->type;
            LIST_FOREACH(returned_nodes) {
                AST* b = it->data;
                Type* b_t = b->type;
                // ..raise an error if not
                if (!is_same_type(a_t, b_t)) {
                    error("[%s] Type_Error. Differing return types in block.\n%s <- %s\n!=\n%s <- %s", LOCATION_OF(ctx->module, node), type_to_str(a_t), ast_to_str(a), type_to_str(b_t), ast_to_str(b));
                }
            }
            node_t = a_t;
        }

        SCOPE_END;
        break;
    }


    // @Debug(marcus): remove this later
    switch (node->kind) {
    case AST_COMMENT:  // fallthrough
    case AST_NOP:      // fallthrough
    case AST_MODULE:   // fallthrough
    case AST_LOAD:     // fallthrough
    case AST_LINK:     // fallthrough
    case AST_BLOCK:    // fallthrough
    case AST_WHILE:    // fallthrough
    case AST_IF:       // fallthrough
    case AST_FOR:      // fallthrough
    case AST_SWITCH:   // fallthrough
    case AST_IS:       // fallthrough
    case AST_DEFER:    // fallthrough
    case AST_BREAK:    // fallthrough
    case AST_CONTINUE: // fallthrough
    case AST_FALLTHROUGH: break;
    default: assert(node_t);
    }
    node->type = node_t;
}
