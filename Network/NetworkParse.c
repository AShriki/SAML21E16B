#include "Network/TCP.h"
#include "Drivers/userTC.h"
#include "Drivers/userRNG.h"
#include "Drivers/userPort.h"
#include "Initialization/userInit.h"

uint16_t expectedPacketNum;

/*
  @description  : perform a task dictated by the packet
  @var packet : the packet to be handled
  @return : Nothing 
*/
void localCommands(uint8_t *packet){
  switch(packet[DATA_START]){
    case 0 : {
        if(packet[DATA_START + 1] == 0)
          setLow((packet[DATA_START + 2] < 4) ? led[packet[DATA_START + 2]] : led[0]);
        else if(packet[DATA_START + 1] == 1)
          setHigh((packet[DATA_START + 2] < 4) ? led[packet[DATA_START + 2]] : led[0]);
        else if(packet[DATA_START + 1] == 2)
          setToggle((packet[DATA_START + 2] < 4) ? led[packet[DATA_START + 2]] : led[0]);
      break;
    }
  }
}

/*
  @description  : check a packet for some basic conditions that require our attention such as address collision and IPCheck. If all is well we interpret the packet data
  @var packet : the packet to be handled
  @return : Nothing 
*/
void parsePacket(uint8_t *packet){
    uint16_t dst = packet[DST_START] * 0x100 + packet[DST_START + 1];
    // if the source of the transmission is my address then an address has been duplicated. do the following
    if((packet[SRC_START] * 0x100 + packet[SRC_START + 1]) == myAddress){
      // get a new address
      getAddress(1);
    }
    // if the packet is type 1, has a destination within the subnet of 0 and is in my subnet do the following
    else if(packet[PACKET_TYPE_START] == 1){
      if((packet[DST_START] == (myAddress / 0x100)) && (packet[DST_START + 1] == 0)){
        // transmit my address
        sendAddress(packet[SRC_START] * 0x100 + packet[SRC_START + 1]);
      }
    }
    // if we are the intended recipient then handle the packet
    else if(dst == myAddress){
      localCommands(packet);
      // send ack
      sendMessage("",STD_MODE,0,ACK,packet[SRC_START] * 0x100 + packet[SRC_START + 1]);
    }
}

