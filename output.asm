extern _puts
section .data
	d0: dq `Hello`, 0 
	d1: dq `Hello`, 0 
	d2: dq `this is a test`, 0 
global _main
section .text
_get:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov rax, d0; string_ref
	push rax
	push rax
	mov rax, [rbp-8]; load
	lea rax, [rbp-8]
	pop rcx
	mov [rax], rcx; store
	pop rax
.l1:
	jmp .l0
.l0:
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 44 alloc, 4 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, [rbp-12]; load
	mov eax, 16
	push rax
	mov rax, [rbp-12]; load
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	push rax
	pop rdi
	call _puts
	call _get
	mov rax, d1; string_ref
	push rax
	pop rdi
	call _compile
	mov rax, d2; string_ref
	push rax
	pop rdi
	call _puts
.l1:
	jmp .l0
.l0:
	mov eax, 8
	push rax
	mov eax, 3
	push rax
	mov eax, 3
	pop rcx
	imul rax, rcx
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	add rsp, 48; 44 alloc, 4 padding
	leave
	ret
_compile:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	call _puts
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
