/* 
 * File:   Midterm2.c
 * Author: Spencer Will
 *
 * Created on July 25, 2022, 4:31 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "LiquidCrystal.h"
#include "config.h"

#define _XTAL_FREQ 1000000
void __interrupt() adc(void);

/*
 * 
 */

#define SWITCH1 PORTDbits.RD3
#define SWITCH2 PORTDbits.RD4


volatile int unsignednum, num1;
int  num2, answerpt2;
double answer;
int main() 
{
    
    ADCON1 = 0x0E;  //Setting AN0-3 to Analog
    TRISA = 0xff;   //input from potentiometers
    TRISD = 0xF8;   // signal pins for LCD
    TRISB = 0x00;   // data pins for LCD
    
    // Setting A/d AQ Time and conversion clock
    ADCON2bits.ADCS = 0;
    ADCON2bits.ACQT = 1;
    ADCON2bits.ADFM = 1;
    
    //Configuring Interrupt
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    RCONbits.IPEN = 0;  //no priority
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
          
    pin_setup(&PORTB, &PORTD);
            
    begin(16, 2, LCD_5x8DOTS);
    
    while(1)
    {
        ADCON0 = 0x04;  //reading AN1 first to gather first digit from potentiometer.
        ADCON0 = 0x05;
        ADCON0 = 0x07;
        __delay_ms(2);  //does not gather first digit without this delay
    
        num2 = num1;    //sending first adc to integer for later use
        
        ADCON0 = 0x00;  //reading AN0 to gather second digit
        ADCON0 = 0x01;
        ADCON0 = 0x03;
        
        if (SWITCH1 == 1)
        {
            if(SWITCH2 == 1)  //case 0: display is off
            {
                clear();  
            }
            else            // case 2: subtraction
            {
                answer = num2 - num1;
                home();
                print("(");
                print_int(num2);
                print(")-(");
                print_int(num1);
                print(")=       ");
                setCursor(0,1);
                print_int(answer);
                print("      ");
            }                     
        }
        else
        {
            if (SWITCH2 == 1)  // case 1: addition
            {
                answer = num2 + num1;
                home();
                print("(");
                print_int(num2);
                print(")+(");
                print_int(num1);
                print(")=       ");
                setCursor(0,1);
                print_int(answer);
                print("      ");
            }
            else             // case 3: multiplication
            {
                double a, b;  // janky method i found to output large digits
                a = num2;     // I make it calculate two seperate numbers...
                int c, d, e;  // that when displayed next to each other...
                b = a / 10;   // show the correct answer.
                answer = b*num1;
                c = num1 % 10;
                d = num2 % 10;
                e = d * c;
                e = abs(e);
                answerpt2 = e % 10;
              
               
                home();
                print("(");
                print_int(num2);
                print(")x(");
                print_int(num1);
                print(")=       ");
                setCursor(0,1);
                print_int(answer);
                print_int(answerpt2);
                print("      ");
            }
        }
    
    }
    return (1);
}

void __interrupt() adc(void)
    {
        if(PIR1bits.ADIF && PIE1bits.ADIE)
        {
            PIR1bits.ADIF = 0;
    
            unsignednum = (ADRESL >> 8) | ADRESH;
            
            num1 = unsignednum - 511;  
        }
    }
        
