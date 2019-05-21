section .data
global _main
section .text
_comp:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	call _get
	cmp al, 0
	je .l1
	mov rax, 3
	jmp .end
	jmp .l1
.l0:
.l1:
	mov rax, [rbp-8]; load
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_get:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov rax, [rbp-8]; load
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov rax, 3
	push rax
	push rax
	pop rcx
	mov [rbp-24], rcx; store
	pop rax
	mov rax, 3
	push rax
	pop rdi
	call _sq
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_sq:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 4 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov eax, [rbp-4]; load
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	imul rax, rcx
.end:
	add rsp, 16; 4 alloc, 12 padding
	leave
	ret
