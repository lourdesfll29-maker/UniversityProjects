#include <HardwareSerial.h>

HardwareSerial MySerial(1); // Definir un Serial para UART
const int MySerialRX = 18;
const int MySerialTX = 17;

void setup() 
{ 
  Serial.begin(9600);

  // Inicializar el Serial con los pines para el RS485
  MySerial.begin(9600, SERIAL_8N1, MySerialRX, MySerialTX);
} 
 
void loop() 
{ 
  Serial.write("Enviando mensaje...\n");

  String message = "Hola";

  MySerial.println(message); // Envío del mensaje como texto
  delay(1000);               // Espera 1 segundo antes de enviar de nuevo
} 