extern _puts
extern _putchar
extern _malloc
extern _atoi
extern _exit
extern _system
section .data
	D0 db `Hello`, 0 

global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, D0; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 0
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _putchar
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 1
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _putchar
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 2
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _putchar
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 3
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _putchar
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 4
	POP RCX
	IMUL RAX, RCX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _putchar
	LEA RAX, [RBP-8]; load_ref
	PUSH RAX
	POP RDI
	CALL _puts
.L1:
	JMP .L0
.L0:
	MOV RAX, 1
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
