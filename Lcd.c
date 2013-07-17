// Lcd source file
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#include "Util.h"
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

#define MAX_COLUMNS 20

#define EIGHTPINS   1
#define FOURPINS    2

// Variables
byte LcdFirstColumnPositions[] = {0, 64, MAX_COLUMNS, 64 + MAX_COLUMNS};
byte _LcdDisplayCtrl;
byte _LcdDisplayFunction;
byte _LcdDisplayMode;
Lcd *lcd_instance = NULL;
volatile uint8_t *_LcdDataPort;
volatile uint8_t *_LcdDataDDR;
volatile uint8_t *_LcdCtrlPort;
volatile uint8_t *_LcdCtrlDDR;
byte _LcdRS;
byte _LcdRW;
byte _LcdE;
byte _LcdDataPins[8];

// Internal functions
void _EnablePulse(void);
void _WaitBusyFlagIsZero(void);
void _SendCommand(byte cmd);
void _SendCharacter(char c);
void _Write4Bits(byte value);
void _Write8Bits(byte value);

// Public functions
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

// Lcd Factory function
Lcd *LcdCreate(
    byte dataregister,
    byte pin0,
    byte pin1,
    byte pin2,
    byte pin3,
    byte pin4,
    byte pin5,
    byte pin6,
    byte pin7,    
    byte ctrlregister,
    byte pin_rs,
    byte pin_rw,
    byte pin_e) {

    if (lcd_instance != NULL) return lcd_instance;
    
    lcd_instance = (Lcd *)malloc(sizeof(Lcd));
    
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
                              
    _LcdDataPort = GetPort(dataregister);
    _LcdDataDDR = GetDDR(dataregister);
    _LcdCtrlPort = GetPort(ctrlregister);
    _LcdCtrlDDR = GetDDR(ctrlregister);
    _LcdRS = pin_rs;
    _LcdRW = pin_rw;
    _LcdE = pin_e;
    
    _LcdDataPins[0] = pin0;
    _LcdDataPins[1] = pin1;
    _LcdDataPins[2] = pin2;
    _LcdDataPins[3] = pin3;
    _LcdDataPins[4] = pin4;
    _LcdDataPins[5] = pin5;
    _LcdDataPins[6] = pin6;
    _LcdDataPins[7] = pin7;

    if ((pin4 + pin5 + pin6 + pin7) == 0) {
        _LcdDisplayFunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    } else {
        _LcdDisplayFunction = LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS;
    }   
    
    //------------------------------------------------------
    *_LcdCtrlDDR |= 1 << _LcdRS | 1 << _LcdRW | 1 << _LcdE;
    
    // RW and RS low for command mode and Enable Low
    *_LcdCtrlPort &= ~(1 << _LcdRW | 1 << _LcdRS | 1 << _LcdE);

    _delay_ms(50);  // Delay for 50 ms
    
    if (!(_LcdDisplayFunction & LCD_8BITMODE)) {
        // 4-Bit Interface
        // See HD44780U documentation page 46
        _Write4Bits(0x03);
        _delay_us(4500);

        _Write4Bits(0x03);
        _delay_us(100);

        _Write4Bits(0x03);
        
        // Set 4-Bit Interface
        _Write4Bits(0x02);
    } else {
        // 8-Bit Interface
        // See HD44780U documentation page 45
        _Write8Bits(0x30);
        _delay_us(4500);

        _Write8Bits(0x30);
        _delay_us(100);

        _Write8Bits(0x30);
        
        // Set 8-Bit Interface
        _Write8Bits(0x30);
    }
             
    // finally, set # lines, font size, etc.    
    _SendCommand(LCD_FUNCTIONSET | _LcdDisplayFunction);
    
    // turn the display on with no cursor or blinking default
    _LcdDisplayCtrl = LCD_CURSORON | LCD_BLINKOFF;
    LcdDisplayOn();

    // clear it off
    LcdClearDisplay();

    // Initialize to default text direction (for romance languages)
    _LcdDisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    _SendCommand(LCD_ENTRYMODESET | _LcdDisplayMode);
  
    return lcd_instance;        
}

// Lcd Factory helper function for 8-bit data communication
Lcd *Lcd8Create(
    byte dataregister,
    byte pin0,
    byte pin1,
    byte pin2,
    byte pin3,
    byte pin4,
    byte pin5,
    byte pin6,
    byte pin7,    
    byte ctrlregister,
    byte pin_rs,
    byte pin_rw,
    byte pin_e) {
        
    return LcdCreate(dataregister, 
        pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7,
        ctrlregister, pin_rs, pin_rw, pin_e);
}

// Lcd Factory helper function for 4-bit data communication
Lcd *Lcd4Create(
    byte dataregister,
    byte pin0,
    byte pin1,
    byte pin2,
    byte pin3,
    byte ctrlregister,
    byte pin_rs,
    byte pin_rw,
    byte pin_e) {
        
    return LcdCreate(dataregister, 
        pin0, pin1, pin2, pin3, 0, 0, 0, 0,
        ctrlregister, pin_rs, pin_rw, pin_e);
}

//----- PUBLIC-FUNCTIONS -----

void LcdPrintString(char *str) {
    while(*str != 0)
        _SendCharacter(*str++);
}

void LcdSetCursor(byte x, byte y) {
    _SendCommand(0x80 + LcdFirstColumnPositions[y-1] + (x-1));
}

void LcdReturnHome(void) {
    _SendCommand(LCD_RETURNHOME);
    // Takes a long time
    _delay_us(2000);
}

void LcdClearDisplay(void) {
    _SendCommand(LCD_CLEARDISPLAY);
    // Takes a long time
    _delay_us(2000);
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


//----- INTERNAL-FUNCTIONS -----

void _EnablePulse(void) {
    *_LcdCtrlPort |= 1 << _LcdE;
    asm volatile ("nop");
    asm volatile ("nop");
    *_LcdCtrlPort &= ~(1 << _LcdE);
}

void _Write8Bits(byte value) {
    for (int i=0; i<8; i++) {
        *_LcdDataDDR |= 1 << _LcdDataPins[i];
        *_LcdDataPort &= ~(1 << _LcdDataPins[i]);
        *_LcdDataPort |= ((value >> i) & 0x01) << _LcdDataPins[i];
    }
    _EnablePulse();
}

void _Write4Bits(byte value) {
    for (int i=0; i<4; i++) {
        *_LcdDataDDR |= (1 << _LcdDataPins[i]);
        *_LcdDataPort &= ~(1 << _LcdDataPins[i]);
        *_LcdDataPort |= (((value >> i) & 0x01) << _LcdDataPins[i]);
    }
    _EnablePulse();
}

void _WaitBusyFlagIsZero(void) {
    int count;
    if (_LcdDisplayFunction & LCD_8BITMODE) {
        count = 8;
    } else {
        count = 4;
    }
    
    for (int i=0; i<count; i++) {
        *_LcdDataDDR &= ~(1 << _LcdDataPins[i]);
        *_LcdDataPort &= ~(1 << _LcdDataPins[i]);
    }
    
    *_LcdCtrlPort |= 1 << _LcdRW;
    *_LcdCtrlPort &= ~(1 << _LcdRS);
    
    while((*_LcdDataPort) & (1 << _LcdDataPins[count-1])) {
        _EnablePulse();
    }
}

void _SendCommand(byte cmd) {
    _WaitBusyFlagIsZero();

    *_LcdCtrlPort &= ~(1 << _LcdRW | 1 << _LcdRS);
    
    if (_LcdDisplayFunction & LCD_8BITMODE) {
        _Write8Bits(cmd);
    } else {
        _Write4Bits(cmd >> 4);
        _Write4Bits(cmd);
    }
}

void _SendCharacter(char c) {
    _WaitBusyFlagIsZero();

    *_LcdCtrlPort &= ~(1 << _LcdRW);
    *_LcdCtrlPort |= 1 << _LcdRS;

    if (_LcdDisplayFunction & LCD_8BITMODE) {
        _Write8Bits(c);
    } else {
        _Write4Bits(c >> 4);
        _Write4Bits(c);
    }
}
