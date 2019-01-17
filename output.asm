extern _puts
section .data
	D0 db `Hello. And welcome.`, 0 
	D1 db `1st`, 0 
	D2 db `2nd`, 0 
	D3 db `3rd`, 0 

global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, 0
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	POP RCX
	MOV [RBP-8], RCX; store
	POP RAX
.L1:
	MOV RAX, D0; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, D1; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, 3
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	POP RCX
	MOV [RBP-8], RCX; store
	POP RAX
	MOV RAX, D2; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, 2
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	POP RCX
	MOV [RBP-8], RCX; store
	POP RAX
	MOV RAX, D3; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, 1
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	POP RCX
	MOV [RBP-8], RCX; store
	POP RAX
	JMP .L0
.L0:
	MOV RAX, QWORD [RBP-8]; load
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
