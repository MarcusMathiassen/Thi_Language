	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 14
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	movl	$0, -4(%rsp)
	movl	$3, -8(%rsp)
	movl	$3, %eax
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
