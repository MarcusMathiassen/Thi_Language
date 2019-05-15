extern _printf
section .data
	d0: dd 415.000000
	d1: dd 53.000000
	d2: dq `s32`, 0 
	d3: dq `%s\n`, 0 
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	movss xmm0, [rel d0]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	mov eax, [rbp-4]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mov [rbp-4], ecx; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d1]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-8]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-8], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	mov rax, d2; string_ref
	push rax
	mov rax, d3; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 1
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
