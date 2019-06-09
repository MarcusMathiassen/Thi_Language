extern _puts
extern _printf
section .data
	d0: db `Hello`, 0 
	d1: db `you`, 0 
	d2: db `1st`, 0 
	d3: db `2nd`, 0 
	d4: db `3rd`, 0 
	d5: db `Hi. How are you? %d\n`, 0 
	d6: db `4th`, 0 
global _main
section .text
_func:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 0 alloc, 16 padding
.begin:
		mov rax, 53
		mov rax, rax
		jmp .end
.end:
		mov rax, d0; string_ref
		push rax
		pop rdi
		call _puts
		mov rax, d1; string_ref
		push rax
		pop rdi
		call _puts
		add rsp, 16; 0 alloc, 16 padding
		leave
		ret
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 32; 16 alloc, 16 padding
.begin:
		mov rax, 0
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		mov [rbp-8], rcx; store i
		mov rax, [rbp-8]; load i
		call _func
		push rax
		mov rax, [rbp-16]; load k
		pop rcx
		mov [rbp-16], rcx; store k
		mov rax, [rbp-16]; load k
		mov rax, d2; string_ref
		push rax
		pop rdi
		call _puts
		mov rax, [rbp-8]; load i
		mov rax, rax
		jmp .end
.end:
		mov rax, d3; string_ref
		push rax
		pop rdi
		call _puts
		mov rax, d4; string_ref
		push rax
		pop rdi
		call _puts
		mov rax, 2
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		mov [rbp-8], rcx; store i
		mov rax, [rbp-8]; load i
		mov rax, [rbp-16]; load k
		push rax
		mov rax, d5; string_ref
		push rax
		pop rdi
		pop rsi
		call _printf
		mov rax, d6; string_ref
		push rax
		pop rdi
		call _puts
		mov rax, 1
		push rax
		mov rax, [rbp-8]; load i
		pop rcx
		mov [rbp-8], rcx; store i
		mov rax, [rbp-8]; load i
		add rsp, 32; 16 alloc, 16 padding
		leave
		ret
