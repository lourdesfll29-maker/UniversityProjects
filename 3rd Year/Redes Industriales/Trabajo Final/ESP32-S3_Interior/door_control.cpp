//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Time.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "door_control.hpp"
#include "data_base.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
doorState currentState;

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Sets the servo motor to a specific angle using PWM. Ensures the
 *         angle is within allowed limits.
 *
 * @param  angle  Desired servo angle (degrees).
 * @return void
 */
void servoSetAngle(int angle)
{
    if (angle > OPENED_ANGLE || angle < CLOSED_ANGLE)
    {
        return;
    }

    // Convert angle to PWM duty cycle
    long pwm_value = map(angle, 0, 180, 103, 512); 
    // Send PWM signal to servo
    ledcWrite(SERVO_CHN, pwm_value);              
}

/******************************************************************************/
/*!
 * @brief  Initializes the servo motor control for the door and sets the
 *         initial state to CLOSED.
 *
 * @param  void
 * @return void
 */
void setupDoor() 
{
    ledcSetup(SERVO_CHN, SERVO_FRQ, SERVO_BIT);   // Set up PWM configuration
    ledcAttachPin(SERVO_PIN, SERVO_CHN);          // Attach servo pin to channel

    servoSetAngle(CLOSED_ANGLE);                  // Start with the door closed
    currentState = doorState::CLOSED;
}

/******************************************************************************/
/*!
 * @brief  Main logic for automatic door control based on object detection
 *         and temperature. Controls servo and logs state to database.
 *
 * @param  void
 * @return void
 */
void doorControl() 
{
    if (objectDetected)
    {
        if (temperature == -1)
        {
            Serial.println("[door_control.cpp INFO] Objeto detectado. Fallo al determinar la temperatura.");
        }
        else
        {
            if (temperature > MAX_TEMPERATURE)
            {
                Serial.println("[door_control.cpp INFO] Objeto detectado. Temperatura demasiado elevada.");
                
                if (currentState == doorState::OPENED)
                {
                    Serial.println("[door_control.cpp INFO] Cerrando puerta...");
                    servoSetAngle(CLOSED_ANGLE);  // Close door
                    currentState = doorState::CLOSED;
                    delay(2000);  // Simulate delay for door action
                }
            }
            else // Temperature is within allowed range
            {
                Serial.println("[door_control.cpp INFO] Objeto detectado y temperatura adecuada.");

                if (currentState == doorState::CLOSED)
                {
                    Serial.println("[door_control.cpp INFO] Abriendo puerta...");
                    servoSetAngle(OPENED_ANGLE);  // Open door
                    currentState = doorState::OPENED;
                    delay(2000);  // Simulate delay for door action
                }
            }
        }
    }
    else 
    {
        Serial.println("[door_control.cpp INFO] No se detecta objeto.");

        if (currentState == doorState::OPENED)
        {
            Serial.println("[door_control.cpp INFO] Cerrando puerta...");
            servoSetAngle(CLOSED_ANGLE);  // Close door
            currentState = doorState::CLOSED;
            delay(2000);  // Simulate delay for door action
        }
    }

    // Update current timestamp and save AMR data
    currentDate = getCurrentDate(); // Obtener la hora actual
    String state = (currentState == doorState::OPENED ? "Abierto" : "Cerrado");

    Serial.println("[door_control.cpp INFO] Guardando información en la base de datos...");
    saveData(state);
}

/******************************************************************************/
/*!
 * @brief  Returns the current date and time formatted as a string.
 *
 * @param  void
 * @return String  Current date and time, or error message if unavailable.
 */
String getCurrentDate()
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) 
    {
        char date[1000];
        sprintf(date, "%02d-%02d-%04d %02d:%02d:%02d",
                timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900,
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        return String(date);
    }
    return "Error obteniendo el tiempo";
}