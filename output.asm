section .data
global _main
section .text
_foo:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
.BEGIN:
	MOV [RBP-4], EDI; store
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-4]; load
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	; HELLO THERE
	MOV RAX, 1
	PUSH RAX
	POP RDI
	CALL _foo
	JMP .END
.END:
	LEAVE
	RET
