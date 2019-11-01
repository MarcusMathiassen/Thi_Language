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

#include "ast.h" // AST, ERROR_UNHANDLED_AST
#include "ast_to_c.h"
#include "common.h" // UNREACHABLE, 

// static char* typedefs = "#include <stddef.h>\n#include <stdint.h>\ntypedef int8_t   s8;typedef int16_t  s16;typedef int32_t  s32;typedef int64_t  s64;typedef uint8_t  u8;typedef uint16_t u16;typedef uint32_t u32;typedef uint64_t u64;typedef float    f32;typedef double   f64; typedef s8 bool;";

char* ast_to_c(AST* node) {
    if (!node) return "---";
    AST_Kind kind = node->kind;
    TASSERT_KIND_IN_RANGE(AST, kind);
    switch(kind) {
        ERROR_UNHANDLED_AST_KIND(kind);
        case AST_FUNCTION: {
            char* func_name = node->Function.name;
            char* return_type = type_to_str(node->type->Function.return_type);
            string *s = make_string_f("%s %s(", return_type, func_name);
            list_foreach(node->type->Function.parameters) {
                Type_Name_Pair* tp = it->data;
                string_append_f(s, "%s %s", type_to_str(tp->type), tp->name);
                if (it->next) string_append(s, ", ");
            }
            string_append_f(s, ") %s", ast_to_c(node->Function.body));
            return string_data(s);
        } break;
        case AST_MODULE: {
            string *s = make_string("");
            list_foreach(node->Module.top_level) {
                string_append(s, ast_to_c(it->data));
            }
            return string_data(s);
        } break;
        case AST_BLOCK: {
            string *s = make_string("{");
            list_foreach(node->Block.stmts) {
                string_append_f(s, "%s;", ast_to_c(it->data));
            }
            string_append(s, "}");
            return string_data(s);
        } break;

        case AST_RETURN: {
            return strf("return %s", ast_to_c(node->Return.node));
        } break;
        case AST_BINARY: {
            return ast_to_str(node);
        } break;
    }
    UNREACHABLE;
    return NULL;
}


