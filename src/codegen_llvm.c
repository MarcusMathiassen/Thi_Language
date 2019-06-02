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

// [1] System V Application Binary Interface AMD64 Architecture Processor Supplement (With LP64 and ILP32 Programming Models) Version 1.0

#include "codegen_llvm.h"
#include "ast.h" // AST*, ast_to_str
#include "codegen_utility.h"
#include "constants.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "typedefs.h"
#include "utility.h" // error warning info, wrap_with_colored_parens
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdio.h>   //
#include <string.h>  // strcmp

#define DEBUG_START                                                              \
    assert(ctx);                                                                 \
    assert(node);                                                                \
    info("%s: %s", (char*)__func__, wrap_with_colored_parens(ast_to_str(node))); \
    // emit(ctx, "; %s", ast_to_str(node));


static char* codegen_node(Codegen_Context* ctx, AST* node) {
    assert(ctx);
    if (!node) return NULL;
    switch (node->kind) {
    ERROR_UNHANDLED_AST_KIND(node->kind);
    case AST_MODULE:
        LIST_FOREACH(node->Module.top_level) {
            codegen_node(ctx, it->data);
        }
        return NULL;
    case AST_COMMENT: return NULL;
    case AST_NOP: return NULL;
    case AST_FALLTHROUGH: return NULL;
    case AST_LOAD: return codegen_node(ctx, node->Load.module);
    case AST_LINK: return NULL;
    case AST_FUNCTION:return NULL;
    case AST_INT: {
        return NULL;
    }
    }
    UNREACHABLE;
    return NULL;
}

char* generate_llvm_from_ast(AST* ast) {
    assert(ast);
    info("Generating code from ast");

    Codegen_Context ctx = make_codegen_context();

    string_append(ctx.section_data, "section .data\n");
    emit_no_tab(&ctx, "section .text");

    codegen_node(&ctx, ast);

    char* output = "define i32 @main() {\n"
      "\t%1 = alloca i32, align 4\n"
      "\t%2 = alloca i32, align 4\n"
      "\tstore i32 0, i32* %1, align 4\n"
      "\tstore i32 3, i32* %2, align 4\n"
      "\t%3 = load i32, i32* %2, align 4\n"
      "\tret i32 %3\n"
    "}\n";

    info("%s", output);

    return output;
}
