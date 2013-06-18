// Lcd source file
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "Lcd.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

//==================================================
byte LcdFirstColumnPositions[] PROGMEM = {0, 64, MAX_COLUMNS, 64 + MAX_COLUMNS};
//==================================================
void _EnablePulse(void);
void _WaitBusyFlagIsZero(void);
void _SendCommand(byte cmd);
void _SendCharacter(char c);
//==================================================
byte *_LcdDisplayCtrlPtr;
//==================================================
void LcdInit(void) {
    _LcdDisplayCtrlPtr = (byte)malloc(sizeof(byte));
    
    LCD_CTRL_DDR |= 1 << LCD_RS | 1 << LCD_RW | 1 << LCD_E;
    _delay_ms(15);
    
    _SendCommand(LCD_CLEARDISPLAY);
    _delay_ms(2);
    
    _SendCommand(0x38); // Function set 8-bit data length (DL), 2 display lines, fonts 5x8 = 00111000 
    _delay_us(50);
    
    *_LcdDisplayCtrlPtr = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF;
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr); // Display ctrl On, Cursor, Blinking = 00001110
    _delay_us(50);
}

void LcdPrintString(char *str) {
    while(*str != 0)
        _SendCharacter(*str++);
}

void LcdPrintStringP(char *str) {
    while (pgm_read_byte(str) != 0)
        _SendCharacter(pgm_read_byte(str++));
}

void LcdSetCursor(byte x, byte y) {
    _SendCommand(0x80 + pgm_read_word(&LcdFirstColumnPositions[y-1]) + (x-1));
}

void LcdReturnHome(void) {
    _SendCommand(LCD_RETURNHOME);
}

void LcdClearDisplay(void) {
    _SendCommand(LCD_CLEARDISPLAY);
}

void LcdDisplayOn(void) {
    *_LcdDisplayCtrlPtr |= LCD_DISPLAYON;
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr);
}

void LcdDisplayOff(void) {
    *_LcdDisplayCtrlPtr &= ~LCD_DISPLAYON;
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr);
}

void LcdCursorOn(void) {
    *_LcdDisplayCtrlPtr |= LCD_CURSORON;
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr);
}

void LcdCursorOff(void) {    
    *_LcdDisplayCtrlPtr &= ~LCD_CURSORON;    
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr);
}

void LcdBlinkOn(void) {
    *_LcdDisplayCtrlPtr |= LCD_BLINKON;
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr);
}

void LcdBlinkOff(void) {
    *_LcdDisplayCtrlPtr &= ~LCD_BLINKON;
    _SendCommand(LCD_DISPLAYCONTROL | *_LcdDisplayCtrlPtr);
}

//==================================================
void _EnablePulse(void) {
    LCD_CTRL_PORT |= 1 << LCD_E;
    asm volatile ("nop");
    asm volatile ("nop");
    LCD_CTRL_PORT &= ~(1 << LCD_E);
}

void _WaitBusyFlagIsZero(void) {
    LCD_DATA_DDR = 0;
    LCD_CTRL_PORT |= 1 << LCD_RW;
    LCD_CTRL_PORT &= ~(1 << LCD_RS);

    while (LCD_DATA_PORT >= 0x80)
    {
        _EnablePulse();
    }
    LCD_DATA_DDR = 0xFF; //0xFF means 0b11111111
}

void _SendCommand(byte cmd) {
    _WaitBusyFlagIsZero();
    LCD_DATA_PORT = cmd;
    LCD_CTRL_PORT &= ~(1 << LCD_RW | 1 << LCD_RS);
    _EnablePulse();
    LCD_DATA_PORT = 0;
}

void _SendCharacter(char c) {
    _WaitBusyFlagIsZero();
    LCD_DATA_PORT = c;
    LCD_CTRL_PORT &= ~(1 << LCD_RW);
    LCD_CTRL_PORT |= 1 << LCD_RS;
    _EnablePulse();
    LCD_DATA_PORT = 0;
}
//==================================================
