section .data
global _main
section .text
_main:
		push rbp; PUSH
		mov rbp, rsp
		sub rsp, 32; 20 alloc, 12 padding
.begin:
		mov [rbp-4], edi; store_r argc
		mov [rbp-12], rsi; store_r argv
		mov rax, 341
		push rax; PUSH
		mov rax, [rbp-20]; load i
		pop rcx; POP
		mov [rbp-20], rcx; store i
		mov rax, [rbp-20]; load i
		mov rax, 1
		mov rax, rax
		jmp .end
.end:
		add rsp, 32; 20 alloc, 12 padding
		leave
		ret
