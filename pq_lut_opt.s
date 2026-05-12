	.arch armv8-a
	.file	"pq_lut_opt.cc"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z27build_lut_centroid_parallelPKfS0_Pfi
	.type	_Z27build_lut_centroid_parallelPKfS0_Pfi, %function
_Z27build_lut_centroid_parallelPKfS0_Pfi:
.LFB4267:
	.cfi_startproc
	mov	x7, x2
	sub	w2, w3, #1
	add	x2, x2, 1
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w9, w3
	mov	x6, x0
	mov	x10, x1
	lsl	x5, x2, 2
	lsl	w12, w3, 2
	add	x11, x7, 1024
	mov	w8, 0
	fmov	s17, 1.0e+0
	.p2align 3,,7
.L4:
	movi	v1.4s, 0
	cmp	w9, 0
	ble	.L5
	add	w2, w8, w9
	add	x4, x10, x8, sxtw 2
	add	w1, w2, w9
	mov	x0, 0
	add	w3, w9, w1
	add	x2, x10, x2, sxtw 2
	mov	v2.16b, v1.16b
	add	x1, x10, x1, sxtw 2
	mov	v3.16b, v1.16b
	add	x3, x10, x3, sxtw 2
	mov	v4.16b, v1.16b
	.p2align 3,,7
.L3:
	ldr	q16, [x4, x0]
	ldr	q7, [x2, x0]
	ldr	q6, [x1, x0]
	ldr	q5, [x0, x3]
	ldr	s0, [x6, x0]
	add	x0, x0, 4
	fmla	v4.4s, v16.4s, v0.s[0]
	fmla	v3.4s, v7.4s, v0.s[0]
	fmla	v2.4s, v6.4s, v0.s[0]
	fmla	v1.4s, v5.4s, v0.s[0]
	cmp	x0, x5
	bne	.L3
.L2:
	str	q4, [sp]
	add	w8, w8, w12
	ldr	s4, [sp]
	str	q3, [sp]
	ldr	s0, [sp]
	fsub	s3, s17, s4
	fsub	s0, s17, s0
	stp	s3, s0, [x7]
	add	x7, x7, 16
	str	q2, [sp]
	ldr	s0, [sp]
	fsub	s0, s17, s0
	str	s0, [x7, -8]
	str	q1, [sp]
	ldr	s0, [sp]
	fsub	s0, s17, s0
	str	s0, [x7, -4]
	cmp	x11, x7
	bne	.L4
	add	sp, sp, 16
	.cfi_remember_state
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L5:
	.cfi_restore_state
	mov	v2.16b, v1.16b
	mov	v3.16b, v1.16b
	mov	v4.16b, v1.16b
	b	.L2
	.cfi_endproc
.LFE4267:
	.size	_Z27build_lut_centroid_parallelPKfS0_Pfi, .-_Z27build_lut_centroid_parallelPKfS0_Pfi
	.ident	"GCC: (GNU) 10.3.1"
	.section	.note.GNU-stack,"",@progbits
