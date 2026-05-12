	.arch armv8-a
	.file	"pq_adc_opt.cc"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z12adc_scan_optPPKhPPKfmiPf
	.type	_Z12adc_scan_optPPKhPPKfmiPf, %function
_Z12adc_scan_optPPKhPPKfmiPf:
.LFB0:
	.cfi_startproc
	cbz	x2, .L17
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	mov	x15, x2
	mov	x14, 0
	mov	x18, 8
	.p2align 3,,7
.L11:
	cmp	x15, 8
	stp	xzr, xzr, [sp]
	movi	v1.2s, #0
	stp	xzr, xzr, [sp, 16]
	csel	x13, x15, x18, ls
	cmp	w3, 0
	ble	.L3
	mov	x9, 0
	b	.L7
	.p2align 2,,3
.L21:
	ldrb	w17, [x6, x14]
	add	x9, x9, 1
	ldrb	w16, [x5, 1]
	ldrb	w12, [x5, 2]
	ldrb	w11, [x5, 3]
	ldrb	w10, [x5, 4]
	ldrb	w8, [x5, 5]
	ldrb	w6, [x5, 6]
	ldrb	w5, [x5, 7]
	ldp	s7, s6, [sp, 4]
	ldp	s5, s4, [sp, 12]
	ldr	s19, [x7, x17, lsl 2]
	ldr	s18, [x7, x16, lsl 2]
	ldr	s17, [x7, x12, lsl 2]
	ldr	s16, [x7, x11, lsl 2]
	fadd	s1, s1, s19
	ldp	s3, s2, [sp, 20]
	fadd	s7, s7, s18
	fadd	s6, s6, s17
	fadd	s5, s5, s16
	ldr	s19, [x7, x10, lsl 2]
	ldr	s18, [x7, x8, lsl 2]
	ldr	s17, [x7, x6, lsl 2]
	ldr	s16, [x7, x5, lsl 2]
	fadd	s4, s4, s19
	ldr	s0, [sp, 28]
	fadd	s3, s3, s18
	fadd	s2, s2, s17
	stp	s1, s7, [sp]
	fadd	s0, s0, s16
	stp	s6, s5, [sp, 8]
	stp	s4, s3, [sp, 16]
	stp	s2, s0, [sp, 24]
	cmp	w3, w9
	ble	.L3
.L7:
	ldr	x6, [x0, x9, lsl 3]
	ldr	x7, [x1, x9, lsl 3]
	add	x5, x6, x14
	prfm	PLDL3KEEP, [x5, 16]
	cmp	x15, 7
	bhi	.L21
	sub	x6, x5, #1
	mov	x5, 1
	b	.L4
	.p2align 2,,3
.L22:
	mov	x5, x8
	ldr	s1, [x12, -4]
.L4:
	ldrb	w11, [x6, x5]
	mov	x8, sp
	add	x10, x8, x5, lsl 2
	add	x8, x5, 1
	mov	x12, sp
	add	x12, x12, x8, lsl 2
	ldr	s0, [x7, x11, lsl 2]
	fadd	s0, s0, s1
	str	s0, [x10, -4]
	cmp	x13, x5
	bne	.L22
	add	x9, x9, 1
	ldr	s1, [sp]
	cmp	w3, w9
	bgt	.L7
.L3:
	sub	x13, x13, #1
	mov	x5, 0
	b	.L9
	.p2align 2,,3
.L23:
	add	x5, x5, 1
	ldr	s1, [x6, 4]
.L9:
	mov	x6, sp
	str	s1, [x4, x5, lsl 2]
	add	x6, x6, x5, lsl 2
	cmp	x13, x5
	bne	.L23
	add	x14, x14, 8
	sub	x15, x15, #8
	add	x4, x4, 32
	cmp	x2, x14
	bhi	.L11
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
.L17:
	ret
	.cfi_endproc
.LFE0:
	.size	_Z12adc_scan_optPPKhPPKfmiPf, .-_Z12adc_scan_optPPKhPPKfmiPf
	.ident	"GCC: (GNU) 10.3.1"
	.section	.note.GNU-stack,"",@progbits
