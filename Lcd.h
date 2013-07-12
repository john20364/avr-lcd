// Lcd header file
#ifndef _LCD_H_
#define _LCD_H_

#include "Type.h"

#define REGISTER_A  1
#define REGISTER_B  2
#define REGISTER_C  4
#define REGISTER_D  8

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

lcd_t *Lcd8Create(
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
    byte rs,
    byte rw,
    byte e);

lcd_t *Lcd4Create(
    byte dataregister,
    byte pin0,
    byte pin1,
    byte pin2,
    byte pin3,
    byte ctrlregister,
    byte rs,
    byte rw,
    byte e);

#endif
