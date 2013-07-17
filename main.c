#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#include "Type.h"
#include "Util.h"
#include "AD9850.h"
#include "FreqMgr.h"
#include "Lcd.h"

#define TIME_DELAY 150

Lcd *lcd;
FreqMgr *freqmgr;
AD9850 *ad9850;

void freq_callback(long freq);
void step_callback(long step);
void mode_callback(long mode);
static char strbuf[20];

void DisplayFreqInfo(byte mode, long freq);

#define START_FREQ 10000000

int main(void)
{ 
	// lcd 4-bit data communication
    lcd = Lcd4Create(REGISTER_B, 0,1,2,3, REGISTER_D, 4, 5, 6);    
	lcd->CursorOff();

	freqmgr = FreqMgrCreate(
		50000000, START_FREQ, 1000, 
		freq_callback, step_callback, mode_callback);
	freqmgr->SetMode(MODE_FREQ);

	ad9850 = AD9850Create(REGISTER_A, 4, 5, 6, 7);
	ad9850->SetFrequency(START_FREQ);
	
	DisplayFreqInfo(MODE_FREQ, freqmgr->GetFreq());
	DisplayFreqInfo(MODE_STEP, freqmgr->GetStep());

    
    //~ lcd->SetCursor(1, 4);
    //~ lcd->PrintString("Mode: ");

	//~ TCCR1B |= 1 << CS10 | 1 << CS11 | 1 << WGM12;	// 64 prescaler
	//~ TIMSK1 |= 1 << OCIE1A; 
	//~ OCR1A = 15624; // (1000000 / 64) - 1
	
	// PINA0, PINA1, PINA2 input
	DDRA &= ~(1 << PINA0 | 1 << PINA1 | 1 << PINA2);
	// PINA0, PINA1, PINA2 HIGH. 
	PORTA |= 1 << PINA0 | 1 << PINA1 | 1 << PINA2;
	
	// PCMSK0 – Pin Change Mask Register 0
	PCMSK0 |= 1 << PCINT0 | 1 << PCINT1 | 1 << PCINT2;
	
	// PCICR – Pin Change Interrupt Control Register
	// Bit 0 – PCIE0: Pin Change Interrupt Enable 1 (PCINT7:0)
	PCICR |= 1 << PCIE0;
	
	_delay_ms(5); // Wait to settle the registers
	
	sei(); // Set interupt enabled
	
    while(1)
    {
		//~ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		//~ }
    }
}

//~ ISR (TIMER1_COMPA_vect) {
	//~ static unsigned int sec = 0;
	//~ static char str[20];
//~ 
	//~ sprintf(str, "%.8d", ++sec);
	//~ lcd->SetCursor(10,1);	
	//~ lcd->PrintString(str);
//~ }

ISR (PCINT0_vect) 
{
	while (bit_is_clear(PINA, 0)) {
		freqmgr->Up();
		_delay_ms(TIME_DELAY);
	}
	while (bit_is_clear(PINA, 1)) {
		freqmgr->Down();
		_delay_ms(TIME_DELAY);
	}
	if (bit_is_clear(PINA, 2)) {
		// Change mode
		freqmgr->ToggleMode();
		// Extra anti bounce safety
		_delay_ms(TIME_DELAY);
	}
}

void DisplayFreqInfo(byte mode, long freq) {
	switch (mode) {
		case MODE_FREQ: 
			lcd->SetCursor(1, 1);
			sprintf(strbuf, "Freq: %8ld Hz", freq);
		break;
		case MODE_STEP: 
			lcd->SetCursor(1, 2);
			sprintf(strbuf, "Step: %8ld Hz", freq);
		break;
		default: break;
	}
	lcd->PrintString(strbuf);
}

void freq_callback(long freq) {
	ad9850->SetFrequency(freq);
	DisplayFreqInfo(MODE_FREQ, freq);
}

void step_callback(long step) {
	DisplayFreqInfo(MODE_STEP, step);
}

void mode_callback(long mode) {
	switch (mode) {
		case MODE_FREQ:
		lcd->SetCursor(20, 1);
		lcd->PrintString("*");
		lcd->SetCursor(20, 2);
		lcd->PrintString(" ");
		break;
		case MODE_STEP:
		lcd->SetCursor(20, 1);
		lcd->PrintString(" ");
		lcd->SetCursor(20, 2);
		lcd->PrintString("*");
		break;
		default: break;
	}
}
