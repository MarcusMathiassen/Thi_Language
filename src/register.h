#ifndef REGISTER_H
#define REGISTER_H

#include "typedefs.h"
#include "value.h"

int   get_num_registers();
char* get_reg(int reg_n);
char* get_reg_fitting_value(Value* value);
int   get_push_or_popable_reg(int reg);
int   get_rax_reg_of_byte_size(u8 bytes);
int   get_parameter_reg(s8 i, s8 size);
int   get_reg_as_another_size(int reg, s8 size);

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

#define TOTAL_REG_COUNT 76

#endif
