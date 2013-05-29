#include <inttypes.h>

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>


#include "z80code/z80code.h"


#define F_CPU 8000000UL
#define BAUD 1200

#include <util/delay.h>
#include <util/setbaud.h>


#define ADDR_LO_PIN PINA
#define ADDR_HI_PIN PINE
#define ADDR_HI_MAX 0x07

#define DATA_PORT PORTC
#define DATA_PIN PINC
#define DATA_DDR DDRC

#define RESET_PORT PORTD
#define RESET_DDR DDRD
#define RESET_PIN PD6

#define DEBUG_PORT PORTD
#define DEBUG_DDR DDRD
#define DEBUG_PIN PD5

#define CONTROL_PORT PORTD
#define CONTROL_PIN PIND
#define CONTROL_DDR DDRD

#define WR_PIN PD2
#define RD_PIN PD3

#define MREQ_PIN PD7

#define ROM_PAGES 4

#define RAM_PAGES 1
#define RAM_START 4

#define MMIO_PAGE 7


uint8_t ram[RAM_PAGES][256];


int main ()
{
    uint8_t addr_hi, addr_lo;

    uint8_t control;


    // *** setup clock port ***

    // CTC mode, toggle OC on compare match, no prescaling
    TCCR0 = _BV(WGM01) | _BV(COM00) | _BV(CS00);

    OCR0 = 16;

    // set PB0(OC0) as output pin
    DDRB |= _BV(0);

    
    // *** setup control bus ports ***

    RESET_PORT &= ~_BV(RESET_PIN);
    RESET_DDR |= _BV(RESET_PIN);

    DEBUG_DDR |= _BV(DEBUG_PIN);

    _delay_ms(10);
    _delay_ms(10);
    _delay_ms(10);
    _delay_ms(10);

    RESET_PORT |= _BV(RESET_PIN);


    // *** setup serial port ***

    UCSRB = _BV(RXEN) | _BV(TXEN);
    UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);

    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;


    // *** main loop ***

    for (;;)
    {
	control = CONTROL_PIN;

	//DEBUG_PORT |= _BV(DEBUG_PIN);

	if ( !( control & ( _BV(MREQ_PIN) | _BV(RD_PIN) ) ) )
	{
	    addr_lo = ADDR_LO_PIN;
	    addr_hi = ADDR_HI_PIN & ADDR_HI_MAX;

	    if ( addr_hi == MMIO_PAGE )
	    {
	        if ( addr_lo == 0x00 )
		{
		    DATA_PORT = UDR;
		}

	        else if ( addr_lo == 0x01 )
		{
		    DATA_PORT = UCSRA;
		}
	    }

	    else if ( addr_hi >= RAM_START && addr_hi < RAM_START + RAM_PAGES )
	    {
	        DATA_PORT = ram[addr_hi - RAM_START][addr_lo];
	    }

	    else if ( addr_hi < ROM_PAGES )
	    {
	        DATA_PORT = pgm_read_byte(&(z80code[addr_hi << 8 | addr_lo]));
	    }

	    DATA_DDR = 0xff;

	    loop_until_bit_is_set(CONTROL_PIN, RD_PIN);

	    DATA_DDR = 0x00;
	    DATA_PORT = 0x00;
	}

	else if ( !( control & ( _BV(MREQ_PIN) | _BV(WR_PIN) ) ) )
	{
	    addr_lo = ADDR_LO_PIN;
	    addr_hi = ADDR_HI_PIN & ADDR_HI_MAX;

	    if ( addr_hi == MMIO_PAGE )
	    {
	        if ( addr_lo == 0x00 )
		{
		    UDR = DATA_PIN;
		}

	        else if ( addr_lo == 0x01 )
		{
		    UCSRA = DATA_PIN & ~_BV(U2X) & ~_BV(MPCM);
		}
	    }

	    else if ( addr_hi >= RAM_START && addr_hi < RAM_START + RAM_PAGES )
	    {
	        ram[addr_hi - RAM_START][addr_lo] = DATA_PIN;
	    }

	    loop_until_bit_is_set(CONTROL_PIN, WR_PIN);
	}

	//DEBUG_PORT &= ~_BV(DEBUG_PIN);

    }

}
