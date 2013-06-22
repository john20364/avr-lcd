#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <stdlib.h>

#include "Type.h"
#include "Lcd.h"

lcd_t *lcd;
int main(void)
{ 
    lcd = LcdCreate(EIGHTPINS, REGISTER_B, REGISTER_D, 4, 5, 6);
    lcd->SetCursor(1, 1);
    lcd->PrintString("1 One");
    lcd->SetCursor(1, 2);
    lcd->PrintString("2 Two");
    lcd->SetCursor(1, 3);
    lcd->PrintString("3 Three");
    lcd->SetCursor(1, 4);
    lcd->PrintString("4 Four");
    
    while(1)
    {
    }
}

