/*
 * CS120BLab3.c
 *
 * Created: 8/1/2018 12:16:11 PM
 * Author : ucrcse
 */ 

// PB0 and PB1 each connect to an LED, and PB0's LED is initially on.
// Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after button release. 
// Pressing the button again turns off PB1's LED and turns on PB0's LED. 
#include <avr/io.h>

enum States {Start,s0, s1,wait1, wait2} state;

void ticktick(){
	switch(state){ //transitions 
		case Start:
			state=s0; //PB0 LED is initially on 
		break;
		
		case s0:
			if((PINA&0x01)==0x01) {					    //if button(PA0) is pressed, transition to wait1
				state = wait1;
				
			}
			else {                                      //if button is not pressed, then stay in s0
				state = s0;	
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

