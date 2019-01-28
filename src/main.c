#include "ast.h" // AST
#include "codegen.h" // generate_code_from_ast
#include "constants.h"
#include "lexer.h" // generate_tokens_from_source, print_tokens
#include "list.h" // list_tests
#include "map.h" // map
#include "parser.h" // generate_ast_from_tokens
#include "stack.h" // stack_tests
#include "string.h" // strcmp
#include "thi.h" // Thi
#include "type.h" // Type
#include "typedefs.h"
#include "utility.h" // get_file_content, success, info, get_time
#include "value.h" // Value
#include <assert.h> // assert
#include <stdio.h> // sprintf
#include <stdlib.h> // free
#include <string.h> // strcmp
#include <sys/ioctl.h>
#include <unistd.h>

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
void  pass_resolve_all_unresolved_types(Thi* thi);
void  pass_type_checker(Thi* thi);
List* parse(Thi* thi, char* source_file);
void  assemble(Thi* thi, char* asm_file, char* exec_name);
void  linking_stage(Thi* thi, char* exec_name);
void  pass_general(Thi* thi);
void  maybe_convert_call_to_def(Thi* thi, List* ast, List_Node* it);

int main(int argc, char** argv)
{
    // Argument validation
    if (argc < 2) error("too few arguments.");

    utility_tests();
    string_tests();
    map_tests();
    list_tests();
    stack_tests();

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
        error("%s is not a .thi file.", source_file);
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
    LIST_FOREACH(get_load_list(&thi))
    {
        char* file  = strf("%s%s", get_current_directory(&thi), it->data);
        List* ast_l = parse(&thi, file);
        list_append_content_of(ast, ast_l);
    }

    thi.ast = ast;


    pass_initilize_all_enums(&thi);

    // pass_initilize_all_enums MUST BE RUN BEFORE THIS
    pass_progate_identifiers_to_constants(&thi);

    pass_resolve_all_unresolved_types(&thi);
    pass_type_inference(&thi);
    pass_give_all_identifiers_a_type(&thi);

    pass_resolve_subscripts(&thi);


    // pass_type_checker(&thi);
    warning("Typechecker disabled");

    // Codegen
    push_timer(&thi, "Codegen");
    char* output = generate_code_from_ast(ast, exec_name);
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
    info("resolved %lld types", thi.unresolved_types.count);
    info("type inferred %lld variables", thi.variables_in_need_of_type_inference.count);
    info("checked calls %lld", thi.calls.count);
    LIST_FOREACH(get_timers(&thi))
    {
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

void assemble(Thi* thi, char* asm_file, char* exec_name)
{
    string comp_call = make_string_f("nasm -f macho64 -g %s -o %s.o", asm_file, exec_name);
    push_timer(thi, "Assembler");
    system(comp_call.c_str);
    free_string(&comp_call);
    pop_timer(thi);
}

void linking_stage(Thi* thi, char* exec_name)
{
    // char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _%s", exec_name, exec_name, exec_name);
    char* link_call = strf("ld -macosx_version_min 10.14 -o %s %s.o -e _main", exec_name, exec_name);
    List* links     = get_link_list(thi);
    LIST_FOREACH(links)
    {
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

void maybe_convert_call_to_def(Thi* thi, List* ast, List_Node* it)
{
    AST* node = (AST*)it->data;
    switch (node->kind) {
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
        List* stmts = node->If.then_block->Block.stmts;
        bool is_actually_a_switch = false;

        LIST_FOREACH(stmts) {
            maybe_convert_call_to_def(thi, ast, it);
            AST* stmt = (AST*)it->data;
            if (stmt->kind == AST_IS) {
                is_actually_a_switch = true;
                break;
            }
        }

        // Go through it again and make sure every statement
        // is a case 
        if (is_actually_a_switch) {
            LIST_FOREACH(stmts) {
                AST* stmt = (AST*)it->data;
                if (stmt->kind != AST_IS) {
                    error("only 'case' statements are allowed inside an if switch");
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
                Type* type      = make_type_function(func_name, args, NULL);
                add_symbol(thi, func_name, type);

                AST* body = (AST*)it->next->data;

                LIST_FOREACH(body->Block.stmts) {
                    maybe_convert_call_to_def(thi, ast, it);
                }

                it->data  = make_ast_function(node->t, type, body);
                list_remove(ast, it->next);
            }
        }
    } break;
    }
}

List* parse(Thi* thi, char* source_file)
{
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
    LIST_FOREACH(ast) { maybe_convert_call_to_def(thi, ast, it); }

    add_all_definitions(thi, &pf);

    print_ast(ast);

    pop_timer(thi);

    set_source_file(thi, last_file);
    set_current_directory(thi, last_dir);

    // Update thi
    thi->lines += lf.lines;
    thi->comments += lf.comments;

    return ast;
}

void add_all_definitions(Thi* thi, Parsed_File* pf)
{
    for (s64 i = 0; i < pf->unresolved_types.count; ++i) {
        type_ref_list_append(&thi->unresolved_types, pf->unresolved_types.data[i]);
    }
    for (s64 i = 0; i < pf->calls.count; ++i) {
        ast_ref_list_append(&thi->calls, pf->calls.data[i]);
    }
    for (s64 i = 0; i < pf->externs.count; ++i) {
        AST*  node = pf->externs.data[i];
        ast_ref_list_append(&thi->externs, pf->externs.data[i]);
    }
    for (s64 i = 0; i < pf->structs.count; ++i) {
        AST* node = pf->structs.data[i];
        ast_ref_list_append(&thi->structs, node);
    }
    for (s64 i = 0; i < pf->enums.count; ++i) {
        AST* node = pf->enums.data[i];
        ast_ref_list_append(&thi->enums, node);
    }
    for (s64 i = 0; i < pf->variables_in_need_of_type_inference.count; ++i) {
        ast_ref_list_append(&thi->variables_in_need_of_type_inference, pf->variables_in_need_of_type_inference.data[i]);
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
    LIST_FOREACH(pf->links) { add_link(thi, it->data); }
    LIST_FOREACH(pf->loads) { add_load(thi, it->data); }

    s64 count = pf->symbols->size;
    for (s64 i = 0; i < count; ++i) {
        Type* type = pf->symbols->data[i].data;
        add_symbol(thi, type->name, type);
    }
}

Type* get_inferred_type_of_expr(Thi* thi, AST* expr)
{
    if (!expr) return NULL;
    switch (expr->kind) {
    case AST_FUNCTION: return get_inferred_type_of_expr(thi, expr->Function.body);
    case AST_BLOCK: {
        Type* type = NULL;
        LIST_FOREACH(expr->Block.stmts)
        {
            AST* stmt = (AST*)it->data;
            if (stmt->kind == AST_RETURN) {
                type = get_inferred_type_of_expr(thi, stmt);
            }
        }
    } break;
    case AST_RETURN: return get_inferred_type_of_expr(thi, expr->Return.expr);
    case AST_SIZEOF: return expr->Sizeof.type;
    case AST_CAST: return expr->Cast.type;
    case AST_NOTE: return get_inferred_type_of_expr(thi, expr->Note.expr);
    case AST_INT: return make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
    case AST_FLOAT: return make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
    case AST_STRING: return make_type_pointer(make_type_int(8, 1));
    case AST_IDENT: return get_symbol(thi, expr->Ident.name);
    case AST_CALL: return get_symbol(thi, expr->Call.callee)->Function.ret_type;
    case AST_UNARY: return get_inferred_type_of_expr(thi, expr->Unary.operand);
    case AST_BINARY: return get_inferred_type_of_expr(thi, expr->Binary.rhs);
    case AST_GROUPING: return get_inferred_type_of_expr(thi, expr->Grouping.expr);
    case AST_SUBSCRIPT: return get_inferred_type_of_expr(thi, expr->Subscript.load);
    default: error("%s has no type", ast_kind_to_str(expr->kind));
    }
    return NULL;
}

void pass_type_checker(Thi* thi)
{
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

            char* str
                = strf("function call '%s' expected %lld arguments. Got %lld.", callee, expected_count, got_count);
            error("[TYPE_CHECKER %lld:%lld]: %s", line, pos, str);
        }

        // And each callers arguments type must match
        // with the callees arguments type.
        List_Node* it2 = func_args->head;
        LIST_FOREACH(call_args)
        {
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

            // NOTE(marcus) I dont really like this..
            it2 = it2->next;
        }
    }

    pop_timer(thi);
}

void pass_progate_identifiers_to_constants(Thi* thi)
{
    info("pass_progate_identifiers_to_constants");
    push_timer(thi, "pass_progate_identifiers_to_constants");

    for (s64 i = 0; i < thi->identifiers.count; ++i) {
        AST* ident = thi->identifiers.data[i];
        for (s64 j = 0; j < thi->constants.count; ++j) {
            AST* const_decl = thi->constants.data[j];
            if (strcmp(ident->Ident.name, const_decl->Constant_Decl.name) == 0) {
                info("%s turned into %s", ast_to_str(ident), ast_to_str(const_decl->Constant_Decl.value));
                *ident = *const_decl->Constant_Decl.value;
                break;
            }
        }
    }

    pop_timer(thi);
}

void pass_initilize_all_enums(Thi* thi)
{
    info("pass_initilize_all_enums");
    push_timer(thi, "pass_initilize_all_enums");

    s64 counter = 0;
    for (s64 i = 0; i < thi->enums.count; ++i) {
        AST* e = thi->enums.data[i];

        LIST_FOREACH(e->Enum.type->Enum.members) {
            AST* m = (AST*)it->data;
            // Turn idents into constant decls 
            switch (m->kind) {
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
            warning("%s", ast_to_json(m));
        }
    }

    for (s64 i = 0; i < thi->field_access.count; ++i) {
        AST* b = thi->field_access.data[i];
        AST* lhs = b->Binary.lhs;
        AST* rhs = b->Binary.rhs;

        assert(lhs->kind == AST_IDENT);
        assert(rhs->kind == AST_IDENT);
        Type* e = get_symbol(thi, lhs->Ident.name);
        LIST_FOREACH(e->Enum.members)
        {
            char* access_member_name = rhs->Ident.name;
            AST* mem = (AST*)it->data;
            if (strcmp(access_member_name, mem->Constant_Decl.name) == 0) {
                *b = *make_ast_int(mem->t, mem->Constant_Decl.value->Int.val);
                break;
            }
        }
    }

    pop_timer(thi);   
}
void pass_resolve_subscripts(Thi* thi)
{
    info("pass_resolve_subscripts");
    push_timer(thi, "pass_resolve_subscripts");

    for (s64 i = 0; i < thi->subscripts.count; ++i) {
        AST* it = thi->subscripts.data[i];
        AST* load = it->Subscript.load;
        AST* sub = it->Subscript.sub;

        sub = make_ast_binary(it->t, TOKEN_ASTERISK, make_ast_int(it->t, 4), sub);
        sub = make_ast_binary(it->t, TOKEN_PLUS, load, sub);
        sub = make_ast_unary(it->t, THI_SYNTAX_POINTER, sub);
        *it = *sub;
    }

    pop_timer(thi);   
}
void pass_give_all_identifiers_a_type(Thi* thi)
{
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

void pass_type_inference(Thi* thi)
{
    info("pass_type_inference");
    push_timer(thi, "pass_type_inference");

    for (s64 i = 0; i < thi->calls.count; ++i) {
        AST* call = thi->calls.data[i];
        warning("%s", ast_to_json(call));
        call->type = get_inferred_type_of_expr(thi, call);
        if (!call->type) {
            call->type = make_type_int(1,1); //NOTE(marcus) should this be void instead?
        }
    }

    for (s64 i = 0; i < thi->variables_in_need_of_type_inference.count; ++i) {
        AST* var_decl                = thi->variables_in_need_of_type_inference.data[i];
        var_decl->Variable_Decl.type = get_inferred_type_of_expr(thi, var_decl->Variable_Decl.value);
        var_decl->type               = var_decl->Variable_Decl.type;
    }

    pop_timer(thi);
}

void pass_resolve_all_unresolved_types(Thi* thi)
{
    info("pass_resolve_all_unresolved_types %lld", thi->unresolved_types.count);
    push_timer(thi, "pass_resolve_all_unresolved_types");
    for (s64 i = 0; i < thi->unresolved_types.count; ++i) {
        Type* t = (Type*)(thi->unresolved_types.data[i]);
        *t      = *get_symbol(thi, get_type_name(t));
    }
    pop_timer(thi);
}
