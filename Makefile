# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


MCU=		atmega8515
DPROG=		stk200

ISP_TARGET=	z80fun.ihx

Z80ROM=		z80code/hello.ihx

TARGETS=	$(ISP_TARGET)
OBJECTS=	z80rom.o z80fun.o

CC=		avr-gcc

CFLAGS=		-mmcu=$(MCU) -Os -std=c99
LDFLAGS=	-mmcu=$(MCU)

OBJCOPY=	avr-objcopy
OBJCOPYFLAGS=	-R .eeprom

UISP=		uisp
UISPFLAGS=	-dprog=$(DPROG) -dpart=$(MCU)


all:		$(TARGETS)


z80fun.elf:	$(OBJECTS)

z80fun.o:	z80rom.h

$(Z80ROM):
		$(error missing $(Z80ROM))

z80rom.bin:	$(Z80ROM)
		srec_cat -Disable_Sequence_Warnings $< \
		    -Intel -Fill 0 0 0x1800 -Output $@ -Binary


%.elf : %.o
		$(CC) $(LDFLAGS) -o $@ $^

%.ihx : %.elf
		$(OBJCOPY) $(OBJCOPYFLAGS) -O ihex $< $@

isp:		$(ISP_TARGET)
		$(UISP) $(UISPFLAGS) --erase
		$(UISP) $(UISPFLAGS) --upload --verify if=$(ISP_TARGET)

reset:
		$(UISP) $(UISPFLAGS)

clean:
		rm -f $(TARGETS) *.elf *.o z80rom.bin z80rom.h


# http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_binarydata

%.o %.h : %.txt
	@echo Converting $< to $(*).o
	@cp $(<) $(*).tmp
	@echo -n 0 | tr 0 '\000' >> $(*).tmp
	@$(OBJCOPY) -I binary -O elf32-avr \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_start=$(notdir $*) \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_end=$(notdir $*)_end \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_size=$(notdir $*)_size_sym \
	$(*).tmp $(*).o
	@echo "extern const char" $(notdir $*)"[] PROGMEM;" > $(*).h
	@echo "extern const char" $(notdir $*)_end"[] PROGMEM;" >> $(*).h
	@echo "extern const char" $(notdir $*)_size_sym"[];" >> $(*).h
	@echo "#define $(notdir $*)_size ((int)$(notdir *)_size_sym)" >> $(*).h
	@rm $(*).tmp

%.o %.h : %.bin
	@echo Converting $< to $(*).o
	@$(OBJCOPY) -I binary -O elf32-avr \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(subst /,_,$*)_bin_start=$(notdir $*) \
	--redefine-sym _binary_$(subst /,_,$*)_bin_end=$(notdir $*)_end \
	--redefine-sym _binary_$(subst /,_,$*)_bin_size=$(notdir $*)_size_sym \
	$(<) $(*).o
	@echo "extern const char" $(notdir $*)"[] PROGMEM;" > $(*).h
	@echo "extern const char" $(notdir $*)_end"[] PROGMEM;" >> $(*).h
	@echo "extern const char" $(notdir $(*))_size_sym"[];" >> $(*).h
	@echo "#define $(notdir $*)_size ((int)$(notdir $*)_size_sym)" >> $(*).h
