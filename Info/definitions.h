#ifndef _DEFINITIONS_
#define _DEFINITIONS_

#include "saml21.h"
#include <stdint.h>

#define BOARDTYPE 0
#define BOARDVERSION 1
#define BOARDPRIORITY 3 

#define REFYEAR 2016

#define FCPU 16000000

#define UART_GCLK 0

#define NUM_MUX_FUNCTIONS 14

#define NUM_MUX_LETTERS 9

#define MUXABLEPINS 51
// this is defined in definitions.c
extern const  uint32_t muxDistribution[NUM_MUX_LETTERS];

#endif