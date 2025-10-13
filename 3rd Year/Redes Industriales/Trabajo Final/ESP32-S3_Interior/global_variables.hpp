#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <Arduino.h>

//-----[ WIFI SENDER CONFIGURATION ]------------------------------------------//
extern const char* ssid;                    // WiFi network SSID
extern const char* password;                // WiFi password
extern const char* mqttServer;              // MQTT broker IP or hostname
extern const char* mqttTopic_temperature;   // MQTT topic for temperature messages
extern const int   port;                    // MQTT broker port

//-----[ SENSOR DATA ]--------------------------------------------------------//
extern float temperature;                   // Current temperature reading

//-----[ RS485 RECEIVER & DOOR CONTROL ]--------------------------------------//
extern bool objectDetected;                  // Flag indicating if an object is detected
extern int  AMR_id;                          // ID of the detected AMR 

//-----[ DOOR CONTROL & DATABASE ]--------------------------------------------//
extern String currentDate;                   // Current date and time as a String

#endif // GLOBAL_VARIABLES_H
