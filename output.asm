extern _puts
section .data
	D0 db `Hello world!`, 0 

global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 12 alloc, 4 padding
.BEGIN:
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, 1
	PUSH RAX
	PUSH RAX
	MOV EAX, DWORD [RBP-12]; load
	POP RCX
	MOV [RBP-12], RCX; store
	POP RAX
.L1:
	JMP .L0
.L0:
	MOV RAX, 2
	NEG RAX
	PUSH RAX
	MOV RAX, 2
	PUSH RAX
	MOV RAX, 3
	PUSH RAX
	POP RSI
	POP RDI
	CALL _mul
	PUSH RAX
	MOV RAX, 6
	PUSH RAX
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 2
	PUSH RAX
	POP RSI
	POP RDI
	CALL _add
	PUSH RAX
	MOV RAX, 6
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 12 alloc, 4 padding
	LEAVE
	RET
_mul:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-4], EDI
	MOV [RBP-8], ESI
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-4]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-8]; load
	POP RCX
	IMUL RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
_add:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-4], EDI
	MOV [RBP-8], ESI
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
