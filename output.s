extern _printf
section .data
	d0: db `Hello!\n`, 0 
global _main
section .text
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 12 alloc, 4 padding
.begin:
		mov [rbp-4], edi; store_r argc
		mov [rbp-12], rsi; store_r argv
		mov rax, d0; string_ref
		push rax
		pop rdi
		mov al, 1; var_arg_count
		call _printf
		mov rax, 1
		mov rax, rax
		jmp .end
.end:
		add rsp, 16; 12 alloc, 4 padding
		leave
		ret
