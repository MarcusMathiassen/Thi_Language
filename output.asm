extern _fopen
extern _freopen
extern _fclose
extern _remove
extern _rename
extern _feof
extern _ferror
extern _fflush
extern _clearerr
extern _fseek
extern _ftell
extern _rewind
extern _fread
extern _fwrite
extern _fgetc
extern _getc
extern _getchar
extern _ungetc
extern _fputc
extern _putc
extern _putchar
extern _fgets
extern _fputs
extern _puts
extern _printf
extern _fprintf
extern _sprintf
extern _snprintf
extern _scanf
extern _fscanf
extern _sscanf
extern _perror
extern _atoi
extern _atol
extern _atoll
extern _atof
extern _strtof
extern _strtod
extern _strtold
extern _strtol
extern _strtoul
extern _strtoll
extern _strtoull
extern _rand
extern _srand
extern _malloc
extern _calloc
extern _realloc
extern _free
extern _aligned_alloc
extern _abort
extern _exit
extern __Exit
extern _quick_exit
extern _getenv
extern _system
extern _abs
extern _labs
extern _llabs
section .data
	d0: dq `%s\n`, 0 
	d1: dq `%s\n`, 0 
	d2: dq 53.350000
	d3: dq 14.010000
	d4: dq 3.000000
	d5: dq `v.x = %f v.x = %f\n`, 0 
	d6: dq 5.000000
	d7: dq 3.000000
	d8: dq `f = %f x = %f\n`, 0 
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
		sub rsp, 80; 72 alloc, 8 padding
.begin:
		mov [rbp-8], rdi; store_r argc at 8
		mov [rbp-16], rsi; store_r argv at 16
		mov rax, 1
		push rax
		pop rcx
		mov [rbp-32], rcx; store i of type 's64' at 32
		mov rax, [rbp-32]; load i of type 's64' from 32
		mov rax, [rbp-32]; load i of type 's64' from 32
		lea rax, [rbp-32]; addrsof 'i'
		push rax
		pop rcx
		mov [rbp-40], rcx; store k of type 's64*' at 40
		mov rax, [rbp-40]; load k of type 's64*' from 40
		mov rax, 42
		push rax
		mov rax, [rbp-40]; load k of type 's64*' from 40
		pop rcx
		mov [rax], rcx; store k
		mov rax, [rbp-40]; load k of type 's64*' from 40
		mov rax, 0
		push rax
		pop rcx
		mov [rbp-48], rcx; store x of type 's64' at 48
		mov rax, [rbp-48]; load x of type 's64' from 48
.l0:
		mov rax, [rbp-48]; load x of type 's64' from 48
		push rax
		mov rax, [rbp-8]; load argc of type 's64' from 8
		pop rcx
		cmp rcx, rax
		setl al
		cmp al, 0
		je .l2
		mov rax, 8
		push rax
		mov rax, [rbp-48]; load x of type 's64' from 48
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
		mov rax, [rbp-48]; load x of type 's64' from 48
		pop rcx
		imul rax, rcx
		push rax
		mov rax, d1; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
.l1:
		mov rax, [rbp-48]; load x of type 's64' from 48
		mov rax, 1
		push rax
		mov rax, [rbp-48]; load x of type 's64' from 48
		pop rcx
		add rax, rcx
		push rax
		mov rax, [rbp-48]; load x of type 's64' from 48
		pop rcx
		mov [rbp-48], rcx; store x of type 's64' at 48
		mov rax, [rbp-48]; load x of type 's64' from 48
		jmp .l0
.l2:
		movsd xmm0, [rel d2]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, [rbp-48]; load v of type 'v2' from 48
		movsd xmm1, [rsp]
		add rsp, 8
		mov [rbp-48], rcx; store v of type 'v2' at 48
		mov rax, [rbp-48]; load v of type 'v2' from 48
		movsd xmm0, [rel d3]; float_ref
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
		movsd xmm0, [rel d4]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-64], xmm1; store f of type 'f64' at 64
		movsd xmm0, [rbp-64]; load f of type 'f64' from 64
		mov rax, 8
		push rax
		mov rax, [rbp-48]; load v of type 'v2' from 48
		push rax
		mov rax, d5; string_ref
		push rax
		pop rdi
		pop rsi
		pop rdx
		mov al, 3; var_arg_count
		call _printf
		movsd xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-72], xmm1; store x of type 'f64' at 72
		movsd xmm0, [rbp-72]; load x of type 'f64' from 72
		movsd xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-64]; load f of type 'f64' from 64
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-64], xmm1; store f of type 'f64' at 64
		movsd xmm0, [rbp-64]; load f of type 'f64' from 64
		movsd xmm0, [rbp-72]; load x of type 'f64' from 72
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-64]; load f of type 'f64' from 64
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, d8; string_ref
		push rax
		pop rdi
		movsd xmm0, [rsp]
		add rsp, 8
		movsd xmm1, [rsp]
		add rsp, 8
		mov al, 3; var_arg_count
		call _printf
		mov rax, [rbp-40]; load k of type 's64*' from 40
		push rax
		pop rdi
		call _change
		mov rax, [rbp-32]; load i of type 's64' from 32
		jmp .end
.end:
		add rsp, 80; 72 alloc, 8 padding
		leave
		ret
