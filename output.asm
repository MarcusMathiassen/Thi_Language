section .data

global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 16 alloc, 0 padding
.BEGIN:
	MOV RAX, 65
	PUSH RAX
	PUSH RAX
	MOV RAX, 0
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	LEA RAX, [RSP-8]; addrsof
	POP RCX
	ADD RAX, RCX
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, 0
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-16]; load
	POP RCX
	MOV [RBP-16], RCX; store
	POP RAX
	MOV RAX, 0
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	LEA RAX, [RSP-8]; addrsof
	POP RCX
	ADD RAX, RCX
	MOV RAX, [RAX]; deref
	JMP .END
.END:
	ADD RSP, 16; 16 alloc, 0 padding
	LEAVE
	RET
