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

	.area	_HEADER (ABS)
	;; Reset vector
	.org 	0
	jp	init

	.org	0x08
	cp	#0x01
	jp	z,putchar
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

__rst38_vector::

	push	af
	ld	a, (0x1f10)

	cp	#1
	jr	nz, 01$
	pop	af
	jp	__int01_vector_addr

01$:
	cp	#2
	jr	nz, 02$
	pop	af
	jp	__int02_vector_addr

02$:
	cp	#3
	jr	nz, 03$
	pop	af
	jp	__int03_vector_addr

03$:
	cp	#4
	jr	nz, 04$
	pop	af
	jp	__int04_vector_addr

04$:
	cp	#5
	jr	nz, 05$
	pop	af
	jp	__int05_vector_addr

	;; save some space for the NMI vector
	.blkb	0x40

05$:
	cp	#6
	jr	nz, 06$
	pop	af
	jp	__int06_vector_addr

06$:
	cp	#7
	jr	nz, 07$
	pop	af
	jp	__int07_vector_addr

07$:
	cp	#8
	jr	nz, 08$
	pop	af
	jp	__int08_vector_addr

08$:
	cp	#9
	jr	nz, 09$
	pop	af
	jp	__int09_vector_addr

09$:
	cp	#10
	jr	nz, 10$
	pop	af
	jp	__int10_vector_addr

10$:
	cp	#11
	jr	nz, 11$
	pop	af
	jp	__int11_vector_addr

11$:
	cp	#12
	jr	nz, 12$
	pop	af
	jp	__int12_vector_addr

12$:
	cp	#13
	jr	nz, 13$
	pop	af
	jp	__int13_vector_addr

13$:
	cp	#14
	jr	nz, 14$
	pop	af
	jp	__int14_vector_addr

14$:
	cp	#15
	jr	nz, 15$
	pop	af
	jp	__int15_vector_addr

15$:
	cp	#16
	jr	nz, 16$
	pop	af
	jp	__int16_vector_addr

16$:
	pop	af
	ei
	ret


	.org	0x66

__nmi_vector::

	retn


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

	.include	"isr.s"

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

putchar:
	ld	a,(0x1f01)
	and	a,#0x20
	jr	z,putchar
	ld	a,l
	ld	(0x1f00),a
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
