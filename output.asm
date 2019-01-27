extern _puts
section .data
	D0 DQ `Hello, world!`, 0 
global _main
section .text
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
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 12 alloc, 4 padding
.BEGIN:
	MOV [RBP-4], EDI; store
	MOV [RBP-12], RSI; store
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
.L1:
	JMP .L0
.L0:
	MOV EAX, 5
	PUSH RAX
	MOV EAX, 4
	PUSH RAX
	POP RDI
	POP RSI
	CALL _add
	JMP .END
.L3:
	JMP .L2
.L2:
	MOV EAX, 1
	JMP .END
.END:
	ADD RSP, 16; 12 alloc, 4 padding
	LEAVE
	RET
