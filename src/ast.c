#include "ast.h"
#include <assert.h>  // assert
#include "globals.h"
#include "lexer.h"    // token_kind_to_str,
#include "string.h"   // strf, append_string, string
#include "utility.h"  // info, success, error, warning, xmalloc, xrealloc

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* expr_kind_to_str(AST_Kind kind) {
    switch (kind) {
        case AST_EXTERN: return "AST_EXTERN";
        case AST_LOAD: return "AST_LOAD";
        case AST_LINK: return "AST_LINK";
        case AST_MACRO: return "AST_MACRO";
        case AST_NOTE: return "AST_NOTE";
        case AST_INT: return "AST_INT";
        case AST_FLOAT: return "AST_FLOAT";
        case AST_STRING: return "AST_STRING";
        case AST_IDENT: return "AST_IDENT";
        case AST_CALL: return "AST_CALL";
        case AST_UNARY: return "AST_UNARY";
        case AST_BINARY: return "AST_BINARY";
        case AST_VARIABLE_DECL: return "AST_VARIABLE_DECL";
        case AST_CONSTANT_DECL: return "AST_CONSTANT_DECL";
        case AST_FUNCTION: return "AST_FUNCTION";
        case AST_STRUCT: return "AST_STRUCT";
        case AST_ENUM: return "AST_ENUM";
        case AST_BLOCK: return "AST_BLOCK";
        case AST_GROUPING: return "AST_GROUPING";
        case AST_SUBSCRIPT: return "AST_SUBSCRIPT";
        case AST_IF: return "AST_IF";
        case AST_FOR: return "AST_FOR";
        case AST_WHILE: return "AST_WHILE";
        case AST_RETURN: return "AST_RETURN";
        case AST_DEFER: return "AST_DEFER";
        case AST_BREAK: return "AST_BREAK";
        case AST_CONTINUE: return "AST_CONTINUE";
        case AST_CAST: return "AST_CAST";
        default: warning("expr_kind_to_str unhandled case '%d'", kind);
    }
    return NULL;
}

char* ast_to_str(AST* expr) {
    if (!expr) return "NULL";
    switch (expr->kind) {
        case AST_EXTERN: return strf("extern %s", ast_to_str(expr->Extern.node));
        case AST_LOAD: return strf("load %s", ast_to_str(expr->Load.node));
        case AST_LINK: return strf("link %s", ast_to_str(expr->Link.node));
        case AST_DEFER: return strf("defer %s", ast_to_str(expr->Defer.expr));
        case AST_BREAK: return "break";
        case AST_CONTINUE: return "continue";
        case AST_CAST: return strf("cast(%s, %s)", type_to_str(expr->Cast.type), ast_to_str(expr->Cast.expr));
        case AST_MACRO: {
            return strf("%s :: %s", expr->Macro.name, ast_to_str(expr->Macro.expr));
        }
        case AST_RETURN: {
            if (expr->Return.expr) {
                return strf("return %s", ast_to_str(expr->Return.expr));
            } else {
                return strf("return");
            }
        }
        case AST_NOTE: {
            return strf("$%s", ast_to_str(expr->Note.expr));
        }
        case AST_INT: {
            return strf("%lld", expr->Int.val);
        }
        case AST_FLOAT: {
            return strf("%f", expr->Float.val);
        }
        case AST_STRING: {
            return strf("\"%s\"", expr->String.val);
        }
        case AST_IDENT: {
            return strf("%s", expr->Ident.name);
        }
        case AST_UNARY: {
            return strf("%s(%s)", token_kind_to_str(expr->Unary.op), ast_to_str(expr->Unary.operand));
        }
        case AST_BINARY: {
            return strf("%s %s %s", ast_to_str(expr->Binary.lhs), token_kind_to_str(expr->Binary.op),
                        ast_to_str(expr->Binary.rhs));
        }
        case AST_VARIABLE_DECL: {
            if (expr->Variable_Decl.value) {
                if (expr->Variable_Decl.name) {
                    return strf("%s: %s = %s", expr->Variable_Decl.name, type_to_str(expr->Variable_Decl.type),
                                ast_to_str(expr->Variable_Decl.value));
                } else {
                    return strf("%s", type_to_str(expr->Variable_Decl.type));
                }
            } else {
                if (expr->Variable_Decl.name) {
                    return strf("%s: %s", expr->Variable_Decl.name, type_to_str(expr->Variable_Decl.type));
                } else {
                    return strf("%s", type_to_str(expr->Variable_Decl.type));
                }
            }
        }
        case AST_CONSTANT_DECL: {
            return strf("%s %s", expr->Constant_Decl.name, ast_to_str(expr->Constant_Decl.value));
        }
        case AST_BLOCK: {
            string str = make_string("{");
            LIST_FOREACH(expr->Block.stmts) {
                AST* stmt = (AST*)it->data;
                append_string_f(&str, "%s\n", ast_to_str(stmt));
            }
            append_string(&str, "}");
            return str.c_str;
        }
        case AST_STRUCT: {
            return strf("%s", type_to_str(expr->Struct.type));
        }
        case AST_ENUM: {
            return strf("%s", type_to_str(expr->Enum.type));
        }
        case AST_FUNCTION: {
            string str = make_string_f("%s %s", type_to_str(expr->Function.type), ast_to_str(expr->Function.body));
            return str.c_str;
        }
        case AST_GROUPING: return strf("(%s)", ast_to_str(expr->Grouping.expr));
        case AST_SUBSCRIPT: return strf("%s[%s]", ast_to_str(expr->Subscript.load), ast_to_str(expr->Subscript.sub));
        case AST_IF: {
            if (expr->If.else_block) {
                return strf("if %s %s else %s", ast_to_str(expr->If.cond), ast_to_str(expr->If.then_block),
                            ast_to_str(expr->If.else_block));
            } else {
                return strf("if %s %s", ast_to_str(expr->If.cond), ast_to_str(expr->If.then_block));
            }
        }
        case AST_FOR: {
            return strf("for %s, %s, %s %s", ast_to_str(expr->For.init), ast_to_str(expr->For.cond),
                        ast_to_str(expr->For.step), ast_to_str(expr->For.then_block));
        }
        case AST_WHILE: {
            return strf("while %s %s", ast_to_str(expr->While.cond), ast_to_str(expr->While.then_block));
        }
        case AST_CALL: {
            string str   = make_string(expr->Call.callee);
            s64    count = expr->Call.args->count;
            s64    index = 0;
            append_string(&str, "(");
            LIST_FOREACH(expr->Call.args) {
                AST* arg = (AST*)it->data;
                append_string(&str, ast_to_str(arg));
                if (index++ != count - 1) append_string(&str, ", ");
            }
            append_string(&str, ")");
            return str.c_str;
        }
        default: warning("%s: unhandled case %s", __func__, expr_kind_to_str(expr->kind));
    }
    return NULL;
}

char* ast_to_json(AST* expr) {
    if (!expr) return "\"NULL\"";
    char* result = NULL;
    switch (expr->kind) {
        case AST_EXTERN: {
            result = strf("{\"%s\": {\"extern\": %s}}", expr_kind_to_str(expr->kind), ast_to_str(expr->Extern.node));
        } break;
        case AST_LOAD: {
            result = strf("{\"%s\": {\"load\": %s}}", expr_kind_to_str(expr->kind), ast_to_str(expr->Load.node));
        } break;
        case AST_LINK: {
            result = strf("{\"%s\": {\"link\": %s}}", expr_kind_to_str(expr->kind), ast_to_str(expr->Link.node));
        } break;
        case AST_CONTINUE: {
            result = strf("{\"%s\": {%s}}", expr_kind_to_str(expr->kind), "continue");
        } break;
        case AST_BREAK: {
            result = strf("{\"%s\": {%s}}", expr_kind_to_str(expr->kind), "break");
        } break;
        case AST_MACRO: {
            result = strf("{\"%s\": {\"name\": \"%s\", \"expr\": \"%s\"}}", expr_kind_to_str(expr->kind),
                          expr->Macro.name, ast_to_json(expr));
        } break;
        case AST_DEFER: {
            result = strf("{\"%s\": {\"expr\": %s}}", expr_kind_to_str(expr->kind), ast_to_json(expr->Defer.expr));
        } break;
        case AST_NOTE: {
            result = strf("{\"%s\": {\"note\":\"%s\"}}", expr_kind_to_str(expr->kind), ast_to_json(expr->Note.expr));
        } break;
        case AST_INT: {
            result = strf("{\"%s\": {\"value\": %lld}}", expr_kind_to_str(expr->kind), expr->Int.val);
        } break;
        case AST_STRING: {
            result = strf("{\"%s\": {\"value\": \"%s\"}}", expr_kind_to_str(expr->kind), expr->String.val);
        } break;
        case AST_FLOAT: {
            result = strf("{\"%s\": {\"value\": %f}}", expr_kind_to_str(expr->kind), expr->Float.val);
        } break;
        case AST_IDENT: {
            result = strf("{\"%s\": {\"ident\": \"%s\"}}", expr_kind_to_str(expr->kind), expr->Ident.name);
        } break;
        case AST_UNARY: {
            result = strf("{\"%s\": {\"op\": \"%s\", \"expr\": \"%s\"}}", expr_kind_to_str(expr->kind),
                          token_kind_to_str(expr->Unary.op), ast_to_json(expr->Unary.operand));
        } break;
        case AST_BINARY: {
            result =
                strf("{\"%s\": {\"op\": \"%s\", \"lhs\": %s, \"rhs\": %s}}", expr_kind_to_str(expr->kind),
                     token_kind_to_str(expr->Binary.op), ast_to_json(expr->Binary.lhs), ast_to_json(expr->Binary.rhs));
        } break;
        case AST_RETURN: {
            result = strf("{\"%s\": {\"expr\": %s}}", expr_kind_to_str(expr->kind), ast_to_json(expr->Return.expr));
        } break;
        case AST_VARIABLE_DECL: {
            result = strf("{\"%s\": {\"name\": \"%s\", \"type\": \"%s\", \"value\": %s}}", expr_kind_to_str(expr->kind),
                          expr->Variable_Decl.name, type_to_str(expr->Variable_Decl.type),
                          ast_to_json(expr->Variable_Decl.value));
        } break;
        case AST_CONSTANT_DECL: {
            result = strf("{\"%s\": {\"name\": \"%s\", \"value\": \"%s\"}}", expr_kind_to_str(expr->kind),
                          expr->Constant_Decl.name, ast_to_json(expr->Constant_Decl.value));
        } break;
        case AST_BLOCK: {
            s64    block_count = expr->Block.stmts->count;
            s64    counter     = 0;
            string str         = make_string("{\"AST_BLOCK\": [");
            LIST_FOREACH(expr->Block.stmts) {
                append_string(&str, ast_to_json(it->data));
                if (counter != block_count - 1) append_string(&str, ", ");
                block_count += 1;
            }
            append_string(&str, "]}");
            result = str.c_str;
        } break;
        case AST_FUNCTION: {
            result = strf("{\"%s\": {\"type\": \"%s\", \"body\": %s }}", expr_kind_to_str(expr->kind),
                          type_to_str(expr->Function.type), ast_to_json(expr->Function.body));
        } break;
        case AST_STRUCT: {
            result = strf("{\"%s\": {\"type\": \"%s\"}}", expr_kind_to_str(expr->kind), type_to_str(expr->Struct.type));
        } break;
        case AST_ENUM: {
            result = strf("{\"%s\": {\"type\": \"%s\"}}", expr_kind_to_str(expr->kind), type_to_str(expr->Enum.type));
        } break;
        case AST_GROUPING: {
            result = strf("{\"%s\": {\"expr\": %s}}", expr_kind_to_str(expr->kind), ast_to_json(expr->Grouping.expr));
        } break;
        case AST_WHILE: {
            result = strf("{\"%s\": {\"cond\": %s, \"then_block\": %s}}", expr_kind_to_str(expr->kind),
                          ast_to_json(expr->While.cond), ast_to_json(expr->While.then_block));
        } break;
        case AST_FOR: {
            result = strf("{\"%s\": {\"init\": %s, \"cond\": %s, \"step\": %s, \"then_block\": %s }}",
                          expr_kind_to_str(expr->kind), ast_to_json(expr->For.init), ast_to_json(expr->For.cond),
                          ast_to_json(expr->For.step), ast_to_json(expr->For.then_block));
        } break;
        case AST_IF: {
            result =
                strf("{\"%s\": {\"cond\": %s, \"then_block\": %s, \"else_block\": %s }}", expr_kind_to_str(expr->kind),
                     ast_to_json(expr->If.cond), ast_to_json(expr->If.then_block), ast_to_json(expr->If.else_block));
        } break;
        case AST_CALL: {
            string str = make_string("");
            append_string_f(&str, "{\"%s\": {\"callee\": \"%s\", ", expr_kind_to_str(expr->kind), expr->Call.callee);
            append_string(&str, "\"args\": [");
            s64 arg_count = expr->Call.args->count;
            s64 counter   = 0;
            LIST_FOREACH(expr->Call.args) {
                append_string(&str, ast_to_json(it->data));
                if (counter != arg_count - 1) append_string(&str, ", ");
                counter += 1;
            }
            append_string(&str, "]}}");
            result = str.c_str;
        } break;
        default: warning("%s: unhandled case %s", __func__, expr_kind_to_str(expr->kind));
    }
    assert(result);
    return result;
}

Type* get_inferred_type_of_expr(AST* expr) {
    switch (expr->kind) {
        case AST_EXTERN:
        case AST_LOAD:
        case AST_LINK:
        case AST_BLOCK:
        case AST_BREAK:
        case AST_CONTINUE:
        case AST_DEFER: return NULL;
        case AST_RETURN:
            if (expr->Return.expr) return get_inferred_type_of_expr(expr->Return.expr);

        case AST_CAST: return expr->Cast.type;
        case AST_MACRO: return get_inferred_type_of_expr(expr->Macro.expr);
        case AST_NOTE: return get_inferred_type_of_expr(expr->Note.expr);
        case AST_INT: return make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
        case AST_FLOAT: return make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
        case AST_STRING: return make_type_pointer(make_type_int(8, 1));
        case AST_IDENT: return get_symbol(expr->Ident.name);
        case AST_CALL: return get_symbol(expr->Call.callee)->Function.ret_type;
        case AST_UNARY: return get_inferred_type_of_expr(expr->Unary.operand);
        case AST_BINARY: return get_inferred_type_of_expr(expr->Binary.rhs);
        case AST_VARIABLE_DECL: return expr->Variable_Decl.type;
        case AST_FUNCTION: return expr->Function.type->Function.ret_type;
        case AST_STRUCT: return expr->Struct.type;
        case AST_GROUPING: return get_inferred_type_of_expr(expr->Grouping.expr);
        case AST_SUBSCRIPT: return get_inferred_type_of_expr(expr->Subscript.load);
        default: error("%s has no type", expr_kind_to_str(expr->kind));
    }
    return NULL;
}

AST* get_arg_from_func(Type* func_t, s64 arg_index) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    assert(arg_index >= 0 && arg_index <= type_function_get_arg_count(func_t));
    AST* expr = (AST*)list_at(func_t->Function.args, arg_index);
    assert(expr);
    return expr;
}

// @HACK
bool last_was_true = false;
AST* constant_fold_expr(AST* expr) {
    assert(expr);

    info("constant_fold_expr %s: %s", expr_kind_to_str(expr->kind), ast_to_str(expr));

    switch (expr->kind) {
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
            expr->Defer.expr = constant_fold_expr(expr->Defer.expr);
        } break;
        case AST_RETURN: {
            if (expr->Return.expr) {
                expr->Return.expr = constant_fold_expr(expr->Return.expr);
            }
        } break;
        case AST_MACRO: {
            expr->Macro.expr = constant_fold_expr(expr->Macro.expr);
        } break;
        case AST_IDENT: {
            AST* macro_expr = get_macro_def(expr->Ident.name);
            if (macro_expr) {
                expr = constant_fold_expr(macro_expr);
            }
        } break;
        case AST_CALL: {
            LIST_FOREACH(expr->Call.args) {
                AST* arg = (AST*)it->data;
                it->data = constant_fold_expr(arg);
            }
        } break;
        case AST_UNARY: {
            AST* oper = expr->Unary.operand;
            oper      = constant_fold_expr(oper);
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
            lhs            = constant_fold_expr(lhs);
            rhs            = constant_fold_expr(rhs);
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
                info("folded %s into %lld", ast_to_str(expr), value);
                expr = make_ast_int(value);
            }
        } break;
        case AST_VARIABLE_DECL: {
            if (expr->Variable_Decl.value) expr->Variable_Decl.value = constant_fold_expr(expr->Variable_Decl.value);
        } break;
        case AST_FUNCTION: {
            return constant_fold_expr(expr->Function.body);
        } break;
        case AST_BLOCK: {
            LIST_FOREACH(expr->Block.stmts) {
                AST* stmt = (AST*)it->data;
                it->data  = constant_fold_expr(stmt);
            }
        } break;
        case AST_GROUPING: {
            expr->Grouping.expr = constant_fold_expr(expr->Grouping.expr);
        } break;
        case AST_CAST: {
            expr = constant_fold_expr(expr->Cast.expr);
        } break;
        case AST_SUBSCRIPT: {
            expr->Subscript.load = constant_fold_expr(expr->Subscript.load);
            expr->Subscript.sub  = constant_fold_expr(expr->Subscript.sub);
        } break;
        case AST_IF: {
            expr->If.cond       = constant_fold_expr(expr->If.cond);
            expr->If.then_block = constant_fold_expr(expr->If.then_block);
        } break;
        case AST_FOR: {
            expr->For.init       = constant_fold_expr(expr->For.init);
            expr->For.cond       = constant_fold_expr(expr->For.cond);
            expr->For.step       = constant_fold_expr(expr->For.step);
            expr->For.then_block = constant_fold_expr(expr->For.then_block);
        } break;
        case AST_WHILE: {
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
        AST*  expr = it->data;
        char* str  = strf("%s", wrap_with_colored_parens(ast_to_str(expr)));
        info("%s", str);
    }
}

void print_ast_json(List* ast) {
    string json      = make_string("{\"AST\": [");
    s64    ast_count = ast->count;
    s64    counter   = 0;
    LIST_FOREACH(ast) {
        append_string(&json, ast_to_json(it->data));
        if (counter != ast_count - 1) append_string(&json, ",");
        counter += 1;
    }
    append_string(&json, "]}");
    info(json.c_str);
}

//------------------------------------------------------------------------------
//                               AST* Maker Functions
//------------------------------------------------------------------------------

AST* make_ast(AST_Kind kind) {
    AST* e  = xmalloc(sizeof(AST));
    e->kind = kind;
    return e;
}

AST* make_ast_extern(AST* node) {
    assert(node);
    AST* e       = make_ast(AST_EXTERN);
    e->Extern.node = node;
    return e;
}
AST* make_ast_load(AST* node) {
    assert(node);
    AST* e       = make_ast(AST_LOAD);
    e->Load.node = node;
    return e;
}
AST* make_ast_link(AST* node) {
    assert(node);
    AST* e       = make_ast(AST_LINK);
    e->Link.node = node;
    return e;
}

AST* make_ast_macro(char* name, AST* expr) {
    assert(name);
    assert(expr);
    AST* e        = make_ast(AST_MACRO);
    e->Macro.name = name;
    e->Macro.expr = expr;
    return e;
}

AST* make_ast_note(AST* expr) {
    assert(expr);
    AST* e       = make_ast(AST_NOTE);
    e->Note.expr = expr;
    return e;
}

AST* make_ast_int(s64 value) {
    AST* e     = make_ast(AST_INT);
    e->Int.val = value;
    return e;
}

AST* make_ast_float(f64 value) {
    AST* e       = make_ast(AST_FLOAT);
    e->Float.val = value;
    return e;
}

AST* make_ast_string(char* value) {
    AST* e        = make_ast(AST_STRING);
    e->String.val = value;
    return e;
}

AST* make_ast_ident(char* ident) {
    assert(ident);
    AST* e        = make_ast(AST_IDENT);
    e->Ident.name = ident;
    return e;
}

AST* make_ast_struct(Type* struct_t) {
    assert(struct_t);
    AST* e         = make_ast(AST_STRUCT);
    e->Struct.type = struct_t;
    return e;
}

AST* make_ast_enum(Type* enum_t) {
    assert(enum_t);
    AST* e       = make_ast(AST_ENUM);
    e->Enum.type = enum_t;
    return e;
}

AST* make_ast_function(Type* func_t, AST* body) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    AST* e             = make_ast(AST_FUNCTION);
    e->Function.type   = func_t;
    e->Function.body   = body;
    e->Function.defers = make_list();
    return e;
}

AST* make_ast_binary(Token_Kind op, AST* lhs, AST* rhs) {
    assert(op != TOKEN_UNKNOWN);
    assert(lhs);
    assert(rhs);
    AST* e        = make_ast(AST_BINARY);
    e->Binary.op  = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

AST* make_ast_unary(Token_Kind op, AST* operand) {
    assert(op != TOKEN_UNKNOWN);
    assert(operand);
    AST* e           = make_ast(AST_UNARY);
    e->Unary.op      = op;
    e->Unary.operand = operand;
    return e;
}

AST* make_ast_block(List* stmts) {
    AST* e         = make_ast(AST_BLOCK);
    e->Block.stmts = stmts;
    return e;
}

AST* make_ast_call(char* callee, List* args) {
    assert(callee);
    assert(args);
    AST* e         = make_ast(AST_CALL);
    e->Call.callee = callee;
    e->Call.args   = args;
    return e;
}

AST* make_ast_grouping(AST* expr) {
    assert(expr);
    AST* e           = make_ast(AST_GROUPING);
    e->Grouping.expr = expr;
    return e;
}

AST* make_ast_variable_decl(char* name, Type* type, AST* value) {
    // 'value' and 'name' can be NULL
    assert(type);
    AST* e                 = make_ast(AST_VARIABLE_DECL);
    e->Variable_Decl.name  = name;
    e->Variable_Decl.type  = type;
    e->Variable_Decl.value = value;
    return e;
}

AST* make_ast_constant_decl(char* name, AST* value) {
    assert(name);
    assert(value);
    AST* e                 = make_ast(AST_CONSTANT_DECL);
    e->Constant_Decl.name  = name;
    e->Constant_Decl.value = value;
    return e;
}

AST* make_ast_subscript(AST* load, AST* sub) {
    assert(load);
    assert(sub);
    AST* e            = make_ast(AST_SUBSCRIPT);
    e->Subscript.load = load;
    e->Subscript.sub  = sub;
    return e;
}

AST* make_ast_if(AST* cond, AST* then_block, AST* else_block) {
    assert(cond);
    assert(then_block);
    AST* e           = make_ast(AST_IF);
    e->If.cond       = cond;
    e->If.then_block = then_block;
    e->If.else_block = else_block;
    return e;
}
AST* make_ast_for(AST* init, AST* cond, AST* step, AST* then_block) {
    assert(init);
    assert(cond);
    assert(step);
    assert(then_block);
    AST* e            = make_ast(AST_FOR);
    e->For.init       = init;
    e->For.cond       = cond;
    e->For.step       = step;
    e->For.then_block = then_block;
    return e;
}
AST* make_ast_while(AST* cond, AST* then_block) {
    assert(cond);
    assert(then_block);
    AST* e              = make_ast(AST_WHILE);
    e->While.cond       = cond;
    e->While.then_block = then_block;
    return e;
}

AST* make_ast_return(AST* expr) {
    // assert(expr);
    AST* e         = make_ast(AST_RETURN);
    e->Return.expr = expr;
    return e;
}

AST* make_ast_defer(AST* expr) {
    assert(expr);
    AST* e        = make_ast(AST_DEFER);
    e->Defer.expr = expr;
    return e;
}
AST* make_ast_break() {
    AST* e = make_ast(AST_BREAK);
    return e;
}

AST* make_ast_continue() {
    AST* e = make_ast(AST_CONTINUE);
    return e;
}

AST* make_ast_cast(AST* expr, Type* type) {
    assert(expr);
    assert(type);
    AST* e       = make_ast(AST_CAST);
    e->Cast.type = type;
    e->Cast.expr = expr;
    return e;
}
