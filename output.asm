section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 32 alloc, 16 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov [rbp-16], rsi; store_r
	mov rax, 0
	push rax
	push rax
	mov rax, [rbp-32]; load
	pop rcx
	mov [rbp-32], rcx; store
	pop rax
	mov rax, [rbp-32]; load
	push rax
	mov rax, 0
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l1
	mov rax, 3
	jmp .end
	jmp .l1
.l0:
.l1:
	mov rax, [rbp-24]; load
	jmp .end
.end:
	add rsp, 48; 32 alloc, 16 padding
	leave
	ret
