section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 16 alloc, 0 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov eax, 3
	push rax
	push rax
	mov eax, [rbp-16]; load
	pop rcx
	mov [rbp-16], ecx; store
	pop rax
	mov eax, 3
	push rax
	mov eax, 3
	push rax
	mov eax, 1
	pop rcx
	add rax, rcx
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	add rsp, 16; 16 alloc, 0 padding
	leave
	ret
