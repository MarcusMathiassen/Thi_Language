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
//                               ast.c
//------------------------------------------------------------------------------

#include "ast.h"
#include "constants.h"
#include "cst.h"    // CST
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
    default:                ERROR_UNHANDLED_KIND(strf("%d", kind));
    case AST_MODULE:        return "AST_MODULE";
    case AST_IS:            return "AST_IS";
    case AST_FALLTHROUGH:   return "AST_FALLTHROUGH";
    case AST_VAR_ARGS:      return "AST_VAR_ARGS";
    case AST_EXTERN:        return "AST_EXTERN";
    case AST_LOAD:          return "AST_LOAD";
    case AST_LINK:          return "AST_LINK";
    case AST_NOTE:          return "AST_NOTE";
    case AST_INT:           return "AST_INT";
    case AST_FLOAT:         return "AST_FLOAT";
    case AST_STRING:        return "AST_STRING";
    case AST_IDENT:         return "AST_IDENT";
    case AST_CALL:          return "AST_CALL";
    case AST_UNARY:         return "AST_UNARY";
    case AST_BINARY:        return "AST_BINARY";
    case AST_GROUPING:      return "AST_GROUPING";
    case AST_SUBSCRIPT:     return "AST_SUBSCRIPT";
    case AST_FIELD_ACCESS:  return "AST_FIELD_ACCESS";
    case AST_AS:            return "AST_AS";
    case AST_BLOCK:         return "AST_BLOCK";
    case AST_STRUCT:        return "AST_STRUCT";
    case AST_ENUM:          return "AST_ENUM";
    case AST_FUNCTION:      return "AST_FUNCTION";
    case AST_VARIABLE_DECL: return "AST_VARIABLE_DECL";
    case AST_CONSTANT_DECL: return "AST_CONSTANT_DECL";
    case AST_IF:            return "AST_IF";
    case AST_FOR:           return "AST_FOR";
    case AST_WHILE:         return "AST_WHILE";
    case AST_RETURN:        return "AST_RETURN";
    case AST_DEFER:         return "AST_DEFER";
    case AST_BREAK:         return "AST_BREAK";
    case AST_CONTINUE:      return "AST_CONTINUE";
    case AST_TYPEOF:        return "AST_TYPEOF";
    case AST_SIZEOF:        return "AST_SIZEOF";
    case AST_SWITCH:        return "AST_SWITCH";
    case AST_COUNT:         return "AST_COUNT";
    }
    // clang-format on
    UNREACHABLE;
    return NULL;
}

char* ast_to_str(AST* expr) {
    if (!expr) return "---";
    // clang-format off
    switch (expr->kind) {
    default:                    ERROR_UNHANDLED_KIND(ast_kind_to_str(expr->kind));
    case AST_MODULE:            return strf("module '%s'= %s", expr->Module.name, ast_to_str(expr->Module.top_level));
    case AST_VAR_ARGS:          return "...";
    case AST_FALLTHROUGH:       return "fallthrough";
    case AST_SWITCH:            return strf("switch %s %s else %s", ast_to_str(expr->Switch.cond), ast_to_str(expr->Switch.cases),
                    ast_to_str(expr->Switch.default_case));
    case AST_IS:                return strf("is %s %s", ast_to_str(expr->Is.expr), ast_to_str(expr->Is.body));
    case AST_TYPEOF:            return strf("typeof %s", ast_to_str(expr->Typeof.expr));
    case AST_SIZEOF:            return strf("sizeof %s", ast_to_str(expr->Sizeof.expr));
    case AST_EXTERN:            return strf("extern %s", type_to_str(expr->type));
    case AST_LOAD:              return strf("load %s", expr->Load.str);
    case AST_LINK:              return strf("link %s", expr->Link.str);
    case AST_DEFER:             return strf("defer %s", ast_to_str(expr->Defer.expr));
    case AST_BREAK:             return "break";
    case AST_CONTINUE:          return "continue";
    case AST_AS:                return strf("%s as %s)", ast_to_str(expr->As.expr), ast_to_str(expr->As.type_expr));
    case AST_RETURN:            return strf("return %s", ast_to_str(expr->Return.expr));
    case AST_FIELD_ACCESS:      return strf("%s.%s", ast_to_str(expr->Field_Access.load), expr->Field_Access.field);
    case AST_NOTE:              return strf("$%s", ast_to_str(expr->Note.expr));
    case AST_INT:               return strf("%lld", expr->Int.val);
    case AST_FLOAT:             return strf("%f", expr->Float.val);
    case AST_STRING:            return strf("\"%s\"", expr->String.val);
    case AST_IDENT:             return strf("%s", expr->Ident.name);
    case AST_UNARY:             return strf("%s(%s)", token_kind_to_str(expr->Unary.op), ast_to_str(expr->Unary.operand));
    case AST_BINARY:            return strf("%s %s %s", ast_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op),
                    ast_to_str(expr->Binary.rhs));
    case AST_VARIABLE_DECL:     return strf("%s: %s = %s", expr->Variable_Decl.name, type_to_str(expr->type),
                    ast_to_str(expr->Variable_Decl.value));
    case AST_CONSTANT_DECL:     return strf("%s :: %s", expr->Constant_Decl.name, ast_to_str(expr->Constant_Decl.value));
    case AST_STRUCT:            return type_to_str(expr->Struct.type);
    case AST_ENUM:              return type_to_str(expr->Enum.type);
    case AST_FUNCTION:          return strf("%s %s", type_to_str(expr->type), ast_to_str(expr->Function.body));
    case AST_GROUPING:          return strf("(%s)", ast_to_str(expr->Grouping.expr));
    case AST_SUBSCRIPT:         return strf("%s[%s]", ast_to_str(expr->Subscript.load), ast_to_str(expr->Subscript.sub));
    case AST_IF:                return strf(expr->If.else_block ? "if %s %s else %s" : "if %s %s", ast_to_str(expr->If.cond),
                    ast_to_str(expr->If.then_block), ast_to_str(expr->If.else_block));
    case AST_FOR:               return strf("for %s, %s, %s %s", ast_to_str(expr->For.init), ast_to_str(expr->For.cond),
                    ast_to_str(expr->For.step), ast_to_str(expr->For.then_block));
    case AST_WHILE:             return strf("while %s %s", ast_to_str(expr->While.cond), ast_to_str(expr->While.then_block));
    case AST_BLOCK: {
        string s = string_create("{ ");
        LIST_FOREACH(expr->Block.stmts) {
            string_append_f(&s, "%s; ", ast_to_str(it->data));
        }
        string_append(&s, "}");
        return string_data(&s);
    }
    case AST_CALL: {
        string s   = string_create_f("%s(", expr->Call.callee);
        LIST_FOREACH(expr->Call.args) {
            string_append(&s, ast_to_str(it->data));
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

char* ast_json_prelude(AST* expr) {
    return strf(
        "\"token\": {\"kind\": \"%s\", \"value\": \"%s\"}, \"type\": "
        "%s, \"%s\"",
        token_kind_to_str(expr->t.kind),
        expr->t.value,
        type_to_json(expr->type),
        ast_kind_to_str(expr->kind));
}

char* ast_to_json(AST* expr) {
    if (!expr) return "\"NULL\"";
    // clang-format off
    switch (expr->kind) {
    default:                ERROR_UNHANDLED_KIND(ast_kind_to_str(expr->kind));
    case AST_MODULE:        return strf("{%s:{\"name\":\"%s\",\"top_level\":%s}}", ast_json_prelude(expr), expr->Module.name, ast_to_json(expr->Module.top_level));
    case AST_SWITCH:        return strf("{%s:{\"cond\":%s,\"cases\":%s,\"default\":%s}}", ast_json_prelude(expr), ast_to_json(expr->Switch.cond), ast_to_json(expr->Switch.cases), ast_to_json(expr->Switch.default_case));
    case AST_IS:            return strf("{%s:{\"case\":%s,\"body\":%s,\"has_fallthrough\":%s}}", ast_json_prelude(expr), ast_to_json(expr->Is.expr), ast_to_json(expr->Is.body), expr->Is.has_fallthrough ? "true" : "false");
    case AST_FIELD_ACCESS:  return strf("{%s:{\"load\":%s,\"field\":\"%s\"}}", ast_json_prelude(expr), ast_to_json(expr->Field_Access.load), expr->Field_Access.field);
    case AST_TYPEOF:        return strf("{%s:{\"typeof\": %s}}", ast_json_prelude(expr), ast_to_json(expr->Typeof.expr));
    case AST_SIZEOF:        return strf("{%s:{\"sizeof\": %s}}", ast_json_prelude(expr), ast_to_json(expr->Sizeof.expr));
    case AST_AS:            return strf("{%s:{\"expr\": %s, \"type_expr\":%s}}", ast_json_prelude(expr), ast_to_json(expr->As.expr), ast_to_json(expr->As.type_expr));
    case AST_EXTERN:        return strf("{%s:{\"extern\": %s}}", ast_json_prelude(expr), type_to_json(expr->type));
    case AST_LOAD:          return strf("{%s:{\"load\": \"%s\"}}", ast_json_prelude(expr), expr->Load.str);
    case AST_VAR_ARGS:      return strf("{%s:{\"var_args\": ...}}", ast_json_prelude(expr));
    case AST_LINK:          return strf("{%s:{\"link\": \"%s\"}}", ast_json_prelude(expr), expr->Link.str);
    case AST_SUBSCRIPT:     return strf("{%s:{\"load\": %s, \"sub\": %s}}", ast_json_prelude(expr), ast_to_json(expr->Subscript.load), ast_to_json(expr->Subscript.sub));
    case AST_CONTINUE:      return strf("{%s:{%s}}", ast_json_prelude(expr), "\"continue\"");
    case AST_FALLTHROUGH:   return strf("{%s: true}", ast_json_prelude(expr));
    case AST_BREAK:         return strf("{%s:{%s}}", ast_json_prelude(expr), "\"break\"");
    case AST_DEFER:         return strf("{%s:{\"expr\": %s}}", ast_json_prelude(expr), ast_to_json(expr->Defer.expr));
    case AST_NOTE:          return strf("{%s:{\"note\":\"%s\"}}", ast_json_prelude(expr), ast_to_json(expr->Note.expr));
    case AST_INT:           return strf("{%s:{\"value\": %lld}}", ast_json_prelude(expr), expr->Int.val);
    case AST_STRING:        return strf("{%s:{\"value\": \"%s\"}}", ast_json_prelude(expr), expr->String.val);
    case AST_FLOAT:         return strf("{%s:{\"value\": %f}}", ast_json_prelude(expr), expr->Float.val);
    case AST_IDENT:         return strf("{%s:{\"ident\": \"%s\"}}", ast_json_prelude(expr), expr->Ident.name);
    case AST_UNARY:         return strf("{%s:{\"op\": \"%s\", \"expr\": \"%s\"}}", ast_json_prelude(expr), token_kind_to_str(expr->Unary.op), ast_to_json(expr->Unary.operand));
    case AST_BINARY:        return strf("{%s:{\"op\": \"%s\", \"lhs\": %s, \"rhs\": %s}}", ast_json_prelude(expr), token_kind_to_str(expr->Binary.op), ast_to_json(expr->Binary.lhs), ast_to_json(expr->Binary.rhs));
    case AST_RETURN:        return strf("{%s:{\"expr\": %s}}", ast_json_prelude(expr), ast_to_json(expr->Return.expr));
    case AST_VARIABLE_DECL: return strf("{%s:{\"name\": \"%s\", \"type\": %s, \"value\": %s}}", ast_json_prelude(expr), expr->Variable_Decl.name, type_to_json(expr->type), ast_to_json(expr->Variable_Decl.value));
    case AST_CONSTANT_DECL: return strf("{%s:{\"name\": \"%s\", \"value\": %s}}", ast_json_prelude(expr), expr->Constant_Decl.name, ast_to_json(expr->Constant_Decl.value));
    case AST_FUNCTION:      return strf("{%s:{\"type\": %s, \"body\": %s }}", ast_json_prelude(expr), type_to_json(expr->type), ast_to_json(expr->Function.body));
    case AST_STRUCT:        return strf("{%s:{\"type\": %s}}", ast_json_prelude(expr), type_to_json(expr->Struct.type));
    case AST_ENUM:          return strf("{%s:{\"type\": %s}}", ast_json_prelude(expr), type_to_json(expr->Enum.type));
    case AST_GROUPING:      return strf("{%s:{\"expr\": %s}}", ast_json_prelude(expr), ast_to_json(expr->Grouping.expr));
    case AST_WHILE:         return strf("{%s:{\"cond\": %s, \"then_block\": %s}}", ast_json_prelude(expr), ast_to_json(expr->While.cond), ast_to_json(expr->While.then_block));
    case AST_FOR:           return strf("{%s:{\"init\": %s, \"cond\": %s, \"step\": %s, ""\"then_block\": %s }}", ast_json_prelude(expr), ast_to_json(expr->For.init), ast_to_json(expr->For.cond), ast_to_json(expr->For.step), ast_to_json(expr->For.then_block));
    case AST_IF:            return strf("{%s:{\"cond\": %s, \"then_block\": %s, \"else_block\": %s }}", ast_json_prelude(expr), ast_to_json(expr->If.cond), ast_to_json(expr->If.then_block), ast_to_json(expr->If.else_block));
    // clang-format on
    case AST_BLOCK: {
        string str = string_create(strf("{%s: [", ast_json_prelude(expr)));
        LIST_FOREACH(expr->Block.stmts) {
            string_append(&str, ast_to_json(it->data));
            if (it->next) string_append(&str, ", ");
        }
        string_append(&str, "]}");
        return str.c_str;
    } break;
    case AST_CALL: {
        string str = string_create("");
        string_append_f(&str, "{%s:{\"callee\": \"%s\", ", ast_json_prelude(expr), expr->Call.callee);
        string_append(&str, "\"args\": [");
        s64 arg_count = expr->Call.args->count;
        s64 counter   = 0;
        LIST_FOREACH(expr->Call.args) {
            string_append(&str, ast_to_json(it->data));
            if (counter != arg_count - 1) string_append(&str, ",");
            counter += 1;
        }
        string_append(&str, "]}}");
        return str.c_str;
    } break;
    }
    UNREACHABLE;
    return NULL;
}

void ast_visit(void (*func)(void*, AST*), void* ctx, AST* expr) {
    if (!expr) return;
    assert(func);
    switch (expr->kind) {
    default: ERROR_UNHANDLED_KIND(ast_kind_to_str(expr->kind));

    // clang-format off
    case AST_MODULE: ast_visit(func, ctx, expr->Module.top_level); break;
    case AST_TYPEOF: ast_visit(func, ctx, expr->Typeof.expr);      break;
    case AST_SIZEOF: ast_visit(func, ctx, expr->Sizeof.expr);      break;
    case AST_NOTE:   ast_visit(func, ctx, expr->Note.expr);        break;
    
    case AST_FALLTHROUGH: break;
    case AST_LOAD:        break;
    case AST_LINK:        break;
    case AST_INT:         break;
    case AST_FLOAT:       break;
    case AST_STRING:      break;
    case AST_IDENT:       break;
    case AST_STRUCT:      break;
        // clang-format on

    case AST_UNARY:
        ast_visit(func, ctx, expr->Unary.operand);
        break;
    case AST_BINARY:
        ast_visit(func, ctx, expr->Binary.lhs);
        ast_visit(func, ctx, expr->Binary.rhs);
        break;
    case AST_VARIABLE_DECL:
        ast_visit(func, ctx, expr->Variable_Decl.value);
        break;
    case AST_CONSTANT_DECL:
        ast_visit(func, ctx, expr->Constant_Decl.value);
        break;
    case AST_ENUM: break;
    case AST_GROUPING:
        ast_visit(func, ctx, expr->Grouping.expr);
        break;
    case AST_SUBSCRIPT:
        ast_visit(func, ctx, expr->Subscript.load);
        ast_visit(func, ctx, expr->Subscript.sub);
        break;
    case AST_FIELD_ACCESS:
        ast_visit(func, ctx, expr->Field_Access.load);
        break;
    case AST_IF:
        ast_visit(func, ctx, expr->If.cond);
        ast_visit(func, ctx, expr->If.else_block);
        ast_visit(func, ctx, expr->If.then_block);
        break;
    case AST_FOR:
        ast_visit(func, ctx, expr->For.init);
        ast_visit(func, ctx, expr->For.cond);
        ast_visit(func, ctx, expr->For.step);
        ast_visit(func, ctx, expr->For.then_block);
        break;
    case AST_WHILE:
        ast_visit(func, ctx, expr->While.cond);
        ast_visit(func, ctx, expr->While.then_block);
        break;
    case AST_RETURN:
        ast_visit(func, ctx, expr->Return.expr);
        break;
    case AST_DEFER:
        ast_visit(func, ctx, expr->Defer.expr);
        break;
    case AST_BREAK:
        ast_visit(func, ctx, expr->Break.expr);
        break;
    case AST_CONTINUE:
        ast_visit(func, ctx, expr->Continue.expr);
        break;
    case AST_AS:
        ast_visit(func, ctx, expr->As.expr);
        ast_visit(func, ctx, expr->As.type_expr);
        break;
    case AST_IS:
        ast_visit(func, ctx, expr->Is.expr);
        ast_visit(func, ctx, expr->Is.body);
        break;
    case AST_SWITCH:
        ast_visit(func, ctx, expr->Switch.cond);
        ast_visit(func, ctx, expr->Switch.cases);
        ast_visit(func, ctx, expr->Switch.default_case);
        break;
    case AST_EXTERN:
        LIST_FOREACH(expr->Extern.type->Function.args) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_FUNCTION:
        ast_visit(func, ctx, expr->Function.body);
        LIST_FOREACH(expr->Function.type->Function.args) {
            ast_visit(func, ctx, it->data);
        }
        LIST_FOREACH(expr->Function.defers) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_CALL:
        LIST_FOREACH(expr->Call.args) {
            ast_visit(func, ctx, it->data);
        }
        break;
    case AST_BLOCK:
        LIST_FOREACH(expr->Block.stmts) {
            ast_visit(func, ctx, it->data);
        }
        break;
    }
    (*func)(ctx, expr);
}

AST* get_arg_from_func(Type* func_t, s64 arg_index) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    assert(arg_index >= 0 && arg_index <= type_function_get_arg_count(func_t));
    AST* expr = (AST*)list_at(func_t->Function.args, arg_index);
    assert(expr);
    return expr;
}

AST_Ref_List
make_ast_ref_list() {
    AST_Ref_List l;
    l.count     = 0;
    l.allocated = AST_REF_LIST_STARTING_ALLOC;
    l.data      = xmalloc(l.allocated * sizeof(AST*));
    return l;
}

void ast_ref_list_append(AST_Ref_List* l, AST* a) {
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(AST*));
    }
    l->data[l->count] = a;
    l->count += 1;
}

void ast_replace(AST* a, AST* b) {
    assert(a);
    assert(b);
    info("REPLACED %s WITH %s", give_unique_color(ast_to_str(a)), give_unique_color(ast_to_str(b)));
    *a = *b;
    // CLEANUP
}

//------------------------------------------------------------------------------
//                               AST Parser
//------------------------------------------------------------------------------

// #define DEBUG_START \
//     assert(ctx);    \
//     info("%s: %s", __func__, cst_to_str(curr(ctx)));

// typedef struct {
//     List_Node* iter;
//     CST*       prev;
//     CST*       curr;
//     List*      loads;
//     AST*       llast_if_statement; // used for dangling else
//     AST*       olast_if_statement; // used for dangling else
// } AST_Parser_Context;

// //------------------------------------------------------------------------------
// //              Each construct of the language gets its own function
// //------------------------------------------------------------------------------

// // clang-format off
// static AST* parse_statement                (AST_Parser_Context* ctx);
// static AST* parse_primary                  (AST_Parser_Context* ctx);
// static AST* parse_identifier               (AST_Parser_Context* ctx);
// static AST* parse_expression_identifier    (AST_Parser_Context* ctx);
// static AST* parse_variable_decl            (AST_Parser_Context* ctx, char* ident);
// static AST* parse_constant_decl            (AST_Parser_Context* ctx, char* ident);
// static AST* parse_function_call            (AST_Parser_Context* ctx, char* ident);
// static AST* parse_block                    (AST_Parser_Context* ctx);
// static AST* parse_return                   (AST_Parser_Context* ctx);
// static AST* parse_note                     (AST_Parser_Context* ctx);
// static AST* parse_expression               (AST_Parser_Context* ctx);
// static AST* parse_unary                    (AST_Parser_Context* ctx);
// static AST* parse_binary                   (AST_Parser_Context* ctx, s8 min_prec, AST* lhs);
// static AST* parse_integer                  (AST_Parser_Context* ctx);
// static AST* parse_float                    (AST_Parser_Context* ctx);
// static AST* parse_parens                   (AST_Parser_Context* ctx);
// static AST* parse_defer                    (AST_Parser_Context* ctx);
// static AST* parse_load                     (AST_Parser_Context* ctx);
// static AST* parse_extern                   (AST_Parser_Context* ctx);
// static AST* parse_link                     (AST_Parser_Context* ctx);
// static AST* parse_if                       (AST_Parser_Context* ctx);
// static AST* parse_is                       (AST_Parser_Context* ctx);
// static AST* parse_fallthrough              (AST_Parser_Context* ctx);
// static AST* parse_dangling_else            (AST_Parser_Context* ctx);
// static AST* parse_for                      (AST_Parser_Context* ctx);
// static AST* parse_while                    (AST_Parser_Context* ctx);
// static AST* parse_break                    (AST_Parser_Context* ctx);
// static AST* parse_continue                 (AST_Parser_Context* ctx);
// static AST* parse_string                   (AST_Parser_Context* ctx);
// static AST* parse_enum                     (AST_Parser_Context* ctx);
// static AST* parse_struct                   (AST_Parser_Context* ctx);
// static AST* parse_comma_delim_list         (AST_Parser_Context* ctx);
// static AST* parse_prefix                   (AST_Parser_Context* ctx);
// static AST* parse_postfix                  (AST_Parser_Context* ctx);
// // clang-format on

// //------------------------------------------------------------------------------
// //                               Helpers
// //------------------------------------------------------------------------------

// // clang-format off
// Type* parse_enum_signature             (AST_Parser_Context* ctx, char* enum_name);
// Type* parse_struct_signature           (AST_Parser_Context* ctx, char* struct_name);
// Type* parse_function_signature         (AST_Parser_Context* ctx, char* func_name);
// Type* parse_extern_function_signature  (AST_Parser_Context* ctx, char* func_name);
// // clang-format on
// //

// #define BIN_OP_COUNT 35
// struct
// {
//     Token_Kind kind;
//     s32        p;
// } binop_precedence[BIN_OP_COUNT] = {
//     {TOKEN_OPEN_PAREN, 15},   // ()
//     {TOKEN_OPEN_BRACKET, 15}, // []
//     {TOKEN_DOT, 15},          // .
//     {TOKEN_ASTERISK, 13},     // *
//     {TOKEN_FWSLASH, 13},      // /
//     {TOKEN_PERCENT, 13},      // %
//     {TOKEN_PLUS, 12},         // +
//     {TOKEN_MINUS, 12},        // -
//     {TOKEN_LT_LT, 11},        // <<
//     {TOKEN_GT_GT, 11},        // >>
//     {TOKEN_LT, 10},           // <
//     {TOKEN_LT_EQ, 10},        // <=
//     {TOKEN_GT, 10},           // >
//     {TOKEN_GT_EQ, 10},        // >=
//     {TOKEN_EQ_EQ, 9},         // ==
//     {TOKEN_BANG_EQ, 9},       // !=
//     {TOKEN_AND, 8},           // &
//     {TOKEN_HAT, 7},           // ^
//     {TOKEN_PIPE, 6},          // |
//     {TOKEN_AND_AND, 5},       // &&
//     {TOKEN_PIPE_PIPE, 4},     // ||
//     {TOKEN_QUESTION_MARK, 3}, // ?
//     {TOKEN_COLON, 3},         // :
//     {TOKEN_EQ, 2},            // =
//     {TOKEN_PLUS_EQ, 2},       // +=
//     {TOKEN_MINUS_EQ, 2},      // -=
//     {TOKEN_ASTERISK_EQ, 2},   // *=
//     {TOKEN_FWSLASH_EQ, 2},    // /=
//     {TOKEN_PERCENT_EQ, 2},    // %=
//     {TOKEN_AND_EQ, 2},        // &=
//     {TOKEN_HAT_EQ, 2},        // ^=
//     {TOKEN_PIPE_EQ, 2},       // |=
//     {TOKEN_LT_LT_EQ, 2},      // <<=
//     {TOKEN_GT_GT_EQ, 2},      // >>=
//                               // {TOKEN_COMMA, 1},          // ,
// };

// static Token_Kind tok_kind(AST_Parser_Context* ctx) {
//     assert(is(ctx,CST_TOKEN));
//     return curr_tok(ctx).kind;
// }
// static char* tok_val(AST_Parser_Context* ctx) {
//     assert(is(ctx,CST_TOKEN));
//     return curr_tok(ctx).value;
// }

// static Type* get_type(AST_Parser_Context* ctx) {
//     DEBUG_START;

//     char* type_name = tok_val(ctx);

//     // // Check if var arg
//     // if (tok_is(ctx, TOKEN_DOT_DOT_DOT)) {
//     //     eat_kind(ctx, TOKEN_DOT_DOT_DOT);
//     //     return make_type_var_args();
//     // }

//     eat_tok_kind(ctx, TOKEN_IDENTIFIER);

//     Type* type = NULL;
//     if (!type) {
//         type       = make_type_unresolved(type_name);
//         type->name = type_name;
//     }
//     type->name = type_name;

//     switch (ctx->curr_tok.kind) {
//     case THI_SYNTAX_POINTER: {
//         while (tok_is(ctx, THI_SYNTAX_POINTER)) {
//             eat_kind(ctx, THI_SYNTAX_POINTER);
//             type = make_type_pointer(type);
//         }
//     } break;
//     case TOKEN_OPEN_BRACKET: {
//         while (tok_is(ctx, TOKEN_OPEN_BRACKET)) {
//             eat_kind(ctx, TOKEN_OPEN_BRACKET);
//             s64 size = 0;
//             if (tok_is(ctx, TOKEN_INTEGER) || tok_is(ctx, TOKEN_HEX)) {
//                 size = get_integer(ctx);
//             }
//             eat_kind(ctx, TOKEN_CLOSE_BRACKET);
//             type = make_type_array(type, size);
//         }
//     } break;
//     default: break;
//     }

//     return type;
// }

// static s64 get_integer(AST_Parser_Context* ctx) {
//     DEBUG_START;

//     s64 value = 0;
//     switch (ctx->curr_tok.kind) {
//     case TOKEN_CHAR: {
//         u8 c = ctx->curr_tok.value[0];
//         if (c == '\\') {
//             u8 c = ctx->curr_tok.value[1];
//             switch (c) {
//             case 'a': value = 7; break;
//             case 'n': value = 10; break;
//             case 't': value = 9; break;
//             case '\\': value = 92; break;
//             case '\'': value = 27; break;
//             case '"': value = 22; break;
//             }
//         } else
//             value = c;
//         break;
//     }
//     case TOKEN_INTEGER: value = atoll(ctx->curr_tok.value); break;
//     case TOKEN_HEX: value = strtoll(ctx->curr_tok.value, NULL, 0); break;
//     default: ERROR_UNHANDLED_KIND(token_kind_to_str(ctx->curr_tok.kind));
//     }
//     eat(ctx);

//     return value;
// }

// static f64 get_float(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     f64 value = atof(tok_val(ctx));
//     eat_tok_kind(ctx, TOKEN_FLOAT);
//     return value;
// }

// static int get_tok_precedence(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     for (int i = 0; i < BIN_OP_COUNT; ++i)
//         if (tok_is(ctx, binop_precedence[i].kind))
//             return binop_precedence[i].p;
//     return -1;
// }

// static bool tok_is(AST_Parser_Context* ctx, Token_Kind kind) {
//     return curr_tok(ctx).kind == kind;
// }

// static void eat_tok_kind(AST_Parser_Context* ctx, Token_Kind kind) {
//     Token_Kind tk = ctx->curr_tok.kind;
//     if (tk != kind) {
//         error("Expected '%s' got '%s'", token_kind_to_str(kind), token_kind_to_str(tk));
//     }
//     eat(ctx);
// }

// static void set_if_statement(AST_Parser_Context* ctx, AST* if_statement) {
//     ctx->olast_if_statement = ctx->llast_if_statement;
//     ctx->llast_if_statement = if_statement;
// }
// static void restore_if_statement(AST_Parser_Context* ctx) {
//     ctx->llast_if_statement = ctx->olast_if_statement;
// }
// static void set_dangling_else(AST_Parser_Context* ctx, AST* else_block) {
//     ctx->llast_if_statement->If.else_block = else_block;
// }

// static CST* curr(AST_Parser_Context* ctx) {
//     assert(ctx);
//     return ctx->curr;
// }

// static CST* prev(AST_Parser_Context* ctx) {
//     assert(ctx);
//     return ctx->prev;
// }

// static CST_Kind kind(AST_Parser_Context* ctx) {
//     assert(ctx);
//     return ctx->curr->kind;
// }

// static char* value(AST_Parser_Context* ctx) {
//     assert(ctx);
//     assert(ctx->curr->kind == CST_TOKEN);
//     return ctx->curr->token.value;
// }

// static bool same_line(AST_Parser_Context* ctx) {
//     assert(ctx);
//     s64 l1 = curr(ctx)->loc_info.line_pos;
//     s64 l2 = prev(ctx)->loc_info.line_pos;
//     return l1 == l2;
// }

// static bool is(AST_Parser_Context* ctx, CST_Kind k) {
//     assert(ctx);
//     return kind(ctx) == k;
// }

// static void eat(AST_Parser_Context* ctx) {
//     ctx->prev = ctx->curr;
//     ctx->curr = ctx->iter->data;
//     if (ctx->iter->next)
//         ctx->iter = ctx->iter->next;
//     else
//         error("cant eat no more");
// }

// static void eat_kind(AST_Parser_Context* ctx, CST_Kind expected_kind) {
//     assert(ctx);
//     if (kind(ctx) != expected_kind) {
//         error("Expected '%s' got '%s'", cst_kind_to_str(expected_kind), cst_kind_to_str(kind(ctx)));
//     }
//     eat(ctx);
// }

// static AST* parse_constant_decl(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     /*
//         <constant_decl>         ::= <ident> <expr>
//     */
//     switch (kind(ctx)) {
//     default: ERROR_UNHANDLED_KIND(cst_kind_to_str(kind(ctx)));
//     }
//     UNREACHABLE;
//     return NULL;
// }

// static AST* parse_identifier(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     /*
//         <ident>                 ::= "_" + <character | digit>..
//                                 | <character | digit>..
//     */
//     /*
//         <variable_decl>         ::= <ident> <ident>
//                                   | <ident> "=" <expr>
//         <constant_decl>         ::= <ident> <expr>

//         <struct_def>            ::= <ident> <var_decl_block>
//         <enum_def>              ::= <ident> <constant_decl_block>

//         <func_call>             ::= <ident> "(" <expr>.. ")"
//         <func_def_extern>       ::= <ident> "(" <ident>.. ")" <ident>
//         <func_def>              ::= <ident> "(" <var_decl>.. ")" <ident> <block>
//         <func_lambda>           ::= <ident> '=>' <block>
//     */
//     switch (kind(ctx)) {
//     default: ERROR_UNHANDLED_KIND(cst_kind_to_str(kind(ctx)));
//     }
//     UNREACHABLE;
//     return NULL;
// }

// static AST* parse_prefix(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     return parse_unary(ctx);
// }
// static AST* parse_postfix(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     AST* primary_expr = parse_primary(ctx);
//     return parse_postfix_tail(ctx, primary_expr);
// }
// static AST* parse_unary(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     AST* unary = NULL;

//     for (s8 i = 0; i < UNARY_OP_COUNT; ++i) {
//         if (tok_is(ctx, unary_ops[i])) {
//             Token_Kind op = tokKind(ctx);
//             eat(ctx);
//             AST* operand = parse_unary(ctx);
//             if (operand) {
//                 unary = make_ast_unary(curr_tok(ctx), op, operand);
//             }
//         }
//     }

//     if (unary) {
//         if (unary->Unary.op == TOKEN_SIZEOF) {
//             ast_replace(unary, make_ast_sizeof(unary->t, unary->Unary.operand));
//         } else if (unary->Unary.op == TOKEN_TYPEOF) {
//             ast_replace(unary, make_ast_typeof(unary->t, unary->Unary.operand));
//         }
//     }

//     // If the current token is not an operator, it must be a primary expression.
//     return unary ? unary : parse_postfix(ctx);
// }

// static AST* parse_expression(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     AST* lhs  = parse_prefix(ctx);
//     AST* expr = lhs ? parse_binary(ctx, 0, lhs) : NULL;
//     return expr;
// }

// static AST* parse_float(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     AST* res = make_ast_float(curr_tok(ctx), get_float(ctx));
//     return res;
// }

// static AST* parse_integer(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     AST* res = make_ast_int(curr_tok(ctx), get_integer(ctx));
//     return res;
// }

// static AST* parse_parens(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     eat_kind(ctx, TOKEN_OPEN_PAREN);
//     AST* expr = parse_expression(ctx);
//     eat_kind(ctx, TOKEN_CLOSE_PAREN);
//     return make_ast_grouping(curr_tok(ctx), expr);
// }

// static AST* parse_primary(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     // clang-format off
//     switch (ctx->curr->token.kind) {
//     default: ERROR_UNHANDLED_KIND(cst_kind_to_str(kind(ctx)));
//     // case TOKEN_DOT_DOT_DOT: eat(ctx); return make_ast_var_args(curr_tok(ctx));
//     // case TOKEN_TRUE:        eat(ctx); return make_ast_int(curr_tok(ctx), 1);
//     // case TOKEN_FALSE:       eat(ctx); return make_ast_int(curr_tok(ctx), 0);
//     case TOKEN_IDENTIFIER:  return parse_identifier(ctx);
//     // case TOKEN_DOLLAR_SIGN: return parse_note(ctx);
//     // case TOKEN_FLOAT:       return parse_float(ctx);
//     // case TOKEN_CHAR:        // fallthrough
//     // case TOKEN_HEX:         // fallthrough
//     // case TOKEN_INTEGER:     return parse_integer(ctx);
//     // case TOKEN_STRING:      return parse_string(ctx);
//     // case TOKEN_OPEN_PAREN:  return parse_parens(ctx);
//     }
//     // clang-format on
//     UNREACHABLE;
//     return NULL;
// }

// static AST* parse_statement(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     // clang-format off
//     switch (ctx->curr->token.kind) {
//     default: ERROR_UNHANDLED_KIND(cst_kind_to_str(kind(ctx)));
//     // case TOKEN_ENUM:                return parse_enum(ctx);
//     // case TOKEN_STRUCT:              return parse_struct(ctx);
//     // case TOKEN_RETURN:              return parse_return(ctx);
//     // case TOKEN_BREAK:               return parse_break(ctx);
//     // case TOKEN_CONTINUE:            return parse_continue(ctx);
//     // case TOKEN_IF:                  return parse_if(ctx);
//     // case TOKEN_IS:                  return parse_is(ctx);
//     // case TOKEN_FALLTHROUGH:         return parse_fallthrough(ctx);
//     // case TOKEN_ELSE:                return parse_dangling_else(ctx);
//     // case TOKEN_DEFER:               return parse_defer(ctx);
//     // case TOKEN_FOR:                 return parse_for(ctx);
//     // case TOKEN_WHILE:               return parse_while(ctx);
//     // case TOKEN_EXTERN:              return parse_extern(ctx);
//     // case TOKEN_LOAD:                return parse_load(ctx);
//     // case TOKEN_LINK:                return parse_link(ctx);
//     }
//     // clang-format on
//     UNREACHABLE;
//     return NULL;
// }

// static AST* parse_node(AST_Parser_Context* ctx) {
//     DEBUG_START;
//     switch (kind(ctx)) {
//     default: ERROR_UNHANDLED_KIND(cst_kind_to_str(kind(ctx)));
//     case CST_IDENTIFIER: return parse_expression(ctx);
//     case CST_TOKEN: return parse_statement(ctx);
//     }
//     UNREACHABLE;
//     return NULL;
// }

// List* generate_ast_from_cst(List* cst) {
//     info("Generating AST from CST..");
//     assert(cst);

//     AST_Parser_Context ctx;
//     ctx.iter  = cst->head;
//     ctx.loads = make_list();

//     eat(&ctx); // prep

//     List* ast = make_list();
//     LIST_FOREACH(cst) {
//         AST* node = parse_node(&ctx);
//         assert(node);
//         list_append(ast, node);
//     }
//     return ast;
// }

//------------------------------------------------------------------------------
//                               AST Tests
//------------------------------------------------------------------------------
void ast_tests(void) {
}

//------------------------------------------------------------------------------
//                               AST Maker Functions
//------------------------------------------------------------------------------

AST* make_ast(AST_Kind kind, Token t) {
    AST* e  = xmalloc(sizeof(AST));
    e->kind = kind;
    e->t    = t;
    e->type = NULL;
    return e;
}

AST* make_ast_typeof(Token t, AST* expr) {
    assert(expr);
    AST* e         = make_ast(AST_TYPEOF, t);
    e->Typeof.expr = expr;
    return e;
}

AST* make_ast_module(Token t, char* name, AST* top_level) {
    assert(name);
    AST* e              = make_ast(AST_MODULE, t);
    e->Module.name      = name;
    e->Module.top_level = top_level;
    return e;
}

AST* make_ast_sizeof(Token t, AST* expr) {
    assert(expr);
    AST* e         = make_ast(AST_SIZEOF, t);
    e->Sizeof.expr = expr;
    return e;
}

AST* make_ast_field_access(Token t, AST* load, char* field) {
    assert(load);
    assert(field);
    AST* e                = make_ast(AST_FIELD_ACCESS, t);
    e->Field_Access.load  = load;
    e->Field_Access.field = field;
    return e;
}

AST* make_ast_extern(Token t, Type* type) {
    assert(type);
    AST* e         = make_ast(AST_EXTERN, t);
    e->type        = type;
    e->Extern.type = type;
    return e;
}

AST* make_ast_load(Token t, char* str) {
    assert(str);
    AST* e      = make_ast(AST_LOAD, t);
    e->Load.str = str;
    return e;
}

AST* make_ast_link(Token t, char* str) {
    assert(str);
    AST* e      = make_ast(AST_LINK, t);
    e->Link.str = str;
    return e;
}

AST* make_ast_note(Token t, AST* expr) {
    assert(expr);
    AST* e       = make_ast(AST_NOTE, t);
    e->Note.expr = expr;
    return e;
}

AST* make_ast_int(Token t, s64 value) {
    AST* e     = make_ast(AST_INT, t);
    e->type    = make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
    e->Int.val = value;
    return e;
}

AST* make_ast_float(Token t, f64 value) {
    AST* e       = make_ast(AST_FLOAT, t);
    e->type      = make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
    e->Float.val = value;
    return e;
}

AST* make_ast_string(Token t, char* value) {
    AST* e        = make_ast(AST_STRING, t);
    e->type       = make_type_pointer(make_type_int(1, 1));
    e->String.val = value;
    return e;
}

AST* make_ast_ident(Token t, char* ident) {
    assert(ident);
    AST* e        = make_ast(AST_IDENT, t);
    e->Ident.name = ident;
    return e;
}

AST* make_ast_struct(Token t, Type* struct_t) {
    assert(struct_t);
    AST* e         = make_ast(AST_STRUCT, t);
    e->type        = struct_t;
    e->Struct.type = struct_t;
    return e;
}

AST* make_ast_enum(Token t, Type* enum_t) {
    assert(enum_t);
    AST* e       = make_ast(AST_ENUM, t);
    e->type      = enum_t;
    e->Enum.type = enum_t;
    return e;
}

AST* make_ast_function(Token t, Type* func_t, AST* body) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    AST* e             = make_ast(AST_FUNCTION, t);
    e->type            = func_t;
    e->Function.type   = func_t;
    e->Function.body   = body;
    e->Function.defers = make_list();
    return e;
}

AST* make_ast_binary(Token t, Token_Kind op, AST* lhs, AST* rhs) {
    assert(op != TOKEN_UNKNOWN);
    assert(lhs);
    assert(rhs);
    AST* e        = make_ast(AST_BINARY, t);
    e->t          = t;
    e->Binary.op  = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

AST* make_ast_unary(Token t, Token_Kind op, AST* operand) {
    assert(op != TOKEN_UNKNOWN);
    assert(operand);
    AST* e           = make_ast(AST_UNARY, t);
    e->Unary.op      = op;
    e->Unary.operand = operand;
    return e;
}

AST* make_ast_switch(Token t, AST* if_statement) {
    AST* e                 = make_ast(AST_SWITCH, t);
    e->Switch.cond         = if_statement->If.cond;
    e->Switch.cases        = if_statement->If.then_block;
    e->Switch.default_case = if_statement->If.else_block;
    return e;
}

AST* make_ast_is(Token t, AST* expr, AST* body, bool has_fallthrough) {
    AST* e                = make_ast(AST_IS, t);
    e->Is.expr            = expr;
    e->Is.body            = body;
    e->Is.has_fallthrough = has_fallthrough;
    return e;
}

AST* make_ast_block(Token t, List* stmts) {
    AST* e         = make_ast(AST_BLOCK, t);
    e->Block.stmts = stmts;
    return e;
}

AST* make_ast_call(Token t, char* callee, List* args) {
    assert(callee);
    assert(args);
    AST* e         = make_ast(AST_CALL, t);
    e->Call.callee = callee;
    e->Call.args   = args;
    return e;
}

AST* make_ast_var_args(Token t) {
    AST* e = make_ast(AST_VAR_ARGS, t);
    return e;
}

AST* make_ast_fallthrough(Token t) {
    AST* e = make_ast(AST_FALLTHROUGH, t);
    return e;
}

AST* make_ast_grouping(Token t, AST* expr) {
    assert(expr);
    AST* e           = make_ast(AST_GROUPING, t);
    e->Grouping.expr = expr;
    return e;
}

AST* make_ast_variable_decl(Token t, char* name, Type* type, AST* value) {
    // 'value' and 'name' can be NULL
    // assert(type);
    AST* e                 = make_ast(AST_VARIABLE_DECL, t);
    e->Variable_Decl.name  = name;
    e->type                = type;
    e->Variable_Decl.type  = type;
    e->Variable_Decl.value = value;
    return e;
}

AST* make_ast_constant_decl(Token t, char* name, AST* value) {
    assert(name);
    assert(value);
    AST* e                 = make_ast(AST_CONSTANT_DECL, t);
    e->Constant_Decl.name  = name;
    e->Constant_Decl.value = value;
    return e;
}

AST* make_ast_subscript(Token t, AST* load, AST* sub) {
    assert(load);
    assert(sub);
    AST* e            = make_ast(AST_SUBSCRIPT, t);
    e->Subscript.load = load;
    e->Subscript.sub  = sub;
    return e;
}

AST* make_ast_if(Token t, AST* cond, AST* then_block, AST* else_block) {
    assert(cond);
    assert(then_block);
    // assert(else_block); // else block is optional
    AST* e           = make_ast(AST_IF, t);
    e->If.cond       = cond;
    e->If.then_block = then_block;
    e->If.else_block = else_block;
    return e;
}

AST* make_ast_for(Token t, AST* init, AST* cond, AST* step, AST* then_block) {
    assert(init);
    assert(cond);
    assert(step);
    assert(then_block);
    AST* e            = make_ast(AST_FOR, t);
    e->For.init       = init;
    e->For.cond       = cond;
    e->For.step       = step;
    e->For.then_block = then_block;
    return e;
}

AST* make_ast_while(Token t, AST* cond, AST* then_block) {
    assert(cond);
    assert(then_block);
    AST* e              = make_ast(AST_WHILE, t);
    e->While.cond       = cond;
    e->While.then_block = then_block;
    return e;
}

AST* make_ast_return(Token t, AST* expr) {
    // assert(expr); // returns can be called with out an expression
    AST* e         = make_ast(AST_RETURN, t);
    e->Return.expr = expr;
    return e;
}

AST* make_ast_defer(Token t, AST* expr) {
    assert(expr);
    AST* e        = make_ast(AST_DEFER, t);
    e->Defer.expr = expr;
    return e;
}

AST* make_ast_break(Token t) {
    AST* e = make_ast(AST_BREAK, t);
    return e;
}

AST* make_ast_continue(Token t) {
    AST* e = make_ast(AST_CONTINUE, t);
    return e;
}

AST* make_ast_as(Token t, AST* expr, AST* type_expr) {
    assert(expr);
    assert(type_expr);
    AST* e          = make_ast(AST_AS, t);
    e->As.expr      = expr;
    e->As.type_expr = type_expr;
    return e;
}
