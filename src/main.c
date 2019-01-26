#include <assert.h>   // assert
#include <stdio.h>    // sprintf
#include <stdlib.h>   // free
#include <string.h>   // strcmp
#include "ast.h"      // AST
#include "codegen.h"  // generate_code_from_ast
#include "constants.h"
#include "globals.h"  // init_maps
#include "lexer.h"    // generate_tokens_from_source, print_tokens
#include "list.h"     // list_tests
#include "map.h"      // map
#include "parser.h"   // generate_ast_from_tokens
#include "stack.h"    // stack_tests
#include "string.h"   // strcmp
#include "type.h"     // Type
#include "typedefs.h"
#include "utility.h"  // get_file_content, success, info, get_time
#include "value.h"    // Value

#include <sys/ioctl.h>
#include <unistd.h>

typedef struct {
    bool detailed_print;
    bool debug_mode;
    bool enable_constant_folding;
    bool optimize;

    List*  extern_list;
    List*  link_list;
    Map*   symbol_map;
    Map*   macro_map;
    Stack* timer_stack;
    List*  timer_list;

    string output_name;
    string source_file;
    char*  previous_file;
    string current_directory;
    List*  file_list;

} Thi_Context;

//------------------------------------------------------------------------------
//                               Main Driver
//------------------------------------------------------------------------------
void  ast_type_resolution_pass(Thi_Context* tctx, List* ast);
void  ast_definitions_pass(Thi_Context* tctx, List* ast);
Type* get_inferred_type_of_expr(Thi_Context* tctx, AST* expr);

List* parse(Thi_Context* tctx, char* source_file);

void assemble(char* asm_file, char* exec_name);
void linking_stage(char* exec_name);

/*
    Figure out all the files that are loaded by the source.
*/
Thi_Context make_thi_context() {
    Thi_Context t;

    t.detailed_print          = false;
    t.debug_mode              = false;
    t.enable_constant_folding = true;
    t.optimize                = true;
    t.extern_list             = make_list();
    t.link_list               = make_list();
    t.file_list               = make_list();
    t.timer_list              = make_list();
    t.symbol_map              = make_map();
    t.macro_map               = make_map();
    t.timer_stack             = make_stack();
    t.output_name             = make_string("");
    t.previous_file           = NULL;
    t.source_file             = make_string("");
    t.current_directory       = make_string("");

    return t;
}

int main(int argc, char** argv) {
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();

    initilize_globals();

    Thi_Context tctx = make_thi_context();

    add_link("-lSystem");

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
        detailed_print          = true;
        optimize                = false;
        enable_constant_folding = false;
    }

    push_timer("Total time");

    // Grab the source file
    char* source_file = get_source_file();
    char* exec_name   = get_output_name();
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

    add_symbol("void", u8_t);
    add_symbol("bool", u8_t);
    add_symbol("char", u8_t);

    add_symbol("s8", s8_t);
    add_symbol("s16", s16_t);
    add_symbol("s32", s32_t);
    add_symbol("s64", s64_t);

    add_symbol("u8", u8_t);
    add_symbol("u16", u16_t);
    add_symbol("u32", u32_t);
    add_symbol("u64", u64_t);

    add_symbol("f32", f32_t);
    add_symbol("f64", f64_t);

    char* ext  = get_file_extension(source_file);
    char* dir  = get_file_directory(source_file);
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
    List* ast = parse(&tctx, source_file);

    LIST_FOREACH(file_list) {
        warning("file %s", it->data);
        char* file  = strf("%s%s", get_current_dir(), it->data);
        List* ast_l = parse(&tctx, file);
        list_append_content_of(ast, ast_l);
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
    info("size of AST:   %lu bytes", sizeof(AST));
    info("size of Type:  %lu bytes", sizeof(Type));
    info("size of Value: %lu bytes", sizeof(Value));

    pop_timer();

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    List* timers = get_timers();
    info("--- Compiler timings ---");
    LIST_FOREACH(timers) {
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

void assemble(char* asm_file, char* exec_name) {
    string comp_call = make_string_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer("Assembler");
    system(comp_call.c_str);
    free_string(&comp_call);
    pop_timer();
}

void linking_stage(char* exec_name) {
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main", exec_name, exec_name);
    List* links     = get_link_list();
    LIST_FOREACH(links) {
        char* l   = (char*)it->data;
        link_call = strf("%s %s", link_call, l);
    }
    info("Linking with options '%s'", link_call);
    push_timer("Linker");
    system(link_call);
    pop_timer();

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
}

List* parse(Thi_Context* tctx, char* source_file) {
    char* last_file = get_source_file();
    char* last_dir  = get_current_dir();
    char* dir       = get_file_directory(source_file);

    set_source_file(source_file);
    set_current_dir(dir);

    push_timer(source_file);

    char*       source = get_file_content(source_file);
    Token_Array tokens = generate_tokens_from_source(source);

    List* ast = generate_ast_from_tokens(tokens);

    push_timer("Finding all definitions");
    ast_definitions_pass(tctx, ast);
    pop_timer();
    push_timer("Type resolution");
    ast_type_resolution_pass(tctx, ast);
    pop_timer();

    // Constant folding
    if (enable_constant_folding) {
        push_timer("Constant Folding");
        LIST_FOREACH(ast) {
            AST* expr = (AST*)it->data;
            expr      = constant_fold_expr(expr);
        }
        pop_timer();
    }

    // print_tokens(tokens);
    // print_symbol_map();
    // print_ast(ast);

    pop_timer();

    set_source_file(last_file);
    set_current_dir(last_dir);

    return ast;
}

Type* get_inferred_type_of_expr(Thi_Context* tctx, AST* expr) {
    switch (expr->kind) {
        case AST_EXTERN:
        case AST_LOAD:
        case AST_LINK:
        case AST_BLOCK:
        case AST_BREAK:
        case AST_CONTINUE:
        case AST_DEFER: return NULL;
        case AST_RETURN:
            if (expr->Return.expr) return get_inferred_type_of_expr(tctx, expr->Return.expr);

        case AST_CAST: return expr->Cast.type;
        case AST_NOTE: return get_inferred_type_of_expr(tctx, expr->Note.expr);
        case AST_INT: return make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
        case AST_FLOAT: return make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
        case AST_STRING: return make_type_pointer(make_type_int(8, 1));
        case AST_IDENT: return get_symbol(expr->Ident.name);
        case AST_CALL: return get_symbol(expr->Call.callee)->Function.ret_type;
        case AST_UNARY: return get_inferred_type_of_expr(tctx, expr->Unary.operand);
        case AST_BINARY: return get_inferred_type_of_expr(tctx, expr->Binary.rhs);
        case AST_VARIABLE_DECL: return expr->Variable_Decl.type;
        case AST_FUNCTION: return expr->Function.type->Function.ret_type;
        case AST_STRUCT: {
                List* members = expr->Struct.type->Struct.members;
                LIST_FOREACH(members) {
                    AST* mem = (AST*)it->data;
                    // if (mem->kind == Variable_Decl) {
                        // mem->Variable_Decl->type = get_inferred_type_of_expr(mem);
                    // }
                }

        } break;


        return get_symbol(expr->Struct.type->Struct.name);
        case AST_ENUM: return get_symbol(expr->Struct.type->Struct.name);
        case AST_GROUPING: return get_inferred_type_of_expr(tctx, expr->Grouping.expr);
        case AST_SUBSCRIPT: return get_inferred_type_of_expr(tctx, expr->Subscript.load);
        default: error("%s has no type", ast_kind_to_str(expr->kind));
    }
    return NULL;
}

void ast_type_resolution_pass(Thi_Context* tctx, List* ast) {
    info("Type resolution..");
    LIST_FOREACH(ast) {
        AST*  expr_1 = (AST*)it->data;
        Type* type   = get_inferred_type_of_expr(tctx, expr_1);

        if (!type) continue;


        warning("before %s", ast_to_str(expr_1));
        warning("%s", type_to_str(type));
        if (type->kind == TYPE_PLACEHOLDER) {
            type = get_symbol(type->Placeholder.name);
        }

        if (expr_1->kind == AST_VARIABLE_DECL) {
            expr_1->Variable_Decl.type = type;
        }
        warning("after %s", ast_to_str(expr_1));

        expr_1->type = type;
    }
}
void ast_definitions_pass(Thi_Context* tctx, List* ast) {
    info("Finding all definition..");
    LIST_FOREACH(ast) {
        AST* expr_1 = (AST*)it->data;

        switch (expr_1->kind) {
            case AST_ENUM: {
                char* enum_name = expr_1->Enum.type->Enum.name;
                add_symbol(enum_name, expr_1->Enum.type);
                list_remove(ast, it);
            } break;
            case AST_STRUCT: {
                char* struct_name = expr_1->Struct.type->Struct.name;
                add_symbol(struct_name, expr_1->Struct.type);
                list_remove(ast, it);
            } break;
            case AST_EXTERN: {
                char* func_name = expr_1->Extern.node->Function.type->Function.name;
                add_symbol(func_name, expr_1->Extern.node->Function.type);
            } break;
            case AST_LOAD: {
                char* file_name = expr_1->Load.node->String.val;
                list_append(file_list, file_name);
                list_remove(ast, it);
            } break;

            case AST_LINK: {
                char* link_name = expr_1->Link.node->String.val;
                add_link(link_name);
                list_remove(ast, it);
            } break;

            case AST_CALL: {
                if (it->next) {
                    AST* expr_2 = (AST*)it->next->data;
                    if (expr_2->kind == AST_BLOCK) {
                        char* func_name = expr_1->Call.callee;
                        List* args      = expr_1->Call.args;
                        AST*  body      = expr_2;

                        Type* type = make_type_function(func_name, args, NULL);
                        add_symbol(func_name, type);

                        list_append(ast, make_ast_function(type, body));
                        list_remove(ast, it->next);
                        list_remove(ast, it);
                    }

                    if (it->next->next) {
                        AST* expr_3 = (AST*)it->next->next->data;
                        if (expr_1->kind == AST_CALL && expr_2->kind == AST_IDENT && expr_3->kind == AST_BLOCK) {
                            char* func_name     = expr_1->Call.callee;
                            char* ret_type_name = expr_2->Ident.name;

                            Type* ret_type = get_symbol(ret_type_name);
                            AST*  body     = expr_3;
                            List* args     = expr_1->Call.args;

                            Type* type = make_type_function(func_name, args, ret_type);
                            add_symbol(func_name, type);

                            list_append(ast, make_ast_function(type, body));
                            list_remove(ast, it->next->next);
                            list_remove(ast, it->next);
                            list_remove(ast, it);
                        }
                    }
                }
            } break;
        }
    }
}
