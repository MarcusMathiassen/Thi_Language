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
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 40 alloc, 8 padding
.begin:
	mov rax, 53
	push rax
	push rax
	mov rax, 8
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rbp-40]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 2
	push rax
	push rax
	mov rax, 16
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rbp-40]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, 54
	push rax
	mov rax, 16
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rbp-40]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	push rax
	mov rax, 8
	push rax
	mov rax, 0
	push rax
	lea rax, [rbp-40]; load_lea
	lea rax, [rbp-40]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	add rax, rcx
	mov rax, [rax]; deref
	pop rcx
	add rax, rcx
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	add rsp, 48; 40 alloc, 8 padding
	leave
	ret
