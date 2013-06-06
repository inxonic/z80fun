/*
    z80fun.c - AVR based computer with Z80 processor. Just for fun.

    Copyright (C) 2013  Daniel Balko

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <inttypes.h>

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/atomic.h>


#include "z80rom.h"


#define F_CPU 8000000UL
#define BAUD 1200

#include <util/delay.h>
#include <util/setbaud.h>


#define DEBUG 1

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

#define IO_PORT_MASK 0x42
#define IO_PORT_PORT PORTB
#define IO_PORT_PIN PINB
#define IO_PORT_DDR DDRB

#define INTR_PORT PORTD
#define INTR_DDR DDRD
#define INTR_PIN PD3


uint8_t ram[RAM_SIZE];


int main ()
{
    uint8_t page, addr_lo;

    uint16_t addr;

    uint8_t control;


    if (DEBUG) DEBUG_DDR |= _BV(DEBUG_PIN);


    // *** setup clock port ***

    // CTC mode, toggle OC on compare match, no prescaling
    TCCR0 = _BV(WGM01) | _BV(COM00) | _BV(CS00);

    OCR0 = 16;

    // set PB0(OC0) as output pin
    DDRB |= _BV(PB0);


    // *** setup interrupt timer ***

    // CTC mode, prescaling clk/256
    TCCR1B = _BV(WGM12) | _BV(CS12);

    OCR1AH = 16000UL / 256;
    OCR1AL = 16000UL % 256;

    TIMSK |= _BV(OCIE1A);

    
    // *** setup serial port ***

    UCSRB = _BV(RXEN) | _BV(TXEN);
    UCSRC = _BV(URSEL) | _BV(USBS) | _BV(UCSZ1) | _BV(UCSZ0);

    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;


    // *** setup interrupt pin ***
    
    INTR_PORT |= _BV(INTR_PIN);
    INTR_DDR |= _BV(INTR_PIN);


    // *** setup and release reset pin ***

    RESET_PORT &= ~_BV(RESET_PIN);
    RESET_DDR |= _BV(RESET_PIN);

    _delay_ms(10);
    _delay_ms(10);
    _delay_ms(10);
    _delay_ms(10);

    RESET_PORT |= _BV(RESET_PIN);


    // *** enable interrupts ***
    
    sei();


    // *** main loop ***

    for (;;)
    {
        control = CONTROL_PIN;

        if ( !( control & _BV(RD_PIN) ) )
        {
            //if ( DEBUG ) DEBUG_PORT |= _BV(DEBUG_PIN);

            page = ADDR_HI_PIN;
            addr_lo = ADDR_LO_PIN;
            addr = page << 8 | addr_lo;

            if ( addr >= RAM_ADDR && addr < RAM_ADDR + RAM_SIZE )
            {
                DATA_PORT = ram[addr - RAM_ADDR];
            }

            else if ( page < ROM_PAGES )
            {
                DATA_PORT = pgm_read_byte(&(z80rom[addr]));
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

                else if ( addr_lo == 0x08 )
                {
                    DATA_PORT = IO_PORT_PORT & IO_PORT_MASK;
                }

                else if ( addr_lo == 0x09 )
                {
                    DATA_PORT = IO_PORT_DDR & IO_PORT_MASK;
                }

                else if ( addr_lo == 0x0a )
                {
                    DATA_PORT = IO_PORT_PIN & IO_PORT_MASK;
                }

                else if ( addr_lo == 0x10 )
                {
                    DATA_PORT = (bit_is_set(INTR_PORT, INTR_PIN) ? _BV(1) : 0)
                        | (bit_is_set(SREG, SREG_I) ? _BV(0) : 0);
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

                else if ( addr_lo == 0x08 )
                {
                    //IO_PORT_PORT could be same as INTR_PORT
                    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                    {
                        IO_PORT_PORT = IO_PORT_PORT & ~IO_PORT_MASK | DATA_PIN & IO_PORT_MASK;
                    }

                }

                else if ( addr_lo == 0x09 )
                {
                    if ( DEBUG ) DEBUG_PORT ^= _BV(DEBUG_PIN);

                    IO_PORT_DDR = IO_PORT_DDR & ~IO_PORT_MASK | DATA_PIN & IO_PORT_MASK;
                }

                else if ( addr_lo == 0x10 )
                {
                    INTR_PORT |= _BV(INTR_PIN);
                    sei();
                }
            }

            loop_until_bit_is_set(CONTROL_PIN, WR_PIN);
        }

        //if ( DEBUG ) DEBUG_PORT &= ~_BV(DEBUG_PIN);

    }

}


ISR(BADISR_vect, ISR_NAKED)
{
    INTR_PORT &= ~_BV(INTR_PIN);

    //leave interrupts disabled
    __asm volatile("ret"::);
}


// vim: et si sw=4 ts=4
