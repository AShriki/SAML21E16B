#include "Drivers/userRTC.h"
#include "Drivers/userPort.h"
#include "Drivers/userSERCOM.h"
#include "Drivers/userADC.h"
#include "Drivers/userRNG.h"
#include "Drivers/userTC.h"

#include "Alarm/AlarmHeap.h"
#include "Initialization/userInit.h"

#include "Network/TCP.h"

uint32_t baud;

struct Heap almHeap;

static volatile unsigned int _Continue;

void HardFault_Handler(void) {
  _Continue = 0u;
  //
  // When stuck here, change the variable value to != 0 in order to step out
  //
  while (_Continue == 0u);
}


void RTC_IRQHandler(void){

  RTC->MODE2.INTFLAG.reg |= RTC_MODE2_INTFLAG_ALARM0 ;

}

int main(void) {
  almHeap.numItems = 0;

  __DMB();
  __enable_irq();
  
  //initADC(AIN1);

  initClks(0,0);
  if(initPins() == -1)return 0;

  lastPort.sercomNum = 3;  
  lastPort.rxPad = 1; 
  lastPort.txPad = 0; 
  lastPort.form = FRAME; 
  lastPort.cmode = ASYNCHRONOUS;
  lastPort.dord = LSB;
  lastPort.stopBits = ONESTOPBIT; 
  lastPort.baud = 256000;
  
  initUART(lastPort);
  initTC(0);
  initTC(1);
  //setAlarm(0,1,1,0,0,2);;
  getAddress(0);
  do{
   
  }while(1);
  
  return 0;
}

/*************************** End of file ****************************/
