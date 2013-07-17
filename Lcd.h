// Lcd header file
#ifndef _LCD_H_
#define _LCD_H_

#include "Type.h"

typedef void (* printstring_fptr)(char *);
typedef void (* setcursor_fptr)(byte, byte);
typedef void (* void_fptr)(void);

typedef struct LcdStruct {
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
} Lcd;

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
    byte rs,
    byte rw,
    byte e);

Lcd *Lcd4Create(
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
