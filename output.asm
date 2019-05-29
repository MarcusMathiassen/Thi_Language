extern _printf
section .data
	d0: dq `%s\n`, 0 
	d1: dq `%s\n`, 0 
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
		mov rax, [rbp-8]; load argc of type 's64' from 8
		pop rcx
		cmp rcx, rax
		setl al
		cmp al, 0
		je .l2
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
		mov al, 2; var_arg_count
		call _printf
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
		mov rax, d1; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
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
		mov rax, 4
		jmp .end
.end:
		add rsp, 48; 40 alloc, 8 padding
		leave
		ret
