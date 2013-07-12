#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <stdlib.h>

#include "Type.h"
#include "Lcd.h"

lcd_t *lcd;

int main(void)
{ 
    //~ lcd = LcdCreate(REGISTER_B, 0, 1, 2, 3, 4, 5, 6, 7, 
        //~ REGISTER_D, 4, 5, 6);
    lcd = Lcd4Create(REGISTER_B, 0,1,2,3, REGISTER_D, 4, 5, 6);    

    lcd->SetCursor(1, 1);
    lcd->PrintString("Wat een opluchting.");
    lcd->SetCursor(1, 2);
    lcd->PrintString("Is het toch gelukt.");
    lcd->SetCursor(1, 3);
    lcd->PrintString("REGISTER B 0123");
    lcd->SetCursor(1, 4);
    lcd->PrintString("4-Bit Interface");

    while(1)
    {
    }
}

