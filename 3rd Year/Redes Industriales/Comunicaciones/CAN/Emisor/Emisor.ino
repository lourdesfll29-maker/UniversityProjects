#include <ESP32-TWAI-CAN.hpp>

// Pines de transmisión y recepción CAN
#define CAN_TX 17
#define CAN_RX 18

void sendFrame() 
{    
    CanFrame frame           = {0};
    frame.identifier         = 0x7DF;  // ID estándar del mensaje
    frame.extd               = 0;
    frame.data_length_code   = 8;

    // Datos enviados en el mensaje CAN
    frame.data[0]            = 0x48; // 'H'
    frame.data[1]            = 0x6f; // 'o'
    frame.data[2]            = 0x6c; // 'l'
    frame.data[3]            = 0x61; // 'a'
    frame.data[4]            = 0xAA;
    frame.data[5]            = 0xAA;
    frame.data[6]            = 0xAA;
    frame.data[7]            = 0xAA;

    ESP32Can.writeFrame(frame);
}

void setup() 
{
    Serial.begin(115200);
    ESP32Can.setPins(CAN_TX, CAN_RX);
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
    static uint32_t lastStamp   = 0;
    uint32_t currentStamp       = millis();

    if (currentStamp - lastStamp > 1000) 
    {
        lastStamp = currentStamp;
        sendFrame();  // Envía el mensaje cada segundo
    }
}