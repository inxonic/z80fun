# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


TARGETS=	hello.ihx
OBJECTS=	hello.rel

CC=		sdcc-sdcc

CFLAGS=		-mz80
LDFLAGS=	-mz80 --code-loc 0x200 --data-loc 0x1800 -Wl"-g __stack_loc = 0x1980" --no-std-crt0
LDLIBS=		-L../z80lib -lz80lib ../z80lib/crt0.rel


all:		$(TARGETS)

clean:
		$(RM) z80code.bin $(TARGETS) $(OBJECTS) \
		    $(addsuffix .asm,$(basename $(OBJECTS))) \
		    $(addsuffix .lst,$(basename $(OBJECTS))) \
		    $(addsuffix .map,$(basename $(OBJECTS))) \
		    $(addsuffix .noi,$(basename $(OBJECTS))) \
		    $(addsuffix .sym,$(basename $(OBJECTS))) \
		    $(addsuffix .lk,$(basename $(OBJECTS)))

%.ihx : %.rel
		$(CC) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -o $@

%.rel : %.c
		$(CC) $(CFLAGS) -c $<


z80code.bin:	$(TARGETS)
		srec_cat -Disable_Sequence_Warnings $(firstword $(TARGETS)) \
		    -Intel -Fill 0 0 0x1800 -Output z80code.bin -Binary
