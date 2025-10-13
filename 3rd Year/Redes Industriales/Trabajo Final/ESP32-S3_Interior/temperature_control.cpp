//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <DHT.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "temperature_control.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
DHT dht(DHTPIN, DHTTYPE); // Create DHT sensor instance

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Initializes the DHT temperature sensor.
 *
 * @param  void
 * @return void
 */
void setupTemperature() 
{
    dht.begin();
}

/******************************************************************************/
/*!
 * @brief  Reads the current temperature from the DHT sensor and stores it in
 *         the global variable `temperature`. If the reading fails, it sets
 *         the temperature to -1 and logs an error.
 *
 * @param  void
 * @return void
 */
void updateTemperature() 
{
    temperature = dht.readTemperature();

    if (isnan(temperature)) 
    {
        Serial.println("[temperature_control.cpp ERROR] Error al leer el sensor.");
        temperature = -1;
    }
}