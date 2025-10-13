//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h> 

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "wifi_sender.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
WiFiClient espInteriorSenderClient;
PubSubClient clientSender(espInteriorSenderClient);

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Ensures that the MQTT client remains connected. If the connection
 *         is lost, attempts to reconnect to the MQTT broker.
 *
 * @param  void
 * @return void
 */
void maintainSender()
{
    if (!clientSender.connected())
    {
        Serial.print("[wifi_sender.cpp INFO] Intentando reconectar MQTT...");
        if (clientSender.connect("ESP32_Interior_Sender")) 
        {
            Serial.println("[wifi_sender.cpp INFO] Reconectado!");
        } 
        else 
        {
            Serial.print("[wifi_sender.cpp ERROR] Error código: ");
            Serial.print(clientSender.state());
            delay(2000);
        }
    }

    clientSender.loop(); // Keep MQTT client alive and handle messages
}

/******************************************************************************/
/*!
 * @brief  Connects the ESP32 to the WiFi network and establishes a connection
 *         to the MQTT broker. Also logs local and broker IP information.
 *
 * @param  void
 * @return void
 */
void setupWifiSender() 
{
    Serial.print("[wifi_sender.cpp INFO] Conectando a WiFi...");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[wifi_sender.cpp INFO] Conectado al WiFi");

    // Set up MQTT server connection
    clientSender.setServer(mqttServer, port);
    while (!clientSender.connected()) 
    {
        if (clientSender.connect("ESP32_Interior_Sender")) 
        {
            Serial.println("[wifi_sender.cpp INFO] Conectado a MQTT");
        } 
        else 
        {
            delay(2000); // Wait and retry
        }
    }

    // Display IP address info
    Serial.print("[wifi_sender.cpp INFO] IP local: ");
    Serial.println(WiFi.localIP());

    Serial.print("[wifi_sender.cpp INFO] Broker MQTT IP (esperada): ");
    Serial.println(mqttServer);
}

/******************************************************************************/
/*!
 * @brief  Publishes the current temperature to the configured MQTT topic.
 *         The temperature is sent as a JSON-formatted message.
 *
 * @param  void
 * @return void
 */
void publishTemperature() 
{
    JsonDocument json_message;

    // Add temperature to JSON message
    json_message["temperature"] = temperature;
    
    char buffer[100];
    // Serialize to JSON string
    serializeJson(json_message, buffer);
    // Publish to MQTT topic
    clientSender.publish(mqttTopic_temperature, buffer);

    Serial.println("[wifi_sender.cpp INFO] Temperatura publicada: " + String(buffer));
}