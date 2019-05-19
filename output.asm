section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov rax, 5
	push rax
	push rax
	mov rax, [rbp-8]; load
	pop rcx
	mov [rbp-8], rcx; store
	pop rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _comp
	cmp al, 0
	je .l1
	mov rax, 3
	jmp .end
	jmp .l1
.l0:
.l1:
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _comp
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
_comp:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov rax, [rbp-8]; load
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
