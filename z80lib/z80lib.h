/*
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
*/


#define di() __asm__("di")
#define ei() __asm__("ei")


extern volatile unsigned char IO_PORT_DDR;

extern volatile unsigned char INTR_CTRL;
