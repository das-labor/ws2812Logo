#ifndef ANIMATIONEN_H
#define ANIMATIONEN_H
#define MAXPIX 253
#define COLORLENGTH 100
#define FADE 5

#include <inttypes.h>
#include <stdbool.h>
#include "light_ws2812.h"

extern struct cRGB colors[8];
extern struct cRGB led[MAXPIX];
extern uint8_t mode; 
extern uint8_t j; //remove before congress
extern uint8_t k; //remove before congress
extern uint8_t pixcount;


void blackFade(void);
void kirmesFoo(void);
void rainbowFade(void);
void white(void);
void pixelFlow(bool newPixel, uint8_t red, uint8_t green, uint8_t blue);
#endif
