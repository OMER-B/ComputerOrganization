	# 313264053 Omer Barak
.section	.rodata
print:		.string "here\n"
error: 		.string "Invalid input!\n"

/*
 * Return the length of the pstring.
 * Arguments: (Pstring* pstr).
 *	
 * Registers:	%rax = return value
 *		%rdi = pstr.len
 * No need to push or pop - registers are already in the heap.
 */
.text
.globl pstrlen
 	.type pstrlen, @function
pstrlen:
	movzbq	(%rdi), %rax	# Move first argument (pstr->len) to return value register.
	ret			# Return length.

/*
 * Replace every occurrence of the second arguement with the third arguement.
 * Arguments: (Pstring* pstr, char oldChar, char newChar).
 * Example: (AASSDF8ghj, 'A', 'B') -> BBSSDF8ghj
 */
 .globl replaceChar
 	.type replaceChar, @function
replaceChar:
	pushq	%r11 		# Push %r11 into the stack.
	pushq	%r10		# Push %r10 into the stack.
	pushq	%rdi
	call	pstrlen	# [LENGTH] Call pstrlen to get the size of the string
	addq	$1, %rdi
	xor	%r10, %r10	# [COUNTER] initialize to 0
	xor	%r11, %r11	# [STR] initalize to 0

.loop_rplcChar:
	# Check if need to end the loop.
	cmpq	%rax, %r10	# Compare counter and length of string.
	je	.end_rplcChar	# [END] If equal, end the loop

	movb	(%rdi), %r11b	# [STR] Put current char to the last byte of r11.

	cmpq	%r11, %rsi	# Compare current char (%r11b) to oldChar (%rsi).
	je 	.replace 	# If equal, then replace


.iter_rplcChar:
	addq	$1, %r10	# [COUNTER+1] Increase counter by 1.
	addq	$1, %rdi	# [STR+1] Go to next char in string.
	jmp	.loop_rplcChar	# Go back to the loop.

.replace:
	movb	%dl, (%rdi)	# Move newChar to the current char.
	jmp	.iter_rplcChar

.end_rplcChar:
	popq	%rdi
	movq	%rdi, %rax
	popq	%r10 		# Pop %r10 from the stack.
	popq	%r11 		# Pop %r11 from the stack.
	ret 			# Finished, finally.

/*
 * Copy src[i:j] into dst[i:j], and return dst.
 * Arguments: (Pstring* dst, Pstring* src char i, char j).
 * Example: (Pstring *dst, Pstring *src, char i, char j)
 *	    ("ABCDEF", "GHIJK", "1", "4") -> GHIJEF	
 *
 * If i > pstrlen or j > pstrlen, do nothing.
 */
 .globl pstrijcpy
 	.type pstrijcpy, @function
pstrijcpy:
	pushq 	%r15
	movq	%rdi, %r13	# src is %r13
	movq	%rsi, %r14	# dst is %r14
				# i is %rdx
				# j is %rcx
	subb	$48, %dl
	subb	$48, %cl

	call 	pstrlen		# Get the length of src.

	movq 	%rax, %r10	# Save src length in %r10
	movq	%rsi, %rdi	# Move the second pstring to %rdi for pstrlen
	xor	%rax, %rax	# Reset %rax
	call 	pstrlen 	# Get the length of dst.
	
	movq	%rax, %r11 	# Save dst length in %r11

	cmpq	%rcx, %rdx
	jg 	.pstrijcpy_invalid	# If i > j, end.

	cmpq 	%r10, %rcx
	jge 	.pstrijcpy_invalid	# If j >= src.len, end.

	cmpq 	%r11, %rcx
	jge 	.pstrijcpy_invalid	# If j >= dst.len, end.

	cmpq 	$0, %rcx
	jl 	.pstrijcpy_invalid	# If j < 0, end.

	cmpq 	$0, %rdx
	jl 	.pstrijcpy_invalid	# If i < 0, end
	addq	$1, %r13
	addq	$1, %r14
	addq	%rdx, %r13
	addq	%rdx, %r14

.loop_pstrijcpy:
	cmpq	%rcx, %rdx		# Check if counter == j
	ja	.end_pstrijcpy		# End function
	movb	(%r13), %r15b
	movb	%r15b, (%r14)
	addq	$1, %r13
	addq	$1, %r14
	addq	$1, %rdx		# [COUNTER+1] Increase counter by 1.
	jmp	.loop_pstrijcpy		# Go back to the loop.

.pstrijcpy_invalid:
	movq	%rdi, %r15
	movq	$error, %rdi	# Move wrong format string to %rdi.
	xor	%rax, %rax
	call 	printf
	movq	%r15, %rdi

.end_pstrijcpy:
	movq	%rdi, %rax
	popq %r15
	ret

/*
 * Swap every lower-case letter to upper-case letter and every upper-case to lower-case.
 * Arguments: (Pstring* pstr):
 * Example: AbcDef421fRafff -> aBCdEF421FrAFFF
 *
 * Ignore chars that are not letters (c > 'z' (0x7a),
 *				      c < 'a' (0x61),
 *				      c > 'Z' (0x5a),
 *				      c < 'A' (0x41)).
 */
  .globl swapCase
 	.type swapCase, @function
swapCase:
	pushq	%r15
	movq 	%rdi, %r15

	call	pstrlen		# [LENGTH] Call pstrlen to get the size of the string

	addq	$1, %rdi	# Add one to start the string (first address is length)
	xor	%r10, %r10	# [COUNTER] Set %r10 as counter and initialize to 0
	xor	%r11, %r11	# [STR] Set %r11 as string and initalize to 0

.loop_swapCase:	
	# Check if need to end the loop.
	cmpq	%rax, %r10	# Compare counter and length of string.
	je	.end_swapCase	# [END] If equal, end the loop

	movb	(%rdi), %r11b	# [STR] Put current char to the last byte of r11
	
	# Skip characters that aren't letters
	cmpb	$65, %r11b	# Compare char to 'A'
	jl	.iter_swapCase	# If char is less than 'A', it's not a letter!

	cmpb	$90, %r11b	# Compare char to 'Z'
	jle	.lowerCase	# Letter is upper-case
	
	cmpb	$97, %r11b	# Compare char to 'a'
	jl	.iter_swapCase	# if char is less than 'a', it's not a letter!

	cmpb	$122, %r11b	# Compare char to 'z'
	jle	.upperCase	# Letter is lower-case

	jmp	.iter_swapCase	# Next letter.

.iter_swapCase:
	addq	$1, %r10	# [COUNTER+1] Increase counter by 1.
	addq	$1, %rdi	# [STR+1] Go to next char in string.
	jmp	.loop_swapCase	# Go back to the loop.

.upperCase:
	subq	$32, %r11	# [STR = STR+32] Add 32 to the letter to make it lower-case
	movb	%r11b, (%rdi)	# Put the letter back in the string.
	jmp	.iter_swapCase	# Go to next iteration
	
.lowerCase:
	addq	$32, %r11	# [STR = STR-32] Subtract 32 from the letter to make it upper-case
	movb	%r11b, (%rdi)	# Put the letter back in the string.
	jmp	.iter_swapCase	# Go to next iteration

.end_swapCase:
	movq	%r15, %rax
	popq	%r15
	ret			# Finished, finally.
