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

#ifndef REGISTER_H
#define REGISTER_H

#include "common.h"
#include "value.h"

typedef enum {
    
    RAX,
    EAX,
    AX,
    AL,
    AH,
    RCX,
    ECX,
    CX,
    CL,
    CH,
    RDX,
    EDX,
    DX,
    DL,
    DH,
    RBX,
    EBX,
    BX,
    BL,
    BH,
    RSP,
    ESP,
    SP,
    SPL,
    RBP,
    EBP,
    BP,
    BPL,
    RSI,
    ESI,
    SI,
    SIL,
    RDI,
    EDI,
    DI,
    DIL,
    R8,
    R8D,
    R8W,
    R8B,
    R9,
    R9D,
    R9W,
    R9B,
    R10,
    R10D,
    R10W,
    R10B,
    R11,
    R11D,
    R11W,
    R11B,
    R12,
    R12D,
    R12W,
    R12B,
    R13,
    R13D,
    R13W,
    R13B,
    R14,
    R14D,
    R14W,
    R14B,
    R15,
    R15D,
    R15W,
    R15B,

    XMM0,
    XMM1,
    XMM2,
    XMM3,
    XMM4,
    XMM5,
    XMM6,
    XMM7,
    XMM8,
    XMM9,
    XMM10,
    XMM11,
    XMM12,
    XMM13,
    XMM14,
    XMM15,

    _REGISTER_COUNT_,

} Register_Kind;

char* get_reg(Register_Kind reg_n);
char* get_reg_fitting_value(Value* value);
Register_Kind get_push_or_popable_reg(Register_Kind reg);
Register_Kind get_rax_reg_of_byte_size(u8 bytes, char c);
Register_Kind get_parameter_reg_int(s8 i, s8 size);
Register_Kind get_parameter_reg_float(s8 i);
Register_Kind get_reg_as_another_size(Register_Kind reg, s8 size);
Register_Kind get_return_reg_int(s8 i, s8 size);
Register_Kind get_return_reg_float(s8 i);
Register_Kind get_size_of_reg(Register_Kind reg);

#endif
