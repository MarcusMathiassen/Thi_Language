extern _puts
section .data
	d0: dq `Hello`, 0 
	d1: dq `Hi`, 0 
	d2: dq `Hello`, 0 
	d3: dq `this is a test`, 0 
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
	mov rax, 0
	call _get
	mov eax, 0
	push rax
	push rax
	mov eax, [rbp-48]; load
	pop rcx
	mov [rbp-48], ecx; store
	pop rax
.l0:
	mov eax, [rbp-48]; load
	push rax
	mov eax, 3
	pop rcx
	cmp rcx, rax
	setl al
	cmp al, 0
	je .l2
	mov rax, d1; string_ref
	push rax
	pop rdi
	mov rax, 1
	call _puts
.l1:
	mov eax, 1
	push rax
	mov eax, [rbp-48]; load
	pop rcx
	add rax, rcx
	push rax
	push rax
	mov eax, [rbp-48]; load
	pop rcx
	mov [rbp-48], ecx; store
	pop rax
	jmp .l0
.l2:
	mov rax, d2; string_ref
	push rax
	pop rdi
	mov rax, 1
	call _compile
	mov rax, d3; string_ref
	push rax
	pop rdi
	mov rax, 1
	call _puts
.l4:
	jmp .l3
.l3:
	mov eax, 1
	push rax
	mov eax, 3
	push rax
	mov eax, 3
	pop rcx
	imul rax, rcx
	push rax
	mov eax, 3
	push rax
	mov eax, 3
	pop rcx
	imul rax, rcx
	pop rcx
	sub rax, rcx
	pop rcx
	add rax, rcx
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
	mov rax, 1
	call _puts
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
