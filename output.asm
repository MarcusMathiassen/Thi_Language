section .data
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
.BEGIN:
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
.L0:
	MOV EAX, DWORD [RBP-4]; load
	PUSH RAX
	MOV EAX, 4
	POP RCX
	CMP RCX, RAX
	SETL AL
	CMP AL, 0
	JE .L1
	MOV EAX, 1
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RCX
	ADD RAX, RCX
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
	JMP .L0
.L1:
.L3:
	JMP .L2
.L2:
	MOV EAX, 3
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RCX
	SUB RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
