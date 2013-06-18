// Lcd header file
#ifndef _LCD_H_
#define _LCD_H_

#include "Type.h"

#define MAX_COLUMNS 20

#define EIGHTPINS   1
#define FOURPINS    2

#define LCD_DATA_PORT   PORTB
#define LCD_DATA_DDR    DDRB

#define LCD_CTRL_PORT   PORTD
#define LCD_CTRL_DDR    DDRD

#define LCD_RS  4
#define LCD_RW  5
#define LCD_E   6     

#define LcdPrintStaticString(aString) LcdPrintStringP(PSTR(aString))

void LcdInit(void);
void LcdPrintString(char *str);
void LcdPrintStringP(char *str);
void LcdSetCursor(byte x, byte y);
void LcdReturnHome(void);
void LcdClearDisplay(void);
void LcdDisplayOn(void);
void LcdDisplayOff(void);
void LcdCursorOn(void);
void LcdCursorOff(void);
void LcdBlinkOn(void);
void LcdBlinkOff(void);
//~ //================================================================
//~ typedef struct JBLcd_struct JBLcd;
//~ 
//~ typedef void (* setdatapins_fptr)(JBLcd *, byte d0, byte d1, byte d2, byte d3, 
        //~ byte d4, byte d5, byte d6, byte d7);
        //~ 
//~ typedef void (* setctrlpins_fptr)(JBLcd *, byte rs, byte rw, byte e);
//~ typedef void (* init_fptr)(JBLcd *);
        //~ 
//~ struct JBLcd_struct {
    //~ byte _pin_mode;     // 4 or 8 bit mode
    //~ 
    //~ byte _data_pins[8]; // Data pins
    //~ 
    //~ byte _rs;           // Register Set
    //~ byte _rw;           // Read / Write
    //~ byte _e;            // Enable
    //~ 
    //~ void (* setdatapins)(JBLcd *, byte d0, byte d1, byte d2, byte d3, 
        //~ byte d4, byte d5, byte d6, byte d7);
    //~ 
    //~ void (* setctrlpins)(JBLcd *, byte rs, byte rw, byte e);
    //~ 
    //~ void (* init)(JBLcd *);
    
    //~ void (* printString)(char *str);
    //~ void (* printStringP)(char *str);
    //~ void (* setCursor)(byte x, byte y);
    //~ void (* home)(void);
    //~ void (* clear)(void);
    //~ void (* displayOn)(void);
    //~ void (* displayOff)(void);
    //~ void (* cursorOn)(void);
    //~ void (* cursorOff)(void);
    //~ void (* blinkOn)(void);
    //~ void (* blinkOff)(void);
    //~ // See for more functions LiquidCrystal.cpp
    //~ 
//~ };
    //~ 
//~ JBLcd *JBCreateLcd(void);
//~ 
//~ //================================================================
    
#endif
