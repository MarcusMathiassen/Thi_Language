extern _printf
section .data
	d0: dq 53.350000
	d1: dq 3.000000
	d2: dq `%f\n`, 0 
global _main
section .text
_change:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 8 alloc, 8 padding
.begin:
		mov [rbp-8], rdi; store_r x at 8
		mov rax, 53
		push rax
		mov rax, [rbp-8]; load x of type 's64*' from 8
		pop rcx
		mov [rax], rcx; store x
		mov rax, [rbp-8]; load x of type 's64*' from 8
.end:
		add rsp, 16; 8 alloc, 8 padding
		leave
		ret
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 48; 40 alloc, 8 padding
.begin:
		mov [rbp-8], rdi; store_r argc at 8
		mov [rbp-16], rsi; store_r argv at 16
		movsd xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, [rbp-32]; load v of type 'v2' from 32
		movsd xmm1, [rsp]
		add rsp, 8
		mov [rbp-32], rcx; store v of type 'v2' at 32
		mov rax, [rbp-32]; load v of type 'v2' from 32
		movsd xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-40]; load f of type 'f64' from 40
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-40], xmm1; store f of type 'f64' at 40
		movsd xmm0, [rbp-40]; load f of type 'f64' from 40
		mov rax, [rbp-32]; load v of type 'v2' from 32
		push rax
		mov rax, d2; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, 1
		jmp .end
.end:
		add rsp, 48; 40 alloc, 8 padding
		leave
		ret
