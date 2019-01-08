section .data
global _main
section .text

_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
.BEGIN:
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
	PUSH RAX
	MOV EAX, 1
	POP RCX
	ADD RAX, RCX
	PUSH RAX
	POP RCX
	MOV [RBP-4], EAX; store
	JMP .L0
.L1:
	MOV EAX, [RBP-4]; load
	PUSH RAX
	MOV EAX, 3
	NEG EAX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
