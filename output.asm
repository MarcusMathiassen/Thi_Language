extern _puts
section .data
	d0: dq `Its a float!`, 0 
	d1: dq `Hello`, 0 
	d2: dq `are`, 0 
	d3: dq `you?`, 0 
	d4: dd 5.000000
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov eax, 0
	push rax
	push rax
	mov rax, [rbp-8]; load
	pop rcx
	mov [rbp-8], rcx; store
	pop rax
	mov rax, [rbp-8]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l1
	mov rax, d0; string_ref
	push rax
	pop rdi
	call _puts
	jmp .l1
.l0:
.l1:
	mov rax, [rbp-8]; load
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l4
	mov rax, [rbp-8]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l5
	mov rax, [rbp-8]; load
	push rax
	mov eax, 2
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l6
	mov rax, [rbp-8]; load
	push rax
	mov eax, 3
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l7
	mov rax, [rbp-8]; load
	push rax
	mov eax, 4
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l8
	mov rax, [rbp-8]; load
	push rax
	mov eax, 5
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l9
	mov rax, [rbp-8]; load
	push rax
	mov eax, 6
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l10
.l2:
.l12:
	jmp .l11
.l11:
	mov eax, 7
	jmp .end
	jmp .l3
.l4:
.l5:
	mov rax, d1; string_ref
	push rax
	pop rdi
	call _puts
.l6:
	mov rax, d2; string_ref
	push rax
	pop rdi
	call _puts
	jmp .l3
.l7:
	mov rax, d3; string_ref
	push rax
	pop rdi
	call _puts
	jmp .l3
.l8:
.l14:
	jmp .l13
.l13:
	mov eax, 4
	jmp .end
	jmp .l3
.l9:
.l16:
	jmp .l15
.l15:
	mov eax, 5
	jmp .end
	jmp .l3
.l10:
	movss xmm0, [rel d4]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movss xmm1, [rsp]
	add rsp, 8
	movss [rbp-12], xmm1; store
	movss xmm0, [rsp]
	add rsp, 8
.l18:
	jmp .l17
.l17:
	mov eax, 6
	jmp .end
	jmp .l3
.l3:
.l20:
	jmp .l19
.l19:
	mov eax, 1
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
