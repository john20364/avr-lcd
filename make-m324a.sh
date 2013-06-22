#!/bin/sh
rm main.o
rm main.hex

#avr-gcc -mmcu=atmega324p -Wall -Os -o main.o main.c Lcd.c -std=gnu99 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I.
avr-gcc -mmcu=atmega324p -Wall -Os -o main.o main.c Lcd.c
avr-objcopy -j .text -j .data -O ihex main.o main.hex

sudo avrdude -F -V -c usbasp -p m324p -P usb -U flash:w:main.hex
 
