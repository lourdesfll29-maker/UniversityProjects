#include <ESP32-TWAI-CAN.hpp>

// Pines CAN
#define CAN_TX 17
#define CAN_RX 18

CanFrame frame; // Estructura para almacenar el frame recibido

void setup() 
{
    Serial.begin(115200);
    ESP32Can.setPins(CAN_TX, CAN_RX);
    // Configuración de buffers
    ESP32Can.setRxQueueSize(5);
    ESP32Can.setTxQueueSize(5);
    ESP32Can.setSpeed(ESP32Can.convertSpeed(500));

    if (ESP32Can.begin()) 
    {
        Serial.println("CAN bus iniciado correctamente.");
    } else 
    {
        Serial.println("Error al iniciar CAN bus.");
    }
}

void loop() 
{
    // Lee un frame CAN con un timeout de 1000 ms
    if (ESP32Can.readFrame(frame, 1000)) 
    {
        Serial.printf("Received frame: %03X  \r\n", frame.identifier);

        // Si el mensaje es el esperado, se reconstruye el texto
        if (frame.identifier == 0x7DF) 
        {
            Serial.print("Mensaje recibido: ");

            char receivedMessage[5] = {0}; // Buffer para texto
            for (int i = 0; i < 4; i++)
            {
                receivedMessage[i] = static_cast<char>(frame.data[i]);
            }          

            Serial.println(receivedMessage); // Muestra el mensaje como cadena de texto
        }
    }
}