/* 
 * File:   FinalExam.c
 * Author: Spencer Will
 *
 * Created on August 7, 2022, 10:55 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "LiquidCrystal.h"
#include "config.h"

#define _XTAL_FREQ 1000000
# define STOP_LED   PORTBbits.RB1
/*
 * 
 */
void __interrupt(high_priority) STOPPED(void);
volatile int speed = 0;
volatile int delay = 0;
volatile float wait = 0;
volatile int UPR8bits = 0;
volatile int LWR2bits = 0;
int delayset;

int main()
{
    // Configure analog pins, voltage reference and digital I/O 
    ADCON1 = 0x0E;
    TRISA = 0xFF;
    TRISD = 0x00;
    TRISE = 0x00;
    TRISBbits.RB1 = 0;
    TRISBbits.RB0 = 1;
    
    //CCP2 PWM Setup with TIMER 2
    PR2 =  249;
    T2CONbits.T2CKPS = 0b00; // Prescaler 1:1
    T2CONbits.TMR2ON = 1;
    TRISCbits.RC1 = 0;
    CCP2CONbits.CCP2M = 0b1100;
    
    
    // Setting A/d AQ Time and conversion clock
    ADCON2bits.ADCS = 0;
    ADCON2bits.ACQT = 1; 
    ADCON2bits.ADFM = 0; // left justified
    
    // Setting A/d channel and turning it on
    ADCON0bits.CHS = 0;
    ADCON0bits.ADON = 1;
    
    //Timer0 Setup
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0x05; // 1:64 prescale value 
    TMR0 = 65497; //for 10ms delay with 1:64 prescale
    // there was no reason to use 1:64 prescale i forgot to change back to 1:1
    
    //Configuring Interrupts
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 0;
    RCONbits.IPEN = 1;  //priority
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.INT0IE = 1; // Enable INT0
    INTCONbits.INT0IF = 0; // reset INT0 flag
    INTCON2bits.INTEDG0 = 0; // falling edge
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.TMR0IP = 0; // low priority 
    
    STOP_LED = 0;
    
    //Setting up LCD display
    pin_setup(&PORTD, &PORTE);
    begin(16, 2, LCD_5x8DOTS);
   
    while(1)
    {
        if (delay > 0)   
        {  
            delayset = wait;
            while (delayset > 0)   // this while loop is the delay
            {
                if (STOP_LED == 0)
                {
                    ADCON0bits.GO = 1;              //reading adc
                    CCPR2L = UPR8bits;              //pwm speed
                    CCP2CONbits.DC2B = LWR2bits;    
                    home();
                    print("Max Speed: ");
                    print_int(speed);
                    print("      ");
                    setCursor(0,1);
                    print("Delay: ");
                    print_int(delay);
                    print(" mS     ");
                    delayset--;
                }
                else  //emergency stop
                {
                    ADCON0bits.GO = 0;    //stops reading adc for emergency stop
                    home();
                    print("    STOPPED!    ");
                    setCursor(0,1);
                    print("                ");
                    CCPR2L = 0;            //sets pwm to 0 for emergency stop
                }
            }
            CCPR2L = 0;
            __delay_ms(200);
        }
        else        // when the delay = 0, it only adjusts pwm
        {   
            if (STOP_LED == 0)      
            {
                ADCON0bits.GO = 1;
                CCPR2L = UPR8bits;
                CCP2CONbits.DC2B = LWR2bits;
                home();
                print("Max Speed: ");
                print_int(speed);
                print("      ");
                setCursor(0,1);
                print("Delay: ");
                print_int(delay);
                print(" mS     ");
            }
            else    //emergency stop
            {
                ADCON0bits.GO = 0;
                home();
                print("    STOPPED!    ");
                setCursor(0,1);
                print("                ");
                CCPR2L = 0;
            }
        }
    }
    return 0;
}

void __interrupt(high_priority) STOPPED(void)   //emergency stop interrupt
{

    if (INTCONbits.INT0IE && INTCONbits.INT0IF)  //using int0
    {
        INTCONbits.INT0IF = 0;
        
        STOP_LED = ~STOP_LED;  //turns on emergency red led
        CCPR2L = 0;             //turns off pwm
        ADCON0bits.GO = 0;
        
    }
}

void __interrupt(low_priority) timer0_interrupt()   //10ms readings of adc
{
    
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;
        
        
        if (PIR1bits.ADIF && PIE1bits.ADIE)
        {
            PIR1bits.ADIF = 0; //reads an0 then an1
       
                if (ADCON0bits.CHS == 0) // channel AN0 
                {
                    UPR8bits = ADRESH;       //collecting adc reading for pwm
                    LWR2bits = ((ADRESL >> 6) & 0x03);
                    speed = ADRESH * 4;
                    ADCON0bits.CHS = 1;
                }
                else if (ADCON0bits.CHS == 1) // channel AN1 
                {
                    delay = ADRESH * 40;    //display delay time
                    ADCON0bits.CHS = 0;
                    wait = (ADRESH / 6);    //while loop delay time
                }               //dividing by 6 yields delay time for while loop
       }
        TMR0 = 65497;  //reseting for 10ms delay
        
    }
}