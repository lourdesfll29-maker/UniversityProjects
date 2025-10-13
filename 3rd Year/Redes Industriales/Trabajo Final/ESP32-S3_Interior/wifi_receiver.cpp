//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "wifi_receiver.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
WiFiClient espInteriorReceiverClient;
PubSubClient clientReceiver(espInteriorReceiverClient);

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Attempts to reconnect to the MQTT broker if the connection is lost.
 *         Retries until the connection is successfully reestablished.
 *
 * @param  void
 * @return void
 */
void reconnectReceiver() 
{
    while (!clientReceiver.connected()) 
    {
        Serial.print("[wifi_receiver.cpp INFO] Intentando reconectar MQTT...");
        if (clientReceiver.connect("ESP32_Interior_Receiver")) 
        {
            Serial.println("[wifi_receiver.cpp INFO] Reconectado!");
            clientReceiver.subscribe(mqttTopic_temperature);
        } 
        else 
        {
            Serial.print("[wifi_receiver.cpp ERROR] Error, código: ");
            Serial.print(clientReceiver.state());
            delay(2000);
        }
    }
}

/******************************************************************************/
/*!
 * @brief  Establishes a WiFi connection and initializes the MQTT client.
 *         Subscribes to the temperature topic once MQTT is connected.
 *
 * @param  void
 * @return void
 */
void setupTemperatureWifiReceiver() 
{
    WiFi.begin(ssid, password);
    Serial.print("[wifi_receiver.cpp INFO] Conectando a WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[wifi_receiver.cpp INFO] Conectado a WiFi con IP: " + WiFi.localIP().toString());

    // Configure MQTT connection
    clientReceiver.setServer(mqttServer, port);
    clientReceiver.setCallback(mqttCallback);

    while (!clientReceiver.connected()) 
    {
        Serial.print("[wifi_receiver.cpp INFO] Conectando a MQTT...");
        if (clientReceiver.connect("ESP32_Interior_Receiver")) 
        {
            Serial.println("[wifi_receiver.cpp INFO] Conectado a MQTT!");
            clientReceiver.subscribe(mqttTopic_temperature);
            Serial.println("[wifi_receiver.cpp INFO] Suscrito al topic: " + String(mqttTopic_temperature));
        } 
        else 
        {
            Serial.print("[wifi_receiver.cpp ERROR] Error código: ");
            Serial.print(clientReceiver.state());
            delay(2000);
        }
    }
}


/******************************************************************************/
/*!
 * @brief  Callback function executed when a message is received via MQTT.
 *         It deserializes the JSON payload and extracts the temperature value.
 *         Example payload: {"temperature": 24.52}
 *
 * @param  topic    The topic on which the message was received
 * @param  payload  The message content (JSON format)
 * @param  length   The length of the message
 * @return void
 */
void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
    Serial.print("[wifi_receiver.cpp INFO] Mensaje recibido en el topic: ");
    Serial.print(topic);
    Serial.print(" -> ");

    // Convert payload to a null-terminated string
    char data[length + 1];         
    memcpy(data, payload, length); 
    data[length] = '\0';         

    Serial.print("[wifi_receiver.cpp INFO] Contenido JSON recibido: ");
    Serial.println(data);

    JsonDocument json_message;
    DeserializationError error = deserializeJson(json_message, data);
    if (!error) 
    {
        temperature = json_message["temperature"];

        Serial.print("[wifi_receiver.cpp INFO] Recibido: \n");
        Serial.println(data);
    } 
    else 
    {
        Serial.println("[wifi_receiver.cpp ERROR] Error al deserializar JSON");
    }
}

/******************************************************************************/
/*!
 * @brief  Keeps the MQTT connection alive and processes incoming messages.
 *         Reconnects if the connection is lost.
 *
 * @param  void
 * @return void
 */
void receiveTemperatureData()
{
    if (!clientReceiver.connected()) 
    {
        reconnectReceiver();
    }
    clientReceiver.loop(); // Process incoming messages
}