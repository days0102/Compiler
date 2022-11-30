	.text
	.file	"ir"
	.globl	program                 # -- Begin function program
	.p2align	4, 0x90
	.type	program,@function
program:                                # @program
	.cfi_startproc
# %bb.0:                                # %global
	retq
.Lfunc_end0:
	.size	program, .Lfunc_end0-program
	.cfi_endproc
                                        # -- End function
	.globl	out                     # -- Begin function out
	.p2align	4, 0x90
	.type	out,@function
out:                                    # @out
	.cfi_startproc
# %bb.0:                                # %print
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
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function main
.LCPI2_0:
	.quad	4617315517961601024     # double 5
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %class
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movabsq	$4607182418800017408, %rax # imm = 0x3FF0000000000000
	movq	%rax, 16(%rsp)
	movabsq	$4617315517961601024, %rax # imm = 0x4014000000000000
	movq	%rax, (%rsp)
	movsd	.LCPI2_0(%rip), %xmm0   # xmm0 = mem[0],zero
	callq	out
	movsd	(%rsp), %xmm0           # xmm0 = mem[0],zero
	addsd	%xmm0, %xmm0
	movsd	%xmm0, 8(%rsp)
	callq	out
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.globl	abc                     # -- Begin function abc
	.p2align	4, 0x90
	.type	abc,@function
abc:                                    # @abc
	.cfi_startproc
# %bb.0:                                # %class
	movabsq	$4625196817309499392, %rax # imm = 0x4030000000000000
	movq	%rax, -16(%rsp)
	movabsq	$4627448617123184640, %rax # imm = 0x4038000000000000
	movq	%rax, -8(%rsp)
	retq
.Lfunc_end3:
	.size	abc, .Lfunc_end3-abc
	.cfi_endproc
                                        # -- End function
	.globl	abcd                    # -- Begin function abcd
	.p2align	4, 0x90
	.type	abcd,@function
abcd:                                   # @abcd
	.cfi_startproc
# %bb.0:                                # %class
	movabsq	$4620693217682128896, %rax # imm = 0x4020000000000000
	movq	%rax, -8(%rsp)
	retq
.Lfunc_end4:
	.size	abcd, .Lfunc_end4-abcd
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%f"
	.size	.L__unnamed_1, 3

	.section	".note.GNU-stack","",@progbits
