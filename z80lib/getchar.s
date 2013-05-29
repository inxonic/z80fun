	.area _CODE
_getchar::
	ld	a, (0x0701)
	and	a, #0x80
	jr	z, _getchar
	ld	a, (0x700)
	ld	l, a
	ret
