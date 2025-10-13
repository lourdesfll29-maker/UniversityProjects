#ifndef RS485_RECEIVER_H
#define RS485_RECEIVER_H

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
void setupRS485Receiver();
void receiveData();

#endif // RS485_RECEIVER_H