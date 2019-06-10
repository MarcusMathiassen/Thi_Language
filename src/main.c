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
#include "lexer.h"        // generate_tokens_from_source, print_tokens
#include "list.h"         // list_tests
#include "map.h"          // map
#include "parser.h"       // generate_ast_from_tokens
#include "sema.h"         // semantic_analysis
#include "stack.h"        // stack_tests
#include "string.h"       // strcmp
#include "type.h"         // Type
#include "common.h"     // u8, u16, s32, etc.
#include "utility.h"      // get_file_content, success, info, get_time
#include "value.h"        // Value
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

void assemble(char* asm_file, char* exec_name);
void linking_stage(List* links, char* exec_name);

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

    error("iogjeroigjeriogj");
    // switch (node->kind) {
    // default: break;
    // case AST_ENUM: {

    //     s64 counter = 0;
    //     AST* e = node;
    //     list_foreach(e->Enum.members) {
    //         AST* m = it->data;
    //         // Turn idents into constant decls
    //         switch (m->kind) {
    //             ERROR_UNHANDLED_AST_KIND(m->kind);
    //         case AST_IDENT:
    //             it->data = make_ast_constant_decl(m->loc_info, m->Ident.name, make_ast_int(m->loc_info, counter, make_type_int(DEFAULT_INT_BYTE_SIZE, 0)));
    //             break;
    //         case AST_CONSTANT_DECL:
    //             xassert(m->Constant_Decl.value->kind == AST_INT);
    //             counter = m->Constant_Decl.value->Int.val;
    //             break;
    //         }
    //         ++counter;
    //     }

    // } break;
    // }
}

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

void* make_sure_all_nodes_have_a_valid_type(void* dont_care, AST* node) {
    xassert(node);
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
    case AST_FALLTHROUGH: return NULL;
    default: break;
    }
    // info("%s: %s -> %s", ast_kind_to_str(node->kind), wrap_with_colored_parens(ast_to_str(node)), give_unique_color(type_to_str(node->type)));

    if (!node->type) {
        error(
            "[make_sure_all_nodes_have_a_valid_type]: missing type for "
            "node: %s",
            ast_to_str(node));
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

bool expr_is_literal_value(AST* expr, float val) {
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
            error("[%d:%d] divide by 0", node->loc_info.line_pos, node->loc_info.col_pos);
        }
    }

    if (lhs->kind == AST_INT && rhs->kind == AST_INT) {
        s64 lhs_v = lhs->Int.val;
        s64 rhs_v = rhs->Int.val;
        s64 value = 0;

        switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_EQ_EQ:               value = (lhs_v == rhs_v);break;
        case TOKEN_BANG_EQ:             value = (lhs_v != rhs_v);break;
        case TOKEN_PLUS:                value = (lhs_v +  rhs_v);break;
        case TOKEN_MINUS:               value = (lhs_v -  rhs_v);break;
        case TOKEN_ASTERISK:            value = (lhs_v *  rhs_v);break;
        case TOKEN_FWSLASH:             value = (lhs_v /  rhs_v);break;
        case TOKEN_AND:                 value = (lhs_v &  rhs_v);break;
        case TOKEN_PIPE:                value = (lhs_v |  rhs_v);break;
        case TOKEN_LT:                  value = (lhs_v <  rhs_v);break;
        case TOKEN_GT:                  value = (lhs_v >  rhs_v);break;
        case TOKEN_GT_GT:               value = (lhs_v >> rhs_v);break;
        case TOKEN_LT_LT:               value = (lhs_v << rhs_v);break;
        case TOKEN_PERCENT:             value = (lhs_v %  rhs_v);break;
        case TOKEN_HAT:                 value = (lhs_v ^  rhs_v);break;
        case TOKEN_AND_AND:             value = (lhs_v && rhs_v);break;
        case TOKEN_PIPE_PIPE:           value = (lhs_v || rhs_v);break;
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
        case TOKEN_EQ_EQ:              value = (lhs_v == rhs_v); break; 
        case TOKEN_BANG_EQ:            value = (lhs_v != rhs_v); break; 
        case TOKEN_PLUS:               value = (lhs_v +  rhs_v); break; 
        case TOKEN_MINUS:              value = (lhs_v -  rhs_v); break; 
        case TOKEN_ASTERISK:           value = (lhs_v *  rhs_v); break; 
        case TOKEN_FWSLASH:            value = (lhs_v /  rhs_v); break; 
        case TOKEN_LT:                 value = (lhs_v <  rhs_v); break; 
        case TOKEN_GT:                 value = (lhs_v >  rhs_v); break; 
        case TOKEN_AND_AND:            value = (lhs_v && rhs_v); break; 
        case TOKEN_PIPE_PIPE:          value = (lhs_v || rhs_v); break; 
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

bool timer_sort_func(void* a, void* b) {
    return ((Timer*)a)->ms > ((Timer*)b)->ms;
}

int main(int argc, char** argv) {
    // @Todo(marcus) do more robust argument handling
    // Argument validation

    if (argc < 2) error("too few arguments.");

    initilize_timers();
    push_timer("Total time");

    info("Compiler was last compiled: "__TIME__);

#ifndef NDEBUG
    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    lexer_test();
#endif

    s32 opt;
    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
        case 'h': {
            puts("Usage:");
            puts("      thi <file>");
            puts("      thi -v");
            return 0;
        } break;
        case 'v': puts(COMPILER_VERSION); return 0;
        case ':': info("option needs a value\n"); return 0;
        case '?': info("unknown option: %c\n", optopt); return 0;
        }
    }

    // Grab the source file
    char* source_file = argv[optind];
    info("Compiling %s", source_file);

    char* ext = get_file_extension(source_file);
    char* dir = get_file_directory(source_file);
    char* name = get_file_name(source_file);
    char* exec_name = remove_file_extension(name);

    info(source_file);
    info("ext: %s", ext);
    info("dir: %s", dir);
    info("name: %s", name);
    info("exec_name: %s", exec_name);

    // Make sure it's actually a .thi file
    if (strcmp(ext, ".thi") != 0) {
        error("thats not a thi file...");
    }

    // Holds all symbols defined in the compilation unit.
    Map* symbols = make_map();
    
    map_set(symbols, "void", make_type_void());
    map_set(symbols, "bool", make_type_int(1, true));
    map_set(symbols, "char", make_type_int(1, true));
    map_set(symbols, "int", make_type_int(DEFAULT_INT_BYTE_SIZE, false));
    map_set(symbols, "float", make_type_float(DEFAULT_FLOAT_BYTE_SIZE));
    map_set(symbols, "double", make_type_float(8));

    map_set(symbols, "s8", make_type_int(1, false));
    map_set(symbols, "s16", make_type_int(2, false));
    map_set(symbols, "s32", make_type_int(4, false));
    map_set(symbols, "s64", make_type_int(8, false));

    map_set(symbols, "u8", make_type_int(1, true));
    map_set(symbols, "u16", make_type_int(2, true));
    map_set(symbols, "u32", make_type_int(4, true));
    map_set(symbols, "u64", make_type_int(8, true));

    map_set(symbols, "f32", make_type_float(4));
    map_set(symbols, "f64", make_type_float(8));

    // Parse
    Parser_Context pctx = make_parser_context();
    pctx.file = source_file;
    pctx.symbols = symbols;
    AST* ast = parse_file(&pctx, source_file);

    List* links = ast_find_all_of_kind(AST_LINK, ast);
    list_append(links, make_ast_link(ast->loc_info, "-lSystem"));

    run_pass(ast, "pass_add_all_symbols", pass_add_all_symbols, symbols);
    run_pass(ast, "resolve_unresolved_types", visitor_resolve_unresolved_types, symbols);

    // Semantic Analysis
    sema(ast);


    // @Todo: move these into the semantic pass
    // passDesc.description = "Resolve sizeofs";
    // passDesc.kind = AST_SIZEOF;
    // passDesc.passKind = PASS_UNSAFE;
    // passDesc.visitor_func = resolve_sizeofs;
    // passDesc.visitor_arg = NULL;
    // thi_install_pass(passDesc);

    // passDesc.description = "Resolve typeofs";
    // passDesc.kind = AST_TYPEOF;
    // passDesc.visitor_func = resolve_typeofs;
    // thi_install_pass(passDesc);

    // passDesc.description = "Resolve subscripts";
    // passDesc.kind = AST_SUBSCRIPT;
    // passDesc.visitor_func = resolve_subscript;
    // thi_install_pass(passDesc);

    // passDesc.description = "Resolve field access";
    // passDesc.kind = AST_FIELD_ACCESS;
    // passDesc.visitor_func = resolve_field_access;
    // thi_install_pass(passDesc);  

    // Sanity check.. Make sure the typer did what it was supposed to do.
    run_pass(ast, "make_sure_all_nodes_have_a_valid_type", make_sure_all_nodes_have_a_valid_type, NULL);

    //  Optimization passes
    run_pass(ast, "constant_fold", constant_fold, NULL);

    // Sanity checks
    run_pass(ast, "check_for_unresolved_types", check_for_unresolved_types, NULL);

    // Codegen
    char* code = to_x64(ast);
    info("%s", code);

    // Write to file
    if (code) {
        char* output_filename = strf("%s", remove_file_extension(name));
        write_to_file(output_filename, code);
        assemble(output_filename, exec_name);
        linking_stage(links, exec_name);
    } else error("generating code from ast failed.");

    // Debug info. Writing out sizes of our types.
    // table_entry align the 1st argument to the left and the 2nd to the right.
    info(ucolor(table_entry("size of Token", size_with_suffix(sizeof(Token)))));
    info(ucolor(table_entry("size of AST", size_with_suffix(sizeof(AST)))));
    info(ucolor(table_entry("size of Type", size_with_suffix(sizeof(Type)))));
    info(ucolor(table_entry("size of Value", size_with_suffix(sizeof(Value)))));
    info(ucolor(table_entry("size of Map", size_with_suffix(sizeof(Map)))));
    info(ucolor(table_entry("size of List", size_with_suffix(sizeof(List)))));
    info(ucolor(table_entry("size of Stack", size_with_suffix(sizeof(Stack)))));
    info(ucolor(table_entry("size of string", size_with_suffix(sizeof(string)))));
    info(ucolor(table_entry("size of code", size_with_suffix(strlen(code) / sizeof(code)))));
    info(ucolor(table_entry("size of symbols", size_with_suffix(symbols->count * sizeof(*symbols->elements)))));

    pop_timer();

    success("--- Compiler timings ---");
    success("lines %s%s comments %s", give_unique_color(strf("%lld", pctx.lines)), RGB_GRAY, give_unique_color(strf("%lld", pctx.comments)));
    #if TIMERS_SORT
    list_sort(timer_list, timer_sort_func);
    #endif
    // Figure out percentage of total time for each timer
    Timer* total_time_timer = timer_list->tail->data;
    f64 total = total_time_timer->ms;
    list_foreach_reverse(timer_list) {
        Timer* tm = it->data;
        char* ms = strf("(%.2f%%) %fs", (tm->ms / total)*1e2, tm->ms / 1e3);
        success("%s", give_unique_color(table_entry(tm->desc, ms)));
    }
    success("---------------------------");

#ifndef NDEBUG
    write_to_file("output.thi", ast_to_source(ast));
#endif

    return 0;
}

void assemble(char* asm_file, char* exec_name) {
#ifdef NDEBUG
    string* comp_call = string_create_f("nasm -f macho64 %s.s -o %s.o", asm_file, exec_name);
#else 
    string* comp_call = string_create_f("nasm -f macho64 -w+all -g %s.s -o %s.o", asm_file, exec_name);
#endif
    info("Assembling with options '%s'", ucolor(string_data(comp_call)));
    push_timer("Assembler");
    system(string_data(comp_call));
    pop_timer();
}

void linking_stage(List* links, char* exec_name) {
    string* link_call = string_create_f("ld -macosx_version_min 10.14 -o %s %s.o -e _main", exec_name, exec_name);
    list_foreach(links) {
        AST* link = it->data;
        string_append_f(link_call, " %s", link->Link.str);
    }
    info("Linking with options '%s'", ucolor(string_data(link_call)));
    push_timer("Linker");
    system(string_data(link_call));
    pop_timer();

    // Cleanup object files
    system(strf("rm %s.o", exec_name));
}
