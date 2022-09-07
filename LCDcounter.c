/* 
 * File:   Lab_10LCD.c
 * Author: Spencer Will
 *
 * Created on July 20, 2022, 7:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "LiquidCrystal.h"

#define _XTAL_FREQ 1000000
#define RESET PORTBbits.RB2

/*
 * 
 */

void __interrupt(high_priority) incrementLCD(void);
void __interrupt(low_priority) decrementLCD(void);

volatile int num = 0;
int main() 
{
    
    ADCON1 = 0x0f;
    TRISA = 0x00;
    TRISD = 0x00;
    TRISB = 0xff;
    
  
    INTCONbits.GIE = 1; // Global interrupt enable
    INTCONbits.INT0E = 1; // Enable INT0
    INTCONbits.INT0IF = 0; // reset INT0 flag
    INTCON2bits.INTEDG0 = 0;
    INTCON3bits.INT1IE = 1; // Enable INT1
    INTCON3bits.INT1IF = 0; // reset INT1 flag
    // falling edge
    
    RCONbits.IPEN = 1; // enable priority
    INTCONbits.GIEH = 1; // enable all high priority
    INTCONbits.GIEL = 1; // enable all low priority
    INTCON3bits.INT1IP = 0; // Set INT1 to low priority 
    
    pin_setup(&PORTA, &PORTD);
    
            
    begin(16, 2, LCD_5x8DOTS);
    
    while(1)
    {
        clear();
        print_int(num);
        __delay_ms(100);
        
        if (RESET == 0)
        {
            clear();
            num = 0;
        }
    }
    return 0;
}

void __interrupt(high_priority) incrementLCD(void)
{
    
    if (INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        // Turn off the interrupt flag to avoid recursive interrupt calls
        INTCONbits.INT0IF = 0;
        
        num = num++;
    }
    return;
}

void __interrupt(low_priority) decrementLCD(void)
{
     if (INTCON3bits.INT1IE && INTCON3bits.INT1IF)
    {
        // Turn off the interrupt flag to avoid recursive interrupt calls
        INTCON3bits.INT1IF = 0;
        
        num = num--;
    }
    return;
}

