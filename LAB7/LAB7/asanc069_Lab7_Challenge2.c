/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab 7 Challenge 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/

//DONT TOUCH __________________________________________________________
#include <avr/io.h>													   //
#include <avr/interrupt.h>											   //							   //
#define  tasksSize 2  //ALWAYS UPDATE EVERYTIME YOU ADD OR REMOVE STATES
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
unsigned long tasksPeriod = 200;


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

unsigned char cnt = 0x00;
unsigned char val = 0x00;
enum I_States  {I_Start, idle, push1, push2, reset, count1, count2, fastCount1, fastCount2 }I_state;  //share variable threeLEDs
int Input_SM(int I_state){ //sequence of LEDs 123
	switch(I_state){//Transition
		case I_Start:
			I_state = idle;
		break;
		
	case idle:
			if(((~PINA&0x01)==0x01)/* if button is pressed to increment*/){
				I_state= push1;
				cnt =0x00;
			}
			else if(((~PINA&0x02)==0x02)/*if button is pressed to decrement*/){
				I_state= push2;
				cnt = 0x00;
			}
			else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
				I_state= reset;
				cnt = 0x00;
			}
			else{
				/*NOTHING*/
				I_state= idle;
			}
	break;
	
	case push1:
				if(((~PINA&0x01)==0x01)&&(cnt<5)/* if button is pressed and count in less than 1 second*/){
					I_state= push1;
				}
				else if(((~PINA&0x01)!=0x01)&&(cnt<5)/*if button is released and count is less than 1 second*/){
					if(val<0x09){
						val+=1;
					}					
					I_state= idle;

				}
				
				else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
					I_state= reset;
				}				
				else{
					/*WHEN COUNT GOES MORE THAN A SECOND*/
					I_state= count1;
					cnt = 0x00;
				}
	break;
	
	case push2:
				if(((~PINA&0x02)==0x02)&&(cnt<5)/* if button is pressed and count in less than 1 second*/){
					I_state= push2;
				}
				else if(((~PINA&0x02)!=0x02)&&(cnt<5)/*if button is released and count is less than 1 second*/){
					if(val>0x00){
						val-=1;
					}
					I_state= idle;

				}
				
				else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
					I_state= reset;
				}
				else{
					/*WHEN COUNT GOES MORE THAN A SECOND*/
					I_state= count2;
					cnt = 0x00;
					
				}		
	break;
	
	case count1: //ACTION SHOULD ADD 1 EVERY OTHER PERIOD %2 and count 1 period
				if(((~PINA&0x01)==0x01)&&(cnt<10)/*if button is pressed and count in less than 2 seconds*/){
					I_state= count1;
				}
				else if(((~PINA&0x01)!=0x01)&&(cnt<10)/*if button is released and count is less than 2 second*/){
					I_state= idle;
				}
				
				else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
					I_state= reset;
				}
				else{
					/*WHEN COUNT GOES MORE THAN 2 SECONDS*/
					I_state= fastCount1;
					cnt =0x00;
				}
	break;
	
	case count2: 
				if(((~PINA&0x02)==0x02)&&(cnt<10)/* if button is pressed and count in less than 2 seconds*/){
					I_state= count2;
				}
				else if(((~PINA&0x02)!=0x02)&&(cnt<10)/*if button is released and count is less than 2 second*/){
					I_state= idle;

				}
				
				else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
					I_state= reset;
				}
				else{
					/*WHEN COUNT GOES MORE THAN 2 SECONDS*/
					I_state= fastCount2;
					cnt =0x00;
				}
	break;
	
	case fastCount1: 
				if(((~PINA&0x01)==0x01)/* if button is pressed*/){
					I_state= fastCount1;
				}
				else if(((~PINA&0x01)!=0x01)/*if button is released */){
					I_state= idle;

				}
				else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
					I_state= reset;
				}
				else{
					/*DO NOTHING AND CONTINUE COUNTING*/
				}
	break;
	
		case fastCount2: 
				if(((~PINA&0x02)==0x02)/* if button is pressed and count in less than 2 seconds*/){
					I_state= fastCount2;
				}
				else if(((~PINA&0x02)!=0x02)/*if button is released and count is less than 2 second*/){
					I_state= idle;

				}
				
				else if((~PINA&0x03)==0x03/* if both buttons are pressed*/){
					I_state= reset;
				}
				else{
					/*DO NOTHING AND CONTINUE COUNTING*/
				}
	break;
	
	case reset:
		I_state = idle;
	break;
		
	}
	switch(I_state){//actions
		case I_Start:
		cnt = 0x00;
		break;
		
		case idle:
		cnt =0x00;
		break;
		
		case reset:
		val = 0x00; 
		
		case push1:
		cnt+=1;
		break;
		
		case push2:
		cnt+=1;
		break;
		
		case count1:
		cnt+=1;
		if(cnt==0x05){
			if(val !=0x09){
				val+=1;
			}
			//deleted cnt = 0x00
		}
		break;
		
		case count2:
		cnt+=1;
		if(cnt==0x05){
			if(val >0x00){
				val-=1;
			}
			//deleted cnt = 0x00
		}
		break;
		
		case fastCount1:
			cnt+=1;
			if((cnt%2)&&(val<0x09)){
				val+=1;
			}
		break;
		
		case fastCount2:
			cnt+=1;
			if((cnt%2)&&val>0x00){
				val-=1;
			}
	}
	return I_state;
}
//done


enum CombineLEDs_States  {O_Start, STATE1}O_state;
int Output_SM(int O_state){
	
	switch(O_state){//Transition
		case O_Start:
		O_state =STATE1 ;
		break;
		
		case STATE1:
		O_state= O_state;
		break;

		
		default:
		O_state = O_Start;
		break;
		
	}
	switch(O_state){//actions
		case O_Start:
			PORTB = 0x0F;
		break;
		
		case STATE1:
			PORTB = val; //only change this part for challenge 2
		break;
		
	}
	return O_state;
}

int main()
{	
	unsigned char j = 0;
	tasks[j].state =I_Start;
	tasks[j].period =200 ;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &Input_SM;
	j++;
	tasks[j].state = O_Start ;
	tasks[j].period = 200;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &Output_SM;

	
DDRA = 0x00; PORTA  = 0xFF;
DDRB = 0xFF ; PORTB = 0x00;


	TimerSet(tasksPeriod); //Period
	TimerOn();

	
	while(1) {}
		return 0;
}