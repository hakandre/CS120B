#include <avr/io.h>


enum States{start, off, off1, one, one1, two, two1, three, three1, four, four1, five, five1 }state;
void Festivity(){
	switch(state){//transitions
		case start:
		state = off;
		break;
		
		case off:
		if((PINA&0x01)==0x01 ){ //if button press
			state = off1;
		}
		else{ //no button press
			state = state;
		}
		break;

		case off1:
		if((PINA&0x01)==0x00 ){ //if button release
			state = one;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case one:
		if((PINA&0x01)==0x01 ){ //if button press
			state = two;
		}
		else{ //no button press
			state = state;
		}
		break;

		case one1:
		if((PINA&0x01)==0x00 ){ //if button release
			state = two;
		}
		else{ //no button press
			state = state;
		}
		break;

		
		case two:
		if((PINA&0x01)==0x01 ){ //if button press
			state = three;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case two1:
		if((PINA&0x01)==0x00 ){ //if button release
			state = three;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case three:
		if((PINA&0x01)==0x01 ){ //if button press
			state = four;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case three1:
		if((PINA&0x01)==0x00 ){ //if button release
			state = four;
		}
		else{ //no button press
			state = state;
		}
		break;

		case four:
		if((PINA&0x01)==0x01 ){ //if button press
			state = four1;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case four1:
		if((PINA&0x01)==0x00 ){ //if button release
			state = five;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case five:
		if((PINA&0x01)==0x01 ){ //if button press
			state = five1;
		}
		else{ //no button press
			state = state;
		}
		break;
		
		case five1:
		if((PINA&0x01)==0x00 ){ //if button release
			state = off;
		}
		else{ //no button press
			state = state;
		}
		break;
		
	}
	
	switch(state){//actions
		case start:
		PORTC = 0x00;
		break;
		
		case off:
		PORTC = 0x00;
		break;
		
		case off1://on button press
		PORTC = 0x21;
		break;
		
		case one:
		PORTC = 0x33;
		break;
		
		case one1:
		PORTC = 0x0C;
		break;
		
		case two:
		PORTC = 0x00;
		break;
		
		case two1:
		PORTC =0x3F;
		break;
		
		case three:
		PORTC = 0x00;
		break;
		
		case three1:
		PORTC = 0x3F;
		break;
		
		case four:
		PORTC = 0x00;
		break;
		
		case four1:
		PORTC = 0x0C;
		break;
		
		case five:
		PORTC = 0x33;
		break;
		
		case five1:
		PORTC = 0x3F;
		break;
		
	}
	
}


int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;


	while (1)
	{
		Festivity();
		//if button is pressed, start a sequence that repeats itself
		//starts off
		//does 3 states

		
	}
}