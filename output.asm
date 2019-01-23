section .data
global _main
section .text
_add:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 12 alloc, 4 padding
.BEGIN:
	MOV [RBP-4], EDI; store
	MOV [RBP-8], ESI; store
	MOV [RBP-12], EDX; store
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-12]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-8]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RCX
	ADD RAX, RCX
	POP RCX
	SUB RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 12 alloc, 4 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 2
	PUSH RAX
	MOV EAX, 2
	PUSH RAX
	MOV EAX, 1
	PUSH RAX
	POP RDI
	POP RSI
	POP RDX
	CALL _add
	JMP .END
.END:
	LEAVE
	RET
