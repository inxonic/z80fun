	.area	_ISR_INT0
	ld	a,(0x1f0a)
	ret

	.area	_ISR_TIMER1_COMPA
	ld	a,(0x1f08)
	inc	a
	inc	a
	ld	(0x1f08),a
	ret
