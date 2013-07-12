#include <avr/io.h>
#define F_CPU 8000000UL
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
    lcd->PrintString("Row One");
    lcd->SetCursor(1, 2);
    lcd->PrintString("Row Two");
    lcd->SetCursor(1, 3);
    lcd->PrintString("Row Three");
    lcd->SetCursor(1, 4);
    lcd->PrintString("Row Four");

    while(1)
    {
    }
}

