MCU=atmega8515

DPROG=stk200

OBJECTS=z80fun.o
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
