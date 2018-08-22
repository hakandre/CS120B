/*	Partner(s) Name & E-mail: Richard McGee
 *	Lab Section: B21
 *	Assignment: Lab 4 Challenge 1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.*/

//DONT TOUCH __________________________________________________________
#include <avr/io.h>													   //
#include <avr/interrupt.h>											   //							   //
#define  tasksSize   1  //ALWAYS UPDATE EVERYTIME YOU ADD OR REMOVE STATES
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
unsigned long tasksPeriod = 100;

void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string);
void delay_ms(int miliSec);

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTD		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTA	// port connected to pins 4 and 6 of LCD disp.
#define RS 0			// pin number of uC connected to pin 4 of LCD disp.
#define E 1			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
	LCD_WriteCommand(0x01);
}

void LCD_init(void) {

	//wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
}

void LCD_WriteCommand (unsigned char Command) {
	CLR_BIT(CONTROL_BUS,RS);
	DATA_BUS = Command;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
	SET_BIT(CONTROL_BUS,RS);
	DATA_BUS = Data;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
	LCD_ClearScreen();
	unsigned char c = column;
	while(*string) {
		LCD_Cursor(c++);
		LCD_WriteData(*string++);
	}
}

void LCD_Cursor(unsigned char column) {
	if ( column < 17 ) { // 16x1 LCD: column < 9
		// 16x2 LCD: column < 17
		LCD_WriteCommand(0x80 + column - 1);
		} else {
		LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
		// 16x2 LCD: column - 9
	}
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
	int i,j;
	for(i=0;i<miliSec;i++)
	for(j=0;j<775;j++)
	{
		asm("nop");
	}
}
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

unsigned char index = 2;
enum States  {START,INIT,Wait, Write}state;  
int WriterSM(int state){ 
	switch(state){//Transition
		case START:
			state = INIT;
			break;
		case INIT:
			state = Wait;
			break;
		case Wait:
			state = Write;
		break;
		
		case Write:
			state = Write;
		break;
		default:
		state = Write;
		break;
	}
	switch(state){//actions
		
		case START:
		
		break;
		
		case INIT:
		index = 2;
		break;
		case Wait:

		break;
		case Write:

			if(index==2){
				LCD_DisplayString(1,"");
				index+=1;
			}
			else if(index==3){
				LCD_DisplayString(16,"C");
				index+=1;
			}
			else if(index==4){
				LCD_DisplayString(15,"CS");
				index+=1;
			}
			else if(index==5){
				LCD_DisplayString(14,"CS1");
				index+=1;
			}
			else if(index==6){
				LCD_DisplayString(13,"CS12");
				index+=1;
			}
			else if(index==7){
				LCD_DisplayString(12,"CS120");
				index+=1;
			}
			else if(index==8){
				LCD_DisplayString(11,"CS120B");
				index+=1;
			}
			else if(index==9){
				LCD_DisplayString(10,"CS120B ");
				index+=1;
			}
			else if(index==10){
				LCD_DisplayString(9,"CS120B i");
				index+=1;
			}
			else if(index==11){
				LCD_DisplayString(8,"CS120B is");
				index+=1;
			}
			else if(index==12){
				LCD_DisplayString(7,"CS120B is ");
				index+=1;
			}
			else if(index==13){
				LCD_DisplayString(6,"CS120B is l");
				index+=1;
			}
			else if(index==14){
				LCD_DisplayString(5,"CS120B is le");
				index+=1;
			}
			else if(index==15){
				LCD_DisplayString(4,"CS120B is leg");
				index+=1;
			}
			else if(index==16){
				LCD_DisplayString(3,"CS120B is lege");
				index+=1;
			}
			else if(index==17){
				LCD_DisplayString(2,"CS120B is legen");
				index+=1;
			}
			else if(index==18){
				LCD_DisplayString(1,"CS120B is legen");
				index+=1;
			}
			else if(index==19){
				LCD_DisplayString(1,"CS120B is legen.");
				index+=1;
			}
			else if(index==20){
				LCD_DisplayString(1,"S120B is legen..");
				index+=1;
			}
			else if(index==21){
				LCD_DisplayString(1,"120B is legen...");
				index+=1;
			}
			else if(index==22){
				LCD_DisplayString(1,"20B is legen...w");
				index+=1;
			}
			else if(index==23){
				LCD_DisplayString(1,"0B is legen...wa");
				index+=1;
			}
			else if(index==24){
				LCD_DisplayString(1,"B is legen...wai");
				index+=1;
			}
			else if(index==25){
				LCD_DisplayString(1," is legen...wait");
				index+=1;
			}
			else if(index==26){
				LCD_DisplayString(1,"is legen...wait ");
				index+=1;
			}
			else if(index==27){
				LCD_DisplayString(1,"s legen...wait f");
				index+=1;
			}
			else if(index==28){
				LCD_DisplayString(1," legen...wait fo");
				index+=1;
			}
			else if(index==29){
				LCD_DisplayString(1,"legen...wait for");
				index+=1;
			}
			else if(index==30){
				LCD_DisplayString(1,"egen...wait for ");
				index+=1;
			}
			else if(index==31){
				LCD_DisplayString(1,"gen...wait for i");
				index+=1;
			}
			else if(index==32){
				LCD_DisplayString(1,"en...wait for it");
				index+=1;
			}
			else if(index==33){
				LCD_DisplayString(1,"n...wait for it.");
				index+=1;
			}
			else if(index==34){
				LCD_DisplayString(1,"...wait for it..");
				index+=1;
			}
			else if(index==35){
				LCD_DisplayString(1,"..wait for it...");
				index+=1;
			}
			else if(index==36){
				LCD_DisplayString(1,".wait for it... ");
				index+=1;
			}
			else if(index==37){
				LCD_DisplayString(1,"wait for it... D");
				index+=1;
			}
			else if(index==38){
				LCD_DisplayString(1,"ait for it... DA");
				index+=1;
			}
			else if(index==39){
				LCD_DisplayString(1,"it for it... DAR");
				index+=1;
			}
			else if(index==40){
				LCD_DisplayString(1,"t for it... DARY");
				index+=1;
			}
			else if(index==41){
				LCD_DisplayString(1," for it... DARY ");
				index+=1;
			}
			else if(index==42){
				LCD_DisplayString(1,"for it... DARY  ");
				index+=1;
			}
			else if(index==43){
				LCD_DisplayString(1,"or it... DARY   ");
				index+=1;
			}
			else if(index==44){
				LCD_DisplayString(1,"r it... DARY    ");
				index+=1;
			}
			else if(index==45){
				LCD_DisplayString(1," it... DARY      ");
				index+=1;
			}
			else if(index==46){
				LCD_DisplayString(1,"it... DARY       ");
				index+=1;
			}
			else if(index==47){
				LCD_DisplayString(1,"t... DARY        ");
				index+=1;
			}
			else if(index==48){  
				LCD_DisplayString(1,"... DARY        ");
				index+=1;
			}
			else if(index==49){
				LCD_DisplayString(1,".. DARY         ");
				index+=1;
			}
			else if(index==50){
				LCD_DisplayString(1,". DARY          ");
				index+=1;
			}
			else if(index==51){
				LCD_DisplayString(1," DARY           ");
				index+=1;
			}
			else if(index==52){
				LCD_DisplayString(1,"DARY            ");
				index+=1;
			}
			else if(index==53){
				LCD_DisplayString(1,"ARY             ");
				index+=1;
			}
			else if(index==54){
				LCD_DisplayString(1,"RY              ");
				index+=1;
			}
			else if(index==55){
				LCD_DisplayString(1,"Y               ");
				index+=1;
			}
			if(index ==56){
				index = 2;
			}	
			

		break; 
		
	}
	return state;
}
//done

int main()
{
	LCD_init();
	unsigned char j = 0;
	tasks[j].state =Wait;
	tasks[j].period =300 ;
	tasks[j].elapsedTime =0;
	tasks[j].TickFct = &WriterSM;

	
DDRD = 0xFF;  PORTD  = 0x00;
DDRA = 0xFF ; PORTA = 0x00;


	TimerSet(tasksPeriod); //Period
	TimerOn();

	
	while(1) {}
		return 0;
}