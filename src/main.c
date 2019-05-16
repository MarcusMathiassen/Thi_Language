#include "ast.h"          // AST, AST_Kind
#include "codegen.h"      // generate_code_from_ast
#include "constants.h"    // all constnats
#include "lexer.h"        // generate_tokens_from_source, print_tokens
#include "list.h"         // list_tests
#include "map.h"          // map
#include "parser.h"       // generate_ast_from_tokens
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

//------------------------------------------------------------------------------
//                               Main Driver
//------------------------------------------------------------------------------

void  add_all_definitions(Thi* thi, Parsed_File* pf);
Type* get_inferred_type_of_expr(Thi* thi, AST* expr);
void  pass_progate_identifiers_to_constants(Thi* thi);
void  pass_type_inference(Thi* thi);
void  pass_initilize_all_enums(Thi* thi);
void  pass_resolve_subscripts(Thi* thi);
void  pass_give_all_identifiers_a_type(Thi* thi);
void  pass_type_checker(Thi* thi);
List* parse(Thi* thi, char* source_file);
void  assemble(Thi* thi, char* asm_file, char* exec_name);
void  linking_stage(Thi* thi, char* exec_name);
void  pass_general(Thi* thi);
void  maybe_convert_call_to_def(Thi* thi, List* ast, List_Node* it);

void check_for_unresolved_types(void* ctx, AST* expr) {
    if (expr->type && expr->type->kind == TYPE_UNRESOLVED) {
        error(
            "[check_for_unresolved_types]: unresolved type found for expr: %s",
            ast_to_str(expr));
    }
}

void make_sure_all_nodes_have_a_valid_type(void* ctx, AST* expr) {
    info("%s: %s -> %s", ast_kind_to_str(expr->kind), wrap_with_colored_parens(ast_to_str(expr)), give_unique_color(type_to_str(expr->type)));
    // clang-format off
    switch (expr->kind) {
    case AST_LOAD:        return;
    case AST_LINK:        return;
    case AST_BLOCK:       return;
    case AST_WHILE:       return;
    case AST_IF:          return;
    case AST_FOR:         return;
    case AST_SWITCH:      return;
    case AST_IS:          return;
    case AST_DEFER:       return;
    case AST_BREAK:       return;
    case AST_CONTINUE:    return;
    case AST_FALLTHROUGH: return;
    default: break;
    }
    // clang-format on
    if (!expr->type) {
        error(
            "[make_sure_all_nodes_have_a_valid_type]: missing type for "
            "expr: %s",
            ast_to_str(expr));
    }
}

void get_all_variables(void* list, AST* expr) {
    if (expr->kind == AST_VARIABLE_DECL) {
        list_append((List*)list, expr);
    }
}

void thi_run_pass(Thi* thi, char* pass_description, void (*visitor_func)(void*, AST*), void* visitor_func_arg) {
    push_timer(thi, pass_description);
    LIST_FOREACH(thi->ast) {
        ast_visit(visitor_func, visitor_func_arg, it->data);
    }
    pop_timer(thi);
}

void visitor_resolve_unresolved_types(void* thi, AST* expr) {
    if (!expr->type) return;
    Type* placeholder_t = expr->type;
    while (placeholder_t->kind == TYPE_POINTER) {
        placeholder_t = placeholder_t->Pointer.pointee;
    }
    if (placeholder_t->kind == TYPE_UNRESOLVED) {
        *placeholder_t = *get_symbol(thi, get_type_name(placeholder_t));
    }
}

void constant_fold(void* ctx, AST* expr) {
    switch (expr->kind) {
    case AST_BINARY: {
        Token_Kind op  = expr->Binary.op;
        AST*       lhs = expr->Binary.lhs;
        AST*       rhs = expr->Binary.rhs;
        if (lhs->kind == AST_GROUPING) lhs = lhs->Grouping.expr;
        if (rhs->kind == AST_GROUPING) rhs = rhs->Grouping.expr;
        if (lhs->kind == AST_INT && rhs->kind == AST_INT) {
            s64 lhs_v = lhs->Int.val;
            s64 rhs_v = rhs->Int.val;
            s64 value = 0;
            // clang-format off
            switch (op) {
            case TOKEN_EQ_EQ:         value = (lhs_v == rhs_v); break;
            case TOKEN_BANG_EQ:       value = (lhs_v != rhs_v); break;
            case TOKEN_PLUS:          value = (lhs_v + rhs_v);  break;
            case TOKEN_MINUS:         value = (lhs_v - rhs_v);  break;
            case TOKEN_ASTERISK:      value = (lhs_v * rhs_v);  break;
            case TOKEN_FWSLASH:       value = (lhs_v / rhs_v);  break;
            case TOKEN_AND:           value = (lhs_v & rhs_v);  break;
            case TOKEN_PIPE:          value = (lhs_v | rhs_v);  break;
            case TOKEN_LT:            value = (lhs_v < rhs_v);  break;
            case TOKEN_GT:            value = (lhs_v > rhs_v);  break;
            case TOKEN_GT_GT:         value = (lhs_v >> rhs_v); break;
            case TOKEN_LT_LT:         value = (lhs_v << rhs_v); break;
            case TOKEN_PERCENT:       value = (lhs_v % rhs_v);  break;
            case TOKEN_HAT:           value = (lhs_v ^ rhs_v);  break;
            case TOKEN_AND_AND:       value = (lhs_v && rhs_v); break;
            case TOKEN_PIPE_PIPE:     value = (lhs_v || rhs_v); break;
            case TOKEN_QUESTION_MARK: return;
            case TOKEN_COLON:         return;
            default: ERROR_UNHANDLED_KIND(token_kind_to_str(op));
            }
            // clang-format on
            info("folded %s into %lld", ast_to_str(expr), value);
            ast_replace(expr, make_ast_int(expr->t, value));
        } else if (lhs->kind == AST_FLOAT && rhs->kind == AST_FLOAT) {
            f64 lhs_v = lhs->Float.val;
            f64 rhs_v = rhs->Float.val;
            f64 value = 0.0;
            // clang-format off
            switch (op) {
            case TOKEN_EQ_EQ:     value = (lhs_v == rhs_v); break;
            case TOKEN_BANG_EQ:   value = (lhs_v != rhs_v); break;
            case TOKEN_PLUS:      value = (lhs_v + rhs_v);  break;
            case TOKEN_MINUS:     value = (lhs_v - rhs_v);  break;
            case TOKEN_ASTERISK:  value = (lhs_v * rhs_v);  break;
            case TOKEN_FWSLASH:   value = (lhs_v / rhs_v);  break;
            case TOKEN_LT:        value = (lhs_v < rhs_v);  break;
            case TOKEN_GT:        value = (lhs_v > rhs_v);  break;
            case TOKEN_AND_AND:   value = (lhs_v && rhs_v); break;
            case TOKEN_PIPE_PIPE: value = (lhs_v || rhs_v); break;
            default: ERROR_UNHANDLED_KIND(token_kind_to_str(op));
            }
            // clang-format on
            info("folded %s into %lld", ast_to_str(expr), value);
            AST* constant_value = make_ast_int(expr->t, value);
            ast_replace(expr, constant_value);
        }
    } break;
    case AST_UNARY: {
        AST* operand = expr->Unary.operand;
        if (operand->kind == AST_GROUPING) operand = operand->Grouping.expr;
        if (operand->kind == AST_INT) {
            Token_Kind op     = expr->Unary.op;
            s64        oper_v = operand->Int.val;
            s64        value  = 0;
            // clang-format off
            switch (op) {
            case TOKEN_BANG:    value = !oper_v; break;
            case TOKEN_PLUS:    value = oper_v;  break;
            case TOKEN_TILDE:   value = ~oper_v; break;
            case TOKEN_MINUS:   value = -oper_v; break;
            default: ERROR_UNHANDLED_KIND(token_kind_to_str(op));
            }
            // clang-format on
            info("folded %s into %lld", ast_to_str(expr), value);
            AST* constant_value = make_ast_int(expr->t, value);
            ast_replace(expr, constant_value);
        }
    } break;
    default: break;
    }
}

typedef struct
{
    AST_Kind kind;
    void*    list;
} AST_FindAll_Query;

void ast_query(void* query, AST* expr) {
    AST_FindAll_Query* q = query;
    if (expr->kind == q->kind) {
        list_append((List*)q->list, expr);
    }
}

List* ast_find_all_of_kind(AST_Kind kind, List* ast) {
    List*             list  = make_list();
    AST_FindAll_Query query = {kind, list};
    LIST_FOREACH(ast) {
        ast_visit(ast_query, &query, it->data);
    }
    return list;
}

int main(int argc, char** argv) {
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();
    lexer_test();

    Thi thi = make_thi();

    s32 opt;
    while ((opt = getopt(argc, argv, "f:dv:")) != -1) {
        switch (opt) {
        case 'v': thi.detailed_print = true; break;
        case 'd': thi.debug_mode = true; break;
        case 'f':
            set_source_file(&thi, optarg);
            info("filename: %s\n", optarg);
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
    info("Compiling %s", source_file);

    char* ext       = get_file_extension(source_file);
    char* dir       = get_file_directory(source_file);
    char* name      = get_file_name(source_file);
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
    if (strcmp(ext, "thi") != 0) {
    }

    List* ast = make_list();

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
    LIST_FOREACH(get_load_list(&thi)) {
        char* file = strf("%s%s", get_current_directory(&thi), it->data);
        warning("Started parsing %s", file);
        List* ast_l = parse(&thi, file);
        list_append_content_of(ast, ast_l);
        warning("finished parsing %s", file);
    }

    thi.ast = ast;

    info("Running passes");

    //
    // PASS: give type-inferred variables a type
    //
    List* var_decls = ast_find_all_of_kind(AST_VARIABLE_DECL, ast);
    LIST_FOREACH(var_decls) {
        AST* var = it->data;
        if (var->Variable_Decl.type == NULL) {
            var->type =
                get_inferred_type_of_expr(&thi, var->Variable_Decl.value);
            var->Variable_Decl.type = var->type;
        }
    }
    info("PASS: give type-inferred variables a type");

    //
    // PASS: resolve all unresolved types
    //

    thi_run_pass(&thi, "resolve_unresolved_types", visitor_resolve_unresolved_types, &thi);
    type_checker(thi.symbol_map, thi.ast);

    //
    //  PASS: resolve all sizeof calls
    //
    List* sizeofs = ast_find_all_of_kind(AST_SIZEOF, ast);
    success("sizeofs: %d", sizeofs->count);
    LIST_FOREACH(sizeofs) {
        AST* expr = it->data;

        // Get the size of the type
        // ..expr->type would give us the size of a 'sizeof' call
        // ..which would be an integer of 8 bytes.
        // ..so we get the size of the underlaying expression instead.
        s64 size = get_size_of_type(expr->Sizeof.expr->type);

        // Transform the expr into a constant value
        AST* constant_value = make_ast_int(expr->t, size);
        ast_replace(expr, constant_value);
    }

    //
    //  PASS: resolve all typeof calls
    //
    List* typeofs = ast_find_all_of_kind(AST_TYPEOF, ast);
    success("typeofs: %d", typeofs->count);
    LIST_FOREACH(typeofs) {
        AST* expr = it->data;

        Type* type = expr->type;

        // Transform the expr into a stringfied version of the type
        AST* string_value = make_ast_string(expr->t, type_to_str(type));
        ast_replace(expr, string_value);
    }

    //
    // Optimization Pass:
    //      Constant propogation.
    //       References to constant variables are replaced by their constant
    //       value.
    //
    List* idents = ast_find_all_of_kind(AST_IDENT, ast);
    List* constant_decls =
        ast_find_all_of_kind(AST_CONSTANT_DECL, ast);
    info("idents %d", idents->count);
    LIST_FOREACH(idents) {
        AST* ident = it->data;
        success("%s", ast_to_str(ident));
        LIST_FOREACH(constant_decls) {
            AST* const_decl = it->data;
            if (strcmp(ident->Ident.name, const_decl->Constant_Decl.name) ==
                0) {
                info("%s turned into %s", ast_to_str(ident), ast_to_str(const_decl->Constant_Decl.value));
                ast_replace(ident, const_decl->Constant_Decl.value);
                ident->type = const_decl->type;
                info("%s after  %s", ast_to_str(ident), ast_to_str(const_decl->Constant_Decl.value));
                break;
            }
        }
    }

    //  Optimization passes
    thi_run_pass(&thi, "constant_fold", constant_fold, NULL);

    // Sanity checks
    thi_run_pass(&thi, "check_for_unresolved_types", check_for_unresolved_types, NULL);
    thi_run_pass(&thi, "make_sure_all_nodes_have_a_valid_type", make_sure_all_nodes_have_a_valid_type, NULL);

    pass_progate_identifiers_to_constants(&thi);
    pass_type_inference(&thi);

    char* json = full_ast_to_json(ast);
    write_to_file("ast.json", json);

    print_ast(ast);

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
        s64    padding = w.ws_col - len - ms_l - 1; // -1 is the ':'
        info("%s:%*s%s", tm->desc, padding, "", ms);
    }
    info("---------------------------");

    return 0;
}

void assemble(Thi* thi, char* asm_file, char* exec_name) {
    string comp_call =
        make_string_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer(thi, "Assembler");
    system(comp_call.c_str);
    free_string(&comp_call);
    pop_timer(thi);
}

void linking_stage(Thi* thi, char* exec_name) {
    // char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _%s",
    // exec_name, exec_name, exec_name);
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main",
                           exec_name,
                           exec_name);
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

void maybe_convert_call_to_def(Thi* thi, List* ast, List_Node* it) {
    AST* node = (AST*)it->data;
    switch (node->kind) {
    default: break; // error("unhandled case: %s", ast_kind_to_str(node->kind));
    case AST_WHILE: {
        LIST_FOREACH(node->While.then_block->Block.stmts) {
            maybe_convert_call_to_def(thi, ast, it);
        }
    } break;
    case AST_FOR: {
        LIST_FOREACH(node->For.then_block->Block.stmts) {
            maybe_convert_call_to_def(thi, ast, it);
        }
    } break;
    case AST_IF: {
        LIST_FOREACH(node->If.then_block->Block.stmts) {
            maybe_convert_call_to_def(thi, ast, it);
        }
        if (node->If.else_block) {
            LIST_FOREACH(node->If.else_block->Block.stmts) {
                maybe_convert_call_to_def(thi, ast, it);
            }
        }

        // check for any AST_IS inside
        List* stmts                             = node->If.then_block->Block.stmts;
        bool  if_statement_is_actually_a_switch = false;

        LIST_FOREACH(stmts) {
            maybe_convert_call_to_def(thi, ast, it);
            AST* stmt = (AST*)it->data;
            if (stmt->kind == AST_IS) {
                if_statement_is_actually_a_switch = true;
                break;
            }
        }

        // Go through it again and make sure every statement
        // is a case
        if (if_statement_is_actually_a_switch) {
            // Find all NON switchy things and post and error
            LIST_FOREACH(stmts) {
                AST* stmt = (AST*)it->data;
                if (stmt->kind != AST_IS) {
                    error(
                        "%s\nonly 'case' statements are allowed inside an if "
                        "switch",
                        ast_to_str(stmt));
                }
            }

            // Transform this if into a switch
            it->data = make_ast_switch(node->t, node);
        }
    } break;
    case AST_CALL: {
        if (it->next) {
            AST* next_expr = (AST*)it->next->data;
            if (next_expr->kind == AST_BLOCK) {
                char* func_name = node->Call.callee;
                List* args      = node->Call.args;

                bool has_var_args = false;
                LIST_FOREACH(args) {
                    AST* d = (AST*)it->data;
                    if (d->kind == AST_VAR_ARGS) {
                        error("found var args %s", ast_to_str(d));
                        has_var_args = true;
                        break;
                    }
                }

                Type* type =
                    make_type_function(func_name, args, NULL, has_var_args);
                add_symbol(thi, func_name, type);

                AST* body = (AST*)it->next->data;

                LIST_FOREACH(body->Block.stmts) {
                    maybe_convert_call_to_def(thi, ast, it);
                }

                it->data = make_ast_function(node->t, type, body);
                list_remove(ast, it->next);
            }
        }
    } break;
    }
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

    // Find all definitions
    LIST_FOREACH(ast) {
        maybe_convert_call_to_def(thi, ast, it);
    }

    add_all_definitions(thi, &pf);

    // print_ast(ast);

    pop_timer(thi);

    set_source_file(thi, last_file);
    set_current_directory(thi, last_dir);

    // Update thi
    thi->lines += lf.lines;
    thi->comments += lf.comments;

    return ast;
}

void add_all_definitions(Thi* thi, Parsed_File* pf) {
    for (s64 i = 0; i < pf->unresolved_types.count; ++i) {
        type_ref_list_append(&thi->unresolved_types,
                             pf->unresolved_types.data[i]);
    }
    for (s64 i = 0; i < pf->calls.count; ++i) {
        ast_ref_list_append(&thi->calls, pf->calls.data[i]);
    }
    for (s64 i = 0; i < pf->externs.count; ++i) {
        ast_ref_list_append(&thi->externs, pf->externs.data[i]);
    }
    for (s64 i = 0; i < pf->structs.count; ++i) {
        ast_ref_list_append(&thi->structs, pf->structs.data[i]);
    }
    for (s64 i = 0; i < pf->enums.count; ++i) {
        ast_ref_list_append(&thi->enums, pf->enums.data[i]);
    }
    for (s64 i = 0; i < pf->variables_in_need_of_type_inference.count; ++i) {
        ast_ref_list_append(&thi->variables_in_need_of_type_inference,
                            pf->variables_in_need_of_type_inference.data[i]);
    }
    for (s64 i = 0; i < pf->constants.count; ++i) {
        ast_ref_list_append(&thi->constants, pf->constants.data[i]);
    }
    for (s64 i = 0; i < pf->identifiers.count; ++i) {
        ast_ref_list_append(&thi->identifiers, pf->identifiers.data[i]);
    }
    for (s64 i = 0; i < pf->field_access.count; ++i) {
        ast_ref_list_append(&thi->field_access, pf->field_access.data[i]);
    }
    for (s64 i = 0; i < pf->subscripts.count; ++i) {
        ast_ref_list_append(&thi->subscripts, pf->subscripts.data[i]);
    }
    LIST_FOREACH(pf->links) {
        add_link(thi, it->data);
    }
    LIST_FOREACH(pf->loads) {
        add_load(thi, it->data);
    }

    s64 count = pf->symbols->size;
    for (s64 i = 0; i < count; ++i) {
        Type* type = pf->symbols->data[i].data;
        add_symbol(thi, type->name, type);
    }
}

Type* get_inferred_type_of_expr(Thi* thi, AST* expr) {
    if (!expr) return NULL;
    switch (expr->kind) {
    case AST_FUNCTION:
        return get_inferred_type_of_expr(thi, expr->Function.body);
    case AST_BLOCK: {
        Type* type = NULL;
        LIST_FOREACH(expr->Block.stmts) {
            AST* stmt = (AST*)it->data;
            if (stmt->kind == AST_RETURN) {
                type = get_inferred_type_of_expr(thi, stmt);
            }
        }
    } break;
    case AST_RETURN: return get_inferred_type_of_expr(thi, expr->Return.expr);
    case AST_SIZEOF: return get_inferred_type_of_expr(thi, expr->Sizeof.expr);
    case AST_AS: return get_inferred_type_of_expr(thi, expr->As.type_expr);
    case AST_NOTE: return get_inferred_type_of_expr(thi, expr->Note.expr);
    case AST_INT: return make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
    case AST_FLOAT: return make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
    case AST_STRING: return make_type_pointer(make_type_int(8, 1));
    case AST_IDENT: return get_symbol(thi, expr->Ident.name);
    case AST_CALL: return get_symbol(thi, expr->Call.callee)->Function.ret_type;
    case AST_UNARY: return get_inferred_type_of_expr(thi, expr->Unary.operand);
    case AST_BINARY: return get_inferred_type_of_expr(thi, expr->Binary.rhs);
    case AST_GROUPING:
        return get_inferred_type_of_expr(thi, expr->Grouping.expr);
    case AST_SUBSCRIPT:
        return get_inferred_type_of_expr(thi, expr->Subscript.load);
    default: error("%s has no type", ast_kind_to_str(expr->kind));
    }
    return NULL;
}

void pass_type_checker(Thi* thi) {
    info("pass_type_checker");
    push_timer(thi, "pass_type_checker");

    // For all functions call..
    for (s64 i = 0; i < thi->calls.count; ++i) {
        // Get the caller and its args
        AST*  call      = thi->calls.data[i];
        List* call_args = call->Call.args;

        warning("Typechecking: %s", ast_to_str(call));

        // Get the callee function and its args
        Type* callee_t  = get_symbol(thi, call->Call.callee);
        List* func_args = callee_t->Function.args;

        // The args must be of equal length.
        if (call_args->count != func_args->count) {
            // Make a nice error message
            char* callee         = call->Call.callee;
            s64   expected_count = func_args->count;
            s64   got_count      = call_args->count;
            s64   line           = call->t.line_pos;
            s64   pos            = call->t.col_pos;

            char* str =
                strf("function call '%s' expected %lld arguments. Got %lld.",
                     callee,
                     expected_count,
                     got_count);
            error("[TYPE_CHECKER %lld:%lld]: %s", line, pos, str);
        }

        // And each callers arguments type must match
        // with the callees arguments type.
        List_Node* it2 = func_args->head;
        LIST_FOREACH(call_args) {
            AST* call_arg = (AST*)it->data;
            AST* func_arg = (AST*)it2->data;

            Type* t0 = call_arg->type;
            Type* t1 = func_arg->type;

            assert(t0);
            assert(t1);

            warning("%s %s", ast_to_str(call_arg), ast_to_str(func_arg));

            if (t0->kind != t1->kind) {
                s64   line = call->t.line_pos;
                s64   pos  = call->t.col_pos;
                char* str  = strf("type missmatch");
                error("[TYPE_CHECKER %lld:%lld]: %s", line, pos, str);
            }

            // NOTE(marcus) This is just asking for trouble.
            it2 = it2->next;
        }
    }

    pop_timer(thi);
}

void pass_progate_identifiers_to_constants(Thi* thi) {
    info("pass_progate_identifiers_to_constants");
    push_timer(thi, "pass_progate_identifiers_to_constants");

    for (s64 i = 0; i < thi->identifiers.count; ++i) {
        AST* ident = thi->identifiers.data[i];
        for (s64 j = 0; j < thi->constants.count; ++j) {
            AST* const_decl = thi->constants.data[j];
            if (strcmp(ident->Ident.name, const_decl->Constant_Decl.name) ==
                0) {
                info("%s turned into %s", ast_to_str(ident), ast_to_str(const_decl->Constant_Decl.value));
                *ident = *const_decl->Constant_Decl.value;
                break;
            }
        }
    }

    pop_timer(thi);
}

void pass_initilize_all_enums(Thi* thi) {
    info("pass_initilize_all_enums");
    push_timer(thi, "pass_initilize_all_enums");

    s64 counter = 0;
    for (s64 i = 0; i < thi->enums.count; ++i) {
        AST* e = thi->enums.data[i];

        LIST_FOREACH(e->Enum.type->Enum.members) {
            AST* m = (AST*)it->data;
            // Turn idents into constant decls
            switch (m->kind) {
            default:
                error("unhandled case: %s, %s, %s", ast_kind_to_str(m->kind), __func__, __LINE__);
            case AST_IDENT:
                it->data = make_ast_constant_decl(m->t, m->Ident.name, make_ast_int(m->t, counter));
                // ast_ref_list_append(&thi->constants, it->data);
                break;
            case AST_CONSTANT_DECL:
                assert(m->Constant_Decl.value->kind == AST_INT);
                counter = m->Constant_Decl.value->Int.val;
                break;
            }
            counter += 1;
            // warning("%s", ast_to_json(m));
        }
    }

    for (s64 i = 0; i < thi->field_access.count; ++i) {
        AST* b   = thi->field_access.data[i];
        AST* lhs = b->Binary.lhs;
        AST* rhs = b->Binary.rhs;

        assert(lhs->kind == AST_IDENT);
        assert(rhs->kind == AST_IDENT);
        Type* e = get_symbol(thi, lhs->Ident.name);
        LIST_FOREACH(e->Enum.members) {
            char* access_member_name = rhs->Ident.name;
            AST*  mem                = (AST*)it->data;
            if (strcmp(access_member_name, mem->Constant_Decl.name) == 0) {
                *b = *make_ast_int(mem->t, mem->Constant_Decl.value->Int.val);
                break;
            }
        }
    }

    pop_timer(thi);
}
void pass_resolve_subscripts(Thi* thi) {
    info("pass_resolve_subscripts");
    push_timer(thi, "pass_resolve_subscripts");

    // for (s64 i = 0; i < thi->subscripts.count; ++i) {
    //     AST* it = thi->subscripts.data[i];
    //     AST* load = it->Subscript.load;
    //     AST* sub = it->Subscript.sub;

    //     sub = make_ast_binary(it->t, TOKEN_ASTERISK, make_ast_int(it->t, 4),
    //     sub); load = make_ast_unary(it->t, THI_SYNTAX_ADDRESS, load); sub =
    //     make_ast_binary(it->t, TOKEN_PLUS, load, sub); sub =
    //     make_ast_unary(it->t, THI_SYNTAX_POINTER, sub); *it = *sub;
    // }

    pop_timer(thi);
}

void pass_give_all_identifiers_a_type(Thi* thi) {
    info("pass_give_all_identifiers_a_type");
    push_timer(thi, "pass_give_all_identifiers_a_type");

    for (s64 i = 0; i < thi->identifiers.count; ++i) {
        AST* ident = thi->identifiers.data[i];
        if (ident->kind != AST_IDENT) continue;
        ident->type = get_symbol(thi, ident->Ident.name);
        warning("%s got type %s", ast_to_str(ident), type_to_str(ident->type));
    }

    pop_timer(thi);
}

void pass_type_inference(Thi* thi) {
    info("pass_type_inference");
    push_timer(thi, "pass_type_inference");

    for (s64 i = 0; i < thi->calls.count; ++i) {
        AST* call = thi->calls.data[i];
        // warning("%s", ast_to_json(call));
        call->type = get_inferred_type_of_expr(thi, call);
        if (!call->type) {
            call->type = make_type_int(
                1, 1); // NOTE(marcus) should this be void instead?
        }
    }

    for (s64 i = 0; i < thi->variables_in_need_of_type_inference.count; ++i) {
        AST* var_decl = thi->variables_in_need_of_type_inference.data[i];
        var_decl->Variable_Decl.type =
            get_inferred_type_of_expr(thi, var_decl->Variable_Decl.value);
        var_decl->type = var_decl->Variable_Decl.type;
    }

    pop_timer(thi);
}
