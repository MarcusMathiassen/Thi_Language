section .data
	D0 db 5.300000
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, D0; float_ref
	MOVSS XMM0, [RAX]; float_ref
	PUSH RAX
	PUSH RAX
	MOVSD XMM0, QWORD [RBP-8]; load
	POP RCX
	MOVSD [RBP-8], XMM0; store
	POP RAX
.L1:
	JMP .L0
.L0:
	MOVSD XMM0, QWORD [RBP-8]; load
	CVTTSS2SI RAX, XMM0
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
