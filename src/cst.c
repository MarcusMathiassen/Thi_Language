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
#include "ast.h"    // AST
#include "list.h"   // List
#include "string.h" // string
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
    case CST_IDENTIFIER: return node->Identifier.name;
    case CST_SPACE_SEPARATED_IDENTIFIER_LIST: {
        string *s = string_create("");
        LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
            string_append(s, cst_to_str(it->data));
            if (it->next) string_append(s, " ");
        }
        return string_data(s);
    }
    case CST_COMMA_SEPARATED_LIST: {
        string *s = string_create("");
        LIST_FOREACH(node->Comma_Separated_List.nodes) {
            string_append(s, cst_to_str(it->data));
            if (it->next) string_append(s, ", ");
        }
        return string_data(s);
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

typedef struct {
    Token* tokens;
    Token  prev;
    Token  curr;
} CST_Parser_Context;

static Token curr(CST_Parser_Context* ctx) {
    return ctx->curr;
}

static Token prev(CST_Parser_Context* ctx) {
    return ctx->prev;
}

static Token_Kind kind(CST_Parser_Context* ctx) {
    return ctx->curr.kind;
}

static Loc_Info loc(CST_Parser_Context* ctx) {
    Loc_Info loc_info;
    loc_info.line_pos = curr(ctx).line_pos;
    loc_info.col_pos  = curr(ctx).col_pos;
    return loc_info;
}

static char* value(CST_Parser_Context* ctx) {
    return ctx->curr.value;
}

static bool same_line(CST_Parser_Context* ctx) {
    s64 l1 = curr(ctx).line_pos;
    s64 l2 = prev(ctx).line_pos;
    return l1 == l2;
}

static bool is(CST_Parser_Context* ctx, Token_Kind k) {
    return kind(ctx) == k;
}

static void eat(CST_Parser_Context* ctx) {
    ctx->prev = ctx->curr;
    ctx->curr = *ctx->tokens;
    if (ctx->tokens)
        ++ctx->tokens;
    else
        error("cant eat no more");
}

static void eat_kind(CST_Parser_Context* ctx, Token_Kind expected_kind) {
    if (kind(ctx) != expected_kind) {
        error("Expected '%s' got '%s'", token_kind_to_str(expected_kind), token_kind_to_str(kind(ctx)));
    }
    eat(ctx);
}

CST* parse_node(CST_Parser_Context* ctx) {
    DEBUG_START;
    switch (kind(ctx)) {
    default: {
        CST* node = make_cst_token(loc(ctx), curr(ctx));
        eat(ctx);
        return node;
    }
    case TOKEN_IDENTIFIER: {
        List* idents = make_list();
        list_append(idents, make_cst_identifier(loc(ctx), value(ctx)));
        eat_kind(ctx, TOKEN_IDENTIFIER);
        while (is(ctx, TOKEN_IDENTIFIER) && same_line(ctx)) {
            list_append(idents, make_cst_identifier(loc(ctx), value(ctx)));
            eat_kind(ctx, TOKEN_IDENTIFIER);
        }
        if (idents->count > 1) return make_cst_space_separated_identifier_list(loc(ctx), idents);
        return list_first(idents);
    }
    }
    UNREACHABLE;
    return NULL;
}

List* generate_cst_from_tokens(Token* tokens) {
    info("Generating CST from tokens..");

    CST_Parser_Context ctx;
    ctx.tokens    = tokens;
    ctx.curr.kind = TOKEN_UNKNOWN;
    ctx.prev.kind = TOKEN_UNKNOWN;

    eat(&ctx); // prep the first token
    List* cst = make_list();

    while (!is(&ctx, TOKEN_EOF)) {
        CST* node = parse_node(&ctx);
        switch (kind(&ctx)) {
        default: break;
        case TOKEN_COMMA: {
            List* node_list = make_list();
            list_append(node_list, node);
            while (is(&ctx, TOKEN_COMMA)) {
                eat_kind(&ctx, TOKEN_COMMA);
                CST* node = parse_node(&ctx);
                list_append(node_list, node);
            }
            if (node_list->count > 1) node = make_cst_comma_separated_list(loc(&ctx), node_list);
        } break;
        }
        assert(node);
        list_append(cst, node);
    }
    return cst;
}

//------------------------------------------------------------------------------
//                               CST Tests
//------------------------------------------------------------------------------
void cst_tests(void) {
}
//------------------------------------------------------------------------------
//                               CST Maker Functions
//------------------------------------------------------------------------------

CST* make_cst(Loc_Info loc_info, CST_Kind kind) {
    CST* e      = xmalloc(sizeof(CST));
    e->loc_info = loc_info;
    e->kind     = kind;
    return e;
}

CST* make_cst_token(Loc_Info loc_info, Token token) {
    CST* e   = make_cst(loc_info, CST_TOKEN);
    e->token = token;
    return e;
}
CST* make_cst_identifier(Loc_Info loc_info, char* name) {
    assert(name);
    CST* e             = make_cst(loc_info, CST_IDENTIFIER);
    e->Identifier.name = name;
    return e;
}
CST* make_cst_space_separated_identifier_list(Loc_Info loc_info, List* identifiers) {
    assert(identifiers);
    CST* e                                         = make_cst(loc_info, CST_SPACE_SEPARATED_IDENTIFIER_LIST);
    e->Space_Separated_Identifier_List.identifiers = identifiers;
    return e;
}
CST* make_cst_comma_separated_list(Loc_Info loc_info, List* nodes) {
    assert(nodes);
    CST* e                        = make_cst(loc_info, CST_COMMA_SEPARATED_LIST);
    e->Comma_Separated_List.nodes = nodes;
    return e;
}
