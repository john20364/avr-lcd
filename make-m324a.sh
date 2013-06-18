#!/bin/sh
rm main.o
rm main.hex

avr-gcc -mmcu=atmega324a -Wall -Os -o main.o main.c Lcd.c
avr-objcopy -j .text -j .data -O ihex main.o main.hex

sudo avrdude -F -V -c usbasp -p m324pa -P usb -U flash:w:main.hex
