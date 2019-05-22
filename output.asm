extern _puts
extern _printf
section .data
	d0: dq 1333.333300
	d1: dq 4.330000
	d2: dq `%d %d %f %f\n`, 0 
	d3: dq `v2`, 0 
	d4: dq `get`, 0 
	d5: dq `%s %s\n`, 0 
	d6: dq `Hello`, 0 
	d7: dq `You!`, 0 
	d8: dq ` k = %d\n`, 0 
	d9: dq `*i = %d\n`, 0 
	d10: dq ` k = %d\n`, 0 
	d11: dq `*i = %d\n`, 0 
	d12: dq `s64**`, 0 
	d13: dq `typeof &i = %s\n`, 0 
	d14: dq `s64`, 0 
	d15: dq `typeof *i = %s\n`, 0 
	d16: dq `s64*`, 0 
	d17: dq `typeof  i = %s\n`, 0 
	d18: dq `s64*`, 0 
	d19: dq `typeof &k = %s\n`, 0 
	d20: dq `s64`, 0 
	d21: dq `typeof  k = %s\n`, 0 
	d22: dq `sizeof *i = %d\n`, 0 
	d23: dq `sizeof  i = %d\n`, 0 
	d24: dq `addrsof &k = %llu\n`, 0 
	d25: dq `addrsof &i = %llu\n`, 0 
	d26: dq `addrsof  i = %llu\n`, 0 
	d27: dq `*i = %llu\n`, 0 
	d28: dq ` k = %llu\n`, 0 
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
	sub rsp, 96; 84 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, 53
	push rax
	push rax
	mov rax, 8
	push rax
	mov rax, [rbp-60]; load
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rbp-60], rcx; store
	pop rax
	mov rax, 2
	push rax
	push rax
	mov rax, 16
	push rax
	mov rax, [rbp-60]; load
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rbp-60], rcx; store
	pop rax
	movsd xmm0, [rel d0]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	mov rax, 40
	push rax
	mov rax, [rbp-60]; load
	lea rax, [rbp-60]; addrsof
	pop rcx
	add rax, rcx
	movsd xmm1, [rsp]
	add rsp, 8
	mov [rbp-60], rcx; store
	movsd xmm0, [rsp]
	add rsp, 8
	push rax
	movsd xmm0, [rel d1]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	mov rax, 8
	push rax
	mov rax, [rbp-60]; load
	lea rax, [rbp-60]; addrsof
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
	mov rax, d6; string_ref
	push rax
	push rax
	mov rax, [rbp-68]; load
	pop rcx
	mov [rbp-68], rcx; store
	pop rax
	mov rax, [rbp-68]; load
	push rax
	pop rdi
	mov al, 1; var_arg_count
	call _puts
	mov rax, d7; string_ref
	push rax
	pop rdi
	mov al, 1; var_arg_count
	call _puts
	mov rax, -5
	push rax
	push rax
	mov rax, [rbp-76]; load
	pop rcx
	mov [rbp-76], rcx; store
	pop rax
	mov rax, [rbp-76]; load
	lea rax, [rbp-76]; addrsof
	push rax
	push rax
	mov rax, [rbp-84]; load
	pop rcx
	mov [rbp-84], rcx; store
	pop rax
	mov rax, 0
	cmp al, 0
	je .l1
	mov rax, 3
	jmp .end
	jmp .end
	jmp .l1
.l0:
.l1:
	mov rax, [rbp-76]; load
	push rax
	mov rax, d8; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-84]; load
	mov rax, [rax]; deref
	push rax
	mov rax, d9; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, 3
	push rax
	push rax
	mov rax, [rbp-76]; load
	pop rcx
	mov [rbp-76], rcx; store
	pop rax
	mov rax, [rbp-76]; load
	push rax
	mov rax, d10; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-84]; load
	mov rax, [rax]; deref
	push rax
	mov rax, d11; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, d12; string_ref
	push rax
	mov rax, d13; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, d14; string_ref
	push rax
	mov rax, d15; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, d16; string_ref
	push rax
	mov rax, d17; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, d18; string_ref
	push rax
	mov rax, d19; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, d20; string_ref
	push rax
	mov rax, d21; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, 8
	push rax
	mov rax, d22; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, 8
	push rax
	mov rax, d23; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-76]; load
	lea rax, [rbp-76]; addrsof
	push rax
	mov rax, d24; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-84]; load
	lea rax, [rbp-84]; addrsof
	push rax
	mov rax, d25; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-84]; load
	push rax
	mov rax, d26; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-84]; load
	mov rax, [rax]; deref
	push rax
	mov rax, d27; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, [rbp-76]; load
	push rax
	mov rax, d28; string_ref
	push rax
	pop rdi
	pop rsi
	mov al, 2; var_arg_count
	call _printf
	mov rax, 1
	jmp .end
.end:
	add rsp, 96; 84 alloc, 12 padding
	leave
	ret
