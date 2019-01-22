#include "ast.h"
#include "globals.h"
#include "lexer.h"   // token_kind_to_str,
#include "string.h"  // strf, append_string, string
#include "utility.h" // info, success, error, warning, xmalloc, xrealloc
#include <assert.h>  // assert

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* expr_kind_to_str(Expr_Kind kind) {
    switch (kind) {
    case EXPR_ASM: return "EXPR_ASM";
    case EXPR_MACRO: return "EXPR_MACRO";
    case EXPR_NOTE: return "EXPR_NOTE";
    case EXPR_INT: return "EXPR_INT";
    case EXPR_FLOAT: return "EXPR_FLOAT";
    case EXPR_STRING: return "EXPR_STRING";
    case EXPR_IDENT: return "EXPR_IDENT";
    case EXPR_CALL: return "EXPR_CALL";
    case EXPR_UNARY: return "EXPR_UNARY";
    case EXPR_BINARY: return "EXPR_BINARY";
    case EXPR_VARIABLE_DECL: return "EXPR_VARIABLE_DECL";
    case EXPR_FUNCTION: return "EXPR_FUNCTION";
    case EXPR_STRUCT: return "EXPR_STRUCT";
    case EXPR_BLOCK: return "EXPR_BLOCK";
    case EXPR_GROUPING: return "EXPR_GROUPING";
    case EXPR_SUBSCRIPT: return "EXPR_SUBSCRIPT";
    case EXPR_IF: return "EXPR_IF";
    case EXPR_FOR: return "EXPR_FOR";
    case EXPR_WHILE: return "EXPR_WHILE";
    case EXPR_RETURN: return "EXPR_RETURN";
    case EXPR_DEFER: return "EXPR_DEFER";
    case EXPR_BREAK: return "EXPR_BREAK";
    case EXPR_CONTINUE: return "EXPR_CONTINUE";
    case EXPR_CAST: return "EXPR_CAST";
    default: warning("expr_kind_to_str unhandled case '%d'", kind);
    }
    return NULL;
}

char* expr_to_str(Expr* expr) {
    switch (expr->kind) {
    case EXPR_DEFER: return strf("defer %s", expr_to_str(expr->Defer.expr));
    case EXPR_BREAK: return "break";
    case EXPR_CONTINUE: return "continue";
    case EXPR_CAST: return strf("cast(%s, %s)", typespec_to_str(expr->Cast.type), expr_to_str(expr->Cast.expr));
    case EXPR_ASM: {
        return strf("%s", expr->Asm.str);
    }
    case EXPR_MACRO: {
        return strf("%s :: %s", expr->Macro.name, expr_to_str(expr->Macro.expr));
    }
    case EXPR_RETURN: {
        if (expr->Return.expr) {
            return strf("return %s", expr_to_str(expr->Return.expr));
        } else {
            return strf("return");
        }
    }
    case EXPR_NOTE: {
        return strf("$%s", expr_to_str(expr->Note.expr));
    }
    case EXPR_INT: {
        return strf("%lld", expr->Int.val);
    }
    case EXPR_FLOAT: {
        return strf("%f", expr->Float.val);
    }
    case EXPR_STRING: {
        return strf("\"%s\"", expr->String.val);
    }
    case EXPR_IDENT: {
        return strf("%s", expr->Ident.name);
    }
    case EXPR_UNARY: {
        return strf("%s(%s)", token_kind_to_str(expr->Unary.op), expr_to_str(expr->Unary.operand));
    }
    case EXPR_BINARY: {
        return strf("%s %s %s", expr_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op),
                    expr_to_str(expr->Binary.rhs));
    }
    case EXPR_VARIABLE_DECL: {
        if (expr->Variable_Decl.value) {
            if (expr->Variable_Decl.name) {
                return strf("%s: %s = %s", expr->Variable_Decl.name, typespec_to_str(expr->Variable_Decl.type),
                            expr_to_str(expr->Variable_Decl.value));
            } else {
                return strf("%s", typespec_to_str(expr->Variable_Decl.type));
            }
        } else {
            if (expr->Variable_Decl.name) {
                return strf("%s: %s", expr->Variable_Decl.name, typespec_to_str(expr->Variable_Decl.type));
            } else {
                return strf("%s", typespec_to_str(expr->Variable_Decl.type));
            }
        }
    }
    case EXPR_BLOCK: {
        string str = make_string("{");
        LIST_FOREACH(expr->Block.stmts) {
            Expr* stmt = (Expr*)it->data;
            append_string_f(&str, "%s\n", expr_to_str(stmt));
        }
        append_string(&str, "}");
        return str.c_str;
    }
    case EXPR_STRUCT: {
        return strf("%s", typespec_to_str(expr->Struct.type));
    }
    case EXPR_FUNCTION: {
        string str = make_string_f("%s %s", typespec_to_str(expr->Function.type), expr_to_str(expr->Function.body));
        return str.c_str;
    }
    case EXPR_GROUPING: return strf("(%s)", expr_to_str(expr->Grouping.expr));
    case EXPR_SUBSCRIPT: return strf("%s[%s]", expr_to_str(expr->Subscript.load), expr_to_str(expr->Subscript.sub));
    case EXPR_IF: {
        if (expr->If.else_block) {
            return strf("if %s %s else %s", expr_to_str(expr->If.cond), expr_to_str(expr->If.then_block),
                        expr_to_str(expr->If.else_block));
        } else {
            return strf("if %s %s", expr_to_str(expr->If.cond), expr_to_str(expr->If.then_block));
        }
    }
    case EXPR_FOR: {
        return strf("for %s, %s, %s %s", expr_to_str(expr->For.init), expr_to_str(expr->For.cond),
                    expr_to_str(expr->For.step), expr_to_str(expr->For.then_block));
    }
    case EXPR_WHILE: {
        return strf("while %s %s", expr_to_str(expr->While.cond), expr_to_str(expr->While.then_block));
    }
    case EXPR_CALL: {
        string str   = make_string(expr->Call.callee);
        s64    count = expr->Call.args->count;
        s64    index = 0;
        append_string(&str, "(");
        LIST_FOREACH(expr->Call.args) {
            Expr* arg = (Expr*)it->data;
            append_string(&str, expr_to_str(arg));
            if (index++ != count - 1) append_string(&str, ", ");
        }
        append_string(&str, ")");
        return str.c_str;
    }
    default: warning("%s: unhandled case %s", __func__, expr_kind_to_str(expr->kind));
    }
    return NULL;
}

Typespec* get_inferred_type_of_expr(Expr* expr) {
    switch (expr->kind) {
    case EXPR_BLOCK:
    case EXPR_BREAK:
    case EXPR_CONTINUE:
    case EXPR_DEFER: return NULL;
    case EXPR_RETURN: if(expr->Return.expr) return get_inferred_type_of_expr(expr->Return.expr);

    case EXPR_CAST: return expr->Cast.type;
    case EXPR_MACRO: return get_inferred_type_of_expr(expr->Macro.expr);
    case EXPR_NOTE: return get_inferred_type_of_expr(expr->Note.expr);
    case EXPR_INT: return make_typespec_int(DEFAULT_INT_BYTE_SIZE, 0);
    case EXPR_FLOAT: return make_typespec_float(DEFAULT_FLOAT_BYTE_SIZE);
    case EXPR_STRING: return make_typespec_pointer(make_typespec_int(8, 1));
    case EXPR_IDENT: return get_symbol(expr->Ident.name);
    case EXPR_CALL: return get_symbol(expr->Call.callee)->Function.ret_type;
    case EXPR_UNARY: return get_inferred_type_of_expr(expr->Unary.operand);
    case EXPR_BINARY: return get_inferred_type_of_expr(expr->Binary.rhs);
    case EXPR_VARIABLE_DECL: return expr->Variable_Decl.type;
    case EXPR_FUNCTION: return expr->Function.type->Function.ret_type;
    case EXPR_STRUCT: return expr->Struct.type;
    case EXPR_GROUPING: return get_inferred_type_of_expr(expr->Grouping.expr);
    case EXPR_SUBSCRIPT: return get_inferred_type_of_expr(expr->Subscript.load);
    default: error("%s has no type", expr_kind_to_str(expr->kind));
    }
    return NULL;
}

Expr* get_arg_from_func(Typespec* func_t, s64 arg_index) {
    assert(func_t);
    assert(func_t->kind == TYPESPEC_FUNCTION);
    assert(arg_index >= 0 && arg_index <= typespec_function_get_arg_count(func_t));
    Expr* expr = (Expr*)list_at(func_t->Function.args, arg_index);
    assert(expr);
    return expr;
}

// @HACK
bool  last_was_true = false;
Expr* constant_fold_expr(Expr* expr) {
    assert(expr);

    info("constant_fold_expr %s: %s", expr_kind_to_str(expr->kind), expr_to_str(expr));

    switch (expr->kind) {
    case EXPR_CONTINUE: return expr;
    case EXPR_BREAK: return expr;
    case EXPR_ASM: return expr;
    case EXPR_INT: return expr;
    case EXPR_STRING: return expr;
    case EXPR_FLOAT: return expr;
    case EXPR_NOTE: return expr;
    case EXPR_DEFER: {
        expr->Defer.expr = constant_fold_expr(expr->Defer.expr);
    } break;
    case EXPR_RETURN: {
        if (expr->Return.expr) {
            expr->Return.expr = constant_fold_expr(expr->Return.expr);
        }
    } break;
    case EXPR_MACRO: {
        expr->Macro.expr = constant_fold_expr(expr->Macro.expr);
    } break;
    case EXPR_IDENT: {
        Expr* macro_expr = get_macro_def(expr->Ident.name);
        if (macro_expr) {
            expr = constant_fold_expr(macro_expr);
        }
    } break;
    case EXPR_CALL: {
        LIST_FOREACH(expr->Call.args) {
            Expr* arg = (Expr*)it->data;
            it->data  = constant_fold_expr(arg);
        }
    } break;
    case EXPR_UNARY: {
        Expr* oper = expr->Unary.operand;
        oper       = constant_fold_expr(oper);
        if (oper->kind == EXPR_INT) {
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
            info("folded %s into %lld", expr_to_str(expr), value);
            expr = make_expr_int(value);
        }
    } break;
    case EXPR_BINARY: {
        Token_Kind op  = expr->Binary.op;
        Expr*      lhs = expr->Binary.lhs;
        Expr*      rhs = expr->Binary.rhs;
        lhs            = constant_fold_expr(lhs);
        rhs            = constant_fold_expr(rhs);
        if (op == TOKEN_EQ) expr = make_expr_binary(TOKEN_EQ, lhs, rhs);
        if (lhs->kind == EXPR_INT && rhs->kind == EXPR_INT) {
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
            case TOKEN_COLON: {
                value         = last_was_true ? lhs_v : rhs_v;
                last_was_true = false;
            } break;
            case TOKEN_QUESTION_MARK: {
                if (lhs_v) {
                    last_was_true = true;
                    value         = rhs_v;
                }
            } break;
            default: error("constant_fold_expr binary %s not implemented", token_kind_to_str(op));
            }
            info("folded %s into %lld", expr_to_str(expr), value);
            expr = make_expr_int(value);
        }
    } break;
    case EXPR_VARIABLE_DECL: {
        if (expr->Variable_Decl.value) expr->Variable_Decl.value = constant_fold_expr(expr->Variable_Decl.value);
    } break;
    case EXPR_FUNCTION: {
        return constant_fold_expr(expr->Function.body);
    } break;
    case EXPR_BLOCK: {
        LIST_FOREACH(expr->Block.stmts) {
            Expr* stmt = (Expr*)it->data;
            it->data   = constant_fold_expr(stmt);
        }
    } break;
    case EXPR_GROUPING: {
        expr->Grouping.expr = constant_fold_expr(expr->Grouping.expr);
    } break;
    case EXPR_CAST: {
        expr = constant_fold_expr(expr->Cast.expr);
    } break;
    case EXPR_SUBSCRIPT: {
        expr->Subscript.load = constant_fold_expr(expr->Subscript.load);
        expr->Subscript.sub  = constant_fold_expr(expr->Subscript.sub);
    } break;
    case EXPR_IF: {
        expr->If.cond       = constant_fold_expr(expr->If.cond);
        expr->If.then_block = constant_fold_expr(expr->If.then_block);
    } break;
    case EXPR_FOR: {
        expr->For.init       = constant_fold_expr(expr->For.init);
        expr->For.cond       = constant_fold_expr(expr->For.cond);
        expr->For.step       = constant_fold_expr(expr->For.step);
        expr->For.then_block = constant_fold_expr(expr->For.then_block);
    } break;
    case EXPR_WHILE: {
        expr->While.cond       = constant_fold_expr(expr->While.cond);
        expr->While.then_block = constant_fold_expr(expr->While.then_block);
    } break;
    default: error("constant_fold_expr %s not implemented", expr_kind_to_str(expr->kind));
    }
    return expr;
}

void print_ast(List* ast) {
    info("Printing AST..");
    LIST_FOREACH(ast) { 
        Expr* expr = it->data;
        char* str = strf("%s", wrap_with_colored_parens(expr_to_str(expr)));
        info("%s", str); 
    }
}

//------------------------------------------------------------------------------
//                               Expr Maker Functions
//------------------------------------------------------------------------------

Expr* make_expr(Expr_Kind kind) {
    Expr* e = xmalloc(sizeof(Expr));
    e->kind = kind;
    return e;
}

Expr* make_expr_asm(char* str) {
    assert(str);
    Expr* e    = make_expr(EXPR_ASM);
    e->Asm.str = str;
    return e;
}

Expr* make_expr_macro(char* name, Expr* expr) {
    assert(name);
    assert(expr);
    Expr* e       = make_expr(EXPR_MACRO);
    e->Macro.name = name;
    e->Macro.expr = expr;
    return e;
}

Expr* make_expr_note(Expr* expr) {
    assert(expr);
    Expr* e      = make_expr(EXPR_NOTE);
    e->Note.expr = expr;
    return e;
}

Expr* make_expr_int(s64 value) {
    Expr* e    = make_expr(EXPR_INT);
    e->Int.val = value;
    return e;
}

Expr* make_expr_float(f64 value) {
    Expr* e      = make_expr(EXPR_FLOAT);
    e->Float.val = value;
    return e;
}

Expr* make_expr_string(char* value) {
    Expr* e       = make_expr(EXPR_STRING);
    e->String.val = value;
    return e;
}

Expr* make_expr_ident(char* ident) {
    assert(ident);
    Expr* e       = make_expr(EXPR_IDENT);
    e->Ident.name = ident;
    return e;
}

Expr* make_expr_struct(Typespec* struct_t) {
    assert(struct_t);
    Expr* e        = make_expr(EXPR_STRUCT);
    e->Struct.type = struct_t;
    return e;
}
Expr* make_expr_function(Typespec* func_t, Expr* body) {
    assert(func_t);
    assert(func_t->kind == TYPESPEC_FUNCTION);
    Expr* e            = make_expr(EXPR_FUNCTION);
    e->Function.type   = func_t;
    e->Function.body   = body;
    e->Function.defers = make_list();
    return e;
}

Expr* make_expr_binary(Token_Kind op, Expr* lhs, Expr* rhs) {
    assert(op != TOKEN_UNKNOWN);
    assert(lhs);
    assert(rhs);
    Expr* e       = make_expr(EXPR_BINARY);
    e->Binary.op  = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

Expr* make_expr_unary(Token_Kind op, Expr* operand) {
    assert(op != TOKEN_UNKNOWN);
    assert(operand);
    Expr* e          = make_expr(EXPR_UNARY);
    e->Unary.op      = op;
    e->Unary.operand = operand;
    return e;
}

Expr* make_expr_block(List* stmts) {
    Expr* e        = make_expr(EXPR_BLOCK);
    e->Block.stmts = stmts;
    return e;
}

Expr* make_expr_call(char* callee, List* args) {
    assert(callee);
    assert(args);
    Expr* e        = make_expr(EXPR_CALL);
    e->Call.callee = callee;
    e->Call.args   = args;
    return e;
}

Expr* make_expr_grouping(Expr* expr) {
    assert(expr);
    Expr* e          = make_expr(EXPR_GROUPING);
    e->Grouping.expr = expr;
    return e;
}

Expr* make_expr_variable_decl(char* name, Typespec* type, Expr* value) {
    // 'value' and 'name' can be NULL
    assert(type);
    Expr* e                = make_expr(EXPR_VARIABLE_DECL);
    e->Variable_Decl.name  = name;
    e->Variable_Decl.type  = type;
    e->Variable_Decl.value = value;
    return e;
}

Expr* make_expr_subscript(Expr* load, Expr* sub) {
    assert(load);
    assert(sub);
    Expr* e           = make_expr(EXPR_SUBSCRIPT);
    e->Subscript.load = load;
    e->Subscript.sub  = sub;
    return e;
}

Expr* make_expr_if(Expr* cond, Expr* then_block, Expr* else_block) {
    assert(cond);
    assert(then_block);
    Expr* e          = make_expr(EXPR_IF);
    e->If.cond       = cond;
    e->If.then_block = then_block;
    e->If.else_block = else_block;
    return e;
}
Expr* make_expr_for(Expr* init, Expr* cond, Expr* step, Expr* then_block) {
    assert(init);
    assert(cond);
    assert(step);
    assert(then_block);
    Expr* e           = make_expr(EXPR_FOR);
    e->For.init       = init;
    e->For.cond       = cond;
    e->For.step       = step;
    e->For.then_block = then_block;
    return e;
}
Expr* make_expr_while(Expr* cond, Expr* then_block) {
    assert(cond);
    assert(then_block);
    Expr* e             = make_expr(EXPR_WHILE);
    e->While.cond       = cond;
    e->While.then_block = then_block;
    return e;
}

Expr* make_expr_return(Expr* expr) {
    assert(expr);
    Expr* e        = make_expr(EXPR_RETURN);
    e->Return.expr = expr;
    return e;
}

Expr* make_expr_defer(Expr* expr) {
    assert(expr);
    Expr* e       = make_expr(EXPR_DEFER);
    e->Defer.expr = expr;
    return e;
}
Expr* make_expr_break() {
    Expr* e = make_expr(EXPR_BREAK);
    return e;
}

Expr* make_expr_continue() {
    Expr* e = make_expr(EXPR_CONTINUE);
    return e;
}

Expr* make_expr_cast(Expr* expr, Typespec* type) {
    assert(expr);
    assert(type);
    Expr* e      = make_expr(EXPR_CAST);
    e->Cast.type = type;
    e->Cast.expr = expr;
    return e;
}
