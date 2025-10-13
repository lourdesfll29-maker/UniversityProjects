#include <HardwareSerial.h>

HardwareSerial MySerial(1); // Definir un Serial para UART
const int MySerialRX = 18;
const int MySerialTX = 17;

void setup() 
{ 
  Serial.begin(9600);  

  // Inicializar el Serial con los pines
  MySerial.begin(9600, SERIAL_8N1, MySerialRX, MySerialTX);
} 
 
void loop() 
{  
  if (MySerial.available()) 
  {
    String message = MySerial.readStringUntil('\n');  // Lee hasta salto de línea
    
    Serial.println("Mensaje recibido: " + message); // Imprime lo recibido
  }
}