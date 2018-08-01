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
		//------------------------------------------------------------------------
		case add:
		state=buttonWait1; //only performs the operation  once and leads it to debounce state
		
		break;
		//------------------------------------------------------------------------
		case buttonWait1:
		if(((PINA &0x01)==0x01) && (PINA &0x02)!=0x02){ //checking if only button 0 is on AND button 1 is off
			state = buttonWait1;
		}
		else if(((PINA&0x01)==0x01)&&((PINA &0x02)==0x02)){//check if both buttons are pressed
			state = reset;
		}
		else if((PINA & 0x01)==0x00){ //check if button 0 is released
			state = wait;
			
		}
		
		break;
		//------------------------------------------------------------------------
		case reset:
		if((PINA&0x03)==0x03){//both buttons are pressed
			state = reset;
		}
		else{
			state = wait;
		}
		break;
		//------------------------------------------------------------------------
		case wait:
		if((PINA&0x01)==0x01){      //check if button 0 is pressed
			state = add;
		}
		else if((PINA&0x02)==0x02){ //check if button 1 is pressed
			state = decrement;
		}
		else{
			state = wait;
		}
		break;
		//------------------------------------------------------------------------
		case decrement:
		state =buttonWait2;
		break;
		//-----------------------------------------------------------------------
		case buttonWait2:
		if(((PINA &0x02)==0x02) && (PINA &0x01)!=0x01){ //checking if only button 0 is on AND button 1 is off
			state = buttonWait1;
		}
		else if(((PINA&0x01)==0x01)&&((PINA &0x02)==0x02)){//check if both buttons are pressed
			state = reset;
		}
		else if((PINA & 0x02)==0x00){ //check if button 1 is released
			state = wait;
			
		}
		
		default:
		state = wait;
		break;
	}
	
	switch(state){ //actions
		//----------------
		case Start:
		PORTC = 0x07;
		break;
		//----------------
		case add:
		if(PORTC<=0x08){
			PORTC +=1;
		}
		break;
		//----------------
		case decrement:
		if(PORTC >=0X01){
			PORTC -=1;
		}
		break;
		//----------------
		case reset:
		PORTC = PORTC&0x00;
		break;
		//----------------
		case buttonWait1:
		break;
		//----------------
		case buttonWait2:
		break;
		//----------------
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