/*
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static unsigned char count = 32;


int talk ()
{
    char name[32];

    __asm
        ld  a,#0x02
        ld  (0x1f09),a
        ld  (0x1f10),a
        ei
    __endasm;

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


// vim: et si sw=4 ts=4
