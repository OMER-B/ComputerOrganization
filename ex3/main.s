	# 313264053 Omer Barak
.section	.rodata
	.align	8
int:	.string "%d"
string:	.string "%s"
char:	.string "%c"

print_test:	.string "here\n"

.text
.globl main
	.type main, @function

/*
 * Main function scans a integer, length for the string, and then the string itself
 * and stores them in the memory as [6 | S | T | R | I | N | G | /0 ]. Does it twice for two strings.
 * Then scans a third arguement as the option for the switch case and calls the switch-case function.
 */
main:
	pushq	%rbp		# Push %rbp to start frame
	movq	%rsp, %rbp	# Move %rsp to same location as %rbp
	pushq	%r13 		# pstring2
	pushq	%r12 		# pstring1

	/***********First String***********/
	leaq	4(%rsp), %rsp
	movq	$int, %rdi	# First parameter for scanf.
	movq	%rsp, %rsi 	# Second parameter for scanf.
	xor	%rax, %rax	# Initialize %rax
	call 	scanf		# Read the length.

	movl	(%rsp), %r12d 	# This is the length of the string.		
	leaq	-4(%rsp), %rsp
	addq	$1, %r12	# Add 1 to length VALUE of string, for \0

	subq	%r12, %rsp	# Allocate for string

	movq	$string, %rdi	# First parameter for scanf.
	movq	%rsp, %rsi 	# Second parameter for scanf.
	xor	%rax, %rax	# Initialize %rax
	call 	scanf		# Read the string.

	subq	$1, %r12 	# Need the length of the string without \0
	subq	$1, %rsp 	# Allocate for \0 in %rsp
	movb	%r12b, (%rsp) 	
	movq	%rsp, %r12	# First pstring in %r12

	/***********Second String***********/
	subq	$4, %rsp
	movq	$int, %rdi	# First parameter for scanf.
	movq	%rsp, %rsi 	# Second parameter for scanf.
	xor	%rax, %rax	# Initialize %rax
	call 	scanf		# Read the length.

	movl	(%rsp), %r13d 	# This is the length of the string.		
	addq	$4, %rsp
	addq	$1, %r13	# For \0

	subq	%r13, %rsp	# Allocate for string

	movq	$string, %rdi	# First parameter for scanf.
	movq	%rsp, %rsi 	# Second parameter for scanf.
	xor	%rax, %rax	# Initialize %rax
	call 	scanf		# Read the string.

	subq	$1, %r13 	# Need the length of the string without \0
	subq	$1, %rsp 	# Allocate for \0 in %rsp
	movb	%r13b, (%rsp)
	movq	%rsp, %r13	# Second pstring in %r13

	/***********Option***********/
	subq	$4, %rsp	# Increase stack by 4 for case number
	movq	$int, %rdi	# First parameter for scanf
	movq	%rsp, %rsi	# Second parameter for scanf
	xor	%rax, %rax	# Reset %rax
	call	scanf
	movl	(%rsp), %edi	# Case number is first arguement for function call

	/***********Call Cases***********/
	movq	%r13, %rdx	# Move second pstring to %rdx
	movq 	%r12, %rsi	# Move first pstring to %rsi
	call	run_func

	/***********End***********/

	xor	%rax, %rax	# Set return value to 0
	popq	%r12
	popq	%r13
	movq 	%rbp, %rsp
	popq	%rbp
	ret 			# End program.
