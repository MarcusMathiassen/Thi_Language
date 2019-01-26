#include <assert.h>   // assert
#include <stdio.h>    // sprintf
#include <stdlib.h>   // free
#include <string.h>   // strcmp
#include "ast.h"      // AST
#include "codegen.h"  // generate_code_from_ast
#include "constants.h"
#include "lexer.h"   // generate_tokens_from_source, print_tokens
#include "list.h"    // list_tests
#include "map.h"     // map
#include "parser.h"  // generate_ast_from_tokens
#include "stack.h"   // stack_tests
#include "string.h"  // strcmp
#include "thi.h"     // Thi
#include "type.h"    // Type
#include "typedefs.h"
#include "utility.h"  // get_file_content, success, info, get_time
#include "value.h"    // Value

#include <sys/ioctl.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//                               Main Driver
//------------------------------------------------------------------------------
void  pass_resolve_all_unresolved_types(Thi* thi, Type_Ref_List* unresolved_types);
void  pass_find_and_add_all_definitions_to_symbol_table(Thi* thi, List* ast);
List* parse(Thi* thi, char* source_file);
void  assemble(Thi* thi, char* asm_file, char* exec_name);
void  linking_stage(Thi* thi, char* exec_name);

int main(int argc, char** argv) {
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();

    Thi thi = make_thi();

    add_link(&thi, "-lSystem");

    s32 opt;
    while ((opt = getopt(argc, argv, "f:dvo:")) != -1) {
        switch (opt) {
            case 'v': thi.detailed_print = true; break;
            case 'd': thi.debug_mode = true; break;
            case 'f':
                set_source_file(&thi, optarg);
                info("filename: %s\n", optarg);
                break;
            case 'o':
                set_output_name(&thi, optarg);
                info("exec_name: %s\n", optarg);
                break;
            case ':': info("option needs a value\n"); break;
            case '?': info("unknown option: %c\n", optopt); break;
        }
    }

    if (thi.debug_mode) {
        thi.detailed_print          = true;
        thi.optimize                = false;
        thi.enable_constant_folding = false;
    }

    push_timer(&thi, "Total time");

    // Grab the source file
    char* source_file = get_source_file(&thi);
    char* exec_name   = get_output_name(&thi);
    info("Compiling %s", source_file);

    // Setup types
    Type* u8_t  = make_type_int(1, true);
    Type* u16_t = make_type_int(2, true);
    Type* u32_t = make_type_int(4, true);
    Type* u64_t = make_type_int(8, true);

    Type* s8_t  = make_type_int(1, false);
    Type* s16_t = make_type_int(2, false);
    Type* s32_t = make_type_int(4, false);
    Type* s64_t = make_type_int(8, false);

    Type* f32_t = make_type_float(4);
    Type* f64_t = make_type_float(8);

    add_symbol(&thi, "void", u8_t);
    add_symbol(&thi, "bool", u8_t);
    add_symbol(&thi, "char", u8_t);

    add_symbol(&thi, "s8", s8_t);
    add_symbol(&thi, "s16", s16_t);
    add_symbol(&thi, "s32", s32_t);
    add_symbol(&thi, "s64", s64_t);

    add_symbol(&thi, "u8", u8_t);
    add_symbol(&thi, "u16", u16_t);
    add_symbol(&thi, "u32", u32_t);
    add_symbol(&thi, "u64", u64_t);

    add_symbol(&thi, "f32", f32_t);
    add_symbol(&thi, "f64", f64_t);

    char* ext  = get_file_extension(source_file);
    char* dir  = get_file_directory(source_file);
    char* name = get_file_name(source_file);

    set_source_file(&thi, name);
    set_current_directory(&thi, dir);

    info(source_file);
    info("ext: %s", ext);
    info("dir: %s", dir);
    info("name: %s", name);
    info("exec_name: %s", exec_name);

    // Make sure it's actually a .thi file
    if (strcmp(ext, "thi") != 0) {
        error("%s is not a .thi file.", source_file);
    }

    List* ast = parse(&thi, source_file);

    // Parse
    LIST_FOREACH(get_file_list(&thi)) {
        char* file  = strf("%s%s", get_current_directory(&thi), it->data);
        List* ast_l = parse(&thi, file);
        list_append_content_of(ast, ast_l);
    }

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
    } else {
        error("generating code from ast failed.");
    }

    // Debug info. Writing out sizes of our types.
    info("size of Token: %lu bytes", sizeof(Token));
    info("size of AST:   %lu bytes", sizeof(AST));
    info("size of Type:  %lu bytes", sizeof(Type));
    info("size of Value: %lu bytes", sizeof(Value));

    pop_timer(&thi);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    info("--- Compiler timings ---");
    info("lines %lld comments %lld", thi.lines, thi.comments);
    LIST_FOREACH(get_timers(&thi)) {
        Timer* tm      = (Timer*)it->data;
        s64    len     = strlen(tm->desc);
        char*  ms      = strf("%f seconds", tm->ms / 1e3);
        s64    ms_l    = strlen(ms);
        s64    padding = w.ws_col - len - ms_l - 1;  // -1 is the ':'
        info("%s:%*s%s", tm->desc, padding, "", ms);
    }
    info("---------------------------");

    return 0;
}

void assemble(Thi* thi, char* asm_file, char* exec_name) {
    string comp_call = make_string_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer(thi, "Assembler");
    system(comp_call.c_str);
    free_string(&comp_call);
    pop_timer(thi);
}

void linking_stage(Thi* thi, char* exec_name) {
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main", exec_name, exec_name);
    List* links     = get_link_list(thi);
    LIST_FOREACH(links) {
        char* l   = (char*)it->data;
        link_call = strf("%s %s", link_call, l);
    }
    info("Linking with options '%s'", link_call);
    push_timer(thi, "Linker");
    system(link_call);
    pop_timer(thi);

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
}

List* parse(Thi* thi, char* source_file) {
    char* last_file = get_source_file(thi);
    char* last_dir  = get_current_directory(thi);
    char* dir       = get_file_directory(source_file);

    set_source_file(thi, source_file);
    set_current_directory(thi, dir);

    push_timer(thi, source_file);

    char*       source = get_file_content(source_file);
    Lexed_File  lf     = generate_tokens_from_source(source);
    Parsed_File pf     = generate_ast_from_tokens(lf.tokens);
    List*       ast    = pf.ast;

    pass_find_and_add_all_definitions_to_symbol_table(thi, ast);
    pass_resolve_all_unresolved_types(thi, &pf.unresolved_types);

    // print_symbol_map(thi);
    // print_tokens(tokens);
    // print_ast(ast);

    pop_timer(thi);

    set_source_file(thi, last_file);
    set_current_directory(thi, last_dir);

    // Update thi
    thi->lines += lf.lines;
    thi->comments += lf.comments;

    return ast;
}

void pass_resolve_all_unresolved_types(Thi* thi, Type_Ref_List* unresolved_types) {
    info("pass_resolve_all_unresolved_types");
    push_timer(thi, "pass_resolve_all_unresolved_types");
    for (s64 i = 0; i < unresolved_types->count; ++i) {
        Type* t = unresolved_types->data[i];
        *t      = *get_symbol(thi, get_type_name(t));
    }
    pop_timer(thi);
}

void pass_find_and_add_all_definitions_to_symbol_table(Thi* thi, List* ast) {
    info("pass_find_and_add_all_definitions_to_symbol_table");
    push_timer(thi, "pass_find_and_add_all_definitions_to_symbol_table");
    LIST_FOREACH(ast) {
        AST* expr = (AST*)it->data;
        switch (expr->kind) {
            case AST_FUNCTION: {
                char* func_name = expr->Function.type->Function.name;
                add_symbol(thi, func_name, expr->Function.type);
            } break;
            case AST_ENUM: {
                char* enum_name = expr->Enum.type->Enum.name;
                add_symbol(thi, enum_name, expr->Enum.type);
            } break;
            case AST_STRUCT: {
                char* struct_name = expr->Struct.type->Struct.name;
                add_symbol(thi, struct_name, expr->Struct.type);
            } break;
            case AST_EXTERN: {
                char* func_name = expr->Extern.node->Function.type->Function.name;
                add_symbol(thi, func_name, expr->Extern.node->Function.type);
            } break;
            case AST_LOAD: {
                char* file_name = expr->Load.node->String.val;
                add_load(thi, file_name);
                list_remove(ast, it);
            } break;
            case AST_LINK: {
                char* link_name = expr->Link.node->String.val;
                add_link(thi, link_name);
                list_remove(ast, it);
            } break;
        }
    }
    pop_timer(thi);
}
