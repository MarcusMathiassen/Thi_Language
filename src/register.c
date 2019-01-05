#include "register.h"
#include "context.h" // Context
#include "utility.h" // error

#define REG_COUNT 68
char* reg[REG_COUNT] = {
    "RAX",  "EAX",  "AX",   "AL",   "AH",   "RCX",  "ECX",  "CX",   "CL",   "CH",   "RDX",
    "EDX",  "DX",   "DL",   "DH",   "RBX",  "EBX",  "BX",   "BL",   "BH",

    "RSP",  "ESP",  "SP",   "SPL",  "RBP",  "EBP",  "BP",   "BPL",  "RSI",  "ESI",  "SI",
    "SIL",  "RDI",  "EDI",  "DI",   "DIL",

    "R8",   "R8D",  "R8W",  "R8B",  "R9",   "R9D",  "R9W",  "R9B",  "R10",  "R10D", "R10W",
    "R10B", "R11",  "R11D", "R11W", "R11B", "R12",  "R12D", "R12W", "R12B", "R13",  "R13D",
    "R13W", "R13B", "R14",  "R14D", "R14W", "R14B", "R15",  "R15D", "R15W", "R15B",
};

int get_num_registers() { return REG_COUNT; }
char* get_reg(int reg_n) { return reg[reg_n]; }
char* get_reg_fitting_value(Value* value)
{
    i64 size = get_size_of_value(value);
    i64 reg_n = get_rax_reg_of_byte_size(size);
    return reg[reg_n];
}

int get_rax_reg_of_byte_size(u8 bytes)
{
    switch (bytes) {
    case 1: return AL;
    case 2: return AX;
    case 4: return EAX;
    case 8: return RAX;
    default: warning("get_rax_reg_of_byte_size unhandled case: %d. Defaulted to RAX.", bytes);
    }
    return RAX;
}

int get_parameter_reg(i8 i, i8 size)
{
    switch (i) {
    case 0:
        switch (size) {
        case 8: return RDI;
        case 4: return EDI;
        case 2: return DI;
        case 1: return DIL;
        }
    case 1:
        switch (size) {
        case 8: return RSI;
        case 4: return ESI;
        case 2: return SI;
        case 1: return SIL;
        }
    case 2:
        switch (size) {
        case 8: return RDX;
        case 4: return EDX;
        case 2: return DX;
        case 1: return DL;
        }
    case 3:
        switch (size) {
        case 8: return RCX;
        case 4: return ECX;
        case 2: return CX;
        case 1: return CL;
        }
    case 4:
        switch (size) {
        case 8: return R8;
        case 4: return R8D;
        case 2: return R8W;
        case 1: return R8B;
        }
    case 5:
        switch (size) {
        case 8: return R9;
        case 4: return R9D;
        case 2: return R9W;
        case 1: return R9B;
        }
    default: error("we only support upto 6 parameters per function.");
    }
    return -1; // to silence warning
};

int get_next_available_reg(Context* ctx, i8 size)
{
    i32 res = -1;
    switch (ctx->next_available_reg_index) {
    case 0:
        switch (size) {
        case 8: res = R10; break;
        case 4: res = R10D; break;
        case 2: res = R10W; break;
        case 1: res = R10B; break;
        }
        break;
    case 1:
        switch (size) {
        case 8: res = R11; break;
        case 4: res = R11D; break;
        case 2: res = R11W; break;
        case 1: res = R11B; break;
        }
        break;
    case 2:
        switch (size) {
        case 8: res = R12; break;
        case 4: res = R12D; break;
        case 2: res = R12W; break;
        case 1: res = R12B; break;
        }
        break;
    case 3:
        switch (size) {
        case 8: res = R13; break;
        case 4: res = R13D; break;
        case 2: res = R13W; break;
        case 1: res = R13B; break;
        }
        break;
    case 4:
        switch (size) {
        case 8: res = R14; break;
        case 4: res = R14D; break;
        case 2: res = R14W; break;
        case 1: res = R14B; break;
        }
        break;
    case 5:
        switch (size) {
        case 8: res = R15; break;
        case 4: res = R15D; break;
        case 2: res = R15W; break;
        case 1: res = R15B; break;
        }
        break;
    }

    if (ctx->next_available_reg_index == 5) {
        ctx->next_available_reg_index = 0;
    }

    ++ctx->next_available_reg_index;

    return res;
};

int get_reg_as_another_size(int reg, i8 size)
{
    switch (reg) {
    case R10:
    case R10D:
    case R10W:
    case R10B:
        switch (size) {
        case 8: return R10;
        case 4: return R10D;
        case 2: return R10W;
        case 1: return R10B;
        }
    case R11:
    case R11D:
    case R11W:
    case R11B:
        switch (size) {
        case 8: return R11;
        case 4: return R11D;
        case 2: return R11W;
        case 1: return R11B;
        }
    case R12:
    case R12D:
    case R12W:
    case R12B:
        switch (size) {
        case 8: return R12;
        case 4: return R12D;
        case 2: return R12W;
        case 1: return R12B;
        }
    case R13:
    case R13D:
    case R13W:
    case R13B:
        switch (size) {
        case 8: return R13;
        case 4: return R13D;
        case 2: return R13W;
        case 1: return R13B;
        }
    case R14:
    case R14D:
    case R14W:
    case R14B:
        switch (size) {
        case 8: return R14;
        case 4: return R14D;
        case 2: return R14W;
        case 1: return R14B;
        }
    case R15:
    case R15D:
    case R15W:
    case R15B:
        switch (size) {
        case 8: return R15;
        case 4: return R15D;
        case 2: return R15W;
        case 1: return R15B;
        }
    }
    return -1; // to silence warning
}

int get_push_or_popable_reg(int reg)
{
    switch (reg) {
    case R10:
    case R10B:
    case R10D:
    case R10W: return R10;
    case R11:
    case R11D:
    case R11W:
    case R11B: return R11;
    case R12:
    case R12D:
    case R12W:
    case R12B: return R12;
    case R13:
    case R13D:
    case R13W:
    case R13B: return R13;
    case R14:
    case R14D:
    case R14W:
    case R14B: return R14;
    case R15:
    case R15D:
    case R15W:
    case R15B: return R15;
    default: error("get_push_or_popable_reg unhandled register: %s", get_reg(reg));
    }
    return -1; // to silence warning
}
