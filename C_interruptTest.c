/* 
 * File:   Lab8C.c
 * Author: Spencer Will
 *
 * Created on July 14, 2022, 11:23 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
/*
 * 
 */
# define _XTAL_FREQ 1000000 
# define LED          PORTDbits.RD1
# define COMPARATOR   PORTBbits.RB1

void __interrupt() TurnONLed(void);

int main() 
{
    
    // All AN inputs are digital
    ADCON1 = 0x0f;
    
    // PORTC is output (connected to LED)
    TRISC = 0x00;
    
    // RB0 is input (connected to comparator)
    TRISB = 0xff;
    
    // Setup all interrupts
    INTCONbits.GIE = 1; // Globabl interrupt enable
    INTCONbits.INT0E = 1; // Enable INT0
    INTCONbits.INT0IF = 0; // reset INT0 flag
    
    INTCON2bits.INTEDG0 = 0; // falling edge
    // INTCON2bits.INTEDG0 = 1; // rising edge
    while (1)
    {    
        LED = 0;
    }
    return (EXIT_SUCCESS);
}

void __interrupt() TurnONLed(void)
{
    // test which interrupt called this Interrupt Service Routine
    
    // INT0
    if (INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        // Turn off the interrupt flag to avoid recursive interrupt calls
        INTCONbits.INT0IF = 0;
       
        LED = ~LED;

    }

    return;

}
