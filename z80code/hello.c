/*
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <z80lib.h>


static unsigned char count = 32;


int talk ()
{
    char name[32];


    IO_PORT_DDR = 0x02;

    INTR_CTRL = 0x00;

    ei();


    for (;;)
    {
        printf("%05d: Hello! What's your name?\r\n", count);

        gets(name);

        printf("%05d: Hello, %s!\r\n\r\n", count, name);

        count += 2;
    }
}

int main ()
{
    talk();

    return 0;
}


void _int01_vector(void) __interrupt __naked
{
    __asm
        .area   _INT01_VECTOR
        push    af
        ld      a,(0x1f0a)
        pop     af
        ei
        ret
    __endasm;
}


// vim: et si sw=4 ts=4
