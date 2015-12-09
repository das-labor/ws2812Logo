#include "animationen.h"
#include <avr/io.h>
#include <util/delay.h>

#define COLORLENGTH 100
#define FADE 5

//für rainbowFade() benötigt
 uint8_t j=1; //remove before congress
 uint8_t k=1; //remove before congress


	



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
	paint();
}

void kirmesFoo()
{	
	//Kirmesfarben TODO
	struct cRGB kirmes[2];	
	kirmes[0].r=255; kirmes[0].g=000; kirmes[0].b=000;//red
	kirmes[1].r=000; kirmes[1].g=000; kirmes[1].b=255;//blue

	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	shiftUp(); paint();
	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	shiftUp(); paint();
	led[0].r=kirmes[0].r; led[0].g=kirmes[0].g; led[0].b=kirmes[0].b;
	shiftUp(); paint();

	_delay_ms(50);//remove before congress TODO

	led[0].r=kirmes[1].r; led[0].g=kirmes[1].g; led[0].b=kirmes[1].b;
	shiftUp(); paint();
	led[0].r=kirmes[1].r; led[0].g=kirmes[1].g; led[0].b=kirmes[1].b;
	shiftUp(); paint();

	led[1].r=0; led[1].g=0; led[1].b=0;//insert one black led
	shiftUp(); paint();
}

void pixelFlow(bool newPixel, uint8_t red, uint8_t green, uint8_t blue)
{
	if(newPixel == true)
	{
		led[0].r=red; led[0].g=green; led[0].b=blue;
	}
	if(led[0].r>0)
		led[0].r-=5;
	if(led[0].g>0)
		led[0].g-=5;
	if(led[0].b>0)
		led[0].b-=5;
	led[104].r=000; led[104].g=000; led[104].b=000;
	//led65 ist die letzte led unter dem i
	//led 189 ist die erste led vom Platinenteil
	led[189]=led[65];
	led[152]=led[65];
	led[229]=led[193];
	led[210]=led[198];

	//rechter teil der i-punktes(innerer Streifen rechts)
	led[114]=led[92];
	led[115]=led[94];
	led[116]=led[96];
	led[117]=led[98];
	led[118]=led[100];
	led[119]=led[102];
	led[120]=led[104];
	//rechter teil der i(äußerer Streifen rechts)
	uint8_t i;
	for(i=0; i<23; i++)
	{
		led[151-i]=led[152+i];
	}
	//mittlerer Teil des i(Streifen nach rechts leuchtend)
	for(i=0; i<16; i++)
	{
		led[188-i]=led[156+i];
	}
	//rechter teil des i-Punktes, Streifen nach links leuchtend
	uint8_t K=3;
	led[104]=led[102-K];
	led[105]=led[101-K];
	led[106]=led[100-K];
	led[107]=led[99-K];
	led[108]=led[98-K];
	led[109]=led[97-K];
	led[110]=led[96-K];
	led[111]=led[95-K];
	led[112]=led[94-K];

	//rechter teil des i-Punktes, Streifen nach rechts leuchtend

	//TODO
	
	//letztes pixel der rechten Hälfte: 188
	led[208].r=000; led[208].g=000; led[208].b=000;
	led[220].r=000; led[104].g=000; led[104].b=000;
	led[175].r=000; led[104].g=000; led[104].b=000;
	//led[119].b=255;
	//led[102].g=255;
	shiftUp(); paint();
	
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

void rainbowFade(bool shift)
{
	fadeUp();

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
    	if(led[0].r<FADE)
            led[0].r=0;

        //fade green
        if(led[0].g<colors[j].g)
            led[0].g+=FADE;
        if(led[0].g>255)
            led[0].g=255;
            
        if(led[0].g>colors[j].g)
            led[0].g-=FADE;
        if(led[0].g<FADE)
            led[0].g=0;

        //fade blue
        if(led[0].b<colors[j].b)
            led[0].b+=FADE;
        if(led[0].b>255)
            led[0].b=255;
            
        if(led[0].b>colors[j].b)
            led[0].b-=FADE;
        if(led[0].b<FADE)
            led[0].b=0;
							
	paint();//TODO fade funktion
}





















