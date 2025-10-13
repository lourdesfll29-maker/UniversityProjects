# Proyecto Final: Sistema de Monitorización de Cámara Frigorífica

## Contexto del Proyecto

Este proyecto, desarrollado como trabajo final para la asignatura de **Redes Industriales**, consiste en el diseño e implementación de un sistema de monitorización y control para una cámara frigorífica. El sistema utiliza una arquitectura distribuida con dos microcontroladores ESP32 que se comunican a través de múltiples protocolos industriales y de IoT (WiFi, MQTT, RS485) para gestionar el acceso a la cámara y supervisar su temperatura.

## Arquitectura del Sistema

El sistema se compone de dos módulos principales que interactúan entre sí:

1.  **Módulo Exterior (`ESP32-S3_Exterior.ino`):**
    * **Función:** Actúa como punto de control de acceso y monitorización externa.
    * **Periféricos:**
        * Un **sensor de ultrasonidos** para detectar la presencia de un AMR (Robot Móvil Autónomo) que solicita acceso.
        * Una **pantalla LCD I2C** para mostrar la temperatura actual del interior de la cámara.
        * Un **LED RGB** que actúa como indicador de estado visual (verde para temperatura normal, rojo para alta y azul para error).
    * **Comunicaciones:**
        * Se conecta a la red **WiFi** para actuar como cliente **MQTT** y suscribirse al topic de temperatura.
        * Utiliza un bus **RS485** para enviar al módulo interior la información sobre la presencia de un AMR.

2.  **Módulo Interior (`ESP32-S3_Interior.ino`):**
    * **Función:** Controla el entorno dentro de la cámara frigorífica.
    * **Periféricos:**
        * Un **sensor de temperatura y humedad DHT11** para medir las condiciones ambientales internas.
        * Un **servomotor** para controlar la apertura y cierre de la puerta de la cámara.
    * **Comunicaciones:**
        * Actúa como **publisher MQTT** a través de WiFi, enviando periódicamente las lecturas de temperatura.
        * Escucha en el bus **RS485** para recibir los mensajes de presencia de AMR desde el módulo exterior.
        * Realiza peticiones **HTTP POST** a un servidor local para registrar todos los eventos en una base de datos.

## Flujo de Datos y Funcionamiento

El sistema opera siguiendo una lógica de eventos coordinada:

1.  El **módulo interior** lee la temperatura del sensor DHT11 y la publica en un topic MQTT (`sensors/inside/temperature`).
2.  El **módulo exterior**, suscrito a dicho topic, recibe la temperatura y la muestra en la pantalla LCD.
3.  El LED RGB del módulo exterior cambia de color según la temperatura recibida (verde si es correcta, rojo si supera el umbral de `MAX_TEMPERATURE`).
4.  Cuando un AMR se acerca, el sensor de ultrasonidos del **módulo exterior** lo detecta.
5.  El módulo exterior envía una trama por el bus **RS485** al módulo interior, informando de la presencia y el ID del AMR.
6.  El **módulo interior** recibe el mensaje RS485. Si la temperatura es segura (no supera `MAX_TEMPERATURE`), acciona el servomotor para **abrir la puerta**.
7.  Una vez que el AMR se retira y deja de ser detectado, el módulo exterior informa al interior (vía RS485), y este procede a **cerrar la puerta**.
8.  Cada vez que la puerta se abre o se cierra, el **módulo interior** registra el evento (ID del AMR, estado de la puerta y timestamp) enviando una petición HTTP POST a un servidor local, que lo almacena en una base de datos y en un fichero de texto.

## Tecnologías y Componentes

* **Hardware:**
    * Microcontrolador: **ESP32-S3** (x2)
    * Sensores: DHT11 (temperatura y humedad), HC-SR04 (ultrasonidos).
    * Actuadores: Servomotor SG90, LED RGB.
    * Visualización: Pantalla LCD 16x2 con interfaz I2C.
    * Comunicaciones: Módulos conversores TTL a RS485 (MAX485).

* **Software y Protocolos:**
    * Entorno de Desarrollo: **Arduino IDE**.
    * Protocolos: **WiFi**, **MQTT** (para telemetría), **RS485** (para control local), **HTTP** (para registro de datos).
    * Simulación: Un script de Python (`temperature_simulator.py`) se utiliza para simular el sensor de temperatura y publicar datos en el broker MQTT, facilitando las pruebas.

## Estructura de Archivos del Proyecto

El proyecto está organizado de forma modular para facilitar su comprensión y mantenimiento:

* **`ESP32-S3_Exterior.ino` / `ESP32-S3_Interior.ino`**: Archivos principales que contienen la lógica `setup()` y `loop()` para cada microcontrolador.
* **`global_variables.hpp/.cpp`**: Definen y declaran variables globales compartidas entre los diferentes módulos de cada programa (credenciales WiFi, topics MQTT, pines, etc.).
* **Módulos de Control (`*_control.hpp/.cpp`):**
    * `led_control`: Gestiona el LED RGB.
    * `lcd_control`: Controla la pantalla LCD.
    * `presence_control`: Maneja el sensor de ultrasonidos.
    * `door_control`: Acciona el servomotor de la puerta.
    * `temperature_control`: Lee el sensor de temperatura DHT11.
* **Módulos de Comunicación (`wifi_*.hpp/.cpp`, `rs485_*.hpp/.cpp`):**
    * `wifi_receiver` / `wifi_sender`: Gestionan la conexión WiFi y la comunicación MQTT (suscripción y publicación).
    * `rs485_receiver` / `rs485_sender`: Implementan la comunicación por el bus RS485.
* **`data_base.hpp/.cpp`**: Contiene la función para enviar los registros de eventos al servidor web mediante HTTP POST.
* **`temperature_simulator.py`**: Script de Python para simular el envío de datos de temperatura a través de MQTT.
* **`Trabajo Académico - Trabajo Final.pdf`**: Documentación exhaustiva del proyecto.
