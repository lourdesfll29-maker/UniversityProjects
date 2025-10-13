# Juego 8-Puzzle en Qt/C++

## Descripción del Proyecto

Este proyecto es una implementación de escritorio del clásico juego de lógica **8-Puzzle**, desarrollado en C++ utilizando el framework **Qt 6**. La aplicación presenta un tablero de 3x3 con 8 fichas numeradas y un espacio vacío. El objetivo del jugador es deslizar las fichas para ordenarlas numéricamente del 1 al 8.

La interfaz ha sido diseñada con Qt Designer y estilizada mediante hojas de estilo (QSS), ofreciendo una experiencia de usuario visualmente agradable e interactiva.

## Características Principales

* **Interfaz Gráfica Intuitiva:** La ventana principal muestra el tablero de juego, un contador de pasos (`Steps`) y un botón de "Start/Restart" para comenzar o reiniciar la partida.

* **Mecánica de Arrastrar y Soltar (Drag & Drop):** El jugador interactúa con el juego arrastrando las fichas. El sistema de eventos de Qt (`QDrag`, `QDropEvent`) ha sido implementado para gestionar esta mecánica.

* **Animaciones Fluidas:** Cada movimiento válido de una ficha se visualiza con una animación suave gracias a `QPropertyAnimation`, que desplaza la ficha desde su posición original hasta el espacio vacío.

* **Previsualización de Movimiento:** Durante el arrastre, si el cursor se sitúa sobre el hueco vacío y el movimiento es válido, aparece una "sombra" semitransparente de la ficha para indicar dónde se colocará. Si el movimiento no es válido (la ficha no es adyacente), la sombra aparece en rojo como una advertencia visual.

* **Validación de Movimientos:** La lógica del juego solo permite mover las fichas que son directamente adyacentes al espacio vacío. Si se intenta un movimiento no válido, se muestra un mensaje de advertencia.

* **Generación de Puzzles Resolubles:** Al iniciar o reiniciar el juego, las fichas se barajan de forma aleatoria. El programa incluye una función (`isSolvable()`) que utiliza un algoritmo de conteo de inversiones para garantizar que cada tablero generado tenga siempre una solución.

* **Detección de Victoria:** Después de cada movimiento, el programa comprueba si las fichas están en la configuración final ordenada. Al alcanzarla, se muestra un mensaje de felicitación y se ofrece la opción de volver a jugar.

* **Contador de Movimientos:** La interfaz incluye un contador que se actualiza con cada movimiento válido, permitiendo al jugador llevar un registro de su progreso.

## Tecnologías y Componentes

* **Lenguaje:** C++
* **Framework:** Qt (utilizando el módulo Qt Widgets)
* **Interfaz de Usuario (UI):** Diseñada con **Qt Designer** (`.ui` file) y estilizada mediante **Qt Style Sheets** (QSS).
* **Sistema de Build:** CMake
* **Componentes Clave de Qt:**
    * `QMainWindow`, `QPushButton`, `QGridLayout` para la estructura de la interfaz.
    * `QPropertyAnimation` para las animaciones de las fichas.
    * Sistema de Eventos de Arrastrar y Soltar: `QDrag`, `QDropEvent`, `QMimeData`.

## Estructura de Archivos

* **`mainwindow.h` / `.cpp` / `.ui`**: Definen la clase principal de la ventana, implementan toda la lógica del juego (generación del tablero, validación, animaciones, victoria) y estructuran la interfaz gráfica.
* **`main.cpp`**: Punto de entrada de la aplicación que inicializa y muestra la ventana principal.
* **`estilos/estilos.qss`**: Hoja de estilos externa que define la apariencia de los widgets.
* **`resources.qrc`**: Archivo de recursos de Qt que permite compilar la hoja de estilos dentro del ejecutable.
* **`CMakeLists.txt`**: Script de compilación que gestiona las dependencias de Qt y compila el proyecto.
