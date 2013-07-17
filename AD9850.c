// 9850 datasheet at http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#include "Type.h"
#include "Util.h"
#include "AD9850.h"

byte _w_clk;
byte _fq_ud;
byte _data;
byte _reset;
volatile uint8_t *_DDR;
volatile uint8_t *_PORT;

AD9850 *ad9850_instance = NULL;

void _SetFrequency(long frequency);
void _PulsHigh(byte pin);

AD9850 *AD9850Create(
    byte ctrlregister,
    byte w_clk,
    byte fq_ud,
    byte data, 
    byte reset) {
        
    if (ad9850_instance != NULL) return ad9850_instance;
    ad9850_instance = (AD9850 *)malloc(sizeof(AD9850));        

    _DDR = GetDDR(ctrlregister);    
    _PORT = GetPort(ctrlregister);
    
    _w_clk = w_clk;
    _fq_ud = fq_ud;
    _data = data;
    _reset = reset;
    
    // set DDR to output 
    *_DDR |= 1 << _w_clk | 1 << _fq_ud | 1 << _data | 1 << _reset;
    
    _PulsHigh(_reset);
    _PulsHigh(_w_clk);
    _PulsHigh(_fq_ud); // this pulse enables serial mode - Datasheet page 12 figure 10
    
    ad9850_instance->SetFrequency = _SetFrequency;
    
    return ad9850_instance;
}

void _PulsHigh(byte pin) {
    *_PORT |= 1 << pin;
    *_PORT &= ~(1 << pin);
}

// transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
void _TransferByte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    (data & 0x01) ? (*_PORT |= 1 << _data) : (*_PORT &= ~(1 << _data));
    _PulsHigh(_w_clk);   //after each bit sent, CLK is pulsed high
  }
}

// frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void _SetFrequency(long frequency) {
//  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850
  int32_t freq = frequency * 4295016000/125000000;  // adjusted to the frequency measurement on the Rigol DS1102E
  for (int b=0; b<4; b++, freq>>=8) {
    _TransferByte(freq & 0xFF);
  }
  _TransferByte(0x00);   // Final control byte, all 0 for 9850 chip
  _PulsHigh(_fq_ud);  // Done!  Should see output
}
