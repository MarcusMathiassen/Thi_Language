#include "ast.h"     // AST
#include "codegen.h" // generate_code_from_ast
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

void run_all_tests(void);

int main(int argc, char** argv)
{
    run_all_tests();

    initilize_globals();

    push_timer("Total time");

    // Argument validation
    if (argc < 2)
        error("too few arguments.");
    else if (argc > 3)
        error("too many arguments.");

    // Grab the source file
    const char* source_file = argv[1];
    const char* exec_name = argv[2];
    success("Compiling %s", source_file);

    // Setup types
    add_builtin_type("char", make_typespec_int(8, true));

    add_builtin_type("i8", make_typespec_int(8, false));
    add_builtin_type("i16", make_typespec_int(16, false));
    add_builtin_type("i32", make_typespec_int(32, false));
    add_builtin_type("i64", make_typespec_int(64, false));

    add_builtin_type("u8", make_typespec_int(8, true));
    add_builtin_type("u16", make_typespec_int(16, true));
    add_builtin_type("u32", make_typespec_int(32, true));
    add_builtin_type("u64", make_typespec_int(64, true));
    add_builtin_type("f32", make_typespec_float(32));
    add_builtin_type("f64", make_typespec_float(64));

    const char* ext = get_file_ext(source_file);
    const char* dir = get_file_dir(source_file);
    const char* name = get_file_name(source_file);

    info(source_file);
    info("ext: %s", ext);
    info("dir: %s", dir);
    info("name: %s", name);
    info("exec_name: %s", exec_name);

    // Make sure it's actually a .thi file
    if (strcmp(ext, "thi") != 0) error("%s is not a .thi file.", source_file);

    // Read in the contents of the source file
    char* source = get_file_content(source_file);

    // Lexing
    push_timer("Lexing");
    Token* tokens = generate_tokens_from_source(source);
    pop_timer();
    // if (tokens) print_tokens(tokens);

    // we can free the source
    free(source);
    source = NULL;

    // Order-independence
    push_timer("Order-independence");
    generate_symbol_table_from_tokens(tokens);
    pop_timer();
    // print_symbol_map();

    // Parsing
    push_timer("Parsing");
    AST** ast = generate_ast_from_tokens(tokens);
    pop_timer();
    // if (ast) print_ast(ast);

    // Codegen
    push_timer("Codegen");
    char* output = generate_code_from_ast(ast);
    pop_timer();

    // Write to file
    if (output) {
        const char* output_filename = "output.asm";
        write_to_file(output_filename, output);
    } else
        error("generating code from ast failed.");

    // Linking
    push_timer("Linking");
#define format "macho64"
#define compiler "nasm"
#define src "output.asm"
#define flags " -g "
#define linker "ld"
#define linker_options "-macosx_version_min 10.14 -lSystem" // -lc";

#define compiler_call compiler " -f " format flags src " -o "
#define linker_call linker " " linker_options " -o "

    char comp_call[256];
    char link_call[256];
    sprintf(comp_call, compiler_call "%s.o", exec_name);
    sprintf(link_call, linker_call "%s %s.o -e main", exec_name, exec_name);

    // system("cat output.asm");
    system(comp_call);
    system(link_call);

    char rm_call[256];
    sprintf(rm_call, "rm %s.o", exec_name);
    system(rm_call);
    system("rm output.asm");
    pop_timer();

    // Debug info. Writing out sizes of our types.
    // info("size of Token: %lu bytes", sizeof(Token));
    // info("size of Expr: %lu bytes", sizeof(Expr));
    // info("size of Typespec: %lu bytes", sizeof(Typespec));
    // info("size of Value: %lu bytes", sizeof(Value));

    pop_timer();

    success("==------------ Thi ------------==");
    List* timers = get_timers();
    LIST_FOREACH(timers)
    {
        Timer* tm = (Timer*)it->data;
        success("%s: %f seconds", tm->desc, tm->ms / 1e3);
    }
    success("==------------ === ------------==");

    return 0;
}

void run_all_tests(void)
{
    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
}
