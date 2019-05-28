section .data
global _main
section .text
_change:
		push rbp
		mov rbp, rsp
		sub rsp, 32; 16 alloc, 16 padding
.begin:
		mov [rbp-8], rdi; store_r x at 8
		mov [rbp-16], rsi; store_r ri at 16
		mov rax, 53
		push rax
		mov rax, [rbp-8]; load x of type 's64*' from 8
		pop rcx
		mov [rax], rcx; store x of type 's64*'
		mov rax, [rbp-8]; load x of type 's64*' from 8
.end:
		add rsp, 32; 16 alloc, 16 padding
		leave
		ret
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 48; 32 alloc, 16 padding
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
		mov [rax], rcx; store k of type 's64*'
		mov rax, [rbp-32]; load k of type 's64*' from 32
		mov rax, 0
		push rax
		mov rax, [rbp-32]; load k of type 's64*' from 32
		push rax
		pop rdi
		pop rsi
		call _change
		mov rax, 3
		push rax
		mov rax, [rbp-24]; load i of type 's64' from 24
		push rax
		mov rax, [rbp-24]; load i of type 's64' from 24
		pop rcx
		imul rax, rcx
		pop rcx
		add rax, rcx
		jmp .end
.end:
		add rsp, 48; 32 alloc, 16 padding
		leave
		ret
