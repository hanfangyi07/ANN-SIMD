	.arch armv8-a
	.file	"hand_simd.cc"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z23inner_product_neon_optBPKfS0_m
	.type	_Z23inner_product_neon_optBPKfS0_m, %function
_Z23inner_product_neon_optBPKfS0_m:
.LFB4266:
	.cfi_startproc
	cmp	x2, 15
	bls	.L6
	movi	v1.4s, 0
	mov	x6, x0
	mov	x5, x1
	mov	x4, 16
	mov	v0.16b, v1.16b
	mov	v2.16b, v1.16b
	mov	v3.16b, v1.16b
	.p2align 3,,7
.L3:
	ld1	{v16.4s - v19.4s}, [x6], 64
	mov	x3, x4
	ld1	{v4.4s - v7.4s}, [x5], 64
	add	x4, x4, 16
	fmla	v3.4s, v16.4s, v4.4s
	fmla	v2.4s, v17.4s, v5.4s
	fmla	v0.4s, v18.4s, v6.4s
	fmla	v1.4s, v19.4s, v7.4s
	cmp	x2, x4
	bcs	.L3
	fadd	v0.4s, v0.4s, v1.4s
	fadd	v2.4s, v2.4s, v3.4s
	fadd	v0.4s, v0.4s, v2.4s
.L2:
	faddp	v0.4s, v0.4s, v0.4s
	faddp	v0.4s, v0.4s, v0.4s
	cmp	x2, x3
	bls	.L4
	.p2align 3,,7
.L5:
	ldr	s2, [x0, x3, lsl 2]
	ldr	s1, [x1, x3, lsl 2]
	add	x3, x3, 1
	fmadd	s0, s2, s1, s0
	cmp	x2, x3
	bne	.L5
.L4:
	fmov	s1, 1.0e+0
	fsub	s0, s1, s0
	ret
	.p2align 2,,3
.L6:
	movi	v0.4s, 0
	mov	x3, 0
	b	.L2
	.cfi_endproc
.LFE4266:
	.size	_Z23inner_product_neon_optBPKfS0_m, .-_Z23inner_product_neon_optBPKfS0_m
	.ident	"GCC: (GNU) 10.3.1"
	.section	.note.GNU-stack,"",@progbits
