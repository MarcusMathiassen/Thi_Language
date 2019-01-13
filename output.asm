section .data

global _main

section .text

_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, 0
	MOV [RBP-8], RAX; store
.L0:
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	MOV RAX, 4
	POP RCX
	CMP RCX, RAX
	SETL AL
	JE .L1
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	MOV RAX, 1
	POP RCX
	ADD RAX, RCX
	MOV [RBP-8], RAX; store
	JMP .L0
.L1:
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	MOV RAX, 3
	NEG RAX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
