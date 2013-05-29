#include <stdio.h>
#include <string.h>
#include <stdlib.h>


const char hello[] = "Hello, ";


int count = 32;


int talk ()
{
    char name[32];
    char* buffer;

    for (;;)
    {

        puts("Hello! What's your name?\r\n");

        gets(name);

        buffer = malloc(strlen(hello) + strlen(name) + 10);

        _itoa(count++, buffer, 10);
        strcat(buffer, ": ");
        strcat(buffer, hello);
        strcat(buffer, name);
        strcat(buffer, "!\r");

        puts(buffer);
        putchar('\r');
        putchar('\n');
    }
}

int main ()
{
    talk();
}


// vim: ts=4 sw=4 et
