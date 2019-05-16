section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 40 alloc, 8 padding
.begin:
	mov rax, 1
	push rax
	push rax
	
; codegen_field_access
	mov rax, 0
	push rax
	
; codegen_subscript
	mov rax, 40
	push rax
	mov rax, 4
	pop rcx
	imul rax, rcx
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	pop rcx
	mov [rax], rcx; store
	pop rax
	
; codegen_field_access
	mov rax, 0
	push rax
	
; codegen_subscript
	mov rax, 40
	push rax
	mov rax, 4
	pop rcx
	imul rax, rcx
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	jmp .end
.end:
	add rsp, 48; 40 alloc, 8 padding
	leave
	ret
