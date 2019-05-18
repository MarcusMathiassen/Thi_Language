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
    default:                  ERROR_UNHANDLED_KIND(strf("%d", kind));
    case CST_IDENTIFIER:      return "CST_IDENTIFIER";
    case CST_IDENTIFIER_LIST: return "CST_IDENTIFIER_LIST";
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

//------------------------------------------------------------------------------
//                               CST Parser
//------------------------------------------------------------------------------

typedef struct {

} CST_Parser_Context;

CST* generate_cst_from_tokens(Token* tokens) {
    UNFINISHED;
    return NULL;
}

//------------------------------------------------------------------------------
//                               CST Tests
//------------------------------------------------------------------------------
void cst_tests(void) {
    List* cst = make_list();

    list_append(cst, make_cst_identifier("main"));
    List* ident_list = make_list();
    list_append(ident_list, make_cst_identifier("x"));
    list_append(ident_list, make_cst_identifier("s32"));

    CST* arg0 = make_cst_identifier_list(ident_list);
    CST* arg1 = make_cst_identifier_list(ident_list);
    List* arg_list = make_list();

    list_append(arg_list, arg0);
    list_append(arg_list, arg1);
    list_append(cst, make_cst_argument_list(arg_list));

    LIST_FOREACH(cst) {
        info(cst_to_str(it->data));
    }

    error("");
}

//------------------------------------------------------------------------------
//                               CST Maker Functions
//------------------------------------------------------------------------------

CST* make_cst(CST_Kind kind) {
    CST* e  = xmalloc(sizeof(CST));
    e->kind = kind;
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
CST* make_cst_argument_list(List* arguments) {
    assert(arguments);
    CST* e                     = make_cst(CST_ARGUMENT_LIST);
    e->Argument_List.arguments = arguments;
    return e;
}
