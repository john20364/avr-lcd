// Lcd header file
#ifndef _LCD_H_
#define _LCD_H_

#include "Type.h"

#define MAX_COLUMNS 20

#define EIGHTPINS   1
#define FOURPINS    2

#define REGISTER_A  1
#define REGISTER_B  2
#define REGISTER_C  4
#define REGISTER_D  8

//~ #define LCD_DATA_PORT   PORTB
//~ #define LCD_DATA_DDR    DDRB
//~ 
//~ #define LCD_CTRL_PORT   PORTD
//~ #define LCD_CTRL_DDR    DDRD

//~ #define LCD_RS  4
//~ #define LCD_RW  5
//~ #define LCD_E   6     

typedef void (* printstring_fptr)(char *);
typedef void (* setcursor_fptr)(byte, byte);
typedef void (* void_fptr)(void);

typedef struct _lcd_struct_ lcd_t;

struct _lcd_struct_ {
    printstring_fptr PrintString;
    setcursor_fptr SetCursor;
    void_fptr ReturnHome;
    void_fptr ClearDisplay;
    void_fptr DisplayOn;
    void_fptr DisplayOff;
    void_fptr CursorOn;
    void_fptr CursorOff;    
    void_fptr BlinkOn;
    void_fptr BlinkOff;
};

lcd_t *LcdCreate(
    byte datapins, 
    byte dataregister,
    byte ctrlregister,
    byte rs,
    byte rw,
    byte e);
    
#endif
