extern _printf
section .data
	d0: dq `v.x = %d\n`, 0 
	d1: dq `v.y = %d\n`, 0 
	d2: dq `v.z.z = %d\n`, 0 
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 44 alloc, 4 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, 6
	push rax
	push rax
	lea rax, [rbp-36]; load_lea
	mov rax, [rbp-28]; load
	pop rcx
	mov [rbp-28], rcx; store
	pop rax
	mov rax, 56
	push rax
	push rax
	lea rax, [rbp-36]; load_lea
	mov rax, [rbp-20]; load
	pop rcx
	mov [rbp-20], rcx; store
	pop rax
	mov rax, 3
	push rax
	push rax
	mov rax, [rbp-44]; load
	pop rcx
	mov [rbp-44], rcx; store
	pop rax
	mov rax, 43
	push rax
	push rax
	lea rax, [rbp-36]; load_lea
	lea rax, [rbp-12]; load_lea
	mov rax, [rbp-4]; load
	pop rcx
	mov [rbp-4], rcx; store
	pop rax
	lea rax, [rbp-36]; load_lea
	mov rax, [rbp-28]; load
	push rax
	mov rax, d0; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	lea rax, [rbp-36]; load_lea
	mov rax, [rbp-20]; load
	push rax
	mov rax, d1; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	lea rax, [rbp-36]; load_lea
	lea rax, [rbp-12]; load_lea
	mov rax, [rbp-4]; load
	push rax
	mov rax, d2; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	lea rax, [rbp-36]; load_lea
	mov rax, [rbp-28]; load
	jmp .end
.end:
	add rsp, 48; 44 alloc, 4 padding
	leave
	ret
