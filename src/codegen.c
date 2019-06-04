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

// [1] System V Application Binary Interface AMD64 Architecture Processor Supplement (With LP64 and ILP32 Programming Models) Version 1.0

#include "codegen.h"
#include "ast.h" // AST*, ast_to_str
#include "codegen_utility.h"
#include "constants.h"
#include "lexer.h" // token_kind_to_str
#include "list.h"
#include "register.h"
#include "typedefs.h"
#include "utility.h" // error warning info, wrap_with_colored_parens
#include "value.h"   // Value, Scope
#include <assert.h>  // assert
#include <stdio.h>   //
#include <string.h>  // strcmp

#define DEBUG_START                                                              \
    assert(ctx);                                                                 \
    assert(node);                                                                \
    info("%s: %s", (char*)__func__, wrap_with_colored_parens(ast_to_str(node))); \
    // emit(ctx, "; %s", ast_to_str(node));

Value* codegen_sizeof(Codegen_Context* ctx, AST* node);
Value* codegen_extern(Codegen_Context* ctx, AST* node);
Value* codegen_unary(Codegen_Context* ctx, AST* node);
Value* codegen_binary(Codegen_Context* ctx, AST* node);
Value* codegen_variable_decl(Codegen_Context* ctx, AST* node);
Value* codegen_constant_decl(Codegen_Context* ctx, AST* node);
Value* codegen_call(Codegen_Context* ctx, AST* node);
Value* codegen_float(Codegen_Context* ctx, AST* node);
Value* codegen_int(Codegen_Context* ctx, AST* node);
Value* codegen_block(Codegen_Context* ctx, AST* node);
Value* codegen_macro(Codegen_Context* ctx, AST* node);
Value* codegen_ident(Codegen_Context* ctx, AST* node);
Value* codegen_string(Codegen_Context* ctx, AST* node);
Value* codegen_note(Codegen_Context* ctx, AST* node);
Value* codegen_if(Codegen_Context* ctx, AST* node);
Value* codegen_for(Codegen_Context* ctx, AST* node);
Value* codegen_while(Codegen_Context* ctx, AST* node);
Value* codegen_defer(Codegen_Context* ctx, AST* node);
Value* codegen_return(Codegen_Context* ctx, AST* node);
Value* codegen_break(Codegen_Context* ctx, AST* node);
Value* codegen_continue(Codegen_Context* ctx, AST* node);
Value* codegen_struct(Codegen_Context* ctx, AST* node);
Value* codegen_enum(Codegen_Context* ctx, AST* node);
Value* codegen_function(Codegen_Context* ctx, AST* node);
Value* codegen_as(Codegen_Context* ctx, AST* node);
Value* codegen_switch(Codegen_Context* ctx, AST* node);
Value* codegen_post_inc_or_dec(Codegen_Context* ctx, AST* node);
Value* codegen_node(Codegen_Context* ctx, AST* node);

// @Hotpath @Recursive
Value* codegen_node(Codegen_Context* ctx, AST* node) {
    assert(ctx);
    if (!node) return NULL;
    switch (node->kind) {
        ERROR_UNHANDLED_AST_KIND(node->kind);
    case AST_MODULE:
        LIST_FOREACH(node->Module.top_level) {
            codegen_node(ctx, it->data);
        }
        return NULL;
    case AST_POST_INC_OR_DEC: return codegen_post_inc_or_dec(ctx, node);
    case AST_COMMENT: return NULL;
    case AST_NOP: return NULL;
    case AST_FALLTHROUGH: return NULL;
    case AST_LOAD: return codegen_node(ctx, node->Load.module);
    case AST_LINK: return NULL;
    case AST_SWITCH: return codegen_switch(ctx, node);
    case AST_EXTERN: return codegen_extern(ctx, node);
    case AST_STRUCT: return codegen_struct(ctx, node);
    case AST_ENUM: return codegen_enum(ctx, node);
    case AST_FUNCTION: return codegen_function(ctx, node);
    case AST_NOTE: return codegen_note(ctx, node);
    case AST_INT: return codegen_int(ctx, node);
    case AST_FLOAT: return codegen_float(ctx, node);
    case AST_STRING: return codegen_string(ctx, node);
    case AST_CHAR: return codegen_node(ctx, make_ast_int(node->loc_info, node->Char.val, node->type));
    case AST_IDENT: return codegen_ident(ctx, node);
    case AST_CALL: return codegen_call(ctx, node);
    case AST_UNARY: return codegen_unary(ctx, node);
    case AST_BINARY: return codegen_binary(ctx, node);
    case AST_VARIABLE_DECL: return codegen_variable_decl(ctx, node);
    case AST_CONSTANT_DECL: return codegen_constant_decl(ctx, node);
    case AST_BLOCK: return codegen_block(ctx, node);
    case AST_GROUPING: return codegen_node(ctx, node->Grouping.node);
    case AST_IF: return codegen_if(ctx, node);
    case AST_FOR: return codegen_for(ctx, node);
    case AST_WHILE: return codegen_while(ctx, node);
    case AST_RETURN: return codegen_return(ctx, node);
    case AST_DEFER: return codegen_defer(ctx, node);
    case AST_BREAK: return codegen_break(ctx, node);
    case AST_CONTINUE: return codegen_continue(ctx, node);
    case AST_AS: return codegen_as(ctx, node);
    }
    UNREACHABLE;
    return NULL;
}

char* generate_code_from_ast(AST* ast) {
    assert(ast);
    info("Generating code from ast");
    Codegen_Context ctx = make_codegen_context();
    string_append(ctx.section_data, "section .data\n");
    emit_no_tab(&ctx, "section .text");
    codegen_node(&ctx, ast);
    char* output = strf("%s%sglobal _main\n%s", string_data(ctx.section_extern), string_data(ctx.section_data), string_data(ctx.section_text));
    info("%s", output);
    return output;
}

Value* codegen_unary(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    Token_Kind op = node->Unary.op;
    AST* operand = node->Unary.operand;

    // s64    operand_size = get_size_of_value(operand_val);

    Value* operand_val = codegen_node(ctx, operand);
    char* reg = get_result_reg(operand->type);
    Value* result = operand_val;

    switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
    case TOKEN_MINUS_MINUS: {
        s64 size = 1;
        if (operand->type->kind == TYPE_POINTER)
            size = get_size_of_underlying_type_if_any(operand->type);
        result = codegen_node(ctx, make_ast_binary(node->loc_info, TOKEN_MINUS_EQ, operand, make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, 0))));
    } break;
    case TOKEN_PLUS_PLUS: {
        s64 size = 1;
        if (operand->type->kind == TYPE_POINTER)
            size = get_size_of_underlying_type_if_any(operand->type);
        result = codegen_node(ctx, make_ast_binary(node->loc_info, TOKEN_PLUS_EQ, operand, make_ast_int(node->loc_info, size, make_type_int(DEFAULT_INT_BYTE_SIZE, 0))));
    } break;
    case THI_SYNTAX_ADDRESS: {
        s64 stack_pos = get_stack_pos_of_variable(operand_val);
        emit(ctx, "lea rax, [rbp-%lld]; addrsof", stack_pos);
    } break;
    case THI_SYNTAX_POINTER: {
        Type* t = operand->type;
        char* reg = get_result_reg(t);
        emit(ctx, "mov %s, [rax]; deref ", reg);
        // A deref expects an lvalue and returns an lvalue
    } break;
    case TOKEN_BANG: {
        emit(ctx, "cmp %s, 0", reg);
        emit(ctx, "sete al");
    } break;
    case TOKEN_PLUS: { // no nothing
    } break;
    case TOKEN_TILDE: {
        emit(ctx, "not al");
    } break;
    case TOKEN_DOT: {
    } break;
    case TOKEN_MINUS: {
        Type_Kind tk = operand->type->kind;
        switch (tk) {
            ERROR_UNHANDLED_TYPE_KIND(tk);
        case TYPE_INT:
            emit(ctx, "neg %s", reg);
            break;

        case TYPE_FLOAT:
            emit(ctx, "movd ecx, xmm0");
            emit(ctx, "xor ecx, 2147483648");
            emit(ctx, "movd xmm0, ecx");
            break;
        }
        break;
    }
    }
    return result;
}

Value* codegen_binary(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    Token_Kind op = node->Binary.op;
    AST* lhs = node->Binary.lhs;
    AST* rhs = node->Binary.rhs;

    switch (op) {
        ERROR_UNHANDLED_TOKEN_KIND(op);
    case THI_SYNTAX_ASSIGNMENT: {

        bool is_deref = false;

        if (lhs->kind == AST_UNARY) {
            lhs = lhs->Unary.operand;
            is_deref = true;
        }

        Value* rhs_v = codegen_node(ctx, rhs);
        push_type(ctx, rhs_v->type);
        Value* lhs_v = codegen_node(ctx, lhs);
        pop_type_2(ctx, rhs_v->type);
        if (is_deref) {
            emit_store_deref(ctx, lhs_v);
        } else
            emit_store(ctx, lhs_v);
        emit_load(ctx, lhs_v);
        return lhs_v;
    }

    case TOKEN_PLUS: {
        Value* rhs_v = codegen_node(ctx, rhs);
        if (rhs_v->type->kind == TYPE_FLOAT) {
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, XMM0);
            Value* lhs_v = codegen_node(ctx, lhs);
            pop(ctx, XMM1);
            emit(ctx, "%s xmm0, xmm1", inst);
            return lhs_v;
        } else {
            push(ctx, RAX);
            Value* lhs_v = codegen_node(ctx, lhs);
            pop(ctx, RCX);
            emit(ctx, "add rax, rcx");
            return lhs_v;
        }
    }
    case TOKEN_MINUS: {
        Value* rhs_v = codegen_node(ctx, rhs);
        if (rhs_v->type->kind == TYPE_FLOAT) {
            Value* rhs_v = codegen_node(ctx, rhs);
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, XMM0);
            Value* lhs_v = codegen_node(ctx, lhs);
            pop(ctx, XMM1);
            emit(ctx, "%s xmm0, xmm1", inst);
            return lhs_v;
        } else {
            push(ctx, RAX);
            Value* lhs_v = codegen_node(ctx, lhs);
            pop(ctx, RCX);
            emit(ctx, "sub rax, rcx");
            return lhs_v;
        }
    }
    case TOKEN_ASTERISK: {
        Value* lhs_v = codegen_node(ctx, lhs);
        if (lhs_v->type->kind == TYPE_FLOAT) {
            char* inst = get_instr(op, lhs_v->type);
            push(ctx, XMM0);
            codegen_node(ctx, rhs);
            pop(ctx, XMM1);
            emit(ctx, "%s xmm0, xmm1", inst);
            return lhs_v;
        } else {
            push(ctx, RAX);
            codegen_node(ctx, rhs);
            pop(ctx, RCX);
            emit(ctx, "imul rax, rcx");
            return lhs_v;
        }
    }
    case TOKEN_FWSLASH: {
        Value* rhs_v = codegen_node(ctx, rhs);
        if (rhs_v->type->kind == TYPE_FLOAT) {
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, XMM0);
            codegen_node(ctx, lhs);
            pop(ctx, XMM1);
            emit(ctx, "%s xmm1", inst);
            return rhs_v;
        } else {
            char* inst = get_instr(op, rhs_v->type);
            push(ctx, RAX);
            codegen_node(ctx, lhs);
            pop(ctx, RCX);
            emit(ctx, "cdq");
            emit(ctx, "%s rcx", inst);
            return rhs_v;
        }
    }

    case TOKEN_PERCENT: {
        node = make_ast_binary(node->loc_info, TOKEN_FWSLASH, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        emit(ctx, "mov rax, rdx");
        return variable;
    }
    case TOKEN_PIPE: {
        Value* lhs_v = codegen_node(ctx, lhs);
        push(ctx, RAX);
        codegen_node(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "or al, cl");
        return lhs_v;
    }
    case TOKEN_AND: {
        Value* lhs_v = codegen_node(ctx, lhs);
        push(ctx, RAX);
        codegen_node(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "and al, cl");
        return lhs_v;
    }
    case TOKEN_HAT: {
        Value* lhs_v = codegen_node(ctx, lhs);
        push(ctx, RAX);
        codegen_node(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "xor al, cl");
        return lhs_v;
    }
    case TOKEN_LT_LT: {
        Value* lhs_v = codegen_node(ctx, lhs);
        assert(rhs->kind == AST_INT);
        emit(ctx, "shl al, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_GT_GT: {
        Value* lhs_v = codegen_node(ctx, lhs);
        assert(rhs->kind == AST_INT);
        emit(ctx, "shr al, %lld", rhs->Int.val);
        return lhs_v;
    }
    case TOKEN_AND_AND: {
        Value* lhs_v = codegen_node(ctx, lhs);
        push(ctx, RAX);
        codegen_node(ctx, rhs);
        pop(ctx, RCX);
        emit(ctx, "cmp rax, 0");
        emit(ctx, "setne al");
        emit(ctx, "cmp rcx, 0");
        emit(ctx, "setne cl");
        emit(ctx, "and al, cl");
        return lhs_v;
    }

    case TOKEN_PIPE_PIPE: {
        node = make_ast_binary(node->loc_info, TOKEN_PIPE, lhs, rhs);
        Value* v = codegen_node(ctx, node);
        emit(ctx, "setne al");
        return v;
    }

    case TOKEN_LT:    // FALLTHROUGH
    case TOKEN_GT:    // FALLTHROUGH
    case TOKEN_LT_EQ: // FALLTHROUGH
    case TOKEN_GT_EQ: // FALLTHROUGH
    case TOKEN_EQ_EQ: // FALLTHROUGH
    case TOKEN_BANG_EQ: {
        Value* lhs_v = codegen_node(ctx, lhs);
        if (lhs_v->type->kind == TYPE_FLOAT) {
            push(ctx, XMM0);
            codegen_node(ctx, rhs);
            pop(ctx, XMM1);
            if (lhs_v->type->Float.bytes == 4) {
                emit(ctx, "ucomiss xmm1, xmm0");
            } else {
                emit(ctx, "ucomisd xmm1, xmm0");
            }
        } else {
            push(ctx, RAX);
            codegen_node(ctx, rhs);
            pop(ctx, RCX);
            emit(ctx, "cmp rcx, rax");
        }
        switch (op) {
            ERROR_UNHANDLED_TOKEN_KIND(op);
        case TOKEN_LT:
            lhs_v->type->kind == TYPE_FLOAT ? emit(ctx, "setb al")
                                            : emit(ctx, "setl al");
            break;
        case TOKEN_GT: emit(ctx, "setg al"); break;
        case TOKEN_LT_EQ:
            lhs_v->type->kind == TYPE_FLOAT ? emit(ctx, "setna al")
                                            : emit(ctx, "setle al");
            break;
        case TOKEN_GT_EQ: emit(ctx, "setge al"); break;
        case TOKEN_EQ_EQ: emit(ctx, "sete al"); break;
        case TOKEN_BANG_EQ: emit(ctx, "setne al"); break;
        }
        return lhs_v;
    } break;

    case TOKEN_LT_LT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_LT_LT, lhs, rhs);
        node = make_ast_unary(node->loc_info, TOKEN_EQ, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_GT_GT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_GT_GT, lhs, rhs);
        node = make_ast_unary(node->loc_info, TOKEN_EQ, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_PLUS_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_PLUS, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_MINUS_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_MINUS, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_ASTERISK_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_ASTERISK, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_FWSLASH_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_FWSLASH, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_PERCENT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_PERCENT, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_PIPE_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_PIPE, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_HAT_EQ: {
        rhs = make_ast_binary(node->loc_info, TOKEN_HAT, lhs, rhs);
        node = make_ast_binary(node->loc_info, TOKEN_EQ, lhs, rhs);
        Value* variable = codegen_node(ctx, node);
        return variable;
    }
    case TOKEN_QUESTION_MARK: {
        char* l0 = make_text_label(ctx);
        char* l1 = make_text_label(ctx);
        set_temp_labels(ctx, l0, l1);
        Value* lhs_v = codegen_node(ctx, lhs);
        char* reg = get_result_reg(lhs_v->type);
        emit(ctx, "cmp %s, 0", reg);
        emit(ctx, "je %s", l0);
        Value* rhs_v = codegen_node(ctx, rhs);
        emit(ctx, "jmp %s", l1);
        return rhs_v;
    }
    case TOKEN_COLON: {
        codegen_node(ctx, lhs); // '?' part
        emit(ctx, "%s:", ctx->l0);
        Value* rhs_v = codegen_node(ctx, rhs);
        emit(ctx, "%s:", ctx->l1);
        restore_temp_labels(ctx);
        return rhs_v;
    }
    }

    UNREACHABLE;
    return NULL;
}

Value* codegen_constant_decl(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    return NULL;
}

Value* codegen_variable_decl(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_VARIABLE_DECL);
    char* name = node->Variable_Decl.name;
    Type* type = node->type;
    AST* assignment_expr = node->Variable_Decl.value;
    bool is_global = node->flags & AST_FLAG_GLOBAL_VARIABLE;
    Value* variable = NULL;
    // Set the label for it..
    if (is_global) {
        char* initial_value = NULL;
        if (assignment_expr)
            initial_value = ast_get_literal_value_as_str(assignment_expr);
        else
            initial_value = "0";
        char* db_op = get_db_op(type);
        char* label = emit_data(ctx, "%s %s", db_op, initial_value);
        variable = make_value_global_variable(name, type, label);
    } else {
        s64 type_size = get_size_of_type(type);
        s64 stack_pos = type_size + ctx->stack_pos;
        info("name: %s stack_pos: %d type_size: %d", name, stack_pos, type_size);
        variable = make_value_variable(name, type, stack_pos);
    }
    add_variable(ctx, variable);
    if (assignment_expr && !is_global) {
        codegen_node(ctx, make_ast_binary(node->loc_info, TOKEN_EQ, make_ast_ident(node->loc_info, name), assignment_expr));
    }
    return variable;
}

Value* codegen_float(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_FLOAT);
    Value* val = make_value_float(node->type, node->Float.val);
    char* db_op = get_db_op(val->type);
    char* flabel = emit_data(ctx, "%s %f", db_op, val->Float.value);
    char* mov_op = get_move_op(val->type);
    char* reg = get_result_reg(val->type);
    emit(ctx, "%s %s, [rel %s]; float_ref", mov_op, reg, flabel);
    return val;
}

Value* codegen_int(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_INT);
    Value* val = make_value_int(DEFAULT_INT_BYTE_SIZE, node->type, node->Int.val);
    char* reg = get_result_reg(val->type);
    char* mov_op = get_move_op(val->type);
    emit(ctx, "%s %s, %d", mov_op, reg, val->Int.value);
    return val;
}

Value* codegen_block(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    push_scope(ctx);
    List* stmts = node->Block.stmts;
    Value* last = NULL;
    // if (node->Block.flags & BLOCK_LAST_EXPR_IS_IMPLICITLY_RETURNED) {
    // AST* last_stmt = list_last(stmts);
    // ast_replace(last_stmt, make_ast_return(last_stmt->loc_info, last_stmt));
    // }
    LIST_FOREACH(stmts) {
        last = codegen_node(ctx, it->data);
    }
    pop_scope(ctx);
    return last;
}

Value* codegen_ident(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_IDENT);
    Value* var = get_variable(ctx, node);
    emit_load(ctx, var);
    return var;
}

Value* codegen_string(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_STRING);
    char* val = node->String.val;
    Type* t = make_type_pointer(make_type_int(8, 1));
    char* slabel = emit_data(ctx, "db `%s`, 0 ", val);
    char* mov_op = get_move_op(t);
    char* reg = get_result_reg(t);
    emit(ctx, "%s %s, %s; string_ref", mov_op, reg, slabel);
    return make_value_string(val, t);
}

Value* codegen_note(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_NOTE);
    assert(node->Note.node->kind == AST_INT);
    s64 integer_value = node->Note.node->Int.val;
    if (integer_value < 1) error("note parameters start at 1.");

    error("@get_arg_from_func thinks parametrs is AST node. CHANGE IT.");
    AST* arg = get_arg_from_func(ctx->current_function->type,
                                 integer_value - 1);
    Value* var = get_variable(ctx, make_ast_ident(arg->loc_info, arg->Variable_Decl.name));

    emit_load(ctx, var);

    return var;
}

Value* codegen_if(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_IF);

    char* else_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    AST* cond = node->If.cond;
    AST* then_block = node->If.then_block;
    AST* else_block = node->If.else_block;

    codegen_node(ctx, cond);
    emit(ctx, "cmp al, 0");
    emit(ctx, "je %s", else_block ? else_l : end_l);
    codegen_node(ctx, then_block);
    emit(ctx, "jmp %s", end_l);
    emit(ctx, "%s:", else_l);
    if (else_block) {
        codegen_node(ctx, else_block);
    }
    emit(ctx, "%s:", end_l);

    return NULL;
}

Value* codegen_for(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_FOR);

    push_scope(ctx);

    char* begin_l = make_text_label(ctx);
    char* mid_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    AST* init = node->For.init;
    AST* cond = node->For.cond;
    AST* step = node->For.step;
    AST* then_block = node->For.then_block;

    codegen_node(ctx, init);
    emit(ctx, "%s:", begin_l);
    codegen_node(ctx, cond);
    emit(ctx, "cmp al, 0");
    emit(ctx, "je %s", end_l);

    set_jump_labels(ctx, mid_l, end_l);
    codegen_node(ctx, then_block);
    restore_jump_labels(ctx);

    emit(ctx, "%s:", mid_l);
    codegen_node(ctx, step);
    emit(ctx, "jmp %s", begin_l);
    emit(ctx, "%s:", end_l);

    pop_scope(ctx);

    return NULL;
}

Value* codegen_while(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_WHILE);

    char* begin_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    AST* cond = node->While.cond;
    AST* then_block = node->While.then_block;

    emit(ctx, "%s:", begin_l);
    codegen_node(ctx, cond);
    emit(ctx, "cmp al, 0");
    emit(ctx, "je %s", end_l);

    set_jump_labels(ctx, begin_l, end_l);
    codegen_node(ctx, then_block);
    restore_jump_labels(ctx);

    emit(ctx, "jmp %s", begin_l);
    emit(ctx, "%s:", end_l);

    return NULL;
}

Value* codegen_defer(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_DEFER);
    AST* defer_expr = node->Defer.node;
    list_append(ctx->current_function->Function.defers, defer_expr);
    return NULL;
}
// From the System V Application Binary Interface Manual
// -- Returning of Values
//  The returning of values is done according to the following algorithm:
//      1. Classify the return type with the classification algorithm.
//      2. If the type has class MEMORY, then the caller provides space for the return value and passes the address of this storage in %rdi as if it were the first argument to the function.
//         In effect, this address becomes a “hidden” first ar- gument. This storage must not overlap any data visible to the callee through other names than this argument.
//         On return %rax will contain the address that has been passed in by the caller in %rdi.
//      3. If the class is INTEGER, the next available register of the sequence %rax, %rdx is used.
//      4. If the class is SSE, the next available vector register of the sequence %xmm0, %xmm1 is used.
//      5. If the class is SSEUP, the eightbyte is returned in the next available eightbyte chunk of the last used vector register.
Value* codegen_return(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_RETURN);
    AST* ret_e = node->Return.node;
    Value* ret_v = NULL;
    if (ret_e) {
        Class_Kind class = classify(ret_e->type);
        ret_v = codegen_node(ctx, ret_e);
        s64 size = get_size_of_value(ret_v);
        s8 return_reg = -1;
        s8 class_integer_counter = 0;
        s8 class_sse_counter = 0;
        switch (class) {
            ERROR_UNHANDLED_CLASS_KIND(class);
        // case CLASS_MEMORY: break;
        case CLASS_INTEGER: return_reg = get_return_reg_int(class_integer_counter++, size); break;
        case CLASS_SSE:
            return_reg = get_return_reg_float(class_sse_counter++);
            break;
            // case CLASS_SSEUP: break;
        }
        char* mov_op = get_move_op(ret_v->type);
        char* result_reg = get_result_reg(ret_v->type);
        emit(ctx, "%s %s, %s", mov_op, get_reg(return_reg), result_reg);
    }
    emit(ctx, "jmp %s", DEFAULT_FUNCTION_END_LABEL_NAME);
    return ret_v;
}

Value* codegen_break(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_BREAK);
    emit(ctx, "jmp %s", ctx->lbreak);
    return NULL;
}

Value* codegen_post_inc_or_dec(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_POST_INC_OR_DEC);
    Value* v = codegen_node(ctx, node->Post_Inc_or_Dec.node);
    push_type(ctx, v->type);
    v = codegen_node(ctx, make_ast_unary(node->loc_info, node->Post_Inc_or_Dec.op, node->Post_Inc_or_Dec.node));
    pop_type(ctx, v->type);
    return v;
}

Value* codegen_switch(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_SWITCH);

    char* default_l = make_text_label(ctx);
    char* end_l = make_text_label(ctx);

    set_break_label(ctx, end_l);

    AST* cond = node->Switch.cond;
    AST* cases = node->Switch.cases;
    AST* default_case = node->Switch.default_case;

    List* labels = make_list();
    LIST_FOREACH(cases->Block.stmts) {
        AST* c = (AST*)it->data;

        char* l = make_text_label(ctx);
        list_append(labels, l);

        AST* case_cond = c->Is.node;
        codegen_node(ctx, make_ast_binary(c->loc_info, TOKEN_EQ_EQ, cond, case_cond));

        emit(ctx, "cmp al, 1");
        emit(ctx, "je %s", l);
    }

    if (default_case) {
        emit(ctx, "%s:", default_l);
        codegen_node(ctx, default_case);
        emit(ctx, "jmp %s", end_l);
    } else
        emit(ctx, "jmp %s", end_l);

    List_Node* label_it = labels->head;
    LIST_FOREACH(cases->Block.stmts) {
        AST* c = (AST*)it->data;

        char* l = (char*)label_it->data;
        emit(ctx, "%s:", l);
        codegen_node(ctx, c->Is.body);

        if (!c->Is.has_fallthrough) {
            emit(ctx, "jmp %s", end_l);
        }

        label_it = label_it->next;
    }

    emit(ctx, "%s:", end_l);

    restore_break_label(ctx);

    return NULL;
}

Value* codegen_as(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_AS);
    AST* e = node->As.node;
    Type* t = node->As.type_node->type;
    Value* v = codegen_node(ctx, e);
    emit_cast(ctx, v, t);
    return v;
}

Value* codegen_continue(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_CONTINUE);
    emit(ctx, "jmp %s", ctx->lcontinue);
    return NULL;
}

Value* codegen_enum(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_ENUM);
    warning("enum incomplete?");
    return NULL;
}

Value* codegen_extern(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_EXTERN);
    char* func_name = node->Extern.type->Function.name;
    emit_extern(ctx, func_name);
    return NULL;
}

Value* codegen_struct(Codegen_Context* ctx, AST* node) {
    DEBUG_START;
    assert(node->kind == AST_STRUCT);
    info("struct incomplete?");
    return make_value_struct(node->type);
}

// From the System V Application Binary Interface Manual
// -- Passing
//  Once arguments are classified, the registers get assigned(in left-to-right order) for passing as follows:
//      1. If the class is MEMORY, pass the argument on the stack.
//      2. If the class is INTEGER, the next available register of the sequence %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used15.
//      3. If the class is SSE, the next available vector register is used, the registers are taken in the order from %xmm0 to %xmm7.
//      4. If the class is SSEUP, the eightbyte is passed in the next available eightbyte chunk of the last used vector register.
//      5. If the class is X87, X87UP or COMPLEX_X87, it is passed in memory.
//
//  When a value of type _Bool is returned or passed in a register or on the stack, bit 0 contains the truth value and bits 1 to 7 shall be zero.
//
//  If there are no registers available for any eightbyte of an argument, the whole argument is passed on the stack. If registers have already been assigned for
//  some eightbyte of such argument, the assignments get reverted.
//
//  Once registers are assigned, the arguments passed in memory are pushed on the stack in reversed (right-to-left) order.
//  For calls that may call functions that use varargs or stdargs (prototype-less calls or calls to functions containing ellipsis (...) in the declaration) %al is used
//  as hidden argument to specify the number of vector registers used. The contents of %al do not need to match exactly the number of registers, but must be an upper bound
//  on the number of vector registers used and is in the range 0-8 inclusive. When passing __m256 or __m512 arguments to functions that use varargs or stdargs, function
//  prototypes must be provided. Otherwise, the run-time behavior is undefined.
Value* codegen_call(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    List* args = node->Call.args;

    s8 class_integer_counter = 0; // used for getting the next available register
    s8 class_sse_counter = 0;     // used for getting the next available register

    List* values = make_list();
    LIST_FOREACH_REVERSE(args) {
        Value* v = codegen_node(ctx, it->data);
        push_type(ctx, v->type);
        list_append(values, v);
    }

    LIST_FOREACH_REVERSE(values) {
        Value* arg_v = it->data;
        Class_Kind class = classify(arg_v->type);

        s8 param_reg = -1;

        switch (class) {
            ERROR_UNHANDLED_CLASS_KIND(class);
        // case CLASS_MEMORY:          break;
        case CLASS_INTEGER: param_reg = get_parameter_reg_int(class_integer_counter++, 8); break;
        case CLASS_SSE:
            param_reg = get_parameter_reg_float(class_sse_counter++);
            break;
            // case CLASS_SSEUP:           break;
            // case CLASS_X87:             // fallthrough
            // case CLASS_X87UP:           // fallthrough
            // case CLASS_COMPLEX_X87:     break;
        }

        // char* mov_op = get_move_op(arg_v->type);
        // char* result_reg = get_result_reg_of_size(arg_v->type, 8);
        // emit(ctx, "%s %s, %s", mov_op, get_reg(param_reg), result_reg);

        // @FixMe!
        pop(ctx, param_reg);
    }

    if (node->type->flags & TYPE_FLAG_HAS_VAR_ARG) {
        emit(ctx, "mov al, %lld; var_arg_count", args->count);
    }

    char* callee = node->Call.callee;
    emit(ctx, "call _%s", callee);

    Type* return_type = node->type;
    return make_value_call(callee, return_type);
}

Value* codegen_function(Codegen_Context* ctx, AST* node) {
    DEBUG_START;

    set_current_function_expr(ctx, node);

    char* func_name = node->Function.name;
    AST* func_body = node->Function.body;

    push_scope(ctx);

    emit(ctx, "_%s:", func_name);
    push(ctx, RBP);
    emit(ctx, "mov rbp, rsp");

    // Sum the params
    s64 sum = get_size_of_type(node->type);

    sum += get_all_alloca_in_block(func_body);

    s64 stack_allocated = sum;
    s32 padding = X64_ASM_MACOS_STACK_PADDING - (stack_allocated % X64_ASM_MACOS_STACK_PADDING);
    if (stack_allocated + padding)
        emit(ctx, "sub rsp, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);

    emit(ctx, "%s:", DEFAULT_FUNCTION_ENTRY_LABEL_NAME);

    reset_stack(ctx);

    List* args = node->Function.parameters;

    s8 class_integer_counter = 0; // used for getting the next available register
    s8 class_sse_counter = 0;     // used for getting the next available register

    List* classified_arguments = classify_arguments(args);
    LIST_FOREACH(classified_arguments) {

        ClassifiedArgument* ca = it->data;
        AST* arg = ca->argument;
        Class_Kind class = ca->class;

        Value* arg_v = codegen_node(ctx, arg);
        s64 size = get_size_of_value(arg_v);
        s8 param_reg = -1;

        switch (class) {
            ERROR_UNHANDLED_CLASS_KIND(class);
        // case CLASS_MEMORY: break;
        case CLASS_INTEGER: param_reg = get_parameter_reg_int(class_integer_counter++, size); break;
        case CLASS_SSE:
            param_reg = get_parameter_reg_float(class_sse_counter++);
            break;
            // case CLASS_SSEUP:       break;
            // case CLASS_X87:         // fallthrough
            // case CLASS_X87UP:       // fallthrough
            // case CLASS_COMPLEX_X87: break;
        }

        emit_store_r(ctx, arg_v, param_reg);
    }

    push_scope(ctx);
    List* stmts = func_body->Block.stmts;
    LIST_FOREACH(stmts) {
        codegen_node(ctx, it->data);
    }

    emit(ctx, "%s:", DEFAULT_FUNCTION_END_LABEL_NAME);

    List* defers = ctx->current_function->Function.defers;
    LIST_FOREACH_REVERSE(defers) {
        codegen_node(ctx, it->data);
    }
    pop_scope(ctx);

    if (stack_allocated + padding) {
        emit(ctx, "add rsp, %lld; %lld alloc, %lld padding", stack_allocated + padding, stack_allocated, padding);
    }

    emit(ctx, "leave");
    emit(ctx, "ret");

    reset_text_label_counter(ctx);

    pop_scope(ctx);
    return NULL;
}

typedef enum {
    OPER_MEM,
    OPER_REG,
    OPER_IMM,
    _OPER_COUNT_,
} Operand_Kind;

typedef struct {
    Operand_Kind kind;
} Operand;

typedef enum {
    INSTRUCTION_MOV,
    INSTRUCTION_ADD,
    INSTRUCTION_SUB,
    INSTRUCTION_IMUL,
    INSTRUCTION_MUL,
    INSTRUCTION_IDIV,
    INSTRUCTION_DIV,
    INSTRUCTION_PUSH,
    INSTRUCTION_POP,
    INSTRUCTION_JMP,
    INSTRUCTION_JE,
    _INSTRUCTION_COUNT_,
} Instruction_Kind;

typedef struct {
    char* label;
    Instruction_Kind instr;
    Operand o1, o2;
    char* comment;
} NASM_Line;
™
