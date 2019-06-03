section .data
global _main
section .text
_main:
		push rbp; PUSH
		mov rbp, rsp
		sub rsp, 16; 12 alloc, 4 padding
.begin:
		mov [rbp-4], edi; store_r argc
		mov [rbp-12], rsi; store_r argv
		mov rax, 1
		mov rax, rax
		jmp .end
.end:
		add rsp, 16; 12 alloc, 4 padding
		leave
		ret
