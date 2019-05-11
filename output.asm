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
	d0: dq `Hello, how are you?`, 0 
	d1: dq `%s\n`, 0 
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
.begin:
	mov rax, d0; string_ref
	push rax
	pop rdi
	call _print
	mov eax, 1
	jmp .end
.end:
	leave
	ret
_print:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov rax, [rbp-8]; load
	push rax
	mov rax, d1; string_ref
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _printf
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
