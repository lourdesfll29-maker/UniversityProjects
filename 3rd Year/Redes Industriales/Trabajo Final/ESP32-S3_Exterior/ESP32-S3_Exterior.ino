/**
 * @file     ESP32-S3_Exterior.ino
 *
 * @brief    Main program for exterior ESP32-S3 node.
 * 
 * This firmware runs on the exterior module of a cold storage monitoring system.
 * It uses a presence sensor to detect AMRs attempting to access the chamber, receives
 * temperature data via MQTT from the interior module, displays the temperature on an LCD,
 * provides LED-based status indication, and sends AMR presence and temperature data
 * to the interior module over RS485.
 *
 * @authors  
 *    Ángela Espert Cornejo      <aespcor@etsinf.upv.es>
 *    Lourdes Francés Llimerá    <lfralli@epsa.upv.es>
 *
 * @date     May 2025
 */
 
//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"

#include "led_control.hpp"
#include "lcd_control.hpp"
#include "presence_control.hpp"

#include "wifi_receiver.hpp"
#include "rs485_sender.hpp"

//-----[ SETUP FUNCTION ]-----------------------------------------------------//

/******************************************************************************/  
/*!  
 * @brief  Initialization routine called once at boot.
 *
 * Sets up serial communication, initializes RS485, WiFi and MQTT connections,
 * and configures the LED, LCD, and presence sensor.
 *
 * @param  void
 * @return void
 */
void setup() 
{
    Serial.begin(115200);

    setupRS485Sender();
    setupWifiReceiver();

    setupLED();
    setupLCD();
    setupPresenceSensor();
}

//-----[ LOOP FUNCTION ]------------------------------------------------------//

/******************************************************************************/  
/*!  
 * @brief  Main execution loop.
 *
 * Continuously checks for AMR presence, simulates AMR ID incrementation,
 * receives temperature via MQTT, sends AMR data through RS485,
 * and updates visual indicators (LCD and LEDs).
 *
 * @param  void
 * @return void
 */
void loop() 
{    
    checkPresence(); // Update objectDetected flag based on presence sensor
    
    // Simulate AMR ID assignment for testing/demo
    // In a real environment, the AMR would send his ID by WiFi
    if (objectDetected)
    {
      if (AMR_id > 50)
      {
        AMR_id = -1;
      }
      AMR_id += 1;
    }
    
    receiveData();   // Receive temperature from MQTT broker
    publishData();   // Send AMR ID and temperature via RS485

    updateLCD();     // Update LCD display with temperature
    updateLED();     // Update RGB LED based on temperature status

    delay(2000);     // Wait 2 seconds before repeating
}