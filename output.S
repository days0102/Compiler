	.text
	.file	"module"
	.globl	program
	.p2align	4, 0x90
	.type	program,@function
program:
	.cfi_startproc
	retq
.Lfunc_end0:
	.size	program, .Lfunc_end0-program
	.cfi_endproc

	.globl	out
	.p2align	4, 0x90
	.type	out,@function
out:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.L__unnamed_1, %edi
	movb	$1, %al
	callq	printf
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	out, .Lfunc_end1-out
	.cfi_endproc

	.globl	in
	.p2align	4, 0x90
	.type	in,@function
in:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	%rsp, %rsi
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	scanf
	movsd	(%rsp), %xmm0
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	in, .Lfunc_end2-in
	.cfi_endproc

	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI3_0:
	.quad	4617315517961601024
.LCPI3_1:
	.quad	4620693217682128896
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movabsq	$4607182418800017408, %rax
	movq	%rax, 16(%rsp)
	movabsq	$4617315517961601024, %rax
	movq	%rax, (%rsp)
	movsd	.LCPI3_0(%rip), %xmm0
	callq	out
	movsd	(%rsp), %xmm0
	addsd	%xmm0, %xmm0
	movsd	%xmm0, 8(%rsp)
	callq	out
	callq	in
	movsd	%xmm0, 8(%rsp)
	callq	out
	movsd	(%rsp), %xmm0
	callq	out
	movsd	.LCPI3_1(%rip), %xmm0
	callq	out
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc

	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI4_0:
	.quad	4607182418800017408
	.text
	.globl	abc
	.p2align	4, 0x90
	.type	abc,@function
abc:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movabsq	$4625196817309499392, %rax
	movq	%rax, 8(%rsp)
	movabsq	$4627448617123184640, %rax
	movq	%rax, 16(%rsp)
	movsd	.LCPI4_0(%rip), %xmm0
	callq	out
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	abc, .Lfunc_end4-abc
	.cfi_endproc

	.globl	abcd
	.p2align	4, 0x90
	.type	abcd,@function
abcd:
	.cfi_startproc
	movabsq	$4620693217682128896, %rax
	movq	%rax, -8(%rsp)
	retq
.Lfunc_end5:
	.size	abcd, .Lfunc_end5-abcd
	.cfi_endproc

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%f\n"
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"%d"
	.size	.L__unnamed_2, 3

	.section	".note.GNU-stack","",@progbits
