/*
 * GccApplication1.c
 *
 * Created: 8/16/2018 12:31:31 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}





int main(void)
{
		DDRA = 0xFF; PORTA = 0x00; //output
		DDRB = 0xFF; PORTB = 0x00;
		DDRD = 0xFF; PORTD = 0x00;
		unsigned short x;
		ADC_init();
		unsigned char y = 0x00;
		unsigned char z = 0x00;
		unsigned char D = 0x00; 
    /* Replace with your application code */
    while (1) 
    {
		x = ADC; //10 bits 
		y = (char)x; // last 4 bits of x
		z = (char)(x>>4); // following 4 bits of x
		D = (char)(x>>6); //first 2 digits of x
		PORTB = y|(z<<4)|(D<<6);
		
		
	}
}

