# Proyecto de Visión 3D: Minijuego de Realidad Aumentada "Get the Coins"

## Contexto del Proyecto

Este proyecto, desarrollado para la asignatura de **Visión 3D**, consiste en la creación de un minijuego interactivo de **Realidad Aumentada (RA)** llamado "Get the Coins". El objetivo es combinar técnicas de visión por computador para detectar y seguir objetos en el mundo real, y utilizar esa información para controlar un avatar en una escena virtual, creando una experiencia inmersiva que une ambos mundos.

## Descripción del Minijuego

El juego "Get the Coins" consiste en recoger la mayor cantidad de monedas posible en un tiempo limitado. La interacción se realiza de la siguiente manera:

* **Entorno Real:** Un tablero físico (un folio) delimitado por marcadores ArUco. Sobre este tablero se mueve un "personaje", que es un prisma pentagonal de madera con un imán en su interior. El jugador controla este prisma moviendo otro imán por debajo del tablero.
* **Entorno Virtual:** Una cámara captura la escena real y, mediante técnicas de visión 3D, superpone un entorno de juego virtual. El prisma de madera se convierte en un avatar digital, y las monedas aparecen virtualmente en el tablero. El objetivo es mover el avatar para que "toque" y recoja las monedas virtuales.

## Arquitectura y Técnicas Implementadas

La solución integra varios componentes clave de la visión por computador para lograr la experiencia de Realidad Aumentada:

1.  **Calibración de la Cámara:**
    * Se realiza un proceso de calibración utilizando un patrón de tablero de ajedrez.
    * El objetivo es obtener los **parámetros intrínsecos** de la cámara (distancia focal, punto principal) y los **coeficientes de distorsión**.
    * Este paso es fundamental para corregir las aberraciones de la lente y poder realizar cálculos 3D precisos a partir de una imagen 2D.

2.  **Detección de Marcadores ArUco:**
    * El área de juego está definida por marcadores **ArUco** colocados en las esquinas del tablero.
    * El sistema detecta estos marcadores en cada fotograma para determinar la **posición y orientación (pose)** del tablero en el espacio 3D. Esto permite anclar la escena virtual al tablero real de forma estable.

3.  **Tracking del Objeto (Seguimiento):**
    * El personaje principal (el prisma pentagonal) se detecta mediante su **color**.
    * Se realiza una segmentación de color en el espacio de vídeo **HSV** para aislar el objeto del resto de la escena.
    * Una vez aislado, se calcula su centroide, que se utiliza como la posición 2D del personaje en la imagen.

4.  **Realidad Aumentada y Escena Virtual:**
    * Utilizando la pose del tablero obtenida de los marcadores ArUco, se proyecta una **escena virtual** sobre la imagen de la cámara.
    * Esta escena virtual incluye el avatar del personaje (que sigue el movimiento del prisma de madera), las monedas que aparecen en posiciones aleatorias y la interfaz del juego (puntuación y tiempo restante).
    * Se realiza una **transformación de perspectiva** para mapear la posición 2D del objeto (obtenida por color) a su posición correspondiente en el plano 3D del juego.

## Tecnologías y Herramientas

* **Lenguaje:** Python
* **Librerías Principales:**
    * **OpenCV:** Para todas las tareas de visión por computador (calibración, detección de ArUco, segmentación de color, transformaciones de perspectiva).
    * **NumPy:** Para el manejo eficiente de matrices y cálculos numéricos.

## Archivos del Proyecto

* **`get_the_coins-memoria.pdf`**: Documento de memoria que describe en detalle el problema, la solución implementada, las técnicas utilizadas y el resultado final del proyecto.
