#include "codegen.h"
#include "typedefs.h"
#include "ast.h"
#include "typedefs.h"
#include "lexer.h" // token_kind_to_str
#include "globals.h"
#include "utility.h" // error warning info, etc
#include "values.h" // Value
#include "stretchy_buffer.h"  // sb_free

#define REG_COUNT 68
const char* reg[REG_COUNT] = {
    "RAX", "EAX", "AX", "AL", "AH",
    "RCX", "ECX", "CX", "CL", "CH",
    "RDX", "EDX", "DX", "DL", "DH",
    "RBX", "EBX", "BX", "BL", "BH",

    "RSP", "ESP", "SP", "SPL",
    "RBP", "EBP", "BP", "BPL",
    "RSI", "ESI", "SI", "SIL",
    "RDI", "EDI", "DI", "DIL",

    "R8", "R8D", "R8W", "R8B",
    "R9", "R9D", "R9W", "R9B",
    "R10", "R10D", "R10W", "R10B",
    "R11", "R11D", "R11W", "R11B",
    "R12", "R12D", "R12W", "R12B",
    "R13", "R13D", "R13W", "R13B",
    "R14", "R14D", "R14W", "R14B",
    "R15", "R15D", "R15W", "R15B",
};


#define RAX 0 
#define EAX 1
#define AX 2
#define AL 3
#define AH 4

#define RCX 5
#define ECX 6
#define CX 7
#define CL 8
#define CH 9

#define RDX 10 
#define EDX 11
#define DX 12
#define DL 13
#define DH 14

#define RBX 15
#define EBX 16
#define BX 17
#define BL 18
#define BH 19

#define RSP 20
#define ESP 21
#define SP 22
#define SPL 23

#define RBP 24
#define EBP 25
#define BP 26
#define BPL 27

#define RSI 28
#define ESI 29
#define SI 30
#define SIL 31

#define RDI 32
#define EDI 33
#define DI 34
#define DIL 35

#define R8 36
#define R8D 37
#define R8W 38
#define R8B 39

#define R9 40
#define R9D 41 
#define R9W 42
#define R9B 43

#define R10 44
#define R10D 45
#define R10W 46
#define R10B 47

#define R11 48
#define R11D 49
#define R11W 50
#define R11B 51

#define R12 52
#define R12D 53
#define R12W 54
#define R12B 55

#define R13 56
#define R13D 57
#define R13W 58
#define R13B 59

#define R14 60
#define R14D 61
#define R14W 62
#define R14B 63

#define R15 64
#define R15D 65
#define R15W 66
#define R15B 67

static int get_rax_reg_of_byte_size(u8 bytes)
{
    switch (bytes)
    {
        case 1: return AL;
        case 2: return AX;
        case 4: return EAX;
        case 8: return RAX;
        default: error("get_rax_reg_of_byte_size unhandled case: %d", bytes);
    }
}

static int get_parameter_reg(i8 i, i8 size)
{
    switch (i)
    {
        case 0: switch (size) { case 8: return RDI; case 4: return EDI; case 2: return DI; case 1: return DIL; }
        case 1: switch (size) { case 8: return RSI; case 4: return ESI; case 2: return SI; case 1: return SIL; }
        case 2: switch (size) { case 8: return RDX; case 4: return EDX; case 2: return DX; case 1: return DL; }
        case 3: switch (size) { case 8: return RCX; case 4: return ECX; case 2: return CX; case 1: return CL; }
        case 4: switch (size) { case 8: return R8;  case 4: return R8D; case 2: return R8W; case 1: return R8B; }
        case 5: switch (size) { case 8: return R9;  case 4: return R9D; case 2: return R9W; case 1: return R9B; }
        default: error("we only support upto 6 parameters per function.");
    }
};

static int next_available_reg_index = 0;
static int  get_next_available_reg(i8 size)
{
    int res = -1;
    switch (next_available_reg_index)
    {
        case 0: switch (size) { case 8: res = R10; break; case 4: res = R10D; break; case 2: res = R10W; break; case 1: res = R10B; break; } break;
        case 1: switch (size) { case 8: res = R11; break; case 4: res = R11D; break; case 2: res = R11W; break; case 1: res = R11B; break; } break;
        case 2: switch (size) { case 8: res = R12; break; case 4: res = R12D; break; case 2: res = R12W; break; case 1: res = R12B; break; } break;
        case 3: switch (size) { case 8: res = R13; break; case 4: res = R13D; break; case 2: res = R13W; break; case 1: res = R13B; break; } break;
        case 4: switch (size) { case 8: res = R14; break; case 4: res = R14D; break; case 2: res = R14W; break; case 1: res = R14B; break; } break;
        case 5: switch (size) { case 8: res = R15; break; case 4: res = R15D; break; case 2: res = R15W; break; case 1: res = R15B; break; } break;
        default: error("get_next_available_reg unhandled register: %d", next_available_reg_index);
    }

    if (next_available_reg_index == 5)
        next_available_reg_index = 0;

    ++next_available_reg_index;
    return res;
};

static int get_reg_as_another_size(int reg, i8 size)
{
    switch(reg)
    {
        case R10: case R10D: case R10W: case R10B:  switch (size) { case 8: return R10; case 4: return R10D; case 2: return R10W; case 1: return R10B; }
        case R11: case R11D: case R11W: case R11B:  switch (size) { case 8: return R11; case 4: return R11D; case 2: return R11W; case 1: return R11B; }
        case R12: case R12D: case R12W: case R12B:  switch (size) { case 8: return R12; case 4: return R12D; case 2: return R12W; case 1: return R12B; }
        case R13: case R13D: case R13W: case R13B:  switch (size) { case 8: return R13; case 4: return R13D; case 2: return R13W; case 1: return R13B; }
        case R14: case R14D: case R14W: case R14B:  switch (size) { case 8: return R14; case 4: return R14D; case 2: return R14W; case 1: return R14B; }
        case R15: case R15D: case R15W: case R15B:  switch (size) { case 8: return R15; case 4: return R15D; case 2: return R15W; case 1: return R15B; }
    }
}

static int get_push_or_popable_reg(int reg)
{
    switch (reg)
    {
        case R10: case R10B: case R10D: case R10W: return R10; 
        case R11: case R11D: case R11W: case R11B: return R11; 
        case R12: case R12D: case R12W: case R12B: return R12; 
        case R13: case R13D: case R13W: case R13B: return R13; 
        case R14: case R14D: case R14W: case R14B: return R14; 
        case R15: case R15D: case R15W: case R15B: return R15;
        default: error("get_push_or_popable_reg unhandled register.");
    }
}

char* output = NULL;
int stack_index = 0;

static Value codegen_expr(Expr* expr);

static void emit_load(Value value)
{
    int reg_n = get_rax_reg_of_byte_size(get_size_of_value(value));
    switch (value.kind)
    {
        case VALUE_INT: emit(output, "MOV %s, %d", reg[reg_n], value.Int.value);
    }
}

static void codegen_function(Expr* expr)
{
    const char* func_name = expr->Func.type->Func.name;
    info("Generating function: %s", func_name);

    Value function = make_value_function(func_name);
    Value block = make_value_block(&function, "entry");


    emit(output, "%s:", func_name);
    codegen_expr(expr->Func.body);

    // Allocate stack for parameters
    int index = 0;
    int stack_before_func = stack_index;

    Arg* args = expr->Func.type->Func.args;
    int arg_count = sb_count(args);
    if (arg_count) info("Printing function parameters");
    for (int i = 0; i < arg_count; ++i)
    {
        Arg arg = args[i];
        info("Parameter %d name: %s", i, arg.name);
        print_type(arg.type);

        int size = get_size_of_type(arg.type);
        info("size: %d", size);
    }
}

static Value codegen_ident(Expr* expr)
{
    const char* name = expr->Ident.name;

    // Value* var = get_variable(name)
    // emit_load(var);

    // return var;
    error("EXPR_IDENT codegen not implemented");
}

static Value codegen_int(Expr* expr)
{
    Value val = make_value_int(DEFAULT_INTEGER_BYTE_SIZE, expr->Int.val);
    emit_load(val);
    return val;
}

static void codegen_block(Expr* expr)
{
    Expr** stmts = expr->Block.stmts;
    u64 stmts_count = sb_count(stmts);
    for (u64 i = 0; i < stmts_count; ++i)
    {
        codegen_expr(stmts[i]);
    }
}
static Value codegen_unary(Expr* expr)
{
    Token_Kind op = expr->Unary.op;
    Expr* operand = expr->Unary.operand;

    Value operand_val = codegen_expr(operand);
    int reg_n = get_rax_reg_of_byte_size(get_size_of_value(operand_val));
    Value result = operand_val;

    switch (op)
    {
        case THI_SYNTAX_ADDRESS: error("AST_Unary '*' not implemented"); break;
        case THI_SYNTAX_POINTER: error("AST_Unary '&' not implemented"); break;
        case TOKEN_BANG:
        {
            emit(output, "CMP %s, 0", reg[reg_n]);
            emit(output, "SETE AL");
            break; 
        }
        case TOKEN_MINUS:
        {
            emit(output, "NEG %s", reg[reg_n]);
            break;
        }
        default: error("unhandled unary case: %c", token_kind_to_str(op)); break;
    }
    return result;
}

static Value codegen_binary(Expr* expr)
{
    Token_Kind op = expr->Binary.op;
    Expr* lhs = expr->Binary.lhs;
    Expr* rhs = expr->Binary.rhs;

    switch (op)
    {
        case TOKEN_PLUS:
        {
            Value lhs_val  = codegen_expr(lhs);
            int lhs_size = get_size_of_value(lhs_val);
            int res_n  = get_rax_reg_of_byte_size(lhs_size);

            int temp_reg_n = get_next_available_reg(lhs_size);
            // add_used_reg(temp_reg_n);

            emit(output, "MOV %s, %s", reg[temp_reg_n], reg[res_n]);
            Value rhs_val = codegen_expr(rhs);
            emit(output, "ADD %s, %s", reg[res_n], reg[temp_reg_n]);
            return lhs_val;
        } break;
    }
}

static void codegen_ret(Expr* expr)
{
    Expr* ret_expr = expr->Ret.expr;
    codegen_expr(ret_expr);
    emit(output, "RET");
}

static Value codegen_expr(Expr* expr)
{
    info("Generating code for: %s", expr_kind_to_str(expr->kind));
    switch (expr->kind)
    {
        case EXPR_INT:      return codegen_int(expr);
        case EXPR_FLOAT:    error("EXPR_FLOAT codegen not implemented");
        case EXPR_IDENT:    return codegen_ident(expr);
        case EXPR_CALL:     error("EXPR_CALL codegen not implemented");
        case EXPR_UNARY:    return codegen_unary(expr);
        case EXPR_BINARY:   return codegen_binary(expr);
        case EXPR_COMPOUND: error("EXPR_COMPOUND codegen not implemented");
        case EXPR_RET:      codegen_ret(expr); break;
        case EXPR_VAR_DECL: error("EXPR_VAR_DECL codegen not implemented");
        case EXPR_FUNC:     codegen_function(expr); break;
        case EXPR_IF:       error("EXPR_IF codegen not implemented");
        case EXPR_FOR:      error("EXPR_FOR codegen not implemented");
        case EXPR_BLOCK:    codegen_block(expr); break;
        case EXPR_WHILE:    error("EXPR_WHILE codegen not implemented");
        case EXPR_GROUPING: error("EXPR_GROUPING codegen not implemented");
    }
    Value t;
    return t;
}

char* generate_code_from_ast(AST** ast)
{

    // for (int i = 0; i < REGISTERS_COUNT; ++i)
    // {
    //     if (i < 4) info("b8:%s b4:%s b2:%s b1:%s b1h:%s",
    //                 registers[i].b8.str,
    //                 registers[i].b4.str,
    //                 registers[i].b2.str,
    //                 registers[i].b1.str,
    //                 registers[i].b1h.str); 
    //     else info("b8:%s b4:%s b2:%s b1:%s",
    //                 registers[i].b8.str,
    //                 registers[i].b4.str,
    //                 registers[i].b2.str,
    //                 registers[i].b1.str);
    // }

    output = xmalloc(0);

    emit(output, "global main");
    emit(output, "section .text");
    
    // Codegen AST
    u64 ast_count = sb_count(ast);
    for (u64 i = 0; i < ast_count; ++i)
    {
        codegen_expr(ast[i]);
    }
    return output;
}
