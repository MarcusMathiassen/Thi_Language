#include "ast.h"              // AST
#include "codegen.h"          // generate_code_from_ast
#include "lexer.h"            // generate_tokens_from_source, print_tokens
#include "parser.h"           // generate_ast_from_tokens
#include "stretchy_buffer.h"  // sb_free
#include "utility.h"          // get_file_content, success, info, get_time
#include <string.h>           // strcmp
#include <assert.h>           // assert
#include "map.h"              // map
#include "values.h"           // Value
#include "typedefs.h"        
#include "globals.h"          // init_maps

//------------------------------------------------------------------------------
//                               Main Driver
//------------------------------------------------------------------------------

void run_all_tests();

int main(int argc, char** argv) {

    push_timer("Total time");

    run_all_tests();

    init_maps();

    // Setup types
    add_builtin_type("i8", make_type_int(8, false));
    add_builtin_type("i16", make_type_int(16, false));
    add_builtin_type("i32", make_type_int(32, false));
    add_builtin_type("i64", make_type_int(64, false));

    add_builtin_type("u8", make_type_int(8, true));
    add_builtin_type("u16", make_type_int(16, true));
    add_builtin_type("u32", make_type_int(32, true));
    add_builtin_type("u64", make_type_int(64, true));

    add_builtin_type("f32", make_type_float(32));
    add_builtin_type("f64", make_type_float(64));

    // Argument validation
    if (argc < 2)
        error("too few arguments.");
    else if (argc > 3)
        error("too many arguments.");

    success("Thi Compiler..");
    const char* source_file = argv[1];

    // Make sure it's actually a .thi file
    const char* ext = get_file_ext(source_file);
    const char* dir = get_file_dir(source_file);
    const char* name = get_file_name(source_file);

    info(source_file);
    info("ext: %s", ext);
    info("dir: %s", dir);
    info("name: %s", name);

    if (strcmp(ext, "thi") != 0)
        error("%s is not a .thi file.", source_file);

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
    push_timer("Building Symbol Table");
    success("Building Symbol Table");
    generate_symbol_table_from_tokens(tokens);
    pop_timer();
    print_symbol_map();

    // Parsing
    push_timer("Building AST");
    AST** ast = generate_ast_from_tokens(tokens);
    pop_timer();
    if (ast) print_ast(ast);

    // Codegen
    push_timer("Generating X64 Assembly");
    char* output = generate_code_from_ast(ast);
    pop_timer();
    // if (output) info(output);

    // Write to file
    if (output) {
        const char* output_filename = "output.asm";
        write_to_file(output_filename, output);
    }

    // Linking
    push_timer("Linking");
    #define name           "out"
    #define format         "macho64"
    #define compiler       "nasm"
    #define src            "output.asm"
    #define flags          " -g "
    #define linker         "ld"
    #define linker_options "-macosx_version_min 10.14 -lSystem" // -lc";
    #define compiler_call  compiler " -f "  format  flags  src  " -o "  name  ".o"
    #define linker_call    linker  " "  linker_options  " -o "  name  " "  name  ".o -e main"

    system("cat output.asm");
    system(compiler_call);
    system(linker_call);
    #define dd "rm " name ".o"
    system(dd);
    pop_timer();

    // Debug info. Writing out sizes of our types.
    info("size of Token: %lu bytes", sizeof(Token));
    info("size of AST: %lu bytes", sizeof(AST));
    info("size of Expr: %lu bytes", sizeof(Expr));
    info("size of Type: %lu bytes", sizeof(Type));
    info("size of Value: %lu bytes", sizeof(Value));

    // Cleanup
    if (tokens) sb_free(tokens);
    if (ast) sb_free(ast);
    if (output) free(output);

    pop_timer();
    
    success("==------------ Thi ------------==");
    Timer* timers = get_timers();
    for (int i = 0; i < sb_count(timers); ++i)
        success("%s: %d ms", timers[i].desc, timers[i].ms);
    success("==------------ === ------------==");

    return 0;
}

void run_all_tests()
{
    utility_tests();
    map_tests();
}
