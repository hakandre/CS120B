/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab 4 Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/

//DONT TOUCH __________________________________________________________
#include <avr/io.h>													   //
#include <avr/interrupt.h>											   //							   //
#define  tasksSize   3  //ALWAYS UPDATE EVERYTIME YOU ADD OR REMOVE STATES
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
unsigned long tasksPeriod =  100;


typedef struct Task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int(*TickFct)(int);
}Task;
Task tasks[tasksSize];
void TimerISR() {
	unsigned char i;
	for(i = 0; i <tasksSize; ++i){
		if(tasks[i].elapsedTime >= tasks[i].period){
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += tasksPeriod;
	}
}
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;	
	TIMSK1 = 0x02; 
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80; 
}
void TimerOff() {
	TCCR1B = 0x00;
}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if (_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}            
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;                                 //
}	
							                                   //                                     //




//______________________________________________________________________


unsigned char threeLED = 0x00;
enum ThreeLEDSs_States  {TL_Start, LED1, LED2, LED3}TL_state;  //share variable threeLEDs
int TL_SM(int TL_state){ //sequence of LEDs 123
	
	switch(TL_state){//Transition
		case TL_Start:
			TL_state = LED1;
		break;
		
		case LED1:
			TL_state = LED2;
		break;
		
		case LED2:
			TL_state = LED3;
		break;
		
		case LED3:
			TL_state = LED1;
		break;
		
		default:
			TL_state = LED2;
		break;
	
	}
	switch(TL_state){//actions
		case TL_Start:
			threeLED = 0xFF;
		break;
		
		case LED1:
			threeLED = 0x01;
		break;
		
		case LED2:
			threeLED = 0x02;
		break;
		
		case LED3:
			threeLED = 0x04;
		break;
		
		default:
			threeLED = 0xFF;
		break;
		
	}
	return TL_state;
}
//done

unsigned char blinkLED = 0x00;
enum BlinkingLED_States  {BL_Start, ON, OFF}BL_state; //share variable blinkLED
int BL_SM(int BL_state){
	
	switch(BL_state){//Transition
		case BL_Start:
			BL_state = ON;
		break;
		
		case ON:
			BL_state = OFF;
		break;
		
		case OFF:
			BL_state = ON;
		break;
		
		default:
			BL_state = ON;
		break;
		
	}
	switch(BL_state){//actions
		case BL_Start:
		blinkLED = 0x0F;
		break;
		
		case ON:
			blinkLED = 0x08;
		break;
		
		case OFF:
			blinkLED = 0x00;
		break;

	}
	return BL_state;
}

enum CombineLEDs_States  {C_Start, STATE1}C_state;
int Combining_SM(int state){
	
	switch(C_state){//Transition
		case C_Start:
		C_state =STATE1 ;
		break;
		
		case STATE1:
		C_state= C_state;
		break;

		
		default:
		C_state = C_Start;
		break;
		
	}
	switch(C_state){//actions
		case C_Start:
			PORTB = 0x0F;
		break;
		
		case STATE1:
			PORTB = blinkLED | threeLED;
		break;
		default:
			PORTB = 0x0F;
		break;
		
	}
	return C_state;
}

int main()
{	
	unsigned char j = 0;
	tasks[j].state =TL_Start;
	tasks[j].period =300 ;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &TL_SM;
	j++;
	tasks[j].state = BL_Start ;
	tasks[j].period = 1000;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &BL_SM;
	j++;
	tasks[j].state = C_Start ;
	tasks[j].period = 100;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &Combining_SM;


DDRB = 0xFF ; PORTB = 0x00;


	TimerSet(tasksPeriod); //Period
	TimerOn();

	
	while(1) {}
		return 0;
}