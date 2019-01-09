extern _puts
extern _putchar
extern _malloc
extern _atoi
extern _exit
extern _system
section .data

global _main

section .text

_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 12 alloc, 4 padding
.BEGIN:
	MOV EAX, 3
	PUSH RAX
	POP RAX
	MOV [RBP-4], RAX; store
	MOV RAX, [RBP-4]; load
	LEA RAX, [RSP-4]; addrsof
	PUSH RAX
	POP RAX
	MOV [RBP-12], RAX; store
	MOV EAX, 1
	PUSH RAX
	LEA RAX, [RBP-12]; load
	PUSH RAX
	MOV EAX, 4
	PUSH RAX
	MOV EAX, 0
	POP RCX
	IMUL RAX, RCX
	POP RCX
	ADD RAX, RCX
	POP RAX
	MOV [RBP-12], RAX; store
	MOV RAX, [RBP-4]; load
	JMP .END
.END:
	ADD RSP, 16; 12 alloc, 4 padding
	LEAVE
	RET
