section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 12 alloc, 4 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov rax, 3
	jmp .end
.end:
	add rsp, 16; 12 alloc, 4 padding
	leave
	ret
