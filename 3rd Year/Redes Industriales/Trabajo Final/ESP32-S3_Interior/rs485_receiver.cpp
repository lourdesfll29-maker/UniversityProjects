//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <time.h> 

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "rs485_receiver.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
// Initialize software serial for RS485 communication
SoftwareSerial rs485Serial(RS485_RX, RS485_TX);

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Initializes the RS485 software serial communication at 9600 baud.
 *
 * @param  void
 * @return void
 */
void setupRS485Receiver() 
{
    rs485Serial.begin(9600);
}

/******************************************************************************/
/*!
 * @brief  Receives and processes data from the RS485 bus.
 *         Validates the frame structure and updates global variables if the
 *         message is intended for this device.
 *
 * @param  void
 * @return void
 */
void receiveData()
{
    if (rs485Serial.available() > 0) 
    {
        // Variables to store incoming frame
        byte frameData;
        byte frame[8];
        int i_frame = 0;

        // Read up to 8 bytes from the RS485 bus
        while (rs485Serial.available() > 0 && i_frame < 8) 
        {
            frameData = rs485Serial.read();
            frame[i_frame++] = frameData;
            // Small delay to ensure complete frame is received
            delay(3); 
        }

        // Validate the frame header and tail markers
        if (frame[0] == HEAD && frame[6] == TAIL) 
        {
            // Check if the message is addressed to this slave device
            if (frame[1] == SLAVE_TARGET) 
            {
                AMR_id = frame[2];
                objectDetected = (frame[3] == 0x01) ? true:false;

                Serial.print("[rs485_receiver.cpp INFO] Mensaje válido recibido: ");
                Serial.print("AMR_id: ");
                Serial.print(frame[2]);
                Serial.print(" - objectDetected (0x01 o 0x00): ");
                Serial.println(frame[3]);
            }
            else
            {
                Serial.println("[rs485_receiver.cpp INFO] Mensaje no dirigido a este dispositivo.");
            }
        }
        else
        {
            Serial.println("[rs485_receiver.cpp ERROR] Frame no válido.");
        }
    }
}