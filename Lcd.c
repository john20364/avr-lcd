// Lcd source file
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL
#include <util/delay.h>

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
byte LcdFirstColumnPositions[] = {0, 64, MAX_COLUMNS, 64 + MAX_COLUMNS};
//==================================================
void _EnablePulse(void);
void _WaitBusyFlagIsZero(void);
void _SendCommand(byte cmd);
void _SendCharacter(char c);
volatile uint8_t *_GetPort(byte dataregister);
volatile uint8_t *_GetDDR(byte dataregister);
//==================================================
byte _LcdDisplayCtrl;
byte _LcdNrOfDataPins;
lcd_t *lcd_instance = NULL;
volatile uint8_t *_LcdDataPort;
volatile uint8_t *_LcdDataDDR;
volatile uint8_t *_LcdCtrlPort;
volatile uint8_t *_LcdCtrlDDR;
byte _LcdRS;
byte _LcdRW;
byte _LcdE;
//==================================================

void LcdPrintString(char *str);
void LcdSetCursor(byte x, byte y);
void LcdReturnHome(void);
void LcdClearDisplay(void);
void LcdDisplayOn(void);
void LcdDisplayOff(void);
void LcdCursorOn(void);
void LcdCursorOff(void);    
void LcdBlinkOn(void);
void LcdBlinkOff(void);

lcd_t *LcdCreate(
    byte datapins, 
    byte dataregister,
    byte ctrlregister,
    byte rs,
    byte rw,
    byte e) {

    if (lcd_instance != NULL) return lcd_instance;
    
    lcd_instance = (lcd_t *)malloc(sizeof(lcd_t));
    
    lcd_instance->PrintString = LcdPrintString;
    lcd_instance->SetCursor = LcdSetCursor;
    lcd_instance->ReturnHome = LcdReturnHome;
    lcd_instance->ClearDisplay = LcdClearDisplay;
    lcd_instance->DisplayOn = LcdDisplayOn;
    lcd_instance->DisplayOff = LcdDisplayOff;
    lcd_instance->CursorOn = LcdCursorOn;
    lcd_instance->CursorOff = LcdCursorOff;
    lcd_instance->BlinkOn = LcdBlinkOn;
    lcd_instance->BlinkOff = LcdBlinkOff;
                              
    _LcdNrOfDataPins = datapins;
    _LcdDataPort = _GetPort(dataregister);
    _LcdDataDDR = _GetDDR(dataregister);
    _LcdCtrlPort = _GetPort(ctrlregister);
    _LcdCtrlDDR = _GetDDR(ctrlregister);
    _LcdRS = rs;
    _LcdRW = rw;
    _LcdE = e;
                                      
    *_LcdCtrlDDR |= 1 << _LcdRS | 1 << _LcdRW | 1 << _LcdE;
    _delay_ms(15);
    
    lcd_instance->ClearDisplay();
    
    switch(_LcdNrOfDataPins) {
        case FOURPINS:
            _SendCommand(0x28); // Function set 4-bit data length (DL), 2 display lines, fonts 5x8 = 00101000 
            break;
        case EIGHTPINS:
        default:
            _SendCommand(0x38); // Function set 8-bit data length (DL), 2 display lines, fonts 5x8 = 00111000 
    }    
    _delay_us(50);
    
    _LcdDisplayCtrl = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF;
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl); // Display ctrl On, Cursor, Blinking = 00001110
    _delay_us(50);

    return lcd_instance;        
}

void LcdPrintString(char *str) {
    while(*str != 0)
        _SendCharacter(*str++);
}

void LcdSetCursor(byte x, byte y) {
    _SendCommand(0x80 + LcdFirstColumnPositions[y-1] + (x-1));
}

void LcdReturnHome(void) {
    _SendCommand(LCD_RETURNHOME);
}

void LcdClearDisplay(void) {
    _SendCommand(LCD_CLEARDISPLAY);
    _delay_ms(2);
}

void LcdDisplayOn(void) {
    _LcdDisplayCtrl |= LCD_DISPLAYON;
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl);
}

void LcdDisplayOff(void) {
    _LcdDisplayCtrl &= ~LCD_DISPLAYON;
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl);
}

void LcdCursorOn(void) {
    _LcdDisplayCtrl |= LCD_CURSORON;
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl);
}

void LcdCursorOff(void) {    
    _LcdDisplayCtrl &= ~LCD_CURSORON;    
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl);
}

void LcdBlinkOn(void) {
    _LcdDisplayCtrl |= LCD_BLINKON;
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl);
}

void LcdBlinkOff(void) {
    _LcdDisplayCtrl &= ~LCD_BLINKON;
    _SendCommand(LCD_DISPLAYCONTROL | _LcdDisplayCtrl);
}

//==================================================
volatile uint8_t *_GetPort(byte dataregister) {
    switch(dataregister) {
        case REGISTER_A: return &PORTA;
        case REGISTER_B: return &PORTB;
        case REGISTER_C: return &PORTC;
        case REGISTER_D: return &PORTD;
    }
    return NULL;
}
volatile uint8_t *_GetDDR(byte dataregister) {
    switch(dataregister) {
        case REGISTER_A: return &DDRA;
        case REGISTER_B: return &DDRB;
        case REGISTER_C: return &DDRC;
        case REGISTER_D: return &DDRD;
    }
    return NULL;
}

void _EnablePulse(void) {
    *_LcdCtrlPort |= 1 << _LcdE;
    asm volatile ("nop");
    asm volatile ("nop");
    *_LcdCtrlPort &= ~(1 << _LcdE);
}

void _WaitBusyFlagIsZero(void) {
    *_LcdDataDDR = 0;
    *_LcdCtrlPort |= 1 << _LcdRW;
    *_LcdCtrlPort &= ~(1 << _LcdRS);

    while (*_LcdDataPort >= 0x80)
    {
        _EnablePulse();
    }
    *_LcdDataDDR = 0xFF; //0xFF means 0b11111111
}

void _SendCommand(byte cmd) {
    _WaitBusyFlagIsZero();
    *_LcdDataPort = cmd;
    *_LcdCtrlPort &= ~(1 << _LcdRW | 1 << _LcdRS);
    _EnablePulse();
    *_LcdDataPort = 0;
}

void _SendCharacter(char c) {
    _WaitBusyFlagIsZero();
    *_LcdDataPort = c;
    *_LcdCtrlPort &= ~(1 << _LcdRW);
    *_LcdCtrlPort |= 1 << _LcdRS;
    _EnablePulse();
    *_LcdDataPort = 0;
}
//==================================================
