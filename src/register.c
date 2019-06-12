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

#include "register.h"
#include "utility.h" // error

static char* reg[_REGISTER_COUNT_] = {
    "rax",
    "eax",
    "ax",
    "al",
    "ah",
    "rcx",
    "ecx",
    "cx",
    "cl",
    "ch",
    "rdx",
    "edx",
    "dx",
    "dl",
    "dh",
    "rbx",
    "ebx",
    "bx",
    "bl",
    "bh",

    "rsp",
    "esp",
    "sp",
    "spl",
    "rbp",
    "ebp",
    "bp",
    "bpl",
    "rsi",
    "esi",
    "si",
    "sil",
    "rdi",
    "edi",
    "di",
    "dil",

    "r8",
    "r8d",
    "r8w",
    "r8b",
    "r9",
    "r9d",
    "r9w",
    "r9b",
    "r10",
    "r10d",
    "r10w",
    "r10b",
    "r11",
    "r11d",
    "r11w",
    "r11b",
    "r12",
    "r12d",
    "r12w",
    "r12b",
    "r13",
    "r13d",
    "r13w",
    "r13b",
    "r14",
    "r14d",
    "r14w",
    "r14b",
    "r15",
    "r15d",
    "r15w",
    "r15b",
    "xmm0",
    "xmm1",
    "xmm2",
    "xmm3",
    "xmm4",
    "xmm5",
    "xmm6",
    "xmm7",
    "xmm8",
    "xmm9",
    "xmm10",
    "xmm11",
    "xmm12",
    "xmm13",
    "xmm14",
    "xmm15",
};

char* get_reg(Register_Kind reg_n) {
    return reg[reg_n];
}
char* get_reg_fitting_value(Value* value) {
    s64 size = get_size_of_value(value);
    s64 reg_n = get_rax_reg_of_byte_size(size, 'a');
    return reg[reg_n];
}

Register_Kind get_rax_reg_of_byte_size(u8 bytes, char c) {
    switch (bytes) {
    case 1: return c == 'a' ? AL : CL;
    case 2: return c == 'a' ? AX : CX;
    case 4: return c == 'a' ? EAX : ECX;
    case 8: return c == 'a' ? RAX : RCX;
    default: return c == 'a' ? RAX : RCX;
    }
    return -1;
}

Register_Kind get_return_reg_int(s8 i, s8 size) {
    switch (i) {
    ERROR_UNHANDLED_KIND(strf("i = %d, s = %d", i, size));
    case 0:
        switch (size) {
        case 8: return RAX;
        case 4: return EAX;
        case 2: return AX;
        case 1: return AL;
        }
    case 1:
        switch (size) {
        case 8: return RDX;
        case 4: return EDX;
        case 2: return DX;
        case 1: return DL;
        }
    }
    UNREACHABLE;
    return 0;
}

Register_Kind get_return_reg_float(s8 i) {
    switch (i) {
    ERROR_UNHANDLED_KIND(strf("i = %d", i));
    case 0: return XMM0;
    case 1: return XMM1;
    }
    UNREACHABLE;
    return 0;
}

Register_Kind get_parameter_reg_int(s8 i, s8 size) {
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

Register_Kind get_parameter_reg_float(s8 i) {
    switch (i) {
    case 0: return XMM0;
    case 1: return XMM1;
    case 2: return XMM2;
    case 3: return XMM3;
    case 4: return XMM4;
    case 5: return XMM5;
    case 6: return XMM6;
    case 7: return XMM7;
    default: error("we only support upto 6 parameters per function.");
    }
    UNREACHABLE;
    return 0;
};

Register_Kind get_size_of_reg(Register_Kind reg) {
    if (reg >= XMM0) return 8;
    switch (reg) {
    ERROR_UNHANDLED_KIND(strf("%d", reg));
    case RAX: return 8;
    case EAX: return 4;
    case AX: return 2;
    case AL: return 1;
    case RCX: return 8;
    case ECX: return 4;
    case CX: return 2;
    case CL: return 1;
    case RDX: return 8;
    case EDX: return 4;
    case DX: return 2;
    case DL: return 1;
    case RBX: return 8;
    case EBX: return 4;
    case BX: return 2;
    case BL: return 1;
    case RSP: return 8;
    case ESP: return 4;
    case SP: return 2;
    case SPL: return 1;
    case RBP: return 8;
    case EBP: return 4;
    case BP: return 2;
    case BPL: return 1;
    case RSI: return 8;
    case ESI: return 4;
    case SI: return 2;
    case SIL: return 1;
    case RDI: return 8;
    case EDI: return 4;
    case DI: return 2;
    case DIL: return 1;
    case R8: return 8;
    case R8D: return 4;
    case R8W: return 2;
    case R8B: return 1;
    case R9: return 8;
    case R9D: return 4;
    case R9W: return 2;
    case R9B: return 1;
    case R10: return 8;
    case R10D: return 4;
    case R10W: return 2;
    case R10B: return 1;
    case R11: return 8;
    case R11D: return 4;
    case R11W: return 2;
    case R11B: return 1;
    case R12: return 8;
    case R12D: return 4;
    case R12W: return 2;
    case R12B: return 1;
    case R13: return 8;
    case R13D: return 4;
    case R13W: return 2;
    case R13B: return 1;
    case R14: return 8;
    case R14D: return 4;
    case R14W: return 2;
    case R14B: return 1;
    case R15: return 8;
    case R15D: return 4;
    case R15W: return 2;
    case R15B: return 1;
    }
    UNREACHABLE;
    return 0;
}
Register_Kind get_reg_as_another_size(Register_Kind reg, s8 size) {
    switch (reg) {
    ERROR_UNHANDLED_KIND(strf("%d", reg));
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

Register_Kind get_push_or_popable_reg(Register_Kind reg) {
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
    default:
        error("get_push_or_popable_reg unhandled register: %s", get_reg(reg));
    }
    return -1; // to silence warning
}
