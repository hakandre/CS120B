/*
 * CS120BLab1.c
 *
 * Created: 7/31/2018 11:45:03 AM
 * Author : ucrcse
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as input
	DDRC = 0xFF; PORTC = 0x00; //OUTPUT NUMBER OF CAR SPACES IN BINARY
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpC = 0x00; 
	
while(1)
	{
		tmpA = PINA;
		tmpC = 0x00;
		
		for(unsigned i=0; i<8;i++){
			if((tmpA & 0x01)== 0x01){
				tmpC++;
			}
			tmpA=tmpA>>1;
		}
		

		if(tmpC == 0x08){
			tmpC = tmpC |0x80;
		}
		// 3) Write output
		PORTC= tmpC;
	}
	return 0;
}


