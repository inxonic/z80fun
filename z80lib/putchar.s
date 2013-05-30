;--------------------------------------------------------------------------
;  putchar.s - Customized putchar implementation for the Z80Fun computer
;
;  This is a modified version of:
;  http://sourceforge.net/p/sdcc/code/HEAD/tree/tags/sdcc-3.2.0/sdcc/device/lib/z80/putchar.s
;--------------------------------------------------------------------------

;--------------------------------------------------------------------------
;  putchar.s
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

        .area   _CODE
_putchar::
_putchar_rr_s::
        ld      hl,#2
        add     hl,sp
0$:
        ld      a,(0x1f01)
        and     a,#0x20
        jr      z,0$
        ld      a,(hl)
        ld      (0x1f00),a
        ret

_putchar_rr_dbs::
0$:
        ld      a,(0x1f01)
        and     a,#0x20
        jr      z,_putchar_rr_dbs
        ld      a,e
        ld      (0x1f00),a
        ret
