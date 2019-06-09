section .data
global _main
section .text
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 8 alloc, 8 padding
.begin:
		mov rax, 3
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		add rax, rcx
		mov rax, rax
		jmp .end
.end:
		add rsp, 16; 8 alloc, 8 padding
		leave
		ret
