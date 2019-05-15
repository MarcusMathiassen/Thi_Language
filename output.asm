section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 20 alloc, 12 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	mov eax, 40
	jmp .end
.end:
	add rsp, 32; 20 alloc, 12 padding
	leave
	ret
