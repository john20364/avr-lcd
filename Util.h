#ifndef _UTIL_H_
#define _UTIL_H_

#include "Type.h"

#define REGISTER_A  1
#define REGISTER_B  2
#define REGISTER_C  4
#define REGISTER_D  8

volatile uint8_t *GetPort(byte dataregister);
volatile uint8_t *GetDDR(byte dataregister);

#endif
