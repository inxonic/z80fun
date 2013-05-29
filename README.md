# Z80Fun

## Purpose

The aim of this project is to build a minimalistic computer with Z80
processor based on an Atmel AVR. That is the Z80 will run the code and
the AVR will provide the rest like ROM, RAM and IO.

This is of course just a fun project with little practical sense.

## Schematic

This is my hardware setup:

```
 __________________      (+)              (+)      __________________                      
|                  |      |                |      |                  |
|    ATmega8515    |      |                |      |     Z80 CPU      |
|                  |      |                |      |                  |
|         VCC [40] |------+                +------| [11] +5V         |
|                  |                       |      |                  |
|                  |                       +------| [25] /BUSREQ     |
|                  |                       +------| [16] /INT        |
|                  |                       +------| [17] /NMI        |
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
|    PD2/INT0 [39] |------------------------------| [22] /WR         |
|    PD3/INT1 [39] |------------------------------| [21] /RD         |
|         PD7 [39] |------------------------------| [19] /MREQ       |
|                  |                              |                  |
|         PD6 [39] |------------------+-----------| [26] /RESET      |
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

For simplicity I've left out the ISP port and the serial port.

## Build requirements

### Software requirements

* GNU make
* AVR-GCC cross compiler
* SDCC
* SRecord

On a Fedora system simply type:
    yum install make avr-gcc sdcc srecord

### Remarks

On my Fedora system the SDCC binaries are prefixed with sdcc-. You might need
to adjust this if this isn't the case on your system.

## Build instructions

### Z80 part

Build the Z80 platform support library:
    make -C z80lib

Build the Z80 native code example:
    make -C z80code z80code.bin

### AVR part

Build the AVR part and link it with the Z80 ROM image:
    make all

## Links

CPU User Manual: http://www.z80.info/zip/z80cpu_um.pdf

ATmega8515 datasheet: www.atmel.com/Images/doc2512.pdf
