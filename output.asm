extern _printf
section .data
	d0: dq `%s %d\n`, 0 
	d1: dq 53.350000
	d2: dq 14.010000
	d3: dq `%f %f\n`, 0 
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
		sub rsp, 64; 56 alloc, 8 padding
.begin:
		mov [rbp-8], rdi; store_r argc at 8
		mov [rbp-16], rsi; store_r argv at 16
		mov rax, 1
		push rax
		mov rax, [rbp-24]; load i of type 's64' from 24
		pop rcx
		mov [rbp-24], rcx; store i of type 's64' at 24
		mov rax, [rbp-24]; load i of type 's64' from 24
		mov rax, [rbp-24]; load i of type 's64' from 24
		lea rax, [rbp-24]; addrsof 'i'
		push rax
		mov rax, [rbp-32]; load k of type 's64*' from 32
		pop rcx
		mov [rbp-32], rcx; store k of type 's64*' at 32
		mov rax, [rbp-32]; load k of type 's64*' from 32
		mov rax, 42
		push rax
		mov rax, [rbp-32]; load k of type 's64*' from 32
		pop rcx
		mov [rax], rcx; store k
		mov rax, [rbp-32]; load k of type 's64*' from 32
		mov rax, 0
		push rax
		mov rax, [rbp-40]; load x of type 's64' from 40
		pop rcx
		mov [rbp-40], rcx; store x of type 's64' at 40
		mov rax, [rbp-40]; load x of type 's64' from 40
.l0:
		mov rax, [rbp-40]; load x of type 's64' from 40
		push rax
		mov rax, 3
		pop rcx
		cmp rcx, rax
		setl al
		cmp al, 0
		je .l2
		mov rax, 1
		push rax
		mov rax, 8
		push rax
		mov rax, [rbp-40]; load x of type 's64' from 40
		pop rcx
		imul rax, rcx
		push rax
		mov rax, [rbp-16]; load argv of type 'u8**' from 16
		pop rcx
		add rax, rcx
		mov rax, [rax]; deref 'argv'
		push rax
		mov rax, d0; string_ref
		push rax
		pop rdi
		pop rsi
		pop rdx
		mov al, 3; var_arg_count
		call _printf
.l1:
		mov rax, [rbp-40]; load x of type 's64' from 40
		mov rax, 1
		push rax
		mov rax, [rbp-40]; load x of type 's64' from 40
		pop rcx
		add rax, rcx
		push rax
		mov rax, [rbp-40]; load x of type 's64' from 40
		pop rcx
		mov [rbp-40], rcx; store x of type 's64' at 40
		mov rax, [rbp-40]; load x of type 's64' from 40
		jmp .l0
.l2:
		movsd xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, [rbp-48]; load v of type 'v2' from 48
		movsd xmm1, [rsp]
		add rsp, 8
		mov [rbp-48], rcx; store v of type 'v2' at 48
		mov rax, [rbp-48]; load v of type 'v2' from 48
		movsd xmm0, [rel d2]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, 8
		push rax
		mov rax, [rbp-48]; load v of type 'v2' from 48
		lea rax, [rbp-48]; addrsof 'v'
		pop rcx
		add rax, rcx
		movsd xmm1, [rsp]
		add rsp, 8
		mov [rax], rcx; store v
		mov rax, [rbp-48]; load v of type 'v2' from 48
		mov rax, 8
		push rax
		mov rax, [rbp-48]; load v of type 'v2' from 48
		lea rax, [rbp-48]; addrsof 'v'
		pop rcx
		add rax, rcx
		mov rax, [rax]; deref 'v'
		push rax
		mov rax, [rbp-48]; load v of type 'v2' from 48
		push rax
		mov rax, d3; string_ref
		push rax
		pop rdi
		pop rsi
		pop rdx
		mov al, 3; var_arg_count
		call _printf
		mov rax, [rbp-32]; load k of type 's64*' from 32
		push rax
		pop rdi
		call _change
		mov rax, [rbp-24]; load i of type 's64' from 24
		jmp .end
.end:
		add rsp, 64; 56 alloc, 8 padding
		leave
		ret
