	.arch armv8-a
	.file	"auto_vec.cc"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z9naive_dotPKfS0_m
	.type	_Z9naive_dotPKfS0_m, %function
_Z9naive_dotPKfS0_m:
.LFB0:
	.cfi_startproc
	movi	v0.2s, #0
	cbz	x2, .L1
	mov	x3, 0
	.p2align 3,,7
.L3:
	ldr	s2, [x0, x3, lsl 2]
	ldr	s1, [x1, x3, lsl 2]
	add	x3, x3, 1
	fmadd	s0, s2, s1, s0
	cmp	x2, x3
	bne	.L3
.L1:
	ret
	.cfi_endproc
.LFE0:
	.size	_Z9naive_dotPKfS0_m, .-_Z9naive_dotPKfS0_m
	.ident	"GCC: (GNU) 10.3.1"
	.section	.note.GNU-stack,"",@progbits
