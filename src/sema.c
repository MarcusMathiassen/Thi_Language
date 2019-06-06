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

typedef struct {
    AST* module;
    Map* symbols;
    Stack* scopes;
    AST* current_function;
} Sema_Context;

#define SCOPE_START stack_push(ctx->scopes, make_list())
#define SCOPE_END stack_pop(ctx->scopes)
#define SCOPE_ADD(x) list_append(stack_peek(ctx->scopes), x)

static AST* get_symbol_in_scope(Sema_Context* ctx, char* name);
static void add_node_to_scope(Sema_Context* ctx, AST* node);
static void add_all_decls_in_module(Sema_Context* ctx, AST* node);

static Type*  _sema                                 (Sema_Context* ctx, AST* node);
static Type*  sema_comment                          (Sema_Context* ctx, AST* node);
static Type*  sema_nop                              (Sema_Context* ctx, AST* node);
static Type*  sema_space_separated_identifier_list  (Sema_Context* ctx, AST* node);
static Type*  sema_comma_separated_list             (Sema_Context* ctx, AST* node);
static Type*  sema_module                           (Sema_Context* ctx, AST* node);
static Type*  sema_is                               (Sema_Context* ctx, AST* node);
static Type*  sema_fallthrough                      (Sema_Context* ctx, AST* node);
static Type*  sema_var_args                         (Sema_Context* ctx, AST* node);
static Type*  sema_extern                           (Sema_Context* ctx, AST* node);
static Type*  sema_load                             (Sema_Context* ctx, AST* node);
static Type*  sema_link                             (Sema_Context* ctx, AST* node);
static Type*  sema_note                             (Sema_Context* ctx, AST* node);
static Type*  sema_int                              (Sema_Context* ctx, AST* node);
static Type*  sema_float                            (Sema_Context* ctx, AST* node);
static Type*  sema_string                           (Sema_Context* ctx, AST* node);
static Type*  sema_char                             (Sema_Context* ctx, AST* node);
static Type*  sema_ident                            (Sema_Context* ctx, AST* node);
static Type*  sema_call                             (Sema_Context* ctx, AST* node);
static Type*  sema_unary                            (Sema_Context* ctx, AST* node);
static Type*  sema_binary                           (Sema_Context* ctx, AST* node);
static Type*  sema_grouping                         (Sema_Context* ctx, AST* node);
static Type*  sema_subscript                        (Sema_Context* ctx, AST* node);
static Type*  sema_field_access                     (Sema_Context* ctx, AST* node);
static Type*  sema_as                               (Sema_Context* ctx, AST* node);
static Type*  sema_block                            (Sema_Context* ctx, AST* node);
static Type*  sema_struct                           (Sema_Context* ctx, AST* node);
static Type*  sema_enum                             (Sema_Context* ctx, AST* node);
static Type*  sema_function                         (Sema_Context* ctx, AST* node);
static Type*  sema_variable_decl                    (Sema_Context* ctx, AST* node);
static Type*  sema_if                               (Sema_Context* ctx, AST* node);
static Type*  sema_for                              (Sema_Context* ctx, AST* node);
static Type*  sema_while                            (Sema_Context* ctx, AST* node);
static Type*  sema_return                           (Sema_Context* ctx, AST* node);
static Type*  sema_defer                            (Sema_Context* ctx, AST* node);
static Type*  sema_break                            (Sema_Context* ctx, AST* node);
static Type*  sema_continue                         (Sema_Context* ctx, AST* node);
static Type*  sema_typeof                           (Sema_Context* ctx, AST* node);
static Type*  sema_sizeof                           (Sema_Context* ctx, AST* node);
static Type*  sema_switch                           (Sema_Context* ctx, AST* node);
static Type*  sema_post_inc_or_dec                  (Sema_Context* ctx, AST* node);

Type* sema(AST* node) {
    xassert(node);
    Sema_Context ctx;
    ctx.scopes = make_stack();
    ctx.module = NULL;
    ctx.current_function = NULL;
    ctx.symbols = make_map();
    stack_push(ctx.scopes, make_list());
    return _sema(&ctx, node);
}

static Type* _sema(Sema_Context* ctx, AST* node) {
    if (!node) return NULL;
    DEBUG_START;
    AST_Kind kind = node->kind;
    TASSERT_KIND_IN_RANGE(AST, kind);
    Type* result_t = NULL;
    switch (kind) {
    ERROR_UNHANDLED_AST_KIND(kind);
    case AST_COMMENT:                           result_t = sema_comment                         (ctx, node); break;
    case AST_NOP:                               result_t = sema_nop                             (ctx, node); break;
    case AST_SPACE_SEPARATED_IDENTIFIER_LIST:   result_t = sema_space_separated_identifier_list (ctx, node); break;
    case AST_COMMA_SEPARATED_LIST:              result_t = sema_comma_separated_list            (ctx, node); break;
    case AST_MODULE:                            result_t = sema_module                          (ctx, node); break;
    case AST_IS:                                result_t = sema_is                              (ctx, node); break;
    case AST_FALLTHROUGH:                       result_t = sema_fallthrough                     (ctx, node); break;
    case AST_VAR_ARGS:                          result_t = sema_var_args                        (ctx, node); break;
    case AST_EXTERN:                            result_t = sema_extern                          (ctx, node); break;
    case AST_LOAD:                              result_t = sema_load                            (ctx, node); break;
    case AST_LINK:                              result_t = sema_link                            (ctx, node); break;
    case AST_NOTE:                              result_t = sema_note                            (ctx, node); break;
    case AST_INT:                               result_t = sema_int                             (ctx, node); break;
    case AST_FLOAT:                             result_t = sema_float                           (ctx, node); break;
    case AST_STRING:                            result_t = sema_string                          (ctx, node); break;
    case AST_CHAR:                              result_t = sema_char                            (ctx, node); break;
    case AST_IDENT:                             result_t = sema_ident                           (ctx, node); break;
    case AST_CALL:                              result_t = sema_call                            (ctx, node); break;
    case AST_UNARY:                             result_t = sema_unary                           (ctx, node); break;
    case AST_BINARY:                            result_t = sema_binary                          (ctx, node); break;
    case AST_GROUPING:                          result_t = sema_grouping                        (ctx, node); break;
    case AST_SUBSCRIPT:                         result_t = sema_subscript                       (ctx, node); break;
    case AST_FIELD_ACCESS:                      result_t = sema_field_access                    (ctx, node); break;
    case AST_AS:                                result_t = sema_as                              (ctx, node); break;
    case AST_BLOCK:                             result_t = sema_block                           (ctx, node); break;
    case AST_STRUCT:                            result_t = sema_struct                          (ctx, node); break;
    case AST_ENUM:                              result_t = sema_enum                            (ctx, node); break;
    case AST_FUNCTION:                          result_t = sema_function                        (ctx, node); break;
    case AST_VARIABLE_DECL:                     result_t = sema_variable_decl                   (ctx, node); break;
    case AST_IF:                                result_t = sema_if                              (ctx, node); break;
    case AST_FOR:                               result_t = sema_for                             (ctx, node); break;
    case AST_WHILE:                             result_t = sema_while                           (ctx, node); break;
    case AST_RETURN:                            result_t = sema_return                          (ctx, node); break;
    case AST_DEFER:                             result_t = sema_defer                           (ctx, node); break;
    case AST_BREAK:                             result_t = sema_break                           (ctx, node); break;
    case AST_CONTINUE:                          result_t = sema_continue                        (ctx, node); break;
    case AST_TYPEOF:                            result_t = sema_typeof                          (ctx, node); break;
    case AST_SIZEOF:                            result_t = sema_sizeof                          (ctx, node); break;
    case AST_SWITCH:                            result_t = sema_switch                          (ctx, node); break;
    case AST_POST_INC_OR_DEC:                   result_t = sema_post_inc_or_dec                 (ctx, node); break;
    }
    result_t = result_t ? result_t : make_type_void();
    node->type = result_t;
    info("%s: %s -> SEMA REPLACED TYPE %s WITH %s", give_unique_color(ast_kind_to_str(node->kind)), ast_to_str(node), ucolor(type_to_str(node->type)), ucolor(type_to_str(result_t)));
    return result_t;
}

static Type* sema_comment(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_nop(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_space_separated_identifier_list(Sema_Context* ctx, AST* node) {
    LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
        _sema(ctx, it->data);
    }
    return NULL;
}

static Type* sema_comma_separated_list(Sema_Context* ctx, AST* node) {
    LIST_FOREACH(node->Comma_Separated_List.nodes) {
        _sema(ctx, it->data);
    }
    return NULL;
}

static Type* sema_module(Sema_Context* ctx, AST* node) {
    ctx->module = node;
    // To support unordered decls we need to find all before we jump into scopes.
    // So we do a preliminary pass to find all decls and add them to the scope.
    add_all_decls_in_module(ctx, node);
    LIST_FOREACH(node->Module.top_level) {
        _sema(ctx, it->data);
    }
    return NULL;
}

static Type* sema_is(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->Is.node);
    return  _sema(ctx, node->Is.body);
}

static Type* sema_fallthrough(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_var_args(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

static Type* sema_extern(Sema_Context* ctx, AST* node) {
    add_node_to_scope(ctx, node);
    return node->type;
}

static Type* sema_load(Sema_Context* ctx, AST* node) {
    return _sema(ctx, node->Load.module);
}

static Type* sema_link(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_note(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->Note.node);
    s64 val = node->Note.node->Int.val;
    AST* arg = get_arg_from_func(ctx->current_function, val - 1);
    ast_replace(node, make_ast_ident(arg->loc_info, get_ast_name(arg)));
    return arg->type;
}

static Type* sema_int(Sema_Context* ctx, AST* node) {
    return node->type;
}

static Type* sema_float(Sema_Context* ctx, AST* node) {
    return node->type;
}

static Type* sema_string(Sema_Context* ctx, AST* node) {
    return node->type;
}

static Type* sema_char(Sema_Context* ctx, AST* node) {
    return node->type;
}

static Type* sema_ident(Sema_Context* ctx, AST* node) {
    AST* var = get_symbol_in_scope(ctx, node->Ident.name);
    if (!var) {
        error("[%s:%s] undefined identifier %s", get_ast_name(ctx->module), get_ast_loc_str(node), ucolor(ast_to_str(node)));
    }
    // Since the variable must have been defined before this. It must also have a type.
    xassert(var->type);
    return var->type;
}

static Type* sema_call(Sema_Context* ctx, AST* node) {
    char* callee = node->Call.callee;
    AST* callee_f = get_symbol_in_scope(ctx, callee);
    if (!callee_f) error("no function in scope with name %s", ucolor(callee));
    LIST_FOREACH(node->Call.args) {
        AST* arg = it->data;
        _sema(ctx, arg);
    }
    Type* callee_t = callee_f->type;
    callee_t->Function.return_type->flags = callee_t->flags; // @HACK
    return callee_t->Function.return_type;
}

static Type* sema_unary(Sema_Context* ctx, AST* node) {
    AST* operand = node->Unary.operand;
    Type* result_t = _sema(ctx, operand);
    xassert(operand->type);
    Token_Kind op = node->Unary.op;
    switch (op) {
    default: break;
    case THI_SYNTAX_POINTER:
        result_t = result_t->Pointer.pointee;
        break;
    case THI_SYNTAX_ADDRESS:
        result_t = make_type_pointer(result_t);
        break;
    }
    return result_t;
}

static Type* sema_binary(Sema_Context* ctx, AST* node) {
    Token_Kind op = node->Binary.op;
    AST* rhs = node->Binary.rhs;
    AST* lhs = node->Binary.lhs;
    _sema(ctx, rhs);
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
    return !replaced ? _sema(ctx, lhs) : rhs->type;
}

static Type* sema_grouping(Sema_Context* ctx, AST* node) {
    AST* group = node->Grouping.node;
    return _sema(ctx, group);;
}

static Type* sema_subscript(Sema_Context* ctx, AST* node) {
    AST* load = node->Subscript.load;
    AST* sub = node->Subscript.sub;
    Type* result_t = _sema(ctx, load);
    _sema(ctx, sub);
    return result_t;
}

static Type* sema_field_access(Sema_Context* ctx, AST* node) {
    AST* load = node->Field_Access.load;
    char* field_name = node->Field_Access.field;
    Type* result_t = _sema(ctx, load);
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

static Type* sema_as(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->As.node);
    return _sema(ctx, node->As.type_node);
}

static Type* sema_block(Sema_Context* ctx, AST* node) {

    Type* result_t = NULL;
    
    SCOPE_START;
    List* stmts = node->Block.stmts;

    LIST_FOREACH(stmts) {
        _sema(ctx, it->data);
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
        result_t = a_t;
    }

    SCOPE_END;
    return result_t ? result_t : make_type_void();
}

static Type* sema_struct(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_enum(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_function(Sema_Context* ctx, AST* node) {
    add_node_to_scope(ctx, node);
    ctx->current_function = node;
    SCOPE_START;
    LIST_FOREACH(node->Function.parameters) {
        _sema(ctx, it->data);
    }
    _sema(ctx, node->Function.body);
    LIST_FOREACH(node->Function.defers) {
        _sema(ctx, it->data);
    }
    SCOPE_END;
    return node->type;
}

static Type* sema_variable_decl(Sema_Context* ctx, AST* node) {
    if (node->Variable_Decl.value && (node->Variable_Decl.value->kind == AST_IDENT)) {
        AST* s = get_symbol_in_scope(ctx, node->Variable_Decl.value->Ident.name);
        node->Variable_Decl.value = s->Variable_Decl.value;
    }
    add_node_to_scope(ctx, node);
    _sema(ctx, node->Variable_Decl.value);
    return node->Variable_Decl.value ? node->Variable_Decl.value->type : node->type;
}

static Type* sema_if(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->If.cond);
    _sema(ctx, node->If.then_block);
    _sema(ctx, node->If.else_block);
    return NULL;
}

static Type* sema_for(Sema_Context* ctx, AST* node) {
    SCOPE_START;
    _sema(ctx, node->For.init);
    _sema(ctx, node->For.cond);
    _sema(ctx, node->For.step);
    _sema(ctx, node->For.then_block);
    SCOPE_END;
    return NULL;
}

static Type* sema_while(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->While.cond);
    _sema(ctx, node->While.then_block);
    return NULL;
}

static Type* sema_return(Sema_Context* ctx, AST* node) {
    return _sema(ctx, node->Return.node);
}

static Type* sema_defer(Sema_Context* ctx, AST* node) {
    return _sema(ctx, node->Defer.node);
}

static Type* sema_break(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_continue(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_typeof(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->Typeof.node);
    return make_type_pointer(make_type_int(1, true));
}

static Type* sema_sizeof(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->Sizeof.node);
    return make_type_int(DEFAULT_INT_BYTE_SIZE, false);;
}

static Type* sema_switch(Sema_Context* ctx, AST* node) {
    _sema(ctx, node->Switch.cond);
    Type* result_t = _sema(ctx, node->Switch.cases);
    _sema(ctx, node->Switch.default_case);
    return result_t;
}

static Type* sema_post_inc_or_dec(Sema_Context* ctx, AST* node) {
    return _sema(ctx, node->Post_Inc_or_Dec.node);;
}

//------------------------------------------------------------------------------
//                              sema Helpers
//------------------------------------------------------------------------------

static AST* get_symbol_in_scope(Sema_Context* ctx, char* name) {
    tassert(ctx && name, "%zu, %zu", ctx, name);
    info("looking for %s", ucolor(name));
    STACK_FOREACH(ctx->scopes) {
        List* symbols = it->data;
        LIST_FOREACH_REVERSE(symbols) {
            AST* v = it->data;
            // @Todo replace this one with a pointer comparison later
            if (strcmp(get_ast_name(v), name) == 0) return v;
        }
    }
    return NULL;
}


static void add_node_to_scope(Sema_Context* ctx, AST* node) {
    tassert(ctx && node, "%zu, %zu", ctx, node);
    SCOPE_ADD(node);
    info("added %s to scope.", ucolor(get_ast_name(node)));
}

static void add_all_decls_in_module(Sema_Context* ctx, AST* node) {
    tassert(ctx && node, "%zu, %zu", ctx, node);
    LIST_FOREACH(node->Module.top_level) {
        AST* decl = it->data;
        switch (decl->kind) {
        default: error("[%s:%s] illegal top level construct %s", get_ast_name(ctx->module), get_ast_loc_str(decl), ucolor(ast_to_str(decl)));
        case AST_COMMENT: break;
        case AST_LINK: break;
        case AST_LOAD:
            add_all_decls_in_module(ctx, decl->Load.module);
            break;
        case AST_BINARY: {
            _sema(ctx, decl);
            if (decl->kind != AST_VARIABLE_DECL) {
                error("[%s:%s] redeclaration of global variable %s", get_ast_name(ctx->module), get_ast_loc_str(decl), ucolor(ast_to_str(decl)));
            }
            decl->flags |= AST_FLAG_GLOBAL_VARIABLE;
            // fallthrough
        }
        case AST_EXTERN:        // fallthrough
        case AST_FUNCTION:      // fallthrough
        case AST_VARIABLE_DECL: // fallthrough
        case AST_ENUM:          // fallthrough
        case AST_STRUCT:
            add_node_to_scope(ctx, decl);
            break;
        }
    }
}
