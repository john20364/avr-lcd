#ifndef _AD9850_H_
#define _AD9850_H_

#include "Type.h"

typedef void (* setfreq_fptr)(long);

typedef struct AD9850Struct {
    setfreq_fptr SetFrequency;
} AD9850;

AD9850 *AD9850Create(
    byte ctrlregister,
    byte w_clk,
    byte fq_ud,
    byte data, 
    byte reset);
    
#endif
