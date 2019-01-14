extern _puts
section .data
	D0 db `1st`, 0 
	D1 db `2nd`, 0 
	D2 db `3rd`, 0 
	D3 db `1st`, 0 
	D4 db `2nd`, 0 
	D5 db `3rd`, 0 

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
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L1
.L3:
	MOV RAX, D0; string_ref
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
	MOV RAX, D1; string_ref
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
	MOV RAX, D2; string_ref
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
	JMP .L2
.L2:
	MOV RAX, 7
	JMP .END
	JMP .L1
.L0:
.L1:
.L5:
	MOV RAX, D3; string_ref
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
	MOV RAX, D4; string_ref
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
	MOV RAX, D5; string_ref
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
	JMP .L4
.L4:
	MOV RAX, QWORD [RBP-8]; load
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
