#undef F_CPU
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

long a,b;
void my_delay_us(int n) {
	while(n--) {
		_delay_us(1);
	}
}

//Вывод одной цифры
void digit(long number){
	switch (number)
	{
		case 0:PORTB = 0b11111101;break;
		case 1:PORTB = 0b00000101;break;
		case 2:PORTB = 0b01111011;break;
		case 3:PORTB = 0b01101111;break;
		case 4:PORTB = 0b10000111;break;
		case 5:PORTB = 0b11101110;break;
		case 6:PORTB = 0b11111110;break;
		case 7:PORTB = 0b01000101;break;
		case 8:PORTB = 0b11111111;break;
		case 9:PORTB = 0b11101111;break;
	}
}


//Выбор разряда для индикации
void choose_digit (long number){
	switch (number)
	{
		case 0:PORTC |= (1<<PC1)|(1<<PC2)|(1<<PC3);break;
		case 1:PORTC |= (1<<PC0)|(1<<PC2)|(1<<PC3);break;
		case 2:PORTC |= (1<<PC0)|(1<<PC1)|(1<<PC3);break;
		case 3:PORTC |= (1<<PC0)|(1<<PC1)|(1<<PC2);break;
	}
}
//Выводит число на индикатор
void display_number (long number,char count){
	while(number>9999){
		number=number/10;
	}
	
	long x,i,y;
	while(count!=0){
		y=number;
		for(i=0;i<4;i++){
			x=y%10;
			choose_digit(i);
			digit(x);
			_delay_ms(5);
			PORTB=0x00;
			PORTC &=~ (1<<PC0)&(1<<PC1)&(1<<PC2)&(1<<PC3);
			y=y/10;
		}
		count=count-1;
	}
		for(int i=0; i<=50; i++) {
			PORTC |= 1<<4;
			my_delay_us(1000);
			PORTC &= ~1<<4;
			my_delay_us(1000);
			my_delay_us(number*5);
		}
		
	
}

//Инициализация дальномера
void StartHC(void)
{
	PORTC |=(1<<PC5);
	_delay_us(10);
	PORTC &=~(1<<PC5);
}

ISR (INT0_vect)
{
	if((PIND &(1<<PD2)) == 0)
	{
		cli();
		
		a = TCNT1;
		b=(a*8)/58;	
		display_number(b,25);
		sei();
	}
	else
	{
		TCNT1 =0;
	}
}

void InitTimer (void)
{
	TCNT1 = 0;
	TCCR1B |= (1<<CS11)|(1<<CS10);
	TIMSK |= (1<<TOIE1);
}

int main()
{

	DDRC|=(1<<PC0);
	DDRC|=(1<<PC1);
	DDRC|=(1<<PC2);
	DDRC|=(1<<PC3);
	DDRB=0x00;
	DDRC  |= 1<<4;
	PORTC &= ~1<<4;

					
	DDRC|=(1<<PC5);
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC00);
	InitTimer();
	

	sei();

	StartHC();
	

	while(1)
	{
	}
	return 0;
}
