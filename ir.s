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
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %class
	movabsq	$4607182418800017408, %rax # imm = 0x3FF0000000000000
	movq	%rax, -8(%rsp)
	movabsq	$4617315517961601024, %rax # imm = 0x4014000000000000
	movq	%rax, -16(%rsp)
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
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
.Lfunc_end2:
	.size	abc, .Lfunc_end2-abc
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
.Lfunc_end3:
	.size	abcd, .Lfunc_end3-abcd
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
