//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "wifi_receiver.hpp"

//-----[ VARIABLES ]----------------------------------------------------------//
WiFiClient espExteriorReceiverClient;
PubSubClient clientReceiver(espExteriorReceiverClient);

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief Reconnects to the MQTT broker if disconnected.
 *
 * Continuously attempts to reconnect to the MQTT broker until successful.
 * Once connected, it resubscribes to the temperature topic.
 *
 * @param void
 * @return void
 */
void reconnectReceiver() 
{
    while (!clientReceiver.connected()) 
    {
        Serial.print("[wifi_receiver.cpp INFO] Intentando reconectar MQTT...");
        if (clientReceiver.connect("ESP32_Exterior_Receiver")) 
        {
            Serial.println("[wifi_receiver.cpp INFO] Reconectado!");
            clientReceiver.subscribe(mqttTopic_temperature); 
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
 * @brief Initializes the WiFi and MQTT connections.
 *
 * Connects the ESP32 to the WiFi network using the credentials in
 * global_variables.hpp. Then, connects to the MQTT broker and subscribes
 * to the temperature topic, setting up the callback function.
 *
 * @param void
 * @return void
 */
void setupWifiReceiver() 
{
    WiFi.begin(ssid, password);
    Serial.print("[wifi_receiver.cpp INFO] Conectando a WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[wifi_receiver.cpp INFO] Conectado a WiFi con IP: " + WiFi.localIP().toString());

    clientReceiver.setServer(mqttServer, port);
    clientReceiver.setCallback(mqttCallback);

    while (!clientReceiver.connected()) 
    {
        Serial.print("[wifi_receiver.cpp INFO] Conectando a MQTT...");
        if (clientReceiver.connect("ESP32_Exterior_Receiver")) 
        {
            Serial.println("[wifi_receiver.cpp INFO] Conectado a MQTT!");
            clientReceiver.subscribe(mqttTopic_temperature);
            Serial.println("[wifi_receiver.cpp INFO] Suscrito al topic: " + String(mqttTopic_temperature));
        } 
        else 
        {
            Serial.print("[wifi_receiver.cpp ERROR] Fallo código: ");
            Serial.print(clientReceiver.state());
            delay(2000);
        }
    }
}

/******************************************************************************/
/*!
 * @brief Callback for incoming MQTT messages.
 *
 * Parses the payload as JSON and updates the global temperature variable.
 * Handles messages with the format: {"temperature": float_value}.
 *
 * @param topic    The topic the message was received on.
 * @param payload  Pointer to the message payload.
 * @param length   Length of the payload.
 * @return void
 */
void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
    Serial.print("[wifi_receiver.cpp INFO] Mensaje recibido en el topic: ");
    Serial.print(topic);
    Serial.print(" -> ");

    // Convertir el payload a string válido (terminado en '\0')
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
 * @brief Processes incoming MQTT messages.
 *
 * Ensures the MQTT client stays connected and processes any new messages.
 *
 * @param void
 * @return void
 */
void receiveData()
{
    if (!clientReceiver.connected()) 
    {
        reconnectReceiver();  
    }
    clientReceiver.loop();     
}