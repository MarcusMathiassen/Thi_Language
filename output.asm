extern _puts
section .data
	D0 DQ `Hello`, 0 
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
