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
//                              sema.h
//------------------------------------------------------------------------------

#ifndef SEMA_H
#define SEMA_H

#include "ast.h"
#include "map.h"
#include "stack.h"

typedef struct {
    State_Kind state;
    AST* module;
    Map* symbols;
    Stack* scopes;
    AST* current_function;
} Sema_Context;

Sema_Context make_sema_context();

void*  sema_comment                          (void* ctx, AST* node);
void*  sema_nop                              (void* ctx, AST* node);
void*  sema_space_separated_identifier_list  (void* ctx, AST* node);
void*  sema_comma_separated_list             (void* ctx, AST* node);
void*  sema_module                           (void* ctx, AST* node);
void*  sema_is                               (void* ctx, AST* node);
void*  sema_fallthrough                      (void* ctx, AST* node);
void*  sema_var_args                         (void* ctx, AST* node);
void*  sema_extern                           (void* ctx, AST* node);
void*  sema_load                             (void* ctx, AST* node);
void*  sema_link                             (void* ctx, AST* node);
void*  sema_note                             (void* ctx, AST* node);
void*  sema_int                              (void* ctx, AST* node);
void*  sema_float                            (void* ctx, AST* node);
void*  sema_string                           (void* ctx, AST* node);
void*  sema_char                             (void* ctx, AST* node);
void*  sema_ident                            (void* ctx, AST* node);
void*  sema_call                             (void* ctx, AST* node);
void*  sema_unary                            (void* ctx, AST* node);
void*  sema_binary                           (void* ctx, AST* node);
void*  sema_grouping                         (void* ctx, AST* node);
void*  sema_subscript                        (void* ctx, AST* node);
void*  sema_field_access                     (void* ctx, AST* node);
void*  sema_as                               (void* ctx, AST* node);
void*  sema_block                            (void* ctx, AST* node);
void*  sema_struct                           (void* ctx, AST* node);
void*  sema_enum                             (void* ctx, AST* node);
void*  sema_function                         (void* ctx, AST* node);
void*  sema_variable_decl                    (void* ctx, AST* node);
void*  sema_if                               (void* ctx, AST* node);
void*  sema_for                              (void* ctx, AST* node);
void*  sema_while                            (void* ctx, AST* node);
void*  sema_return                           (void* ctx, AST* node);
void*  sema_defer                            (void* ctx, AST* node);
void*  sema_break                            (void* ctx, AST* node);
void*  sema_continue                         (void* ctx, AST* node);
void*  sema_typeof                           (void* ctx, AST* node);
void*  sema_sizeof                           (void* ctx, AST* node);
void*  sema_switch                           (void* ctx, AST* node);
void*  sema_post_inc_or_dec                  (void* ctx, AST* node);
void*  sema_literal                          (void* ctx, AST* node);
void*  sema_asm                              (void* ctx, AST* node);

#endif
