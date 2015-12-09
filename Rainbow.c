/*
 *
 * This example is configured for a Atmega32 at 16MHz
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"
#include <stdbool.h>
#include "animationen.h"

#define MAXPIX 253

uint8_t counter = 0;

 struct cRGB colors[8];
 struct cRGB kirmes[2];
 struct cRGB led[MAXPIX];
 uint8_t mode; 
 uint8_t pixcount;


void shiftUp() //shift all values one led up
{
        uint8_t i=0;           
        for(i=MAXPIX; i>1; i--) 
            led[i-1]=led[i-2];
}

void fadeUp() //shift all values one led up
{
        uint8_t i=0;           
        for(i=MAXPIX; i>1; i--) 
        {
		//fade red
		if(led[i-1].r<(colors[i-2].r-FADE))
			led[i-1].r+=FADE;
		else if(led[i-1].r>(colors[i-2].r+FADE))
			led[i-1].r-=FADE;
		else
			led[i-1]=led[i-2];
		
		//fade green
		if(led[i-1].g<(colors[i-2].g-FADE))
			led[i-1].g+=FADE;
		else if(led[i-1].g>(colors[i-2].g+FADE))
			led[i-1].g-=FADE;
		else
			led[i-1]=led[i-2];

		//fade blue
		if(led[i-1].b<(colors[i-2].b-FADE))
			led[i-1].b+=FADE;
		else if(led[i-1].b>(colors[i-2].b+FADE))
			led[i-1].b-=FADE;
		else
			led[i-1]=led[i-2];
	}

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


int main(void)
{
	mode = 2;
	pixcount = 0;

	DDRB|=_BV(ws2812_pin);
	PORTA|=_BV(PA1);

	DDRD|=0xFF;
	
	
	//Timer (1ms)
   	//TCCR0 |= (1<<CS02) | (1<<CS00);
	//TCNT0 = 178;
	//sei();
	
	//5ms
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
    	colors[0].r=255; colors[0].g=255; colors[0].b=255;//white
    	colors[1].r=255; colors[1].g=000; colors[1].b=000;//red
    	colors[2].r=255; colors[2].g=100; colors[2].b=000;//orange
    	colors[3].r=100; colors[3].g=255; colors[3].b=000;//yellow
    	colors[4].r=000; colors[4].g=255; colors[4].b=000;//green
    	colors[5].r=000; colors[5].g=100; colors[5].b=255;//light blue (türkis)
    	colors[6].r=000; colors[6].g=000; colors[6].b=255;//blue
    	colors[7].r=100; colors[7].g=000; colors[7].b=255;//violet
    

    	while(1)
    	{
		if(mode==0)
		{
			if(counter>=4)//alle 20ms wird geschoben
				rainbowFade(true);
			else
				rainbowFade(false);
			counter-=4;		
		} 		
		else if(mode==1)
			blackFade();
		else if((mode==2 || mode==3 || mode==4) && counter >= 2)//10ms
		{	
			pixcount++;
			if(pixcount>=104)
			{
				pixcount=0;
				if(mode==2)
					pixelFlow(true, 255, 0, 0);
				else if (mode==3)
					pixelFlow(true, 0, 255, 0);
				else 
					pixelFlow(true, 0, 0, 255);
			}
			else
			{
				pixelFlow(false, 0, 0 , 0);
			}
			counter-=2;
		}
		else if(mode==5)
			white();
		else if(counter==1 && mode==6)//entspricht 50ms
		{
			kirmesFoo();
			counter=0;
		}
		if(!(PINA &(_BV(PA1))))
		{
			mode++;
			_delay_ms(400);
			if(mode>6)
				mode=0;
		}
				
    	}
	
}
ISR (TIMER0_COMP_vect)//jede 5ms
{
	counter++;
}












