#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

//-----[ LED PIN CONFIGURATION ]----------------------------------------------//
#define LED_RED    40    // GPIO pin for red LED
#define LED_GREEN  39    // GPIO pin for green LED
#define LED_BLUE   38    // GPIO pin for blue LED

//-----[ TEMPERATURE THRESHOLD ]----------------------------------------------//
#define MAX_TEMPERATURE  5.0   // Max allowed temperature (°C)

//-----[ FUNCTION DECLARATIONS ]----------------------------------------------//
void setupLED();
void updateLED();

#endif // LED_CONTROL_H
