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
#include "type_checker.h" // type_checker
#include "typedefs.h"     // u8, u16, s32, etc.
#include "utility.h"      // get_file_content, success, info, get_time
#include "value.h"        // Value
#include <assert.h>       // assert
#include <stdio.h>        // sprintf
#include <stdlib.h>       // free
#include <string.h>       // strcmp
#include <sys/ioctl.h>    // NOTE(marcus): what do i use this for?
#include <unistd.h>       // NOTE(marcus): what do i use this for?

void assemble(Thi* thi, char* asm_file, char* exec_name);
void linking_stage(Thi* thi, char* exec_name);

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
            default:
                error("unhandled case: %s, %s, %s", ast_kind_to_str(m->kind), __func__, __LINE__);
            case AST_IDENT:
                it->data = make_ast_constant_decl(m->loc_info, m->Ident.name, make_ast_int(m->loc_info, counter, make_type_int(DEFAULT_INT_BYTE_SIZE, 0)));
                break;
            case AST_CONSTANT_DECL:
                assert(m->Constant_Decl.value->kind == AST_INT);
                counter = m->Constant_Decl.value->Int.val;
                break;
            }
            counter += 1;
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
        error(
            "[check_for_unresolved_types]: unresolved type found for node: %s",
            ast_to_str(node));
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
        default: ERROR_UNHANDLED_KIND(token_kind_to_str(op));
        case TOKEN_BANG: value = !oper_v; break;
        case TOKEN_PLUS: value = oper_v; break;
        case TOKEN_TILDE: value = ~oper_v; break;
        case TOKEN_MINUS: value = -oper_v; break;
        }

        operand->Int.val = value;
        ast_replace(node, operand);
    }
}
void constant_fold_binary(AST* node) {
    Token_Kind op = node->Binary.op;
    AST* lhs = node->Binary.lhs;
    AST* rhs = node->Binary.rhs;
    if (lhs->kind == AST_GROUPING) lhs = lhs->Grouping.node;
    if (rhs->kind == AST_GROUPING) rhs = rhs->Grouping.node;

    // Remove any + or - where lhs or rhs is 0.
    if (op == TOKEN_MINUS || op == TOKEN_PLUS) {
        if ((lhs->kind == AST_INT && lhs->Int.val == 0) || (lhs->kind == AST_FLOAT && lhs->Float.val == 0.0)) {
            ast_replace(node, rhs);
            return;
        } else if ((rhs->kind == AST_INT && rhs->Int.val == 0) || (rhs->kind == AST_FLOAT && rhs->Float.val == 0.0)) {
            ast_replace(node, lhs);
            return;
        }
    }

    if (lhs->kind == AST_INT && rhs->kind == AST_INT) {
        s64 lhs_v = lhs->Int.val;
        s64 rhs_v = rhs->Int.val;
        s64 value = 0;

        switch (op) {
        default: ERROR_UNHANDLED_KIND(token_kind_to_str(op));
        case TOKEN_EQ_EQ: value = (lhs_v == rhs_v); break;
        case TOKEN_BANG_EQ: value = (lhs_v != rhs_v); break;
        case TOKEN_PLUS: value = (lhs_v + rhs_v); break;
        case TOKEN_MINUS: value = (lhs_v - rhs_v); break;
        case TOKEN_ASTERISK: value = (lhs_v * rhs_v); break;
        case TOKEN_FWSLASH: value = (lhs_v / rhs_v); break;
        case TOKEN_AND: value = (lhs_v & rhs_v); break;
        case TOKEN_PIPE: value = (lhs_v | rhs_v); break;
        case TOKEN_LT: value = (lhs_v < rhs_v); break;
        case TOKEN_GT: value = (lhs_v > rhs_v); break;
        case TOKEN_GT_GT: value = (lhs_v >> rhs_v); break;
        case TOKEN_LT_LT: value = (lhs_v << rhs_v); break;
        case TOKEN_PERCENT: value = (lhs_v % rhs_v); break;
        case TOKEN_HAT: value = (lhs_v ^ rhs_v); break;
        case TOKEN_AND_AND: value = (lhs_v && rhs_v); break;
        case TOKEN_PIPE_PIPE: value = (lhs_v || rhs_v); break;
        case TOKEN_QUESTION_MARK: return;
        case TOKEN_COLON: return;
        }

        lhs->Int.val = value;
        ast_replace(node, lhs);
    } else if (lhs->kind == AST_FLOAT && rhs->kind == AST_FLOAT) {
        f64 lhs_v = lhs->Float.val;
        f64 rhs_v = rhs->Float.val;
        f64 value = 0.0;

        switch (op) {
        default: ERROR_UNHANDLED_KIND(token_kind_to_str(op));
        case TOKEN_EQ_EQ: value = (lhs_v == rhs_v); break;
        case TOKEN_BANG_EQ: value = (lhs_v != rhs_v); break;
        case TOKEN_PLUS: value = (lhs_v + rhs_v); break;
        case TOKEN_MINUS: value = (lhs_v - rhs_v); break;
        case TOKEN_ASTERISK: value = (lhs_v * rhs_v); break;
        case TOKEN_FWSLASH: value = (lhs_v / rhs_v); break;
        case TOKEN_LT: value = (lhs_v < rhs_v); break;
        case TOKEN_GT: value = (lhs_v > rhs_v); break;
        case TOKEN_AND_AND: value = (lhs_v && rhs_v); break;
        case TOKEN_PIPE_PIPE: value = (lhs_v || rhs_v); break;
        }

        lhs->Float.val = value;
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
        list_append((List*)q->list, node);
    }
}

List* ast_find_all_of_kind(AST_Kind kind, AST* ast) {
    List* list = make_list();
    AST_FindAll_Query query = {kind, list};
    ast_visit(ast_query, &query, ast);
    return list;
}

void thi_run_pass(Thi* thi, char* pass_description, void (*visitor_func)(void*, AST*), void* visitor_func_arg) {
    success("running pass: %s", pass_description);
    push_timer(thi, pass_description);
    ast_visit(visitor_func, visitor_func_arg, thi->ast);
    pop_timer(thi);
    success("... COMPLETED.");
}

int main(int argc, char** argv) {
    // @Todo(marcus) do more robust argument handling
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    lexer_test();

    Thi thi = make_thi();

    set_source_file(&thi, argv[1]);
    info("filename: %s\n", argv[1]);

    s32 opt;
    while ((opt = getopt(argc, argv, ":hdv:")) != -1) {
        switch (opt) {
        case 'h': {
            info("--- Thi Compiler ---");
            info("Usage:");
            info("      thi -f <file>");
            info("      thi -v");
            return 0;
        } break;
        case 'v': thi.detailed_print = true; break;
        case 'd': thi.debug_mode = true; break;
        case ':': info("option needs a value\n"); return 0;
        case '?': info("unknown option: %c\n", optopt); return 0;
        }
    }

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

    success("all loaded files");
    LIST_FOREACH(pctx.loads) {
        success("file: %s", it->data);
    }

    info("%s", ast_to_str(ast));
    thi.ast = ast;

    info("Running passes");

    thi_run_pass(&thi, "pass_initilize_enums", pass_initilize_enums, &thi);
    thi_run_pass(&thi, "pass_add_all_symbols", pass_add_all_symbols, &thi);

    // Run all passes
    print_symbol_map(&thi);

    //
    // PASS: resolve all unresolved types
    //
    thi_run_pass(&thi, "resolve_unresolved_types", visitor_resolve_unresolved_types, &thi);

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

    //
    // Semantic analysis
    //
    semantic_analysis(ast);

    // Run all passes
    push_timer(&thi, "Run all passes");
    ast_visit(run_all_passes, &thi, ast);
    pop_timer(&thi);

    // typechecking pass
    type_checker(thi.symbol_map, ast);

    // Sanity check.. Make sure the typer did what it was supposed to do.
    thi_run_pass(&thi, "make_sure_all_nodes_have_a_valid_type", make_sure_all_nodes_have_a_valid_type, NULL);

    // Write Unoptimized AST out
    write_to_file("output.thi", ast_to_source(ast));

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
            if (strcmp(ident->Ident.name, const_decl->Constant_Decl.name) ==
                0) {
                ast_replace(ident, const_decl->Constant_Decl.value);
                ident->type = const_decl->type;
                break;
            }
        }
    }

    //  Optimization passes
    thi_run_pass(&thi, "constant_fold", constant_fold, NULL);

    // Sanity checks
    thi_run_pass(&thi, "check_for_unresolved_types", check_for_unresolved_types, NULL);
    thi_run_pass(&thi, "make_sure_all_nodes_have_a_valid_type", make_sure_all_nodes_have_a_valid_type, NULL);

    // Remove unused externs
    // List* externs = ast_find_all_of_kind(AST_EXTERN, ast);
    // List* calls = ast_find_all_of_kind(AST_CALL, ast);
    // LIST_FOREACH(externs) {
    //     AST* node_e = it->data;
    //     bool used = false;
    //     LIST_FOREACH(calls) {
    //         AST* node_c = it->data;
    //         if (strcmp(node_e->Extern.type->Function.name, node_c->Call.callee) == 0) {
    //             used = true;
    //         }
    //     }
    //     if (!used) ast_replace(node_e, make_ast_nop(node_e->loc_info));
    // }

    // char* json = ast_to_json(ast);
    // write_to_file("ast.json", json);

    info("%s", ast_to_str(ast));

    // Codegen
    push_timer(&thi, "Codegen");
    char* output = generate_code_from_ast(ast);
    pop_timer(&thi);

    // Write to file
    if (output) {
        char* output_filename = "output.asm";
        write_to_file(output_filename, output);
        assemble(&thi, output_filename, exec_name);
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

    info("--- Compiler timings ---");
    info("lines %s%s comments %s", give_unique_color(strf("%lld", pctx.lines)), RGB_GRAY, give_unique_color(strf("%lld", pctx.comments)));
    LIST_FOREACH(get_timers(&thi)) {
        Timer* tm = (Timer*)it->data;
        s64 len = strlen(tm->desc);
        char* ms = strf("%f seconds", tm->ms / 1e3);
        s64 ms_l = strlen(ms);
        s64 padding = w.ws_col - len - ms_l - 1; // -1 is the ':'
        info("%s", give_unique_color(strf("%s:%*s%s", tm->desc, padding, "", ms)));
    }
    info("---------------------------");

    return 0;
}

void assemble(Thi* thi, char* asm_file, char* exec_name) {
    string* comp_call = string_create_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer(thi, "Assembler");
    system(string_data(comp_call));
    pop_timer(thi);
}

void linking_stage(Thi* thi, char* exec_name) {
    // char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _%s",
    // exec_name, exec_name, exec_name);
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main",
                           exec_name,
                           exec_name);
    List* links = get_link_list(thi);
    LIST_FOREACH(links) {
        char* l = (char*)it->data;
        link_call = strf("%s %s", link_call, l);
    }
    info("Linking with options '%s'", link_call);
    push_timer(thi, "Linker");
    system(link_call);
    pop_timer(thi);

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
}
