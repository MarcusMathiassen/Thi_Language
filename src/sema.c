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

//------------------------------------------------------------------------------
//                              sema.c
//------------------------------------------------------------------------------

#include "sema.h"
#include "ast.h"
#include "constants.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "type.h"
#include "utility.h" // tassert
#include <string.h> // strcmp

#define DEBUG_START                                                                                             \
    tassert(ctx && node, "%zu, %zu", ctx, node);                                                                \
    info("%s: %s", give_unique_color(ast_kind_to_str(node->kind)), wrap_with_colored_parens(ast_to_str(node))); \

#define SCOPE_START stack_push(((Sema_Context*)ctx)->scopes, make_list())
#define SCOPE_END stack_pop(((Sema_Context*)ctx)->scopes)
#define SCOPE_ADD(x) list_append(stack_peek(((Sema_Context*)ctx)->scopes), x)

AST* get_symbol_in_scope(Sema_Context* ctx, char* name);
void add_node_to_scope(Sema_Context* ctx, AST* node);
void add_all_decls_in_module(Sema_Context* ctx, AST* node);

Sema_Context make_sema_context() {
   Sema_Context ctx;
    ctx.scopes = make_stack();
    ctx.module = NULL;
    ctx.state = STATE_SEMA;
    ctx.current_function = NULL;
    ctx.symbols = make_map();
    stack_push(ctx.scopes, make_list());
    return ctx;
}

// @Hotpath @Recursive
inline static Type* sema(Sema_Context* ctx, AST* node) {
    if (!node) return NULL;
    DEBUG_START;
    AST_Kind kind = node->kind;
    State_Kind state = ctx->state;
    ast_callback func = (*ast_transitions[kind][state]);
    tassert(func, "missing callback for %s on %s", kind, state);
    Type* result_t =  (*func)(ctx, node);
    result_t = result_t ? result_t : make_type_void();
    info("%s: %s -> SEMA REPLACED TYPE %s WITH %s", give_unique_color(ast_kind_to_str(node->kind)), ast_to_str(node), ucolor(type_to_str(node->type)), ucolor(type_to_str(result_t)));
    node->type = result_t;
    return result_t;
}

void* sema_comment(void* ctx, AST* node) {
    return NULL;
}

void* sema_nop(void* ctx, AST* node) {
    return NULL;
}

void* sema_space_separated_identifier_list(void* ctx, AST* node) {
    LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
        sema(ctx, it->data);
    }
    return NULL;
}

void* sema_comma_separated_list(void* ctx, AST* node) {
    LIST_FOREACH(node->Comma_Separated_List.nodes) {
        sema(ctx, it->data);
    }
    return NULL;
}

void* sema_module(void* ctx, AST* node) {
    ((Sema_Context*)ctx)->module = node;
    // To support unordered decls we need to find all before we jump into scopes.
    // So we do a preliminary pass to find all decls and add them to the scope.
    add_all_decls_in_module(ctx, node);
    LIST_FOREACH(node->Module.top_level) {
        sema(ctx, it->data);
    }
    return NULL;
}

void* sema_is(void* ctx, AST* node) {
    sema(ctx, node->Is.node);
    return  sema(ctx, node->Is.body);
}

void* sema_fallthrough(void* ctx, AST* node) {
    return NULL;
}

void* sema_var_args(void* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

void* sema_extern(void* ctx, AST* node) {
    add_node_to_scope(ctx, node);
    return node->type;
}

void* sema_load(void* ctx, AST* node) {
    return sema(ctx, node->Load.module);
}

void* sema_link(void* ctx, AST* node) {
    return NULL;
}

void* sema_note(void* ctx, AST* node) {
    sema(ctx, node->Note.node);
    s64 val = node->Note.node->Int.val;
    AST* arg = get_arg_from_func(((Sema_Context*)ctx)->current_function, val - 1);
    ast_replace(node, make_ast_ident(arg->loc_info, get_ast_name(arg)));
    return arg->type;
}

void* sema_int(void* ctx, AST* node) {
    return node->type;
}

void* sema_float(void* ctx, AST* node) {
    return node->type;
}

void* sema_string(void* ctx, AST* node) {
    return node->type;
}

void* sema_char(void* ctx, AST* node) {
    return node->type;
}

void* sema_ident(void* ctx, AST* node) {
    AST* var = get_symbol_in_scope(ctx, node->Ident.name);
    if (!var) {
        error("[%s:%s] undefined identifier %s", get_ast_name(((Sema_Context*)ctx)->module), get_ast_loc_str(node), ucolor(ast_to_str(node)));
    }
    // Since the variable must have been defined before this. It must also have a type.
    xassert(var->type);
    return var->type;
}

void* sema_call(void* ctx, AST* node) {
    char* callee = node->Call.callee;
    AST* callee_f = get_symbol_in_scope(ctx, callee);
    if (!callee_f) error("no function in scope with name %s", ucolor(callee));
    LIST_FOREACH(node->Call.args) {
        AST* arg = it->data;
        sema(ctx, arg);
    }
    Type* callee_t = callee_f->type;
    callee_t->Function.return_type->flags = callee_t->flags; // @HACK
    return callee_t->Function.return_type;
}

void* sema_unary(void* ctx, AST* node) {
    AST* operand = node->Unary.operand;
    Type* result_t = sema(ctx, operand);
    xassert(operand->type);
    Token_Kind op = node->Unary.op;
    switch (op) {
    default: break;
    case THI_SYNTAX_POINTER: result_t = get_underlying_type_if_any(result_t); break;
    case THI_SYNTAX_ADDRESS: result_t = make_type_pointer(result_t);          break;
    }
    return result_t;
}

void* sema_binary(void* ctx, AST* node) {
    Token_Kind op = node->Binary.op;
    AST* rhs = node->Binary.rhs;
    AST* lhs = node->Binary.lhs;
    sema(ctx, rhs);
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
    return !replaced ? sema(ctx, lhs) : rhs->type;
}

void* sema_grouping(void* ctx, AST* node) {
    AST* group = node->Grouping.node;
    return sema(ctx, group);;
}

void* sema_subscript(void* ctx, AST* node) {
    AST* load = node->Subscript.load;
    AST* sub = node->Subscript.sub;
    Type* result_t = sema(ctx, load);
    sema(ctx, sub);
    return result_t;
}

void* sema_field_access(void* ctx, AST* node) {
    AST* load = node->Field_Access.load;
    char* field_name = node->Field_Access.field;
    Type* result_t = sema(ctx, load);
    switch (load->type->kind) {
        ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
    case TYPE_STRUCT: {
        LIST_FOREACH(load->type->Struct.members) {
            Type_Name_Pair* mem = it->data;
            info_no_newline("on %s ", mem->name);
            if (strcmp(mem->name, field_name) == 0) {
                info_no_newline("FOUND -> %s of %s\n", mem->name, type_to_str(mem->type));
                result_t = mem->type;
                break;
            }
        }
        break;
    }
    }
    return result_t;
}

void* sema_as(void* ctx, AST* node) {
    sema(ctx, node->As.node);
    return sema(ctx, node->As.type_node);
}

void* sema_block(void* ctx, AST* node) {

    Type* result_t = NULL;
    
    SCOPE_START;
    List* stmts = node->Block.stmts;

    List* returned_nodes = make_list();

    LIST_FOREACH(stmts) {
        AST* stmt = it->data; 
        sema(ctx, stmt);

        switch(stmt->kind) {
        default: break;
        case AST_RETURN: {
            list_append(returned_nodes, stmt);
        } break;

        case AST_FUNCTION: {
            xassert(((Sema_Context*)ctx)->module);
            list_prepend(((Sema_Context*)ctx)->module->Module.top_level, stmt);
            list_remove(stmts, it);
        } break;

        // case AST_EXTERN: {
        //     xassert(ctx->module);
        //     list_prepend(ctx->module->Module.top_level, stmt);
        //     list_remove(stmts, it);
        // } break;
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
                error("[%s] Type_Error. Differing return types in block.\n%s <- %s\n!=\n%s <- %s", LOCATION_OF(((Sema_Context*)ctx)->module, node), type_to_str(a_t), ast_to_str(a), type_to_str(b_t), ast_to_str(b));
            }
        }
        result_t = a_t;
    }

    SCOPE_END;
    return result_t ? result_t : make_type_void();
}

void* sema_struct(void* ctx, AST* node) {
    return node->type;
}

void* sema_enum(void* ctx, AST* node) {
    return node->type;
}

void* sema_function(void* ctx, AST* node) {
    add_node_to_scope(ctx, node);
    ((Sema_Context*)ctx)->current_function = node;
    SCOPE_START;
    LIST_FOREACH(node->Function.parameters) {
        sema(ctx, it->data);
    }
    sema(ctx, node->Function.body);
    LIST_FOREACH(node->Function.defers) {
        sema(ctx, it->data);
    }
    SCOPE_END;
    return node->type;
}

void* sema_variable_decl(void* ctx, AST* node) {
    if (node->Variable_Decl.value && (node->Variable_Decl.value->kind == AST_IDENT)) {
        AST* s = get_symbol_in_scope(ctx, node->Variable_Decl.value->Ident.name);
        node->Variable_Decl.value = s->Variable_Decl.value;
    }
    add_node_to_scope(ctx, node);
    sema(ctx, node->Variable_Decl.value);
    return node->Variable_Decl.value ? node->Variable_Decl.value->type : node->type;
}

void* sema_if(void* ctx, AST* node) {
    sema(ctx, node->If.cond);
    sema(ctx, node->If.then_block);
    sema(ctx, node->If.else_block);
    return NULL;
}

void* sema_for(void* ctx, AST* node) {
    SCOPE_START;
    sema(ctx, node->For.init);
    sema(ctx, node->For.cond);
    sema(ctx, node->For.step);
    sema(ctx, node->For.then_block);
    SCOPE_END;
    return NULL;
}

void* sema_while(void* ctx, AST* node) {
    sema(ctx, node->While.cond);
    sema(ctx, node->While.then_block);
    return NULL;
}

void* sema_return(void* ctx, AST* node) {
    return sema(ctx, node->Return.node);
}

void* sema_defer(void* ctx, AST* node) {
    return sema(ctx, node->Defer.node);
}

void* sema_break(void* ctx, AST* node) {
    return NULL;
}

void* sema_continue(void* ctx, AST* node) {
    return NULL;
}

void* sema_typeof(void* ctx, AST* node) {
    sema(ctx, node->Typeof.node);
    return make_type_pointer(make_type_int(1, true));
}

void* sema_sizeof(void* ctx, AST* node) {
    sema(ctx, node->Sizeof.node);
    return make_type_int(DEFAULT_INT_BYTE_SIZE, false);;
}

void* sema_switch(void* ctx, AST* node) {
    sema(ctx, node->Switch.cond);
    Type* result_t = sema(ctx, node->Switch.cases);
    sema(ctx, node->Switch.default_case);
    return result_t;
}

void* sema_post_inc_or_dec(void* ctx, AST* node) {
    return sema(ctx, node->Post_Inc_or_Dec.node);;
}

void* sema_literal(void* ctx, AST* node) {
    return node->type;
}

void* sema_asm(void* ctx, AST* node) {
    return sema(ctx, node->Asm.block);
}

//------------------------------------------------------------------------------
//                              Sema Helpers
//------------------------------------------------------------------------------

AST* get_symbol_in_scope(Sema_Context* ctx, char* name) {
    tassert(ctx && name, "%zu, %zu", ctx, name);
    info("looking for %s", ucolor(name));
    STACK_FOREACH(((Sema_Context*)ctx)->scopes) {
        List* symbols = it->data;
        LIST_FOREACH_REVERSE(symbols) {
            AST* v = it->data;
            if (strcmp(get_ast_name(v), name) == 0) 
                return v;
        }
    }
    return NULL;
}


void add_node_to_scope(Sema_Context* ctx, AST* node) {
    tassert(ctx && node, "%zu, %zu", ctx, node);
    SCOPE_ADD(node);
    info("added %s to scope.", ucolor(get_ast_name(node)));
}

void add_all_decls_in_module(Sema_Context* ctx, AST* node) {
    tassert(ctx && node, "%zu, %zu", ctx, node);
    xassert(node->kind == AST_MODULE);
    info("add_all_decls_in_module: %s", get_ast_name(node));
    List* decls = node->Module.top_level;
    LIST_FOREACH(decls) {
        AST* decl = it->data;
        switch (decl->kind) {
        default: error("[%s:%s] illegal top level construct %s", get_ast_name(((Sema_Context*)ctx)->module), get_ast_loc_str(decl), ucolor(ast_to_str(decl)));
        case AST_COMMENT: break;
        case AST_NOP:     break;
        case AST_ASM:     break;
        case AST_LINK:    break;
        case AST_VARIABLE_DECL:
            add_node_to_scope(ctx, decl);
            break;
        case AST_MODULE:
            ((Sema_Context*)ctx)->module = decl;
            add_all_decls_in_module(ctx, decl);
            break;
        case AST_LOAD:
            add_all_decls_in_module(ctx, decl->Load.module);
            break;
        case AST_BINARY: {
            sema(ctx, decl);
            if (decl->kind != AST_VARIABLE_DECL) {
                error("[%s:%s] redeclaration of global variable %s", get_ast_name(((Sema_Context*)ctx)->module), get_ast_loc_str(decl), ucolor(ast_to_str(decl)));
            }
            decl->flags |= AST_FLAG_GLOBAL_VARIABLE;
            add_node_to_scope(ctx, decl);
            break;
        }
        case AST_EXTERN:        // fallthrough
        case AST_ENUM:          // fallthrough
        case AST_STRUCT:        // fallthrough
        case AST_FUNCTION:
            add_node_to_scope(ctx, decl);
            break;
        }
    }
}
