	# 313264053 Omer Barak
.section	.rodata
print_case50:	.string "first pstring length: %d, second pstring length: %d \n"
print_case51:	.string "old char: %c, new char: %c, first string: %s, second string: %s\n"
print_case5253:	.string "length: %d, string: %s\n"
invalid:	.string	"invalid option!\n"
default:	.string	"default\n"
twoChar:	.string " %c"
int:		.string "%d"

.align	8
.table:
	.quad	.case50		# Case 50
	.quad	.case51		# Case 51
	.quad	.case52		# Case 52
	.quad	.case53		# Case 53
	.quad	.default	# Default case

.text
.globl run_func
	.type run_func, @function

/*
 * Removes 50 from the input op and jumps to the correct case using the jump table.
 */
run_func:
	leaq	-50(%rdi), %rdi		# Reduce 50 from the arguemnt so it's 0,1,2,3.
	cmpq	$0, %rdi		# Compare it with 4	
	jb	.invalid		# If arguemnt is above 4, its not a valid option.
	cmpq	$3, %rdi		# Compare it with 4	
	ja	.default		# If arguemnt is above 4, its not a valid option.
	jmp	*.table(,%rdi,8)	# Go to switch-case jump table.

/*
 * Calls pstrlen to get the length of the pstrings, and prints them.
 */
.case50:
	pushq	%r14
	pushq	%r15
	movq	%rsi, %rdi	# Move first pstring to rdi to get it's length
	xor	%rax, %rax
	call	pstrlen
	movq	%rax, %r14	# Save length at %r14

	movq	%rdx, %rdi	# Move second pstring to rdi to get it's length
	xor	%rax, %rax
	call	pstrlen
	movq	%rax, %r15	# Save length at %r14

	movq	%r14, %rsi	# Prepare for print
	movq	%r15, %rdx
	movq	$print_case50, %rdi
	xor	%rax, %rax
	call	printf

	popq	%r15
	popq	%r14
	ret

/*
 * Scans two chars and calls replaceChar to replace every instance of the first char with the second char,
 * in both pstrings, then prints them.
 */
.case51:
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	movq	%rsi, %r14	# Save first pstring in %r15
	movq	%rdx, %r15	# Save second pstring in %r14

	subq	$1, %rsp	# Increase stack by 1
	movq	$twoChar, %rdi	# Prepare to scanf char
	movq	%rsp, %rsi 	# This location for %rsi
	xor	%rax, %rax	# Reset %rax before scanf
	call	scanf		# Call scanf for '%c'
	movzbq 	(%rsp), %r12
	movq	$twoChar, %rdi	# Prepare to scanf char
	movq	%rsp, %rsi 	# This location for %rsi
	xor	%rax, %rax	# Reset %rax before scanf
	call	scanf		# Call scanf for '%c'
	movzbq 	(%rsp), %r13
	addq       $1, %rsp

	movq	%r12, %rsi
	movq	%r13, %rdx 
	movq 	%r14, %rdi	# [REPLACE] First string to %rdi

	call 	replaceChar
	movq 	%rax, %rcx 	# [PRINT] New first string at %rcx

	movq 	%r15, %rdi	# [REPLACE] Second string to %rdi
	call 	replaceChar
	movq 	%rax, %r8 	# [PRINT] New second string at %r8
	movq	%r13, %rdx
	movq	%r12, %rsi
	movq 	$print_case51, %rdi	# Prepare to print
	addq	$1, %rcx
	addq	$1, %r8

	xor 	%rax, %rax
	call 	printf

	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret

/*
 * Scans two indexes, i and j, then calls to pstrijcpy that puts the first pstring between
 * these indexes of the second pstring (src[i:j] in dst[i:j]).
 * Prints the new string and the original src string.
 */
.case52:
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	movq	%rsi, %r14	# Save first pstring in %r14
	movq	%rdx, %r15	# Save second pstring in %r15

	/* Scan i */
	subq 	$1, %rsp	# Allocate for bytes for i
	movq 	$twoChar, %rdi	# Prepare to scan i
	leaq 	(%rsp), %rsi
	xor 	%rax, %rax	# Reset %rax before scanf
	call 	scanf
	movq 	(%rsp), %r12	# Store i at %r12
	addq	$1, %rsp	# Deallocate i

	/* Scan j */
	subq 	$1, %rsp	# Allocate for bytes for j
	movq 	$twoChar, %rdi	# Prepare to scan j
	leaq 	(%rsp), %rsi
	xor 	%rax, %rax	# Reset %rax before scanf
	call 	scanf
	movq 	(%rsp), %r13	# Store j at %r13
	addq	$1, %rsp	# Deallocate j

	/* pstrijcpy */
	movq 	%r14, %rsi	# Move first string to %rdi
	movq 	%r15, %rdi	# Move second string to %rsi
	movq 	%r12, %rdx	# Move i to %rdx
	movq 	%r13, %rcx 	# Move j to %rcx
	xor 	%rax, %rax 	# Reset %rax
	call 	pstrijcpy
	movq	%rax, %r14

	/* First pstring length */
	movq 	%rax, %rdi	# Prepare to get first string length
	xor 	%rax, %rax 	# Reset %rax
	call 	pstrlen
	movq 	%rax, %rsi 	# First pstring length at %r12

	/* Print */
	movq	%r14, %rdx
	addq	$1, %rdx
	movq 	$print_case5253, %rdi	# Prepare to print case string
	xor	%rax, %rax
	call	printf

	/* Second pstring length */
	movq 	%r15, %rdi	# Prepare to get first string length
	xor 	%rax, %rax 	# Reset %rax
	call 	pstrlen
	movq 	%rax, %rsi 	# First pstring length at %r12

	/* Print */
	movq	%r15, %rdx
	addq	$1, %rdx
	movq 	$print_case5253, %rdi	# Prepare to print case string
	xor	%rax, %rax
	call	printf

	/* Print */
	movq	%r15, %rdx
	movq 	$print_case5253, %rdi	# Prepare to print case string

	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	ret

/*
 * Swap every lower-case letter of the pstring with upper-case
 * and every upper-case letter with lower-case.
 */
.case53:
	pushq	%r14
	pushq	%r15

	movq	%rsi, %r14	# Save first pstring in %r14
	movq	%rdx, %r15	# Save second pstring in %r15

	movq 	%r14, %rdi	# Prepare to swap case for first pstring
	pushq 	%rdi		# %rdi is caller-saved.
	call 	swapCase
	popq	%rdi		# Pop %rdi
	movq 	%rax, %rdx 	# Updated first pstring in %r14
	movq 	%rax, %rdi
	call 	pstrlen
	movq 	%rax, %rsi	# first pstring length in %rsi

	movq 	%r15, %rdi	# Prepare to swap case for second pstring
	pushq 	%rdi
	call 	swapCase
	popq	%rdi
	movq 	%rax, %r8 	# Updated second pstring in %r14
	movq 	%rax, %rdi
	call 	pstrlen
	movq 	%rax, %rcx	# second pstring length in %rsi

	addq	$1, %rdx	# Remove the string length from the pstring before printing
	addq	$1, %r8		# Remove the string length from the pstring before printing
	movq 	$print_case5253, %rdi	# Prepare to print case 53 string.
	xor 	%rax, %rax

	pushq 	%r8		# Backup registers because printf resets them
	pushq 	%rcx
	call 	printf
	popq 	%rcx
	popq 	%r8

	movq 	$print_case5253, %rdi	# Prepare to print case 53 string.
	movq	%rcx, %rsi
	movq	%r8, %rdx
	xor 	%rax, %rax
	call 	printf
	popq	%r15
	popq	%r14
	ret

.default:
	movq	$invalid, %rdi
	xor	%rax, %rax
	call 	printf
	ret

.invalid:
	movq	$invalid, %rdi
	xor	%rax, %rax
	call 	printf
	ret
