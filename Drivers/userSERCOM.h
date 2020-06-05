#ifndef _USERSERCOM_
#define _USERSERCOM_

#ifndef _DEFINITIONS_
#include "definitions.h"
#endif

extern Sercom * userSercom;

enum SERDORD{MSB = 0 , LSB = 1};
enum SERFORM{FRAME = 0 , FRAMEPARITY = 1 , AUTOBAUDDETECT = 4 , AUTOBAUDBREAKDETECTPARITY = 5};
enum SERCMODE{ASYNCHRONOUS = 0, SYNCHRONOUS = 1};
enum SERRXPO{RXPAD0 = 0 , RXPAD1 = 1 , RXPAD2 = 2 , RXPAD3 = 3};
enum SERTXPO{TXPAD0 = 0 , TXPAD2 = 1};
enum SERSAMPR{ARITH16 = 0 ,FRACT16 = 1 ,ARITH8 = 2 ,FRACT8 = 3 ,ARITH3 = 4};
enum PARITY{EVEN = 0, ODD = 1};
enum SERSFDE{DISABLED = 0, ONRXC,ONRXS,ONRXSRXC};
enum SERSTOPB{ONESTOPBIT = 0, TWOSTOPBITS = 1};
enum SERCHARSZ{BITS8 = 0, BITS9 = 1, BITS5 = 5, BITS6 = 6, BITS7 = 7};

struct SERCOMPort{
  int sercomNum;
  int rxPad;
  int txPad;
  enum SERFORM form; 
  enum SERCMODE cmode; 
  enum SERDORD dord; 
  enum PARITY parity; 
  enum SERSTOPB stopBits; 
  uint32_t baud;
};
void initUART(struct SERCOMPort);
void sendUART(int,char [], int);
void disableUARTInterrupts(int);
void enableUARTInterrupts(int);

#endif