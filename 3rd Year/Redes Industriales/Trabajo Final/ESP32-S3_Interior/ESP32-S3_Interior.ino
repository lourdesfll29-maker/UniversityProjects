/**
 * @file     ESP32-S3_Interior.ino
 *
 * @brief    Main program for interior ESP32-S3 node.
 * 
 * This firmware coordinates communication between the RS485 sensor (object detection),
 * temperature sensor, and WiFi modules to control a servo-driven door based on logic
 * combining temperature and object detection. It also publishes temperature over MQTT 
 * and logs AMR data to a server via HTTP.
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
#include "temperature_control.hpp" // Comment if using temperature.py
#include "door_control.hpp"
#include "rs485_receiver.hpp"
#include "wifi_sender.hpp" // Comment if using temperature.py
// #include "wifi_receiver.hpp" // For optional use with temperature.py
#include "data_base.hpp"

//-----[ SETUP FUNCTION ]-----------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Initialization routine called once at boot.
 *
 * Sets up serial communication, connects to WiFi and MQTT broker,
 * syncs the time with NTP servers, and initializes temperature sensors,
 * RS485 communication, and servo motor.
 *
 * @param  void
 * @return void
 */
void setup() 
{
    Serial.begin(115200);

    setupWifiSender();       // Connect to WiFi and MQTT broker (comment if using temperature.py)
    setupTime();             // Synchronize time using NTP
    setupRS485Receiver();    // Initialize RS485 communication
    // setupTemperatureWifiReceiver(); // Uncomment if using temperature.py

    setupTemperature();      // Initialize local temperature sensor (comment if using temperature.py)
    setupDoor();             // Initialize servo door system
}

//-----[ MAIN LOOP ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Main control loop executed repeatedly.
 *
 * Reads sensor data (temperature, object presence), updates system state,
 * controls the door, publishes MQTT messages, and sends log data to the server.
 *
 * @param  void
 * @return void
 */
void loop() 
{
    maintainSender();        // Keep MQTT connection alive (comment if using temperature.py)

    updateTemperature();     // Update local temperature reading (comment if using temperature.py)

    if (temperature != -1)  // Check if the reading is valid
    {
        Serial.print("Temperatura actual: ");
        Serial.print(temperature);
        Serial.println(" °C");           
    } 

    publishTemperature();    // Publish temperature to MQTT broker (comment if using temperature.py)
    // receiveTemperatureData(); // Uncomment to use temperature.py
    receiveData();           // Receive RS485 frame with AMR ID + detection

    doorControl();           // Control door based on sensor values
    
    delay(2000);             // Wait before next iteration
}

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Sets and synchronizes system time via NTP servers.
 *
 * Connects to public NTP servers and configures the device's internal clock.
 * Prints the current time once successfully synchronized.
 *
 * @param  void
 * @return void
 */
void setupTime()
{
    // UTC time zone
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.println("[ESP32-S3_Interior.ino INFO] Sincronizando hora con NTP...");

    struct tm timeinfo;
    unsigned long startTime = millis();
    const unsigned long timeout = 10000; // 10 seconds

    while (!getLocalTime(&timeinfo) && millis() - startTime < timeout) 
    {
        Serial.print(".");
        delay(500);
    }

    if (getLocalTime(&timeinfo)) 
    {
        Serial.println("\n[ESP32-S3_Interior.ino INFO] Hora sincronizada correctamente");
        char timeString[30];
        strftime(timeString, sizeof(timeString), "%d-%m-%Y %H:%M:%S", &timeinfo);
        Serial.print("🕒 Hora actual: ");
        Serial.println(timeString);
    } 
    else 
    {
        Serial.println("\n[ESP32-S3_Interior.ino ERROR] ERROR: no se pudo sincronizar la hora");
    }
}