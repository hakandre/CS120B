/*
 * lab9.c
 *
 * Created: 8/20/2018 11:56:25 AM
 * Author : ucrcse
 */ 

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
double note = 0;
enum States  {Start,idle, C_note, D_note, E_note} state;
	void tick(){
		
		switch(state){//Transition
			case Start:
				state = idle;
			break;
			
			case idle: 
			if(((~PINA)&0x01) == 0x01){ //if button 1 is pressed send to C
				state = C_note;	
			}
			else if(((~PINA)&0x02) == 0x02){
				state = D_note;
			}
			else if(((~PINA)&0x04) == 0x04){
				state = E_note;
			}
			else{
				state = idle;
			}
			
			break;
			
			case C_note:
				if(((~PINA)&0x07) == 0x01){ 
					state = C_note;	
				}
				else{
					state = idle;
				}
				
			break;
			
			case E_note:
					if(((~PINA)&0x07) == 0x04){
						state = E_note;
					}
					else{
						state = idle;
					}
			break;
			
			case D_note:
					if(((~PINA)&0x07) == 0x02){
						state = D_note;
					}
					else{
						state = idle;
					}
			break;
			default:
			state = idle;
			break;
			
		}


		switch(state){//Action
			case Start:
				note  = 0;
				set_PWM(note);
			break;
			
			case idle:
				
				note = 0;
				set_PWM(note);
			break;
			
			case C_note:
				
				note = 261.63;
				set_PWM(note);
			break;
			
			case D_note:
				
				note = 293;
				set_PWM(note);
			break;
			 
			case E_note:
				
				note = 329;
				set_PWM(note);
			break;
			default:
			
			break;
			
		}
	}


int main(void)
{
    /* Replace with your application code */
	
	DDRA = 0x00; PORTA  = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	state = Start;

    while (1) 
    {
		
 		tick();
 		set_PWM(note);
    }
	
}

