section .data
global _main
section .text

_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
	MOV EAX, 0
	MOV [RBP-4], EAX; store
.L0:
	MOV EAX, [RBP-4]; load
	PUSH RAX
	MOV EAX, 4
	POP RCX
	CMP RCX, RAX
	SETL AL
	JE .L1
	MOV EAX, [RBP-4]; load
	MOV EAX, 1
	ADD [RBP-4], EAX
	JMP .L0
.L1:
	MOV EAX, 3
	PUSH RAX
	MOV EAX, [RBP-4]; load
	POP RCX
	SUB RAX, RCX
	ADD RSP, 16
	POP RBP
	RET
