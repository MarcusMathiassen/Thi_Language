extern _printf
section .data
	d0: dq `get(x: s64) s64`, 0 
	d1: dq `%s\n`, 0 
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
	mov rax, 64
	push rax
	pop rdi
	call _get
	push rax
	push rax
	mov rax, [rbp-20]; load
	pop rcx
	mov [rbp-20], rcx; store
	pop rax
	mov rax, d0; string_ref
	push rax
	mov rax, d1; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-20]; load
	jmp .end
.end:
	add rsp, 32; 20 alloc, 12 padding
	leave
	ret
