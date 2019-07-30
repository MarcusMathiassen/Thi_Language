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

#include "list.h"
#include "map.h"
#include "stack.h"
#include "type.h"
#include "utility.h" // tassert
#include <string.h> // strcmp

typedef struct {
    AST* module;
    Map* symbols;
    Stack* scopes;
    AST* current_function;
} Sema_Context;

#define DEBUG_START \
    tassert(ctx && node, "%zu, %zu", ctx, node); \
    debug("%s: %s", give_unique_color(ast_kind_to_str(node->kind)), wrap_with_colored_parens(ast_to_str(node)));

#define SCOPE_START stack_push(((Sema_Context*)ctx)->scopes, make_map())
// #define SCOPE_START stack_push(((Sema_Context*)ctx)->scopes, make_ast_ref_list())
#define SCOPE_END map_destroy(stack_pop(((Sema_Context*)ctx)->scopes))
// #define SCOPE_END ast_ref_list_destroy(stack_pop(((Sema_Context*)ctx)->scopes))
#define SCOPE_ADD(x) map_set(stack_peek(((Sema_Context*)ctx)->scopes), get_ast_name(x), x)
// #define SCOPE_ADD(x) ast_ref_list_append(stack_peek(((Sema_Context*)ctx)->scopes), x)

static Sema_Context make_sema_context(void);
static AST* get_symbol_in_scope(Sema_Context* ctx, char* name);
static void add_node_to_scope(Sema_Context* ctx, AST* node);
static void add_all_decls_in_module(Sema_Context* ctx, AST* node);

static Type* _sema                                 (Sema_Context* ctx,  AST* node);
static Type* sema_comment                          (Sema_Context* ctx,  AST* node);
static Type* sema_nop                              (Sema_Context* ctx,  AST* node);
static Type* sema_space_separated_identifier_list  (Sema_Context* ctx,  AST* node);
static Type* sema_comma_separated_list             (Sema_Context* ctx,  AST* node);
static Type* sema_module                           (Sema_Context* ctx,  AST* node);
static Type* sema_is                               (Sema_Context* ctx,  AST* node);
static Type* sema_fallthrough                      (Sema_Context* ctx,  AST* node);
static Type* sema_var_args                         (Sema_Context* ctx,  AST* node);
static Type* sema_extern                           (Sema_Context* ctx,  AST* node);
static Type* sema_load                             (Sema_Context* ctx,  AST* node);
static Type* sema_link                             (Sema_Context* ctx,  AST* node);
static Type* sema_note                             (Sema_Context* ctx,  AST* node);
static Type* sema_int                              (Sema_Context* ctx,  AST* node);
static Type* sema_float                            (Sema_Context* ctx,  AST* node);
static Type* sema_string                           (Sema_Context* ctx,  AST* node);
static Type* sema_char                             (Sema_Context* ctx,  AST* node);
static Type* sema_ident                            (Sema_Context* ctx,  AST* node);
static Type* sema_call                             (Sema_Context* ctx,  AST* node);
static Type* sema_unary                            (Sema_Context* ctx,  AST* node);
static Type* sema_binary                           (Sema_Context* ctx,  AST* node);
static Type* sema_grouping                         (Sema_Context* ctx,  AST* node);
static Type* sema_subscript                        (Sema_Context* ctx,  AST* node);
static Type* sema_field_access                     (Sema_Context* ctx,  AST* node);
static Type* sema_as                               (Sema_Context* ctx,  AST* node);
static Type* sema_block                            (Sema_Context* ctx,  AST* node);
static Type* sema_struct                           (Sema_Context* ctx,  AST* node);
static Type* sema_enum                             (Sema_Context* ctx,  AST* node);
static Type* sema_function                         (Sema_Context* ctx,  AST* node);
static Type* sema_variable_decl                    (Sema_Context* ctx,  AST* node);
static Type* sema_if                               (Sema_Context* ctx,  AST* node);
static Type* sema_for                              (Sema_Context* ctx,  AST* node);
static Type* sema_while                            (Sema_Context* ctx,  AST* node);
static Type* sema_return                           (Sema_Context* ctx,  AST* node);
static Type* sema_defer                            (Sema_Context* ctx,  AST* node);
static Type* sema_break                            (Sema_Context* ctx,  AST* node);
static Type* sema_continue                         (Sema_Context* ctx,  AST* node);
static Type* sema_typeof                           (Sema_Context* ctx,  AST* node);
static Type* sema_sizeof                           (Sema_Context* ctx,  AST* node);
static Type* sema_switch                           (Sema_Context* ctx,  AST* node);
static Type* sema_post_inc_or_dec                  (Sema_Context* ctx,  AST* node);
static Type* sema_literal                          (Sema_Context* ctx,  AST* node);
static Type* sema_asm                              (Sema_Context* ctx,  AST* node);
static Type* sema_cast                             (Sema_Context* ctx,  AST* node);


static Type* (*sema_transitions[])(Sema_Context*, AST*) = {
    [AST_COMMENT]                         =  sema_comment,
    [AST_NOP]                             =  sema_nop,
    [AST_SPACE_SEPARATED_IDENTIFIER_LIST] =  sema_space_separated_identifier_list,
    [AST_COMMA_SEPARATED_LIST]            =  sema_comma_separated_list,
    [AST_MODULE]                          =  sema_module,
    [AST_IS]                              =  sema_is,
    [AST_FALLTHROUGH]                     =  sema_fallthrough,
    [AST_VAR_ARGS]                        =  sema_var_args,
    [AST_EXTERN]                          =  sema_extern,
    [AST_LOAD]                            =  sema_load,
    [AST_LINK]                            =  sema_link,
    [AST_NOTE]                            =  sema_note,
    [AST_INT]                             =  sema_int,
    [AST_FLOAT]                           =  sema_float,
    [AST_STRING]                          =  sema_string,
    [AST_CHAR]                            =  sema_char,
    [AST_IDENT]                           =  sema_ident,
    [AST_CALL]                            =  sema_call,
    [AST_UNARY]                           =  sema_unary,
    [AST_BINARY]                          =  sema_binary,
    [AST_GROUPING]                        =  sema_grouping,
    [AST_SUBSCRIPT]                       =  sema_subscript,
    [AST_FIELD_ACCESS]                    =  sema_field_access,
    [AST_AS]                              =  sema_as,
    [AST_BLOCK]                           =  sema_block,
    [AST_STRUCT]                          =  sema_struct,
    [AST_ENUM]                            =  sema_enum,
    [AST_FUNCTION]                        =  sema_function,
    [AST_VARIABLE_DECL]                   =  sema_variable_decl,
    [AST_IF]                              =  sema_if,
    [AST_FOR]                             =  sema_for,
    [AST_WHILE]                           =  sema_while,
    [AST_RETURN]                          =  sema_return,
    [AST_DEFER]                           =  sema_defer,
    [AST_BREAK]                           =  sema_break,
    [AST_CONTINUE]                        =  sema_continue,
    [AST_TYPEOF]                          =  sema_typeof,
    [AST_SIZEOF]                          =  sema_sizeof,
    [AST_SWITCH]                          =  sema_switch,
    [AST_POST_INC_OR_DEC]                 =  sema_post_inc_or_dec,
    [AST_LITERAL]                         =  sema_literal,
    [AST_ASM]                             =  sema_asm,
    [AST_CAST]                            =  sema_cast,
};

static Sema_Context make_sema_context() {
   Sema_Context ctx;
    ctx.scopes = make_stack();
    ctx.module = NULL;
    ctx.current_function = NULL;
    ctx.symbols = make_map();
    stack_push(ctx.scopes, make_map());
    return ctx;
}

// @Hotpath @Recursive
Type* sema(AST* node) { 
    push_timer((char*)__func__);
    if (!node) return NULL;
    Sema_Context ctx = make_sema_context();
    Type* t = _sema(&ctx, node);
    pop_timer();
    return t;
}

// @Hotpath @Recursive
static Type* _sema(Sema_Context* ctx, AST* node) {
    if (!node) return NULL;
    DEBUG_START;
    AST_Kind kind = node->kind;
    Type* (*func)(Sema_Context*, AST*) = (*sema_transitions[kind]);
    tassert(func, "sema missing callback for %s", kind);
    Type* result_t =  (*func)(ctx, node);
    result_t = result_t ? result_t : make_type_void();
    debug("%s: %s -> SEMA REPLACED TYPE %s WITH %s", give_unique_color(ast_kind_to_str(node->kind)), ast_to_str(node), ucolor(type_to_str(node->type)), ucolor(type_to_str(result_t)));
    node->type = result_t;
    return result_t;
}

static Type* sema_comment(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_nop(Sema_Context* ctx, AST* node) {
    return NULL;
}

static Type* sema_space_separated_identifier_list(Sema_Context* ctx, AST* node) {
    list_foreach(node->Space_Separated_Identifier_List.identifiers) {
        _sema(ctx, it->data);
    }
    return NULL;
}

static Type* sema_comma_separated_list(Sema_Context* ctx, AST* node) {
    list_foreach(node->Comma_Separated_List.nodes) {
        _sema(ctx, it->data);
    }
    return NULL;
}

static Type* sema_module(Sema_Context* ctx, AST* node) {
    ((Sema_Context*)ctx)->module = node;
    // To support unordered decls we need to find all before we jump into scopes.
    // So we do a preliminary pass to find all decls and add them to the scope.
    add_all_decls_in_module(ctx, node);
    list_foreach(node->Module.top_level) {
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
    AST* arg = get_arg_from_func(((Sema_Context*)ctx)->current_function, val - 1);
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
        error("[%s:%s] undefined identifier %s", get_ast_name(((Sema_Context*)ctx)->module), get_ast_loc_str(node), ucolor(ast_to_str(node)));
    }
    // Since the variable must have been defined before this. It must also have a type.
    xassert(var->type);
    return var->type;
}

static Type* sema_call(Sema_Context* ctx, AST* node) {
    char* callee = node->Call.callee;
    AST* callee_f = get_symbol_in_scope(ctx, callee);
    if (!callee_f) error("no function in scope with name %s", ucolor(callee));
    list_foreach(node->Call.args) {
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
    case THI_SYNTAX_POINTER: result_t = get_underlying_type_if_any(result_t); break;
    case THI_SYNTAX_ADDRESS: result_t = make_type_pointer(result_t);          break;
    }
    return result_t;
}

static Type* sema_binary(Sema_Context* ctx, AST* node) {
    Token_Kind op = node->Binary.op;
    AST* rhs = node->Binary.rhs;
    AST* lhs = node->Binary.lhs;
    _sema(ctx, rhs);
    bool replaced = false;


    switch(op) {
    default: break; //@Audit

    // @Audit, @Document, @Note: duplicate _sema calls. Look up.
    // Comma separated list
    case TOKEN_COMMA: {
        List* expr_list = make_list();
        list_append(expr_list, lhs);
        AST* expr = rhs;
        while(expr->Binary.op == TOKEN_COMMA) {
            
            AST* lhs_e = expr->Binary.lhs;
            AST* rhs_e = expr->Binary.rhs;

            // LHS
            _sema(ctx, lhs_e);
            if (lhs_e->kind == AST_VARIABLE_DECL)
                error("hrtokhrothpk");

            // RHS
            _sema(ctx, rhs_e);

            list_append(expr_list, expr);
            expr = expr->Binary.rhs;
        }
        list_append(expr_list, expr);
        ast_replace(node, make_ast_comma_separated_list(node->loc_info, expr_list));
        return NULL;
    } break;

    case THI_SYNTAX_ASSIGNMENT: {
        if (lhs->kind == AST_IDENT) {
            // Look for it in the current scope and any parent scope.
            AST* var = get_symbol_in_scope(ctx, lhs->Ident.name);
            if (!var) {
                replaced = true;
                ast_replace(node, make_ast_variable_decl(lhs->loc_info, lhs->Ident.name, rhs->type, rhs));
                add_node_to_scope(ctx, node);
            }
        }


        // a, b, c = 1, 2, 3
        // A list equal another list is 
        if (lhs->kind == AST_COMMA_SEPARATED_LIST && rhs->kind == AST_COMMA_SEPARATED_LIST) {

            error("gergerg");
            // First of all, they MUST be the same length. If not, the user has made a mistake.
            if (lhs->Comma_Separated_List.nodes->count != rhs->Comma_Separated_List.nodes->count) {
                error("[%s] %s or %s is missing some elements.", LOCATION_OF(((Sema_Context*)ctx)->module, node), ast_to_str(lhs), ast_to_str(rhs));
            }

            // So for every element in the LHS list..
            list_foreach(lhs->Comma_Separated_List.nodes) {
                // .. is matched with the corrosponding element in the RHS list.
                list_foreach(lhs->Comma_Separated_List.nodes) {

                }
            }
        }

    } break;

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

    _sema(ctx, load);
    _sema(ctx, sub);

    s64 size = get_size_of_underlying_type_if_any(load->type);

    sub = make_ast_binary(node->loc_info, TOKEN_ASTERISK, make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, TYPE_IS_SIGNED)), sub);
    load = make_ast_binary(node->loc_info, TOKEN_PLUS, load, sub);
    load = make_ast_grouping(node->loc_info, load);
    load = make_ast_unary(node->loc_info, THI_SYNTAX_POINTER, load);
    ast_replace(node, load);

    return _sema(ctx, node);
}

static Type* sema_field_access(Sema_Context* ctx, AST* node) {
    AST* load = node->Field_Access.load;
    char* field_name = node->Field_Access.field;
    Type* result_t = _sema(ctx, load);
    switch (result_t->kind) {
    ERROR_UNHANDLED_TYPE_KIND(result_t->kind);
    case TYPE_STRUCT: {
        list_foreach(load->type->Struct.members) {
            Type_Name_Pair* mem = it->data;
            debug_no_newline("on %s ", mem->name);
            if (strcmp(mem->name, field_name) == 0) {
                debug_no_newline("FOUND -> %s of %s\n", mem->name, type_to_str(mem->type));
                result_t = mem->type;
                break;
            }
        }
        break;
    }
    }


    s64 offset_size = get_offset_in_struct_to_field(load->type, field_name);
    AST* offset = make_ast_int(node->loc_info, offset_size, make_type_int(DEFAULT_INT_BYTE_SIZE, 0));

    load = make_ast_unary(node->loc_info, THI_SYNTAX_ADDRESS, load);
    load = make_ast_binary(node->loc_info, TOKEN_PLUS, load, offset);
    load = make_ast_grouping(node->loc_info, load);
    load = make_ast_unary(node->loc_info, THI_SYNTAX_POINTER, load);

    ast_replace(node, load);

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
    List* returned_nodes = make_list();

    list_foreach(stmts) {
        AST* stmt = it->data; 
        _sema(ctx, stmt);

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
        list_foreach(returned_nodes) {
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

static Type* sema_struct(Sema_Context* ctx, AST* node) {
    return node->type;
}

static Type* sema_enum(Sema_Context* ctx, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_ENUM: {
        s64 counter = 0;
        AST* e = node;
        list_foreach(e->Enum.members) {
            AST* m = it->data;
            switch (m->kind) {
                ERROR_UNHANDLED_AST_KIND(m->kind);
            case AST_IDENT:
             {
                Type* int_t = make_type_int(DEFAULT_INT_BYTE_SIZE, false);
                it->data = make_ast_variable_decl(m->loc_info, m->Ident.name, int_t, make_ast_int(m->loc_info, counter, int_t));
                break;
             }
            case AST_VARIABLE_DECL:
                xassert(m->Variable_Decl.value);
                xassert(m->Variable_Decl.value->kind == AST_INT);
                counter = m->Variable_Decl.value->Int.val;
                break;
            }
            ++counter;
        }

    } break;
    }

    return node->type;
}

static Type* sema_function(Sema_Context* ctx, AST* node) {
    add_node_to_scope(ctx, node);
    ((Sema_Context*)ctx)->current_function = node;
    SCOPE_START;
    list_foreach(node->Function.parameters) {
        AST* p = it->data;
        _sema(ctx, p);
    }
    _sema(ctx, node->Function.body);
    list_foreach(node->Function.defers) {
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
    Type *t = _sema(ctx, node->Typeof.node);

    AST* string_value = make_ast_string(node->loc_info, get_type_name(t));
    ast_replace(node, string_value);

    return make_type_pointer(make_type_int(1, TYPE_IS_UNSIGNED));
}

static Type* sema_sizeof(Sema_Context* ctx, AST* node) {
    Type *t = _sema(ctx, node->Sizeof.node);

    s64 size = get_size_of_type(t);
    AST* constant_value = make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, TYPE_IS_SIGNED));
    ast_replace(node, constant_value);

    return t;
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

static Type* sema_literal(Sema_Context* ctx, AST* node) {
    return node->type;
}

static Type* sema_asm(Sema_Context* ctx, AST* node) {
    return _sema(ctx, node->Asm.block);
}

static Type* sema_cast(Sema_Context* ctx, AST* node) {
    AST* desired_type = node->Cast.desired_type;
    AST* operand = node->Cast.node;
    _sema(ctx, operand);
    _sema(ctx, desired_type);
    return desired_type->type;
}

//------------------------------------------------------------------------------
//                              Sema Helpers
//------------------------------------------------------------------------------

AST* get_symbol_in_scope(Sema_Context* ctx, char* name) {
    xassert(ctx && name);
    stack_foreach(ctx->scopes) {
        Map* symbols = it->data;
        AST* v;
        if ((v = map_get(symbols, name))) 
            return v;
    }
    return NULL;
}

static void add_node_to_scope(Sema_Context* ctx, AST* node) {
    tassert(ctx && node, "%zu, %zu", ctx, node);
    SCOPE_ADD(node);
    debug("scope added %s", ucolor(get_ast_name(node)));
}

void add_all_decls_in_module(Sema_Context* ctx, AST* node) {
    tassert(ctx && node, "%zu, %zu", ctx, node);
    xassert(node->kind == AST_MODULE);
    debug("add_all_decls_in_module: %s", get_ast_name(node));
    List* decls = node->Module.top_level;
    list_foreach(decls) {
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
        case AST_LOAD:
            add_all_decls_in_module(ctx, decl->Load.module);
            break;
        case AST_BINARY: {
            _sema(ctx, decl);
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
