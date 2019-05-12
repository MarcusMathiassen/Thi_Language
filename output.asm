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
	d0: dq `'i' has the value of %d\n`, 0 
	d1: dq `Dark!`, 0 
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 8 alloc, 8 padding
.begin:
	mov eax, 3
	push rax
	push rax
	mov eax, [rbp-4]; load
	pop rcx
	mov [rbp-4], ecx; store
	pop rax
	mov eax, [rbp-4]; load
	push rax
	mov rax, d0; string_ref
	push rax
	pop rdi
	pop rsi
	call _printf
	mov eax, 1
	push rax
	push rax
	mov eax, [rbp-8]; load
	pop rcx
	mov [rbp-8], ecx; store
	pop rax
	mov eax, [rbp-8]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l2
	mov eax, [rbp-8]; load
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l3
	jmp .l1
.l2:
.l3:
	mov rax, d1; string_ref
	push rax
	pop rdi
	call _puts
	jmp .l1
.l1:
	mov eax, 1
	jmp .end
.end:
	add rsp, 16; 8 alloc, 8 padding
	leave
	ret
