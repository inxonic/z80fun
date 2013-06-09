;--------------------------------------------------------------------------
;  crt0.s - Customized crt0.s for the Z80Fun system
;
;  This is a modified version of:
;  http://sourceforge.net/p/sdcc/code/HEAD/tree/tags/sdcc-3.2.0/sdcc/device/lib/z80/crt0.s
;--------------------------------------------------------------------------

;--------------------------------------------------------------------------
;  crt0.s - Generic crt0.s for a Z80
;
;  Copyright (C) 2000, Michael Hope
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

        .module crt0
       	.globl	_main

	.globl	_INT0_vect_crt
	.globl	_INT1_vect_crt
	.globl	_TIMER1_COMPA_vect_crt

	.area	_HEADER (ABS)
	;; Reset vector
	.org 	0
	jp	init

	.org	0x08
	cp	#0x01
	jr	z,putchar
	reti

	.org	0x10
	reti
	.org	0x18
	reti
	.org	0x20
	reti
	.org	0x28
	reti
	.org	0x30
	reti

	.org	0x38
	push	af
	ld	a,(0x1f10)
	cp	#1
	jr	nz,1$
	call	_INT0_vect_crt
	jr	99$
1$:	
	cp	#2
	jr	nz,2$
	call	_INT1_vect_crt
	jr	99$
2$:	
	cp	#4
	jr	nz,99$
	call	_TIMER1_COMPA_vect_crt
99$:
	pop	af
	ei
	ret

	.org	0x66
	retn

putchar:
	ld	a,(0x1f01)
	and	a,#0x20
	jr	z,putchar
	ld	a,l
	ld	(0x1f00),a
	ret

	.org	0x100
init:
	.globl	__stack_loc
	ld	sp,#__stack_loc
	im	1

        ;; Initialise global variables
        call    gsinit
	call	_main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE

	.area	_ISRSTART_INT0
_INT0_vect_crt::
	.area	_ISR_INT0
	.area	_ISRFINAL_INT0
	ret

	.area	_ISRSTART_INT1
_INT1_vect_crt::
	.area	_ISR_INT1
	.area	_ISRFINAL_INT1
	ret

	.area	_ISRSTART_TIMER1_COMPA
_TIMER1_COMPA_vect_crt::
	.area	_ISR_TIMER1_COMPA
	.area	_ISRFINAL_TIMER1_COMPA
	ret

        .area   _GSINIT
        .area   _GSFINAL

	.area	_DATA
	.area	_BSEG
        .area   _BSS
        .area   _HEAP

        .area   _CODE
__clock::
	ld	a,#2
        rst     0x08
	ret

_exit::
	;; Exit - special code to the emulator
	ld	a,#0
        rst     0x08
1$:
	halt
	jr	1$

        .area   _GSINIT
gsinit::

        .area   _GSFINAL
        ret
