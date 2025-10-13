//-----[ INCLUDES ]-----------------------------------------------------------//
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

//-----[ OWN INCLUDES ]-------------------------------------------------------//
#include "global_variables.hpp"
#include "data_base.hpp"

//-----[ FUNCTIONS ]----------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  Sends AMR data to a local server using an HTTP POST request.
 *
 * This function creates a JSON object containing:
 * - `AMR_ID` (the identifier of the mobile robot),
 * - `Estado_de_la_puerta` (door state, either "Abierto" or "Cerrado"),
 * - `Fecha` (timestamp of the event),
 * 
 * The JSON is serialized into a string and sent to a local PHP server via HTTP POST.
 * The server is assumed to be running on a local network (e.g., using XAMPP).
 *
 * @param  doorState  A string representing the door's current state.
 * @return void
 */
void saveData(String doorState)
{
    JsonDocument jsonData;

    jsonData["AMR_ID"] = AMR_id;
    jsonData["Estado_de_la_puerta"] = doorState;
    jsonData["Fecha"] = currentDate;

    String output;
    serializeJson(jsonData, output);

    Serial.print("[data_base.cpp INFO] Enviando datos al servidor: ");
    Serial.println(output);

    HTTPClient http;
    http.begin("http://192.168.1.16/data.php"); // Local IP address of the server (XAMPP)
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(output);

    if (httpResponseCode > 0) 
    {
        String response = http.getString();
        Serial.println("[data_base.cpp INFO] Respuesta del servidor: " + response);
    } 
    else 
    {
        Serial.println("[data_base.cpp ERROR] Error al enviar. Código de error HTTP: ")
        Serial.print(String(httpResponseCode));
    }

    http.end();
}