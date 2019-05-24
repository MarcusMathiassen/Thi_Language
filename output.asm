extern _puts
section .data
global _main
section .text
_get:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r x at 8
	mov rax, [rbp-8]; load x from 8
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 24 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r argc at 8
	mov [rbp-16], rsi; store_r argv at 16
	mov rax, 3
	push rax
	push rax
	mov rax, [rbp-24]; load num from 24
	pop rcx
	mov [rbp-24], rcx; store num at 24
	pop rax
	mov rax, [rbp-16]; load argv from 16
	mov rax, [rax]; deref
	push rax
	pop rdi
	call _puts
	mov rax, [rbp-24]; load num from 24
	push rax
	pop rdi
	call _get
	jmp .end
.end:
	add rsp, 32; 24 alloc, 8 padding
	leave
	ret
