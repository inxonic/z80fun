;--------------------------------------------------------------------------
;  getchar.s - getchar implementation for the Z80fun system
;
;  Copying and distribution of this file, with or without modification,
;  are permitted in any medium without royalty provided the copyright
;  notice and this notice are preserved.  This file is offered as-is,
;  without any warranty.
;--------------------------------------------------------------------------

	.area _CODE
_getchar::
	ld	a, (0x1f01)
	and	a, #0x80
	jr	z, _getchar
	ld	a, (0x1f00)
	ld	l, a
	ret
