section .data
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
.begin:
	mov eax, 1
	jmp .end
.end:
	leave
	ret
