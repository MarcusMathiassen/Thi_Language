extern _printf
section .data
	d0: dq 1333.333300
	d1: dq 4.330000
	d2: dq `%d %d %f %f\n`, 0 
	d3: dq `v2 = { i: s64[5], z: f64 }`, 0 
	d4: dq `get(x: s64) s64`, 0 
	d5: dq `%s %s\n`, 0 
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
	sub rsp, 64; 60 alloc, 4 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, 53
	push rax
	push rax
	mov rax, 8
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-60]; load_lea
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 2
	push rax
	push rax
	mov rax, 16
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-60]; load_lea
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	movsd xmm0, [rel d0]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	mov rax, 40
	push rax
	lea rax, [rbp-60]; load_lea
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	movsd xmm1, [rsp]
	add rsp, 8
	mov [rax], rcx; store
	movsd xmm0, [rsp]
	add rsp, 8
	push rax
	movsd xmm0, [rel d1]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	mov rax, 8
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-60]; load_lea
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	push rax
	mov rax, 64
	push rax
	mov rax, d2; string_ref
	push rax
	pop rdi
	pop rsi
	pop rdx
	movsd xmm0, [rsp]
	add rsp, 8
	pop rcx
	mov al, 5; var_arg_count
	call _printf
	mov rax, d3; string_ref
	push rax
	mov rax, d4; string_ref
	push rax
	mov rax, d5; string_ref
	push rax
	pop rdi
	pop rsi
	pop rdx
	mov al, 3; var_arg_count
	call _printf
	mov rax, 54
	push rax
	mov rax, 16
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-60]; load_lea
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	push rax
	mov rax, 8
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-60]; load_lea
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	pop rcx
	add rax, rcx
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	add rsp, 64; 60 alloc, 4 padding
	leave
	ret
