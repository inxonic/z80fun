	.area _CODE
_getchar::
	ld	a, (0x1f01)
	and	a, #0x80
	jr	z, _getchar
	ld	a, (0x1f00)
	ld	l, a
	ret
