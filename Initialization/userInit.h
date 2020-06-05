#ifndef _UINIT_
#define _UINIT_

#ifndef _USERPORT_
#include "Drivers/userPort.h"
#endif

extern struct pin rE;
extern struct pin dE;

extern struct pin led[4];

extern struct pin comboTxRx;

extern struct pin irLED;

extern struct pin gClkIO;

int initPins(void);
void initClks(int,int);

#endif