/*
 * Exercise1.c
 *
 * Created: 8/6/2018 4:28:02 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M =1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	
	_avr_timer_cntcurr= _avr_timer_M;
	
	SREG |= 0x80;
}


void TImerOFF(){
	TCCR1B = 0x00;
	
}

void TimerISR(){
	TimerFlag =1;
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
	_avr_timer_cntcurr = _avr_timer_M;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
   
    while (1) 
    {
		tmpB = ~tmpB;	
		PORTB = tmpB;
		while (!TimerFlag);
		TimerFlag = 0;

    }
}

