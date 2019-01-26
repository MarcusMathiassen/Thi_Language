section .data
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 16 alloc, 0 padding
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	ADD RSP, 16; 16 alloc, 0 padding
	LEAVE
	RET
