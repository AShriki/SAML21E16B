#include "Network/TCP.h"
#include "Drivers/userSERCOM.h"
#include "Drivers/userTC.h"
#include "Drivers/userPort.h"
#include "Drivers/userRNG.h"
#include "Initialization/userInit.h"

unsigned short crc16(const void * , int);

struct SERCOMPort lastPort;

uint8_t lastReceived[PACKET_SIZE];
uint8_t tmpTXBuf[PACKET_SIZE];
uint16_t usedSpace;
volatile uint8_t receivedMsg[BUF_SIZE];

uint32_t theirLastSeqNum; // last recieved seq number
uint32_t myLastSeqNum; // last sent seq number

uint16_t openChatAddr;
uint16_t myAddress;

uint8_t recieveInProgress;

uint8_t bytesReceived;

uint32_t fingerprint[(CHKSUM_START - DATA_START) / 4];

float byteSendTime;

const uint16_t crc16Table[256] = { 0x0,0xAC9A,0xF5AE,0x5934,0x47C6,0xEB5C,0xB268,0x1EF2,0x8F8C,0x2316,0x7A22,0xD6B8,0xC84A,0x64D0,0x3DE4,0x917E,
                                   0xB382,0x1F18,0x462C,0xEAB6,0xF444,0x58DE,0x1EA,0xAD70,0x3C0E,0x9094,0xC9A0,0x653A,0x7BC8,0xD752,0x8E66,0x22FC,
                                   0xCB9E,0x6704,0x3E30,0x92AA,0x8C58,0x20C2,0x79F6,0xD56C,0x4412,0xE888,0xB1BC,0x1D26,0x3D4,0xAF4E,0xF67A,0x5AE0,
                                   0x781C,0xD486,0x8DB2,0x2128,0x3FDA,0x9340,0xCA74,0x66EE,0xF790,0x5B0A,0x23E,0xAEA4,0xB056,0x1CCC,0x45F8,0xE962,
                                   0x3BA6,0x973C,0xCE08,0x6292,0x7C60,0xD0FA,0x89CE,0x2554,0xB42A,0x18B0,0x4184,0xED1E,0xF3EC,0x5F76,0x642,0xAAD8,
                                   0x8824,0x24BE,0x7D8A,0xD110,0xCFE2,0x6378,0x3A4C,0x96D6,0x7A8,0xAB32,0xF206,0x5E9C,0x406E,0xECF4,0xB5C0,0x195A,
                                   0xF038,0x5CA2,0x596,0xA90C,0xB7FE,0x1B64,0x4250,0xEECA,0x7FB4,0xD32E,0x8A1A,0x2680,0x3872,0x94E8,0xCDDC,0x6146,
                                   0x43BA,0xEF20,0xB614,0x1A8E,0x47C,0xA8E6,0xF1D2,0x5D48,0xCC36,0x60AC,0x3998,0x9502,0x8BF0,0x276A,0x7E5E,0xD2C4,
                                   0x774C,0xDBD6,0x82E2,0x2E78,0x308A,0x9C10,0xC524,0x69BE,0xF8C0,0x545A,0xD6E,0xA1F4,0xBF06,0x139C,0x4AA8,0xE632,
                                   0xC4CE,0x6854,0x3160,0x9DFA,0x8308,0x2F92,0x76A6,0xDA3C,0x4B42,0xE7D8,0xBEEC,0x1276,0xC84,0xA01E,0xF92A,0x55B0,
                                   0xBCD2,0x1048,0x497C,0xE5E6,0xFB14,0x578E,0xEBA,0xA220,0x335E,0x9FC4,0xC6F0,0x6A6A,0x7498,0xD802,0x8136,0x2DAC,
                                   0xF50,0xA3CA,0xFAFE,0x5664,0x4896,0xE40C,0xBD38,0x11A2,0x80DC,0x2C46,0x7572,0xD9E8,0xC71A,0x6B80,0x32B4,0x9E2E,
                                   0x4CEA,0xE070,0xB944,0x15DE,0xB2C,0xA7B6,0xFE82,0x5218,0xC366,0x6FFC,0x36C8,0x9A52,0x84A0,0x283A,0x710E,0xDD94,
                                   0xFF68,0x53F2,0xAC6,0xA65C,0xB8AE,0x1434,0x4D00,0xE19A,0x70E4,0xDC7E,0x854A,0x29D0,0x3722,0x9BB8,0xC28C,0x6E16,
                                   0x8774,0x2BEE,0x72DA,0xDE40,0xC0B2,0x6C28,0x351C,0x9986,0x8F8,0xA462,0xFD56,0x51CC,0x4F3E,0xE3A4,0xBA90,0x160A,
                                   0x34F6,0x986C,0xC158,0x6DC2,0x7330,0xDFAA,0x869E,0x2A04,0xBB7A,0x17E0,0x4ED4,0xE24E,0xFCBC,0x5026,0x912,0xA588 };

/*
  @description : sends structured packets enough to fit the message (at most 64*10 bits of data)
  @var t : the packet type
  @var dataWidth : the width of the data being transmitted
  @var setFlags : flags to set
  @var dest : address with which to send the message
  @return : 1 if send acknowledged by receiving party, 2 if unable to send message, but was able to close connection, 3 if unable to send message or close connection 
*/

uint8_t sendMessage(char * data , Packet_Type t , uint16_t dataWidth , uint8_t setFlags, uint32_t dest){

  uint16_t dataBytes = 0 ;

  unsigned int i;

  unsigned int packetStart = 0;
  unsigned int dataCursor = 0;
  int packetsSent = 0;
//  unsigned char lastSent[PACKET_SIZE] = {0};
  char msg[PACKET_SIZE * 10] = {0};

  uint16_t crc = 0;

  msg[0] = (myAddress) >> 8;
  msg[1] = ((0xFF) & myAddress);
  msg[2] = (dest) >> 8;
  msg[3] = ((0xFF) & dest);

  msg[FLAGS_START] = setFlags;

  for(i = 0 ; i < dataWidth ; i++){
    
    if(dataBytes == DATA_SIZE){

      //set seqNum
      myLastSeqNum++;
      msg[packetStart + SEQ_START] = (0xFF000000 & myLastSeqNum) >> 24;
      msg[packetStart + SEQ_START + 1] = (0x00FF0000 & myLastSeqNum) >> 16;
      msg[packetStart + SEQ_START + 2] = (0x0000FF00 & myLastSeqNum) >> 8;
      msg[packetStart + SEQ_START + 3] = (0x000000FF & myLastSeqNum);
      //set chksum
      crc = crc16((msg + packetStart),CHKSUM_START);
      msg[packetStart + CHKSUM_START] = ((0xFF00) & crc) >> 8;
      msg[packetStart + CHKSUM_START + 1] = (0x00FF) & crc;

      packetStart = packetsSent++ * (PACKET_SIZE);
      dataCursor = 0;
      msg[packetStart] = (myAddress) >> 8;
      msg[packetStart + 1] = ((0xFF) & myAddress);
      msg[packetStart + 2] = (dest) >> 8;
      msg[packetStart + 3] = ((0xFF) & dest);
      
      msg[packetStart + PACKET_TYPE_START] = t;

      dataBytes = 0;
    }
    msg[packetStart + DATA_START + dataCursor++] = data[i];
    dataBytes++;
  }

  //shift remainng data to the far right of the data field
  for(i = 0 ; i < dataBytes ; i++){
    msg[packetStart + DATA_START + DATA_SIZE - 1 - i] = msg[packetStart + DATA_START + dataBytes - 1 - i];
  }
  for(; i < DATA_SIZE ; i++){
    msg[packetStart + DATA_START + DATA_SIZE - 1 - i] = 0;
  }
  //set seqNum
  myLastSeqNum++;
  msg[packetStart + SEQ_START] = (0xFF000000 & myLastSeqNum) >> 24;
  msg[packetStart + SEQ_START + 1] = (0x00FF0000 & myLastSeqNum) >> 16;
  msg[packetStart + SEQ_START + 2] = (0x0000FF00 & myLastSeqNum) >> 8;
  msg[packetStart + SEQ_START + 3] = (0x000000FF & myLastSeqNum);

  msg[packetStart + PACKET_TYPE_START] = t;
  //set chksum
  crc = crc16((msg + packetStart),CHKSUM_START);
  msg[packetStart + CHKSUM_START] = ((0xFF00) & crc) >> 8;
  msg[packetStart + CHKSUM_START + 1] = (0x00FF) & crc;

  if(recieveInProgress){
    setHigh(led[0]);
    while(recieveInProgress){
      wait( (randomNumber() % 10 ) + 1);
    }
    setLow(led[0]);
  }
  sendUART(3,msg,(PACKET_SIZE) * ++packetsSent);

  return 1;
}

/*
  @description : performs table based crc calculation
  @var *vbuf : string for which to generate crc
  @var len : length of the string to generate checksum
  @return : crc
*/
unsigned short crc16(const void *vbuf , int len)
{
    register int counter;
    register unsigned short crc = 0;
    for( counter = 0; counter < len; counter++){
        crc = (crc<<8) ^ crc16Table[((crc>>8) ^ *(unsigned char *)vbuf++)];
    }
    return crc;
}
/*
  @description : generate a random address and check to see if anyone else claimed this address
  @var n : used as boolean to indicate change of address
  @return : nothing
*/
void getAddress(uint8_t n){
  
  // Assemble the packet we need to send
  int i, idIndex;
  char tmpID[DATA_SIZE] = {0};

  // create and store a random fingerprint in between the start of the data section and the checksum section
  for(i = 0; i < DATA_SIZE; i+=4 ){
    idIndex = (i / 4);
    if(!n)
      fingerprint[idIndex] = randomNumber();
    tmpID[i] = (fingerprint[idIndex] & 0xFF000000) >> 24;
    tmpID[i + 1] = (fingerprint[idIndex] & 0xFF0000) >> 16;
    tmpID[i + 2] = (fingerprint[idIndex] & 0xFF00) >> 8;
    tmpID[i + 3] = fingerprint[idIndex] & 0xFF;
  }

  myAddress = 0b101000000000;
  myAddress |= (randomNumber() % 0xFD) + 2;
    
  wait(randomNumber() & 0xFF);
  // shout my name into the void
  sendMessage(tmpID,IP_CHECK,DATA_SIZE,0,0);
  // we're setting this timer for one byte of data, one stop bit and one start bit
  setTimer(1,(((8 + 2) / (double)lastPort.baud) * 1000),1);
}
/*
  @description : Repeats address
  @var dst : destination address 
  @return : nothing
*/
void sendAddress(uint16_t dst){
  setHigh(led[1]);
  // Assemble the data we need to send
  int i, idIndex;
  char tmpID[DATA_SIZE] = {0};
  for(i = 0; i < DATA_SIZE; i+=4 ){
    idIndex = (i / 4);
    tmpID[i] =  (fingerprint[idIndex] & 0xFF000000) >> 24;
    tmpID[i + 1] = (fingerprint[idIndex] & 0xFF0000) >> 16;
    tmpID[i + 2] = (fingerprint[idIndex] & 0xFF00) >> 8;
    tmpID[i + 3] = fingerprint[idIndex] & 0xFF;
  }
  wait(randomNumber() & 0xFF);
  sendMessage(tmpID,IP_CHECK,DATA_SIZE,0,dst);
  setLow(led[1]);
}

void SERCOM3_IRQHandler(void){
  switch(userSercom->USART.INTFLAG.reg & (SERCOM_USART_INTFLAG_RXS | SERCOM_USART_INTFLAG_RXC | SERCOM_USART_INTFLAG_ERROR)){
    case SERCOM_USART_INTFLAG_RXC | SERCOM_USART_INTFLAG_RXS :{
      userSercom->USART.INTFLAG.bit.RXS = 1;
      // this may pose an issue if we're receiving and we have to parse the data
    }
    case SERCOM_USART_INTFLAG_RXC :{
      // add the byte to our array
      lastReceived[bytesReceived++] = userSercom->USART.DATA.reg;
      // the two important cases are the 0th byte and the last byte
      switch(bytesReceived){
        case 1 : {
          // if the first byte received does not match the first half of this devices address then disregard that byte
          // we do this because we may have entered the line after a transmission started we may not have the complete packet
          if(lastReceived[bytesReceived - 1] != (myAddress >> 8)){
            bytesReceived--;
            break;
          }
          resetTimer(1);
          break;
        }
        case (PACKET_SIZE) : {
          disableUARTInterrupts(3);
          stopTimer(1);
          theirLastSeqNum = 0;
          myLastSeqNum = 0;
          bytesReceived = 0;
          recieveInProgress = 0;
          parsePacket(lastReceived);
          enableUARTInterrupts(3);
          break;
        }
        default : {
          resetTimer(1);
          break;
        }
      }
      break;
    }
    case SERCOM_USART_INTFLAG_RXS :{
      userSercom->USART.INTFLAG.bit.RXS = 1;
      if(!recieveInProgress)
        startTimer(1,1);
      resetTimer(1);
      recieveInProgress = 1;
      break;
    }
    default :{
      disableUARTInterrupts(3);
      stopTimer(1);
      recieveInProgress = 0;
      bytesReceived = 0;
      userSercom->USART.DATA.reg;
      userSercom->USART.INTFLAG.reg = 0xFF;
      enableUARTInterrupts(3);
      break;
    }
  }
}

void TC1_IRQHandler(void){
  stopTimer(1);
  usedSpace = 0;
  recieveInProgress = 0;
  bytesReceived = 0;
  TC1->COUNT16.INTFLAG.bit.MC0 = 1;
}