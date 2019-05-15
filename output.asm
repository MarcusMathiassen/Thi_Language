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
	d0: dd 1.000000
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 16 alloc, 16 padding
.begin:
	mov [rbp-4], edi; store_r
	mov [rbp-12], rsi; store_r
	movss xmm0, [rel d0]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-16]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-16], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	mov rax, 4
	jmp .end
.end:
	add rsp, 32; 16 alloc, 16 padding
	leave
	ret
