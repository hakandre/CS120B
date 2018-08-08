/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab 4 Exercise 1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned char score =0;
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}
void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}
void TimerISR() {
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States  {Start, LED1, LED2, LED3,push, wait, scoring,messup, victory} state;
	#define button (~PINA &0x01)
void tick(){
	
	 
		switch(state){
		case Start:
		state = LED1;
		break;
		case LED1:

			if(button != 0x01){
				state = LED2;
			}
			else{
				state = messup;
			}
		break;
		
		case LED2:
			if(button != 0x01){
				state = LED3;
			}
			else{
				state = scoring;
				
			}
		break;
		case messup:
			state = push;
		break;
		case scoring:
		if(score <=0x09){
			state = push;
		}
		else{
			state = victory;
		}
		break;
		
		case victory:
			state = state;	
		break;
		
		case LED3:
			if(button != 0x01){
				state = LED1;
			}
			else{
				state = messup;
			}
		break;
		
		case push:
			if(button == 0x01){
				state = state;
			}
			else{
				state = wait;
			}
		break;
		
		case wait:
			if(button == 0x01){
				state = LED1;
				
			}
			else{
				state = state;
			}
		
		default:
		
		break;
	
	}


	switch(state){//Action
		case Start:
			PORTB=0x00;
					LCD_ClearScreen();
					LCD_WriteData(score +'0');
		break;
		
		case LED1:
				LCD_ClearScreen();
				LCD_WriteData(score +'0');
			PORTB=0x01;
		break;
		case messup:
				LCD_ClearScreen();
				LCD_WriteData(score +'0');
		if(score>0x00){
			score = score-1;
		}
		else{
			//do nothin
		}
		break;
		case scoring:
				LCD_ClearScreen();
				LCD_WriteData(score +'0');
		PORTB = 0X02;
			score = score+1;
		break;
		case victory:
				LCD_ClearScreen();
			
			LCD_DisplayString(1,"CONGRATULATIONS!");
		break;
		
		case LED2:
				LCD_ClearScreen();
				LCD_WriteData(score +'0');
			PORTB=0X02;
		break;
		
		case LED3:
				LCD_ClearScreen();
				LCD_WriteData(score +'0');
			PORTB =0X04;
		break;
		
		default:
			PORTB= 0X03;
		break;
		
	}
}

int main()
{
	
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	

	LCD_init();
	
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	DDRA = 0X00; PORTA = 0XFF;
	TimerSet(300);
	TimerOn();
	
	
	
	
	while(1) {
		
		tick();
		while(!TimerFlag);
		TimerFlag=0;
	}
}