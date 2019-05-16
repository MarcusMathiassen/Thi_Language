section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 40 alloc, 8 padding
.begin:
	lea rax, [rbp-40]; load_lea
	mov rax, 8
	push rax
	mov rax, 4
	pop rcx
	imul rax, rcx
	push rax
	lea rax, [rbp-40]; load_lea
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	jmp .end
.end:
	add rsp, 48; 40 alloc, 8 padding
	leave
	ret
