// A household has a digital combination deadbolt lock system on the doorway.
// The system has buttons on a keypad. Button 'X' connects to PA0, 'Y' to PA1, and '#' to PA2.
// Pressing and releasing '#', then pressing 'Y', should unlock the door by setting PB0 to 1.
// Any other sequence fails to unlock. Pressing a button from inside the house (PA7) locks the door (PB0=0).
// For debugging purposes, give each state a number, and always write the current state to PORTC (consider using the enum state variable).
// Also, be sure to check that only one button is pressed at a time. 
#include <avr/io.h>

enum States {Start,Unlocked, Locked,s0, s1,s2} state;

void ticktick(){
	switch(state){ //transitions 
		case Start:
			state = Unlocked;
		break;
		//------------------------------------------------------------------------
		case Unlocked:
			if((PINA & 0x80) == 0x80){ //IF PINA7 IS 1, LOCK
				state = Locked;
			}
			else{
				state = Unlocked;
			}
		break;
		//------------------------------------------------------------------------
		case Locked:
			
		
		break;
		//------------------------------------------------------------------------
		case s0:
		
		break;
		//------------------------------------------------------------------------
		case s1:
		
		break;
		//------------------------------------------------------------------------
		case s2:
		
		break;
		//-----------------------------------------------------------------------
	}

	
	switch(state){ //actions
		
		case Start:
			PORTB = PORTB|0x01;
		break;
		//------------------------------------------------------------------------
		case Unlocked:
		PORTB = PORTB | 0x01;
		break;
		//------------------------------------------------------------------------
		case Locked:
		PORTB = PORTB & 0x00;
		break;
		//------------------------------------------------------------------------
		case s0:
		
		break;
		//------------------------------------------------------------------------
		case s1:
		
		break;
		//------------------------------------------------------------------------
		case s2:
		
		break;
		//-----------------------------------------------------------------------
		default:
		break;
	}
	
}

int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF; //INPUT
	DDRB = 0xFF; PORTB = 0x00; //OUTPUT
	
    state = Start;
    while (1) 
    {
		ticktick();
    }
}