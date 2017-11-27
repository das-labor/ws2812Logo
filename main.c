/*
OLD SCHEMA!!!! discontinued!
                                                  *0
                                          > > > > > > > > > >
                                     > > > ##################> > > >
                                > > >###############> > > > ####### > >
      Part G                 > > ###################^*7    > > > >#####> >
    (134 Pixels)          > > ######################^             > > ####> >
                       > > #####################>#>#^ < <             > > ###> >
                      > ######################># *0v#### <               > > ###> >
                   > >#######################^   <#<> >###^                 > > ## >
                  > ########################^D v<###^ C>*0#^                   > >##> v
                 ^########*13###########*3#^25v#*1##^ 14>v##^*6                   > ##v
                ^#########<<###############^pxv#####*1px v##^                      >v##v
               ^#########<  <###############^  v>###^ < <##^                         v##v
              ^#########v    ^###############^   >#v^< ###^                           v##v
             ^#####*15*14v  ^*12<<############<# *2v#### >                             v##v
             ^######<#<#<<  ^#<<  ^############^#<#<> > ^ *4                           v##v
            ^######v  Part F       ^*11####>#>#>#>#v^ < < < <                           v##v
            ^###*16v  >>#>#>#>#>  ^########^       v########^                           v##v
           ^#######v  ^*9##*10##>>#########^  v<#<#<> > >v##^                            v##v
           ^##*17##v  ^###*8###############^  v#####^ *0 v##^                            v##v
           ^<#<#<#<<  ^#<#<################^  v#####^    v##^         Part A           *0v##v
           ^ (85 Pixels)   ^###############^  v#####^*3  v##^       (111 Pixels)         v##v
           ^>#>#>#>#>#>#v  ^*7#############^  v#####^    v##^                            v##v
           ^##*0########v  ^###############^E v#####^ B  v##^                            v##v
           ^##########*1v  ^###############^52v#####^ 35 v##^                            v##v
           ^#####<<##*2#v  ^*6#<<##########^pxv#####^ px v##^*3                          v##v
            ^###v  <#<#<v  ^#<<  ^#########^  v#####^    v##^                           v##v
            ^*3v                  ^*5######^  v#####^  *1v##^                           v##v
             ^##>  >>#>#>#>#>#>  ^#########^  v#####^    v##^                          v##v
             ^###>>#####*4#####>>##########^  v#####^    v##^                          v##v
              ^############################^  v#####^    v##^                        <v##v
               ^###########################^  v#####^    v##^                       < ##v
                ^##########################^  v#####^ *2 v##^                      < ##v
                 ^#########################^  >#>#>v^< < <##^                     < ##v
                  ^<#######################^       v########^                  < <## <
                    < #####################^#<#<#<#<> > > > ^               < < ##< <
                     < <############################^  *2                < < ### <
                        < <#########################^                 < < ### < <
                           < <######################^*1          < < <#### < <
                              < <###################^       < < < ##### < <
                                 < < ###############^< < < <#######< < <
                                    < < < <################## < < <
                                           < < < < < < < < < <

253 Pixels in total

*/
#ifdef AVR
	#define printf(x, ...) 
	#include <util/delay.h>
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/pgmspace.h>
	#include "light_ws2812.h"
#else
	#include <stdio.h>
	#include <stdint.h>
	struct cRGB { uint8_t g; uint8_t r; uint8_t b; };
	#define PROGMEM 
	#define pgm_read_byte(x) (*(x))
#endif
#include <math.h>

const uint8_t led_splits[] PROGMEM = {
	4, 57, 5, 4, 17, 3, 1, 13, 11, 5, 10, 3, 17, 3, 3, 16, 3, 15,
	5, 4, 2, 8, 5, 7, 1, 6, 2, 2, 3, 8, 1, 2, 6, 1
};

#define LED_AREA_SPLIT 127
#define NUM_LED_MAIN_AREAS 6
enum led_area_enum {
	RIGHT_HALF_INNER,
	RIGHT_HALF_I_DOT,
	RIGHT_HALF_I_STROKE,
	LEFT_HALF_I_DOT,
	LEFT_HALF_I_STROKE,
	CIRCUIT_LEFT,

	I_DOT_INNER,
	I_DOT_OUTER,
	I_STROKE_INNER,
	I_STROKE_OUTER,
} typedef led_area;
const int8_t led_areas[] PROGMEM = {
	1, 2, -13, -12, -11, 6, -10, 0,									LED_AREA_SPLIT,	// rechte Hälfte innen
	8, 9,															LED_AREA_SPLIT,	// rechte Hälfte i-Punkt
	-16, -15, -14, -17,												LED_AREA_SPLIT,	// rechte Hälfte i-Strich
	7,																LED_AREA_SPLIT,	// linke Hälfte i-Punkt
	3, 4, 5,														LED_AREA_SPLIT,	// linke Hälfte i-Strich
	18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,	LED_AREA_SPLIT,	// Schaltung links

	8, 7, 9,														LED_AREA_SPLIT, // i-Punkt innen, (8 == 9 außen nach rechts)
	10,																LED_AREA_SPLIT, // i-Punkt außen
	-16, -15, -14, 3, 4, 5, -17,									LED_AREA_SPLIT, // i-Strich innen, (-15 == -17; -16,-15,-14 == 11,12,13 außen nach rechts)
	11, 12, 13,														LED_AREA_SPLIT, // i-Strich außen
};

const struct cRGB colors[] = {
	{64, 255, 0},	// ORANGE
	{255, 0, 0},	// GREEN
	{255, 0, 255},	// CYAN
	{0, 255, 255},	// MAGENTA
	{0, 0, 255},	// BLUE
	{0, 255, 0},	// RED
	{255, 255, 255},	// WHITE
};

static uint8_t ledGetAreaIndexFromArea(const led_area area) {
	uint8_t a = 0;
	for (uint8_t i = 0; i < sizeof(led_areas); i++) {
		if (a == (uint8_t)area) {
			return i;
		}
		int8_t c = pgm_read_byte(&led_areas[i]);
		if (c == LED_AREA_SPLIT) {
			a++;
		}
	}
	return 0;
}

uint8_t areaCurrent = 0;
uint8_t areaPixelLength = 0;
uint8_t areaSegmentStart = 0;
uint8_t areaSegmentLength = 0;
uint8_t areaSegmentIndex = 0;
int8_t areaSegmentDirection = 0;

uint8_t ledIncrementAreaIndex(uint8_t increment) {
	areaSegmentIndex += increment;
	if (areaSegmentIndex < areaSegmentLength) {
		return 1; // success
	}

	// no success, we need to load the next segment
	int8_t segment = pgm_read_byte(&led_areas[areaCurrent++]);
	printf("segment = %d\n", segment);
	if (segment == LED_AREA_SPLIT) {
		areaSegmentDirection = 0;
		return 0; // already at end
	}
	if (segment < 0) {
		segment = -segment;
		areaSegmentDirection = -1;
	} else {
		areaSegmentDirection = 1;
	}

	areaSegmentStart = 0;
	for (uint8_t i = 0; i <= segment && i < sizeof(led_splits); i++) {
		uint8_t c = pgm_read_byte(&led_splits[i]);
		if (i < segment) {
			areaSegmentStart += c;
		} else {
			areaSegmentLength = c;
			if (areaSegmentDirection < 0) {
				areaSegmentStart += c - 1;
			}
		}
	}
	areaSegmentIndex = 0;

	return 1; // success
}

void ledLoadArea(const led_area area) {
	printf("lade area %d\n", area);
	uint8_t areaIndex = ledGetAreaIndexFromArea(area);
	areaCurrent = areaIndex;
	printf("areaCurrent = %d\n", areaCurrent);

	// calculate areaPixelLength
	areaPixelLength = 0;
	while (ledIncrementAreaIndex(1)) {
		areaPixelLength++;
	}
	areaCurrent = areaIndex;
	printf("areaPixelLength = %d\n", areaPixelLength);

	ledIncrementAreaIndex(0);
}

uint8_t ledGetNextAreaPixel() {
	if (areaSegmentDirection == 0) {
		return -1;
	}
	uint8_t pixel = areaSegmentStart + areaSegmentDirection * areaSegmentIndex;
	ledIncrementAreaIndex(1);
	return pixel;
}






struct cRGB led[253];
uint8_t counter = 0;

void ledAreaFillColor(const led_area area, const struct cRGB color) {
	ledLoadArea(area);
	while (1) {
		uint8_t c = ledGetNextAreaPixel();
		printf("pixel %d\n", c);
		if (c == 255) {
			break;
		}
		led[c] = color;
	}
}

void ledAreaFillPattern(const led_area area, const struct cRGB pattern[], const uint8_t patternLength, const uint8_t pixelLength, const float offset) {
	ledLoadArea(area);
	if (pixelLength == 0) {
		float floatPixels = 0.0;
		uint8_t pixels = 0;
		float pixelsPerPixel = ((float)(areaPixelLength - 1)) / ((float)(patternLength - 1));
		uint8_t i = 0;
		while (areaSegmentDirection) {
			floatPixels += pixelsPerPixel;
			float roundPixels = round(floatPixels + offset);
			if (roundPixels > 0) {
				for (; pixels < (int)roundPixels; pixels++) {
					if (areaSegmentDirection) {
						led[ledGetNextAreaPixel()] = pattern[i];
					}
				}
			}
			i = (i + 1) % areaPixelLength;
		}
	} else {
		for (uint8_t i = 0; i < areaPixelLength; i++) {
			uint8_t pixels = (int)(round(((float)i + (float)offset) / (float)pixelLength)) % patternLength;
			led[ledGetNextAreaPixel()] = pattern[pixels];
		}
	}
}








int main() {
#ifdef AVR
	DDRB|=_BV(ws2812_pin);
	PORTA|=_BV(PA1);

	DDRD|=0xFF;
#endif
		
	//Timer (1ms)
   	//TCCR0 |= (1<<CS02) | (1<<CS00);
	//TCNT0 = 178;
	//sei();
	
	//5ms
/*	TCCR0 = (1<<CS02) | (1<<CS00) | (1<<WGM01);
	OCR0=156;
	TIMSK|= (1<<OCIE0);
	sei();*/

	/*
		_delay_ms(400);
		while(PINA &(_BV(PA1))) {};
		_delay_ms(100);
		while(!(PINA &(_BV(PA1)))) {};
	}*/

	float offset = 0;
	#ifdef AVR
	uint8_t blink = 0;
	while(1) {
		offset += 0.3333;
		if (offset >= 255.0) {
			offset = 0.0;
		}
		ledAreaFillPattern(RIGHT_HALF_INNER, colors, 7, 5, offset);
		ledAreaFillPattern(CIRCUIT_LEFT, colors, 7, 7, offset);
		if(PINA &(_BV(PA1))) { // if button not held, do not blink!
			blink = 2;
		} else if (blink == 2) {
			blink = 0;
		}
		if (blink < 2) {
			blink = 1 - blink;
		}
		if (blink == 1) {
			ledAreaFillColor(I_DOT_INNER, colors[6]);
			ledAreaFillColor(I_DOT_OUTER, colors[6]);
			ledAreaFillColor(I_STROKE_INNER, colors[6]);
			ledAreaFillColor(I_STROKE_OUTER, colors[6]);
		} else {
			ledAreaFillPattern(I_DOT_INNER, colors, 7, 1, offset);
			ledAreaFillPattern(I_DOT_OUTER, colors, 7, 1, offset);
			ledAreaFillPattern(I_STROKE_INNER, colors, 7, 1, offset);
			ledAreaFillPattern(I_STROKE_OUTER, colors, 7, 1, offset);
		}
		ws2812_sendarray((uint8_t *)led, 253*3);
		/*if(!(PINA &(_BV(PA1)))) { // if button held, wait longer
			_delay_ms(1000);
		}*/
	}
	#else
		for (uint8_t i = 0; i < NUM_LED_MAIN_AREAS; i++) {
			ledAreaFillPattern(i, colors, 7, 0, offset);
		}
		for(uint8_t i = 0; i < 253; i++) {
			printf("%02x %02x %02x\n", led[i].r, led[i].g, led[i].b);
		}
	#endif

	return 0;
}

/*ISR (TIMER0_COMP_vect)//jede 5ms
{
	counter++;
}*/