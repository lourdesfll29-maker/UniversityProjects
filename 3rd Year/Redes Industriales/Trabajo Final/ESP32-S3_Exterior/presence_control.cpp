//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "presence_control.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
// Internal variables for timing
unsigned long lastUltrasonicCheck = 0;
const unsigned long ultrasonicInterval = 5000; // 5 seconds between checks
unsigned long currentTime = 0;

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/  
/*!
 * @brief  Reads the distance from the ultrasonic sensor in centimeters.
 *
 * Sends a trigger pulse and measures the echo pulse duration to compute
 * the distance to the nearest object. Converts duration to distance using
 * the speed of sound.
 *
 * @param  void
 * @return long  Distance in centimeters
 */
long readDistanceCM() 
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

/******************************************************************************/  
/*!
 * @brief  Initializes the ultrasonic sensor pins.
 *
 * Sets the TRIG pin as output and the ECHO pin as input to enable distance
 * measurement via ultrasonic pulses.
 *
 * @param  void
 * @return void
 */
void setupPresenceSensor() 
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

/******************************************************************************/  
/*!
 * @brief  Checks for object presence using ultrasonic sensor.
 *
 * Periodically triggers a distance measurement. If an object is detected
 * within the defined threshold distance, sets `objectDetected` to true;
 * otherwise sets it to false.
 *
 * @param  void
 * @return void
 */
void checkPresence() 
{
    currentTime = millis();
    if (currentTime - lastUltrasonicCheck >= ultrasonicInterval) 
    {
        lastUltrasonicCheck = millis();
        long distance = readDistanceCM();
        Serial.print("Distancia: ");
        Serial.println(distance);

        if (distance > 0 && distance <= UMBRAL_DISTANCE) 
        {
            objectDetected = true;
        }
        else
        {
            objectDetected = false;
        }
    }
}