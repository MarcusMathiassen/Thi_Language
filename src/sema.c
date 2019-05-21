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
    info("%s: %s", give_unique_color(ast_kind_to_str(node->kind)), wrap_with_colored_parens(ast_to_str(node))); \
    assert(node);

typedef struct {
    Map*  symbol_table;
    AST*  module;
} Sema_Context;

// clang-format off
AST* sema_check_node           (Sema_Context* ctx, AST* node);
AST* sema_check_module         (Sema_Context* ctx, AST* node);
AST* sema_check_typeof         (Sema_Context* ctx, AST* node);
AST* sema_check_sizeof         (Sema_Context* ctx, AST* node);
AST* sema_check_switch         (Sema_Context* ctx, AST* node);
AST* sema_check_extern         (Sema_Context* ctx, AST* node);
AST* sema_check_struct         (Sema_Context* ctx, AST* node);
AST* sema_check_enum           (Sema_Context* ctx, AST* node);
AST* sema_check_function       (Sema_Context* ctx, AST* node);
AST* sema_check_note           (Sema_Context* ctx, AST* node);
AST* sema_check_int            (Sema_Context* ctx, AST* node);
AST* sema_check_float          (Sema_Context* ctx, AST* node);
AST* sema_check_string         (Sema_Context* ctx, AST* node);
AST* sema_check_ident          (Sema_Context* ctx, AST* node);
AST* sema_check_call           (Sema_Context* ctx, AST* node);
AST* sema_check_unary          (Sema_Context* ctx, AST* node);
AST* sema_check_binary         (Sema_Context* ctx, AST* node);
AST* sema_check_variable_decl  (Sema_Context* ctx, AST* node);
AST* sema_check_constant_decl  (Sema_Context* ctx, AST* node);
AST* sema_check_block          (Sema_Context* ctx, AST* node);
AST* sema_check_subscript      (Sema_Context* ctx, AST* node);
AST* sema_check_field_access   (Sema_Context* ctx, AST* node);
AST* sema_check_if             (Sema_Context* ctx, AST* node);
AST* sema_check_for            (Sema_Context* ctx, AST* node);
AST* sema_check_while          (Sema_Context* ctx, AST* node);
AST* sema_check_return         (Sema_Context* ctx, AST* node);
AST* sema_check_defer          (Sema_Context* ctx, AST* node);
AST* sema_check_break          (Sema_Context* ctx, AST* node);
AST* sema_check_continue       (Sema_Context* ctx, AST* node);
AST* sema_check_as             (Sema_Context* ctx, AST* node);
AST* sema_check_is             (Sema_Context* ctx, AST* node);
// clang-format on


AST* semantic_analysis(AST* ast) {
    info("Semantic Analysis...");
    Sema_Context ctx;
    sema_check_node(&ctx, ast);
    return ast;
}

AST* sema_check_node(Sema_Context* ctx, AST* node) {
    assert(ctx);
    assert(node);
    DEBUG_START;
    AST* result = NULL;
    // clang-format off
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
    case AST_FALLTHROUGH:    result = NULL;                                                   break;
    case AST_LOAD:           result = NULL;                                                   break;
    case AST_LINK:           result = NULL;                                                   break;
    case AST_TYPEOF:         result = sema_check_typeof        (ctx, node);                   break;
    case AST_SIZEOF:         result = sema_check_sizeof        (ctx, node);                   break;
    case AST_SWITCH:         result = sema_check_switch        (ctx, node);                   break;
    case AST_EXTERN:         result = sema_check_extern        (ctx, node);                   break;
    case AST_STRUCT:         result = sema_check_struct        (ctx, node);                   break;
    case AST_ENUM:           result = sema_check_enum          (ctx, node);                   break;
    case AST_FUNCTION:       result = sema_check_function      (ctx, node);                   break;
    case AST_NOTE:           result = sema_check_note          (ctx, node);                   break;
    case AST_INT:            result = sema_check_int           (ctx, node);                   break;
    case AST_FLOAT:          result = sema_check_float         (ctx, node);                   break;
    case AST_STRING:         result = sema_check_string        (ctx, node);                   break;
    case AST_IDENT:          result = sema_check_ident         (ctx, node);                   break;
    case AST_CALL:           result = sema_check_call          (ctx, node);                   break;
    case AST_UNARY:          result = sema_check_unary         (ctx, node);                   break;
    case AST_BINARY:         result = sema_check_binary        (ctx, node);                   break;
    case AST_VARIABLE_DECL:  result = sema_check_variable_decl (ctx, node);                   break;
    case AST_CONSTANT_DECL:  result = sema_check_constant_decl (ctx, node);                   break;
    case AST_BLOCK:          result = sema_check_block         (ctx, node);                   break;
    case AST_GROUPING:       result = sema_check_node          (ctx, node->Grouping.node);    break;
    case AST_SUBSCRIPT:      result = sema_check_subscript     (ctx, node);                   break;
    case AST_FIELD_ACCESS:   result = sema_check_field_access  (ctx, node);                   break;
    case AST_IF:             result = sema_check_if            (ctx, node);                   break;
    case AST_FOR:            result = sema_check_for           (ctx, node);                   break;
    case AST_WHILE:          result = sema_check_while         (ctx, node);                   break;
    case AST_RETURN:         result = sema_check_return        (ctx, node);                   break;
    case AST_DEFER:          result = sema_check_defer         (ctx, node);                   break;
    case AST_BREAK:          result = sema_check_break         (ctx, node);                   break;
    case AST_CONTINUE:       result = sema_check_continue      (ctx, node);                   break;
    case AST_AS:             result = sema_check_as            (ctx, node);                   break;
    case AST_IS:             result = sema_check_is            (ctx, node);                   break;
    case AST_MODULE:         result = sema_check_module        (ctx, node);                   break;
    }
    // clang-format on
    return result;
}

AST* sema_check_sizeof(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_module(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_typeof(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_switch(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_extern(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_struct(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_enum(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_function(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_note(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_int(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_float(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_string(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_ident(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_call(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_unary(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_binary(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_variable_decl(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_constant_decl(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_block(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_subscript(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_field_access(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_if(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_for(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_while(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_return(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_defer(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_break(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_continue(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
AST* sema_check_as(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}

AST* sema_check_is(Sema_Context* ctx, AST* node) {
    UNFINISHED;
    return NULL;
}
