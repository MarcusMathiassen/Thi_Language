extern _printf
section .data
	d0: dq `framebuffer size: %dx%d\n`, 0 
global _main
section .text
_glfwGetFramebufferSize:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 16 alloc, 0 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov [rbp-16], rsi; store_r
	mov eax, 640
	push rax
	push rax
	mov rax, [rbp-8]; load
	mov eax, 0
	push rax
	mov rax, [rbp-8]; load
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], ecx; store
	pop rax
	mov eax, 480
	push rax
	push rax
	mov rax, [rbp-16]; load
	mov eax, 0
	push rax
	mov rax, [rbp-16]; load
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], ecx; store
	pop rax
.end:
	add rsp, 16; 16 alloc, 0 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov eax, [rbp-8]; load
	lea rax, [rbp-8]; addrsof
	push rax
	mov eax, [rbp-4]; load
	lea rax, [rbp-4]; addrsof
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwGetFramebufferSize
	mov eax, [rbp-8]; load
	push rax
	mov eax, [rbp-4]; load
	push rax
	mov rax, d0; string_ref
	push rax
	pop rdi
	pop rsi
	pop rdx
	mov rax, 3; var_arg_count
	call _printf
.l1:
	jmp .l0
.l0:
	mov eax, 159
	push rax
	mov eax, [rbp-8]; load
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	sub rax, rcx
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
