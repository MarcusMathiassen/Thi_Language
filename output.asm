section .data
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
