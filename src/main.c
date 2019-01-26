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
AST* constant_fold_expr(Thi_Context* tctx, AST* expr);

void  pass_constant_fold(Thi_Context* tctx, List* ast);
void  pass_resolve_all_unresolved_types(Thi_Context* tctx, List* ast);
void  pass_find_and_add_all_definitions_to_symbol_table(Thi_Context* tctx, List* ast);
List* parse(Thi_Context* tctx, char* source_file);

void assemble(char* asm_file, char* exec_name);
void linking_stage(char* exec_name);

int main(int argc, char** argv) {
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();

    initilize_globals();

    Thi_Context tctx;
    tctx.detailed_print          = false;
    tctx.debug_mode              = false;
    tctx.enable_constant_folding = true;
    tctx.optimize                = true;
    tctx.extern_list             = make_list();
    tctx.link_list               = make_list();
    tctx.file_list               = make_list();
    tctx.timer_list              = make_list();
    tctx.symbol_map              = make_map();
    tctx.macro_map               = make_map();
    tctx.timer_stack             = make_stack();
    tctx.output_name             = make_string("");
    tctx.previous_file           = NULL;
    tctx.source_file             = make_string("");
    tctx.current_directory       = make_string("");

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

    list_append(file_list, name);

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

    List* ast = make_list();

    // Parse
    LIST_FOREACH(file_list) {
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

    pass_find_and_add_all_definitions_to_symbol_table(tctx, ast);
    pass_resolve_all_unresolved_types(tctx, ast);
    // pass_constant_fold(tctx, ast); // BUGGY

    // print_symbol_map();
    // print_tokens(tokens);
    // print_ast(ast);

    pop_timer();

    set_source_file(last_file);
    set_current_dir(last_dir);

    return ast;
}

void pass_resolve_all_unresolved_types(Thi_Context* tctx, List* ast) {
    info("pass_resolve_all_unresolved_types");
    push_timer("pass_resolve_all_unresolved_types");
    LIST_FOREACH(type_list) {
        Type* t = (Type*)it->data;
        *t = *get_symbol(get_type_name(t));
    }
    pop_timer();
}

void pass_find_and_add_all_definitions_to_symbol_table(Thi_Context* tctx, List* ast) {
    info("pass_find_and_add_all_definitions_to_symbol_table");
    push_timer("pass_find_and_add_all_definitions_to_symbol_table");
    LIST_FOREACH(ast) {
        AST* expr_1 = (AST*)it->data;
        switch (expr_1->kind) {
            case AST_FUNCTION: {
                char* func_name = expr_1->Function.type->Function.name;
                add_symbol(func_name, expr_1->Function.type);
            } break;
            case AST_ENUM: {
                char* enum_name = expr_1->Enum.type->Enum.name;
                add_symbol(enum_name, expr_1->Enum.type);
            } break;
            case AST_STRUCT: {
                char* struct_name = expr_1->Struct.type->Struct.name;
                add_symbol(struct_name, expr_1->Struct.type);
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
        }
    }
    pop_timer();
}

void  pass_constant_fold(Thi_Context* tctx, List* ast) {
    push_timer("Constant Folding");
    LIST_FOREACH(ast) { it->data = constant_fold_expr(tctx, it->data); }
    pop_timer();
}

AST* constant_fold_expr(Thi_Context* tctx, AST* expr) {
    assert(expr);

    info("constant_fold_expr %s: %s", ast_kind_to_str(expr->kind), ast_to_str(expr));

    switch (expr->kind) {
        case AST_SIZEOF: return ast_replace(expr, make_ast_int(get_size_of_type(expr->Sizeof.type)));
        case AST_EXTERN: return expr;
        case AST_LOAD: return expr;
        case AST_LINK: return expr;
        case AST_CONTINUE: return expr;
        case AST_BREAK: return expr;
        case AST_INT: return expr;
        case AST_STRING: return expr;
        case AST_FLOAT: return expr;
        case AST_NOTE: return expr;
        case AST_DEFER: {
            expr->Defer.expr = constant_fold_expr(tctx, expr->Defer.expr);
        } break;
        case AST_RETURN: {
            if (expr->Return.expr) {
                expr->Return.expr = constant_fold_expr(tctx, expr->Return.expr);
            }
        } break;
        case AST_IDENT: {
            AST* macro_expr = get_macro_def(expr->Ident.name);
            if (macro_expr) {
                expr = constant_fold_expr(tctx, macro_expr);
            }
        } break;
        case AST_CALL: {
            LIST_FOREACH(expr->Call.args) {
                AST* arg = (AST*)it->data;
                it->data = constant_fold_expr(tctx, arg);
            }
        } break;
        case AST_UNARY: {
            AST* oper = expr->Unary.operand;
            oper      = constant_fold_expr(tctx, oper);
            if (oper->kind == AST_INT) {
                Token_Kind op     = expr->Unary.op;
                s64        oper_v = oper->Int.val;
                s64        value  = 0;
                switch (op) {
                    case TOKEN_BANG: value = !oper_v; break;
                    case TOKEN_PLUS: value = oper_v; break;
                    case TOKEN_TILDE: value = ~oper_v; break;
                    case TOKEN_MINUS: value = -oper_v; break;
                    default: error("constant_fold_expr unary %s not implemented", token_kind_to_str(op));
                }
                info("folded %s into %lld", ast_to_str(expr), value);
                expr = make_ast_int(value);
            }
        } break;
        case AST_BINARY: {
            Token_Kind op  = expr->Binary.op;
            AST*       lhs = expr->Binary.lhs;
            AST*       rhs = expr->Binary.rhs;
            lhs            = constant_fold_expr(tctx, lhs);
            rhs            = constant_fold_expr(tctx, rhs);
            if (op == TOKEN_EQ) expr = make_ast_binary(TOKEN_EQ, lhs, rhs);
            if (lhs->kind == AST_INT && rhs->kind == AST_INT) {
                s64 lhs_v = lhs->Int.val;
                s64 rhs_v = rhs->Int.val;
                s64 value = 0;
                switch (op) {
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
                    default: error("constant_fold_expr binary %s not implemented", token_kind_to_str(op));
                }
                info("folded %s into %lld", ast_to_str(expr), value);
                expr = make_ast_int(value);
            }
        } break;
        case AST_VARIABLE_DECL: {
            if (expr->Variable_Decl.value)
                expr->Variable_Decl.value = constant_fold_expr(tctx, expr->Variable_Decl.value);
        } break;
        case AST_FUNCTION: {
            return constant_fold_expr(tctx, expr->Function.body);
        } break;
        case AST_STRUCT: {
            LIST_FOREACH(expr->Struct.type->Struct.members) {
                AST* mem = (AST*)it->data;
                mem = constant_fold_expr(tctx, mem);
            }
        } break;
        case AST_ENUM: {
            LIST_FOREACH(expr->Enum.type->Enum.members) {
                AST* mem = (AST*)it->data;
                mem = constant_fold_expr(tctx, mem);
            }
        } break;

        case AST_BLOCK: {
            LIST_FOREACH(expr->Block.stmts) {
                AST* stmt = (AST*)it->data;
                it->data  = constant_fold_expr(tctx, stmt);
            }
        } break;
        case AST_GROUPING: {
            expr->Grouping.expr = constant_fold_expr(tctx, expr->Grouping.expr);
        } break;
        case AST_CAST: {
            expr = constant_fold_expr(tctx, expr->Cast.expr);
        } break;
        case AST_SUBSCRIPT: {
            expr->Subscript.load = constant_fold_expr(tctx, expr->Subscript.load);
            expr->Subscript.sub  = constant_fold_expr(tctx, expr->Subscript.sub);
        } break;
        case AST_IF: {
            expr->If.cond       = constant_fold_expr(tctx, expr->If.cond);
            expr->If.then_block = constant_fold_expr(tctx, expr->If.then_block);
        } break;
        case AST_FOR: {
            expr->For.init       = constant_fold_expr(tctx, expr->For.init);
            expr->For.cond       = constant_fold_expr(tctx, expr->For.cond);
            expr->For.step       = constant_fold_expr(tctx, expr->For.step);
            expr->For.then_block = constant_fold_expr(tctx, expr->For.then_block);
        } break;
        case AST_WHILE: {
            expr->While.cond       = constant_fold_expr(tctx, expr->While.cond);
            expr->While.then_block = constant_fold_expr(tctx, expr->While.then_block);
        } break;
        default: error("constant_fold_expr %s not implemented", ast_kind_to_str(expr->kind));
    }
    return expr;
}
