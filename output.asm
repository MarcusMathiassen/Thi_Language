extern _malloc
section .data

global _main
section .text
_change:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-8], RDI
	LEA RAX, [RBP-8]; load_ref
	MOV RAX, [RAX]; deref
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, 4
	PUSH RAX
	POP RDI
	CALL _malloc
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, 0
	CMP AL, 0
	JE .L1
.L3:
	JMP .L2
.L2:
	MOV RAX, 0
	JMP .END
	JMP .L1
.L0:
.L1:
	LEA RAX, [RBP-8]; load_ref
	MOV RAX, [RAX]; deref
.L5:
	JMP .L4
.L4:
	LEA RAX, [RBP-8]; load_ref
	MOV RAX, [RAX]; deref
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
