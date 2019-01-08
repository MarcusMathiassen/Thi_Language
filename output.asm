section .data
global _main
section .text

_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
	MOV EAX, 1
	NOT AL
	JMP .END
.END:
	LEAVE
	RET
