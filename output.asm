section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 40 alloc, 8 padding
.begin:
	mov eax, 2
	push rax
	push rax
	mov eax, 8
	push rax
	lea rax, [rbp-40]; load
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
.l1:
	jmp .l0
.l0:
	mov eax, 8
	push rax
	lea rax, [rbp-40]; load
	pop rcx
	add rax, rcx
	mov eax, dword [rax]; deref
	jmp .end
.end:
	add rsp, 48; 40 alloc, 8 padding
	leave
	ret
