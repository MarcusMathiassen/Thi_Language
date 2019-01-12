#include "ast.h"     // AST
#include "codegen.h" // generate_code_from_ast
#include "context.h" // ctx_tests
#include "globals.h" // init_maps
#include "lexer.h"   // generate_tokens_from_source, print_tokens
#include "list.h"    // list_tests
#include "map.h"     // map
#include "parser.h"  // generate_ast_from_tokens
#include "stack.h"   // stack_tests
#include "string.h"  // strcmp
#include "typedefs.h"
#include "typespec.h" // Typespec
#include "utility.h"  // get_file_content, success, info, get_time
#include "value.h"    // Value
#include <assert.h>   // assert
#include <stdio.h>    // sprintf
#include <stdlib.h>   // free
#include <string.h>   // strcmp

//------------------------------------------------------------------------------
//                               Main Driver
//------------------------------------------------------------------------------
void assemble(char* asm_file, char* exec_name);
void link(char* exec_name);

/*
    Figure out all the files that are loaded by the source.
*/

int main(int argc, char** argv)
{
    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    ctx_tests();

    initilize_globals();
    init_interns_list();

    push_timer("Total time");

    // Argument validation
    if (argc < 2)
        error("too few arguments.");
    else if (argc > 3)
        error("too many arguments.");

    // Grab the source file
    char* source_file = argv[1];
    char* exec_name = argv[2];
    info("Compiling %s", source_file);

    // Setup types
    Typespec* u8_t = make_typespec_int(1, true);
    Typespec* u16_t = make_typespec_int(2, true);
    Typespec* u32_t = make_typespec_int(4, true);
    Typespec* u64_t = make_typespec_int(8, true);

    Typespec* s8_t = make_typespec_int(1, false);
    Typespec* s16_t = make_typespec_int(2, false);
    Typespec* s32_t = make_typespec_int(4, false);
    Typespec* s64_t = make_typespec_int(8, false);

    Typespec* f32_t = make_typespec_float(4);
    Typespec* f64_t = make_typespec_float(8);

    add_builtin_type("void", u8_t);
    
    add_builtin_type("int", s32_t);
    add_builtin_type("float", f32_t);
    add_builtin_type("double", f32_t);
    
    add_builtin_type("s8", s8_t);
    add_builtin_type("s16", s16_t);
    add_builtin_type("s32", s32_t);
    add_builtin_type("s64", s64_t);

    add_builtin_type("u8", u8_t);
    add_builtin_type("u16", u16_t);
    add_builtin_type("u32", u32_t);
    add_builtin_type("u64", u64_t);
    add_builtin_type("f32", f32_t);
    add_builtin_type("f64", f64_t);

    char* ext = get_file_extension(source_file);
    char* dir = get_file_directory(source_file);
    char* name = get_file_name(source_file);

    set_source_file(name);
    set_current_dir(dir);

    info(source_file);
    info("ext: %s", ext);
    info("dir: %s", dir);
    info("name: %s", name);
    info("exec_name: %s", exec_name);

    // Make sure it's actually a .thi file
    if (strcmp(ext, "thi") != 0) {
        error("%s is not a .thi file.", source_file);
    }

    // Parse
    List ast;
    list_init(&ast);
    parse(&ast, source_file);

    // Codegen
    push_timer("Codegen");
    char* output = generate_code_from_ast(&ast);
    pop_timer();

    // Write to file
    if (output) {
        char* output_filename = "output.asm";
        write_to_file(output_filename, output);
        assemble(output_filename, exec_name);
        link(exec_name);
    } else {
        error("generating code from ast failed.");
    }

    // Debug info. Writing out sizes of our types.
    info("size of Token: %lu bytes", sizeof(Token));
    info("size of Expr: %lu bytes", sizeof(Expr));
    info("size of Typespec: %lu bytes", sizeof(Typespec));
    info("size of Value: %lu bytes", sizeof(Value));

    pop_timer();
    info("==------------ Thi ------------==");
    List* timers = get_timers();
    LIST_FOREACH(timers)
    {
        Timer* tm = (Timer*)it->data;
        info("%s: %f seconds", tm->desc, tm->ms / 1e3);
    }
    info("==------------ === ------------==");

    return 0;
}

void assemble(char* asm_file, char* exec_name)
{
    string comp_call = make_string_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer("Assembler");
    system(comp_call.c_str);
    free_string(&comp_call);
    pop_timer();

    // system("cat output.asm");

    // Cleanup
    // string rm_asm_file = make_string_f("rm %s", asm_file);
    // system(rm_asm_file.c_str);
    // free_string(&rm_asm_file);
}

void link(char* exec_name)
{
    string link_call =
        make_string_f("ld -macosx_version_min 10.14 -lSystem -o %s %s.o -e _main -lc", exec_name, exec_name);
    push_timer("Linker");
    system(link_call.c_str);
    free_string(&link_call);
    pop_timer();

    // Cleanup
    string rm_call = make_string_f("rm %s.o", exec_name);
    system(rm_call.c_str);
    free_string(&rm_call);
}
