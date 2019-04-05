section .data
global _main
section .text
_fib:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 4 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov eax, [rbp-4]; load
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	sete al
	push rax
	mov eax, [rbp-4]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	pop rcx
	or al, cl
	setne al
	cmp al, 0
	je .l0
.l3:
	jmp .l2
.l2:
	mov eax, [rbp-4]; load
	jmp .end
	jmp .l1
.l0:
.l5:
	jmp .l4
.l4:
	mov eax, 2
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	sub rax, rcx
	push rax
	pop rdi
	mov rax, 1
	call _fib
	push rax
	mov eax, 1
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	sub rax, rcx
	push rax
	pop rdi
	mov rax, 1
	call _fib
	pop rcx
	add rax, rcx
	jmp .end
.l1:
.end:
	add rsp, 16; 4 alloc, 12 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
.begin:
.l1:
	jmp .l0
.l0:
	mov eax, 54
	push rax
	mov eax, 10
	push rax
	pop rdi
	mov rax, 1
	call _fib
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	leave
	ret
