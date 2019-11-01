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
#include "set.h"          // Set, set_tests
#include "ast.h"          // AST, AST_Kind
#include "ast_to_c.h"     // ast_to_c
#include "codegen.h"      // generate_code_from_ast
#include "lex.h"          // generate_tokens_from_source, print_tokens
#include "list.h"         // list_tests
#include "map.h"          // map
#include "parse.h"        // parse2
#include "parser.h"       // generate_ast_from_tokens
#include "sema.h"         // semantic_analysis
#include "stack.h"        // stack_tests
#include "string.h"       // strcmp
#include "type.h"         // Type
#include "common.h"       // u8, u16, s32, etc.
#include "utility.h"      // get_file_content, success, info, get_time
#include "value.h"        // Value
#include <stdio.h>        // sprintf
#include <stdlib.h>       // free
#include <string.h>       // strcmp
#include <sys/ioctl.h>    // @Note: what do i use this for?
#include <unistd.h>       // @Note: what do i use this

#ifdef __unix__
#include <getopt.h>
#endif

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

void assemble(char* asm_file, char* exec_name);

void linking_stage(List* links, char* exec_name);

//------------------------------------------------------------------------------
//                               Passes
//------------------------------------------------------------------------------

void* pass_add_all_symbols(void* symbols, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_ENUM:   // fallthrough
    case AST_STRUCT: // fallthrough
    case AST_FUNCTION: // fallthrough
    case AST_EXTERN: map_set(symbols, get_type_name(node->type), node->type); break;
    }
    return NULL;
}
void* pass_resolve_all_symbols(void* symbols, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_ENUM:   // fallthrough
    case AST_STRUCT: // fallthrough
    case AST_FUNCTION: node->type = map_get(symbols, get_type_name(node->type)); break;
    }
    return NULL;
}

void* check_for_unresolved_types(void* ctx, AST* node) {
    if (node->type && node->type->kind == TYPE_UNRESOLVED) {
        error("[check_for_unresolved_types]: unresolved type found for node: %s", ast_to_str(node));
    }
    return NULL;
}


void* visitor_resolve_unresolved_types(void* symbols, AST* node) {
    if (!node->type) return NULL;
    Type* placeholder_t = node->type;
    while (placeholder_t->kind == TYPE_POINTER) {
        placeholder_t = placeholder_t->Pointer.pointee;
    }
    if (placeholder_t->kind == TYPE_UNRESOLVED) {
        *placeholder_t = *((Type*)map_get(symbols, get_type_name(placeholder_t)));
    }
    return NULL;
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

bool expr_is_literal_value(AST* expr, f64 val) {
    if ((expr->kind == AST_INT && expr->Int.val == (s64)val) || (expr->kind == AST_FLOAT && expr->Float.val == val)) {
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
            error("[%d:%d] divide by 0", node->loc_info.line, node->loc_info.col);
        }
    }

    if (lhs->kind == AST_INT && rhs->kind == AST_INT) {
        s64 lhs_v = lhs->Int.val;
        s64 rhs_v = rhs->Int.val;
        s64 value = 0;

        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_EQ_EQ:               value = (lhs_v == rhs_v);  break;
        case TOKEN_BANG_EQ:             value = (lhs_v != rhs_v);  break;
        case TOKEN_PLUS:                value = (lhs_v +  rhs_v);  break;
        case TOKEN_MINUS:               value = (lhs_v -  rhs_v);  break;
        case TOKEN_ASTERISK:            value = (lhs_v *  rhs_v);  break;
        case TOKEN_FWSLASH:             value = (lhs_v /  rhs_v);  break;
        case TOKEN_AND:                 value = (lhs_v &  rhs_v);  break;
        case TOKEN_PIPE:                value = (lhs_v |  rhs_v);  break;
        case TOKEN_LT:                  value = (lhs_v <  rhs_v);  break;
        case TOKEN_GT:                  value = (lhs_v >  rhs_v);  break;
        case TOKEN_GT_GT:               value = (lhs_v >> rhs_v);  break;
        case TOKEN_LT_LT:               value = (lhs_v << rhs_v);  break;
        case TOKEN_PERCENT:             value = (lhs_v %  rhs_v);  break;
        case TOKEN_HAT:                 value = (lhs_v ^  rhs_v);  break;
        case TOKEN_AND_AND:             value = (lhs_v && rhs_v);  break;
        case TOKEN_PIPE_PIPE:           value = (lhs_v || rhs_v);  break;
        case TOKEN_QUESTION_MARK:       return;
        case TOKEN_COLON:               return;
        }

        lhs->Int.val = value;
        ast_replace(node, lhs);
    } else if (lhs->kind == AST_FLOAT && rhs->kind == AST_FLOAT) {
        f64 lhs_v = lhs->Float.val;
        f64 rhs_v = rhs->Float.val;
        f64 value = 0.0;

        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_EQ_EQ:              value = (lhs_v == rhs_v);  break; 
        case TOKEN_BANG_EQ:            value = (lhs_v != rhs_v);  break; 
        case TOKEN_PLUS:               value = (lhs_v +  rhs_v);  break; 
        case TOKEN_MINUS:              value = (lhs_v -  rhs_v);  break; 
        case TOKEN_ASTERISK:           value = (lhs_v *  rhs_v);  break; 
        case TOKEN_FWSLASH:            value = (lhs_v /  rhs_v);  break; 
        case TOKEN_LT:                 value = (lhs_v <  rhs_v);  break; 
        case TOKEN_GT:                 value = (lhs_v >  rhs_v);  break; 
        case TOKEN_AND_AND:            value = (lhs_v && rhs_v);  break; 
        case TOKEN_PIPE_PIPE:          value = (lhs_v || rhs_v);  break; 
        }

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
void* constant_fold(void* dont_care, AST* node) {
    switch (node->kind) {
    default: break;
    case AST_BINARY: constant_fold_binary(node); break;
    case AST_UNARY: constant_fold_unary(node); break;
    }
    return NULL;
}

typedef struct {
    AST_Kind kind;
    void* list;
} AST_FindAll_Query;

void* ast_query(void* query, AST* node) {
    AST_FindAll_Query* q = query;
    if (node->kind == q->kind) {
        list_append(q->list, node);
    }
    return NULL;
}

List* ast_find_all_of_kind(AST_Kind kind, AST* ast) {
    List* list = make_list();
    AST_FindAll_Query query = {kind, list};
    ast_visit(ast_query, &query, ast);
    return list;
}

void run_pass(AST* ast, char* pass_description, ast_callback visitor_func, void* visitor_func_arg) {
    push_timer(pass_description);
    ast_visit(visitor_func, visitor_func_arg, ast);
    pop_timer();
}

bool timer_comparison_func(void* a, void* b) {
    return ((Timer*)a)->ns > ((Timer*)b)->ns;
}

int main(int argc, char** argv) {
    // @Todo(marcus) do more robust argumemnt handling
    // Argument validation

    // @Todo: cleanup main. Needs better overview of 
    // every step.

    if (argc < 2) error("too few arguments.");

    initilize_lex();
    initilize_timers();
    push_timer("Total time");

    // error("%s", parse2(get_file_content(argv[1])));

    // s32 opt;
    // while ((opt = getopt(argc, argv, "hv")) != -1) {
    //     switch (opt) {
    //     case 'h': {
    //         puts("Usage:");
    //         puts("      thi <file>");
    //         puts("      thi -v");
    //         return 0;
    //     } break;
    //     case 'v': puts(COMPILER_VERSION); return 0;
    //     case ':': debug("option needs a value\n"); return 0;
    //     case '?': debug("unknown option: %c\n", opt); return 0;
    //     }
    // }

    debug("Compiler was last compiled: "__TIME__);

#ifndef NDEBUG
    set_tests();
    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    lexer_test();
#endif

    push_timer("Frontend");

    // Grab the source file
    char* source_file = argv[optind];

    char* ext = get_file_extension(source_file);
    char* dir = get_file_directory(source_file);
    char* name = get_file_name(source_file);
    char* exec_name = remove_file_extension(name);

    debug(source_file);
    debug("ext: %s", ext);
    debug("dir: %s", dir);
    debug("name: %s", name);
    debug("exec_name: %s", exec_name);

    debug(ucolor(str_replace_center(" parsing ", pad_out_full_width('_'))));
    Parsed_File pf = parse(source_file);
    
    AST* ast = pf.ast;
    Map* symbols = pf.symbols;
    s64 line_count = pf.lines;
    s64 comment_count = pf.comments;

    List* links = ast_find_all_of_kind(AST_LINK, ast);
    // @Audit
    #ifdef __APPLE__
    list_append(links, make_ast_link(ast->loc_info, "-lSystem"));
    #endif

    debug("%s", ast_to_str(ast));

    run_pass(ast, "pass_add_all_symbols", pass_add_all_symbols, symbols);
    run_pass(ast, "resolve_unresolved_types", visitor_resolve_unresolved_types, symbols);

    // Semantic Analysis
    debug(ucolor(str_replace_center(" sema ", pad_out_full_width('_'))));
    debug("");
    sema(ast);

    //  Optimization passes
#if OPTIMIZATION_CONSTANT_FOLD
    run_pass(ast, "constant_fold", constant_fold, NULL);
#endif

    // Sanity check
    run_pass(ast, "check_for_unresolved_types", check_for_unresolved_types, NULL);


    // Codegen
    debug(ucolor(str_replace_center(" codegen ", pad_out_full_width('_'))));
    char* code = to_x64(ast);
    debug("");
    // debug("%s", code);
    
#ifndef NDEBUG
    write_to_file("output.thi", ast_to_src(ast));
    write_to_file("output.s", code);
    // write_to_file("output.c", ast_to_c(ast));
#endif

    Timer* tm_frontend = pop_timer();
    push_timer("Backend");

    // Write to file
    if (code) {
        char* name_with_ext_removed = remove_file_extension(name);
        write_to_file(strf("%s.s", name_with_ext_removed), code);
        
        assemble(name_with_ext_removed, exec_name);
        linking_stage(links, exec_name);
        
        // Cleanup assembly files
        // system(strf("rm %s.s", name_with_ext_removed));
    } else error("generating code from ast failed.");

    Timer* tm_backend = pop_timer();

    // Debug info. Writing out sizes of our types.
    // table_entry align the 1st argument to the left and the 2nd to the right.
    debug(ucolor(table_entry("size of Token",   size_with_suffix(sizeof(Token)))));
    debug(ucolor(table_entry("size of AST",     size_with_suffix(sizeof(AST)))));
    debug(ucolor(table_entry("size of Type",    size_with_suffix(sizeof(Type)))));
    debug(ucolor(table_entry("size of Value",   size_with_suffix(sizeof(Value)))));
    debug(ucolor(table_entry("size of Map",     size_with_suffix(sizeof(Map)))));
    debug(ucolor(table_entry("size of List",    size_with_suffix(sizeof(List)))));
    debug(ucolor(table_entry("size of Stack",   size_with_suffix(sizeof(Stack)))));
    debug(ucolor(table_entry("size of string",  size_with_suffix(sizeof(string)))));
    debug(ucolor(table_entry("size of code",    size_with_suffix(strlen(code) / sizeof(code)))));
    debug(ucolor(table_entry("size of symbols", size_with_suffix(symbols->count * sizeof(*symbols->elements)))));

    Timer* tm_total = pop_timer();

    bool show_timings = false;
    if (show_timings)
    {
        // @FeltCuteMightDeleteLater
        system("clear");

        success(str_replace_center(" Thi ", pad_out_full_width('_')));
        success(align_center(strf("lines %lld comments %lld", line_count, comment_count)));

        // Sorting the list also disables any tree like representation atm.
    #if TIMERS_SORT
        list_sort(timer_list, timer_comparison_func);
    #endif
        success(align_center(strf("Total    %llu lines/sec", (u64)(((1.0 / (f64)tm_total->ns*1e9)) * (line_count+comment_count)))));
        success(align_center(strf("Backend  %llu lines/sec", (u64)(((1.0 / (f64)tm_backend->ns*1e9)) * (line_count+comment_count)))));
        success(align_center(strf("Frontend %llu lines/sec", (u64)(((1.0 / (f64)tm_frontend->ns*1e9)) * (line_count+comment_count)))));
        list_foreach_reverse(timer_list) {
            Timer* tm = it->data;
            char* sec = strf("(%.2f%%) %s", (((f64)tm->ns / tm_total->ns)*1e2), SHOW_TIMERS_WITH_SUFFIX ? time_with_suffix(tm->ns) : strf("%f"DEFAULT_SECONDS_SUFFIX, tm->ns/1e9));
            success("%s", ucolor(table_entry(strf("%s", tm->desc), sec)));
        }
        success(pad_out_full_width('_'));
    }

    return 0;
}

void assemble(char* asm_file, char* exec_name) {
#ifdef __APPLE__
    string* comp_call = make_string_f("nasm -f macho64 %s.s -o %s.o", asm_file, exec_name);
#elif __unix__
    string* comp_call = make_string_f("nasm -f elf64 %s.s -o %s.o", asm_file, exec_name);
#endif

    debug("Assembling with options '%s'", ucolor(string_data(comp_call)));
    push_timer("assembler");
    system(string_data(comp_call));
    pop_timer();
}

void linking_stage(List* links, char* exec_name) {
#ifdef __APPLE__
    string* link_call = make_string_f("ld -macosx_version_min 10.15 -no_pie -o %s %s.o -e _main", exec_name, exec_name);
#elif __unix__
    string* link_call = make_string_f("ld -o %s %s.o -e main", exec_name, exec_name);
#endif
    list_foreach(links) {
        AST* link = it->data;
        string_append_f(link_call, " %s", link->Link.str);
    }
    debug("Linking with options '%s'", ucolor(string_data(link_call)));

    push_timer("linker");
    system(string_data(link_call));
    pop_timer();

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
    system(strf("rm %s.s", exec_name));
}


