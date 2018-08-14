/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab 4 Challenge 1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/

//DONT TOUCH __________________________________________________________
#include <avr/io.h>													   //
#include <avr/interrupt.h>											   //							   //
#define  tasksSize   4  //ALWAYS UPDATE EVERYTIME YOU ADD OR REMOVE STATES
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
unsigned long tasksPeriod =  1;


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

unsigned char max = 0x08;
unsigned char count = 0x01;
unsigned char Sound = 0x00;
enum Speaker_States  {SP_Start, SP_ON, SP_OFF, Inc, Dec}SP_state; //share variable blinkLED
int SP_SM(int SP_state){
	
	switch(SP_state){//Transition
		case SP_Start:
		SP_state = SP_ON;
		break;
		
		case SP_ON:
			if(((~PINA&0x01)!=0x01)&&(count < max)/* If button is not pressed and count is less than max*/){
				SP_state = SP_ON;
			}
			else if(((~PINA&0x01)==0x01)&&(count < max) /*If up-button is pressed and count is less than max*/){
				SP_state = Inc;
			}
			else{//count is more than max
				SP_state = SP_OFF;
				count = 0x00;
			}
		
		break;
		
		case Inc:
			if((~PINA&0x01) == 0x01){
				SP_state = Inc;
			}
			else{
				count = 0x00;
				if(max >0x02){
				max -= 1; //increase frequency at transition so it only does it once
				}
				SP_state = SP_OFF;
			}
			
		break;
		case SP_OFF:
			if(((~PINA&0x02)!=0x02)&&(count<max)/* If button is not pressed and count is less than max*/){
				SP_state = SP_OFF;
			}
			else if(((~PINA&0x02)==0x02)&&(count<max) /*If down-button is pressed and count is less than max*/){
				SP_state = Dec;
			}
			else{//count is more than max
				SP_state = SP_ON;
				count = 0x00;
			}
		
		break;
		
		case Dec:
			if((~PINA&0x02)==0x02){ //if button is still pressed
				SP_state = Dec;
			}
			else{
				count = 0x00;
				if(max <0xFF){
				max +=1;
				}
				SP_state = SP_ON;
			}
		break;
		
		default:
		SP_state = SP_ON;
		break;
		
	}
	switch(SP_state){//actions
		case SP_Start:
		Sound = 0x00;
		break;
		
		case ON:
			if((~PINA&0x04) == 0x04){
				Sound = 0x10;
			}
			else{
				//do not produce sound
			}
			count+=1;
		break;
		case Inc:
		//no actions, only in transition 
		 
		break;
		
		case Dec:
		//no actions
		break;
		
		case OFF:
			count+=1;
			Sound = 0x00;
		break;

	}
	return SP_state;
}

enum CombineLEDs_States  {C_Start, STATE1}C_state;
int Combining_SM(int C_state){
	
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
			PORTB = blinkLED | threeLED |Sound;
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
	tasks[j].period = 1;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &Combining_SM;
	j++;
	tasks[j].state = SP_Start ;
	tasks[j].period = 1;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &SP_SM;
	
DDRA = 0x00; PORTA  = 0xFF;
DDRB = 0xFF ; PORTB = 0x00;


	TimerSet(tasksPeriod); //Period
	TimerOn();

	
	while(1) {}
		return 0;
}