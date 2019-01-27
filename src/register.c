#include "register.h"
#include "utility.h" // error

char* reg[TOTAL_REG_COUNT] = {
    "RAX",
    "EAX",
    "AX",
    "AL",
    "AH",
    "RCX",
    "ECX",
    "CX",
    "CL",
    "CH",
    "RDX",
    "EDX",
    "DX",
    "DL",
    "DH",
    "RBX",
    "EBX",
    "BX",
    "BL",
    "BH",

    "RSP",
    "ESP",
    "SP",
    "SPL",
    "RBP",
    "EBP",
    "BP",
    "BPL",
    "RSI",
    "ESI",
    "SI",
    "SIL",
    "RDI",
    "EDI",
    "DI",
    "DIL",

    "R8",
    "R8D",
    "R8W",
    "R8B",
    "R9",
    "R9D",
    "R9W",
    "R9B",
    "R10",
    "R10D",
    "R10W",
    "R10B",
    "R11",
    "R11D",
    "R11W",
    "R11B",
    "R12",
    "R12D",
    "R12W",
    "R12B",
    "R13",
    "R13D",
    "R13W",
    "R13B",
    "R14",
    "R14D",
    "R14W",
    "R14B",
    "R15",
    "R15D",
    "R15W",
    "R15B",
    "XMM0",
    "XMM1",
    "XMM2",
    "XMM3",
    "XMM4",
    "XMM5",
    "XMM6",
    "XMM7",
    "XMM8",
    "XMM9",
    "XMM10",
    "XMM11",
    "XMM12",
    "XMM13",
    "XMM14",
    "XMM15",
};

int   get_num_registers() { return TOTAL_REG_COUNT; }
char* get_reg(int reg_n) { return reg[reg_n]; }
char* get_reg_fitting_value(Value* value)
{
    s64 size  = get_size_of_value(value);
    s64 reg_n = get_rax_reg_of_byte_size(size);
    return reg[reg_n];
}

int get_rax_reg_of_byte_size(u8 bytes)
{
    switch (bytes) {
    case 1: return AL;
    case 2: return AX;
    case 4: return EAX;
    case 8: return RAX;
    }
    return RAX;
}

int get_parameter_reg(s8 i, s8 size)
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

int get_reg_as_another_size(int reg, s8 size)
{
    switch (reg) {
    case RAX:
    case EAX:
    case AX:
    case AL:
        switch (size) {
        case 8: return RAX;
        case 4: return EAX;
        case 2: return AX;
        case 1: return AL;
        }

    case RCX:
    case ECX:
    case CX:
    case CL:
        switch (size) {
        case 8: return RCX;
        case 4: return ECX;
        case 2: return CX;
        case 1: return CL;
        }

    case RDX:
    case EDX:
    case DX:
    case DL:
        switch (size) {
        case 8: return RDX;
        case 4: return EDX;
        case 2: return DX;
        case 1: return DL;
        }

    case RBX:
    case EBX:
    case BX:
    case BL:
        switch (size) {
        case 8: return RBX;
        case 4: return EBX;
        case 2: return BX;
        case 1: return BL;
        }
    case RSP:
    case ESP:
    case SP:
    case SPL:
        switch (size) {
        case 8: return RSP;
        case 4: return ESP;
        case 2: return SP;
        case 1: return SPL;
        }

    case RBP:
    case EBP:
    case BP:
    case BPL:
        switch (size) {
        case 8: return RBP;
        case 4: return EBP;
        case 2: return BP;
        case 1: return BPL;
        }

    case RSI:
    case ESI:
    case SI:
    case SIL:
        switch (size) {
        case 8: return RSI;
        case 4: return ESI;
        case 2: return SI;
        case 1: return SIL;
        }

    case RDI:
    case EDI:
    case DI:
    case DIL:
        switch (size) {
        case 8: return RDI;
        case 4: return EDI;
        case 2: return DI;
        case 1: return DIL;
        }

    case R8:
    case R8D:
    case R8W:
    case R8B:
        switch (size) {
        case 8: return R8;
        case 4: return R8D;
        case 2: return R8W;
        case 1: return R8B;
        }

    case R9:
    case R9D:
    case R9W:
    case R9B:
        switch (size) {
        case 8: return R9;
        case 4: return R9D;
        case 2: return R9W;
        case 1: return R9B;
        }
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
