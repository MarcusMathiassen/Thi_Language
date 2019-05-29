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
	d9: dq `c = %c, sizeof c = %d\n`, 0 
	d10: dq `v2`, 0 
	d11: dq `typeof v = %s\n`, 0 
	d12: dq `f64`, 0 
	d13: dq `typeof v.x = %s\n`, 0 
	d14: dq `f64`, 0 
	d15: dq `typeof v.y = %s\n`, 0 
	d16: dq `f64`, 0 
	d17: dq `typeof f = %s\n`, 0 
	d18: dq `f64`, 0 
	d19: dq `typeof x = %s\n`, 0 
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
		sub rsp, 80; 69 alloc, 11 padding
.begin:
		mov [rbp-4], edi; store_r argc at 4
		mov [rbp-12], rsi; store_r argv at 12
		mov rax, 1
		push rax
		mov rax, [rbp-20]; load i of type 's64' from 20
		pop rcx
		mov [rbp-20], rcx; store i of type 's64' at 20
		mov rax, [rbp-20]; load i of type 's64' from 20
		mov rax, [rbp-20]; load i of type 's64' from 20
		lea rax, [rbp-20]; addrsof 'i'
		push rax
		mov rax, [rbp-28]; load k of type 's64*' from 28
		pop rcx
		mov [rbp-28], rcx; store k of type 's64*' at 28
		mov rax, [rbp-28]; load k of type 's64*' from 28
		mov rax, 42
		push rax
		mov rax, [rbp-28]; load k of type 's64*' from 28
		pop rcx
		mov [rax], rcx; store k
		mov rax, [rbp-28]; load k of type 's64*' from 28
		mov rax, 0
		push rax
		mov rax, [rbp-36]; load x of type 's64' from 36
		pop rcx
		mov [rbp-36], rcx; store x of type 's64' at 36
		mov rax, [rbp-36]; load x of type 's64' from 36
.l0:
		mov rax, [rbp-36]; load x of type 's64' from 36
		push rax
		mov eax, [rbp-4]; load argc of type 's32' from 4
		pop rcx
		cmp rcx, rax
		setl al
		cmp al, 0
		je .l2
		mov rax, 8
		push rax
		mov rax, [rbp-36]; load x of type 's64' from 36
		pop rcx
		imul rax, rcx
		push rax
		mov rax, [rbp-12]; load argv of type 'u8**' from 12
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
		mov rax, [rbp-36]; load x of type 's64' from 36
		pop rcx
		imul rax, rcx
		push rax
		mov rax, [rbp-12]; load argv of type 'u8**' from 12
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
		mov rax, [rbp-36]; load x of type 's64' from 36
		mov rax, 1
		push rax
		mov rax, [rbp-36]; load x of type 's64' from 36
		pop rcx
		add rax, rcx
		push rax
		mov rax, [rbp-36]; load x of type 's64' from 36
		pop rcx
		mov [rbp-36], rcx; store x of type 's64' at 36
		mov rax, [rbp-36]; load x of type 's64' from 36
		jmp .l0
.l2:
		movsd xmm0, [rel d2]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, 0
		push rax
		mov rax, [rbp-44]; load v of type 'v2' from 44
		lea rax, [rbp-44]; addrsof 'v'
		pop rcx
		add rax, rcx
		movsd xmm1, [rsp]
		add rsp, 8
		mov [rax], rcx; store v
		mov rax, [rbp-44]; load v of type 'v2' from 44
		movsd xmm0, [rel d3]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		mov rax, 8
		push rax
		mov rax, [rbp-44]; load v of type 'v2' from 44
		lea rax, [rbp-44]; addrsof 'v'
		pop rcx
		add rax, rcx
		movsd xmm1, [rsp]
		add rsp, 8
		mov [rax], rcx; store v
		mov rax, [rbp-44]; load v of type 'v2' from 44
		movsd xmm0, [rel d4]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-52]; load f of type 'f64' from 52
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-52], xmm1; store f of type 'f64' at 52
		movsd xmm0, [rbp-52]; load f of type 'f64' from 52
		mov rax, 8
		push rax
		mov rax, [rbp-44]; load v of type 'v2' from 44
		lea rax, [rbp-44]; addrsof 'v'
		pop rcx
		add rax, rcx
		mov rax, [rax]; deref 'v'
		push rax
		mov rax, 0
		push rax
		mov rax, [rbp-44]; load v of type 'v2' from 44
		lea rax, [rbp-44]; addrsof 'v'
		pop rcx
		add rax, rcx
		mov rax, [rax]; deref 'v'
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
		movsd xmm0, [rbp-60]; load x of type 'f64' from 60
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-60], xmm1; store x of type 'f64' at 60
		movsd xmm0, [rbp-60]; load x of type 'f64' from 60
		movsd xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-52]; load f of type 'f64' from 52
		movsd xmm1, [rsp]
		add rsp, 8
		movsd [rbp-52], xmm1; store f of type 'f64' at 52
		movsd xmm0, [rbp-52]; load f of type 'f64' from 52
		movsd xmm0, [rbp-60]; load x of type 'f64' from 60
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rbp-52]; load f of type 'f64' from 52
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
		mov al, 72
		push rax
		pop rcx
		mov [rbp-69], cl; store c of type 'u8' at 69
		mov al, [rbp-69]; load c of type 'u8' from 69
		mov rax, 8
		push rax
		mov al, [rbp-69]; load c of type 'u8' from 69
		push rax
		mov rax, d9; string_ref
		push rax
		pop rdi
		pop rsi
		pop rdx
		mov al, 3; var_arg_count
		call _printf
		mov rax, d10; string_ref
		push rax
		mov rax, d11; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, d12; string_ref
		push rax
		mov rax, d13; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, d14; string_ref
		push rax
		mov rax, d15; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, d16; string_ref
		push rax
		mov rax, d17; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, d18; string_ref
		push rax
		mov rax, d19; string_ref
		push rax
		pop rdi
		pop rsi
		mov al, 2; var_arg_count
		call _printf
		mov rax, [rbp-28]; load k of type 's64*' from 28
		push rax
		pop rdi
		call _change
		mov rax, [rbp-20]; load i of type 's64' from 20
		jmp .end
.end:
		add rsp, 80; 69 alloc, 11 padding
		leave
		ret
