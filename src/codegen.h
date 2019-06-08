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

#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h" // AST
void* codegen_comment                         (void* ctx, AST* node);
void* codegen_nop                             (void* ctx, AST* node);
void* codegen_space_separated_identifier_list (void* ctx, AST* node);
void* codegen_comma_separated_list            (void* ctx, AST* node);
void* codegen_module                          (void* ctx, AST* node);
void* codegen_is                              (void* ctx, AST* node);
void* codegen_fallthrough                     (void* ctx, AST* node);
void* codegen_var_args                        (void* ctx, AST* node);
void* codegen_extern                          (void* ctx, AST* node);
void* codegen_load                            (void* ctx, AST* node);
void* codegen_link                            (void* ctx, AST* node);
void* codegen_note                            (void* ctx, AST* node);
void* codegen_int                             (void* ctx, AST* node);
void* codegen_float                           (void* ctx, AST* node);
void* codegen_string                          (void* ctx, AST* node);
void* codegen_char                            (void* ctx, AST* node);
void* codegen_ident                           (void* ctx, AST* node);
void* codegen_call                            (void* ctx, AST* node);
void* codegen_unary                           (void* ctx, AST* node);
void* codegen_binary                          (void* ctx, AST* node);
void* codegen_grouping                        (void* ctx, AST* node);
void* codegen_subscript                       (void* ctx, AST* node);
void* codegen_field_access                    (void* ctx, AST* node);
void* codegen_as                              (void* ctx, AST* node);
void* codegen_block                           (void* ctx, AST* node);
void* codegen_struct                          (void* ctx, AST* node);
void* codegen_enum                            (void* ctx, AST* node);
void* codegen_function                        (void* ctx, AST* node);
void* codegen_variable_decl                   (void* ctx, AST* node);
void* codegen_if                              (void* ctx, AST* node);
void* codegen_for                             (void* ctx, AST* node);
void* codegen_while                           (void* ctx, AST* node);
void* codegen_return                          (void* ctx, AST* node);
void* codegen_defer                           (void* ctx, AST* node);
void* codegen_break                           (void* ctx, AST* node);
void* codegen_continue                        (void* ctx, AST* node);
void* codegen_typeof                          (void* ctx, AST* node);
void* codegen_sizeof                          (void* ctx, AST* node);
void* codegen_switch                          (void* ctx, AST* node);
void* codegen_post_inc_or_dec                 (void* ctx, AST* node);
void* codegen_literal                         (void* ctx, AST* node);
void* codegen_asm                             (void* ctx, AST* node);
#endif
