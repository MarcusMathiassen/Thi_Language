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

#include "typedefs.h"
#include "value.h"

s8 get_num_registers(void);
char* get_reg(s8 reg_n);
char* get_reg_fitting_value(Value* value);
s8 get_push_or_popable_reg(s8 reg);
s8 get_rax_reg_of_byte_size(u8 bytes, char c);
s8 get_parameter_reg(s8 i, s8 size);
s8 get_reg_as_another_size(s8 reg, s8 size);

s8 get_size_of_reg(s8 reg);

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
#define XMM0 68
#define XMM1 69
#define XMM2 70
#define XMM3 71
#define XMM4 72
#define XMM5 73
#define XMM6 74
#define XMM7 75
#define XMM8 76
#define XMM9 77
#define XMM10 78
#define XMM11 79
#define XMM12 80
#define XMM13 81
#define XMM14 82
#define XMM15 83

#define XMM_REG_START 68
#define TOTAL_REG_COUNT 84

#endif
