#include "Drivers/userPort.h"
#include "saml21.h"

struct pinTableRow {
  uint32_t phyPinNum;
  uint32_t logicalPinNum;
  uint32_t pinMask;
  enum PINPORT port;
  uint32_t extIntNum;
  uint32_t adcNum;
  enum MUXFUNCTION function[NUM_MUX_FUNCTIONS];
};

const struct pinTableRow pins[MUXABLEPINS] = {
{1,0,PORT_PA00,PORTA,0,-1,{EXTINT_WAKE,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM1PAD0,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{2,1,PORT_PA01,PORTA,1,-1,{EXTINT_WAKE,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM1PAD1,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{3,2,PORT_PA02,PORTA,2,0,{EXTINT_WAKE,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{4,3,PORT_PA03,PORTA,3,1,{EXTINT_WAKE,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{5,4,PORT_PA04,PORTA,4,4,{EXTINT_WAKE,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD0,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{6,5,PORT_PA05,PORTA,5,5,{EXTINT_WAKE,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD1,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{7,6,PORT_PA06,PORTA,6,6,{EXTINT_WAKE,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD2,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{8,7,PORT_PA07,PORTA,7,7,{EXTINT_WAKE,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD3,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{11,8,PORT_PA08,PORTA,-1,16,{NOMUX,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD0,SERCOM2PAD0,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{12,9,PORT_PA09,PORTA,9,17,{EXTINT,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD1,SERCOM2PAD1,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{13,10,PORT_PA10,PORTA,10,18,{EXTINT,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD2,SERCOM2PAD2,NOMUX,NOMUX,NOMUX,GCLKIO4,NOMUX}},
{14,11,PORT_PA11,PORTA,11,19,{EXTINT,NOMUX,AIN,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM0PAD3,SERCOM2PAD3,NOMUX,NOMUX,NOMUX,GCLKIO5,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO4,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO5,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO6,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO7,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO0,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO1,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{15,14,PORT_PA14,PORTA,14,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM2PAD2,SERCOM4PAD2,NOMUX,NOMUX,NOMUX,GCLKIO0,NOMUX}},
{16,15,PORT_PA15,PORTA,15,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM2PAD3,SERCOM4PAD3,NOMUX,NOMUX,NOMUX,GCLKIO1,NOMUX}},
{17,16,PORT_PA16,PORTA,0,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM1PAD0,SERCOM3PAD0,NOMUX,NOMUX,NOMUX,GCLKIO2,NOMUX}},
{18,17,PORT_PA17,PORTA,1,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM1PAD1,SERCOM3PAD1,NOMUX,NOMUX,NOMUX,GCLKIO3,NOMUX}},
{19,18,PORT_PA18,PORTA,2,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM1PAD2,SERCOM3PAD2,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{20,19,PORT_PA19,PORTA,3,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM1PAD3,SERCOM3PAD3,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO2,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO3,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO4,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO5,NOMUX}},
{21,22,PORT_PA22,PORTA,6,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM3PAD0,SERCOM5PAD0,NOMUX,NOMUX,NOMUX,GCLKIO6,NOMUX}},
{22,23,PORT_PA23,PORTA,7,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM3PAD1,SERCOM5PAD1,NOMUX,NOMUX,USB_SOF,GCLKIO7,NOMUX}},
{23,24,PORT_PA24,PORTA,12,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM3PAD2,SERCOM5PAD2,NOMUX,NOMUX,USB_DM,NOMUX,NOMUX}},
{24,25,PORT_PA25,PORTA,13,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,SERCOM3PAD3,SERCOM5PAD3,NOMUX,NOMUX,USB_DP,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO0,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO1,NOMUX}},
{25,27,PORT_PA27,PORTA,15,-1,{EXTINT,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO0,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO1,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO0,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,GCLKIO0,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
{-1,-1,-1,NOPORT,-1,-1,{NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX,NOMUX}},
};

void setHigh(struct pin userPin){
  PORT->Group[userPin.port].OUTSET.bit.OUTSET = userPin.mask;
}
void setLow(struct pin userPin){
  PORT->Group[userPin.port].OUTCLR.bit.OUTCLR = userPin.mask;
}
void setToggle(struct pin userPin){
  PORT->Group[userPin.port].OUTTGL.bit.OUTTGL = userPin.mask;
}
/*
  @description : this method verifies the pin settings ans sets the pin according to the settings stored in a pin variable

  @var userPin : a user created pin variable
  @return      : a verified pin variable
*/
struct pin setPin(struct pin userPin){
 
  uint8_t userHwSel = 0;
  uint8_t gpio;
  if(pinInvalid(userPin)){

    userPin.dir = NODIR;
    userPin.function = NOMUX;
    userPin.logicalPinNumber = -1;
    userPin.mask = 0;
    userPin.muxLetter = MUXOFF;
    userPin.port = NOPORT;
    userPin.pull = NOPULL;

    return userPin;
  }
  if(userPin.dir != -1){
    if(userPin.dir == 1)
      PORT->Group[userPin.port].DIRSET.reg |= (userPin.mask & (( userPin.dir ) << userPin.logicalPinNumber ));

    if(userPin.dir == 0){
      PORT->Group[userPin.port].DIRCLR.reg |= userPin.mask;
    }
  }
  else{
    PORT->Group[userPin.port].PMUX[userPin.logicalPinNumber/2].reg |= (PORT_PMUX_PMUXE( ( (userPin.logicalPinNumber % 2 == 0) * 0xF) & userPin.muxLetter ) | PORT_PMUX_PMUXO( ( ( userPin.logicalPinNumber%2 == 1) * 0xF) & userPin.muxLetter) ) & (0xFF * (userPin.function != GPIO) );
    PORT->Group[userPin.port].PINCFG[userPin.logicalPinNumber].reg |= ( (userPin.pull != NOPULL) << PORT_PINCFG_PULLEN_Pos ) | ( !userPin.dir << PORT_PINCFG_INEN_Pos ) | ( (userPin.function != GPIO) << PORT_PINCFG_PMUXEN_Pos );
  }
  if((userPin.pull == PULLUP)){
      PORT->Group[userPin.port].DIRSET.reg |= userPin.mask;
    }
  else if ((userPin.pull == PULLDOWN)){
    PORT->Group[userPin.port].DIRCLR.reg |= userPin.mask;
  }
  return userPin;
}
/*
  @description : checks a pin's settings and ensures them to be valid

  @var userPin : pin to check
  @return      : 1 if the pin is invalid, 0 if it's okay 
*/
uint32_t pinInvalid(struct pin userPin){
  int i = 0;
  int j = 0;
  // check port and mux function
  for(i=0; i < MUXABLEPINS; i++){
    uint32_t t1 = pins[i].logicalPinNum;
    if(userPin.logicalPinNumber == pins[i].logicalPinNum && userPin.port == pins[i].port){
      for(j=0; j < NUM_MUX_FUNCTIONS; j++){
    	  if(pins[i].function[j] == userPin.function || userPin.function == GPIO){
            return 0;
    	  }
      }
    }
  }
  return 1;
}
/*
  @description       : This function finds the mux letter as defined by the muxDistribution array

  @var logicalPinNum : the logical pin number (PA_xx or PB_xx)
  @var port          : the pin port
  @var function      : the desired mux function
  @return            : -1 if inalid, otherwise it returns the mux letter defined in the enum
*/
uint32_t getMuxLetter(uint32_t logicalPinNum , enum PINPORT port , enum MUXFUNCTION function){
  int i,j,k,sum;
  
  int muxLetters[NUM_MUX_FUNCTIONS];
  for(i = 0 , k = 0; i < NUM_MUX_LETTERS ; i++){
    for(j = 0 ; j < muxDistribution[i] ; j++){
      muxLetters[k++] = i;
    }
  }

  sum = 0;

  // Search through every available pin, looking for a matching logical number and then search through the MUX functions of that pin and return the MUX letter it is filed under
  
  for(i = 0; i < MUXABLEPINS; i++){
    if(logicalPinNum == pins[i].logicalPinNum && port == pins[i].port){
      for(j = 0; j < NUM_MUX_FUNCTIONS; j++){
        if(pins[i].function[j] == function){
            return muxLetters[j];
        }
      }
    }
  }
  return -1;
}
/*
  @description : Sets the pin mask of userPin

  @var userPin : the pin whos mask we want
  @return      : The pin mask of userPin
*/
uint32_t getMask(struct pin userPin){

  if(pinInvalid(userPin)){
    return 0;
  }

  int i;
  for(i = 0; i < MUXABLEPINS ; i++){
    if(pins[i].port == userPin.port && pins[i].logicalPinNum == userPin.logicalPinNumber){
      return pins[i].pinMask;
    }
  }
}
/*
  @description : Gets the logic level of the specified pin (high or low)

  @var userPin : get the state of this pin
  @return      : the pin state (high or low)   
*/
int getState(struct pin userPin){
  return (PORT->Group[userPin.port].OUT.reg & userPin.mask)>>userPin.logicalPinNumber;
} 
/*
  @description       : creates a GPIO pin (set high or low or digital read) with the specified settings

  @var logicalPinNum : the logical pin number (PA_xx or PB_xx)
  @var dir           : specifies the pin as input or output
  @var port          : the port in which the pin is located
  @var pull          : pin pull up or down or none
  @return            : the pin structure with the specified settings 
*/
struct pin createGPIO(uint32_t logicalPinNumber, enum PINDIR dir, enum PINPORT port, enum PINPULL pull){
    struct pin userPin;
    userPin.dir = dir;
    userPin.function = GPIO;
    userPin.logicalPinNumber = logicalPinNumber;
    userPin.port = port;
    userPin.pull = pull;
    userPin.muxLetter = NOMUX;
    userPin.mask = getMask(userPin);

    if(pinInvalid(userPin)){
      userPin.mask = 0;
      return userPin;
    }
    return setPin(userPin);
}
/*
  @description       : creates a pin which uses a function specified by the mux and NOT GPIO

  @var logicalPinNum : the logical pin number (PA_xx or PB_xx)
  @var port          : the pin port
  @var function      : the desired mux function
  @return            : a verified pin with the desired mux settings
*/
struct pin createMUXPin(uint32_t logicalPinNumber , enum PINPORT port , enum MUXFUNCTION function){
    struct pin userPin;

    userPin.function = function;
    userPin.logicalPinNumber = logicalPinNumber;
    userPin.port = port;
    userPin.muxLetter = getMuxLetter(logicalPinNumber , port , function);
    userPin.mask = getMask(userPin); 
    userPin.dir = -1;
    if(pinInvalid(userPin)){
      userPin.mask = 0;
      return userPin;
    }
    return setPin(userPin);
}
/*
  @description   : creates a pin mask consisting of multiple pins with the same settings

  @var  *userPin : a pointer to a list of pins
  @var numPins   : the number of pins in the list
  @return        : a "pin" structure consisting of many pins
*/
struct pin createPinMask(struct pin *userPin, uint32_t numPins){
  int i;
  struct pin pinMasks;
  
  if(numPins <= 1)
    return pinMasks;
  
  pinMasks = userPin[0];
  
  for(i = 1 ; i < numPins ; i++) {
    if(pinInvalid(userPin[i])){
      return userPin[i];
    }
    pinMasks.mask |= userPin[i].mask;
  }

  return pinMasks;
}

/* END */