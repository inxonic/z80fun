
	.area	_INT04_VECTOR

__int04_vector::

	push	af
	ld	a,(0x1f08)
	inc	a
	inc	a
	ld	(0x1f08),a
	pop	af
	ei
	ret
