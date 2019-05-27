section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 16 alloc, 16 padding
.begin:
	mov [rbp-8], rdi; store_r argc at 8
	mov [rbp-16], rsi; store_r argv at 16
	mov rax, 1
	jmp .end
.end:
	add rsp, 32; 16 alloc, 16 padding
	leave
	ret
