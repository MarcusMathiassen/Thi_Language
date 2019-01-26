extern _puts
section .data
	D0 DQ `Hello`, 0 
	D1 DQ `Hello you`, 0 
	D2 DQ `this is a test`, 0 
global _main
section .text
_get:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, D0; string_ref
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	POP RAX
	MOV [RBP-8], RAX; store
.L1:
	JMP .L0
.L0:
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 32; 32 alloc, 0 padding
.BEGIN:
	CALL _get
	MOV RAX, D1; string_ref
	PUSH RAX
	POP RDI
	CALL _compile
	MOV RAX, D2; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
.L1:
	JMP .L0
.L0:
	MOV EAX, 3
	PUSH RAX
	MOV EAX, 3
	POP RCX
	IMUL RAX, RCX
	JMP .END
.END:
	ADD RSP, 32; 32 alloc, 0 padding
	LEAVE
	RET
_compile:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-8], RDI; store
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _puts
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
