section .data
global _main
section .text
_get:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
.BEGIN:
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RAX
	MOV [RBP-4], EAX; store
.L1:
	JMP .L0
.L0:
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
	CALL _get
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
