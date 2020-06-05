#include "userTC.h"
#include "saml21.h"

#define OSC 0

#if(OSC==1)
#define BASECLK 32768
#elif(OSC==0)
#define BASECLK 16000000
#endif

float scaledTimerMax[8];
float scaledTimerFactor[8];
Tc *selTc[5] = {TC0,TC1,0,0,TC4};

/*
  @description : initializes one tc instance
  @var tc : the timer-counter to be instantiated
  @return : nothing
*/
void initTC(int tc){
  
  scaledTimerMax[0] = (uint32_t)(UINT16_MAX) * (1000/(float)BASECLK);
  scaledTimerMax[1] = (uint32_t)(UINT16_MAX) * (2000/(float)BASECLK);
  scaledTimerMax[2] = (uint32_t)(UINT16_MAX) * (4000/(float)BASECLK);
  scaledTimerMax[3] = (uint32_t)(UINT16_MAX) * (8000/(float)BASECLK);
  scaledTimerMax[4] = (uint32_t)(UINT16_MAX) * (16000/(float)BASECLK);
  scaledTimerMax[5] = (uint32_t)(UINT16_MAX) * (64000/(float)BASECLK);
  scaledTimerMax[6] = (uint32_t)(UINT16_MAX) * (256000/(float)BASECLK);
  scaledTimerMax[7] = (uint32_t)(UINT16_MAX) * (1024000/(float)BASECLK);

  scaledTimerFactor[0] = BASECLK/1000.0;
  scaledTimerFactor[1] = BASECLK/2000.0;
  scaledTimerFactor[2] = BASECLK/4000.0;
  scaledTimerFactor[3] = BASECLK/8000.0;
  scaledTimerFactor[4] = BASECLK/16000.0;
  scaledTimerFactor[5] = BASECLK/64000.0;
  scaledTimerFactor[6] = BASECLK/256000.0;
  scaledTimerFactor[7] = BASECLK/1024000.0;

  int tcIndex = ( tc / 2 ) + 27;

  MCLK->AHBMASK.bit.HPB2_ = 1;

  switch(tc){
    case 0:{MCLK->APBCMASK.bit.TC0_ |= 1 ; NVIC_EnableIRQ(TC0_IRQn) ;break;}
    case 1:{MCLK->APBCMASK.bit.TC1_ |= 1 ; NVIC_EnableIRQ(TC1_IRQn) ;break;}
    case 4:{MCLK->APBDMASK.bit.TC4_ |= 1 ; NVIC_EnableIRQ(TC4_IRQn) ;break;}
    default : return;
  };

  GCLK->PCHCTRL[tcIndex].bit.GEN = OSC;
  GCLK->PCHCTRL[tcIndex].bit.CHEN = 1;

  while ((GCLK->PCHCTRL[tcIndex].reg & GCLK_PCHCTRL_CHEN) == 0);

  selTc[tc]->COUNT16.CTRLA.bit.ENABLE = 0;
  selTc[tc]->COUNT16.CTRLA.bit.PRESCSYNC = 0;
  selTc[tc]->COUNT16.CTRLA.bit.MODE = 0;

  //selTc[tc]->COUNT16.DBGCTRL.bit.DBGRUN = 1u;

  while(selTc[tc]->COUNT16.SYNCBUSY.reg){;}
}
/*
  @description      : Sets the timer-counter value in microseconds
  @var interruptEn  : variable to enable the interrupt on timer completion
  @var microSeconds : the number of microseconds to count
  @return           : nothing
*/
void setTimer(uint8_t tc , float mS , uint8_t interruptEn){

  TC0->COUNT16.INTFLAG.bit.MC0 = 1;
  selTc[tc]->COUNT16.CTRLA.bit.ENABLE = 0u;
  
  if(mS < scaledTimerMax[0]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 0;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[0];
  }
  else if(mS < scaledTimerMax[1]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 1;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[1];
  }
  else if(mS < scaledTimerMax[2]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 2;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[2];
  }
  else if(mS < scaledTimerMax[3]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 3;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[3];
  }
  else if(mS < scaledTimerMax[4]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 4;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[4];
  }
  else if(mS < scaledTimerMax[5]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 5;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[5];
  }
  else if(mS < scaledTimerMax[6]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 6;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[6];
  }
  else if(mS < scaledTimerMax[7]){
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 7;
    selTc[tc]->COUNT16.CC[0].reg = mS * scaledTimerFactor[7];
  }
  else{
    selTc[tc]->COUNT16.CTRLA.bit.PRESCALER = 7;
    selTc[tc]->COUNT16.CC[0].reg = UINT16_MAX;
  }

  selTc[tc]->COUNT16.INTENSET.bit.MC0 = interruptEn;
  selTc[tc]->COUNT16.INTENCLR.bit.MC0 = !interruptEn;

  while(selTc[tc]->COUNT16.SYNCBUSY.reg){;}
}
/*
  @description : stops a counter
  @var tc : tc to be stopped 
  @return : nothing
*/
void startTimer(int tc,uint8_t intEn){
  while(selTc[0]->COUNT16.SYNCBUSY.reg){;}
  selTc[tc]->COUNT16.INTENSET.bit.MC0 = intEn;
  selTc[tc]->COUNT16.CTRLA.bit.ENABLE = 1;
  selTc[tc]->COUNT16.CTRLBSET.bit.CMD = 1;
}
/*
  @description : stops a counter
  @var tc : tc to be stopped 
  @return : nothing
*/
void resetTimer(int tc){
  selTc[tc]->COUNT16.CTRLA.bit.ENABLE = 1;
  selTc[tc]->COUNT16.INTFLAG.bit.MC0 = 1;
  selTc[tc]->COUNT16.COUNT.reg = 0;
}
/*
  @description : stops a counter
  @var tc : tc to be stopped 
  @return : nothing
*/
void stopTimer(int tc){
  selTc[tc]->COUNT16.INTFLAG.bit.MC0 = 1;
  selTc[tc]->COUNT16.CTRLA.bit.ENABLE = 0;
  selTc[tc]->COUNT16.INTENCLR.bit.MC0 = 1;
  while(selTc[tc]->COUNT16.SYNCBUSY.reg){;}
}
/*
  @description : disables a counter
  @var tc : counter to be disabled 
  @return : nothing
*/
void disableTimer(int tc){
  switch(tc){
    case 0:{MCLK->APBCMASK.bit.TC0_ = 0u ; NVIC_DisableIRQ(TC0_IRQn) ; stopTimer(0) ; break;}
    case 1:{MCLK->APBCMASK.bit.TC1_ = 0u ; NVIC_DisableIRQ(TC1_IRQn) ; stopTimer(1) ; break;}
    case 4:{MCLK->APBDMASK.bit.TC4_ = 0u ; NVIC_DisableIRQ(TC4_IRQn) ; stopTimer(4) ; break;}
    default : return;
  };
}
/*
  @description : determines if the timer is finished when called

  @var tc : checks the current tc instance
  @return : returns 1 the timer is finished or 0 if not
*/
uint8_t timerFinished(int tc){
  return selTc[tc]->COUNT16.INTFLAG.bit.MC0;
}

/*
  @description : idles for AT LEAST a number of miliseconds
  @var t : number of miliseconds to wait
  @return : nothing
*/
void wait(float mS){

  TC0->COUNT16.INTFLAG.bit.MC0 = 1;
  selTc[0]->COUNT16.CTRLA.bit.ENABLE = 0u;
  
  if(mS < scaledTimerMax[0]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 0;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[0];
  }
  else if(mS < scaledTimerMax[1]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 1;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[1];
  }
  else if(mS < scaledTimerMax[2]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 2;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[2];
  }
  else if(mS < scaledTimerMax[3]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 3;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[3];
  }
  else if(mS < scaledTimerMax[4]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 4;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[4];
  }
  else if(mS < scaledTimerMax[5]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 5;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[5];
  }
  else if(mS < scaledTimerMax[6]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 6;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[6];
  }
  else if(mS < scaledTimerMax[7]){
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 7;
    selTc[0]->COUNT16.CC[0].reg = mS * scaledTimerFactor[7];
  }
  else{
    selTc[0]->COUNT16.CTRLA.bit.PRESCALER = 7;
    selTc[0]->COUNT16.CC[0].reg = UINT16_MAX;
  }

  while(selTc[0]->COUNT16.SYNCBUSY.reg){;}

  selTc[0]->COUNT16.CTRLA.bit.ENABLE = 1;
  while(!TC0->COUNT16.INTFLAG.bit.MC0){;}
}