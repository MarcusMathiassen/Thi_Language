// Copyright (c) 2019 Marcus Mathiassen

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

#include "lex.h"  // token_kind_to_str,
#include "string.h" // strf, string_append, string
#include "common.h"
#include "utility.h" // info, success, error, warning, xmalloc, xrealloc, xstrlen
#include <stdlib.h>
#include <string.h> // strlen

//------------------------------------------------------------------------------
//                               AST Utility
//------------------------------------------------------------------------------

typedef struct {
    string* str;
    bool as_source;
    Loc_Info last;
    u64 indentation_level;
} String_Context;

static AST* make_ast(AST_Kind kind, Loc_Info loc_info);
static char* _ast_to_str(String_Context* ctx, AST* node);

char* ast_kind_to_str(AST_Kind kind) {
    TASSERT_KIND_IN_RANGE(AST, kind);
    switch (kind) {
        ERROR_UNHANDLED_KIND(strf("kind = %d", kind));
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
        case AST_CHAR:                            return "AST_CHAR";
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
        case AST_POST_INC_OR_DEC:                 return "AST_POST_INC_OR_DEC";
        case AST_LITERAL:                         return "AST_LITERAL";
        case AST_ASM:                             return "AST_ASM";
        case AST_CAST:                            return "AST_CAST";
        case AST_EXPR_LIST:                       return "AST_EXPR_LIST";
    }
    UNREACHABLE;
    return NULL;
}

char* get_ast_loc_str(AST* node) {
    return strf("%d:%d", node->loc_info.line, node->loc_info.col);
}

char* get_ast_name(AST* node) {
    if (!node) return "---";
    switch (node->kind) {
        ERROR_UNHANDLED_AST_KIND(node->kind);
        case AST_MODULE:        return node->Module.name;
        case AST_VARIABLE_DECL: return node->Variable_Decl.name;
        case AST_EXTERN:        return get_type_name(node->Extern.type); // @Todo: AST_Extern should follow AST_Functions patterns
        case AST_FUNCTION:      return node->Function.name;
        case AST_STRUCT:        return node->Struct.name;
        case AST_ENUM:          return node->Enum.name;
        case AST_IDENT:         return node->Ident.name;
    }
    UNREACHABLE;
    return NULL;
}

char* ast_to_src(AST* node) {
    String_Context ctx;
    ctx.as_source = true;
    ctx.last.line = 0;
    ctx.last.col = 0;
    ctx.str = make_string("");
    ctx.indentation_level = DEFAULT_INDENT_LEVEL;
    return _ast_to_str(&ctx, node);
}

char* ast_to_str(AST* node) {
    String_Context ctx;
    ctx.as_source = false;
    ctx.last.line = 0;
    ctx.last.col = 0;
    ctx.str = make_string("");
    ctx.indentation_level = DEFAULT_INDENT_LEVEL;
    return _ast_to_str(&ctx, node);
}

static void _ast_to_str_comment                         (String_Context* ctx, AST* node);
static void _ast_to_str_nop                             (String_Context* ctx, AST* node);
static void _ast_to_str_space_separated_identifier_list (String_Context* ctx, AST* node);
static void _ast_to_str_comma_separated_list            (String_Context* ctx, AST* node);
static void _ast_to_str_module                          (String_Context* ctx, AST* node);
static void _ast_to_str_is                              (String_Context* ctx, AST* node);
static void _ast_to_str_fallthrough                     (String_Context* ctx, AST* node);
static void _ast_to_str_var_args                        (String_Context* ctx, AST* node);
static void _ast_to_str_extern                          (String_Context* ctx, AST* node);
static void _ast_to_str_load                            (String_Context* ctx, AST* node);
static void _ast_to_str_link                            (String_Context* ctx, AST* node);
static void _ast_to_str_note                            (String_Context* ctx, AST* node);
static void _ast_to_str_int                             (String_Context* ctx, AST* node);
static void _ast_to_str_float                           (String_Context* ctx, AST* node);
static void _ast_to_str_string                          (String_Context* ctx, AST* node);
static void _ast_to_str_char                            (String_Context* ctx, AST* node);
static void _ast_to_str_ident                           (String_Context* ctx, AST* node);
static void _ast_to_str_call                            (String_Context* ctx, AST* node);
static void _ast_to_str_unary                           (String_Context* ctx, AST* node);
static void _ast_to_str_binary                          (String_Context* ctx, AST* node);
static void _ast_to_str_grouping                        (String_Context* ctx, AST* node);
static void _ast_to_str_subscript                       (String_Context* ctx, AST* node);
static void _ast_to_str_field_access                    (String_Context* ctx, AST* node);
static void _ast_to_str_as                              (String_Context* ctx, AST* node);
static void _ast_to_str_block                           (String_Context* ctx, AST* node);
static void _ast_to_str_struct                          (String_Context* ctx, AST* node);
static void _ast_to_str_enum                            (String_Context* ctx, AST* node);
static void _ast_to_str_function                        (String_Context* ctx, AST* node);
static void _ast_to_str_variable_decl                   (String_Context* ctx, AST* node);
static void _ast_to_str_if                              (String_Context* ctx, AST* node);
static void _ast_to_str_for                             (String_Context* ctx, AST* node);
static void _ast_to_str_while                           (String_Context* ctx, AST* node);
static void _ast_to_str_return                          (String_Context* ctx, AST* node);
static void _ast_to_str_defer                           (String_Context* ctx, AST* node);
static void _ast_to_str_break                           (String_Context* ctx, AST* node);
static void _ast_to_str_continue                        (String_Context* ctx, AST* node);
static void _ast_to_str_typeof                          (String_Context* ctx, AST* node);
static void _ast_to_str_sizeof                          (String_Context* ctx, AST* node);
static void _ast_to_str_switch                          (String_Context* ctx, AST* node);
static void _ast_to_str_post_inc_or_dec                 (String_Context* ctx, AST* node);
static void _ast_to_str_literal                         (String_Context* ctx, AST* node);
static void _ast_to_str_asm                             (String_Context* ctx, AST* node);
static void _ast_to_str_cast                            (String_Context* ctx, AST* node);
static void _ast_to_str_expr_list                       (String_Context* ctx, AST* node);


static void (*ast_to_str_transitions[])(String_Context*, AST*) = {
    [AST_COMMENT]                         =  _ast_to_str_comment,
    [AST_NOP]                             =  _ast_to_str_nop,
    [AST_SPACE_SEPARATED_IDENTIFIER_LIST] =  _ast_to_str_space_separated_identifier_list,
    [AST_COMMA_SEPARATED_LIST]            =  _ast_to_str_comma_separated_list,
    [AST_MODULE]                          =  _ast_to_str_module,
    [AST_IS]                              =  _ast_to_str_is,
    [AST_FALLTHROUGH]                     =  _ast_to_str_fallthrough,
    [AST_VAR_ARGS]                        =  _ast_to_str_var_args,
    [AST_EXTERN]                          =  _ast_to_str_extern,
    [AST_LOAD]                            =  _ast_to_str_load,
    [AST_LINK]                            =  _ast_to_str_link,
    [AST_NOTE]                            =  _ast_to_str_note,
    [AST_INT]                             =  _ast_to_str_int,
    [AST_FLOAT]                           =  _ast_to_str_float,
    [AST_STRING]                          =  _ast_to_str_string,
    [AST_CHAR]                            =  _ast_to_str_char,
    [AST_IDENT]                           =  _ast_to_str_ident,
    [AST_CALL]                            =  _ast_to_str_call,
    [AST_UNARY]                           =  _ast_to_str_unary,
    [AST_BINARY]                          =  _ast_to_str_binary,
    [AST_GROUPING]                        =  _ast_to_str_grouping,
    [AST_SUBSCRIPT]                       =  _ast_to_str_subscript,
    [AST_FIELD_ACCESS]                    =  _ast_to_str_field_access,
    [AST_AS]                              =  _ast_to_str_as,
    [AST_BLOCK]                           =  _ast_to_str_block,
    [AST_STRUCT]                          =  _ast_to_str_struct,
    [AST_ENUM]                            =  _ast_to_str_enum,
    [AST_FUNCTION]                        =  _ast_to_str_function,
    [AST_VARIABLE_DECL]                   =  _ast_to_str_variable_decl,
    [AST_IF]                              =  _ast_to_str_if,
    [AST_FOR]                             =  _ast_to_str_for,
    [AST_WHILE]                           =  _ast_to_str_while,
    [AST_RETURN]                          =  _ast_to_str_return,
    [AST_DEFER]                           =  _ast_to_str_defer,
    [AST_BREAK]                           =  _ast_to_str_break,
    [AST_CONTINUE]                        =  _ast_to_str_continue,
    [AST_TYPEOF]                          =  _ast_to_str_typeof,
    [AST_SIZEOF]                          =  _ast_to_str_sizeof,
    [AST_SWITCH]                          =  _ast_to_str_switch,
    [AST_POST_INC_OR_DEC]                 =  _ast_to_str_post_inc_or_dec,
    [AST_LITERAL]                         =  _ast_to_str_literal,
    [AST_ASM]                             =  _ast_to_str_asm,
    [AST_CAST]                            =  _ast_to_str_cast,
    [AST_EXPR_LIST]                       =  _ast_to_str_expr_list,
};

static char* _ast_to_str(String_Context* ctx, AST* node) {
    xassert(ctx);
    string* s = ctx->str;
    if (!node) {
        string_append(s, "---");
        return string_data(s);
    }
    // @Buggy, @Audit
    if (ctx->as_source) {
        // Add some newlines if we have too :)
        // If there is a difference in line position. Add
        // that many newlines.
        // xassert(node->loc_info.line >= ctx->last.line);
        s64 diff = node->loc_info.line - ctx->last.line;
        while (--diff > 0) {
            string_append_f(s, "\n%s", get_indentation_as_str(ctx->indentation_level));
        }
        ctx->last = node->loc_info;
    }
    AST_Kind kind = node->kind;
    TASSERT_KIND_IN_RANGE(AST, kind);
    void (*func)(String_Context*, AST*) = (*ast_to_str_transitions[kind]);
    tassert(func, "ast_to_str missing callback for %s", kind);
    (*func)(ctx, node);

    return string_data(s);
}


static void _ast_to_str_comment(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "#%s", node->Comment.text);
}

static void _ast_to_str_nop(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "nop");
}

static void _ast_to_str_space_separated_identifier_list(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    list_foreach(node->Space_Separated_Identifier_List.identifiers) {
        _ast_to_str(ctx, it->data);
        if (it->next) string_append(s, " ");
    }
}

static void _ast_to_str_comma_separated_list(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    list_foreach(node->Comma_Separated_List.nodes) {
        _ast_to_str(ctx, it->data);
        if (it->next) string_append(s, ", ");
    }
}

static void _ast_to_str_module(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    list_foreach(node->Module.top_level) {
        AST* stmt = it->data;
        _ast_to_str(ctx, stmt);
        string_append(s, "\n");
    }
}

static void _ast_to_str_is(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "is ");
    _ast_to_str(ctx, node->Is.node);
    string_append(s, " ");
    _ast_to_str(ctx, node->Is.body);
}

static void _ast_to_str_fallthrough(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "fallthrough");
}

static void _ast_to_str_var_args(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "...");
}

static void _ast_to_str_extern(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "extern %s %s", get_ast_name(node), type_to_str(node->Extern.type));
}

static void _ast_to_str_load(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    if (!ctx->as_source)
        string_append_f(s, "load \"%s\"", node->Load.str);
    else
        _ast_to_str(ctx, node->Load.module);
}

static void _ast_to_str_link(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "link \"%s\"", node->Link.str);
}

static void _ast_to_str_note(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "$");
    _ast_to_str(ctx, node->Note.node);
}

static void _ast_to_str_int(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "%lld", node->Int.val);
    if (node->type->Int.is_unsigned) string_append(s, "U");
    if (node->type->Int.bytes == 4) string_append(s, "L");
    else /* it must be 8 */ string_append(s, "LL");
}

static void _ast_to_str_float(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    // @Checkout(marcus): not to sure about the fmt.
    char* str = strf("%.14g", node->Float.val);
    u64 n = xstrlen(str);
    // This makes sure there is at least a single decimal point.
    string_append_f(s, "%s", n == 1 ? strf("%s.0", str) : str);
    if (node->type->Float.bytes < 8) string_append(s, "F");
}

static void _ast_to_str_string(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "\"%s\"", node->String.val);
}

static void _ast_to_str_char(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "%d", node->Char.val);
}

static void _ast_to_str_ident(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, get_ast_name(node));
}

static void _ast_to_str_call(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "%s(", node->Call.callee);
    list_foreach(node->Call.args) {
        _ast_to_str(ctx, it->data);
        if (it->next) string_append(s, ", ");
    }
    string_append(s, ")");
}

static void _ast_to_str_unary(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "%s", token_kind_to_str(node->Unary.op));
    _ast_to_str(ctx, node->Unary.operand);
}

static void _ast_to_str_binary(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "(");
    _ast_to_str(ctx, node->Binary.lhs);
    string_append(s, " ");
    string_append(s, token_kind_to_str(node->Binary.op));
    string_append(s, " ");
    _ast_to_str(ctx, node->Binary.rhs);
    string_append(s, ")");
}

static void _ast_to_str_grouping(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "(");
    _ast_to_str(ctx, node->Grouping.node);
    string_append(s, ")");
}

static void _ast_to_str_subscript(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    _ast_to_str(ctx, node->Subscript.load);
    string_append(s, "[");
    _ast_to_str(ctx, node->Subscript.sub);
    string_append(s, "]");
}

static void _ast_to_str_field_access(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    _ast_to_str(ctx, node->Field_Access.load);
    string_append_f(s, ".%s", node->Field_Access.field);
}

static void _ast_to_str_as(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    _ast_to_str(ctx, node->As.node);
    string_append(s, " as ");
    _ast_to_str(ctx, node->As.type_node);
}

static void _ast_to_str_block(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "\n");
    ctx->indentation_level += DEFAULT_INDENT_LEVEL;
    List* stmts = node->Block.stmts;
    list_foreach(stmts) {
        string_append(s, get_indentation_as_str(ctx->indentation_level));
        AST* stmt = it->data;
        _ast_to_str(ctx, stmt);
        if (it->next) string_append(s, "\n");
    }
    ctx->indentation_level -= DEFAULT_INDENT_LEVEL;
}

static void _ast_to_str_struct(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "def %s\n", get_ast_name(node));
    ctx->indentation_level += DEFAULT_INDENT_LEVEL;
    list_foreach(node->Struct.members) {
        string_append(s, get_indentation_as_str(ctx->indentation_level));
        _ast_to_str(ctx, it->data);
        if (it->next) string_append(s, "\n");
    }
    ctx->indentation_level -= DEFAULT_INDENT_LEVEL;
}

static void _ast_to_str_enum(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "def %s\n", get_ast_name(node));
    ctx->indentation_level += DEFAULT_INDENT_LEVEL;
    list_foreach(node->Enum.members) {
        string_append(s, get_indentation_as_str(ctx->indentation_level));
        _ast_to_str(ctx, it->data);
        if (it->next) string_append(s, "\n");
    }
    ctx->indentation_level -= DEFAULT_INDENT_LEVEL;
}

static void _ast_to_str_function(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append_f(s, "def %s(", get_ast_name(node));
    list_foreach(node->Function.parameters) {
        _ast_to_str(ctx, it->data);
        if (it->next) string_append(s, ", ");
    }
    string_append_f(s, ") %s", get_type_name(node->type->Function.return_type));
    _ast_to_str(ctx, node->Function.body);
}

static void _ast_to_str_variable_decl(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    if (node->Variable_Decl.value) {
        string_append_f(s, "%s %s = ", get_ast_name(node), get_type_name(node->type));
        _ast_to_str(ctx, node->Variable_Decl.value);
    } else {
        string_append_f(s, "%s %s", get_ast_name(node), get_type_name(node->type));
    }
}

static void _ast_to_str_if(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "if ");
    _ast_to_str(ctx, node->If.cond);
    string_append(s, " ");
    _ast_to_str(ctx, node->If.then_block);
    if (node->If.else_block) {
        string_append(s, " else ");
        _ast_to_str(ctx, node->If.else_block);
    }
}

static void _ast_to_str_for(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "for ");
    _ast_to_str(ctx, node->For.init);
    string_append(s, " ");
    _ast_to_str(ctx, node->For.cond);
    string_append(s, " ");
    _ast_to_str(ctx, node->For.step);
    _ast_to_str(ctx, node->For.then_block);
}

static void _ast_to_str_while(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "while ");
    _ast_to_str(ctx, node->While.cond);
    string_append(s, " ");
    _ast_to_str(ctx, node->While.then_block);
}

static void _ast_to_str_return(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "return ");
    _ast_to_str(ctx, node->Return.node);
}

static void _ast_to_str_defer(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "defer ");
    _ast_to_str(ctx, node->Defer.node);
}

static void _ast_to_str_break(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "break");
}

static void _ast_to_str_continue(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "continue");
}

static void _ast_to_str_typeof(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "typeof ");
    _ast_to_str(ctx, node->Typeof.node);
}

static void _ast_to_str_sizeof(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "sizeof ");
    _ast_to_str(ctx, node->Sizeof.node);
}

static void _ast_to_str_switch(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "switch ");
    _ast_to_str(ctx, node->Switch.cond);
    _ast_to_str(ctx, node->Switch.cases);
    string_append(s, " else ");
    _ast_to_str(ctx, node->Switch.default_case);
}

static void _ast_to_str_post_inc_or_dec(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    _ast_to_str(ctx, node->Post_Inc_or_Dec.node);
    string_append(s, token_kind_to_str(node->Post_Inc_or_Dec.op));
}

static void _ast_to_str_literal(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    switch (node->Literal.kind) {
        ERROR_UNHANDLED_LITERAL_KIND(node->Literal.kind);
        case LITERAL_CHAR:     string_append_f(s, "%c",   node->Literal.Char.value);     break;
        case LITERAL_INTEGER:  string_append_f(s, "%lld", node->Literal.Integer.value);  break;
        case LITERAL_HEX:      string_append_f(s, "%llu", node->Literal.Hex.value);      break;
        case LITERAL_FLOAT:    string_append_f(s, "%f",   node->Literal.Float.value);    break;
        case LITERAL_STRING:   string_append_f(s, "%s",   node->Literal.String.value);   break;
    }
}

static void _ast_to_str_asm(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "asm\n");
    ctx->indentation_level += DEFAULT_INDENT_LEVEL;
    list_foreach(node->Asm.block->Block.stmts) {
        AST* stmt = it->data;
        string_append(s, get_indentation_as_str(ctx->indentation_level));
        string_append(s, stmt->String.val);
        if (it->next) string_append(s, "\n");
    }
    ctx->indentation_level -= DEFAULT_INDENT_LEVEL;
}

static void _ast_to_str_cast(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    string_append(s, "cast(");
    _ast_to_str(ctx, node->Cast.desired_type);
    string_append(s, ") ");
    _ast_to_str(ctx, node->Cast.node);
}

static void _ast_to_str_expr_list(String_Context* ctx, AST* node) {
    xassert(ctx && node);
    string* s = ctx->str;
    list_foreach(node->Expr_List.exprs) {
        AST* expr = it->data;
        _ast_to_str(ctx, expr);
        if (it->next) string_append(s, ", ");
    }
}

void* ast_visit(ast_callback func, void* ctx, AST* node) {
    xassert(func);
    if (!node) return NULL;
    switch (node->kind) {
        ERROR_UNHANDLED_AST_KIND(node->kind);

        case AST_COMMENT: break;
        case AST_NOP: break;

        // @Todo(marcus) figure out what to do with these.
        case AST_SPACE_SEPARATED_IDENTIFIER_LIST:
        list_foreach(node->Space_Separated_Identifier_List.identifiers) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_COMMA_SEPARATED_LIST:
        list_foreach(node->Comma_Separated_List.nodes) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_MODULE:
        list_foreach(node->Module.top_level) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_TYPEOF: ast_visit(func, ctx, node->Typeof.node); break;
        case AST_SIZEOF: ast_visit(func, ctx, node->Sizeof.node); break;
        case AST_NOTE: ast_visit(func, ctx, node->Note.node); break;
        case AST_FALLTHROUGH: break;
        case AST_LOAD:
        ast_visit(func, ctx, node->Load.module);
        break;
        case AST_LINK: break;
        case AST_INT: break;
        case AST_FLOAT: break;
        case AST_STRING: break;
        case AST_CHAR: break;
        case AST_IDENT: break;
        case AST_STRUCT: break;
        case AST_POST_INC_OR_DEC:
        ast_visit(func, ctx, node->Post_Inc_or_Dec.node);
        break;

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
        list_foreach(node->Extern.type->Function.parameters) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_FUNCTION:
        ast_visit(func, ctx, node->Function.body);
        list_foreach(node->Function.parameters) {
            ast_visit(func, ctx, it->data);
        }
        list_foreach(node->Function.defers) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_CALL:
        list_foreach(node->Call.args) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_BLOCK:
        list_foreach(node->Block.stmts) {
            ast_visit(func, ctx, it->data);
        }
        break;
        case AST_ASM:
        ast_visit(func, ctx, node->Asm.block);
        break;
        case AST_CAST:
        ast_visit(func, ctx, node->Cast.desired_type);
        ast_visit(func, ctx, node->Cast.node);
        break;
        case AST_EXPR_LIST:
        list_foreach(node->Expr_List.exprs) {
            ast_visit(func, ctx, it->data);
        }
        break;
    }
    return (*func)(ctx, node);
}


AST* get_arg_from_func(AST* func, s64 arg_index) {
    xassert(func);
    xassert(arg_index >= 0 && arg_index <= func->Function.parameters->count);
    AST* param = list_at(func->Function.parameters, arg_index);
    xassert(param);
    return param;
}

void ast_ref_list_destroy(AST_Ref_List* l) {
    free(l->data);
    free(l);
}

AST_Ref_List* make_ast_ref_list() {
    AST_Ref_List* l = xmalloc(sizeof(AST_Ref_List));
    l->count = 0;
    l->allocated = AST_REF_LIST_STARTING_ALLOC;
    l->data = xmalloc(l->allocated * sizeof(l->data));
    return l;
}

void ast_ref_list_append(AST_Ref_List* l, AST* a) {
    if (l->count == l->allocated) {
        l->allocated *= AST_REF_LIST_GROWTH_FACTOR;
        l->data = xrealloc(l->data, l->allocated * sizeof(*l->data));
    }
    l->data[l->count++] = a;
}

char* ast_get_literal_value_as_str(AST* node) {
    AST_Kind kind = node->kind;
    TASSERT_KIND_IN_RANGE(AST, kind);
    switch (kind) {
        ERROR_UNHANDLED_AST_KIND(kind);
        case AST_UNARY: return strf("%s%s", token_kind_to_str(node->Unary.op), ast_get_literal_value_as_str(node->Unary.operand));
        case AST_INT:    return strf("%lld", node->Int.val);
        case AST_FLOAT:  return strf("%f", node->Float.val);
        case AST_STRING: return strf("`%s`, 0", node->String.val);
        case AST_CHAR:   return ast_to_str(node);
    }
    UNREACHABLE;
    return NULL;
}

void ast_add_edge(AST* a, AST* dep) {
    list_append(a->edges, dep);
}

AST* ast_clone(AST* a) {
    AST* b = make_ast(a->kind, a->loc_info);
    *b = *a;
    return b;
}

void ast_replace(AST* a, AST* b) {
    xassert(a && b);
    debug("REPLACED %s -> %s WITH %s -> %s", give_unique_color(ast_to_str(a)), give_unique_color(type_to_str(a->type)), give_unique_color(ast_to_str(b)), give_unique_color(type_to_str(b->type)));
    *a = *b;
}

//------------------------------------------------------------------------------
//                               AST Tests
//------------------------------------------------------------------------------
void ast_tests(void) {
}

//------------------------------------------------------------------------------
//                               AST Maker Functions
//------------------------------------------------------------------------------

static AST* make_ast(AST_Kind kind, Loc_Info loc_info) {
    AST* e = xmalloc(sizeof(AST));
    e->kind = kind;
    e->loc_info = loc_info;
    e->type = NULL;
    e->edges = make_list();
    e->flags = 0;
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
    xassert(node);
    AST* e = make_ast(AST_TYPEOF, loc_info);
    e->Typeof.node = node;
    return e;
}

AST* make_ast_module(Loc_Info loc_info, char* name, List* top_level) {
    xassert(name);
    AST* e = make_ast(AST_MODULE, loc_info);
    e->Module.name = name;
    e->Module.top_level = top_level;
    return e;
}

AST* make_ast_sizeof(Loc_Info loc_info, AST* node) {
    xassert(node);
    AST* e = make_ast(AST_SIZEOF, loc_info);
    e->Sizeof.node = node;
    return e;
}

AST* make_ast_field_access(Loc_Info loc_info, AST* load, char* field) {
    xassert(load);
    xassert(field);
    AST* e = make_ast(AST_FIELD_ACCESS, loc_info);
    e->Field_Access.load = load;
    e->Field_Access.field = field;
    return e;
}

AST* make_ast_extern(Loc_Info loc_info, Type* type) {
    xassert(type);
    AST* e = make_ast(AST_EXTERN, loc_info);
    e->type = type;
    e->Extern.type = type;
    return e;
}

AST* make_ast_load(Loc_Info loc_info, char* str, AST* module) {
    xassert(str);
    xassert(module);
    AST* e = make_ast(AST_LOAD, loc_info);
    e->Load.str = str;
    e->Load.module = module;
    return e;
}

AST* make_ast_link(Loc_Info loc_info, char* str) {
    xassert(str);
    AST* e = make_ast(AST_LINK, loc_info);
    e->Link.str = str;
    return e;
}

AST* make_ast_note(Loc_Info loc_info, AST* node) {
    xassert(node);
    AST* e = make_ast(AST_NOTE, loc_info);
    e->Note.node = node;
    return e;
}

AST* make_ast_int(Loc_Info loc_info, s64 value, Type* type) {
    AST* e = make_ast(AST_INT, loc_info);
    e->type = type;
    e->Int.val = value;
    return e;
}

AST* make_ast_float(Loc_Info loc_info, f64 value, Type* type) {
    AST* e = make_ast(AST_FLOAT, loc_info);
    e->type = type;
    e->Float.val = value;
    return e;
}

AST* make_ast_string(Loc_Info loc_info, char* value) {
    AST* e = make_ast(AST_STRING, loc_info);
    e->type = make_type_pointer(make_type_int(1, TYPE_IS_UNSIGNED));
    e->String.val = value;
    return e;
}

AST* make_ast_char(Loc_Info loc_info, char value) {
    AST* e = make_ast(AST_CHAR, loc_info);
    e->type = make_type_int(1, TYPE_IS_UNSIGNED);
    e->Char.val = value;
    return e;
}

AST* make_ast_ident(Loc_Info loc_info, char* ident) {
    xassert(ident);
    AST* e = make_ast(AST_IDENT, loc_info);
    e->Ident.name = ident;
    return e;
}

AST* make_ast_struct(Loc_Info loc_info, char* name, List* members) {
    AST* e = make_ast(AST_STRUCT, loc_info);
    e->Struct.name = name;
    e->Struct.members = members;

    List* tps = make_list();
    list_foreach(members) {
        AST* member = it->data;
        Type_Name_Pair* tp = xmalloc(sizeof(*tp));
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
    list_foreach(members) {
        AST* member = it->data;
        Type_Name_Pair* tp = xmalloc(sizeof(*tp));
        tp->name = get_ast_name(member);
        tp->type = member->type;
        list_append(tps, tp);
    }

    e->type = make_type_enum(name, tps);
    return e;
}

AST* make_ast_function(Loc_Info loc_info, char* name, List* parameters, Type* func_t, AST* body) {
    xassert(name);
    xassert(parameters);
    xassert(func_t);
    xassert(func_t->kind == TYPE_FUNCTION);
    xassert(body);
    AST* e = make_ast(AST_FUNCTION, loc_info);
    e->type = func_t;
    e->Function.name = name;
    e->Function.parameters = parameters;
    e->Function.body = body;
    e->Function.defers = make_list();
    return e;
}

AST* make_ast_binary(Loc_Info loc_info, Token_Kind op, AST* lhs, AST* rhs) {
    xassert(op != TOKEN_UNKNOWN);
    xassert(lhs);
    xassert(rhs);
    AST* e = make_ast(AST_BINARY, loc_info);
    e->Binary.op = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

AST* make_ast_unary(Loc_Info loc_info, Token_Kind op, AST* operand) {
    xassert(op != TOKEN_UNKNOWN);
    xassert(operand);
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
    e->Block.flags = 0;
    return e;
}

AST* make_ast_call(Loc_Info loc_info, char* callee, List* args) {
    xassert(callee);
    xassert(args);
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
    AST* e = make_ast(AST_GROUPING, loc_info);
    e->Grouping.node = node;
    return e;
}

AST* make_ast_variable_decl(Loc_Info loc_info, char* name, Type* type, AST* value) {
    // 'value' and 'name' can be NULL
    // xassert(type);
    AST* e = make_ast(AST_VARIABLE_DECL, loc_info);
    e->type = type;
    e->Variable_Decl.name = name;
    e->Variable_Decl.type = type;
    e->Variable_Decl.value = value;
    return e;
}

AST* make_ast_subscript(Loc_Info loc_info, AST* load, AST* sub) {
    xassert(load);
    xassert(sub);
    AST* e = make_ast(AST_SUBSCRIPT, loc_info);
    e->Subscript.load = load;
    e->Subscript.sub = sub;
    return e;
}

AST* make_ast_if(Loc_Info loc_info, AST* cond, AST* then_block, AST* else_block) {
    xassert(cond);
    xassert(then_block);
    // xassert(else_block); // else block is optional
    AST* e = make_ast(AST_IF, loc_info);
    e->If.cond = cond;
    e->If.then_block = then_block;
    e->If.else_block = else_block;
    return e;
}

AST* make_ast_for(Loc_Info loc_info, AST* init, AST* cond, AST* step, AST* then_block) {
    xassert(then_block);
    AST* e = make_ast(AST_FOR, loc_info);
    e->For.init = init;
    e->For.cond = cond;
    e->For.step = step;
    e->For.then_block = then_block;
    return e;
}

AST* make_ast_while(Loc_Info loc_info, AST* cond, AST* then_block) {
    xassert(cond);
    xassert(then_block);
    AST* e = make_ast(AST_WHILE, loc_info);
    e->While.cond = cond;
    e->While.then_block = then_block;
    return e;
}

AST* make_ast_return(Loc_Info loc_info, AST* node) {
    xassert(node);
    AST* e = make_ast(AST_RETURN, loc_info);
    e->Return.node = node;
    return e;
}

AST* make_ast_defer(Loc_Info loc_info, AST* node) {
    xassert(node);
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
    xassert(node);
    xassert(type_node);
    AST* e = make_ast(AST_AS, loc_info);
    e->As.node = node;
    e->As.type_node = type_node;
    return e;
}

AST* make_ast_space_separated_identifier_list(Loc_Info loc_info, List* identifiers) {
    xassert(identifiers);
    AST* e = make_ast(AST_SPACE_SEPARATED_IDENTIFIER_LIST, loc_info);
    e->Space_Separated_Identifier_List.identifiers = identifiers;
    return e;
}

AST* make_ast_comma_separated_list(Loc_Info loc_info, List* nodes) {
    xassert(nodes);
    AST* e = make_ast(AST_COMMA_SEPARATED_LIST, loc_info);
    e->Comma_Separated_List.nodes = nodes;
    return e;
}

AST* make_ast_post_inc_or_dec(Loc_Info loc_info, Token_Kind op, AST* node) {
    xassert(node);
    AST* e = make_ast(AST_POST_INC_OR_DEC, loc_info);
    e->Post_Inc_or_Dec.op = op;
    e->Post_Inc_or_Dec.node = node;
    return e;
}

AST* make_ast_literal(Loc_Info loc_info, Literal_Kind kind, char* value) {
    xassert(value);
    TASSERT_KIND_IN_RANGE(LITERAL, kind);
    AST* e = make_ast(AST_LITERAL, loc_info);
    e->Literal.kind = kind;
    switch (kind) {
        ERROR_UNHANDLED_LITERAL_KIND(kind);
        case LITERAL_CHAR:    e->Literal.Char.value    = value[0];     break;
        case LITERAL_INTEGER: e->Literal.Integer.value = atoll(value); break;
        // case LITERAL_HEX:
        case LITERAL_FLOAT:   e->Literal.Float.value   = atof(value);  break;
        case LITERAL_STRING:  e->Literal.String.value  = value;        break;
    }
    return e;
}

AST* make_ast_asm(Loc_Info loc_info, AST* block) {
    xassert(block);
    AST* e = make_ast(AST_ASM, loc_info);
    e->Asm.block = block;
    return e;
}
AST* make_ast_cast(Loc_Info loc_info, AST* desired_type, AST* node) {
    xassert(desired_type && node);
    AST* e = make_ast(AST_CAST, loc_info);
    e->Cast.desired_type = desired_type;
    e->Cast.node = node;
    return e;
}

AST* make_ast_expr_list(Loc_Info loc_info, List* expr_list) {
    xassert(expr_list);
    AST* e = make_ast(AST_EXPR_LIST, loc_info);
    e->Expr_List.exprs = expr_list;
    return e;   
}

char* literal_kind_to_str(Literal_Kind kind) {
    TASSERT_KIND_IN_RANGE(LITERAL, kind);
    switch (kind) {
        ERROR_UNHANDLED_KIND(strf("kind = %d", kind));
        case LITERAL_CHAR:    return "LITERAL_CHAR";
        case LITERAL_INTEGER: return "LITERAL_INTEGER";
        case LITERAL_HEX:     return "LITERAL_HEX";
        case LITERAL_FLOAT:   return "LITERAL_FLOAT";
        case LITERAL_STRING:  return "LITERAL_STRING";
    }
    UNREACHABLE;
    return NULL;
}
