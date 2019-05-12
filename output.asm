section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 16 alloc, 0 padding
.begin:
	mov eax, 3
	push rax
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	mov [rbp-4], ecx; store
	pop rax
	mov eax, 1
	push rax
	push rax
	mov eax, [rbp-8]; load
	pop rcx
	mov [rbp-8], ecx; store
	pop rax
	mov eax, [rbp-8]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l2
	mov eax, [rbp-8]; load
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l3
	jmp .l1
.l2:
.l3:
.l1:
	lea rax, [rbp-16]; load_lea
	jmp .end
.end:
	add rsp, 16; 16 alloc, 0 padding
	leave
	ret
