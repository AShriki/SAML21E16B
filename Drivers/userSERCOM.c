#include "Drivers/userSERCOM.h"
#include "Drivers/userPort.h"
#include "Initialization/userInit.h"

#include <stdlib.h>

#define SetSercomDetails(n) switch(n){                                                                                                                            \
                             case 0:userSercom = SERCOM0 ; userSercomIRQn = SERCOM0_IRQn ; UART_SERCOM_GCLK_CORE = 18 ; UART_SERCOM_GCLK_SLOW = 17 ; break;   \
                             case 1:userSercom = SERCOM1 ; userSercomIRQn = SERCOM1_IRQn ; UART_SERCOM_GCLK_CORE = 19 ; UART_SERCOM_GCLK_SLOW = 17 ; break;   \
                             case 2:userSercom = SERCOM2 ; userSercomIRQn = SERCOM2_IRQn ; UART_SERCOM_GCLK_CORE = 20 ; UART_SERCOM_GCLK_SLOW = 17 ; break;   \
                             case 3:userSercom = SERCOM3 ; userSercomIRQn = SERCOM3_IRQn ; UART_SERCOM_GCLK_CORE = 21 ; UART_SERCOM_GCLK_SLOW = 17 ; break;   \
                             case 4:userSercom = SERCOM4 ; userSercomIRQn = SERCOM4_IRQn ; UART_SERCOM_GCLK_CORE = 22 ; UART_SERCOM_GCLK_SLOW = 17 ; break;   \
                             case 5:userSercom = SERCOM5 ; userSercomIRQn = SERCOM5_IRQn ; UART_SERCOM_GCLK_CORE = 24 ; UART_SERCOM_GCLK_SLOW = 23 ; break;   \
                             default:userSercom = SERCOM0 ; userSercomIRQn = SERCOM0_IRQn ; UART_SERCOM_GCLK_CORE = 18 ; UART_SERCOM_GCLK_SLOW = 17 ; break; }

Sercom * userSercom;
uint8_t userSercomIRQn;
uint8_t UART_SERCOM_GCLK_CORE;
uint8_t UART_SERCOM_GCLK_SLOW;

uint8_t sampr; 
uint8_t userSercomIRQn;
uint8_t UART_SERCOM_GCLK_CORE;
uint8_t UART_SERCOM_GCLK_SLOW;


uint8_t fractionalPart(uint16_t *, uint32_t, uint8_t);
// Initializes the UART
void initUART(struct SERCOMPort port){

  uint16_t baudValue;
  
  

  SetSercomDetails(port.sercomNum);

  GCLK->PCHCTRL[UART_SERCOM_GCLK_SLOW].reg &= ~GCLK_PCHCTRL_CHEN;
  GCLK->PCHCTRL[UART_SERCOM_GCLK_SLOW].reg = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN(0u); // enable PERIPHERAL clock for sercom SLOW. Used by sercom 0 - 4
  
    NVIC_EnableIRQ(userSercomIRQn);

    /* - - - - CLOCK ENABLES - - - - */
    GCLK->PCHCTRL[UART_SERCOM_GCLK_CORE].reg &= ~GCLK_PCHCTRL_CHEN; // switch off
    GCLK->PCHCTRL[UART_SERCOM_GCLK_CORE].reg = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN(0u); // enable PERIPHERAL clock for sercom CORE (GCLK0 enabled by defauls and references main oscilator)
    /* - - - - - - - - - - - - - - - */

    userSercom->USART.CTRLA.bit.ENABLE = 0;
    userSercom->USART.CTRLA.bit.MODE = 1; // Internal clock used. 0 for synchronous to accept clock through external pin
    userSercom->USART.CTRLA.bit.CMODE = port.cmode; 
    userSercom->USART.CTRLA.bit.RXPO = port.rxPad; 
    userSercom->USART.CTRLA.bit.TXPO = port.txPad;
    userSercom->USART.CTRLB.bit.CHSIZE = 0; // 8 bit char size
    userSercom->USART.CTRLB.bit.SBMODE = port.stopBits;
    userSercom->USART.CTRLA.bit.DORD = port.dord;
    userSercom->USART.CTRLA.bit.SAMPR = 3; // 8x sampling using fractional (0x1 for 16x oversample)
    // get fractional and integer parts of the baud
    uint8_t fp = fractionalPart(&baudValue,port.baud , 8);
    userSercom->USART.CTRLA.bit.FORM = port.form; // frame = 0, frame w/ parity = 1, autobaud = 2 , autobaud with parity = 3
    userSercom->USART.INTENSET.bit.RXC = 1;
    userSercom->USART.BAUD.reg = SERCOM_USART_BAUD_FRACFP_BAUD(baudValue) | SERCOM_USART_BAUD_FRACFP_FP(fp) ; 
    userSercom->USART.CTRLB.bit.RXEN = 1; // enables
    userSercom->USART.CTRLB.bit.TXEN = 1;
    userSercom->USART.CTRLA.bit.ENABLE = 1;

    while(userSercom->USART.SYNCBUSY.bit.ENABLE | userSercom->USART.SYNCBUSY.bit.CTRLB){}
}

/*
  @description : sends a string message over uart
  @var raw     : the raw string to send
  @return      : nothing
*/
void sendUART(int sercom,char *raw , int len){
  int i = 0;
  disableUARTInterrupts(sercom);
  SetSercomDetails(sercom);
  setHigh(comboTxRx);
  for (;i < len; i++){
    if(userSercom->USART.INTFLAG.bit.DRE){
      userSercom->USART.DATA.reg = *(raw + i);
      while(!userSercom->USART.INTFLAG.bit.TXC){;}
    }
  }
  setLow(comboTxRx);
  enableUARTInterrupts(sercom);
}

/*
  @description    : determines the baud value to be used in the register
  @var baud       : the desired baud
  @var *baudValue : the value to be put into the register
  @return         : the fractional part of the register baud value
*/
uint8_t fractionalPart(uint16_t *baudValue , uint32_t baud , uint8_t sample){
    
    // equation from page 506 of data sheet for atsaml
    float baudVal = (FCPU / (float)(sample * baud));
    *baudValue = (uint16_t)baudVal;
    float idealFraction = baudVal - *baudValue;
    float smallestDiff = 254;
    uint8_t fp;
    uint8_t i;
    // find fraction of 8 that most closely matches the fractional part of baudVal 
    for(i = 0; i < 8 ; i++){
      float diff = idealFraction - (i/8.0);
      if(diff < 0)
        diff *= -1;
      if(diff < smallestDiff){
        smallestDiff = diff;
        fp = i;
      }
    }
    
    return fp;
} 

void disableUARTInterrupts(int n){
  SetSercomDetails(n);
  userSercom->USART.INTENCLR.reg = 0xFF;
}
void enableUARTInterrupts(int n){
  SetSercomDetails(n);
  userSercom->USART.INTENSET.reg = SERCOM_USART_INTENSET_ERROR | SERCOM_USART_INTENSET_RXC | SERCOM_USART_INTENSET_RXS;
}