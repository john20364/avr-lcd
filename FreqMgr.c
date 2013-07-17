#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#include "FreqMgr.h"

FreqMgr *freqmgr_instance = NULL;
long _minfreq;
long _maxfreq;
long _freq;
long _step;
byte _mode;
callback_fptr _freqcallback;
callback_fptr _stepcallback;
callback_fptr _modecallback;

// Internal functions
void _SetMode(byte mode);
byte _GetMode(void);
void _ToggleMode(void);
void _Up(void);
void _Down(void);
long _GetFreq(void);
long _GetStep(void);

FreqMgr *FreqMgrCreate(
    long maxfreq, 
    long freq,
    long step,
    callback_fptr freqcallback,
    callback_fptr stepcallback,
    callback_fptr modecallback) {
        
    if (freqmgr_instance != NULL) return freqmgr_instance;
    freqmgr_instance = (FreqMgr *)malloc(sizeof(FreqMgr));        
    
    freqmgr_instance->SetMode = _SetMode;
    freqmgr_instance->GetMode = _GetMode;
    freqmgr_instance->ToggleMode = _ToggleMode;
    freqmgr_instance->Up = _Up;
    freqmgr_instance->Down = _Down;
    freqmgr_instance->GetFreq = _GetFreq;
    freqmgr_instance->GetStep = _GetStep;
    
    _minfreq = 0;
    _maxfreq = maxfreq;
    _freq = freq;
    _step = step;
    _freqcallback = freqcallback;
    _stepcallback = stepcallback;
    _modecallback = modecallback;

    if (_maxfreq > 50000000)
        _maxfreq = 50000000;
    
    return freqmgr_instance;
}

// Internal functions
void _SetMode(byte mode) {
    switch (mode) {
        case MODE_FREQ:
        case MODE_STEP:
        _mode = mode; 
        if (_modecallback != NULL)
            _modecallback(_mode);
        break;
        default: mode = MODE_FREQ;
    }
}

byte _GetMode(void) {
    return _mode;
}

void _ToggleMode(void) {
    switch(_mode) {
        case MODE_FREQ:
        _mode = MODE_STEP; 
        if (_modecallback != NULL)
            _modecallback(_mode);
        break;
        case MODE_STEP:
        _mode = MODE_FREQ; 
        if (_modecallback != NULL)
            _modecallback(_mode);
        break;
        default: break;
    }
}

void _Up(void) {
    switch (_mode) {
        case MODE_FREQ:
            if (_freq + _step <= _maxfreq) {
                _freq += _step;
                if (_freqcallback != NULL) 
                    _freqcallback(_freq);
            } else {
                if (_freq < _maxfreq) {
                    _freq = _maxfreq;
                    if (_freqcallback != NULL) 
                        _freqcallback(_freq);
                }
            }
        break;
        case MODE_STEP:
            if (_step * 10 <= _maxfreq) {
                _step *= 10;
                if (_stepcallback != NULL)
                    _stepcallback(_step);
            }
        break;
        default: break;
    }
}

void _Down(void) {
    switch (_mode) {
        case MODE_FREQ:
            if (_freq - _step >= _minfreq) {
                _freq -= _step;
                if (_freqcallback != NULL) 
                    _freqcallback(_freq);
            } else {
                if (_freq >= _minfreq) {
                    _freq = _minfreq;
                    if (_freqcallback != NULL) 
                        _freqcallback(_freq);
                }
            }
        break;
        case MODE_STEP:
            if (_step / 10 >= 1) {
                _step /= 10;
                if (_stepcallback != NULL)
                    _stepcallback(_step);
            }
        break;
        default: break;
    }
}

long _GetFreq(void) {
    return _freq;
}

long _GetStep(void) {
    return _step;
}

