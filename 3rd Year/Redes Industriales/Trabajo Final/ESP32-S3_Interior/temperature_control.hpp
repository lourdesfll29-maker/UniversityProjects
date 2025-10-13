#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <Arduino.h>

//-----[ SENSOR CONFIGURATION ]-----------------------------------------------//
#define DHTPIN  5       // Sensor pin
#define DHTTYPE DHT11   // Sensor type

//-----[ FUNCTION DECLARATIONS ]----------------------------------------------//
void setupTemperature();   // Initialize temperature sensor
void updateTemperature();  // Read and update temperature value

#endif // TEMPERATURE_CONTROL_H
