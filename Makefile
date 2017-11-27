# Makefile to build light_ws2812 library examples
# Change these parameters for your device

F_CPU = 16000000
MMCU = atmega32
PROG=usbasp
PROG_PORT=usb

# Tools:
FORMAT=ihex
OBJCOPY=avr-objcopy
OBJSIZE=avr-size
CC = avr-gcc

TARGET    = main
# Rainbow

DEP	  = Light_WS2812/light_ws2812
# animationen

CFLAGS = -g2 -I. -ILight_WS2812 -mmcu=$(MMCU) -DF_CPU=$(F_CPU) 
CFLAGS+= -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  
CFLAGS+= -Wall -Wno-pointer-to-int-cast
#CFLAGS+= -Wa,-ahls=$<.lst

LDFLAGS = -Wl,--relax,--section-start=.text=0,-Map=main.map -lm

all:	$(TARGET).hex 

flash: $(TARGET).hex
	avrdude -U flash:w:$(TARGET).hex:i -v -p $(MMCU) -b 57600 -c $(PROG) -P $(PROG_PORT)

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.elf: %.o $(DEP:=.o)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ --output $@
	$(OBJSIZE) --format=avr --mcu=$(MMCU) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
PHONY:	clean

clean:
	rm -f $(TARGET).hex $(TARGET).o $(DEP:=.o) $(TARGET).lss $(TARGET).elf
