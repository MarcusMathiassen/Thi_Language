section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 24 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov [rbp-16], rsi; store_r
	mov rax, 24
	jmp .end
.end:
	add rsp, 32; 24 alloc, 8 padding
	leave
	ret
