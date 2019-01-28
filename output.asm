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
extern _getchar
extern _ungetc
extern _fputc
extern _putc
extern _putchar
extern _fgets
extern _fputs
extern _puts
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
	D0 DQ `Hello`, 0 
	D1 DQ `echo Hello there`, 0 
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, D1; string_ref
	PUSH RAX
	POP RDI
	CALL _system
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	PUSH RAX
	MOV EAX, 0
	PUSH RAX
	POP RDI
	POP RSI
	CALL _add
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
_add:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-4], EDI; store
	MOV [RBP-8], ESI; store
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-8]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-4]; load
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
