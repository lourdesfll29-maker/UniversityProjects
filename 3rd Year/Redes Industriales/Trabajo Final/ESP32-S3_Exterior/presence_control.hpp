#ifndef PRESENCE_CONTROL_H
#define PRESENCE_CONTROL_H

#include <Arduino.h>

//-----[ ULTRASONIC SENSOR CONFIGURATION ]-----------------------------------//
#define TRIG_PIN        4       // GPIO pin for ultrasonic trigger
#define ECHO_PIN        5       // GPIO pin for ultrasonic echo
#define UMBRAL_DISTANCE 15      // Distance threshold (cm) for object detection

//-----[ FUNCTION DECLARATIONS ]---------------------------------------------//
void setupPresenceSensor();
void checkPresence();

#endif // PRESENCE_CONTROL_H
