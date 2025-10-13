#include <SoftwareSerial.h>

// Define RS485 pins
#define RS485_TX 17  // TX pin for RS485
#define RS485_RX 16  // RX pin for RS485

// Initialize SoftwareSerial for RS485
SoftwareSerial rs485Serial(RS485_RX, RS485_TX);

void setup() 
{
    Serial.begin(115200);         // Debugging serial
    rs485Serial.begin(9600);      // RS485 serial
}

void loop() 
{
    if (rs485Serial.available() > 0) 
    {
        // Read incoming data
        byte incomingByte;
        byte frame[8];
        int idx = 0;

        while (rs485Serial.available() > 0 && idx < 8) 
        {
            incomingByte = rs485Serial.read();
            frame[idx++] = incomingByte;
            delay(3); // Small delay to ensure all data is read
        }

        // Check if the received frame is valid
        if (frame[0] == 0xAA && frame[6] == 0xFE) 
        {
            Serial.println("Received valid frame:");

            // Print the received frame in hexadecimal
            for (int i = 0; i < 8; i++) 
            {
                Serial.print(frame[i], HEX);
                Serial.print(" ");
            }
            Serial.println();

            // If the message is the expected one, reconstruct the text
            if (frame[1] == 0x23) 
            {
                Serial.print("Mensaje recibido: ");

                char receivedMessage[5] = {0}; // Buffer for text
                for (int i = 0; i < 4; i++)
                {
                    receivedMessage[i] = static_cast<char>(frame[i + 2]);
                }          

                Serial.println(receivedMessage); // Display the message as a text string
            }
        }
    }
}