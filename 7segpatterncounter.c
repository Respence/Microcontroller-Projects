/* 
 * File:   MidtermC.c
 * Author: Spencer Will
 *
 * Created on July 14, 2022, 12:22 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#define _XTAL_FREQ 1000000
/*
 * 
 */
#define SWITCH1 PORTDbits.RD0
#define SWITCH2 PORTDbits.RD1
int main()
{
    char sseg[6] = {0xCF, 0xE7, 0xF3, 0xF9, 0x7D, 0x5F}; //Port RA6 isnt working so changed outputs
    
    ADCON1 = 0xff;
    TRISA  = 0x00;
    TRISD  = 0xff;
    
    
    char counter1 = 0;
    while(1)
    {
        if (SWITCH1 == 0)
        {
            if (SWITCH2 == 0)
            {
                PORTA = 0xFF;
            }
            else
            {
                __delay_ms (500);
                PORTA = sseg[counter1];
                counter1 = (counter1 + 1) % 6 ;
            }
        }    
        else 
        {
            if (SWITCH2 == 0)
            {
                PORTA = 0xFE;
                __delay_ms (250);
                PORTA = 0xFF;
                __delay_ms (250);
            }
            else
            {
                for (counter1 = 5;counter1 <=6; counter1--)
                {
                __delay_ms (500);
                PORTA = sseg[counter1];
                }
     
            }
            
        }         
           
    }    
    return (EXIT_SUCCESS);
}

