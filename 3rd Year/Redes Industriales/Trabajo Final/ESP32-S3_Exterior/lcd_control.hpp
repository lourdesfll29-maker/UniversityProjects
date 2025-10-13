#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

#include <Arduino.h>

//-----[ I2C CONFIGURATION ]--------------------------------------------------//
#define SDA 14                // GPIO for I2C SDA line
#define SCL 13                // GPIO for I2C SCL line

//-----[ FUNCTION DECLARATIONS ]----------------------------------------------//
void setupLCD();
void updateLCD();
bool i2CAddressTest(uint8_t address);

#endif // LCD_CONTROL_H
