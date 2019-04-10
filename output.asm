section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 4 alloc, 12 padding
.begin:
	mov eax, 46
	push rax
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	mov [rbp-4], ecx; store
	pop rax
	mov eax, 0
	jmp .end
.end:
	add rsp, 16; 4 alloc, 12 padding
	leave
	ret
