
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
	 
	DDRB = 0xFE; PORTB = 0x01; //OUTPUT:(weight display: PB0) (Airbag_Enable: PB1) (Airbag_sign: PB2)
	DDRD = 0x00; PORTD = 0xFF; //INPUT & OUTPUT:(weight display: PB0:PB7)
	

	unsigned char tmpB_output = 0x00;
	unsigned char tmpD = 0x00;
	unsigned char tmpB_input = 0x00;
	while(1){
		
		tmpD = PIND & 0xFF;
		tmpB_input = PINB & 0x01;
		tmpB_output= PINB & 0x00;
		
		// A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511) and connects to input PD7..PD0PB0 on the microcontroller.
		
		
		// If the weight is equal to or above 70 pounds, the airbag should be enabled by setting PB1 to 1.
		if((tmpD >= 0x23) || ((tmpD >=0x22) && (GetBit(tmpB_input,0)))  ) {
			tmpB_output = tmpB_output |0x02;
		}
		// If the weight is above 5 but below 70, the airbag should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1.
		else if(  /*Checking if it's above 5*/  ((   (tmpD>0x02)&&(GetBit(tmpB_input,0))   )   || (tmpD>=0x06)) &&/* Checking if below 70*/ ((tmpD<0x23) && ((tmpD< 0x22) ||GetBit(tmpB_input,0) ))   ) {
			tmpB_output = tmpB_output & 0xFD; //disable airbag
			tmpB_output = tmpB_output | 0x04; //enable light
		}

	
	PORTB = tmpB_output;
	}
}
