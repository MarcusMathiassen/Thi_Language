extern _malloc
section .data
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, 8
	PUSH RAX
	POP RDI
	CALL _malloc
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, 1
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	MOV RAX, 8
	PUSH RAX
	MOV RAX, 0
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
.L1:
	JMP .L0
.L0:
	LEA RAX, [RBP-8]; load_ref
	MOV RAX, 8
	PUSH RAX
	MOV RAX, 0
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
