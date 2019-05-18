section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 0 alloc, 16 padding
.begin:
	call _comp
	jmp .end
.end:
	add rsp, 16; 0 alloc, 16 padding
	leave
	ret
_comp:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 0 alloc, 16 padding
.begin:
	mov rax, 1
	jmp .end
.end:
	add rsp, 16; 0 alloc, 16 padding
	leave
	ret
