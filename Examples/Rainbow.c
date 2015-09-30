/*
 *
 * This example is configured for a Atmega32 at 16MHz
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

#define MAXPIX 253
#define COLORLENGTH 100
#define FADE 5
#define KIRMESDELAY 50
#define SPFDELAY 10

struct cRGB colors[8];
struct cRGB kirmes[2];
struct cRGB led[MAXPIX];
uint8_t mode = 0;
uint8_t j = 1; //wtf
uint8_t k = 1; //wtf warum mach ich sowas?



void shiftUp() //shift all values one led up
{
        uint8_t i=0;           
        for(i=MAXPIX; i>1; i--) 
            led[i-1]=led[i-2];
}

void shiftDown() //shift all values one led down
{    
        uint8_t i=MAXPIX;           
        for(i=0; i>1; i++) 		
            led[i+1]=led[i+2];
}

void paint()
{
	ws2812_sendarray((uint8_t *)led,MAXPIX*3);
}

void blackFade()
{   
	uint8_t i;
	for(i=0; i<MAXPIX; i++)
{
        if(led[i].r>0)
            led[i].r--;
	if(led[i].g>0)
            led[i].g--;
	if(led[i].b>0)
            led[i].b--;
}
	 _delay_ms(1);										
	paint();
}

void rainbowFade()
{
	shiftUp();
        //change colour when colourlength is reached   
        if(k>COLORLENGTH)
        {
            j++;
            k=0;
        }
        k++;
        //loop colours(keep colorarray in bounds)
        if(j>8)
            j=0;
        
        //fade red
        if(led[0].r<colors[j].r)
            led[0].r+=FADE;
        if(led[0].r>255)
            led[0].r=255;
            
        if(led[0].r>colors[j].r)
            led[0].r-=FADE;
    //    if(led[0].r<0)
    //        led[0].r=0;
        //fade green
        if(led[0].g<colors[j].g)
            led[0].g+=FADE;
        if(led[0].g>255)
            led[0].g=255;
            
        if(led[0].g>colors[j].g)
            led[0].g-=FADE;
    //    if(led[0].g<0)
    //        led[0].g=0;
        //fade blue
        if(led[0].b<colors[j].b)
            led[0].b+=FADE;
        if(led[0].b>255)
            led[0].b=255;
            
        if(led[0].b>colors[j].b)
            led[0].b-=FADE;
    //    if(led[0].b<0)
    //        led[0].b=0;

		 _delay_ms(20);										
		 paint();
}

void kirmesFoo()
{
	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	shiftUp(); paint();
	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	shiftUp(); paint();
	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	shiftUp(); paint();

	_delay_ms(KIRMESDELAY);

	led[0].r=kirmes[1].r; led[0].g=kirmes[1].g; led[0].b=kirmes[1].b;
	shiftUp(); paint();
	led[0].r=kirmes[1].r; led[0].g=kirmes[1].g; led[0].b=kirmes[1].b;
	shiftUp(); paint();

	led[1].r=0; led[1].g=0; led[1].b=0;//insert one black led
	shiftUp(); paint();

	_delay_ms(KIRMESDELAY);
}

void singlePixelFlow()
{
	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	uint8_t i;
	for(i=0; i<=50;i++)
	{
		shiftUp(); paint();
		//_delay_ms(SPFDELAY);
		if(led[0].r>0)
			led[0].r-=5;
		if(led[0].g>0)
			led[0].g-=5;
		if(led[0].b>0)
			led[0].b-=5;
	}
	for(i=51; i<MAXPIX;i++)
	{
		shiftUp(); paint();
	}
}
void white()
{
	uint8_t i;
    	for(i=MAXPIX; i>0; i--)
    	{    
        	led[i-1].r=255;led[i-1].g=255;led[i-1].b=255;
    	}
	paint();
}

int main(void)
{
	DDRB|=_BV(ws2812_pin);
	PORTA|=_BV(PA1);

	DDRD|=0xFF;
	
	
	//Timer (5ms)
	TCCR0 = (1<<CS02) | (1<<CS00) | (1<<WGM01);
	OCR0=156;
	TIMSK|= (1<<OCIE0);
	sei();

    //init all leds black		
    uint8_t i;
    for(i=MAXPIX; i>0; i--)
    {    
        led[i-1].r=0;led[i-1].g=0;led[i-1].b=0;
    }
		
    //Rainbowcolors
    colors[0].r=150; colors[0].g=150; colors[0].b=150;//white(59% brightness)
    colors[1].r=255; colors[1].g=000; colors[1].b=000;//red
    colors[2].r=255; colors[2].g=100; colors[2].b=000;//orange
    colors[3].r=100; colors[3].g=255; colors[3].b=000;//yellow
    colors[4].r=000; colors[4].g=255; colors[4].b=000;//green
    colors[5].r=000; colors[5].g=100; colors[5].b=255;//light blue (türkis)
    colors[6].r=000; colors[6].g=000; colors[6].b=255;//blue
    colors[7].r=100; colors[7].g=000; colors[7].b=255;//violet

    //Kirmesfarben
    kirmes[0].r=255; kirmes[0].g=000; kirmes[0].b=000;//red
    kirmes[1].r=000; kirmes[1].g=000; kirmes[1].b=255;//blue
    

    while(1)
    {
	if(mode==0)
		rainbowFade();
	if(mode==1)
		kirmesFoo();
	if(mode==2)
		singlePixelFlow();
	if(mode==3)
		white();
	if(mode==4)
		blackFade();

		
    }
	
}
ISR (TIMER0_COMP_vect)
{
	//if(!(PIND &(_BV(PD6))))
      	//	PORTD|= _BV(PD6);
	//else
	//	PORTD &= ~(_BV(PD6));
	if(!(PINA &(_BV(PA1))))
	{
		mode++;
		_delay_ms(500);
	}
	if(mode>3)
		mode=0;
	
	
}












