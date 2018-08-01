//Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. 
//Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). 
//If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
#include <avr/io.h>

enum States {Start,wait,add,buttonWait1, buttonWait2,decrement,reset} state;

void ticktick(){
	switch(state){ //transitions 
		case Start:
			state=wait; //Middle state 
		break;
		
		case add:
			if(){
				
			}
			
		break;
		
		case s1:
			if((PINA&0x01)==0x01) {					    //if button(PA0) is pressed, transition to wait1
				state = wait2;
				
			}
			else {                                      //if button is not pressed, then stay in s1
				state = s1;
			}
			
			
		break;
		
		case wait1:
			if((PINA&0x01)==0x00) {					    //if button(PA0) is pressed, transition to wait1
				state = s1;
				
			}
			else {               //if button is not pressed, then stay in s0
				state = wait1;
			}
			
		break;
		case wait2:
			if((PINA&0x01)==0x00) {					    //if button(PA0) is pressed, transition to wait1
				state = s0;
				
			}
			else {               //if button is not pressed, then stay in s0
				state = wait2;
			}
	
		break;
		default:
			state = state;
		break;
	}
	
	switch(state){ //actions
		case Start:
			PORTB = PORTB&0x0C;
		break;
		
		case s0:
			PORTB = PORTB|0x01;
			PORTB = PORTB&0x01;
		break;
		
		case s1:
			PORTB = PORTB|0x02;
			PORTB = PORTB&0x02;
		break;

		case wait1:
		break;
		
		case wait2:
		break;		
		
		
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

