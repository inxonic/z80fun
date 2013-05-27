MCU=atmega8515

DPROG=stk200

OBJECTS=z80code/z80code.o z80fun.o
ELFFILE=z80fun.elf
HEXFILE=z80fun.hex
DFLACT=elf

CC=avr-gcc

CFLAGS=-mmcu=${MCU} -Os
LDFLAGS=-mmcu=${MCU}

OBJCOPY=avr-objcopy
OBJCOPYFLAGS=-R .eeprom -O ihex

UISP=uisp
UISPFLAGS=-dprog=${DPROG} -dpart=${MCU}


all: ${DFLACT}


elf: ${OBJECTS}
	${CC} ${LDFLAGS} -o ${ELFFILE} ${OBJECTS}

hex: elf
	${OBJCOPY} ${OBJCOPYFLAGS} ${ELFFILE} ${HEXFILE}

isp: hex
	${UISP} ${UISPFLAGS} --erase
	${UISP} ${UISPFLAGS} --upload --verify if=${HEXFILE}

clean:
	rm -f *.o ${ELFFILE} ${HEXFILE}


# http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_binarydata

%.o : %.txt
	@echo Converting $<
	@cp $(<) $(*).tmp
	@echo -n 0 | tr 0 '\000' >> $(*).tmp
	@$(OBJCOPY) -I binary -O elf32-avr \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_start=$(notdir $*) \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_end=$(notdir $*)_end \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_size=$(notdir $*)_size_sym \
	$(*).tmp $(@)
	@echo "extern const char" $(notdir $*)"[] PROGMEM;" > $(*).h
	@echo "extern const char" $(notdir $*)_end"[] PROGMEM;" >> $(*).h
	@echo "extern const char" $(notdir $*)_size_sym"[];" >> $(*).h
	@echo "#define $(notdir $*)_size ((int)$(notdir *)_size_sym)" >> $(*).h
	@rm $(*).tmp

%.o : %.bin
	@echo Converting $<
	@$(OBJCOPY) -I binary -O elf32-avr \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(subst /,_,$*)_bin_start=$(notdir $*) \
	--redefine-sym _binary_$(subst /,_,$*)_bin_end=$(notdir $*)_end \
	--redefine-sym _binary_$(subst /,_,$*)_bin_size=$(notdir $*)_size_sym \
	$(<) $(@)
	@echo "extern const char" $(notdir $*)"[] PROGMEM;" > $(*).h
	@echo "extern const char" $(notdir $*)_end"[] PROGMEM;" >> $(*).h
	@echo "extern const char" $(notdir $(*))_size_sym"[];" >> $(*).h
	@echo "#define $(notdir $*)_size ((int)$(notdir $*)_size_sym)" >> $(*).h
