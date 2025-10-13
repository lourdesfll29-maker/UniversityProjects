//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <Wire.h>                 // I2C communication for the LCD
#include <LiquidCrystal_I2C.h>    // Control the LCD screen

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "lcd_control.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
LiquidCrystal_I2C lcd(0x27, 16, 2);

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/  
/*!  
 * @brief  Initializes the LCD display.
 *
 * Sets up I2C communication and initializes the LCD with default
 * or fallback I2C address if the primary one fails.
 *
 * @param  void
 * @return void
 */
void setupLCD() 
{
    Wire.begin(SDA, SCL);
    if (!i2CAddressTest(0x27)) // fallback if 0x27 fails
    {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2); 
    }
    lcd.init();
    lcd.backlight();
}

/******************************************************************************/  
/*!  
 * @brief  Updates LCD with the current temperature.
 *
 * Displays the temperature if valid, otherwise shows an error message.
 *
 * @param  void
 * @return void
 */
void updateLCD() 
{
    lcd.setCursor(0, 0);

    if (temperature != -1)
    {
        lcd.print("Temperature: ");
        lcd.print(temperature);
        lcd.print(" °C");
    }
    else
    {
        lcd.clear(); // Clean LCD content
        lcd.print("ERROR");
    }
}

/******************************************************************************/  
/*!  
 * @brief  Tests if the given I2C address responds.
 *
 * Used to detect which I2C address the LCD is configured to use.
 *
 * @param  address  I2C address to test.
 * @return true if address responded, false otherwise.
 */
bool i2CAddressTest(uint8_t address) 
{
    // Inicia comunicación I2C
    Wire.beginTransmission(address);
    // Termina la transmisión
    return (Wire.endTransmission() == 0); // Returns 0 if there was no error
}
