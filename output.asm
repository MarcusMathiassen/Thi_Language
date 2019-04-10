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
	d0: dq `framebuffer size: %dx%d\n`, 0 
global _main
section .text
_glfwGetFramebufferSize:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 16 alloc, 0 padding
.begin:
	mov [rbp-8], rdi; store_r
	mov [rbp-16], rsi; store_r
	mov eax, 640
	push rax
	push rax
	mov rax, [rbp-8]; load
	mov eax, 0
	push rax
	mov rax, [rbp-8]; load
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], ecx; store
	pop rax
	mov eax, 480
	push rax
	push rax
	mov rax, [rbp-16]; load
	mov eax, 0
	push rax
	mov rax, [rbp-16]; load
	lea rax, [rax]; addrsof
	pop rcx
	add rax, rcx
	pop rcx
	mov [rax], ecx; store
	pop rax
.end:
	add rsp, 16; 16 alloc, 0 padding
	leave
	ret
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 12 alloc, 4 padding
.begin:
	mov eax, 4
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _malloc
	push rax
	push rax
	mov rax, [rbp-8]; load
	lea rax, [rbp-8]
	pop rcx
	mov [rax], ecx; store
	pop rax
	mov eax, [rbp-12]; load
	lea rax, [rbp-12]; addrsof
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwGetFramebufferSize
	mov eax, [rbp-12]; load
	push rax
	mov rax, [rbp-8]; load
	mov eax, 0
	push rax
	mov rax, [rbp-8]; load
	pop rcx
	add rax, rcx
	mov eax, [rax]; deref
	push rax
	mov rax, d0; string_ref
	push rax
	pop rdi
	pop rsi
	pop rdx
	mov rax, 3; var_arg_count
	call _printf
	mov eax, 159
	push rax
	mov eax, [rbp-12]; load
	push rax
	mov rax, [rbp-8]; load
	mov eax, 0
	push rax
	mov rax, [rbp-8]; load
	pop rcx
	add rax, rcx
	mov eax, [rax]; deref
	pop rcx
	sub rax, rcx
	pop rcx
	sub rax, rcx
	jmp .end
.end:
	add rsp, 16; 12 alloc, 4 padding
	leave
	ret
