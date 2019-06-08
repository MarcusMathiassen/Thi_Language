extern _printf
section .data
	d0: dd 714.340000
	d1: dq 14.345353
	d2: dq 3.141593
	d3: dq 2.000000
	d4: db `argc = %d\n`, 0 
	d5: db `*argv = %s\n`, 0 
	d6: db `Hi\n`, 0 
global _main
section .text
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 80; 72 alloc, 8 padding
.begin:
		mov [rbp-4], edi; store_r argc
		mov [rbp-12], rsi; store_r argv
		mov rax, 3
		push rax
		mov rax, [rbp-20]; load i
		pop rcx
		mov [rbp-20], rcx; store i
		mov rax, [rbp-20]; load i
		mov rax, 42
		push rax
		mov rax, [rbp-28]; load j
		pop rcx
		mov [rbp-28], rcx; store j
		mov rax, [rbp-28]; load j
		mov rax, 73
		push rax
		mov rax, [rbp-36]; load k
		pop rcx
		mov [rbp-36], rcx; store k
		mov rax, [rbp-36]; load k
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-40]; load f
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-40], xmm1; store f
		movss xmm0, [rbp-40]; load f
		movsd xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-48]; load x
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-48], xmm1; store x
		movsd xmm0, [rbp-48]; load x
		movsd xmm0, [rel d2]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-56]; load pi
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-56], xmm1; store pi
		movsd xmm0, [rbp-56]; load pi
		movsd xmm0, [rel d3]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-56]; load pi
		movsd xmm1, [rsp]
		add rsp, 8
		divsd xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-64]; load halfpi
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-64], xmm1; store halfpi
		movsd xmm0, [rbp-64]; load halfpi
		movsd xmm0, [rel d3]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-56]; load pi
		movsd xmm1, [rsp]
		add rsp, 8
		mulsd xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-72]; load twopi
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-72], xmm1; store twopi
		movsd xmm0, [rbp-72]; load twopi
		mov eax, [rbp-4]; load argc
		push rax
		mov rax, d4; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, [rbp-12]; load argv
		mov rax, [rax]; deref 
		push rax
		mov rax, d5; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		call _sayHi
		mov rax, 2
		push rax
		mov rax, [rbp-20]; load i
		pop rcx
		sub rax, rcx
		mov rax, rax
		jmp .end
.end:
		add rsp, 80; 72 alloc, 8 padding
		leave
		ret
_sayHi:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 0 alloc, 16 padding
.begin:
		mov rax, d6; string_ref
		push rax
		pop rdi
		mov al, 1; var_arg_count
		call _printf
.end:
		add rsp, 16; 0 alloc, 16 padding
		leave
		ret
