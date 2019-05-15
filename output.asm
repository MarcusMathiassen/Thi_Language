extern _printf
section .data
	d0: dq `sizeof v == %d\n`, 0 
	d1: dq `sizeof v2 == %d\n`, 0 
	d2: dq `sizeof int == %d\n`, 0 
	d3: dq `sizeof s32 == %d\n`, 0 
	d4: dq `sizeof s64 == %d\n`, 0 
	d5: dq `sizeof s64 * 13 / sizeof s64 == %d\n`, 0 
	d6: dq `sizeof (argc) == %d\n`, 0 
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 20 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, 8
	push rax
	mov rax, d0; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 8
	push rax
	mov rax, d1; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 8
	push rax
	mov rax, d2; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 4
	push rax
	mov rax, d3; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 8
	push rax
	mov rax, d4; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 13
	push rax
	mov rax, d5; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 4
	push rax
	mov rax, d6; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 8
	push rax
	pop rdi
	call _get
	jmp .end
.end:
	add rsp, 32; 20 alloc, 12 padding
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
