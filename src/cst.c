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
    default:                                  ERROR_UNHANDLED_KIND(strf("%d", kind));
    case CST_TOKEN:                           return "CST_TOKEN";
    case CST_PROGRAM:                         return "CST_PROGRAM";
    case CST_MODULE:                          return "CST_MODULE";
    case CST_IDENTIFIER:                      return "CST_IDENTIFIER";
    case CST_SPACE_SEPARATED_IDENTIFIER_LIST: return "CST_SPACE_SEPARATED_IDENTIFIER_LIST";
    case CST_COMMA_SEPARATED_LIST:            return "CST_COMMA_SEPARATED_LIST";
    case CST_COUNT:                           return "CST_COUNT";
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
    case CST_TOKEN: return  node->token.value;
    case CST_PROGRAM:  {
        string s = string_create("Program = [\n");
        LIST_FOREACH(node->Program.modules) {
            string_append_f(&s, "%s", cst_to_str(it->data));
            if (it->next) string_append(&s, "\n");
        }
        string_append(&s, "\n]");
        return string_data(&s);
    }
    case CST_MODULE:  {
        string s = string_create_f("%s = [\n", node->Module.name);
        LIST_FOREACH(node->Module.nodes) {
            string_append(&s, cst_to_str(it->data));
            if (it->next) string_append(&s, "\n");
        }
        string_append(&s, "\n]");
        return string_data(&s);
    }
    case CST_IDENTIFIER: return node->Identifier.name;
    case CST_SPACE_SEPARATED_IDENTIFIER_LIST: {
        string s = string_create("");
        LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
            string_append(&s, cst_to_str(it->data));
            if (it->next) string_append(&s, " ");
        }
        return string_data(&s);
    }
    case CST_COMMA_SEPARATED_LIST: {
        string s = string_create("");
        LIST_FOREACH(node->Comma_Separated_List.nodes) {
            string_append(&s, cst_to_str(it->data));
            if (it->next) string_append(&s, ", ");
        }
        return string_data(&s);
    }
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

void cst_visit(void (*func)(void*, CST*), void* ctx, CST* node) {
    if (!node) return;
    assert(func);
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(cst_kind_to_str(node->kind));
    case CST_TOKEN: break;
    case CST_PROGRAM:
        LIST_FOREACH(node->Program.modules) {
            cst_visit(func, ctx, it->data);
        }
        break;
    case CST_MODULE: {
        LIST_FOREACH(node->Module.nodes) {
            cst_visit(func, ctx, it->data);
        }
    } break;
    case CST_IDENTIFIER: break;
    case CST_SPACE_SEPARATED_IDENTIFIER_LIST:
        LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
            cst_visit(func, ctx, it->data);
        }
        break;
    case CST_COMMA_SEPARATED_LIST:
        LIST_FOREACH(node->Comma_Separated_List.nodes) {
            cst_visit(func, ctx, it->data);
        }
        break;
    }
    (*func)(ctx, node);
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
        list_append(idents, make_cst_identifier(tokValue(ctx)));
        eat_kind(ctx, TOKEN_IDENTIFIER);
        while ((tokKind(ctx) == TOKEN_IDENTIFIER) && tok_is_on_same_line(ctx)) {
            list_append(idents, make_cst_identifier(tokValue(ctx)));
            eat_kind(ctx, TOKEN_IDENTIFIER);
        }
        if (idents->count > 1) return make_cst_space_separated_identifier_list(idents);
        return list_first(idents);
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
    List* top_level = make_list();

    while (!tok_is(&ctx, TOKEN_EOF)) {
        CST* node = get_next_node(&ctx);
        switch (tokKind(&ctx)) {
        default: break;
        case TOKEN_COMMA: {
            List* node_list = make_list();
            list_append(node_list, node);
            while (tok_is(&ctx, TOKEN_COMMA)) {
                eat_kind(&ctx, TOKEN_COMMA);
                CST* node = get_next_node(&ctx);
                list_append(node_list, node);
            }
            if (node_list->count > 1) node = make_cst_comma_separated_list(node_list);
        } break;
        }
        assert(node);
        list_append(top_level, node);
    }
    return top_level;
}

//------------------------------------------------------------------------------
//                               CST Tests
//------------------------------------------------------------------------------
void cst_tests(void) {
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

CST* make_cst_module(char* name, List* nodes) {
    assert(name);
    CST* e              = make_cst(CST_MODULE);
    e->Module.name      = name;
    e->Module.nodes = nodes;
    return e;
}

CST* make_cst_identifier(char* name) {
    assert(name);
    CST* e             = make_cst(CST_IDENTIFIER);
    e->Identifier.name = name;
    return e;
}

CST* make_cst_space_separated_identifier_list(List* identifiers) {
    assert(identifiers);
    CST* e                                         = make_cst(CST_SPACE_SEPARATED_IDENTIFIER_LIST);
    e->Space_Separated_Identifier_List.identifiers = identifiers;
    return e;
}

CST* make_cst_comma_separated_list(List* nodes) {
    assert(nodes);
    CST* e                        = make_cst(CST_COMMA_SEPARATED_LIST);
    e->Comma_Separated_List.nodes = nodes;
    return e;
}
