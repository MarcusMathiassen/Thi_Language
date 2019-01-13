section .data

global _main

section .text

_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
	MOV RAX, 8
	PUSH RAX
	MOV RAX, 7
	NEG RAX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	LEAVE
	RET
