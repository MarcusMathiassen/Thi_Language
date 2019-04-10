#include "ast.h"
#include "constants.h"
#include "lexer.h"   // token_kind_to_str,
#include "string.h"  // strf, append_string, string
#include "utility.h" // info, success, error, warning, xmalloc, xrealloc
#include <assert.h>  // assert
#include <string.h>  // strlen

//------------------------------------------------------------------------------
//                               ast.c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                               Public
//------------------------------------------------------------------------------

char* ast_kind_to_str(AST_Kind kind) {
    switch (kind) {
    case AST_VAR_ARGS: return "AST_VAR_ARGS";
    case AST_FIELD_ACCESS: return "AST_FIELD_ACCESS";
    case AST_FALLTHROUGH: return "AST_FALLTHROUGH";
    case AST_SWITCH: return "AST_SWITCH";
    case AST_IS: return "AST_IS";
    case AST_SIZEOF: return "AST_SIZEOF";
    case AST_EXTERN: return "AST_EXTERN";
    case AST_LOAD: return "AST_LOAD";
    case AST_LINK: return "AST_LINK";
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
    default: warning("ast_kind_to_str unhandled case '%d'", kind);
    }
    return NULL;
}

char* ast_to_str(AST* expr) {
    if (!expr) return "---";
    switch (expr->kind) {
    case AST_VAR_ARGS: return "...";
    case AST_FALLTHROUGH: return "fallthrough";
    case AST_SWITCH:
        return strf("if %s %s else %s", ast_to_str(expr->Switch.cond), ast_to_str(expr->Switch.cases),
                    ast_to_str(expr->Switch.default_case));
    case AST_IS: return strf("is %s %s", ast_to_str(expr->Is.expr), ast_to_str(expr->Is.body));
    case AST_SIZEOF: return strf("sizeof %s", type_to_str(expr->type));
    case AST_EXTERN: return strf("extern %s", type_to_str(expr->type));
    case AST_LOAD: return strf("load %s", expr->Load.str);
    case AST_LINK: return strf("link %s", expr->Link.str);
    case AST_DEFER: return strf("defer %s", ast_to_str(expr->Defer.expr));
    case AST_BREAK: return "break";
    case AST_CONTINUE: return "continue";
    case AST_CAST: return strf("cast(%s, %s)", type_to_str(expr->type), ast_to_str(expr->Cast.expr));
    case AST_RETURN: {
        return strf("return %s", ast_to_str(expr->Return.expr));
    }
    case AST_FIELD_ACCESS: {
        return strf("%s.%s", ast_to_str(expr->Field_Access.load), expr->Field_Access.field);
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
        return strf("%s: %s = %s", expr->Variable_Decl.name, type_to_str(expr->type),
                    ast_to_str(expr->Variable_Decl.value));
    }
    case AST_CONSTANT_DECL: {
        return strf("%s :: %s", expr->Constant_Decl.name, ast_to_str(expr->Constant_Decl.value));
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
        return type_to_str(expr->Struct.type);
    }
    case AST_ENUM: {
        return type_to_str(expr->Enum.type);
    }
    case AST_FUNCTION: {
        string str = make_string_f("%s %s", type_to_str(expr->type), ast_to_str(expr->Function.body));
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
    default: warning("%s: unhandled case %s", __func__, ast_kind_to_str(expr->kind));
    }
    return NULL;
}

char* ast_to_json(AST* expr) {
    if (!expr) return "\"NULL\"";
    char* result = NULL;
    switch (expr->kind) {
    case AST_SWITCH: {
        result = strf("{\"%s\": {\"cond\":%s, \"cases\", \"default\"}}", ast_kind_to_str(expr->kind),
                      ast_to_json(expr->Switch.cond), ast_to_json(expr->Switch.cases),
                      ast_to_json(expr->Switch.default_case));
    } break;
    case AST_IS: {
        result =
            strf("{\"%s\": {\"case\": %s, \"body\": %s, \"has_fallthrough\": %s}}", ast_kind_to_str(expr->kind),
                 ast_to_json(expr->Is.expr), ast_to_json(expr->Is.body), expr->Is.has_fallthrough ? "true" : "false");
    } break;
    case AST_FIELD_ACCESS: {
        result = strf("{\"%s\": {\"load\": %s, \"field\": %s}}", ast_kind_to_str(expr->kind),
                      ast_to_json(expr->Field_Access.load), expr->Field_Access.field);
    } break;
    case AST_SIZEOF: {
        result = strf("{\"%s\": {\"sizeof\": %s}}", ast_kind_to_str(expr->kind), type_to_str(expr->type));
    } break;
    case AST_EXTERN: {
        result = strf("{\"%s\": {\"extern\": \"%s\"}}", ast_kind_to_str(expr->kind), type_to_str(expr->type));
    } break;
    case AST_LOAD: {
        result = strf("{\"%s\": {\"load\": %s}}", ast_kind_to_str(expr->kind), expr->Load.str);
    } break;
    case AST_VAR_ARGS: {
        result = strf("{\"%s\": {\"var_args\": ...}}", ast_kind_to_str(expr->kind));
    } break;
    case AST_LINK: {
        result = strf("{\"%s\": {\"link\": %s}}", ast_kind_to_str(expr->kind), expr->Link.str);
    } break;
    case AST_SUBSCRIPT: {
        result = strf("{\"%s\": {\"load\": %s, \"sub\": %s}}", ast_kind_to_str(expr->kind),
                      ast_to_json(expr->Subscript.load), ast_to_json(expr->Subscript.sub));
    } break;
    case AST_CONTINUE: {
        result = strf("{\"%s\": {%s}}", ast_kind_to_str(expr->kind), "continue");
    } break;
    case AST_FALLTHROUGH: {
        result = strf("{\"%s\": {%s}}", ast_kind_to_str(expr->kind), "fallthrough");
    } break;
    case AST_BREAK: {
        result = strf("{\"%s\": {%s}}", ast_kind_to_str(expr->kind), "break");
    } break;
    case AST_DEFER: {
        result = strf("{\"%s\": {\"expr\": %s}}", ast_kind_to_str(expr->kind), ast_to_json(expr->Defer.expr));
    } break;
    case AST_NOTE: {
        result = strf("{\"%s\": {\"note\":\"%s\"}}", ast_kind_to_str(expr->kind), ast_to_json(expr->Note.expr));
    } break;
    case AST_INT: {
        result = strf("{\"%s\": {\"value\": %lld}}", ast_kind_to_str(expr->kind), expr->Int.val);
    } break;
    case AST_STRING: {
        result = strf("{\"%s\": {\"value\": \"%s\"}}", ast_kind_to_str(expr->kind), expr->String.val);
    } break;
    case AST_FLOAT: {
        result = strf("{\"%s\": {\"value\": %f}}", ast_kind_to_str(expr->kind), expr->Float.val);
    } break;
    case AST_IDENT: {
        result = strf("{\"%s\": {\"ident\": \"%s\"}}", ast_kind_to_str(expr->kind), expr->Ident.name);
    } break;
    case AST_UNARY: {
        result = strf("{\"%s\": {\"op\": \"%s\", \"expr\": \"%s\"}}", ast_kind_to_str(expr->kind),
                      token_kind_to_str(expr->Unary.op), ast_to_json(expr->Unary.operand));
    } break;
    case AST_BINARY: {
        result = strf("{\"%s\": {\"op\": %s, \"lhs\": %s, \"rhs\": %s}}", ast_kind_to_str(expr->kind),
                      token_kind_to_str(expr->Binary.op), ast_to_json(expr->Binary.lhs), ast_to_json(expr->Binary.rhs));
    } break;
    case AST_RETURN: {
        result = strf("{\"%s\": {\"expr\": %s}}", ast_kind_to_str(expr->kind), ast_to_json(expr->Return.expr));
    } break;
    case AST_VARIABLE_DECL: {
        result = strf("{\"%s\": {\"name\": \"%s\", \"type\": \"%s\", \"value\": %s}}", ast_kind_to_str(expr->kind),
                      expr->Variable_Decl.name, type_to_str(expr->type), ast_to_json(expr->Variable_Decl.value));
    } break;
    case AST_CONSTANT_DECL: {
        result = strf("{\"%s\": {\"name\": \"%s\", \"value\": \"%s\"}}", ast_kind_to_str(expr->kind),
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
        result = strf("{\"%s\": {\"type\": \"%s\", \"body\": %s }}", ast_kind_to_str(expr->kind),
                      type_to_str(expr->type), ast_to_json(expr->Function.body));
    } break;
    case AST_STRUCT: {
        result = strf("{\"%s\": {\"type\": \"%s\"}}", ast_kind_to_str(expr->kind), type_to_str(expr->type));
    } break;
    case AST_ENUM: {
        result = strf("{\"%s\": {\"type\": \"%s\"}}", ast_kind_to_str(expr->kind), type_to_str(expr->type));
    } break;
    case AST_GROUPING: {
        result = strf("{\"%s\": {\"expr\": %s}}", ast_kind_to_str(expr->kind), ast_to_json(expr->Grouping.expr));
    } break;
    case AST_WHILE: {
        result = strf("{\"%s\": {\"cond\": %s, \"then_block\": %s}}", ast_kind_to_str(expr->kind),
                      ast_to_json(expr->While.cond), ast_to_json(expr->While.then_block));
    } break;
    case AST_FOR: {
        result = strf("{\"%s\": {\"init\": %s, \"cond\": %s, \"step\": %s, \"then_block\": %s }}",
                      ast_kind_to_str(expr->kind), ast_to_json(expr->For.init), ast_to_json(expr->For.cond),
                      ast_to_json(expr->For.step), ast_to_json(expr->For.then_block));
    } break;
    case AST_IF: {
        result = strf("{\"%s\": {\"cond\": %s, \"then_block\": %s, \"else_block\": %s }}", ast_kind_to_str(expr->kind),
                      ast_to_json(expr->If.cond), ast_to_json(expr->If.then_block), ast_to_json(expr->If.else_block));
    } break;
    case AST_CALL: {
        string str = make_string("");
        append_string_f(&str, "{\"%s\": {\"callee\": \"%s\", ", ast_kind_to_str(expr->kind), expr->Call.callee);
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
    default: warning("%s: unhandled case %s", __func__, ast_kind_to_str(expr->kind));
    }
    assert(result);
    return result;
}

AST* get_arg_from_func(Type* func_t, s64 arg_index) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    assert(arg_index >= 0 && arg_index <= type_function_get_arg_count(func_t));
    AST* expr = (AST*)list_at(func_t->Function.args, arg_index);
    assert(expr);
    return expr;
}

void print_ast(List* ast) {
    warning("Printing AST..");
    LIST_FOREACH(ast) {
        AST*  expr = it->data;
        char* str  = strf("%s", wrap_with_colored_parens(ast_to_str(expr)));
        info("%s", str);
    }
}

void print_ast_json(List* ast) {
    warning("Printing AST as JSON..");
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

AST_Ref_List make_ast_ref_list() {
    AST_Ref_List l;
    l.count     = 0;
    l.allocated = AST_REF_LIST_STARTING_ALLOC;
    l.data      = xmalloc(l.allocated * sizeof(AST*));
    return l;
}

void ast_ref_list_append(AST_Ref_List* l, AST* a) {
    if (l->count >= l->allocated) {
        l->allocated *= 2;
        l->data = xrealloc(l->data, l->allocated * sizeof(AST*));
    }
    l->data[l->count] = a;
    l->count += 1;
}

//------------------------------------------------------------------------------
//                               AST* Maker Functions
//------------------------------------------------------------------------------

AST* make_ast(AST_Kind kind, Token t) {
    AST* e  = xmalloc(sizeof(AST));
    e->kind = kind;
    e->t    = t;
    return e;
}

AST* make_ast_sizeof(Token t, Type* type) {
    assert(type);
    AST* e         = make_ast(AST_SIZEOF, t);
    e->type        = type;
    e->Sizeof.type = type;
    return e;
}

AST* make_ast_field_access(Token t, AST* load, char* field) {
    assert(load);
    assert(field);
    AST* e                = make_ast(AST_FIELD_ACCESS, t);
    e->Field_Access.load  = load;
    e->Field_Access.field = field;
    return e;
}

AST* make_ast_extern(Token t, Type* type) {
    assert(type);
    AST* e         = make_ast(AST_EXTERN, t);
    e->type        = type;
    e->Extern.type = type;
    return e;
}

AST* make_ast_load(Token t, char* str) {
    assert(str);
    AST* e      = make_ast(AST_LOAD, t);
    e->Load.str = str;
    return e;
}

AST* make_ast_link(Token t, char* str) {
    assert(str);
    AST* e      = make_ast(AST_LINK, t);
    e->Link.str = str;
    return e;
}

AST* make_ast_note(Token t, AST* expr) {
    assert(expr);
    AST* e       = make_ast(AST_NOTE, t);
    e->Note.expr = expr;
    return e;
}

AST* make_ast_int(Token t, s64 value) {
    AST* e     = make_ast(AST_INT, t);
    e->type    = make_type_int(DEFAULT_INT_BYTE_SIZE, 0);
    e->Int.val = value;
    return e;
}

AST* make_ast_float(Token t, f64 value) {
    AST* e       = make_ast(AST_FLOAT, t);
    e->type      = make_type_float(DEFAULT_FLOAT_BYTE_SIZE);
    e->Float.val = value;
    return e;
}

AST* make_ast_string(Token t, char* value) {
    AST* e        = make_ast(AST_STRING, t);
    e->type       = make_type_pointer(make_type_int(1, 1));
    e->String.val = value;
    return e;
}

AST* make_ast_ident(Token t, char* ident) {
    assert(ident);
    AST* e        = make_ast(AST_IDENT, t);
    e->Ident.name = ident;
    return e;
}

AST* make_ast_struct(Token t, Type* struct_t) {
    assert(struct_t);
    AST* e         = make_ast(AST_STRUCT, t);
    e->Struct.type = struct_t;
    return e;
}

AST* make_ast_enum(Token t, Type* enum_t) {
    assert(enum_t);
    AST* e       = make_ast(AST_ENUM, t);
    e->Enum.type = enum_t;
    return e;
}

AST* make_ast_function(Token t, Type* func_t, AST* body) {
    assert(func_t);
    assert(func_t->kind == TYPE_FUNCTION);
    AST* e             = make_ast(AST_FUNCTION, t);
    e->type            = func_t;
    e->Function.type   = func_t;
    e->Function.body   = body;
    e->Function.defers = make_list();
    return e;
}

AST* try_fold(AST* e) {
    Token_Kind op  = e->Binary.op;
    AST*       lhs = e->Binary.lhs;
    AST*       rhs = e->Binary.rhs;
    if (lhs->kind == AST_GROUPING) lhs = lhs->Grouping.expr;
    if (rhs->kind == AST_GROUPING) rhs = rhs->Grouping.expr;
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
        case TOKEN_QUESTION_MARK: return e;
        case TOKEN_COLON: return e;
        default: error("constant_fold_expr binary %s not implemented", token_kind_to_str(op));
        }
        info("folded %s into %lld", ast_to_str(e), value);

        return make_ast_int(e->t, value);
    } else if (lhs->kind == AST_FLOAT && rhs->kind == AST_FLOAT) {
        f64 lhs_v = lhs->Float.val;
        f64 rhs_v = rhs->Float.val;
        f64 value = 0.0;
        switch (op) {
        case TOKEN_EQ_EQ: value = (lhs_v == rhs_v); break;
        case TOKEN_BANG_EQ: value = (lhs_v != rhs_v); break;
        case TOKEN_PLUS: value = (lhs_v + rhs_v); break;
        case TOKEN_MINUS: value = (lhs_v - rhs_v); break;
        case TOKEN_ASTERISK: value = (lhs_v * rhs_v); break;
        case TOKEN_FWSLASH: value = (lhs_v / rhs_v); break;
        case TOKEN_LT: value = (lhs_v < rhs_v); break;
        case TOKEN_GT: value = (lhs_v > rhs_v); break;
        case TOKEN_AND_AND: value = (lhs_v && rhs_v); break;
        case TOKEN_PIPE_PIPE: value = (lhs_v || rhs_v); break;
        default: error("constant_fold_expr binary %s not implemented", token_kind_to_str(op));
        }
        info("folded %s into %lld", ast_to_str(e), value);

        return make_ast_float(e->t, value);
    }
    return e;
}

AST* make_ast_binary(Token t, Token_Kind op, AST* lhs, AST* rhs) {
    assert(op != TOKEN_UNKNOWN);
    assert(lhs);
    assert(rhs);
    AST* e        = make_ast(AST_BINARY, t);
    e->t          = t;
    e->Binary.op  = op;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    e             = try_fold(e);
    return e;
}

AST* make_ast_unary(Token t, Token_Kind op, AST* operand) {
    assert(op != TOKEN_UNKNOWN);
    assert(operand);
    AST* e           = make_ast(AST_UNARY, t);
    e->Unary.op      = op;
    e->Unary.operand = operand;

    if (operand->kind == AST_INT) {
        Token_Kind op     = e->Unary.op;
        s64        oper_v = operand->Int.val;
        s64        value  = 0;
        switch (op) {
        case TOKEN_BANG: value = !oper_v; break;
        case TOKEN_PLUS: value = oper_v; break;
        case TOKEN_TILDE: value = ~oper_v; break;
        case TOKEN_MINUS: value = -oper_v; break;
        default: error("constant_fold_expr unary %s not implemented", token_kind_to_str(op));
        }
        info("folded %s into %lld", ast_to_str(e), value);
        e = make_ast_int(t, value);
    }

    return e;
}

AST* make_ast_switch(Token t, AST* if_statement) {
    AST* e                 = make_ast(AST_SWITCH, t);
    e->Switch.cond         = if_statement->If.cond;
    e->Switch.cases        = if_statement->If.then_block;
    e->Switch.default_case = if_statement->If.else_block;
    return e;
}

AST* make_ast_is(Token t, AST* expr, AST* body, bool has_fallthrough) {
    AST* e                = make_ast(AST_IS, t);
    e->Is.expr            = expr;
    e->Is.body            = body;
    e->Is.has_fallthrough = has_fallthrough;
    return e;
}

AST* make_ast_block(Token t, List* stmts) {
    AST* e         = make_ast(AST_BLOCK, t);
    e->Block.stmts = stmts;
    return e;
}

AST* make_ast_call(Token t, char* callee, List* args) {
    assert(callee);
    assert(args);
    AST* e         = make_ast(AST_CALL, t);
    e->Call.callee = callee;
    e->Call.args   = args;
    return e;
}

AST* make_ast_var_args(Token t) {
    AST* e = make_ast(AST_VAR_ARGS, t);
    return e;
}

AST* make_ast_fallthrough(Token t) {
    AST* e = make_ast(AST_FALLTHROUGH, t);
    return e;
}

AST* make_ast_grouping(Token t, AST* expr) {
    assert(expr);
    AST* e           = make_ast(AST_GROUPING, t);
    e->Grouping.expr = expr;
    return e;
}

AST* make_ast_variable_decl(Token t, char* name, Type* type, AST* value) {
    // 'value' and 'name' can be NULL
    // assert(type);
    AST* e                 = make_ast(AST_VARIABLE_DECL, t);
    e->Variable_Decl.name  = name;
    e->type                = type;
    e->Variable_Decl.type  = type;
    e->Variable_Decl.value = value;
    return e;
}

AST* make_ast_constant_decl(Token t, char* name, AST* value) {
    assert(name);
    assert(value);
    AST* e                 = make_ast(AST_CONSTANT_DECL, t);
    e->Constant_Decl.name  = name;
    e->Constant_Decl.value = value;
    return e;
}

AST* make_ast_subscript(Token t, AST* load, AST* sub) {
    assert(load);
    assert(sub);
    AST* e            = make_ast(AST_SUBSCRIPT, t);
    e->Subscript.load = load;
    e->Subscript.sub  = sub;
    return e;
}

AST* make_ast_if(Token t, AST* cond, AST* then_block, AST* else_block) {
    assert(cond);
    assert(then_block);
    AST* e           = make_ast(AST_IF, t);
    e->If.cond       = cond;
    e->If.then_block = then_block;
    e->If.else_block = else_block;
    return e;
}

AST* make_ast_for(Token t, AST* init, AST* cond, AST* step, AST* then_block) {
    assert(init);
    assert(cond);
    assert(step);
    assert(then_block);
    AST* e            = make_ast(AST_FOR, t);
    e->For.init       = init;
    e->For.cond       = cond;
    e->For.step       = step;
    e->For.then_block = then_block;
    return e;
}

AST* make_ast_while(Token t, AST* cond, AST* then_block) {
    assert(cond);
    assert(then_block);
    AST* e              = make_ast(AST_WHILE, t);
    e->While.cond       = cond;
    e->While.then_block = then_block;
    return e;
}

AST* make_ast_return(Token t, AST* expr) {
    // assert(expr);
    AST* e         = make_ast(AST_RETURN, t);
    e->Return.expr = expr;
    return e;
}

AST* make_ast_defer(Token t, AST* expr) {
    assert(expr);
    AST* e        = make_ast(AST_DEFER, t);
    e->Defer.expr = expr;
    return e;
}

AST* make_ast_break(Token t) {
    AST* e = make_ast(AST_BREAK, t);
    return e;
}

AST* make_ast_continue(Token t) {
    AST* e = make_ast(AST_CONTINUE, t);
    return e;
}

AST* make_ast_cast(Token t, AST* expr, Type* type) {
    assert(expr);
    assert(type);
    AST* e       = make_ast(AST_CAST, t);
    e->type      = type;
    e->Cast.type = type;
    e->Cast.expr = expr;
    return e;
}
