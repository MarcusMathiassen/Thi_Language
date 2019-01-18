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
section .data
	D0 db `Hello`, 0 
	D1 db `Hello1`, 0 
	D2 db `Hello2`, 0 
	D3 db `Hello3`, 0 
global _main
section .text
_foo:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 4 alloc, 12 padding
.BEGIN:
	MOV [RBP-4], EDI; store
.L1:
	JMP .L0
.L0:
	MOV EAX, DWORD [RBP-4]; load
	JMP .END
.END:
	ADD RSP, 16; 4 alloc, 12 padding
	LEAVE
	RET
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 32; 32 alloc, 0 padding
.BEGIN:
	MOV RAX, D0; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, D1; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-16]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, D2; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-24]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	MOV RAX, D3; string_ref
	MOV RAX, [RAX]; deref
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-32]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
.L1:
	LEA RAX, [RBP-32]; load_ref
	PUSH RAX
	POP RDI
	CALL _puts
	LEA RAX, [RBP-24]; load_ref
	PUSH RAX
	POP RDI
	CALL _puts
	LEA RAX, [RBP-16]; load_ref
	PUSH RAX
	POP RDI
	CALL _puts
	LEA RAX, [RBP-8]; load_ref
	PUSH RAX
	POP RDI
	CALL _puts
	JMP .L0
.L0:
	MOV RAX, 3
	PUSH RAX
	POP RDI
	CALL _foo
	JMP .END
.END:
	ADD RSP, 32; 32 alloc, 0 padding
	LEAVE
	RET
