/*
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int count = 32;


int talk ()
{
    char name[32];

    for (;;)
    {

        printf("%05d: Hello! What's your name?\r\n", count);

        gets(name);

        printf("%05d: Hello, %s!\r\n\r\n", count, name);
    }
}

int main ()
{
    talk();

    return 0;
}


// vim: ts=4 sw=4 et
