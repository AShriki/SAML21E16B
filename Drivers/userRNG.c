#include "userRNG.h"
#include "saml21.h"

// requires 84 clock cycles to generate a number
/*
  @description : generates a random number 32 bits long

  @return      : a random number
*/
uint32_t randomNumber(){
  
  TRNG->CTRLA.bit.ENABLE = 1 ;

  while(!TRNG->INTFLAG.bit.DATARDY){;}

  TRNG->CTRLA.bit.ENABLE = 0;

  return TRNG->DATA.reg;

}