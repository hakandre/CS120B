//Partner: Richard McGee rmcgee002@ucr.edu
//Buttons are connected to PA0 and PA1. Output for PORTC is initially 7.
//Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0).
//If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.


#include <avr/io.h>


// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


enum States {Start,wait,add,buttonWait1, buttonWait2,decrement,off, offState,offPush} state;
int tmpSpeaker = 0;
double note = 0.0;
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
		if(((~PINA &0x01)==0x01) ){ //checking if only button 0 is on AND button 1 is off
			state = buttonWait1;
		}

		else if((~PINA & 0x01)==0x00){ //check if button 0 is released
			state = wait;
			
		}
		
		break;
		//------------------------------------------------------------------------
		case off:
		if((~PINA&0x04)==0x04){//off button is still pressed
			state = off;
		}
		else{
			state = offState;
		}
		break;
		//---------------------------------------------------------------
		case offState:
		if((~PINA&0x04)==0x04){ //if press during offstate
			state = offPush;
		}
		else{
			state = offState;
		}
		break;
		
		case offPush:
		if((~PINA&0x04)== 0x04){
			state = offPush;
		}
		else{
			state = wait;
		}
		//------------------------------------------------------------------------
		case wait:
		if((~PINA&0x01)==0x01){      //check if button 0 is pressed
			state = add;
		}
		else if((~PINA&0x02)==0x02){ //check if button 1 is pressed
			state = decrement;
		}
		else if((~PINA&0x04)==0x04){
			state = off;
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
		if(((~PINA&0x02)==0x02)){ //checking if only button 0 is on AND button 1 is off
			state = buttonWait2;
		}
		else{ //check if button 1 is released
			state = wait;
			if(tmpSpeaker >=0x01){
				tmpSpeaker -=1;
			}
		}
		
		// 		if(((~PINA &0x02)==0x02) && (~PINA &0x01)!=0x01){ //checking if only button 0 is on AND button 1 is off
		// 			state = buttonWait2;
		// 		}
		// 		else if(((~PINA&0x04)==0x04)){//check if both buttons are pressed
		// 			state = off;
		// 		}
		// 		else if((~PINA & 0x02)==0x00){ //check if button 2 is released
		// 			state = wait;
		//
		// 		}
		
		default:
		state = wait;
		break;
	}
	
	switch(state){ //actions
		//----------------
		case Start:
		note = 440.00;
		break;
		//----------------
		case add:
		if(tmpSpeaker<=6){
			tmpSpeaker +=1;
		}
		break;
		//----------------
		case decrement:
		// 		if(tmpSpeaker >=0x01){
		// 			tmpSpeaker -=1;
		// 		}
		break;
		
		case wait:
		if(tmpSpeaker==0){
			note = 261.63;
		}
		else if(tmpSpeaker == 1){
			note = 293.66;
		}
		else if(tmpSpeaker== 2){
			note = 329.63;
		}
		else if(tmpSpeaker == 3){
			note = 349.23;
		}
		else if(tmpSpeaker== 4){
			note = 392.00;
		}
		else if(tmpSpeaker== 5){
			note = 440.00;
		}
		else if(tmpSpeaker== 6){
			note = 493.88;
		}
		else if(tmpSpeaker ==7){
			note = 523.25;
		}
		else{
			note = note;
		}
		break;
		//----------------
		case off:
		note = 1000;
		break;
		case offState:
		note = 0.00;
		break;
		
		case offPush:
		note = 500;
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
	PWM_on();

	state = Start;
	while (1)
	{
		ticktick();
		set_PWM(note);
	}
}
