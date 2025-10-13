# Proyecto de Comunicaciones Industriales (Redes Industriales)

## Contexto del Proyecto

Este proyecto, desarrollado para la asignatura de **Redes Industriales**, se centra en la implementación y el análisis práctico de tres protocolos de comunicación serie fundamentales en el ámbito industrial: **UART**, **RS485** y **CAN**. El objetivo es configurar y programar microcontroladores ESP32 para establecer una comunicación fiable entre dos dispositivos (emisor y receptor) utilizando cada uno de estos estándares, demostrando su funcionamiento y características principales.

## Protocolos Implementados

El proyecto se divide en tres montajes, uno para cada protocolo de comunicación.

### 1. Comunicación UART

* **Definición:** UART (Transmisor-Receptor Asíncrono Universal) es un protocolo de comunicación serie punto a punto que permite el intercambio de datos entre dos dispositivos. Es una de las formas más sencillas de comunicación, utilizando una línea para transmisión (TX) y otra para recepción (RX).

* **Hardware y Montaje:** Se utilizaron dos placas ESP32-S3. La conexión es directa: el pin TX del emisor se conecta al pin RX del receptor, y viceversa. Es crucial conectar también las masas (GND) de ambas placas para asegurar una referencia de voltaje común.

* **Implementación (Código):**
    * **`Emisor.ino`**: Configura un puerto serie (`MySerial`) en los pines 17 (TX) y 18 (RX). En un bucle, envía la cadena "Hola" cada segundo a una velocidad de 9600 baudios.
    * **`Receptor.ino`**: Configura un puerto serie en los mismos pines. Espera a recibir datos y, cuando detecta una línea completa (terminada en `\n`), la lee y la imprime en su monitor serie, mostrando el "Mensaje recibido".

### 2. Comunicación RS485

* **Definición:** RS485 es un estándar de comunicación serie que permite la transmisión de datos a largas distancias y en entornos con mucho ruido eléctrico, gracias a su sistema de señales diferenciales. A diferencia de UART, permite la comunicación entre múltiples dispositivos en una misma red (hasta 32 nodos).

* **Hardware y Montaje:** Se utilizaron dos ESP32 y dos módulos conversores TTL a RS485 (MAX485). Las conexiones se realizan desde los pines serie del ESP32 al módulo conversor, y luego los terminales A y B de ambos módulos se conectan entre sí (A con A, B con B).

* **Implementación (Código):**
    * **`Emisor.ino`**: Utiliza `SoftwareSerial` en los pines 16 (RX) y 17 (TX). Define una trama personalizada con una cabecera (`0xAA`), una dirección de esclavo (`0x23`), los datos ("Hola") y una cola (`0xFE`). Envía esta trama por el bus RS485 cada segundo.
    * **`Receptor.ino`**: Escucha en el bus RS485. Al recibir una trama, verifica que la cabecera y la cola sean correctas. Si lo son, comprueba si el mensaje va dirigido a su dirección de esclavo. En caso afirmativo, extrae los datos y los imprime en el monitor serie.

### 3. Comunicación CAN Bus

* **Definición:** El Bus CAN (Controller Area Network) es un protocolo robusto diseñado principalmente para el sector de la automoción, pero muy extendido en la automatización industrial. Permite la comunicación entre múltiples microcontroladores y dispositivos sin necesidad de un host central, utilizando un sistema de prioridades basado en el identificador del mensaje.

* **Hardware y Montaje:** Se emplearon dos ESP32 y dos módulos transceptores CAN (como el MCP2515). Los pines TX y RX del ESP32 se conectan al transceptor, y los terminales CAN High (H) y CAN Low (L) de ambos módulos se conectan entre sí (H con H, L con L).

* **Implementación (Código):**
    * **`Emisor.ino`**: Utiliza la librería `ESP32-TWAI-CAN` para configurar el bus en los pines 17 (TX) y 18 (RX) a una velocidad de 500 kbps. Periódicamente, construye una trama CAN con el identificador `0x7DF` y los datos correspondientes al mensaje "Hola", y la envía al bus.
    * **`Receptor.ino`**: Configura el bus CAN de la misma manera. En el bucle principal, espera a recibir una trama. Al llegar una, imprime su identificador y, si coincide con `0x7DF`, decodifica los datos para reconstruir y mostrar el mensaje "Hola" en el monitor serie.

## Tecnologías y Componentes

* **Hardware:**
    * Microcontrolador: **ESP32-S3**
    * Módulo RS485: Convertidor TTL a RS485 (basado en **MAX485**)
    * Módulo CAN: Transceptor CAN (basado en **MCP2515**)
* **Software:**
    * Entorno de Desarrollo: **Arduino IDE**
    * Librerías: `HardwareSerial`, `SoftwareSerial`, `ESP32-TWAI-CAN.hpp`

## Archivos del Proyecto

El repositorio está organizado en tres carpetas principales, una para cada protocolo:

* **`/UART`**:
    * `Emisor/Emisor.ino`: Código del dispositivo emisor.
    * `Receptor/Receptor.ino`: Código del dispositivo receptor.
    * `Trabajo Académico - UART.pdf`: Documentación detallada del montaje y protocolo.
* **`/RS485`**:
    * `Emisor/Emisor.ino`: Código del emisor con la lógica de tramas.
    * `Receptor/Receptor.ino`: Código del receptor con la lógica de validación de tramas.
    * `Trabajo Académico - RS485.pdf`: Documentación del montaje y protocolo.
* **`/CAN`**:
    * `Emisor/Emisor.ino`: Código del emisor para el envío de tramas CAN.
    * `Receptor/Receptor.ino`: Código del receptor para la lectura de tramas CAN.
    * `Trabajo Académico - CAN.pdf`: Documentación del montaje y protocolo.
