#ifndef _USERTC_
#define _USERTC_

#ifndef _DEFINITIONS_
#include "definitions.h"
#endif

typedef void (*alarmAction)(uint8_t, void*);

struct TCAction{
  alarmAction func;
  int param;
  void* p;
};

void initTC(int); // @param : TC 0,1,4
void setTimer(uint8_t,float,uint8_t); // @param : (tc num,miliseconds,interrupt enable (0 or 1))
void stopTimer(int);// @param : tc to be stopped
void startTimer(int,uint8_t);// @param : tc to be started , interrupt enable
void disableTimer(int);// @param : tc to be stopped
uint8_t timerFinished(int);// @param : tc to be stopped @return 1 if timer finished 0 if not
void wait(float);// @param : time in miliseconds
void resetTimer(int);// @param : tc number to be reset
#endif