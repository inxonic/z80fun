TARGETS=	hello.ihx
OBJECTS=	

CC=		sdcc-sdcc
CFLAGS=		-mz80
LDFLAGS=	-mz80 --code-loc 0x50 --data-loc 0x1800 --no-std-crt0
LDLIBS=		../z80lib/crt0.rel ../z80lib/getchar.rel ../z80lib/putchar.rel


all:		$(TARGETS)


z80code.bin:	$(TARGETS)
		srec_cat -Disable_Sequence_Warnings $(firstword $(TARGETS)) \
		    -Intel -Fill 0 0 0x1800 -Output z80code.bin -Binary

clean:
		$(RM) $(OBJECTS) z80code.bin \
		    $(addsuffix .asm,$(basename $(TARGETS))) \
		    $(addsuffix .ihx,$(basename $(TARGETS))) \
		    $(addsuffix .lst,$(basename $(TARGETS))) \
		    $(addsuffix .map,$(basename $(TARGETS))) \
		    $(addsuffix .noi,$(basename $(TARGETS))) \
		    $(addsuffix .rel,$(basename $(TARGETS))) \
		    $(addsuffix .sym,$(basename $(TARGETS))) \
		    $(addsuffix .lk,$(basename $(TARGETS)))


%.ihx : %.rel
		$(CC) $(LDFLAGS) $(LOADLIBES) -o $(firstword $(TARGETS)) $(LDLIBS) $<

%.rel : %.c
		$(CC) $(CFLAGS) -c $<
