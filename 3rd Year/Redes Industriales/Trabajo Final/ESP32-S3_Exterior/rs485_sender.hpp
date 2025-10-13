#ifndef RS485_SENDER_H
#define RS485_SENDER_H

#include <Arduino.h>

//-----[ RS485 PIN CONFIGURATION ]--------------------------------------------//
#define RS485_TX 17
#define RS485_RX 16

//-----[ FRAME CONSTANTS ]----------------------------------------------------//
#define HEAD          0x7E
#define TAIL          0xFE
#define PADDING_BYTE  0xAA

#define SLAVE_TARGET  0x23   // Target slave address for commands

//-----[ FUNCTION DECLARATIONS ]----------------------------------------------//
void sendFrame();
void setupRS485Sender() ;
void publishData();

#endif // RS485_SENDER_H
