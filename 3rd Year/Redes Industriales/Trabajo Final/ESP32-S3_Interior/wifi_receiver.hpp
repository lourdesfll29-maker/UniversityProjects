#ifndef WIFI_RECEIVER_H
#define WIFI_RECEIVER_H

#include <Arduino.h>

//-----[ FUNCTION DECLARATIONS ]-----------------------------------------------//
void setupTemperatureWifiReceiver();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void receiveTemperatureData();

#endif // WIFI_RECEIVER_H
