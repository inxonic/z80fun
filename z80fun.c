#include <inttypes.h>

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>


#include "z80code/z80code.h"


#define F_CPU 8000000UL
#define BAUD 1200

#include <util/delay.h>
#include <util/setbaud.h>


#define DEBUG 0

#define ADDR_LO_PIN PINA
#define ADDR_HI_PIN (PINE & 0x7 | (PINB & 0xc) << 1)

#define DATA_PORT PORTC
#define DATA_PIN PINC
#define DATA_DDR DDRC

#define RESET_PORT PORTD
#define RESET_DDR DDRD
#define RESET_PIN PD4

#define DEBUG_PORT PORTD
#define DEBUG_DDR DDRD
#define DEBUG_PIN PD5

#define CONTROL_PORT PORTD
#define CONTROL_PIN PIND
#define CONTROL_DDR DDRD

#define WR_PIN PD6
#define RD_PIN PD7

#define ROM_PAGES 0x18

#define MMIO_PAGE 0x1f

#define RAM_ADDR 0x1800
#define RAM_SIZE 0x180


uint8_t ram[RAM_SIZE];


int main ()
{
    uint8_t page, addr_lo;

    uint16_t addr;

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

    if ( DEBUG ) DEBUG_DDR |= _BV(DEBUG_PIN);

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

        if ( !( control & _BV(RD_PIN) ) )
        {
            if ( DEBUG ) DEBUG_PORT |= _BV(DEBUG_PIN);

            page = ADDR_HI_PIN;
            addr_lo = ADDR_LO_PIN;
            addr = page << 8 | addr_lo;

            if ( addr >= RAM_ADDR && addr < RAM_ADDR + RAM_SIZE )
            {
                DATA_PORT = ram[addr - RAM_ADDR];
            }

            else if ( page < ROM_PAGES )
            {
                DATA_PORT = pgm_read_byte(&(z80code[addr]));
            }

            else if ( page == MMIO_PAGE )
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

            DATA_DDR = 0xff;

            loop_until_bit_is_set(CONTROL_PIN, RD_PIN);

            DATA_DDR = 0x00;
            DATA_PORT = 0x00;
        }

        else if ( !( control & _BV(WR_PIN) ) )
        {
            page = ADDR_HI_PIN;
            addr_lo = ADDR_LO_PIN;
            addr = page << 8 | addr_lo;

            if ( addr >= RAM_ADDR && addr < RAM_ADDR + RAM_SIZE )
            {
                ram[addr - RAM_ADDR] = DATA_PIN;
            }

            else if ( page == MMIO_PAGE )
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

            loop_until_bit_is_set(CONTROL_PIN, WR_PIN);
        }

        if ( DEBUG ) DEBUG_PORT &= ~_BV(DEBUG_PIN);

    }

}


// vim: ts=4 sw=4 et
