	.file	"hello.c"
	.text
	.globl	add
	.type	add, @function
add:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	addl	%edx, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	add, .-add
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$32, %esp
	movl	$0, -4(%ebp)
	movl	$0, -8(%ebp)
	movl	$0, -12(%ebp)
	movb	$0, -13(%ebp)
	movb	$0, -14(%ebp)
	movb	$0, -15(%ebp)
	movl	.LC0, %eax
	movl	%eax, -20(%ebp)
	movl	.LC0, %eax
	movl	%eax, -24(%ebp)
	movl	.LC0, %eax
	movl	%eax, -28(%ebp)
	movl	$1, -4(%ebp)
	movl	$1, -8(%ebp)
	movl	$1, -12(%ebp)
	movb	$1, -13(%ebp)
	movb	$1, -14(%ebp)
	movb	$1, -15(%ebp)
	movl	.LC1, %eax
	movl	%eax, -20(%ebp)
	movl	.LC2, %eax
	movl	%eax, -24(%ebp)
	movl	.LC3, %eax
	movl	%eax, -28(%ebp)
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	0
	.align 4
.LC1:
	.long	1066192077
	.align 4
.LC2:
	.long	1067030938
	.align 4
.LC3:
	.long	1067869798
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
