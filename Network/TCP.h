#ifndef _UTCP_
#define _UTCP_
#include <stdint.h>
// CRC Algorithm borrowed from Georges Menie (www.menie.org). License follows
/*	
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define BUF_SIZE 255

// field widths are in bytes
#define SRC_SIZE 2
#define DST_SIZE 2
#define SEQNUM_SIZE 4
#define ACKNUM_SIZE 4
#define PACKET_TYPE_SIZE 1 
#define FLAG_SIZE 1
#define WINDOW_SIZE 2
#define DATA_SIZE 12
#define CHKSUM_SIZE 2
#define PACKET_SIZE CHKSUM_SIZE+DATA_SIZE+WINDOW_SIZE+FLAG_SIZE+PACKET_TYPE_SIZE+ACKNUM_SIZE+SEQNUM_SIZE+DST_SIZE+SRC_SIZE

// byte order
#define DATA_START 16
#define CHKSUM_START 28
#define FLAGS_START 13
#define SEQ_START 4
#define ACKNUM_START 8
#define WIN_START 14
#define PACKET_TYPE_START 12
#define DST_START 2
#define WINDOW_START 14
#define SRC_START 0

extern uint16_t usedSpace;
extern volatile uint8_t receivedMsg[BUF_SIZE];
extern const uint16_t crc16Table[256];

extern struct SERCOMPort lastPort;

extern uint16_t openChatAddr;
extern uint16_t myAddress;
extern uint8_t recieveInProgress;
extern uint8_t messageUnread;
extern uint8_t collision;
extern uint8_t sendActive;

extern uint16_t myAddress;
extern uint32_t theirLastSeqNum;

extern uint16_t expectedPacketNum;

extern uint8_t packetSendTime;

enum TCPFLAGS{URG = 0b00100000,ACK = 0b00010000,PSH = 0b00001000,RST = 0b00000100,SYN = 0b00000010,FIN = 0b00000001};
typedef enum {STD_MODE = 0,IP_CHECK = 1}Packet_Type;

uint8_t closeChat(uint32_t); // @param : dest address
uint8_t sendMessage(char *, Packet_Type , uint16_t , uint8_t , uint32_t); // @param : (string to send , flags , dest address)
void getAddress(uint8_t); // @param : boolean var to indicate change of address
void sendAddress(uint16_t); // @param : address to which we send our address
void parsePacket(uint8_t *); // @param : packet data

/*
typedef struct Packet{

  uint16_t src; // 0 1
  uint16_t dest;  // 2 3
  // 4 5 6 7
  uint32_t seqNum; // packet final location in final order
  // 8 9 10 11
  uint32_t ackNum; // number of bytes acknowledged
  // 12
  unsigned int packetType;
  // 13
  unsigned int fin : 1;
  unsigned int syn : 1;
  unsigned int rst : 1;
  unsigned int psh : 1;
  unsigned int ack : 1;
  unsigned int urg : 1;

  // 14 15
  uint16_t window; // specifies size of buffer space available for incoming data
  // 16 17 18 19
  uint32_t data1;
  // 20 21 22 23
  uint32_t data2;
  // 24 25 26 27
  uint32_t data3;
  // 28 29
  uint16_t checksum;

};
*/
#endif