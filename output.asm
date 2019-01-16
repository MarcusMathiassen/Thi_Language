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
extern _fgetpos
extern _fsetpos
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
extern _vprintf
extern _vfprintf
extern _vsprintf
extern _vsnprintf
extern _vscanf
extern _vfscanf
extern _vsscanf
extern _perror
section .data
	D0 db `x64.asm`, 0 
	D1 db `w`, 0 
	D2 db `Hello this is a test`, 0 
	D3 db `Hello world!`, 0 

global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 32; 20 alloc, 12 padding
.BEGIN:
	MOV RAX, D0; string_ref
	PUSH RAX
	MOV RAX, D1; string_ref
	PUSH RAX
	POP RSI
	POP RDI
	CALL _fopen
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, D2; string_ref
	PUSH RAX
	LEA RAX, [RBP-8]; load
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RSI
	POP RDI
	CALL _fputs
	LEA RAX, [RBP-8]; load
	MOV RAX, [RAX]; deref
	PUSH RAX
	POP RDI
	CALL _fclose
	MOV RAX, D3; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, 1
	PUSH RAX
	PUSH RAX
	MOV EAX, DWORD [RBP-20]; load
	POP RCX
	MOV [RBP-20], RCX; store
	POP RAX
.L1:
	JMP .L0
.L0:
	MOV RAX, 2
	NEG RAX
	PUSH RAX
	MOV RAX, 2
	PUSH RAX
	MOV RAX, 3
	PUSH RAX
	POP RSI
	POP RDI
	CALL _mul
	PUSH RAX
	MOV RAX, 6
	PUSH RAX
	MOV RAX, 1
	PUSH RAX
	MOV RAX, 2
	PUSH RAX
	POP RSI
	POP RDI
	CALL _add
	PUSH RAX
	MOV RAX, 6
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	POP RCX
	ADD RAX, RCX
	JMP .END
.END:
	ADD RSP, 32; 20 alloc, 12 padding
	LEAVE
	RET
_mul:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV [RBP-4], EDI
	MOV [RBP-8], ESI
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-4]; load
	PUSH RAX
	MOV EAX, DWORD [RBP-8]; load
	POP RCX
	IMUL RAX, RCX
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
	MOV [RBP-4], EDI
	MOV [RBP-8], ESI
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
