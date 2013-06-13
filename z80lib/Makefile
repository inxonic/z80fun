# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


TARGETS=	crt0.rel z80lib.lib
Z80LIB_OBJECTS=	getchar.rel mmio.rel

AS=		sdcc-sdasz80

AR=		sdcc-sdar

CC=		sdcc-sdcc
CFLAGS=		-mz80


all:		$(TARGETS)

clean:
		$(RM) $(TARGETS) *.rel *.asm *.lst *.map *.noi *.sym *.lk

%.rel : %.s
		$(AS) $(ASFLAGS) -los $@ $<

%.rel : %.c
		$(CC) $(CFLAGS) -c $<


z80lib.lib:	z80lib.lib($(Z80LIB_OBJECTS))
