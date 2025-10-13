# Proyecto de Visión 3D: Sistema de Picking Robótico por Gestos ("Pick by Count")

## Contexto del Proyecto

Este proyecto, desarrollado para la asignatura de **Visión 3D**, consiste en la creación de un sistema avanzado que integra visión estéreo, reconocimiento de gestos y control robótico en un entorno de simulación. El objetivo es permitir que un usuario controle un brazo robótico virtual mediante gestos con la mano, indicando cuántos objetos debe recoger (`pick`) de una escena.

El sistema combina la captura y procesamiento de imágenes del mundo real para guiar las acciones de un robot en un gemelo digital simulado en **RoboDK**.

## Arquitectura y Flujo de Trabajo

El sistema funciona siguiendo una secuencia de pasos que van desde la calibración de las cámaras hasta la ejecución de la tarea robótica:

1.  **Calibración del Sistema Estéreo:** Se utiliza un par de cámaras IP (simuladas con DroidCam) para crear un sistema de visión estéreo. El primer paso es calibrar ambas cámaras (individual y estéreo) con un patrón de tablero de ajedrez para obtener sus parámetros intrínsecos, extrínsecos y corregir la distorsión.

2.  **Detección y Seguimiento de Objetos:**
    * **Mano:** Se utiliza la librería `MediaPipe` para detectar los puntos clave (landmarks) de una mano en la imagen. El sistema cuenta el número de dedos levantados para determinar la cantidad de objetos a recoger.
    * **Objetivo (Target):** El objeto que el robot debe recoger se detecta mediante segmentación por color en el espacio HSV.

3.  **Cálculo de Coordenadas 3D (Triangulación):**
    * Una vez que la mano (o el objeto) es detectado en las imágenes de ambas cámaras, se aplica el proceso de **triangulación estéreo**.
    * Utilizando los parámetros de calibración, el sistema calcula las coordenadas 3D `(X, Y, Z)` reales tanto de la mano como del objeto objetivo.

4.  **Comunicación con el Entorno Virtual (MQTT):**
    * Las coordenadas 3D del objeto detectado y la cantidad de piezas a recoger (obtenida del gesto de la mano) se empaquetan en un mensaje **JSON**.
    * Este mensaje se publica en un topic del broker **MQTT** `broker.emqx.io`.

5.  **Control del Robot en RoboDK:**
    * Un script de Python dentro de **RoboDK** se suscribe al topic MQTT.
    * Al recibir un mensaje, el script decodifica las coordenadas y la cantidad, y traduce esta información en una secuencia de movimientos para el brazo robótico virtual (`UR3e`).
    * El robot ejecuta una tarea de *pick and place*, recogiendo la cantidad indicada de objetos desde la posición calculada y depositándolos en un lugar predefinido.

## Tecnologías y Herramientas

* **Lenguaje:** Python, utilizando Jupyter Notebooks para el desarrollo y las pruebas.
* **Librerías Principales de Visión:**
    * **OpenCV:** Para la calibración de cámaras, procesamiento de imágenes y triangulación.
    * **MediaPipe:** Para el reconocimiento de manos (Hand Tracking) y la detección de gestos.
    * **NumPy:** Para cálculos matriciales y manejo de datos.
* **Simulación Robótica:**
    * **RoboDK:** Para crear el gemelo digital de la célula robótica y simular las tareas de *pick and place*.
    * **API de RoboDK:** Para controlar el robot virtual mediante scripts de Python.
* **Comunicación:**
    * **Paho-MQTT:** Librería de Python para la comunicación a través del protocolo MQTT.
* **Hardware (simulado):**
    * Se utilizan dos smartphones con **DroidCam** para actuar como un par de cámaras IP estéreo.

## Archivos del Proyecto

* **`pick_by_count-memoria.pdf`**: Documento de memoria que describe en detalle la motivación, la solución implementada y los resultados del proyecto.
* **`img_adq_individual.ipynb` / `img_adq_stereo.ipynb`**: Notebooks de Jupyter para la adquisición de imágenes de calibración de las cámaras.
* **`main_program.ipynb`**: Notebook principal que contiene el código completo para la detección, el cálculo 3D y la comunicación MQTT.
* **`Main_Program.py`**: Script de Python para ser ejecutado dentro de RoboDK, que actúa como cliente MQTT y controla el robot.
* **`virtual_scene.rdk`**: Archivo de la estación de RoboDK que contiene la escena virtual con el brazo robótico, la herramienta y los objetos.
