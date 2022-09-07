/* 
 * File:   Lab9Ccode.c
 * Author: Spencer Will
 *
 * Created on July 18, 2022, 11:37 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
/*
 * 
 */
# define _XTAL_FREQ 1000000
void __interrupt() adc(void);

volatile int num;
volatile int voltage;
int conversion; 
int sseg;
int main()
{
    //Setting AN0-3 to Analog
    ADCON1 = 0x0E;
    TRISAbits.RA0 = 1;
    
    //setting port B to output
    TRISB = 0x00;
    // Setting A/d AQ Time and conversion clock
    ADCON2bits.ADCS = 0;
    ADCON2bits.ACQT = 1;
    ADCON2bits.ADFM = 1;
    
    //Setting channel 0 
    ADCON0bits.CHS = 0;

    //Turn A/D on
    ADCON0bits.ADON = 1;
    
    //Configuring Interrupt
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    RCONbits.IPEN = 0;  //no priority
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    PORTB = 0x00;
    
    while(1)
    {
       ADCON0bits.GO = 1; 
       
       //converting voltage to 7 segment
       conversion =  (voltage * 10) / 32;   //multiplied by 10 for remainder, divided by 32 to finish 1024 division
       sseg = ((conversion / 10) * 16) + (conversion % 10);
       PORTB = sseg;
       
    }        
    
    return 0;
}
    
    void __interrupt() adc(void)
    {
        if(PIR1bits.ADIF && PIE1bits.ADIE)
        {
            PIR1bits.ADIF = 0;
    
            num = (ADRESH << 8) | ADRESL;
            
            voltage = (num * 5) / 32;
            //I divided by 32 first because if I divided by 1024 I would lose the remainder
            //If I multiplied by 50 as alternative, the computation is too large for code
        }
        
        
    
}

