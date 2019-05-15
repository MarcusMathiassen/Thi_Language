extern _strcmp
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
section .data
	d0: dd 53.000000
	d1: dq `u8**`, 0 
	d2: dq `typeof argv = %s\n`, 0 
	d3: dq `s64`, 0 
	d4: dq `typeof argc = %s\n`, 0 
	d5: dq `s64`, 0 
	d6: dq `typeof int = %s\n`, 0 
	d7: dq `s64`, 0 
	d8: dq `typeof integer = %s\n`, 0 
	d9: dq `f32`, 0 
	d10: dq `typeof floating = %s\n`, 0 
	d11: dq `v2`, 0 
	d12: dq `typeof v2 = %s\n`, 0 
	d13: dq `v2`, 0 
	d14: dq `typeof v = %s\n`, 0 
	d15: dq `f32`, 0 
	d16: dq `typeof v.x = %s\n`, 0 
	d17: dq `f32`, 0 
	d18: dq `typeof v.y = %s\n`, 0 
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 36 alloc, 12 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov [rbp-16], rsi; store_r
	mov rax, 415
	push rax
	push rax
	mov rax, [rbp-24]; load
	pop rcx
	mov [rbp-24], rcx; store
	pop rax
	movss xmm0, [rel d0]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-28]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-28], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	mov rax, d1; string_ref
	push rax
	mov rax, d2; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d3; string_ref
	push rax
	mov rax, d4; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d5; string_ref
	push rax
	mov rax, d6; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d7; string_ref
	push rax
	mov rax, d8; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d9; string_ref
	push rax
	mov rax, d10; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d11; string_ref
	push rax
	mov rax, d12; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d13; string_ref
	push rax
	mov rax, d14; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d15; string_ref
	push rax
	mov rax, d16; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, d17; string_ref
	push rax
	mov rax, d18; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov rax, 1
	jmp .end
.end:
	add rsp, 48; 36 alloc, 12 padding
	leave
	ret
