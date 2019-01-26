section .data
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 16 alloc, 0 padding
.BEGIN:
	MOV EAX, 1
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
	MOV EAX, 3
	PUSH RAX
	MOV EAX, DWORD [RBP-16]; load
	POP RAX
	MOV [RBP-16], EAX; store
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-4]; load
	JMP .END
.END:
	ADD RSP, 16; 16 alloc, 0 padding
	LEAVE
	RET
