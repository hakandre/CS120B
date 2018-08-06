/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab 4 Exercise 1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/


//A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full).
// A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light. 
//Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0.
// Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less.  (The example below shows the display for a fuel level of 3).   


 // Buttons are connected to PA0 and PA1. Output for PORTB is initially 0. Pressing PA0 increments PORTB (stopping at 9).
 // Pressing PA1 decrements PORTB (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTB resets to 0. 
 // If a reset occurs, both buttons should be fully released before additional increments or decrements are allowed to happen. Use LEDs (and resistors) on PORTB. Use a state machine (not synchronous) captured in C. 
#include <avr/io.h>


int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00; 
	unsigned char tmpC = 0x00; 
	unsigned char tmp = 0x00;


	while (1)
	{
		tmp = ~PINA & 0x0F; //checking 0-3
		if (tmp == 0x00){
			tmpC = 0x00; //should be 0x00
		}
		else if (tmp < 0x03){
			tmpC = 0x20;
		}
		else if (tmp < 0x05){
			tmpC = 0x30;
		}
		else if (tmp < 0x07){
			tmpC = 0x38;
		}
		else if (tmp < 0x0A){
			tmpC = 0x3C;
		}
		else if (tmp < 0x0D){
			tmpC = 0x3E;
		}
		else {
			tmpC = 0x3F;
		}

		//for the low light button
		if (tmp < 0x04){
			tmpC = (tmpC & 0x3F) | 0x40;
		}

//Exercise 2 STARTS Here
		// Loading values of PINA for fastening Seatbelt PA7
//		tmpA = PINA & 0x70;
	//	if (tmpA == 0x30){ //if conditions are true
		//	tmpC = (tmpC & 0x7F) | 0x80; 
		//}
		

		PORTC = tmpC; //should equal to tempC
	}
}