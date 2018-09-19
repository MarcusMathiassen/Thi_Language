#include "codegen.h"
#include "ast.h"
#include "context.h" // Context
#include "globals.h"
#include "lexer.h"           // token_kind_to_str
#include "stretchy_buffer.h" // sb_free
#include "string.h"          // string
#include "typedefs.h"
#include "utility.h" // error warning info, etc
#include "value.h"   // Value, Scope
#include <assert.h>  // assert

Typespec *integer_literal_type = NULL;
Context *ctx = NULL;
string *output = NULL;
Scope *scope = NULL;

static Value *codegen_expr(Expr *expr);
static int get_rax_reg_of_byte_size(u8 bytes);
static int get_parameter_reg(i8 i, i8 size);
static int get_next_available_reg(i8 size);
static int get_reg_as_another_size(int reg, i8 size);

#define REG_COUNT 68
const char *reg[REG_COUNT] = {
    "RAX", "EAX",  "AX",   "AL",   "AH",  "RCX",  "ECX",  "CX",
    "CL",  "CH",   "RDX",  "EDX",  "DX",  "DL",   "DH",   "RBX",
    "EBX", "BX",   "BL",   "BH",

    "RSP", "ESP",  "SP",   "SPL",  "RBP", "EBP",  "BP",   "BPL",
    "RSI", "ESI",  "SI",   "SIL",  "RDI", "EDI",  "DI",   "DIL",

    "R8",  "R8D",  "R8W",  "R8B",  "R9",  "R9D",  "R9W",  "R9B",
    "R10", "R10D", "R10W", "R10B", "R11", "R11D", "R11W", "R11B",
    "R12", "R12D", "R12W", "R12B", "R13", "R13D", "R13W", "R13B",
    "R14", "R14D", "R14W", "R14B", "R15", "R15D", "R15W", "R15B",
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

static int get_rax_reg_of_byte_size(u8 bytes) {
  switch (bytes) {
  case 1:
    return AL;
  case 2:
    return AX;
  case 4:
    return EAX;
  case 8:
    return RAX;
  default:
    error("get_rax_reg_of_byte_size unhandled case: %d", bytes);
  }
  return -1; // to silence warning
}

static int get_parameter_reg(i8 i, i8 size) {
  switch (i) {
  case 0:
    switch (size) {
    case 8:
      return RDI;
    case 4:
      return EDI;
    case 2:
      return DI;
    case 1:
      return DIL;
    }
  case 1:
    switch (size) {
    case 8:
      return RSI;
    case 4:
      return ESI;
    case 2:
      return SI;
    case 1:
      return SIL;
    }
  case 2:
    switch (size) {
    case 8:
      return RDX;
    case 4:
      return EDX;
    case 2:
      return DX;
    case 1:
      return DL;
    }
  case 3:
    switch (size) {
    case 8:
      return RCX;
    case 4:
      return ECX;
    case 2:
      return CX;
    case 1:
      return CL;
    }
  case 4:
    switch (size) {
    case 8:
      return R8;
    case 4:
      return R8D;
    case 2:
      return R8W;
    case 1:
      return R8B;
    }
  case 5:
    switch (size) {
    case 8:
      return R9;
    case 4:
      return R9D;
    case 2:
      return R9W;
    case 1:
      return R9B;
    }
  default:
    error("we only support upto 6 parameters per function.");
  }
  return -1; // to silence warning
};

static int next_available_reg_index = 0;
static int get_next_available_reg(i8 size) {
  int res = -1;
  switch (next_available_reg_index) {
  case 0:
    switch (size) {
    case 8:
      res = R10;
      break;
    case 4:
      res = R10D;
      break;
    case 2:
      res = R10W;
      break;
    case 1:
      res = R10B;
      break;
    }
    break;
  case 1:
    switch (size) {
    case 8:
      res = R11;
      break;
    case 4:
      res = R11D;
      break;
    case 2:
      res = R11W;
      break;
    case 1:
      res = R11B;
      break;
    }
    break;
  case 2:
    switch (size) {
    case 8:
      res = R12;
      break;
    case 4:
      res = R12D;
      break;
    case 2:
      res = R12W;
      break;
    case 1:
      res = R12B;
      break;
    }
    break;
  case 3:
    switch (size) {
    case 8:
      res = R13;
      break;
    case 4:
      res = R13D;
      break;
    case 2:
      res = R13W;
      break;
    case 1:
      res = R13B;
      break;
    }
    break;
  case 4:
    switch (size) {
    case 8:
      res = R14;
      break;
    case 4:
      res = R14D;
      break;
    case 2:
      res = R14W;
      break;
    case 1:
      res = R14B;
      break;
    }
    break;
  case 5:
    switch (size) {
    case 8:
      res = R15;
      break;
    case 4:
      res = R15D;
      break;
    case 2:
      res = R15W;
      break;
    case 1:
      res = R15B;
      break;
    }
    break;
  default:
    error("get_next_available_reg unhandled register: %d",
          next_available_reg_index);
  }

  if (next_available_reg_index == 5)
    next_available_reg_index = 0;

  ++next_available_reg_index;

  function_push_reg(ctx->current_function, res);

  return res;
};

static int get_reg_as_another_size(int reg, i8 size) {
  switch (reg) {
  case R10:
  case R10D:
  case R10W:
  case R10B:
    switch (size) {
    case 8:
      return R10;
    case 4:
      return R10D;
    case 2:
      return R10W;
    case 1:
      return R10B;
    }
  case R11:
  case R11D:
  case R11W:
  case R11B:
    switch (size) {
    case 8:
      return R11;
    case 4:
      return R11D;
    case 2:
      return R11W;
    case 1:
      return R11B;
    }
  case R12:
  case R12D:
  case R12W:
  case R12B:
    switch (size) {
    case 8:
      return R12;
    case 4:
      return R12D;
    case 2:
      return R12W;
    case 1:
      return R12B;
    }
  case R13:
  case R13D:
  case R13W:
  case R13B:
    switch (size) {
    case 8:
      return R13;
    case 4:
      return R13D;
    case 2:
      return R13W;
    case 1:
      return R13B;
    }
  case R14:
  case R14D:
  case R14W:
  case R14B:
    switch (size) {
    case 8:
      return R14;
    case 4:
      return R14D;
    case 2:
      return R14W;
    case 1:
      return R14B;
    }
  case R15:
  case R15D:
  case R15W:
  case R15B:
    switch (size) {
    case 8:
      return R15;
    case 4:
      return R15D;
    case 2:
      return R15W;
    case 1:
      return R15B;
    }
  }
  return -1; // to silence warning
}

static int get_push_or_popable_reg(int reg) {
  switch (reg) {
  case R10:
  case R10B:
  case R10D:
  case R10W:
    return R10;
  case R11:
  case R11D:
  case R11W:
  case R11B:
    return R11;
  case R12:
  case R12D:
  case R12W:
  case R12B:
    return R12;
  case R13:
  case R13D:
  case R13W:
  case R13B:
    return R13;
  case R14:
  case R14D:
  case R14W:
  case R14B:
    return R14;
  case R15:
  case R15D:
  case R15W:
  case R15B:
    return R15;
  default:
    error("get_push_or_popable_reg unhandled register.");
  }
  return -1; // to silence warning
}

static void append_variable_to_scope(Scope *s, Value *value) {
  assert(s);
  assert(value);
  // allocate more space if needed
  if (s->alloc_count < s->count + 1) {
    s->alloc_count += 1;
    s->local_variables =
        xrealloc(s->local_variables, sizeof(Value *) * s->alloc_count);
  }
  s->local_variables[s->count++] = value;
}

static void print_scope(Scope *scope) {
  info("Scope count %d", scope->count);
  info("Scope alloc_count %d", scope->alloc_count);
  for (u64 i = 0; i < scope->count; ++i) {
    info("Scope index: %llu variable: %s", i,
         scope->local_variables[i]->Variable.name);
  }
}

/// Returns the value, or NULL if not found.
Value *get_variable_in_scope(Scope *scope, const char *name) {
  assert(scope);
  assert(name);
  u64 variable_count = scope->count;
  for (u64 i = 0; i < variable_count; ++i) {
    Value *v = scope->local_variables[i];
    if (v->Variable.name, name)
      return v;
  }
  return NULL;
}

void add_variable_to_scope(Scope *scope, Value *variable) {
  assert(variable);
  assert(variable->kind == VALUE_VARIABLE);

  const char *name = variable->Variable.name;

  // Check for redeclaration
  Value *res = get_variable_in_scope(scope, name);
  if (res)
    error("redeclaration of variable %s", name);

  info("Added variable %s", name);
  append_variable_to_scope(scope, variable);
  print_scope(scope);
}

void remove_variable_in_scope(Scope *scope, const char *name) {
  Value *v = get_variable_in_scope(scope, name);
  if (v) {
    info("removing value: %s", name);
    // Swap this element with the last element;
    Value *temp = scope->local_variables[scope->count];
    scope->local_variables[scope->count] = v;
    v = temp;
    free(v);
    --scope->count;
    return;
  }
  error("Trying to remove unknown variable: %s", name);
}

static void emit_store(Value *variable) {
  assert(variable->kind == VALUE_VARIABLE);
  u64 size = get_size_of_value(variable);
  int reg_n = get_rax_reg_of_byte_size(size);
  u64 stack_pos = get_stack_pos_of_variable(variable);
  emit(output, "MOV [RSP-%d], %s", stack_pos, reg[reg_n]);
}

static void emit_load(Value *value) {
  int reg_n = get_rax_reg_of_byte_size(get_size_of_value(value));
  switch (value->kind) {
  case VALUE_INT: {
    emit(output, "MOV %s, %d", reg[reg_n], value->Int.value);
  } break;

  case VALUE_VARIABLE: {
    emit(output, "MOV %s, [RSP-%d]", reg[reg_n],
         get_stack_pos_of_variable(value));
  } break;

  case VALUE_FUNCTION: {
    error("VALUE_FUNCTION EMIT_LOAD NOT IMPLEMENETED");
  } break;
  }
}

static Value *codegen_function(Expr *expr) {
  const char *func_name = expr->Function.type->Function.name;
  Value *function = make_value_function(expr->Function.type);
  ctx->current_function = function;

  emit(output, "%s:", func_name);

  // Allocate stack for parameters
  u64 index = 0;
  u64 stack_before_func = ctx->stack.index;

  Arg *args = expr->Function.type->Function.args;
  int arg_count = sb_count(args);
  if (arg_count)
    info("Printing function parameters");

  u64 temp_stack_index = ctx->stack.index;
  for (int i = 0; i < arg_count; ++i) {
    Arg *arg = &args[i];

    u64 size = get_size_of_typespec(arg->type);
    u64 stack_pos = temp_stack_index + size;
    Value *var = make_value_variable(arg->name, arg->type, stack_pos);

    add_variable_to_scope(scope, var);
    emit(output, "MOV [RSP-%d], %s", stack_pos,
         reg[get_parameter_reg(index, size)]);

    temp_stack_index += size;
    ++index;
  }

  ctx->stack.index = temp_stack_index;
  u64 stack_used = temp_stack_index - stack_before_func;
  function->Function.stack_allocated = stack_used;

  if (stack_used)
    emit(output, "SUB RSP, %d", stack_used);

  codegen_expr(expr->Function.body);

  return function;
}

static Value *codegen_ident(Expr *expr) {
  const char *name = expr->Ident.name;
  Value *var = get_variable_in_scope(scope, name);
  assert(var);
  emit_load(var);
  return var;
}

static Value *codegen_int(Expr *expr) {
  Value *val = make_value_int(DEFAULT_INTEGER_BYTE_SIZE, integer_literal_type,
                              expr->Int.val);
  emit_load(val);
  return val;
}

static Value *codegen_block(Expr *expr) {
  Expr **stmts = expr->Block.stmts;
  u64 stmts_count = sb_count(stmts);
  Value *last = NULL;
  for (u64 i = 0; i < stmts_count; ++i) {
    last = codegen_expr(stmts[i]);
  }
  return last;
}

static Value *codegen_unary(Expr *expr) {
  Token_Kind op = expr->Unary.op;
  Expr *operand = expr->Unary.operand;

  Value *operand_val = codegen_expr(operand);
  int reg_n = get_rax_reg_of_byte_size(get_size_of_value(operand_val));
  Value *result = operand_val;

  switch (op) {
  case THI_SYNTAX_ADDRESS:
    error("AST_Unary '*' not implemented");
    break;
  case THI_SYNTAX_POINTER:
    error("AST_Unary '&' not implemented");
    break;
  case TOKEN_BANG: {
    emit(output, "CMP %s, 0", reg[reg_n]);
    emit(output, "SETE AL");
    break;
  }
  case TOKEN_MINUS: {
    emit(output, "NEG %s", reg[reg_n]);
    break;
  }
  default:
    error("unhandled unary case: %c", token_kind_to_str(op));
    break;
  }
  return result;
}

static Value *codegen_binary(Expr *expr) {
  Token_Kind op = expr->Binary.op;
  Expr *lhs = expr->Binary.lhs;
  Expr *rhs = expr->Binary.rhs;

  switch (op) {
  case TOKEN_EQ: {
    Value *lhs_val = codegen_expr(lhs);
    if (lhs_val->kind != VALUE_VARIABLE)
      error("lhs of an assignment must be a variable.");
    Value *rhs_val = codegen_expr(rhs);
    Value *variable = get_variable_in_scope(scope, lhs->Variable_Decl.name);
    emit_store(variable);
    return rhs_val;
  }
  case TOKEN_PLUS: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int res_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[res_n]);
    codegen_expr(rhs);
    emit(output, "ADD %s, %s", reg[res_n], reg[temp_reg_n]);
    return lhs_val;
  }
  case TOKEN_MINUS: {
    Value *rhs_val = codegen_expr(rhs);
    u64 rhs_size = get_size_of_value(rhs_val);
    int temp_reg_n = get_next_available_reg(rhs_size);
    // add_used_reg(temp_reg_n);
    int reg_n = get_rax_reg_of_byte_size(rhs_size);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    Value *lhs_val = codegen_expr(lhs);
    emit(output, "SUB %s, %s", reg[reg_n], reg[temp_reg_n]);
    return lhs_val;
  }
  case TOKEN_ASTERISK: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int res_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[res_n]);
    codegen_expr(rhs);
    emit(output, "IMUL %s, %s", reg[res_n], reg[temp_reg_n]);
    return lhs_val;
  }
  case TOKEN_FWSLASH: {
    Value *rhs_val = codegen_expr(rhs);
    u64 rhs_size = get_size_of_value(rhs_val);
    int reg_n = get_rax_reg_of_byte_size(rhs_size);
    int temp_reg_n = get_next_available_reg(rhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(lhs);
    emit(output, "CDQ");
    emit(output, "IDIV %s", reg[temp_reg_n]);
    return rhs_val;
  }

  case TOKEN_AND_AND: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    int temp_lower_byte = get_reg_as_another_size(temp_reg_n, 1);
    emit(output, "CMP %s, 0", reg[temp_reg_n]);
    emit(output, "SETNE %s", reg[temp_lower_byte]);
    emit(output, "CMP %s, 0", reg[reg_n]);
    emit(output, "SETNE AL");
    emit(output, "AND %s, AL", reg[temp_lower_byte]);
    return lhs_val;
  }

  case TOKEN_PIPE_PIPE: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "OR %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETNE AL");
    return lhs_val;
  }

  case TOKEN_LT: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "CMP %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETL AL");
    return lhs_val;
  }

  case TOKEN_GT: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "CMP %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETG AL");
    return lhs_val;
  }

  case TOKEN_LT_EQ: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "CMP %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETLE AL");
    return lhs_val;
  }

  case TOKEN_GT_EQ: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "CMP %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETGE AL");
    return lhs_val;
  }

  case TOKEN_EQ_EQ: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "CMP %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETE AL");
    return lhs_val;
  }

  case TOKEN_BANG_EQ: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    int reg_n = get_rax_reg_of_byte_size(lhs_size);
    int temp_reg_n = get_next_available_reg(lhs_size);
    // add_used_reg(temp_reg_n);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);
    codegen_expr(rhs);
    emit(output, "CMP %s, %s", reg[temp_reg_n], reg[reg_n]);
    emit(output, "SETNE AL");
    return lhs_val;
  }

  case TOKEN_PLUS_EQ: {
    Value *variable = codegen_expr(lhs);
    if (variable->kind != VALUE_VARIABLE)
      error("lhs of += must be a variable.");

    Value *rhs_val = codegen_expr(rhs);
    u64 rhs_size = get_size_of_value(rhs_val);
    u64 reg_n = get_rax_reg_of_byte_size(rhs_size);
    u64 stack_pos = get_stack_pos_of_variable(variable);
    emit(output, "ADD [RSP-%d], %s", stack_pos, reg[reg_n]);

    return variable;
  }

  case TOKEN_MINUS_EQ: {
    Value *rhs_val = codegen_expr(rhs);
    u64 rhs_size = get_size_of_value(rhs_val);
    u64 reg_n = get_rax_reg_of_byte_size(rhs_size);

    int temp_reg_n = get_next_available_reg(rhs_size);
    // curr_func->add_used_reg(temp_reg.reg);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);

    Value *variable = codegen_expr(lhs);
    if (variable->kind != VALUE_VARIABLE)
      error("lhs of -= must be a variable.");

    emit(output, "SUB %s, %s", reg[reg_n], reg[temp_reg_n]);
    emit_store(variable);
    return variable;
  }

  case TOKEN_ASTERISK_EQ: {
    Value *variable = codegen_expr(lhs);
    if (variable->kind != VALUE_VARIABLE)
      error("lhs of += must be a variable.");

    Value *rhs_val = codegen_expr(rhs);
    u64 rhs_size = get_size_of_value(rhs_val);
    u64 reg_n = get_rax_reg_of_byte_size(rhs_size);
    u64 stack_pos = get_stack_pos_of_variable(variable);
    emit(output, "IMUL %s, [RSP-%d]", reg[reg_n], stack_pos);
    emit_store(variable);
    return variable;
  }
  case TOKEN_PERCENT_EQ:
    error("percent_eq not implemented.");
  case TOKEN_PIPE_EQ:
    error("pipe_eq not implemented.");
  case TOKEN_HAT_EQ:
    error("hat_eq not implemented.");
  case TOKEN_BITWISE_LEFTSHIFT:
    error("bitwise_leftshift not implemented.");
  case TOKEN_BITWISE_RIGHTSHIFT:
    error("bitwise_rightshift not implemented.");

  case TOKEN_FWSLASH_EQ: {
    Value *rhs_val = codegen_expr(rhs);
    u64 rhs_size = get_size_of_value(rhs_val);
    u64 reg_n = get_rax_reg_of_byte_size(rhs_size);

    int temp_reg_n = get_next_available_reg(rhs_size);
    // curr_func->add_used_reg(temp_reg.reg);
    emit(output, "MOV %s, %s", reg[temp_reg_n], reg[reg_n]);

    Value *variable = codegen_expr(lhs);
    if (variable->kind != VALUE_VARIABLE)
      error("lhs of /= must be a variable.");

    emit(output, "CDQ");
    emit(output, "IDIV %s", reg[temp_reg_n]);
    emit_store(variable);
    return variable;
  }

  case TOKEN_AT:
    error("at not implemented.");
  case TOKEN_HAT:
    error("hat not implemented.");
  case TOKEN_PIPE:
    error("pipe not implemented.");

  // Ternary operator
  case TOKEN_QUESTION_MARK: {
    Value *lhs_val = codegen_expr(lhs);
    u64 lhs_size = get_size_of_value(lhs_val);
    u64 reg_n = get_rax_reg_of_byte_size(lhs_size);
    emit(output, "CMP %s, 0", reg[reg_n]);
    ctx_pop_label(ctx);
    emit(output, "JE %s", ctx_get_unique_label(ctx, "E3"));
    Value *rhs_val = codegen_expr(rhs);
    emit(output, "JMP %s", ctx_get_unique_label(ctx, "CONTINUE"));
    return rhs_val;
  }
  case TOKEN_COLON: {
    ctx_push_label(ctx);
    codegen_expr(lhs);
    emit(output, "%s:", ctx_get_unique_label(ctx, "E3"));
    Value *rhs_val = codegen_expr(rhs);
    emit(output, "%s:", ctx_get_unique_label(ctx, "CONTINUE"));
    return rhs_val;
  }
  }

  error("Codegen: Unhandled binary op %s", token_kind_to_str(op));
  return NULL;
}

static Value *codegen_variable_decl_type_inf(Expr *expr) {
  const char *name = expr->Variable_Decl_Type_Inf.name;
  Expr *assignment_expr = expr->Variable_Decl_Type_Inf.value;

  Value *assign_expr_val =
      codegen_expr(assignment_expr); // Any value this creates is stored in RAX
  Typespec *type = assign_expr_val->type;
  u64 type_size = get_size_of_typespec(type);
  u64 stack_pos = type_size + ctx->stack.index;

  Value *variable = make_value_variable(name, type, stack_pos);
  add_variable_to_scope(scope, variable);
  emit_store(variable); // The variable is set to whatevers in RAX
  ctx->stack.index += type_size;

  return variable;
}
static Value *codegen_variable_decl(Expr *expr) {
  const char *name = expr->Variable_Decl.name;
  Typespec *type = expr->Variable_Decl.type;
  Expr *assignment_expr = expr->Variable_Decl.value;

  if (assignment_expr)
    codegen_expr(assignment_expr); // Any value this creates is stored in RAX

  u64 type_size = get_size_of_typespec(type);
  u64 stack_pos = type_size + ctx->stack.index;
  Value *variable = make_value_variable(name, type, stack_pos);
  add_variable_to_scope(scope, variable);

  emit_store(variable); // The variable is set to whatevers in RAX
  ctx->stack.index += type_size;

  return variable;
}

static Value *codegen_ret(Expr *expr) {
  Expr *ret_expr = expr->Ret.expr;
  Value *ret_val = codegen_expr(ret_expr);

  // Pop off regs
  //    u64* regs = ctx->current_function->Function.regs_used;
  //    u64 regs_count = ctx->current_function->Function.regs_used_count;
  //    for (int i = 0; i < regs_count; ++i)
  //    {
  //        int reg_n = get_push_or_popable_reg(regs[i]);
  //        emit(output, "POP %s", reg[reg_n]);
  //    }
  //
  //    // Deallocate stack used by the function
  //    u64 stack_used = ctx->current_function->Function.stack_allocated;
  //    if (stack_used)
  //    {
  //        emit(output, "ADD RSP, %llu", stack_used);
  //    }

  emit(output, "RET");
  return ret_val;
}

static Value *codegen_expr(Expr *expr) {
  info("Generating code for: %s", expr_to_str(expr));
  switch (expr->kind) {
  case EXPR_NOTE:
    error("EXPR_NOTE codegen not implemented");
    break;
  case EXPR_INT:
    return codegen_int(expr);
  case EXPR_FLOAT:
    error("EXPR_FLOAT codegen not implemented");
  case EXPR_IDENT:
    return codegen_ident(expr);
  case EXPR_CALL:
    error("EXPR_CALL codegen not implemented");
  case EXPR_UNARY:
    return codegen_unary(expr);
  case EXPR_BINARY:
    return codegen_binary(expr);
  case EXPR_COMPOUND:
    error("EXPR_COMPOUND codegen not implemented");
  case EXPR_RET:
    return codegen_ret(expr);
    break;
  case EXPR_VARIABLE_DECL:
    return codegen_variable_decl(expr);
  case EXPR_VARIABLE_DECL_TYPE_INF:
    return codegen_variable_decl_type_inf(expr);
  case EXPR_FUNCTION:
    return codegen_function(expr);
    break;
  case EXPR_STRUCT:
    error("EXPR_STRUCT codegen not implemented");
    break;
  case EXPR_IF:
    error("EXPR_IF codegen not implemented");
  case EXPR_FOR:
    error("EXPR_FOR codegen not implemented");
  case EXPR_BLOCK:
    return codegen_block(expr);
    break;
  case EXPR_WHILE:
    error("EXPR_WHILE codegen not implemented");
  case EXPR_GROUPING:
    return codegen_expr(expr->Grouping.expr);
  }

  return NULL;
}

char *generate_code_from_ast(AST **ast) {
  success("Generating X64 Assembly from AST");

  integer_literal_type = make_typespec_int(DEFAULT_INTEGER_BIT_SIZE, 0);

  ctx = ctx_make();
  scope = make_scope(10);
  output = make_string("", 10000);

  emit(output, "global main");
  emit(output, "section .text");

  // Codegen AST
  u64 ast_count = sb_count(ast);
  info("ast_count: %d", ast_count);
  for (u64 i = 0; i < ast_count; ++i) {
    codegen_expr(ast[i]);
  }

  return output->data;
}
