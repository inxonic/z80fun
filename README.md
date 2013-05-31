# Z80Fun

The aim of this project is to build a minimalistic computer with Z80
processor based on an Atmel AVR. That is the Z80 will run the code and
the AVR will provide the rest like ROM, RAM and IO.

This is of course just a fun project with little practical sense.

## Status

* The hardware is wired on a breadbord.
* There is AVR code that provides ROM, RAM and access to the USART.
* The system has 6k Bytes of ROM and 384 Bytes of RAM.
* There is a really basic Z80 platform support library.
* There is a Z80 example program written in C that can read and write strings via the USART.

## Hardware

### Schematic

```
 __________________      (+)              (+)      __________________                      
|                  |      |                |      |                  |
|    ATmega8515    |      |                |      |     Z80 CPU      |
|                  |      |                |      |                  |
|         VCC [40] |------+--+        +----+------| [11] +5V         |
|                  |        _|_      _|_   |      |                  |
|                  |        ___      ___   +------| [25] /BUSREQ     |
|                  |    100n |   100n |    +------| [16] /INT        |
|                  |        _|_      _|_   +------| [17] /NMI        |
|                  |                       +------| [18] /HALT       |
|                  |                       +------| [24] /WAIT       |
|                  |                              |                  |
|         PA0 [39] |------------------------------| [30] A0          |
|         PA1 [38] |------------------------------| [31] A1          |
|         PA2 [37] |------------------------------| [32] A2          |
|         PA3 [36] |------------------------------| [33] A3          |
|         PA4 [35] |------------------------------| [34] A4          |
|         PA5 [34] |------------------------------| [35] A5          |
|         PA6 [33] |------------------------------| [36] A6          |
|         PA7 [32] |------------------------------| [37] A7          |
|                  |                              |                  |
|         PE0 [31] |------------------------------| [38] A8          |
|         PE1 [30] |------------------------------| [39] A9          |
|         PE2 [29] |------------------------------| [40] A10         |
|                  |                              |                  |
|         PB2 [ 3] |------------------------------| [ 1] A11         |
|         PB3 [ 4] |------------------------------| [ 2] A12         |
|                  |                              |                  |
|         PC0 [21] |------------------------------| [14] D0          |
|         PC1 [22] |------------------------------| [15] D1          |
|         PC2 [23] |------------------------------| [12] D2          |
|         PC3 [24] |------------------------------| [ 8] D3          |
|         PC4 [25] |------------------------------| [ 7] D4          |
|         PC5 [26] |------------------------------| [ 9] D5          |
|         PC6 [27] |------------------------------| [10] D6          |
|         PC7 [28] |------------------------------| [13] D7          |
|                  |                              |                  |
|     PB0/OC0 [ 1] |------------------------------| [ 6] CLK         |
|                  |                              |                  |
|         PD6 [16] |------------------------------| [22] /WR         |
|         PD7 [17] |------------------------------| [21] /RD         |
|                  |                              |                  |
|         PD4 [14] |------------------+-----------| [26] /RESET      |
|                  |                  |           |                  |
|                  |      LED         |           |                  |
|         PD5 [39] |------|>|--+      |           |                  |
|                  |           |      |           |                  |
|         GND [20] |------+    |      |    +------| [29] GND         |
|__________________|      | 1k5|   10k|    |      |__________________|
                          |    /      /    |
                          |    \      \    |
                          |    /      /    |
                          |    |      |    |
                         _|_  _|_    _|_  _|_
```

Note: I've changed the wiring a bit after commit `027f9489e4`. One must not mix
older AVR code with the current wiring as there are conflicting output pins.

For simplicity I've left out the ISP port and the serial port.

I'm using a STK200 compatible parallel port programmer.

My current serial port implementation is build with an OPAmp
and really needs improvement.

### Board

And that's how it looks like:

![Board Image](../../raw/boardimage/board.jpg)

## Build requirements

### Software requirements

* GNU make
* AVR-GCC cross compiler
* SDCC
* SRecord
* UISP

On a Fedora system simply type:
```
yum install make avr-gcc sdcc srecord uisp
```

### Remarks

On my Fedora system the SDCC binaries are prefixed with `sdcc-`. You might need
to adjust this in the makefile if this isn't the case on your system.

## Build instructions

### Z80 part

Build the Z80 platform support library:
```
make -C z80lib
```

Build the Z80 native code example:
```
make -C z80code z80code.bin
```

### AVR part

Build the AVR part and link it with the Z80 ROM image:
```
make
```

Flash the AVR:
```
make isp
```

You might have to tweak the makefile for other programmers or AVR chips.

## Links

CPU User Manual: http://www.z80.info/zip/z80cpu_um.pdf

ATmega8515 datasheet: www.atmel.com/Images/doc2512.pdf

sdas manual: http://sdcc.svn.sourceforge.net/viewvc/sdcc/trunk/sdcc/sdas/doc/asxhtm.html
