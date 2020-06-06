#include "Initialization/userInit.h"
#include "Drivers/userPort.h"
// method to initialize pins to be used in this application

struct pin rE;
struct pin dE;

struct pin led[4];
struct pin comboTxRx;

struct pin irLED;

struct pin uartRx;
struct pin uartTx;

int initPins(){

  uartTx = createMUXPin(22,PORTA,SERCOM3PAD0);
  if(uartTx.logicalPinNumber == -1)return -1;

  uartRx = createMUXPin(23,PORTA,SERCOM3PAD1);
  if(uartRx.logicalPinNumber == -1)return -1;

  rE = createGPIO(9,OUT,PORTA,NOPULL);
  dE = createGPIO(8,OUT,PORTA,NOPULL);

  led[0] = createGPIO(11,OUT,PORTA,NOPULL);
  led[1] = createGPIO(7,OUT,PORTA,NOPULL);
  led[2] = createGPIO(10,OUT,PORTA,NOPULL);
  led[3] = createGPIO(6,OUT,PORTA,NOPULL);

  comboTxRx = led[1];
  comboTxRx.mask = led[1].mask | rE.mask | dE.mask;

  irLED = createGPIO(24,OUT,PORTA,NOPULL);

  return 1;
}

void initClks(int exOscSel,int exOsc32k){
    if(exOscSel){
      OSCCTRL->OSC16MCTRL.bit.ENABLE = 0;
      // start the al oscillator after 8, 32kHz cycles. _xtalen connects the crystal to the internals of the MCU
      OSCCTRL->XOSCCTRL.reg = OSCCTRL_XOSCCTRL_ENABLE | OSCCTRL_XOSCCTRL_XTALEN | OSCCTRL_XOSCCTRL_STARTUP(0x3) | OSCCTRL_XOSCCTRL_GAIN(0x4);
    }

    if(exOsc32k){
      OSC32KCTRL->OSC32K.bit.ENABLE = 0;

      OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_EN1K | OSC32KCTRL_XOSC32K_EN32K | OSC32KCTRL_XOSC32K_XTALEN | OSC32KCTRL_XOSC32K_EN1K | OSC32KCTRL_XOSC32K_ENABLE ;
    
      OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC1K ;
    }else{
      uint32_t calData = *(uint32_t *) NVMCTRL_OTP5;
      // isolate calibration data for internal oscillator
      uint8_t OSCCal = ( calData & 0b1111111000000 ) >> 6 ;
      // enable 1k and 32k outputs
      // enable the 32k oscillator
      // set the calibration values
      OSC32KCTRL->OSC32K.reg = OSC32KCTRL_OSC32K_EN32K | OSC32KCTRL_OSC32K_EN1K | OSC32KCTRL_OSC32K_ENABLE | OSC32KCTRL_OSC32K_CALIB(OSCCal) ;
      // set the rtc clock to run from 1.024kHz derived from the 32k internal oscillator (not the ultra low powered one)
      OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_OSC1K ;
    }
    
    GCLK->GENCTRL[1].bit.SRC = GCLK_GENCTRL_SRC_OSC32K;
    GCLK->GENCTRL[1].bit.DIV = 0;
    GCLK->GENCTRL[1].bit.GENEN = 1;
    //GCLK->GENCTRL[1].bit.OE = 1;
    GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_OSC32K;
    while(GCLK->SYNCBUSY.reg){;}
    OSCCTRL->OSC16MCTRL.bit.ENABLE = 0;
    OSCCTRL->OSC16MCTRL.bit.FSEL = 3;
    OSCCTRL->OSC16MCTRL.bit.ENABLE = 1;
    while(!OSCCTRL->INTFLAG.bit.OSC16MRDY){;}
    GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_OSC16M;
    while(GCLK->SYNCBUSY.reg){;}
    MCLK->APBAMASK.reg |= MCLK_APBAMASK_PM | MCLK_APBAMASK_GCLK | MCLK_APBAMASK_MCLK ;
    while(!MCLK->INTFLAG.bit.CKRDY){;}
    MCLK->INTFLAG.bit.CKRDY = 1;
}