/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab #2  Challenge 1 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/

//Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4.
// That is to say,  take the upper nibble of PINA and map it to the lower nibble of PORTB, likewise take the lower nibble of PINA and map it to the upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).

#include <avr/io.h>

// Bit-access functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //INPUT
	DDRB = 0xFF; PORTB = 0x00; //OUTPUT
	DDRC = 0xFF; PORTC = 0x00; //OUTPUT
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
	while(1)
	{
		tmpA = PINA; //8 bit value
		tmpB=  0x00; //output on PB3 to PB0  UPPER NIBBLE OF PINA
		tmpC=  0x00; //output on PC7 to PC4 BOTTOM NIBBLE OF PINA
		//-------------------------------------------------------------------------------
		for(unsigned i = 0; i <8; i++){
			if(i<3 &&GetBit(tmpA,i)){                        //load values on bottom part of PORTB(3:0) ###can use iteration variable to load value
				tmpB = SetBit(tmpB,i,GetBit(tmpA, i));
			}
			else if(GetBit(tmpA,i)){
				tmpC = SetBit(tmpC,i,GetBit(tmpA, i));
			}
		}
		PORTB = tmpB;
		PORTC = tmpC;
	}
}


