section .data
	d0: dd 53.300000
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 20 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov eax, 0
	push rax
	push rax
	mov eax, [rbp-16]; load
	pop rcx
	mov [rbp-16], ecx; store
	pop rax
	movss xmm0, [rel d0]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-20]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-20], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	mov eax, 1
	jmp .end
.end:
	add rsp, 32; 20 alloc, 12 padding
	leave
	ret
