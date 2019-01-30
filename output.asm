section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov eax, 3
	push rax
	push rax
	lea rax, [rbp-8]; load_lea
	mov eax, [rbp-4]; load
	pop rcx
	mov [rbp-4], ecx; store
	pop rax
	mov eax, 5
	push rax
	push rax
	lea rax, [rbp-8]; load_lea
	mov eax, [rbp-0]; load
	pop rcx
	mov [rbp-0], ecx; store
	pop rax
.l1:
	jmp .l0
.l0:
	lea rax, [rbp-8]; load_lea
	mov eax, [rbp-0]; load
	push rax
	lea rax, [rbp-8]; load_lea
	mov eax, [rbp-4]; load
	pop rcx
	add rax, rcx
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
