# Integración de ROS2, MQTT, OPC-UA e Ignition

## Contexto del Proyecto

Este trabajo, realizado para la asignatura "Entornos Operativos para Robótica e Informática Industrial", documenta la creación de una arquitectura de comunicación completa. El objetivo es capturar datos de una simulación robótica en **ROS2**, transmitirlos a través de diferentes protocolos estándar de la industria (**MQTT** y **OPC-UA**), y finalmente visualizarlos en una interfaz HMI/SCADA creada con **Ignition**.

## Arquitectura y Flujo de Datos

El sistema sigue un flujo de datos en varias etapas, conectando diferentes tecnologías desde la simulación hasta la visualización final:

1.  **Origen de Datos (Máquina Virtual - ROS2):**
    * En una máquina virtual VMware con ROS2, se ejecuta un nodo de simulación **`turtlesim`**.
    * Un nodo de Python (`draw_path.py`) publica velocidades aleatorias para que la tortuga se mueva y genere datos dinámicos de posición y velocidad.

2.  **Puente ROS2 a MQTT:**
    * Un nodo-puente personalizado en Python (`my_bridge.py`) se suscribe a los topics de ROS2 (`/turtle1/pose` y `/turtle1/cmd_vel`).
    * Este nodo empaqueta los datos recibidos (posición, orientación, velocidad lineal y angular) en mensajes **JSON** y los publica en un broker MQTT en los topics `turtle/pose` y `turtle/velocity`.

3.  **Servidor OPC-UA (Host):**
    * Un segundo script en Python (`servidor_OPC-UA_Browser.py`), ejecutándose en la máquina anfitriona (Host), actúa como un servidor OPC-UA.
    * Este servidor se suscribe a los topics MQTT, recibe los mensajes JSON, los decodifica y actualiza sus propios nodos OPC-UA.
    * Además de los datos directos, el servidor genera metadatos como contadores de mensajes y los últimos valores recibidos.

4.  **Cliente OPC-UA y SCADA (Ignition):**
    * La plataforma **Ignition** se configura para actuar como un cliente OPC-UA, conectándose al servidor Python.
    * En **Ignition Designer**, se crea una interfaz HMI que lee los datos del servidor OPC-UA en tiempo real para su visualización gráfica.

## Tecnologías y Configuración

* **Máquina Virtual y ROS2:** Se configuró la red de VMware en modo **Bridged (Puente)** para permitir la comunicación directa entre la máquina virtual y la red del anfitrión. Se creó un paquete de ROS2 (`my_turtle`) para alojar los nodos de Python.

* **Comunicación MQTT:** Se utilizó el broker **Mosquitto** en el anfitrión para gestionar los mensajes. La herramienta **MQTTX** se usó para monitorizar y depurar los topics y los mensajes JSON transmitidos.

* **Servidor OPC-UA:** El servidor se estructuró con dos objetos principales: `Pose` y `Velocity`. Cada objeto contiene variables para los valores actuales (posición, orientación, etc.) y un sub-objeto `State` para metadatos como contadores y los últimos valores registrados.

* **Ignition Designer:**
    * Se utilizaron **OPC Tags** para leer los datos en tiempo real desde el servidor OPC-UA.
    * Se crearon **Memory Tags** para realizar cálculos sobre los datos brutos, como velocidades medias, máximas, mínimas y la distancia total recorrida por la tortuga.
    * Se programaron scripts en los **Tag Events** para ejecutar estos cálculos cada vez que un valor cambiaba.

## Estructura de Archivos

El código del proyecto está organizado en las siguientes tres carpetas:

* **`/designer`**: Contiene el backup del proyecto de Ignition Designer, incluyendo la ventana principal (`Main Window`) con todos los componentes visuales y la configuración de los *tags*.
* **`/host`**: Incluye el script de Python (`servidor_OPC-UA_Browser.py`) que se ejecuta en la máquina anfitriona (Windows) y que actúa como el servidor OPC-UA.
* **`/virtual_machine`**: Contiene el paquete de ROS2 (`my_turtle`) con los dos nodos de Python que se ejecutan dentro de la máquina virtual:
    * `draw_path.py`: Para generar el movimiento aleatorio de la tortuga.
    * `my_bridge.py`: El puente que conecta ROS2 con MQTT.

## Visualización (HMI)

La interfaz final en Ignition Designer proporciona una visualización completa de los datos de la tortuga, utilizando los siguientes componentes:

* **Easy Chart:** Para mostrar gráficos históricos de la posición (X e Y) y la orientación (Theta) a lo largo del tiempo.
* **Meter y Moving Analog Indicator:** Para representar las velocidades lineal y angular actuales de forma visual e intuitiva, mostrando también los valores máximos, mínimos y la media calculada.
* **Numeric Label:** Para mostrar todos los valores numéricos, tanto los recibidos directamente como los calculados (contadores, distancia total, etc.).
