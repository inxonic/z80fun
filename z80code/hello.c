#include <stdio.h>


int qwer = 32;


int asdf ()
{
  char c;
  char s[16];

  for (;;)
  {
    /*
    __asm
	ld	hl, #0x0400
	00001$:
	ld	a, (0x0701)
	and	a, #0x80
	jr	z, 00001$
	ld	a, (0x0700)
	ld	(hl), a
	inc	(hl)
	00002$:
	ld	a, (0x0701)
	and	a, #0x20
	jr	z, 00002$
	ld	a, (hl)
	ld	(0x0700), a
    __endasm;

    putchar(' ');
    putchar('a');
    putchar('b');
    putchar('\r');
    putchar('\n');

    c = getchar();
    putchar(c);
    putchar(' ');
    */

    puts("Hallo, Welt!\r\n");

    gets(s);
    puts(s);
    putchar('\r');
    putchar('\n');
  }
}

int main ()
{
    asdf();
}
