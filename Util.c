#include <avr/io.h>
#include <stdlib.h>

#include "Type.h"
#include "Util.h"

volatile uint8_t *GetPort(byte dataregister) {
    switch(dataregister) {
        case REGISTER_A: return &PORTA;
        case REGISTER_B: return &PORTB;
        case REGISTER_C: return &PORTC;
        case REGISTER_D: return &PORTD;
    }
    return NULL;
}
volatile uint8_t *GetDDR(byte dataregister) {
    switch(dataregister) {
        case REGISTER_A: return &DDRA;
        case REGISTER_B: return &DDRB;
        case REGISTER_C: return &DDRC;
        case REGISTER_D: return &DDRD;
    }
    return NULL;
}
