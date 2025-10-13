#include <SoftwareSerial.h>

// Define RS485 pins
#define RS485_TX 17
#define RS485_RX 16

#define HEAD 0xAA
#define TAIL 0xFE

#define SLAVE_TARGET 0x23   // Esclavo al que se envia el comando

// Initialize SoftwareSerial for RS485
SoftwareSerial rs485Serial(RS485_RX, RS485_TX);

void sendFrame() 
{    
    // Define the frame structure
    byte frame[8];
    frame[0] = HEAD;
    frame[1] = SLAVE_TARGET; 
    frame[2] = 0x48; // 'H'
    frame[3] = 0x6f; // 'o'
    frame[4] = 0x6c; // 'l'
    frame[5] = 0x61; // 'a'
    frame[6] = TAIL; 
    frame[7] = 0xAA; // Padding

    rs485Serial.write(frame, 8);
    rs485Serial.flush(); // Ensure all data is sent
    delay(1000); // Small delay to ensure data is sent

    Serial.print("Mensaje enviado\n");
}

void setup() 
{
    Serial.begin(115200); // Debugging serial
    rs485Serial.begin(9600); // RS485 serial
}

void loop() 
{
    static uint32_t lastStamp   = 0;
    uint32_t currentStamp       = millis();

    if (currentStamp - lastStamp > 1000) 
    {
        lastStamp = currentStamp;
        sendFrame();  // Send the frame every second
    }
}