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
//                               main.c
//------------------------------------------------------------------------------
#include "ast.h"          // AST, AST_Kind
#include "codegen.h"      // generate_code_from_ast
#include "codegen_llvm.h" // generate_llvm_from_ast
#include "constants.h"    // all constnats
#include "lexer.h"        // generate_tokens_from_source, print_tokens
#include "list.h"         // list_tests
#include "map.h"          // map
#include "parser.h"       // generate_ast_from_tokens
#include "sema.h"         // semantic_analysis
#include "stack.h"        // stack_tests
#include "string.h"       // strcmp
#include "thi.h"          // Thi
#include "type.h"         // Type
#include "typedefs.h"     // u8, u16, s32, etc.
#include "utility.h"      // get_file_content, success, info, get_time
#include "value.h"        // Value
#include <assert.h>       // assert
#include <stdio.h>        // sprintf
#include <stdlib.h>       // free
#include <string.h>       // strcmp
#include <sys/ioctl.h>    // NOTE(marcus): what do i use this for?
#include <unistd.h>       // NOTE(marcus): what do i use this
 
//  
//  --  06/05/19 Marcus Mathiassen
//  
//  Need to find a way to handle data..
//  
//  -- AST
//
//      -- The Problem --
//      AST creation functions are not memory mangaged. We just create them where we see fit.
//
//      -- The Solution --
//      ASTBuilder. Like a string builder, a typebuilder, or any other builder. 
//
//  -- Types
//
//      -- The Problem --
//      Right now types are created in-place at the point it is needed with no memory management and no cleanups
//      When we reassign a type the original pointer is lost to the void. Must be a better way.
//      
//      -- The Solution --
//      Single representation. We can store one version of every type found in the system in an associative array.
//      A map in this case. This gives us the following abilities:
//          1. manage when types are created and where (debug checks)
//          2. skips the need for cleanup when resassigning types (the memory is not theirs to cleanup)
//          3. directly compare pointers when testing for equality between types (excluding pointer+pointer checks. Indirection also needs to be checked).
// 
// 

void assemble(Thi* thi, char* asm_file, char* exec_name);
void linking_stage(Thi* thi, char* exec_name);
void write_syntax_file(Thi* thi);

//------------------------------------------------------------------------------
//                               Passes
//------------------------------------------------------------------------------

// Replaces 'sizeof <expr>' calls with the size of the resulting expr.
void resolve_sizeofs(void* dont_care, AST* node) {
    s64 size = get_size_of_type(node->type);
    AST* constant_value = make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, false));
    ast_replace(node, constant_value);
}
// Replaces 'typeof <expr>' calls with the type of the resulting expr.
void resolve_typeofs(void* dont_care, AST* node) {
    AST* string_value = make_ast_string(node->loc_info, get_type_name(node->type));
    ast_replace(node, string_value);
}

// Resolving subscripts means changing
// <load>[<expr>]
// into
// *(&<load> + sizeof(*load) * <expr>)
void resolve_subscript(void* dont_care, AST* node) {

    AST* load = node->Subscript.load;
    AST* sub = node->Subscript.sub;
    Type* type_of_field = node->type;

    s64 size = get_size_of_underlying_type_if_any(load->type);

    sub = make_ast_binary(node->loc_info, TOKEN_ASTERISK, make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, false)), sub);
    load = make_ast_binary(node->loc_info, TOKEN_PLUS, load, sub);
    load = make_ast_grouping(node->loc_info, load);
    load = make_ast_unary(node->loc_info, THI_SYNTAX_POINTER, load);
    load->type = type_of_field;

    ast_replace(node, load);
}

void resolve_field_access(void* dont_care, AST* node) {

    AST* load = node->Field_Access.load;
    char* field_name = node->Field_Access.field;
    Type* type_of_field = node->type;

    s64 offset_size = get_offset_in_struct_to_field(load->type, field_name);
    AST* offset = make_ast_int(node->loc_info, offset_size, make_type_int(DEFAULT_INT_BYTE_SIZE, 0));

    load = make_ast_unary(node->loc_info, THI_SYNTAX_ADDRESS, load);
    load = make_ast_binary(node->loc_info, TOKEN_PLUS, load, offset);
    load = make_ast_grouping(node->loc_info, load);
    load = make_ast_unary(node->loc_info, THI_SYNTAX_POINTER, load);
    load->type = type_of_field;

    ast_replace(node, load);
}

void pass_initilize_enums(void* thi, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_ENUM: {

        s64 counter = 0;
        AST* e = node;
        LIST_FOREACH(e->Enum.members) {
            AST* m = it->data;
            // Turn idents into constant decls
            switch (m->kind) {
                ERROR_UNHANDLED_AST_KIND(m->kind);
            case AST_IDENT:
                it->data = make_ast_constant_decl(m->loc_info, m->Ident.name, make_ast_int(m->loc_info, counter, make_type_int(DEFAULT_INT_BYTE_SIZE, 0)));
                break;
            case AST_CONSTANT_DECL:
                assert(m->Constant_Decl.value->kind == AST_INT);
                counter = m->Constant_Decl.value->Int.val;
                break;
            }
            ++counter;
        }

    } break;
    }
}
void pass_add_all_symbols(void* thi, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_ENUM:   // fallthrough
    case AST_STRUCT: // fallthrough
    case AST_FUNCTION: add_symbol(thi, get_type_name(node->type), node->type); break;
    case AST_EXTERN: add_symbol(thi, get_type_name(node->type), node->type); break;
    }
}
void pass_resolve_all_symbols(void* thi, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_ENUM:   // fallthrough
    case AST_STRUCT: // fallthrough
    case AST_FUNCTION: node->type = get_symbol(thi, get_type_name(node->type)); break;
    }
}

void check_for_unresolved_types(void* ctx, AST* node) {
    if (node->type && node->type->kind == TYPE_UNRESOLVED) {
        error("[check_for_unresolved_types]: unresolved type found for node: %s", ast_to_str(node));
    }
}

void run_all_passes(void* thi, AST* node) {
    List* visitors = thi_get_visitors_for_kind(thi, node->kind);
    LIST_FOREACH(visitors) {
        PassDescriptor* passDesc = it->data;
        (*passDesc->visitor_func)(passDesc->visitor_arg, node);
    }
}

void make_sure_all_nodes_have_a_valid_type(void* dont_care, AST* node) {
    assert(node);
    switch (node->kind) {
    case AST_COMMENT:  // fallthrough
    case AST_NOP:      // fallthrough
    case AST_MODULE:   // fallthrough
    case AST_LOAD:     // fallthrough
    case AST_LINK:     // fallthrough
    case AST_BLOCK:    // fallthrough
    case AST_WHILE:    // fallthrough
    case AST_IF:       // fallthrough
    case AST_FOR:      // fallthrough
    case AST_SWITCH:   // fallthrough
    case AST_IS:       // fallthrough
    case AST_DEFER:    // fallthrough
    case AST_BREAK:    // fallthrough
    case AST_CONTINUE: // fallthrough
    case AST_FALLTHROUGH: return;
    default: break;
    }
    info("%s: %s -> %s", ast_kind_to_str(node->kind), wrap_with_colored_parens(ast_to_str(node)), give_unique_color(type_to_str(node->type)));

    if (!node->type) {
        error(
            "[make_sure_all_nodes_have_a_valid_type]: missing type for "
            "node: %s",
            ast_to_str(node));
    }
}

void visitor_resolve_unresolved_types(void* thi, AST* node) {
    if (!node->type) return;
    Type* placeholder_t = node->type;
    while (placeholder_t->kind == TYPE_POINTER) {
        placeholder_t = placeholder_t->Pointer.pointee;
    }
    if (placeholder_t->kind == TYPE_UNRESOLVED) {
        *placeholder_t = *get_symbol(thi, get_type_name(placeholder_t));
    }
}

void constant_fold_unary(AST* node) {
    AST* operand = node->Unary.operand;
    if (operand->kind == AST_GROUPING) operand = operand->Grouping.node;
    // ex. &(*x) is the same as x
    // ex. *(&x) is the same as x
    if (operand->kind == AST_UNARY) {
        Token_Kind op = node->Unary.op;
        switch (op) {
        default: break;
        case THI_SYNTAX_POINTER: ast_replace(node, operand->Unary.op == THI_SYNTAX_ADDRESS ? operand->Unary.operand : operand); break;
        case THI_SYNTAX_ADDRESS: ast_replace(node, operand->Unary.op == THI_SYNTAX_POINTER ? operand->Unary.operand : operand); break;
        }
    }
    if (operand->kind == AST_INT) {
        Token_Kind op = node->Unary.op;
        s64 oper_v = operand->Int.val;
        s64 value = 0;

        switch (op) {
            ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_BANG: value = !oper_v; break;
        case TOKEN_PLUS: value = oper_v; break;
        case TOKEN_TILDE: value = ~oper_v; break;
        case TOKEN_MINUS: value = -oper_v; break;
        }

        operand->Int.val = value;
        ast_replace(node, operand);
    }
}

bool expr_is_literal_value(AST* expr, float val) {
    if ((expr->kind == AST_INT && expr->Int.val == (s64)val) ||
        (expr->kind == AST_FLOAT && expr->Float.val == val)) {
        return true;
    }
    return false;
}

void constant_fold_binary(AST* node) {
    Token_Kind op = node->Binary.op;
    AST* lhs = node->Binary.lhs;
    AST* rhs = node->Binary.rhs;
    if (lhs->kind == AST_GROUPING) lhs = lhs->Grouping.node;
    if (rhs->kind == AST_GROUPING) rhs = rhs->Grouping.node;

    // Remove lhs or rhs of expressions with + or - with 0 on either side
    if (op == TOKEN_MINUS || op == TOKEN_PLUS) {
        if (expr_is_literal_value(lhs, 0.0)) {
            ast_replace(node, rhs);
            return;
        } else if (expr_is_literal_value(rhs, 0.0)) {
            ast_replace(node, lhs);
            return;
        }
    }

    // Totally remove expressions where either lhs or rhs is 0 in multiplication
    bool lhs_is_0 = false; // used to skip recalc
    if ((lhs_is_0 = expr_is_literal_value(lhs, 0.0)) || expr_is_literal_value(rhs, 0.0)) {
        if (op == TOKEN_ASTERISK) {
            ast_replace(node, lhs_is_0 ? lhs : rhs);
            return;
        } else if (op == TOKEN_FWSLASH) {
            error("[%d:%d] divide by 0", node->loc_info.line_pos, node->loc_info.col_pos);
        }
    }

    if (lhs->kind == AST_INT && rhs->kind == AST_INT) {
        s64 lhs_v = lhs->Int.val;
        s64 rhs_v = rhs->Int.val;
        s64 value = 0;

        // clang-format off
        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_EQ_EQ:         value = (lhs_v == rhs_v); break;
        case TOKEN_BANG_EQ:       value = (lhs_v != rhs_v); break;
        case TOKEN_PLUS:          value = (lhs_v + rhs_v);  break;
        case TOKEN_MINUS:         value = (lhs_v - rhs_v);  break;
        case TOKEN_ASTERISK:      value = (lhs_v * rhs_v);  break;
        case TOKEN_FWSLASH:       value = (lhs_v / rhs_v);  break;
        case TOKEN_AND:           value = (lhs_v & rhs_v);  break;
        case TOKEN_PIPE:          value = (lhs_v | rhs_v);  break;
        case TOKEN_LT:            value = (lhs_v < rhs_v);  break;
        case TOKEN_GT:            value = (lhs_v > rhs_v);  break;
        case TOKEN_GT_GT:         value = (lhs_v >> rhs_v); break;
        case TOKEN_LT_LT:         value = (lhs_v << rhs_v); break;
        case TOKEN_PERCENT:       value = (lhs_v % rhs_v);  break;
        case TOKEN_HAT:           value = (lhs_v ^ rhs_v);  break;
        case TOKEN_AND_AND:       value = (lhs_v && rhs_v); break;
        case TOKEN_PIPE_PIPE:     value = (lhs_v || rhs_v); break;
        case TOKEN_QUESTION_MARK: return;
        case TOKEN_COLON:         return;
        }
        // clang-format on

        lhs->Int.val = value;
        ast_replace(node, lhs);
    } else if (lhs->kind == AST_FLOAT && rhs->kind == AST_FLOAT) {
        f64 lhs_v = lhs->Float.val;
        f64 rhs_v = rhs->Float.val;
        f64 value = 0.0;

        // clang-format off
        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_EQ_EQ:     value = (lhs_v == rhs_v); break;
        case TOKEN_BANG_EQ:   value = (lhs_v != rhs_v); break;
        case TOKEN_PLUS:      value = (lhs_v + rhs_v);  break;
        case TOKEN_MINUS:     value = (lhs_v - rhs_v);  break;
        case TOKEN_ASTERISK:  value = (lhs_v * rhs_v);  break;
        case TOKEN_FWSLASH:   value = (lhs_v / rhs_v);  break;
        case TOKEN_LT:        value = (lhs_v < rhs_v);  break;
        case TOKEN_GT:        value = (lhs_v > rhs_v);  break;
        case TOKEN_AND_AND:   value = (lhs_v && rhs_v); break;
        case TOKEN_PIPE_PIPE: value = (lhs_v || rhs_v); break;
        }
        // clang-format on

        lhs->Float.val = value;
        ast_replace(node, lhs);
    }

    // -- Replace any expression matching:
    //      1 * x => x
    //      x * 1 => x
    //      x / 1 => x
    //    With the non literal value 'x'.
    if (op == TOKEN_ASTERISK) {
        if (expr_is_literal_value(lhs, 1.0)) {
            ast_replace(node, rhs);
        } else if (expr_is_literal_value(rhs, 1.0)) {
            ast_replace(node, lhs);
        }
    } else if (op == TOKEN_FWSLASH && expr_is_literal_value(rhs, 1.0)) {
        ast_replace(node, lhs);
    }
}
void constant_fold(void* dont_care, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_BINARY: constant_fold_binary(node); break;
    case AST_UNARY: constant_fold_unary(node); break;
    }
}

typedef struct
{
    AST_Kind kind;
    void* list;
} AST_FindAll_Query;

void ast_query(void* query, AST* node) {
    AST_FindAll_Query* q = query;
    if (node->kind == q->kind) {
        list_append(q->list, node);
    }
}

List* ast_find_all_of_kind(AST_Kind kind, AST* ast) {
    List* list = make_list();
    AST_FindAll_Query query = {kind, list};
    ast_visit(ast_query, &query, ast);
    return list;
}

void thi_run_pass(Thi* thi, char* pass_description, ast_callback* visitor_func, void* visitor_func_arg) {
    info("running pass: %s", pass_description);
    push_timer(thi, pass_description);
    ast_visit(visitor_func, visitor_func_arg, thi->ast);
    pop_timer(thi);
    info("... COMPLETED.");
}

int main(int argc, char** argv) {
    // @Todo(marcus) do more robust argument handling
    // Argument validation
    if (argc < 2) error("too few arguments.");

    info("Compiler was last compiled: "__TIME__);

#ifndef NDEBUG
    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    lexer_test();
#endif

    Thi thi = make_thi();

    bool listen_mode = false;

    s32 opt;
    while ((opt = getopt(argc, argv, "lhv")) != -1) {
        switch (opt) {
        case 'h': {
            puts("Usage:");
            puts("      thi <file>");
            puts("      thi -v");
            return 0;
        } break;
        case 'v': puts(COMPILER_VERSION); return 0;
        case 'l': listen_mode = true; break;
        case ':': info("option needs a value\n"); return 0;
        case '?': info("unknown option: %c\n", optopt); return 0;
        }
    }

    success("listen mode: %d", listen_mode);

    set_source_file(&thi, argv[optind]);
    info("filename: %s\n", argv[optind]);

    if (thi.debug_mode) {
        thi.detailed_print = true;
        thi.optimize = false;
        thi.enable_constant_folding = false;
    }

    push_timer(&thi, "Total time");

    // Grab the source file
    char* source_file = get_source_file(&thi);
    info("Compiling %s", source_file);

    char* ext = get_file_extension(source_file);
    char* dir = get_file_directory(source_file);
    char* name = get_file_name(source_file);
    char* exec_name = remove_file_extension(name);

    thi.input_file = source_file;

    set_source_file(&thi, name);
    set_current_directory(&thi, dir);

    info(source_file);
    info("ext: %s", ext);
    info("dir: %s", dir);
    info("name: %s", name);
    info("exec_name: %s", exec_name);

    // Make sure it's actually a .thi file
    if (strcmp(ext, ".thi") != 0) {
        error("thats not a thi file...");
    }

    add_load(&thi, name);
    add_link(&thi, "-lSystem");

    add_symbol(&thi, "void", make_type_void());
    add_symbol(&thi, "bool", make_type_int(1, true));
    add_symbol(&thi, "char", make_type_int(1, true));
    add_symbol(&thi, "int", make_type_int(DEFAULT_INT_BYTE_SIZE, false));
    add_symbol(&thi, "float", make_type_float(DEFAULT_FLOAT_BYTE_SIZE));
    add_symbol(&thi, "double", make_type_float(8));

    add_symbol(&thi, "s8", make_type_int(1, false));
    add_symbol(&thi, "s16", make_type_int(2, false));
    add_symbol(&thi, "s32", make_type_int(4, false));
    add_symbol(&thi, "s64", make_type_int(8, false));

    add_symbol(&thi, "u8", make_type_int(1, true));
    add_symbol(&thi, "u16", make_type_int(2, true));
    add_symbol(&thi, "u32", make_type_int(4, true));
    add_symbol(&thi, "u64", make_type_int(8, true));

    add_symbol(&thi, "f32", make_type_float(4));
    add_symbol(&thi, "f64", make_type_float(8));

    // Parse
    Parser_Context pctx = make_parser_context();
    pctx.file = source_file;
    pctx.symbols = thi.symbol_map;
    AST* ast = parse(&pctx, name);

    // ast = make_ast_module(ast->loc_info, source_file, ast);

    thi.links = ast_find_all_of_kind(AST_LINK, ast);
    info("all loaded files");
    LIST_FOREACH(pctx.loads) {
        info("file: %s", it->data);
    }

    info("%s", ast_to_str(ast));
    thi.ast = ast;

    info("Running passes");

    thi_run_pass(&thi, "pass_initilize_enums", pass_initilize_enums, &thi);
    thi_run_pass(&thi, "pass_add_all_symbols", pass_add_all_symbols, &thi);

    if (listen_mode) {
        success("writing syntax file...");
        write_syntax_file(&thi);
        return 0;
    }

    // Run all passes
    print_symbol_map(&thi);

    //
    // PASS: resolve all unresolved types
    //
    thi_run_pass(&thi, "resolve_unresolved_types", visitor_resolve_unresolved_types, &thi);

    //
    // Semantic analysis
    //
    semantic_analysis(ast);

    PassDescriptor passDesc; // We reuse this one
    passDesc.description = "Resolve sizeofs";
    passDesc.kind = AST_SIZEOF;
    passDesc.passKind = PASS_UNSAFE;
    passDesc.visitor_func = resolve_sizeofs;
    passDesc.visitor_arg = NULL;
    thi_install_pass(&thi, passDesc);

    passDesc.description = "Resolve typeofs";
    passDesc.kind = AST_TYPEOF;
    passDesc.visitor_func = resolve_typeofs;
    thi_install_pass(&thi, passDesc);

    passDesc.description = "Resolve subscripts";
    passDesc.kind = AST_SUBSCRIPT;
    passDesc.visitor_func = resolve_subscript;
    thi_install_pass(&thi, passDesc);

    passDesc.description = "Resolve field access";
    passDesc.kind = AST_FIELD_ACCESS;
    passDesc.visitor_func = resolve_field_access;
    thi_install_pass(&thi, passDesc);  

    // Run all passes
    push_timer(&thi, "Run all passes");
    ast_visit(run_all_passes, &thi, ast);
    pop_timer(&thi);

    // typechecking pass
    // type_checker(thi.symbol_map, ast);

    //
    // Optimization Pass:
    //      Constant propogation.
    //       References to constant variables are replaced by their constant
    //       value.
    //
    List* idents = ast_find_all_of_kind(AST_IDENT, ast);
    List* constant_decls =
        ast_find_all_of_kind(AST_CONSTANT_DECL, ast);
    LIST_FOREACH(idents) {
        AST* ident = it->data;
        LIST_FOREACH(constant_decls) {
            AST* const_decl = it->data;
            if (get_ast_name(ident) == get_ast_name(const_decl)) {
                ast_replace(ident, const_decl->Constant_Decl.value);
                ident->type = const_decl->type;
                break;
            }
        }
    }

    // Sanity check.. Make sure the typer did what it was supposed to do.
    thi_run_pass(&thi, "make_sure_all_nodes_have_a_valid_type", make_sure_all_nodes_have_a_valid_type, NULL);

    //  Optimization passes
    thi_run_pass(&thi, "constant_fold", constant_fold, NULL);

    // Sanity checks
    thi_run_pass(&thi, "check_for_unresolved_types", check_for_unresolved_types, NULL);
    thi_run_pass(&thi, "make_sure_all_nodes_have_a_valid_type", make_sure_all_nodes_have_a_valid_type, NULL);

    // Remove unused externs
    List* externs = ast_find_all_of_kind(AST_EXTERN, ast);
    List* calls = ast_find_all_of_kind(AST_CALL, ast);
    LIST_FOREACH(externs) {
        AST* node_e = it->data;
        bool used = false;
        LIST_FOREACH(calls) {
            AST* node_c = it->data;
            if (strcmp(node_e->Extern.type->Function.name, node_c->Call.callee) == 0) {
                used = true;
            }
        }
        if (!used) ast_replace(node_e, make_ast_nop(node_e->loc_info));
    }

    // char* json = ast_to_json(ast);
    // write_to_file("ast.json", json);

    info("%s", ast_to_str(ast));

    // Codegen
    push_timer(&thi, "Codegen");
    char* output = thi.backend == BACKEND_X64 ? generate_code_from_ast(ast) : generate_llvm_from_ast(ast);
    pop_timer(&thi);

    // Write to file
    if (output) {
        char* output_filename = thi.backend == BACKEND_X64 ? "output.s" : "output.ll";
        write_to_file(output_filename, output);
        assemble(&thi, "output", exec_name);
        linking_stage(&thi, exec_name);
    } else
        error("generating code from ast failed.");

    // Debug info. Writing out sizes of our types.
    info("size of Token:   %lu bytes", sizeof(Token));
    info("size of AST:     %lu bytes", sizeof(AST));
    info("size of Type:    %lu bytes", sizeof(Type));
    info("size of Value:   %lu bytes", sizeof(Value));
    info("size of Map:     %lu bytes", sizeof(Map));
    info("size of List:    %lu bytes", sizeof(List));
    info("size of Stack:   %lu bytes", sizeof(Stack));
    info("size of string:  %lu bytes", sizeof(string));

    pop_timer(&thi);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    success("--- Compiler timings ---");
    success("lines %s%s comments %s", give_unique_color(strf("%lld", pctx.lines)), RGB_GRAY, give_unique_color(strf("%lld", pctx.comments)));
    LIST_FOREACH(get_timers(&thi)) {
        Timer* tm = it->data;
        s64 len = strlen(tm->desc);
        char* ms = strf("%f seconds", tm->ms / 1e3);
        s64 ms_l = strlen(ms);
        s64 padding = w.ws_col - len - ms_l - 1; // -1 is the ':'
        success("%s", give_unique_color(strf("%s:%*s%s", tm->desc, padding, "", ms)));
    }
    success("---------------------------");
// Write Unoptimized AST out
#ifndef NDEBUG
    write_to_file("output.thi", ast_to_source(ast));
#endif

    write_syntax_file(&thi);

    return 0;
}

void assemble(Thi* thi, char* asm_file, char* exec_name) {
    if (thi->backend == BACKEND_LLVM) {
        push_timer(thi, "LLVM");
        system(PATH_TO_LLC " ./output.ll --x86-asm-syntax=intel");
        pop_timer(thi);
    }
    string* comp_call = string_create_f("nasm -f macho64 -g %s.s -o %s.o", asm_file, exec_name);
    info("Assembling with options '%s'", string_data(comp_call));
    push_timer(thi, "Assembler");
    system(string_data(comp_call));
    pop_timer(thi);
}

void linking_stage(Thi* thi, char* exec_name) {
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main", exec_name, exec_name);
    List* links = get_link_list(thi);
    LIST_FOREACH(links) {
        AST* link = it->data;
        link_call = strf("%s %s", link_call, link->Link.str);
    }
    info("Linking with options '%s'", link_call);
    push_timer(thi, "Linker");
    system(link_call);
    pop_timer(thi);

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
}

List* string_split(string* this, char delimiter) {
    List* list_of_delimited_strings = make_list();

    char* cursor = this->c_str;
    char* start_of_word = this->c_str;

    while (*cursor) {
        if (*cursor == delimiter) {
            u64 len = cursor - start_of_word;
            char* str = xmalloc(len); // cursor is at the delimiter so we dont need a +1
            memcpy(str, start_of_word, len);
            str[len] = 0;
            list_append(list_of_delimited_strings, str);
            start_of_word = cursor + 1; // +1 skips the delimiter
        }
        ++cursor;
    }

    // u64 len = cursor - start_of_word;
    // char* str = xmalloc(len+1); // cursor is at the delimiter so we dont need a +1
    // memcpy(str, start_of_word, len);
    // str[len] = 0;

    // list_append(list_of_delimited_strings, str);

    return list_of_delimited_strings;
}

List* string_list_remove_duplicates(List* list) {
    List* res = make_list();
    LIST_FOREACH(list) {
        char* a = it->data;
        bool dup = false;
        LIST_FOREACH(res) {
            char* b = it->data;
            if (strcmp(a, b) == 0) {
                dup = true;
            }
        }
        if (!dup) list_append(res, a);
    }
    return res;
}

void write_syntax_file(Thi* thi) {
    // s64 count = map_count(thi->symbol_map);
    // string* known_types = string_create("");
    // string* known_funcs = string_create("");
    // for (s64 i = 0; i < count; ++i) {
    //     Type* type = thi->symbol_map->data[i].data;
    //     switch (type->kind) {
    //     case TYPE_POINTER: break;
    //     case TYPE_FUNCTION:
    //         string_append_f(known_funcs, "%s ", get_type_name(type));
    //         break;
    //     default:
    //         string_append_f(known_types, "%s ", get_type_name(type));
    //         break;
    //     }
    // }
    // List* unique_type_string_list = string_list_remove_duplicates(string_split(known_types, ' '));
    // List* unique_func_string_list = string_list_remove_duplicates(string_split(known_funcs, ' '));

    // string* t = string_create("");
    // string* f = string_create("");
    // LIST_FOREACH(unique_type_string_list) {
    //     string_append_f(t, "%s", it->data);
    //     if (it->next) string_append(t, "|");
    // }
    // LIST_FOREACH(unique_func_string_list) {
    //     string_append_f(f, "%s", it->data);
    //     if (it->next) string_append(f, "|");
    // }

    // string* s = string_create("");
    // string_append(s, "%YAML 1.2\n");
    // string_append(s, "---\n");
    // string_append(s, "# See http://www.sublimetext.com/docs/3/syntax.html\n");
    // string_append(s, "file_extensions:\n");
    // string_append(s, "  - thi\n");
    // string_append(s, "scope: source.thi\n");
    // string_append(s, "\n");
    // string_append(s, "contexts:\n");
    // string_append(s, "  # The prototype context is prepended to all contexts but those setting\n");
    // string_append(s, "  # meta_include_prototype: false.\n");
    // string_append(s, "  prototype:\n");
    // string_append(s, "    - include: singleline_comments\n");
    // string_append(s, "\n");
    // string_append(s, "  main:\n");
    // string_append(s, "    # The main context is the initial starting point of our syntax.\n");
    // string_append(s, "    # Include other contexts from here (or specify them directly).\n");
    // string_append(s, "    - include: keywords\n");
    // string_append(s, "    - include: function_def\n");
    // string_append(s, "    - include: numbers\n");
    // string_append(s, "    - include: hex\n");
    // string_append(s, "    - include: globals\n");
    // string_append(s, "    - include: strings\n");
    // string_append(s, "    - include: character_literal\n");
    // string_append(s, "    - include: basic_types\n");
    // string_append(s, "    - include: operators\n");
    // string_append(s, "\n");
    // string_append(s, "  keywords:\n");
    // string_append(s, "    # Keywords are if, else for and while.\n");
    // string_append(s, "    # Note that blackslashes don't need to be escaped within single quoted\n");
    // string_append(s, "    # strings in YAML. When using single quoted strings, only single quotes\n");
    // string_append(s, "    # need to be escaped: this is done by using two single quotes next to each\n");
    // string_append(s, "    # other.\n");
    // string_append(s, "    - match: '\\b(interface|enum|struct|class|def|astof|sizeof|typeof|fallthrough|in|true|false|extern|link|union|is|load|if|else|for|while|return|break|continue|defer)\\b'\n");
    // string_append(s, "      scope: keyword.control\n");
    // string_append(s, "\n");
    // string_append(s, "  function_def:\n");
    // string_append_f(s, "    - match: '\\b(%s)\\b'\n", string_data(f));
    // string_append(s, "      scope: entity.name.function\n");
    // string_append(s, "\n");
    // string_append(s, "  numbers:\n");
    // string_append(s, "    - match: '[+-]?[0-9_]+(e[0-9]+)?([lL|LL|uU])*'\n");
    // string_append(s, "      scope: constant.numeric\n");
    // string_append(s, "    - match: '[+-]?[0-9_]+.+(e[0-9.]+)?([fF])*'\n");
    // string_append(s, "      scope: constant.numeric\n");
    // string_append(s, "\n");
    // string_append(s, "  hex:\n");
    // string_append(s, "    - match: '\\b0x(-)?[0-9A-Za-z]+\\b'\n");
    // string_append(s, "      scope: constant.numeric\n");
    // string_append(s, "\n");
    // string_append(s, "  operators:\n");
    // string_append(s, "    - match: '[\\.!:=+-?;{},-><&*$\\[\\]]+'\n");
    // string_append(s, "      scope: keyword.operator\n");
    // string_append(s, "\n");
    // string_append(s, "  basic_types:\n");
    // string_append_f(s, "    - match: '\\b(%s)\\b'\n", string_data(t));
    // string_append(s, "      scope: storage.type\n");
    // string_append(s, "\n");
    // string_append(s, "  strings:\n");
    // string_append(s, "    # Strings begin and end with quotes, and use backslashes as an escape\n");
    // string_append(s, "    # character.\n");
    // string_append(s, "    - match: \"\\\"\"\n");
    // string_append(s, "      scope: punctuation.definition.string.begin.example-c\n");
    // string_append(s, "      push: inside_string\n");
    // string_append(s, "\n");
    // string_append(s, "  inside_string:\n");
    // string_append(s, "    - meta_include_prototype: false\n");
    // string_append(s, "    - meta_scope: string.quoted.double.example-c\n");
    // string_append(s, "    - match: '\\.'\n");
    // string_append(s, "      scope: constant.character.escape.example-c\n");
    // string_append(s, "    - match: '\"'\n");
    // string_append(s, "      scope: punctuation.definition.string.end.example-c\n");
    // string_append(s, "      pop: true\n");
    // string_append(s, "\n");
    // string_append(s, "  character_literal:\n");
    // string_append(s, "    # Strings begin and end with quotes, and use backslashes as an escape\n");
    // string_append(s, "    # character.\n");
    // string_append(s, "    - match: '\'''\n");
    // string_append(s, "      scope: punctuation.definition.™©string.begin.example-c\n");
    // string_append(s, "      push: inside_character_literal\n");
    // string_append(s, "\n");
    // string_append(s, "  inside_character_literal:\n");
    // string_append(s, "    - meta_include_prototype: false\n");
    // string_append(s, "    - meta_scope: string.quoted.double.example\n");
    // string_append(s, "    - match: '\\.'\n");
    // string_append(s, "      scope: constant.character.escape.example\n");
    // string_append(s, "    - match: '\'''\n");
    // string_append(s, "      scope: punctuation.definition.string.end.example\n");
    // string_append(s, "      pop: true\n");
    // string_append(s, "\n");
    // string_append(s, "  singleline_comments:\n");
    // string_append(s, "    # Comments begin with a '#' and finish at the end of the line.\n");
    // string_append(s, "    - match: '#'\n");
    // string_append(s, "      scope: comment.line.double-slash\n");
    // string_append(s, "      push:\n");
    // string_append(s, "        # This is an anonymous context push for brevity.\n");
    // string_append(s, "        - meta_scope: comment.line.double-slash\n");
    // string_append(s, "        - match: $\\w?\n");
    // string_append(s, "          pop: true\n");

    // write_to_file("thi.sublime-syntax", string_data(s));
    // system("perl -pi -e 's/[^[:ascii:]]//g' ./thi.sublime-syntax");
    // system("cp ./thi.sublime-syntax /Users/marcusmathiassen/Library/Mobile\\ Documents/com~apple~CloudDocs/Sublime/User/thi.sublime-syntax");
}
