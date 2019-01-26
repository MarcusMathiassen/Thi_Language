section .data
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 2
	PUSH RAX
	MOV EAX, 1
	PUSH RAX
	POP RDI
	POP RSI
	CALL _add
	JMP .END
.END:
	LEAVE
	RET
_add:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-4], EDI; store
	MOV [RBP-8], ESI; store
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-8]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
