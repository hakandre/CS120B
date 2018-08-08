#include <avr/io.h>
#include "io.c"



enum States {Start,wait,add,buttonWait1, buttonWait2,decrement,reset} state;
unsigned char tmpA;
void ticktick(){
	tmpA = tmpA;
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
		if(((~PINA &0x01)==0x01) && (~PINA &0x02)!=0x02){ //checking if only button 0 is on AND button 1 is off
			state = buttonWait1;
		}
		else if(((~PINA&0x01)==0x01)&&((~PINA &0x02)==0x02)){//check if both buttons are pressed
			state = reset;
		}
		else if((~PINA & 0x01)==0x00){ //check if button 0 is released
			state = wait;
			
		}
		
		break;
		//------------------------------------------------------------------------
		case reset:
		if((~PINA&0x03)==0x03){//both buttons are pressed
			state = reset;
		}
		else{
			state = wait;
		}
		break;
		//------------------------------------------------------------------------
		case wait:
		if((~PINA&0x01)==0x01){      //check if button 0 is pressed
			state = add;
		}
		else if((~PINA&0x02)==0x02){ //check if button 1 is pressed
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
		if(((~PINA &0x02)==0x02) && ((~PINA &0x01)!=0x01)){ //checking if only button 0 is on AND button 1 is off
			state = buttonWait2;
		}
		else if(((~PINA&0x01)==0x01)&&((~PINA &0x02)==0x02)){//check if both buttons are pressed
			state = reset;
		}
		else if((~PINA & 0x02)==0x00){ //check if button 1 is released
			state = wait;
			
		}
		break;
		
		default:
		state = wait;
		break;
	}
	
	switch(state){ //actions
		//----------------
		case Start:
		tmpA = 0x07;
		break;
		
		case wait:
			if(tmpA == 0x00){
				LCD_DisplayString(1,"0" );
				
			}
			else if(tmpA ==0x01){
				LCD_DisplayString(1, "1" );
			
			}
			else if(tmpA ==0x02){
				LCD_DisplayString(1, "2");
			}
			else if(tmpA ==0x03){
				LCD_DisplayString(1,"3");
			}
			else if(tmpA ==0x04){
				LCD_DisplayString(1,"4" );
			}
			else if(tmpA ==0x05){
				LCD_DisplayString(1, "5" );
			}
			else if(tmpA ==0x06){
				LCD_DisplayString(1,"6" );
			}
			else if(tmpA ==0x07){
				LCD_DisplayString(1,"7" );
			}
			else if(tmpA ==0x08){
				LCD_DisplayString(1,"8");
			}
			else if(tmpA ==0x09){
				LCD_DisplayString(1,"9");
			}
			else{
				LCD_DisplayString( 1, "Shrek is love");
			}
		break;
		//----------------
		case add:
		if(tmpA<=0x08){
			tmpA +=1;
		}
		break;
		//----------------
		case decrement:
		if(tmpA >=0X01){
			tmpA -=1;
		}
		break;
		//----------------
		case reset:
		tmpA = tmpA&0x00;
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
	DDRA = 0X00; PORTA =0XFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
 tmpA = 0x00;
	// Initializes the LCD display
	LCD_init();
	
	
	// Starting at position 1 on the LCD screen, writes Hello World
	
	while(1) {
		ticktick();
	}
	
}
