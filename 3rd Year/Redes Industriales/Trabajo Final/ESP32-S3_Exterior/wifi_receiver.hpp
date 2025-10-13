#ifndef WIFI_RECEIVER_H
#define WIFI_RECEIVER_H

#include <Arduino.h>

//-----[ FUNCTION DECLARATIONS ]----------------------------------------------//
void setupWifiReceiver();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void receiveData();

#endif // WIFI_RECEIVER_H