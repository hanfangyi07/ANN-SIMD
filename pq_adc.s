	.arch armv8-a
	.file	"pq_adc.cc"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z8adc_scanRKSt6vectorIS_IhSaIhEESaIS1_EERKS_IS_IfSaIfEESaIS7_EEmiPf
	.type	_Z8adc_scanRKSt6vectorIS_IhSaIhEESaIS1_EERKS_IS_IfSaIfEESaIS7_EEmiPf, %function
_Z8adc_scanRKSt6vectorIS_IhSaIhEESaIS1_EERKS_IS_IfSaIfEESaIS7_EEmiPf:
.LFB882:
	.cfi_startproc
	cbz	x2, .L1
	sub	w5, w3, #1
	mov	x8, 0
	add	x5, x5, 1
	add	x5, x5, x5, lsl 1
	lsl	x9, x5, 3
	.p2align 3,,7
.L5:
	movi	v0.2s, #0
	cmp	w3, 0
	ble	.L3
	ldr	x11, [x0]
	mov	x5, 0
	ldr	x10, [x1]
	.p2align 3,,7
.L4:
	ldr	x7, [x11, x5]
	ldr	x6, [x10, x5]
	add	x5, x5, 24
	ldrb	w7, [x7, x8]
	ldr	s1, [x6, x7, lsl 2]
	fadd	s0, s0, s1
	cmp	x9, x5
	bne	.L4
.L3:
	str	s0, [x4, x8, lsl 2]
	add	x8, x8, 1
	cmp	x2, x8
	bne	.L5
.L1:
	ret
	.cfi_endproc
.LFE882:
	.size	_Z8adc_scanRKSt6vectorIS_IhSaIhEESaIS1_EERKS_IS_IfSaIfEESaIS7_EEmiPf, .-_Z8adc_scanRKSt6vectorIS_IhSaIhEESaIS1_EERKS_IS_IfSaIfEESaIS7_EEmiPf
	.ident	"GCC: (GNU) 10.3.1"
	.section	.note.GNU-stack,"",@progbits
