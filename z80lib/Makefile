# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


TARGETS=	crt0.rel z80lib.lib
Z80LIB_OBJECTS=	getchar.rel isr.rel

AS=		sdcc-sdasz80
AR=		sdcc-sdar


all:		$(TARGETS)

clean:
		$(RM) $(TARGETS) *.lst *.rel *.sym

%.rel : %.s
		$(AS) $(ASFLAGS) -los $@ $<


z80lib.lib:	z80lib.lib($(Z80LIB_OBJECTS))
