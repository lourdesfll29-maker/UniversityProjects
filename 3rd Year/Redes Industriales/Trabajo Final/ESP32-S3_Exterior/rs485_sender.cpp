//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <time.h> 

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "rs485_sender.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
// Initialize SoftwareSerial for RS485 communication
SoftwareSerial rs485Serial(RS485_RX, RS485_TX);

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/ 
/*!
 * @brief  Sends a data frame over RS485.
 *
 * Constructs a frame with header, target ID, AMR ID, presence flag, padding,
 * and tail, and transmits it through the RS485 serial interface.
 *
 * @param  void
 * @return void
 */
void sendFrame()
{
    byte frame[8];
    frame[0] = HEAD;
    frame[1] = SLAVE_TARGET; 
    frame[2] = AMR_id;                          // ID of the detected AMR
    frame[3] = objectDetected ? 0x01 : 0x00;    // Presence flag
    frame[4] = PADDING_BYTE; 
    frame[5] = PADDING_BYTE; 
    frame[6] = TAIL; 
    frame[7] = PADDING_BYTE;                    // Extra padding

    rs485Serial.write(frame, 8);
    rs485Serial.flush();                        // Ensure complete transmission

    Serial.println("[rs485_sender.cpp INFO] Mensaje enviado.");
}

/******************************************************************************/ 
/*!
 * @brief  Initializes RS485 sender module.
 *
 * Starts the software serial interface for RS485 communication.
 *
 * @param  void
 * @return void
 */
void setupRS485Sender() 
{
    Serial.println("[rs485_sender.cpp INFO] Iniciando comunicación RS485...");
    rs485Serial.begin(9600); 
}

/******************************************************************************/ 
/*!
 * @brief  Periodically sends RS485 data frame if presence is detected.
 *
 * This function prints status messages and transmits RS485 frames 
 * every second if object detection is active.
 *
 * @param  void
 * @return void
 */
void publishData() 
{
    Serial.print("[rs485_sender.cpp INFO] Publicando objectDetected: ");
    Serial.println(objectDetected);
    Serial.print("[rs485_sender.cpp INFO] Publicando AMR_id: ");
    Serial.println(AMR_id);

    static uint32_t lastStamp   = 0;
    uint32_t currentStamp       = millis();

    if (currentStamp - lastStamp > 1000) 
    {
        lastStamp = currentStamp;
        sendFrame();
    }
}