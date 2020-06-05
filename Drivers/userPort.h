#ifndef _USERPORT_
#define _USERPORT_

#ifndef _DEFINITIONS_
#include "definitions.h"
#endif

enum PINPORT {PORTA = 0 , PORTB = 1 , PORTC = 2 , PORTD = 3 , PORTE = 4 , PORTF = 5 , PORTG = 6 , PORTH = 7 , PORTI = 8 , PORTJ = 9 , PORTK = 10 , PORTL = 11 , PORTM = 12 , PORTN = 13 , PORTO = 14 , PORTP = 15 , NOPORT=-1};
enum PINDIR {IN = 0 , OUT = 1 , NODIR = -1};
enum PINPULL {PULLDOWN = 0 , PULLUP = 1 , NOPULL = -1};
enum MUX {MUXA = 0 ,MUXB = 1 , MUXC = 2 , MUXD = 3 ,MUXE = 4 , MUXF = 5 , MUXG = 6 , MUXH = 7 , MUXI = 8 , MUXOFF = -1};
enum ACTION {DISABLE = 0 , ENABLE = 1};

enum MUXFUNCTION{ NOMUX = -1, EXTINT = 4,EXTWAKE = 5,AIN = 6,PTCA = 7,PTCB = 8,AOUT = 9,OPAMP_NEG = 14,OPAMP_POS = 15,OPAMP_OUT = 16,RESETN = 17,
SERCOM0PAD0 = 00,SERCOM1PAD0 = 10,SERCOM2PAD0 = 20,SERCOM3PAD0 = 30,SERCOM4PAD0 = 40,SERCOM5PAD0 = 50,SERCOM6PAD0 = 60,SERCOM7PAD0 = 70,
SERCOM0PAD1 = 01,SERCOM1PAD1 = 11,SERCOM2PAD1 = 21,SERCOM3PAD1 = 31,SERCOM4PAD1 = 41,SERCOM5PAD1 = 51,SERCOM6PAD1 = 61,SERCOM7PAD1 = 71,
SERCOM0PAD2 = 02,SERCOM1PAD2 = 12,SERCOM2PAD2 = 22,SERCOM3PAD2 = 32,SERCOM4PAD2 = 42,SERCOM5PAD2 = 52,SERCOM6PAD2 = 62,SERCOM7PAD2 = 72,
SERCOM0PAD3 = 03,SERCOM1PAD3 = 13,SERCOM2PAD3 = 23,SERCOM3PAD3 = 33,SERCOM4PAD3 = 43,SERCOM5PAD3 = 53,SERCOM6PAD3 = 63,SERCOM7PAD3 = 73,
SWDIO = 18,SWCLK = 19,VBAT = 24,USB_DM = 25,USB_DP = 26,USB_SOF = 27,VREFA = 28,VREFB = 29,ACMP = 34,XIN = 35,XOUT = 36,XIN32 = 37,XOUT32 = 38,GPIO = 39, EXTINT_WAKE = 44,
GCLKIO0 = 0, GCLKIO1 = 1 , GCLKIO2 = 2, GCLKIO3 = 3, GCLKIO4 = 4, GCLKIO5 = 5, GCLKIO6 = 6, GCLKIO7 = 7
}; // PTC: peripheral touch controller

struct pin {
  int logicalPinNumber;
  uint32_t mask;
  enum PINPORT port;
  enum MUX muxLetter;
  enum MUXFUNCTION function;
  enum PINDIR dir;
  enum PINPULL pull;
};

struct pin setPin(struct pin userPin);
struct pin setPins(struct pin userPins);
void setHigh(struct pin);
void setLow(struct pin);
void setToggle(struct pin);

int getState(struct pin);
uint32_t getMask(struct pin);
uint32_t getMuxLetter(uint32_t,enum PINPORT,enum MUXFUNCTION);

struct pin createGPIO(uint32_t , enum PINDIR , enum PINPORT , enum PINPULL);
struct pin createMUXPin(uint32_t , enum PINPORT , enum MUXFUNCTION);

uint32_t pinInvalid(struct pin);


#endif