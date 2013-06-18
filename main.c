#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "Type.h"
#include "Lcd.h"

int main(void)
{    
    LcdInit();
    LcdSetCursor(1,1);
    LcdPrintStaticString("Een");
    LcdSetCursor(2,2);
    LcdPrintStaticString("Twee");
    LcdSetCursor(3,3);
    LcdPrintStaticString("Drie");
    LcdSetCursor(4,4);
    LcdPrintStaticString("Vier");
    
    _delay_ms(2000);
    LcdCursorOff();
    _delay_ms(2000);
    LcdCursorOn();
    _delay_ms(2000);
    LcdDisplayOff();
    _delay_ms(2000);
    LcdDisplayOn();
    _delay_ms(2000);
    LcdBlinkOn();
    _delay_ms(2000);
    LcdBlinkOff();

    while(1)
    {
    }
}

