extern _printf
section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 40 alloc, 8 padding
.begin:
	mov rax, 53
	push rax
	push rax
	mov rax, 8
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rbp-40]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 1
	jmp .end
.end:
	add rsp, 48; 40 alloc, 8 padding
	leave
	ret
