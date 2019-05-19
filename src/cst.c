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
//                               cst.c
//------------------------------------------------------------------------------

#include "cst.h"
#include "ast.h"            // AST
#include "list.h"           // List
#include "parser_utility.h" // Parser_Context
#include "string.h"         // string
#include "typedefs.h"
#include "utility.h" // xmalloc
#include <assert.h>  // assert

//------------------------------------------------------------------------------
//                               CST Utility
//------------------------------------------------------------------------------

char* cst_kind_to_str(CST_Kind kind) {
    // clang-format off
    switch (kind) {
    default:                  ERROR_UNHANDLED_KIND(strf("%d", kind));
    case CST_TOKEN:           return "CST_TOKEN";
    case CST_PROGRAM:         return "CST_PROGRAM";
    case CST_MODULE:          return "CST_MODULE";
    case CST_IDENTIFIER:      return "CST_IDENTIFIER";
    case CST_IDENTIFIER_LIST: return "CST_IDENTIFIER_LIST";
    case CST_GROUP:           return "CST_GROUP";
    case CST_BLOCK:           return "CST_BLOCK";
    case CST_ARGUMENT_LIST:   return "CST_ARGUMENT_LIST";
    case CST_COUNT:           return "CST_COUNT";
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

char* cst_to_str(CST* node) {
    if (!node) return "---";
    // clang-format off
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(cst_kind_to_str(node->kind));
    case CST_TOKEN: return token_to_str(node->token);
    case CST_PROGRAM:  {
        string s = string_create("Program = [\n");
        LIST_FOREACH(node->Program.modules) {
            string_append_f(&s, "\t%s", cst_to_str(it->data));
            if (it->next) string_append(&s, ",\n");
        }
        string_append(&s, "\n]");
        return string_data(&s);
    }
    case CST_MODULE: return strf("%s = %s", node->Module.name, cst_to_str(node->Module.top_level));
    case CST_IDENTIFIER: return node->Identifier.name;
    case CST_IDENTIFIER_LIST: {
        string s = string_create("");
        LIST_FOREACH(node->Identifier_List.identifiers) {
            string_append(&s, cst_to_str(it->data));
            if (it->next) string_append(&s, " ");
        }
        return string_data(&s);
    }
    case CST_BLOCK: {
        string s = string_create("{ ");
        LIST_FOREACH(node->Block.constructs) {
            string_append_f(&s, "%s; ", cst_to_str(it->data));
        }
        string_append(&s, "}");
        return string_data(&s);
    }
    case CST_GROUP: return strf("(%s)", cst_to_str(node->Group.node));
    case CST_ARGUMENT_LIST: {
        string s = string_create("(");
        LIST_FOREACH(node->Block.constructs) {
            string_append(&s, cst_to_str(it->data));
            if (it->next) string_append(&s, ", ");
        }
        string_append(&s, ")");
        return string_data(&s);
    }
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

void ast_visit(void (*func)(void*, AST*), void* ctx, AST* expr) {
    if (!expr) return;
    assert(func);
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(expr->kind));
    case AST_MODULE: ast_visit(func, ctx, expr->Module.top_level); break;
    case AST_IDENT:       break;
    case AST_BLOCK:
        LIST_FOREACH(expr->Block.stmts) {
            ast_visit(func, ctx, it->data);
        }
        break;
    }
    (*func)(ctx, expr);
}

//------------------------------------------------------------------------------
//                               CST Parser
//------------------------------------------------------------------------------

#define DEBUG_START \
    assert(ctx);    \
    // info("%s: %s", __func__, token_to_str(currTok(ctx)));

CST* get_next_node(Parser_Context* ctx) {
    DEBUG_START;
    switch (tokKind(ctx)) {
    default: {
        CST* node = make_cst_token(currTok(ctx));
        eat(ctx);
        return node;
    }
    case TOKEN_IDENTIFIER: {
        List* idents = make_list();
        while (tokKind(ctx) == TOKEN_IDENTIFIER) {
            list_append(idents, make_cst_identifier(tokValue(ctx)));
            eat(ctx);
        }
        if (idents->count > 1) return make_cst_identifier_list(idents);
        else return make_cst_identifier(list_first(idents));
    }
    case TOKEN_BLOCK_START: {
        List* constructs = make_list();
        eat_kind(ctx, TOKEN_BLOCK_START);
        while (!tok_is(ctx, TOKEN_BLOCK_END)) {
            list_append(constructs, get_next_node(ctx));
        }
        eat_kind(ctx, TOKEN_BLOCK_END);
        return make_cst_block(constructs);
    }
    }
    UNREACHABLE;
    return NULL;
}

List* generate_cst_from_tokens(Token* tokens) {
    info("Generating CST from tokens..");
    Parser_Context ctx = make_parser_context();
    ctx.tokens         = tokens;
    eat(&ctx); // prep the first token
    List* cst = make_list();
    while (!tok_is(&ctx, TOKEN_EOF)) {
        CST* node = get_next_node(&ctx);
        assert(node);
        list_append(cst, node);
    }
    return cst;
}

//------------------------------------------------------------------------------
//                               CST Tests
//------------------------------------------------------------------------------
void cst_tests(void) {

    List* ident_list = make_list();
    list_append(ident_list, make_cst_identifier("x"));
    list_append(ident_list, make_cst_identifier("s32"));

    CST*  arg0     = make_cst_identifier_list(ident_list);
    CST*  arg1     = make_cst_identifier_list(ident_list);
    List* arg_list = make_list();
    list_append(arg_list, arg0);
    list_append(arg_list, arg1);

    List* block = make_list();

    list_append(block, make_cst_identifier("main"));
    list_append(block, make_cst_argument_list(arg_list));

    CST*  top_level = make_cst_block(block);
    CST*  module    = make_cst_module("my_module.thi", top_level);
    List* modules   = make_list();
    list_append(modules, module);

    top_level = make_cst_block(block);
    module    = make_cst_module("main.thi", top_level);
    list_append(modules, module);

    CST* cst = make_cst_program(modules);

    info(cst_to_str(cst));
}

//------------------------------------------------------------------------------
//                               CST Maker Functions
//------------------------------------------------------------------------------

CST* make_cst(CST_Kind kind) {
    CST* e  = xmalloc(sizeof(CST));
    e->kind = kind;
    return e;
}

CST* make_cst_token(Token token) {
    CST* e   = make_cst(CST_TOKEN);
    e->token = token;
    return e;
}

CST* make_cst_program(List* modules) {
    assert(modules);
    CST* e             = make_cst(CST_PROGRAM);
    e->Program.modules = modules;
    return e;
}

CST* make_cst_module(char* name, CST* top_level) {
    assert(name);
    assert(top_level->kind == CST_BLOCK);
    CST* e              = make_cst(CST_MODULE);
    e->Module.name      = name;
    e->Module.top_level = top_level;
    return e;
}

CST* make_cst_identifier(char* name) {
    assert(name);
    CST* e             = make_cst(CST_IDENTIFIER);
    e->Identifier.name = name;
    return e;
}

CST* make_cst_identifier_list(List* identifiers) {
    assert(identifiers);
    CST* e                         = make_cst(CST_IDENTIFIER_LIST);
    e->Identifier_List.identifiers = identifiers;
    return e;
}

CST* make_cst_block(List* constructs) {
    assert(constructs);
    CST* e              = make_cst(CST_BLOCK);
    e->Block.constructs = constructs;
    return e;
}

CST* make_cst_group(CST* node) {
    CST* e        = make_cst(CST_GROUP);
    e->Group.node = node;
    return e;
}

CST* make_cst_argument_list(List* arguments) {
    assert(arguments);
    CST* e                     = make_cst(CST_ARGUMENT_LIST);
    e->Argument_List.arguments = arguments;
    return e;
}
