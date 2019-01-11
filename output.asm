section .data

global _main

section .text

_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
	MOV EAX, 3
	PUSH RAX
	MOV EAX, 2
	NEG EAX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	LEAVE
	RET
