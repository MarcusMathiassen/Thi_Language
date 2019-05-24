section .data
global _main
section .text
_change:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r x at 8
	mov rax, 44
	push rax
	push rax
	mov rax, [rbp-8]; load x from 8
	pop rcx
	mov [rax], rcx; store x
	pop rax
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 16 alloc, 16 padding
.begin:
	mov rax, 5
	push rax
	push rax
	mov rax, [rbp-8]; load i from 8
	pop rcx
	mov [rbp-8], rcx; store i at 8
	pop rax
	mov rax, [rbp-8]; load i from 8
	lea rax, [rbp-8]; addrsof
	push rax
	push rax
	mov rax, [rbp-16]; load k from 16
	pop rcx
	mov [rax], rcx; store k
	pop rax
	mov rax, [rbp-16]; load k from 16
	push rax
	pop rdi
	call _change
	mov rax, [rbp-8]; load i from 8
	jmp .end
.end:
	add rsp, 32; 16 alloc, 16 padding
	leave
	ret
