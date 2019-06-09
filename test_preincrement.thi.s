section .data
global _main
section .text
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 8 alloc, 8 padding
.begin:
		mov rax, 0
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		mov [rbp-8], rcx; store i
		mov rax, [rbp-8]; load i
		mov rax, [rbp-8]; load i
		mov rax, 1
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		add rax, rcx
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		mov [rbp-8], rcx; store i
		mov rax, [rbp-8]; load i
		mov rax, rax
		jmp .end
.end:
		add rsp, 16; 8 alloc, 8 padding
		leave
		ret
