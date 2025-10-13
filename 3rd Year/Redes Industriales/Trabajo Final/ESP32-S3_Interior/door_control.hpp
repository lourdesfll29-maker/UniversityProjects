#ifndef DOOR_CONTROL_H
#define DOOR_CONTROL_H

#include <Arduino.h>

//-----[ SERVO CONFIGURATION ]------------------------------------------------//
#define SERVO_PIN   4    // GPIO pin connected to the servo signal line
#define SERVO_CHN   0    // PWM channel for servo control
#define SERVO_FRQ   50   // PWM frequency (50Hz is standard for servos)
#define SERVO_BIT   12   // PWM resolution in bits (12 bits = 4096 levels)

//-----[ DOOR CONTROL PARAMETERS ]--------------------------------------------//
#define OPENED_ANGLE     90     // Servo angle for door open position
#define CLOSED_ANGLE     0      // Servo angle for door closed position
#define MAX_TEMPERATURE  5.0    // Max allowed temperature (°C) to allow door to open

//-----[ ENUMS ]--------------------------------------------------------------//
/**
 * @brief Enum representing the current state of the door.
 */
enum class doorState { OPENED, CLOSED };

//-----[ FUNCTION DECLARATIONS ]----------------------------------------------//
void servoSetAngle(int angle);
void setupDoor();
void doorControl();
String getCurrentDate();

#endif // DOOR_CONTROL_H
