// Copyright (c) 2019 Marc2us Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, nodeESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------

#include "ast.h"
#include "constants.h"
#include "lexer.h"  // token_kind_to_str,
#include "string.h" // strf, string_append, string
#include "typedefs.h"
#include "utility.h" // info, success, error, warning, xmalloc, xrealloc
#include <assert.h>  // assert
#include <string.h>  // strlen

//------------------------------------------------------------------------------
//                               AST Utility
//------------------------------------------------------------------------------

char* ast_kind_to_str(AST_Kind kind) {
    // clang-format off
    switch (kind) {
    default: ERROR_UNHANDLED_KIND(strf("%d", kind));
    case AST_COMMENT:                         return "AST_COMMENT";
    case AST_NOP:                             return "AST_NOP";
    case AST_SPACE_SEPARATED_IDENTIFIER_LIST: return "AST_SPACE_SEPARATED_IDENTIFIER_LIST";
    case AST_COMMA_SEPARATED_LIST:            return "AST_COMMA_SEPARATED_LIST";
    case AST_MODULE:                          return "AST_MODULE";
    case AST_IS:                              return "AST_IS";
    case AST_FALLTHROUGH:                     return "AST_FALLTHROUGH";
    case AST_VAR_ARGS:                        return "AST_VAR_ARGS";
    case AST_EXTERN:                          return "AST_EXTERN";
    case AST_LOAD:                            return "AST_LOAD";
    case AST_LINK:                            return "AST_LINK";
    case AST_NOTE:                            return "AST_NOTE";
    case AST_INT:                             return "AST_INT";
    case AST_FLOAT:                           return "AST_FLOAT";
    case AST_STRING:                          return "AST_STRING";
    case AST_IDENT:                           return "AST_IDENT";
    case AST_CALL:                            return "AST_CALL";
    case AST_UNARY:                           return "AST_UNARY";
    case AST_BINARY:                          return "AST_BINARY";
    case AST_GROUPING:                        return "AST_GROUPING";
    case AST_SUBSCRIPT:                       return "AST_SUBSCRIPT";
    case AST_FIELD_ACCESS:                    return "AST_FIELD_ACCESS";
    case AST_AS:                              return "AST_AS";
    case AST_BLOCK:                           return "AST_BLOCK";
    case AST_STRUCT:                          return "AST_STRUCT";
    case AST_ENUM:                            return "AST_ENUM";
    case AST_FUNCTION:                        return "AST_FUNCTION";
    case AST_VARIABLE_DECL:                   return "AST_VARIABLE_DECL";
    case AST_CONSTANT_DECL:                   return "AST_CONSTANT_DECL";
    case AST_IF:                              return "AST_IF";
    case AST_FOR:                             return "AST_FOR";
    case AST_WHILE:                           return "AST_WHILE";
    case AST_RETURN:                          return "AST_RETURN";
    case AST_DEFER:                           return "AST_DEFER";
    case AST_BREAK:                           return "AST_BREAK";
    case AST_CONTINUE:                        return "AST_CONTINUE";
    case AST_TYPEOF:                          return "AST_TYPEOF";
    case AST_SIZEOF:                          return "AST_SIZEOF";
    case AST_SWITCH:                          return "AST_SWITCH";
    case AST_COUNT:                           return "AST_COUNT";
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

char* get_ast_loc_str(AST* node) {
    return strf("%d:%d", node->loc_info.line_pos, node->loc_info.col_pos);
}

char* get_ast_name(AST* node) {
    if (!node) {
        return "---";
    }
    char* result = NULL;
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
    case AST_NOP:
        result = "nop";
        break;
    case AST_MODULE: {
        result = node->Module.name;
        break;
    }
    case AST_VARIABLE_DECL: {
        result = node->Variable_Decl.name;
        break;
    }
    case AST_CONSTANT_DECL: {
        result = node->Constant_Decl.name;
        break;
    }
    case AST_EXTERN: {
        result = get_type_name(node->Extern.type);
        break;
    }
    case AST_FUNCTION: {
        result = node->Function.name;
        break;
    }
    case AST_STRUCT: {
        result = node->Struct.name;
        break;
    }
    case AST_ENUM: {
        result = node->Enum.name;
        break;
    }
    case AST_IDENT: {
        result = node->Ident.name;
        break;
    }
    }
    assert(result);
    return result;
}

char* ast_to_str(AST* node) {
    String_Context ctx;
    ctx.str = string_create("");
    ctx.indentation_level = DEFAULT_INDENT_LEVEL;
    return ast_to_str_r(&ctx, node);
}

char* ast_to_str_r(String_Context* ctx, AST* node) {
    assert(ctx);

    // Local alias
    string* s = ctx->str;

    if (!node) {
        string_append(s, "---");
        return string_data(s);
    }

    assert(node->kind < AST_COUNT && node->kind >= 0);

    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
    case AST_COMMENT:
        string_append(s, node->Comment.text);
        break;
    case AST_NOP:
        string_append(s, "nop");
        break;
    case AST_SPACE_SEPARATED_IDENTIFIER_LIST: {
        LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
            ast_to_str_r(ctx, it->data);
            if (it->next) string_append(s, " ");
        }
        break;
    }
    case AST_COMMA_SEPARATED_LIST: {
        LIST_FOREACH(node->Comma_Separated_List.nodes) {
            ast_to_str_r(ctx, it->data);
            if (it->next) string_append(s, ", ");
        }
        break;
    }
    case AST_MODULE: {
        // string_append_f(s, "# %s\n", get_ast_name(node));
        AST* last_stmt = NULL;
        LIST_FOREACH(node->Module.top_level) {
            AST* stmt = it->data;
            if (last_stmt) {
                if(last_stmt->kind != AST_MODULE) {
                    // If there is a difference in line position. Add
                    // that many newlines.
                    s64 diff = stmt->loc_info.line_pos - last_stmt->loc_info.line_pos;
                    // if (diff > 1) error("stmt = %d | last = %d", stmt->loc_info.line_pos, last_stmt->loc_info.line_pos);
                    while (--diff > 0) {
                        warning("Adding a newline!");
                        string_append(s, "\n");
                    }
                }
            }
            ast_to_str_r(ctx, stmt);
            string_append(s, "\n");
            last_stmt = stmt;
        }
        break;
    }
    case AST_VAR_ARGS: {
        string_append(s, "...");
        break;
    }
    case AST_FALLTHROUGH: {
        string_append(s, "fallthrough");
        break;
    }
    case AST_SWITCH: {
        string_append(s, "switch ");
        ast_to_str_r(ctx, node->Switch.cond);
        ast_to_str_r(ctx, node->Switch.cases);
        string_append(s, " else ");
        ast_to_str_r(ctx, node->Switch.default_case);
        break;
    }
    case AST_IS: {
        string_append(s, "is ");
        ast_to_str_r(ctx, node->Is.node);
        string_append(s, " ");
        ast_to_str_r(ctx, node->Is.body);
        break;
    }
    case AST_TYPEOF: {
        string_append(s, "typeof ");
        ast_to_str_r(ctx, node->Typeof.node);
        break;
    }
    case AST_SIZEOF: {
        string_append(s, "sizeof ");
        ast_to_str_r(ctx, node->Sizeof.node);
        break;
    }
    case AST_EXTERN: {
        string_append_f(s, "extern %s", get_ast_name(node));
        type_to_str_r(ctx, node->Extern.type);
        break;
    }
    case AST_LOAD: {
        string_append_f(s, "load \"%s\"", node->Load.str);
        break;
    }
    case AST_LINK: {
        string_append_f(s, "link \"%s\"", node->Link.str);
        break;
    }
    case AST_DEFER: {
        string_append(s, "defer ");
        ast_to_str_r(ctx, node->Defer.node);
        break;
    }
    case AST_BREAK: {
        string_append(s, "break");
        break;
    }
    case AST_CONTINUE: {
        string_append(s, "continue");
        break;
    }
    case AST_AS: {
        ast_to_str_r(ctx, node->As.node);
        string_append(s, " as ");
        ast_to_str_r(ctx, node->As.type_node);
        break;
    }
    case AST_RETURN: {
        string_append(s, "return ");
        ast_to_str_r(ctx, node->Return.node);
        break;
    }
    case AST_FIELD_ACCESS: {
        ast_to_str_r(ctx, node->Field_Access.load);
        string_append_f(s, ".%s", node->Field_Access.field);
        break;
    }
    case AST_NOTE: {
        string_append(s, "$");
        ast_to_str_r(ctx, node->Note.node);
        break;
    }
    case AST_INT: {
        string_append_f(s, "%lld", node->Int.val);
        break;
    }
    case AST_FLOAT: {
        string_append_f(s, "%f", node->Float.val);
        break;
    }
    case AST_STRING: {
        string_append_f(s, "\"%s\"", node->String.val);
        break;
    }
    case AST_IDENT: {
        string_append(s, get_ast_name(node));
        break;
    }
    case AST_UNARY: {
        string_append_f(s, "%s", token_kind_to_str(node->Unary.op));
        ast_to_str_r(ctx, node->Unary.operand);
        break;
    }
    case AST_BINARY: {
        ast_to_str_r(ctx, node->Binary.lhs);
        string_append(s, " ");
        string_append(s, token_kind_to_str(node->Binary.op));
        string_append(s, " ");
        ast_to_str_r(ctx, node->Binary.rhs);
        break;
    }
    case AST_VARIABLE_DECL: {
        if (node->Variable_Decl.value) {
            string_append_f(s, "%s %s = ", get_ast_name(node), get_type_name(node->type));
            ast_to_str_r(ctx, node->Variable_Decl.value);
        } else {
            string_append_f(s, "%s %s", get_ast_name(node), get_type_name(node->type));
        }
        break;
    }
    case AST_CONSTANT_DECL: {
        string_append_f(s, "%s :: ", get_ast_name(node));
        ast_to_str_r(ctx, node->Constant_Decl.value);
        break;
    }
    case AST_STRUCT: {
        string_append_f(s, "def %s\n", get_ast_name(node));
        ctx->indentation_level += DEFAULT_INDENT_LEVEL;
        LIST_FOREACH(node->Struct.members) {
            string_append(s, get_indentation_as_str(ctx->indentation_level));
            ast_to_str_r(ctx, it->data);
            if (it->next) string_append(s, "\n");
        }
        ctx->indentation_level -= DEFAULT_INDENT_LEVEL;
        break;
    }
    case AST_ENUM: {
        string_append(s, "def ");
        type_to_str_r(ctx, node->type);
        break;
    }
    case AST_FUNCTION: {
        string_append_f(s, "def %s(", get_ast_name(node));
        LIST_FOREACH(node->Function.parameters) {
            ast_to_str_r(ctx, it->data);
            if (it->next) string_append(s, ", ");
        }
        string_append_f(s, ") %s", get_type_name(node->type->Function.return_type));
        ast_to_str_r(ctx, node->Function.body);
        break;
    }
    case AST_GROUPING: {
        string_append(s, "(");
        ast_to_str_r(ctx, node->Grouping.node);
        string_append(s, ")");
        break;
    }
    case AST_SUBSCRIPT: {
        ast_to_str_r(ctx, node->Subscript.load);
        string_append(s, "[");
        ast_to_str_r(ctx, node->Subscript.sub);
        string_append(s, "]");
        break;
    }
    case AST_IF: {
        string_append(s, "if ");
        ast_to_str_r(ctx, node->If.cond);
        string_append(s, " ");
        ast_to_str_r(ctx, node->If.then_block);
        if (node->If.else_block) {
            string_append(s, " else ");
            ast_to_str_r(ctx, node->If.else_block);
        }
        break;
    }
    case AST_FOR: {
        string_append(s, "for ");
        ast_to_str_r(ctx, node->For.init);
        string_append(s, " ");
        ast_to_str_r(ctx, node->For.cond);
        string_append(s, " ");
        ast_to_str_r(ctx, node->For.step);
        ast_to_str_r(ctx, node->For.then_block);
        break;
    }
    case AST_WHILE: {
        string_append(s, "while ");
        ast_to_str_r(ctx, node->While.cond);
        string_append(s, " ");
        ast_to_str_r(ctx, node->While.then_block);
        break;
    }
    case AST_BLOCK: {
        string_append(s, "\n");
        ctx->indentation_level += DEFAULT_INDENT_LEVEL;
        AST* last_stmt = NULL;
        LIST_FOREACH(node->Block.stmts) {
            string_append(s, get_indentation_as_str(ctx->indentation_level));

            AST* stmt = it->data;
            ast_to_str_r(ctx, stmt);
            if (it->next) string_append(s, "\n");

            if (last_stmt) {
                if(last_stmt->kind != AST_MODULE) {
                    // If there is a difference in line position. Add
                    // that many newlines.
                    s64 diff = stmt->loc_info.line_pos - last_stmt->loc_info.line_pos;
                    // if (diff > 1) error("stmt = %d | last = %d", stmt->loc_info.line_pos, last_stmt->loc_info.line_pos);
                    while (--diff > 0) {
                        warning("Adding a newline!");
                        string_append(s, "\n");
                    }
                }
            }

            last_stmt = stmt;

        }
        ctx->indentation_level -= DEFAULT_INDENT_LEVEL;
        break;
    }
    case AST_CALL: {
        string_append_f(s, "%s(", node->Call.callee);
        LIST_FOREACH(node->Call.args) {
            ast_to_str_r(ctx, it->data);
            if (it->next) string_append(s, ", ");
        }
        string_append(s, ")");
        break;
    }
    }
    return string_data(s);
}

// char* ast_json_prelude(AST* node) {
//     return strf(
//         "\"type\": %s, \"%s\"",
//         type_to_json(node->type),
//         ast_kind_to_str(node->kind));
// }

// char* ast_to_json(AST* node) {
//     if (!node) return "\"NULL\"";
//     // clang-format off
//     switch (node->kind) {
//     default:                ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));
//     case AST_MODULE:        return strf("{%s:{\"name\":\"%s\",\"top_level\":%s}}", ast_json_prelude(node), node->Module.name, ast_to_json(node->Module.top_level));
//     case AST_SWITCH:        return strf("{%s:{\"cond\":%s,\"cases\":%s,\"default\":%s}}", ast_json_prelude(node), ast_to_json(node->Switch.cond), ast_to_json(node->Switch.cases), ast_to_json(node->Switch.default_case));
//     case AST_IS:            return strf("{%s:{\"case\":%s,\"body\":%s,\"has_fallthrough\":%s}}", ast_json_prelude(node), ast_to_json(node->Is.node), ast_to_json(node->Is.body), node->Is.has_fallthrough ? "true" : "false");
//     case AST_FIELD_ACCESS:  return strf("{%s:{\"load\":%s,\"field\":\"%s\"}}", ast_json_prelude(node), ast_to_json(node->Field_Access.load), node->Field_Access.field);
//     case AST_TYPEOF:        return strf("{%s:{\"typeof\": %s}}", ast_json_prelude(node), ast_to_json(node->Typeof.node));
//     case AST_SIZEOF:        return strf("{%s:{\"sizeof\": %s}}", ast_json_prelude(node), ast_to_json(node->Sizeof.node));
//     case AST_AS:            return strf("{%s:{\"node\": %s, \"type_node\":%s}}", ast_json_prelude(node), ast_to_json(node->As.node), ast_to_json(node->As.type_node));
//     case AST_EXTERN:        return strf("{%s:{\"extern\": %s}}", ast_json_prelude(node), type_to_json(node->type));
//     case AST_LOAD:          return strf("{%s:{\"load\": \"%s\"}}", ast_json_prelude(node), node->Load.str);
//     case AST_VAR_ARGS:      return strf("{%s:{\"var_args\": ...}}", ast_json_prelude(node));
//     case AST_LINK:          return strf("{%s:{\"link\": \"%s\"}}", ast_json_prelude(node), node->Link.str);
//     case AST_SUBSCRIPT:     return strf("{%s:{\"load\": %s, \"sub\": %s}}", ast_json_prelude(node), ast_to_json(node->Subscript.load), ast_to_json(node->Subscript.sub));
//     case AST_CONTINUE:      return strf("{%s:{%s}}", ast_json_prelude(node), "\"continue\"");
//     case AST_FALLTHROUGH:   return strf("{%s: true}", ast_json_prelude(node));
//     case AST_BREAK:         return strf("{%s:{%s}}", ast_json_prelude(node), "\"break\"");
//     case AST_DEFER:         return strf("{%s:{\"node\": %s}}", ast_json_prelude(node), ast_to_json(node->Defer.node));
//     case AST_NOTE:          return strf("{%s:{\"note\":\"%s\"}}", ast_json_prelude(node), ast_to_json(node->Note.node));
//     case AST_INT:           return strf("{%s:{\"value\": %lld}}", ast_json_prelude(node), node->Int.val);
//     case AST_STRING:        return strf("{%s:{\"value\": \"%s\"}}", ast_json_prelude(node), node->String.val);
//     case AST_FLOAT:         return strf("{%s:{\"value\": %f}}", ast_json_prelude(node), node->Float.val);
//     case AST_IDENT:         return strf("{%s:{\"ident\": \"%s\"}}", ast_json_prelude(node), node->Ident.name);
//     case AST_UNARY:         return strf("{%s:{\"op\": \"%s\", \"node\": \"%s\"}}", ast_json_prelude(node), token_kind_to_str(node->Unary.op), ast_to_json(node->Unary.operand));
//     case AST_BINARY:        return strf("{%s:{\"op\": \"%s\", \"lhs\": %s, \"rhs\": %s}}", ast_json_prelude(node), token_kind_to_str(node->Binary.op), ast_to_json(node->Binary.lhs), ast_to_json(node->Binary.rhs));
//     case AST_RETURN:        return strf("{%s:{\"node\": %s}}", ast_json_prelude(node), ast_to_json(node->Return.node));
//     case AST_VARIABLE_DECL: return strf("{%s:{\"name\": \"%s\", \"type\": %s, \"value\": %s}}", ast_json_prelude(node), node->Variable_Decl.name, type_to_json(node->type), ast_to_json(node->Variable_Decl.value));
//     case AST_CONSTANT_DECL: return strf("{%s:{\"name\": \"%s\", \"value\": %s}}", ast_json_prelude(node), node->Constant_Decl.name, ast_to_json(node->Constant_Decl.value));
//     case AST_FUNCTION:      return strf("{%s:{\"type\": %s, \"body\": %s }}", ast_json_prelude(node), type_to_json(node->type), ast_to_json(node->Function.body));
//     case AST_STRUCT:        return strf("{%s:{\"type\": %s}}", ast_json_prelude(node), type_to_json(node->type));
//     case AST_ENUM:          return strf("{%s:{\"type\": %s}}", ast_json_prelude(node), type_to_json(node->type));
//     case AST_GROUPING:      return strf("{%s:{\"node\": %s}}", ast_json_prelude(node), ast_to_json(node->Grouping.node));
//     case AST_WHILE:         return strf("{%s:{\"cond\": %s, \"then_block\": %s}}", ast_json_prelude(node), ast_to_json(node->While.cond), ast_to_json(node->While.then_block));
//     case AST_FOR:           return strf("{%s:{\"init\": %s, \"cond\": %s, \"step\": %s, ""\"then_block\": %s }}", ast_json_prelude(node), ast_to_json(node->For.init), ast_to_json(node->For.cond), ast_to_json(node->For.step), ast_to_json(node->For.then_block));
//     case AST_IF:            return strf("{%s:{\"cond\": %s, \"then_block\": %s, \"else_block\": %s }}", ast_json_prelude(node), ast_to_json(node->If.cond), ast_to_json(node->If.then_block), ast_to_json(node->If.else_block));
//     // clang-format on
//     case AST_BLOCK: {
//         string* str = string_create(strf("{%s: [", ast_json_prelude(node)));
//         LIST_FOREACH(node->Block.stmts) {
//             string_append(str, ast_to_json(it->data));
//             if (it->next) string_append(str, ", ");
//         }
//         string_append(str, "]}");
//         return string_data(str);
//     } break;
//     case AST_CALL: {
//         string* str = string_create("");
//         string_append_f(str, "{%s:{\"callee\": \"%s\", ", ast_json_prelude(node), node->Call.callee);
//         string_append(str, "\"args\": [");
//         s64 arg_count = node->Call.args->count;
//         s64 counter   = 0;
//         LIST_FOREACH(node->Call.args) {
//             string_append(str, ast_to_json(it->data));
//             if (counter != arg_count - 1) string_append(str, ",");
//             counter += 1;
//         }
//         string_append(str, "]}}");
//         return string_data(str);
//     } break;
//     }
//     UNREACHABLE;
//     return NULL;
// }

void ast_visit(void (*func)(void*, AST*), void* ctx, AST* node) {
    if (!node) return;
    assert(func);
    switch (node->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(node->kind));

    case AST_COMMENT: break;
    case AST_NOP: break;

    // @Todo(marcus) figure out what to do with these.
    case AST_SPACE_SEPARATED_IDENTIFIER_LIST:
        LIST_FOREACH(node->Space_Separated_Identifier_List.identifiers) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_COMMA_SEPARATED_LIST:
        LIST_FOREACH(node->Comma_Separated_List.nodes) {
            ast_visit(func, ctx, it->data);
        }
        break;
    // clang-format off
    case AST_MODULE: 
        LIST_FOREACH(node->Module.top_level) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_TYPEOF: ast_visit(func, ctx, node->Typeof.node);      break;
    case AST_SIZEOF: ast_visit(func, ctx, node->Sizeof.node);      break;
    case AST_NOTE:   ast_visit(func, ctx, node->Note.node);        break;
    
    case AST_FALLTHROUGH: break;
    case AST_LOAD:
        ast_visit(func, ctx, node->Load.module);
        break;
    case AST_LINK:        break;
    case AST_INT:         break;
    case AST_FLOAT:       break;
    case AST_STRING:      break;
    case AST_IDENT:       break;
    case AST_STRUCT:      break;
        // clang-format on

    case AST_UNARY:
        ast_visit(func, ctx, node->Unary.operand);
        break;
    case AST_BINARY:
        ast_visit(func, ctx, node->Binary.lhs);
        ast_visit(func, ctx, node->Binary.rhs);
        break;
    case AST_VARIABLE_DECL:
        ast_visit(func, ctx, node->Variable_Decl.value);
        break;
    case AST_CONSTANT_DECL:
        ast_visit(func, ctx, node->Constant_Decl.value);
        break;
    case AST_ENUM: break;
    case AST_GROUPING:
        ast_visit(func, ctx, node->Grouping.node);
        break;
    case AST_SUBSCRIPT:
        ast_visit(func, ctx, node->Subscript.load);
        ast_visit(func, ctx, node->Subscript.sub);
        break;
    case AST_FIELD_ACCESS:
        ast_visit(func, ctx, node->Field_Access.load);
        break;
    case AST_IF:
        ast_visit(func, ctx, node->If.cond);
        ast_visit(func, ctx, node->If.then_block);
        ast_visit(func, ctx, node->If.else_block);
        break;
    case AST_FOR:
        ast_visit(func, ctx, node->For.init);
        ast_visit(func, ctx, node->For.cond);
        ast_visit(func, ctx, node->For.step);
        ast_visit(func, ctx, node->For.then_block);
        break;
    case AST_WHILE:
        ast_visit(func, ctx, node->While.cond);
        ast_visit(func, ctx, node->While.then_block);
        break;
    case AST_RETURN:
        ast_visit(func, ctx, node->Return.node);
        break;
    case AST_DEFER:
        ast_visit(func, ctx, node->Defer.node);
        break;
    case AST_BREAK:
        ast_visit(func, ctx, node->Break.node);
        break;
    case AST_CONTINUE:
        ast_visit(func, ctx, node->Continue.node);
        break;
    case AST_AS:
        ast_visit(func, ctx, node->As.node);
        ast_visit(func, ctx, node->As.type_node);
        break;
    case AST_IS:
        ast_visit(func, ctx, node->Is.node);
        ast_visit(func, ctx, node->Is.body);
        break;
    case AST_SWITCH:
        ast_visit(func, ctx, node->Switch.cond);
        ast_visit(func, ctx, node->Switch.cases);
        ast_visit(func, ctx, node->Switch.default_case);
        break;
    case AST_EXTERN:
        LIST_FOREACH(node->Extern.type->Function.parameters) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_FUNCTION:
        ast_visit(func, ctx, node->Function.body);
        LIST_FOREACH(node->Function.parameters) {
            ast_visit(func, ctx, it->data);
        }
        LIST_FOREACH(node->Function.defers) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_CALL:
        LIST_FOREACH(node->Call.args) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_BLOCK:
        LIST_FOREACH(node->Block.stmts) {
            ast_visit(func, ctx, it->data);
        }
        break;
    }
    (*func)(ctx, node);
}

AST* get_arg_from_func(Type* func_t, s64 arg_index) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    assert(arg_index >= 0 && arg_index <= type_function_get_arg_count(func_t));
    AST* node = (AST*)list_at(func_t->Function.parameters, arg_index);
    assert(node);
    return node;
}

AST_Ref_List make_ast_ref_list() {
    AST_Ref_List l;
    l.count = 0;
    l.allocated = AST_REF_LIST_STARTING_ALLOC;
    l.data = xmalloc(l.allocated * sizeof(AST*));
    return l;
}

void ast_ref_list_append(AST_Ref_List* l, AST* a) {
    if (l->count >= l->allocated) {
        l->allocated *= 1.5;
        l->data = xrealloc(l->data, l->allocated * sizeof(AST*));
    }
    l->data[l->count++] = a;
}

void ast_add_edge(AST* a, AST* dep) {
    list_append(a->edges, dep);
}

void ast_replace(AST* a, AST* b) {
    assert(a);
    assert(b);
    info("REPLACED %s -> %s WITH %s -> %s", give_unique_color(ast_to_str(a)), give_unique_color(type_to_str(a->type)), give_unique_color(ast_to_str(b)), give_unique_color(type_to_str(b->type)));
    *a = *b;
    // @Cleanup(marcus) we dont free memory here.
}

//------------------------------------------------------------------------------
//                               AST Tests
//------------------------------------------------------------------------------
void ast_tests(void) {
}

//------------------------------------------------------------------------------
//                               AST Maker Functions
//------------------------------------------------------------------------------

AST* make_ast(AST_Kind kind, Loc_Info loc_info) {
    AST* e = xmalloc(sizeof(AST));
    e->kind = kind;
    e->loc_info = loc_info;
    e->type = NULL;
    e->edges = make_list();
    return e;
}

AST* make_ast_comment(Loc_Info loc_info, char* text) {
    AST* e = make_ast(AST_COMMENT, loc_info);
    e->Comment.text = text;
    return e;
}
AST* make_ast_nop(Loc_Info loc_info) {
    AST* e = make_ast(AST_NOP, loc_info);
    return e;
}

AST* make_ast_typeof(Loc_Info loc_info, AST* node) {
    assert(node);
    AST* e = make_ast(AST_TYPEOF, loc_info);
    e->Typeof.node = node;
    return e;
}

AST* make_ast_module(Loc_Info loc_info, char* name, List* top_level) {
    assert(name);
    AST* e = make_ast(AST_MODULE, loc_info);
    e->Module.name = name;
    e->Module.top_level = top_level;
    return e;
}

AST* make_ast_sizeof(Loc_Info loc_info, AST* node) {
    assert(node);
    AST* e = make_ast(AST_SIZEOF, loc_info);
    e->Sizeof.node = node;
    return e;
}

AST* make_ast_field_access(Loc_Info loc_info, AST* load, char* field) {
    assert(load);
    assert(field);
    AST* e = make_ast(AST_FIELD_ACCESS, loc_info);
    e->Field_Access.load = load;
    e->Field_Access.field = field;
    return e;
}

AST* make_ast_extern(Loc_Info loc_info, Type* type) {
    assert(type);
    AST* e = make_ast(AST_EXTERN, loc_info);
    e->type = type;
    e->Extern.type = type;
    return e;
}

AST* make_ast_load(Loc_Info loc_info, char* str, AST* module) {
    assert(str);
    assert(module);
    AST* e = make_ast(AST_LOAD, loc_info);
    e->Load.str = str;
    e->Load.module = module;
    return e;
}

AST* make_ast_link(Loc_Info loc_info, char* str) {
    assert(str);
    AST* e = make_ast(AST_LINK, loc_info);
    e->Link.str = str;
    return e;
}

AST* make_ast_note(Loc_Info loc_info, AST* node) {
    assert(node);
    AST* e = make_ast(AST_NOTE, loc_info);
    e->Note.node = node;
    return e;
}

AST* make_ast_int(Loc_Info loc_info, s64 value, Type* type) {
    AST* e = make_ast(AST_INT, loc_info);
    e->type = type;
    // e->type = make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
    e->Int.val = value;
    return e;
}

AST* make_ast_float(Loc_Info loc_info, f64 value) {
    AST* e = make_ast(AST_FLOAT, loc_info);
    e->type = make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
    e->Float.val = value;
    return e;
}

AST* make_ast_string(Loc_Info loc_info, char* value) {
    AST* e = make_ast(AST_STRING, loc_info);
    e->type = make_type_pointer(make_type_int(1, 1));
    e->String.val = value;
    return e;
}

AST* make_ast_ident(Loc_Info loc_info, char* ident) {
    assert(ident);
    AST* e = make_ast(AST_IDENT, loc_info);
    e->Ident.name = ident;
    return e;
}

AST* make_ast_struct(Loc_Info loc_info, char* name, List* members) {
    AST* e = make_ast(AST_STRUCT, loc_info);
    e->Struct.name = name;
    e->Struct.members = members;

    List* tps = make_list();
    LIST_FOREACH(members) {
        AST* member = it->data;
        Type_Name_Pair* tp = xmalloc(sizeof(Type_Name_Pair));
        tp->name = get_ast_name(member);
        tp->type = member->type;
        list_append(tps, tp);
    }

    e->type = make_type_struct(name, tps);
    return e;
}

AST* make_ast_enum(Loc_Info loc_info, char* name, List* members) {
    AST* e = make_ast(AST_ENUM, loc_info);
    e->Enum.name = name;
    e->Enum.members = members;
    ;

    List* tps = make_list();
    LIST_FOREACH(members) {
        AST* member = it->data;
        Type_Name_Pair* tp = xmalloc(sizeof(Type_Name_Pair));
        tp->name = get_ast_name(member);
        tp->type = member->type;
        list_append(tps, tp);
    }

    e->type = make_type_enum(name, tps);
    return e;
}

AST* make_ast_function(Loc_Info loc_info, char* name, List* parameters, Type* func_t, AST* body) {
    assert(name);
    assert(parameters);
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    assert(body);
    AST* e = make_ast(AST_FUNCTION, loc_info);
    e->type = func_t;
    e->Function.name = name;
    e->Function.parameters = parameters;
    e->Function.body = body;
    e->Function.defers = make_list();
    return e;
}

AST* make_ast_binary(Loc_Info loc_info, Token_Kind op, AST* lhs, AST* rhs) {
    assert(op != TOKEN_UNKNOWN);
    assert(lhs);
    assert(rhs);
    AST* e = make_ast(AST_BINARY, loc_info);
    e->Binary.op = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

AST* make_ast_unary(Loc_Info loc_info, Token_Kind op, AST* operand) {
    assert(op != TOKEN_UNKNOWN);
    assert(operand);
    AST* e = make_ast(AST_UNARY, loc_info);
    e->Unary.op = op;
    e->Unary.operand = operand;
    return e;
}

AST* make_ast_switch(Loc_Info loc_info, AST* if_statement) {
    AST* e = make_ast(AST_SWITCH, loc_info);
    e->Switch.cond = if_statement->If.cond;
    e->Switch.cases = if_statement->If.then_block;
    e->Switch.default_case = if_statement->If.else_block;
    return e;
}

AST* make_ast_is(Loc_Info loc_info, AST* node, AST* body, bool has_fallthrough) {
    AST* e = make_ast(AST_IS, loc_info);
    e->Is.node = node;
    e->Is.body = body;
    e->Is.has_fallthrough = has_fallthrough;
    return e;
}

AST* make_ast_block(Loc_Info loc_info, List* stmts) {
    AST* e = make_ast(AST_BLOCK, loc_info);
    e->Block.stmts = stmts;
    return e;
}

AST* make_ast_call(Loc_Info loc_info, char* callee, List* args) {
    assert(callee);
    assert(args);
    AST* e = make_ast(AST_CALL, loc_info);
    e->Call.callee = callee;
    e->Call.args = args;
    return e;
}

AST* make_ast_var_args(Loc_Info loc_info) {
    AST* e = make_ast(AST_VAR_ARGS, loc_info);
    return e;
}

AST* make_ast_fallthrough(Loc_Info loc_info) {
    AST* e = make_ast(AST_FALLTHROUGH, loc_info);
    return e;
}

AST* make_ast_grouping(Loc_Info loc_info, AST* node) {
    assert(node);
    AST* e = make_ast(AST_GROUPING, loc_info);
    e->Grouping.node = node;
    return e;
}

AST* make_ast_variable_decl(Loc_Info loc_info, char* name, Type* type, AST* value) {
    // 'value' and 'name' can be NULL
    // assert(type);
    AST* e = make_ast(AST_VARIABLE_DECL, loc_info);
    e->type = type;
    e->Variable_Decl.name = name;
    e->Variable_Decl.type = type;
    e->Variable_Decl.value = value;
    return e;
}

AST* make_ast_constant_decl(Loc_Info loc_info, char* name, AST* value) {
    assert(name);
    assert(value);
    AST* e = make_ast(AST_CONSTANT_DECL, loc_info);
    e->Constant_Decl.name = name;
    e->Constant_Decl.value = value;
    return e;
}

AST* make_ast_subscript(Loc_Info loc_info, AST* load, AST* sub) {
    assert(load);
    assert(sub);
    AST* e = make_ast(AST_SUBSCRIPT, loc_info);
    e->Subscript.load = load;
    e->Subscript.sub = sub;
    return e;
}

AST* make_ast_if(Loc_Info loc_info, AST* cond, AST* then_block, AST* else_block) {
    assert(cond);
    assert(then_block);
    // assert(else_block); // else block is optional
    AST* e = make_ast(AST_IF, loc_info);
    e->If.cond = cond;
    e->If.then_block = then_block;
    e->If.else_block = else_block;
    return e;
}

AST* make_ast_for(Loc_Info loc_info, AST* init, AST* cond, AST* step, AST* then_block) {
    assert(then_block);
    AST* e = make_ast(AST_FOR, loc_info);
    e->For.init = init;
    e->For.cond = cond;
    e->For.step = step;
    e->For.then_block = then_block;
    return e;
}

AST* make_ast_while(Loc_Info loc_info, AST* cond, AST* then_block) {
    assert(cond);
    assert(then_block);
    AST* e = make_ast(AST_WHILE, loc_info);
    e->While.cond = cond;
    e->While.then_block = then_block;
    return e;
}

AST* make_ast_return(Loc_Info loc_info, AST* node) {
    // assert(node); // returns can be called with out an nodeession
    AST* e = make_ast(AST_RETURN, loc_info);
    e->Return.node = node;
    return e;
}

AST* make_ast_defer(Loc_Info loc_info, AST* node) {
    assert(node);
    AST* e = make_ast(AST_DEFER, loc_info);
    e->Defer.node = node;
    return e;
}

AST* make_ast_break(Loc_Info loc_info) {
    AST* e = make_ast(AST_BREAK, loc_info);
    return e;
}

AST* make_ast_continue(Loc_Info loc_info) {
    AST* e = make_ast(AST_CONTINUE, loc_info);
    return e;
}

AST* make_ast_as(Loc_Info loc_info, AST* node, AST* type_node) {
    assert(node);
    assert(type_node);
    AST* e = make_ast(AST_AS, loc_info);
    e->As.node = node;
    e->As.type_node = type_node;
    return e;
}

AST* make_ast_space_separated_identifier_list(Loc_Info loc_info, List* identifiers) {
    assert(identifiers);
    AST* e = make_ast(AST_SPACE_SEPARATED_IDENTIFIER_LIST, loc_info);
    e->Space_Separated_Identifier_List.identifiers = identifiers;
    return e;
}

AST* make_ast_comma_separated_list(Loc_Info loc_info, List* nodes) {
    assert(nodes);
    AST* e = make_ast(AST_COMMA_SEPARATED_LIST, loc_info);
    e->Comma_Separated_List.nodes = nodes;
    return e;
}
