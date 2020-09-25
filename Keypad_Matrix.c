/*
 * File:   Keypad_Matrix.c
 * Author: A. Inácio Morais
 *
 * PIC16F886 - MPLAB X IDE v5.40 - XC8-CC v2.30
 * MCLRE OFF - INTRC_NOCLKOUT 8 MHz
 * 
 * See attached Proteus v8.9 SP2 simulation
 *
 * Created on 25 de Setembro de 2020, 11:55
 */

// PIC16F886 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 8000000

#define col1 PORTBbits.RB4
#define col2 PORTBbits.RB5
#define col3 PORTBbits.RB6
//#define col4 PORTBbits.RB7

unsigned char const seven_segment_LUT[8] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07}; // Seven-segment display LookUp Table

char kpd_scan();
void seven_seg_disp(char digit);

void main(void) {
    OSCCONbits.IRCF = 0b111;    // IRCF<2:0> = 111 = 8 MHz
    ANSEL = 0x00;  // 0 = Digital I/O. Pin is assigned to port or special function
    ANSELH = 0x00; // 0 = Digital I/O. Pin is assigned to port or special function
    
    TRISA = 0x00;
    PORTA = 0x00;
    TRISB = 0b11110000;
    PORTB = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;
    
    while(1) {    	
        seven_seg_disp(kpd_scan());
    }
}    

char kpd_scan(){
    char key_scanned = 0, row;
	
    for (row = 0; row < 3; row++) {
        PORTB |= (1 << row);
				
        if (col1) {
            __delay_ms(10);
            if (col1) {
                while(col1);
                PORTB &= ~(1 << row);
                break;
            }
        }
        key_scanned++;
        
        if (col2) {
            __delay_ms(10);
            if (col2) {
                while(col2);
                PORTB &= ~(1 << row);
                break;
            }
        }
        key_scanned++;
        
        if (col3) {
            __delay_ms(10);
            if (col3) {
                while(col3);
                PORTB &= ~(1 << row);
                break;
            }
        }
        key_scanned++;
        
        //if (col4) {
        //    __delay_ms(10);
        //    if (col4) {
        //        while(col4);
        //        PORTB &= ~(1 << row);
        //        break;
        //    }
        //}
        //key_scanned++;
        
        PORTB &= ~(1 << row);
        }
        	
        return key_scanned;	
}

void seven_seg_disp(char digit) {
    if (digit == 8) { // 'R' - Reset Display and PORTA outputs        
        PORTA = 0x00;
        PORTC = 0x00;        
    }
    
    if (digit < 8) { // Set Display with number related to the pressed key and change PORTA output according to the same number
        PORTC = seven_segment_LUT[digit];
        PORTA ^= (1 << digit);        		 
    }	
}
	 
