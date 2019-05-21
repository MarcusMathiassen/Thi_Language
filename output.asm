extern _printf
section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 64; 56 alloc, 8 padding
.begin:
	mov rax, 0
	push rax
	push rax
	mov rax, 0
	push rax
	mov rax, 16
	push rax
	lea rax, [rbp-56]; load_lea
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 1
	push rax
	push rax
	mov rax, 40
	push rax
	mov rax, 16
	push rax
	lea rax, [rbp-56]; load_lea
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 2
	push rax
	push rax
	mov rax, 80
	push rax
	mov rax, 16
	push rax
	lea rax, [rbp-56]; load_lea
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 142
	push rax
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-56]; load_lea
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 80
	push rax
	mov rax, 16
	push rax
	lea rax, [rbp-56]; load_lea
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	push rax
	mov rax, 40
	push rax
	mov rax, 16
	push rax
	lea rax, [rbp-56]; load_lea
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rbp-56]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	pop rcx
	add rax, rcx
	jmp .end
.end:
	add rsp, 64; 56 alloc, 8 padding
	leave
	ret
