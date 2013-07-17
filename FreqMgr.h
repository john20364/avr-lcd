#ifndef _FREQMGR_H_
#define _FREQMGR_H_
#include "Type.h"

#define MODE_FREQ   1
#define MODE_STEP   2

typedef void (* setmode_fptr)(byte);
typedef byte (* getmode_fptr)(void);
typedef void (* void_fptr)(void);
typedef void (* callback_fptr)(long);
typedef long (* freq_fptr)(void);

typedef struct FreqMgrStruct {
    setmode_fptr SetMode;
    getmode_fptr GetMode;
    void_fptr ToggleMode;
    void_fptr Up;
    void_fptr Down;
    freq_fptr GetFreq;    
    freq_fptr GetStep;    
} FreqMgr;

FreqMgr *FreqMgrCreate(
    long maxfreq, 
    long freq,
    long step,
    callback_fptr freqcallback,
    callback_fptr stepcallback,
    callback_fptr modecallback);
    
#endif
