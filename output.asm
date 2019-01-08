section .data
global _main
section .text

_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
.L0:
	MOV EAX, 0
	MOV [RBP-4], EAX; store
	MOV EAX, 0
	MOV [RBP-8], EAX; store
.L2:
	MOV EAX, [RBP-8]; load
	PUSH RAX
	MOV EAX, 10
	POP RCX
	CMP RCX, RAX
	SETE AL
	JE .L4
	MOV EAX, [RBP-4]; load
	MOV EAX, 1
	ADD [RBP-4], EAX
.L3:
	MOV EAX, [RBP-8]; load
	MOV EAX, 1
	ADD [RBP-8], EAX
	JMP .L2
.L4:
	MOV EAX, 9
	PUSH RAX
	MOV EAX, [RBP-4]; load
	POP RCX
	SUB RAX, RCX
	JMP .L1
.L1:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
