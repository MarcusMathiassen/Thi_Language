section .data

global _main

section .text

_add:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-4], EDI
	MOV [RBP-8], ESI
	MOV EAX, DWORD [RBP-4]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-8]; load
	POP RCX
	ADD RAX, RCX
	PUSH RAX
	MOV RAX, 2
	PUSH RAX
	MOV RAX, 2
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, 16
	MOV [RBP-8], RAX; store
	MOV RAX, 1
	NOT AL
	PUSH RAX
	MOV RAX, 6
	PUSH RAX
	MOV RAX, 25
	PUSH RAX
	POP RSI
	POP RDI
	CALL _add
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
