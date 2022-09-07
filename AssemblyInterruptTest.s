config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>
	goto main	
	

psect intCodeHi, class = CODE, reloc = 2
    bcf	    INTCON, 1, a
    call    LED
    retfie  0
	
psect code
PREV_STATUS equ 0x50
main: 

    ; set the I/O port directions
    setf    ADCON1, a   ; AN inputs are now digital inputs
    bcf	    TRISD, 1, a ; RD1 is output (connected to LED)
    bsf	    TRISB, 1, a ; RB0 is input (connected to comparator)
	
    ; initial output value
    clrf    PORTD, a	; Start with LED off
    
    ; Setup interrupts
    movlw   10010000B	
    movwf   INTCON, f, a    ; Enable global interrupt, enable INT0, reset INT0 Flag
    bcf	    INTCON2, 6, a   ; Interrupt on falling edge (default)
    ;bsf	    INTCON2, 6, a   ; Interrupt on rising edge 
    
    setf    PREV_STATUS	; start with comparator with positive output
loop:
    

LED:
    movff   PORTB, PORTD   ;moving input B to output D
    return
    
delay2550us:			    ; 2550 us delay
	movlw	255
l1:	decf	WREG, w, a
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bnz	l1
	return 1

delay500ms:			    
	movlw	100		    
	movwf	0x10, a
l2:	call	delay2550us
	decf	0x10, f, a
	bnz	l2
	return 1
end


