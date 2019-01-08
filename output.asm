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
	MOV EAX, 0
	MOV [RBP-8], EAX; store
.L0:
	MOV EAX, [RBP-8]; load
	PUSH RAX
	MOV EAX, 10
	POP RCX
	CMP RCX, RAX
	SETE AL
	JE .L2
	MOV EAX, [RBP-4]; load
	MOV EAX, 1
	ADD [RBP-4], EAX
.L1:
	MOV EAX, [RBP-8]; load
	MOV EAX, 1
	ADD [RBP-8], EAX
	JMP .L0
.L2:
	MOV EAX, 9
	PUSH RAX
	MOV EAX, [RBP-4]; load
	POP RCX
	SUB RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
