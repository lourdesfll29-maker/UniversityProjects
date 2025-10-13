//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
// WiFi credentials
const char* ssid                  = "Livebox6-6587";       // WiFi network SSID
const char* password              = "72kKLstTueD2";        // WiFi network password
// MQTT configuration
const char* mqttServer            = "192.168.1.132";       // IP address of the MQTT broker
const char* mqttTopic_temperature = "sensors/inside/temperature"; // MQTT topic for publishing temperature data
const int   port                  = 1883;                  // MQTT broker port
// Sensor data and system status
float       temperature           = -1;                    // Latest temperature reading
bool        objectDetected        = false;                 // Object detection flag from RS485 sensor
int         AMR_id                = -1;                    // ID of the detected AMR 