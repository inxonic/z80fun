	.area	_CODE

_INT0_vect::
	ld	a,(0x1f0a)
	ld	a,(0x1f08)
	inc	a
	inc	a
	ld	(0x1f08),a
	ret

_TIMER1_COMPA_vect::
	ld	a,(0x1f08)
	inc	a
	inc	a
	ld	(0x1f08),a
	ret
