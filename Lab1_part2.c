


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // inputs
	DDRB = 0xFF; PORTB = 0x00; // outputs, initialized to 0s
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	
	
	while(1)
	{

		tmpA = PINA & 0x03;

		if (tmpA == 0x01) { // True door is open and no light is detected(night)
			tmpB = (tmpB & 0xFE) | 0x01; 
		}
		else {
			tmpB = (tmpB & 0xFE) | 0x00; 
		}	
		PORTB = tmpB;
	}
	return 0;
}