section .data
global _main
section .text
_get:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov rax, 6
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 20 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, 1
	push rax
	push rax
	mov rax, [rbp-20]; load
	pop rcx
	mov [rbp-20], rcx; store
	pop rax
	mov rax, [rbp-20]; load
	mov rax, 1
	push rax
	mov rax, [rbp-20]; load
	pop rcx
	add rax, rcx
	push rax
	push rax
	mov rax, [rbp-20]; load
	pop rcx
	mov [rbp-20], rcx; store
	pop rax
	jmp .end
.end:
	add rsp, 32; 20 alloc, 12 padding
	leave
	ret
