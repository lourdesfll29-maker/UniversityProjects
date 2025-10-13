//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "led_control.hpp"

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/  
/*!  
 * @brief  Initializes LED pins and sets default state to green.
 *
 * Configures red, green, and blue LED pins as outputs. By default,
 * the green LED is turned on to indicate a safe temperature state.
 *
 * @param  void
 * @return void
 */
void setupLED() 
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW); // Default: green on
  digitalWrite(LED_BLUE, HIGH);
}

/******************************************************************************/  
/*!  
 * @brief  Updates LED color based on the current temperature.
 *
 * Sets the LED to:
 * - Red: if temperature exceeds maximum.
 * - Green: if temperature is normal.
 * - Blue: if temperature is invalid/unreadable.
 *
 * @param  void
 * @return void
 */
void updateLED() 
{
  Serial.print("[led_control.cpp INFO] Actualizando LED. Temperatura: ");

  if (temperature == -1) 
  {
    Serial.println("[led_control.cpp INFO] Temperatura inválida. LED en azul.");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
  } 
  else 
  {
    if (temperature > MAX_TEMPERATURE)
    {
    Serial.println("[led_control.cpp INFO] Temperatura alta. LED en rojo.");
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    }
    else
    {
      Serial.println("[led_control.cpp INFO] Temperatura normal. LED en verde.");
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
  }
}
