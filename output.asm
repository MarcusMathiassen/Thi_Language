section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 16 alloc, 16 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov [rbp-16], rsi; store_r
	mov rax, 0
	push rax
	push rax
	pop rcx
	mov [rbp-40], rcx; store
	pop rax
	mov rax, 1
	jmp .end
.end:
	add rsp, 32; 16 alloc, 16 padding
	leave
	ret
