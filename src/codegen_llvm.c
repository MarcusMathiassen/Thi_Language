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
  // assert
#include <stdio.h>   //
#include <string.h>  // strcmp

#define DEBUG_START                                                              \
    xassert(ctx);                                                                 \
    xassert(node);                                                                \
    info("%s: %s", (char*)__func__, wrap_with_colored_parens(ast_to_str(node))); \
    // emit(ctx, "; %s", ast_to_str(node));


static char* codegen_node(Codegen_Context* ctx, AST* node) {
    xassert(ctx);
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
    case AST_IDENT: {
        string* s = string_create("%%1 = load i32, i32* %%0");
        emit_no_tab(ctx, string_data(s));
        return NULL;
    }
    case AST_VARIABLE_DECL: {
        string* s = string_create("%%0 = alloca ");
        string_append_f(s, "%s", get_type_name(node->Variable_Decl.type));
        emit_no_tab(ctx, string_data(s));
        return NULL;
    }
    case AST_RETURN: {
        codegen_node(ctx, node->Return.node);
        return NULL;
    }
    case AST_BLOCK: {
        string* s = string_create("{\n");
        emit_no_tab(ctx, string_data(s));
        LIST_FOREACH(node->Block.stmts) {
            codegen_node(ctx, it->data);
        }
        emit_no_tab(ctx, "\n}");
        return NULL;
    }
    case AST_FUNCTION: {
        AST* func = node;
        Type* return_type = func->type->Function.return_type;
        string* s = string_create_f("define %s @%s(", get_type_name(return_type), func->Function.name);
        LIST_FOREACH(func->Function.parameters) {
            AST* mem = it->data;
            string_append(s, get_type_name(mem->type));
            if (it->next) string_append(s, ", ");
        }
        string_append(s, ")");
        emit_no_tab(ctx, string_data(s));
        codegen_node(ctx, func->Function.body);
        return NULL;
    }
    case AST_INT: {
        return NULL;
    }
    }
    UNREACHABLE;
    return NULL;
}

char* generate_llvm_from_ast(AST* ast) {
    xassert(ast);
    info("Generating code from ast");
    Codegen_Context ctx = make_codegen_context();
    codegen_node(&ctx, ast);
    char* output = string_data(ctx.section_text);
    info("%s", output);
    return output;
}
