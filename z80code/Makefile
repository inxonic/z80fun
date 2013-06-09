# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


TARGETS=	hello.ihx
HELLO_OBJECTS=	hello.rel isr.rel

AS=		sdcc-sdasz80

CC=		sdcc-sdcc

CFLAGS=		-mz80 -I../z80lib
LDFLAGS=	-mz80 --code-loc 0x200 --data-loc 0x1800 -Wl"-g __stack_loc = 0x1980" --no-std-crt0
LDLIBS=		-L../z80lib -lz80lib ../z80lib/crt0.rel


all:		$(TARGETS)

hello.ihx:	$(HELLO_OBJECTS)

clean:
		$(RM) $(TARGETS) *.rel *.asm *.lst *.map *.noi *.sym *.lk

%.ihx : %.rel
		$(CC) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -o $@

%.rel : %.s
		$(AS) $(ASFLAGS) -los $@ $<

%.rel : %.c
		$(CC) $(CFLAGS) -c $<
