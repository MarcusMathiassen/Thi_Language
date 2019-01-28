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
	mov rax, qword [rbp-40]; load
	mov eax, 4
	push rax
	mov rax, qword [rbp-40]; load
	pop rcx
	add rax, rcx
	mov eax, dword [rax]; deref
	pop rax
	mov [rbp-40], rax; store
.l1:
	jmp .l0
.l0:
	mov rax, qword [rbp-40]; load
	mov eax, 4
	push rax
	mov rax, qword [rbp-40]; load
	pop rcx
	add rax, rcx
	mov eax, dword [rax]; deref
	jmp .end
.end:
	add rsp, 48; 40 alloc, 8 padding
	leave
	ret
