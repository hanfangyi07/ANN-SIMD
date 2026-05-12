	.arch armv8-a
	.file	"sq_dot.cc"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z13dot_int8_neonPKaS0_m
	.type	_Z13dot_int8_neonPKaS0_m, %function
_Z13dot_int8_neonPKaS0_m:
.LFB4266:
	.cfi_startproc
	movi	v1.4s, 0
	mov	x6, x0
	cmp	x2, 15
	bls	.L6
	sub	x5, x0, #16
	sub	x0, x1, #16
	mov	x4, 16
	.p2align 3,,7
.L3:
	ldr	q0, [x5, x4]
	mov	x3, x4
	ldr	q2, [x0, x4]
	add	x4, x4, 16
	dup	d3, v0.d[0]
	dup	d0, v0.d[1]
	dup	d4, v2.d[0]
	dup	d2, v2.d[1]
	smull	v3.8h, v3.8b, v4.8b
	smull	v0.8h, v0.8b, v2.8b
#APP
// 9497 "/usr/lib/gcc/aarch64-linux-gnu/10.3.1/include/arm_neon.h" 1
	sadalp v1.4s,v3.8h
// 0 "" 2
// 9497 "/usr/lib/gcc/aarch64-linux-gnu/10.3.1/include/arm_neon.h" 1
	sadalp v1.4s,v0.8h
// 0 "" 2
#NO_APP
	cmp	x2, x4
	bcs	.L3
.L2:
	addv	s1, v1.4s
	umov	w0, v1.s[0]
	cmp	x2, x3
	bls	.L1
	.p2align 3,,7
.L5:
	ldrsb	w5, [x6, x3]
	ldrsb	w4, [x1, x3]
	add	x3, x3, 1
	madd	w0, w5, w4, w0
	cmp	x2, x3
	bne	.L5
.L1:
	ret
	.p2align 2,,3
.L6:
	mov	x3, 0
	b	.L2
	.cfi_endproc
.LFE4266:
	.size	_Z13dot_int8_neonPKaS0_m, .-_Z13dot_int8_neonPKaS0_m
	.ident	"GCC: (GNU) 10.3.1"
	.section	.note.GNU-stack,"",@progbits
