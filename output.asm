section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 24 alloc, 8 padding
.begin:
	mov rax, 5
	push rax
	push rax
	mov rax, [rbp-8]; load
	pop rcx
	mov [rbp-8], rcx; store
	pop rax
	mov rax, [rbp-8]; load
	lea rax, [rbp-8]; addrsof
	push rax
	push rax
	mov rax, [rbp-16]; load
	pop rcx
	mov [rbp-16], rcx; store
	pop rax
	mov rax, 51
	push rax
	push rax
	mov rax, [rbp-8]; load
	pop rcx
	mov [rbp-8], rcx; store
	pop rax
	mov rax, [rbp-16]; load
	mov rax, [rax]; deref
	jmp .end
.end:
	add rsp, 32; 24 alloc, 8 padding
	leave
	ret
