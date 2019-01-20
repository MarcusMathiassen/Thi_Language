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

#include <sys/ioctl.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//                               Main Driver
//------------------------------------------------------------------------------
void assemble(char* asm_file, char* exec_name);
void linking_stage(char* exec_name);

/*
    Figure out all the files that are loaded by the source.
*/

typedef struct
{
    s64     len;
    char*   str;
} Intern;

typedef struct {
    Intern* data;
    s64 count;
    s64 allocated;
} Intern_Array;

Intern_Array* make_intern_array()
{
    Intern_Array* l = xmalloc(sizeof(Intern_Array));
    l->count = 0;
    l->allocated = INTERN_ARRAY_STARTING_ALLOC;
    l->data = xmalloc(sizeof(Intern) * l->allocated);
    return l;
}

void intern_array_append(Intern_Array* l, Intern intern)
{
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated);
    }
    l->data[l->count] = intern;
    l->count += 1;
}

char* intern_range(Intern_Array* intern_array, char* start, char* end)
{
    s64 len = end - start;

    for (s64 i = 0; i < intern_array->count; ++i) {
        Intern intern = (Intern)intern_array->data[i];
        if (intern.len == len && strncmp(intern.str, start, len) == 0) {
            return intern.str;
        }
    }
    char* str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = 0;

    Intern intern;
    intern.len = len;
    intern.str = str;
    intern_array_append(intern_array, intern);

    return str;
}

typedef struct 
{
    bool detailed_print;
    bool debug_mode;
    bool enable_constant_folding;
    bool optimize;

    List* foreign_function_list;
    List* constant_string_list;
    List* link_list;
    Map* symbol_map;
    Map* macro_map;
    Map* builtin_type_map;
    Stack* timer_stack;
    List* timer_list;

    string output_name;
    string source_file;
    char* previous_file;
    string current_directory;
    List* file_list;
    Intern_Array* intern_array;

} Thi_Context;

Thi_Context make_thi_context()
{
    Thi_Context t;

    t.detailed_print = false;
    t.debug_mode = false;
    t.enable_constant_folding = true;
    t.optimize = true;
    t.foreign_function_list = make_list();
    t.constant_string_list = make_list();
    t.link_list = make_list();
    t.file_list = make_list();
    t.timer_list = make_list();
    t.symbol_map = make_map();
    t.macro_map = make_map();
    t.builtin_type_map = make_map();
    t.timer_stack = make_stack();
    t.output_name = make_string("");
    t.previous_file = NULL;
    t.source_file = make_string("");
    t.current_directory = make_string("");
    t.intern_array = make_intern_array();

    return t;
}










int main(int argc, char** argv)
{
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    ctx_tests();

    initilize_globals();
    init_interns_list();

    // Thi_Context thi_ctx = make_thi_context();

    add_link("System");

    s32 opt;
    while ((opt = getopt(argc, argv, "f:dvo:")) != -1) {
        switch (opt) {
        case 'v': detailed_print = true; break;
        case 'd': debug_mode = true; break;
        case 'f':
            set_source_file(optarg);
            info("filename: %s\n", optarg);
            break;
        case 'o':
            set_output_name(optarg);
            info("exec_name: %s\n", optarg);
            break;
        case ':': info("option needs a value\n"); break;
        case '?': info("unknown option: %c\n", optopt); break;
        }
    }

    if (debug_mode) {
        detailed_print = true;
        optimize = false;
        enable_constant_folding = false;
    }

    push_timer("Total time");

    // Grab the source file
    char* source_file = get_source_file();
    char* exec_name = get_output_name();
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
    List* ast = make_list();
    parse(ast, source_file);

    // Constant folding
    if (enable_constant_folding) {
        push_timer("Constant folding");
        LIST_FOREACH(ast)
        {
            Expr* expr = (Expr*)it->data;
            expr = constant_fold_expr(expr);
        }
        pop_timer();
        print_ast(ast);
    }

    // Codegen
    push_timer("Codegen");
    char* output = generate_code_from_ast(ast);
    pop_timer();

    // Write to file
    if (output) {
        char* output_filename = "output.asm";
        write_to_file(output_filename, output);
        assemble(output_filename, exec_name);
        linking_stage(exec_name);
    } else {
        error("generating code from ast failed.");
    }

    // Debug info. Writing out sizes of our types.
    info("size of Token: %lu bytes", sizeof(Token));
    info("size of Expr: %lu bytes", sizeof(Expr));
    info("size of Typespec: %lu bytes", sizeof(Typespec));
    info("size of Value: %lu bytes", sizeof(Value));

    pop_timer();

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    List* timers = get_timers();
    info("--- Compiler timings ---");
    LIST_FOREACH(timers)
    {
        Timer* tm = (Timer*)it->data;
        s64 len = strlen(tm->desc);
        char* ms = strf("%f seconds", tm->ms / 1e3);
        s64 ms_l = strlen(ms);
        s64 padding = w.ws_col - len - ms_l - 1; // -1 is the ':'
        info("%s:%*s%s", tm->desc, padding, "", ms);
    }
    info("---------------------------");

    return 0;
}

void assemble(char* asm_file, char* exec_name)
{
    string comp_call = make_string_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer("Assembler");
    system(comp_call.c_str);
    free_string(&comp_call);
    pop_timer();
}

void linking_stage(char* exec_name)
{
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main", exec_name, exec_name);
    List* links = get_link_list();
    LIST_FOREACH(links)
    {
        char* l = (char*)it->data;
        link_call = strf("%s -l%s", link_call, l);
    }
    info("Linking with options '%s'", link_call);
    push_timer("Linker");
    system(link_call);
    pop_timer();

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
}
 
