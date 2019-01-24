extern _puts
section .data
	D0 DQ `1st`, 0 
	D1 DQ `2nd`, 0 
	D2 DQ `3rd`, 0 
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
.L1:
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV EAX, 3
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
	MOV RAX, D1; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV EAX, 2
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
	MOV RAX, D2; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV EAX, 1
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-4]; load
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
