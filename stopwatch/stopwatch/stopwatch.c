/*
 * stopwatch.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Mourad
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


unsigned char sec_unit=0;
unsigned char sec_tenth=0;

unsigned char min_unit=0;
unsigned char min_tenth=0;

unsigned char hour_unit=0;
unsigned char hour_tenth=0;

ISR(TIMER1_COMPA_vect){
	sec_unit++;
	if(sec_unit==10){
		sec_unit=0;
		sec_tenth++;
	}
	if(sec_tenth==6){
		sec_tenth=0;
		min_unit++;
	}
	if(min_unit==10){
		min_unit=0;
		min_tenth++;
	}
	if(min_tenth==6){
		min_tenth=0;
		hour_unit++;
	}
	if(hour_unit==10){
		hour_unit=0;
		hour_tenth++;
	}
	if(hour_unit==10&&hour_tenth==9){
		    sec_unit=0;
			sec_tenth=0;
			min_unit=0;
			min_tenth=0;
			hour_unit=0;
			hour_tenth=0;
			TCNT1=24;
	}

}

ISR(INT0_vect){
	sec_unit=0;
	sec_tenth=0;
	min_unit=0;
	min_tenth=0;
	hour_unit=0;
	hour_tenth=0;
	TCNT1=0;
}

ISR(INT1_vect){
	TCCR1B&= ~(1<<CS12)&~(1<<CS11)&~(1<<CS10)&~(1<<WGM12); // pausing the timer, No clock source
}

ISR(INT2_vect){
	TCCR1B=(1<<CS11)|(1<<CS10)|(1<<WGM12); // resuming the stop watch

}
void Timer1_CTC_Init(void){
	TCNT1=0;
	TCCR1A=(1<<FOC0);
	TCCR1B=(1<<CS11)|(1<<CS10)|(1<<WGM12);
	OCR1A=15625;
	TIMSK|=(1<<OCIE1A);

}

void INT0_falling_Init(void){
	DDRD&=~(1<<2);
	PORTD|=(1<<2); //Activate internal pull-up
	MCUCR|=(1<<ISC01);
	GICR|=(1<<INT0);

}


void INT1_rising_Init(void){
	DDRD&=~(1<<3);
	MCUCR|=(1<<ISC11)|(1<<ISC10);
	GICR|=(1<<INT1);
}

void INT2_falling_Init(void){
	DDRB&=~(1<<2);
	PORTB|=(1<<2);
	MCUCSR&=~(1<<ISC2);
	GICR|=(1<<INT2);
}


int main(void){
	SREG|=(1<<7);

	DDRC|=0x0F;
	PORTC&=0xF0;

	DDRA|=0x3F;  // First 6 pins
	PORTA&=0xC0; // The pins are disabled initially

	INT0_falling_Init();

	INT1_rising_Init();

	INT2_falling_Init();

	Timer1_CTC_Init();

	while(1){
		PORTA=(PORTA&0xC0)|(1<<0);
		PORTC=(PORTC&0xF0)|(sec_unit&0x0F);
		_delay_ms(5);
		PORTA=(PORTA&0xC0)|(1<<1);
		PORTC=(PORTC&0xF0)|(sec_tenth&0x0F);
		_delay_ms(5);
		PORTA=(PORTA&0xC0)|(1<<2);
		PORTC=(PORTC&0xF0)|(min_unit&0x0F);
		_delay_ms(5);
		PORTA=(PORTA&0xC0)|(1<<3);
		PORTC=(PORTC&0xF0)|(min_tenth&0x0F);
		_delay_ms(5);
		PORTA=(PORTA&0xC0)|(1<<4);
		PORTC=(PORTC&0xF0)|(hour_unit&0x0F);
		_delay_ms(5);
		PORTA=(PORTA&0xC0)|(1<<5);
		PORTC=(PORTC&0xF0)|(hour_tenth&0x0F);
		_delay_ms(5);

	}

}
