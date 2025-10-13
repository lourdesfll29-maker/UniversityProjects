# Juego Conecta 4 en Qt/C++

## Descripción del Proyecto

Este proyecto es una implementación completa del clásico juego **Conecta 4**, desarrollado en C++ con el framework **Qt** para la asignatura de Interacción Hombre-Máquina (IHM). Se trata de una aplicación de escritorio con una arquitectura multi-ventana que va más allá del simple juego, incorporando un robusto sistema de perfiles de usuario, historial de partidas, rankings y amplias opciones de personalización.

La aplicación está diseñada de forma modular, con una ventana principal (`MainWindow`) que actúa como un controlador central, gestionando la navegación fluida entre las diferentes pantallas (widgets).

## Características Principales

* **Sistema de Múltiples Ventanas:** La aplicación cuenta con una navegación intuitiva entre diversas pantallas:
    * **Menú Principal:** Pantalla de bienvenida con opciones para iniciar una nueva partida, ver el "Acerca de", acceder a opciones o salir.
    * **Registro e Inicio de Sesión:** Un completo sistema para crear perfiles de usuario y acceder a ellos. Incluye validación de datos para garantizar la integridad de la información y un sistema de recuperación de contraseña.
    * **Configuración de Partida:** Permite a los jugadores seleccionar sus nombres (iniciando sesión o como invitado) y el color de sus fichas entre varias opciones (azul, morado, verde o rosa).
    * **Tablero de Juego:** Interfaz donde se desarrolla la partida, mostrando el tablero, el turno del jugador y la ronda actual. Cuenta con una mecánica de previsualización de la ficha y animaciones al soltarla.
    * **Perfiles, Historial y Rankings:** Secciones dedicadas a consultar estadísticas detalladas del usuario, el historial completo de partidas jugadas (con filtros por fecha) y clasificaciones de los mejores jugadores en un podio visual.

* **Personalización Avanzada:**
    * **Temas y Colores:** Permite cambiar dinámicamente entre un tema claro y uno oscuro, además de elegir entre varias paletas de colores (azul, verde, rosa, naranja) que se aplican a toda la interfaz.
    * **Música y Sonido:** Integra el módulo `Qt Multimedia` para gestionar una lista de reproducción de fondo, con controles de volumen y la posibilidad de seleccionar una canción específica o reproducir la lista completa.
    * **Multi-idioma:** La aplicación soporta traducciones a través de archivos `.qm` y `.ts`, permitiendo cambiar entre español e inglés en tiempo real.

* **Gestión de Base de Datos:** El proyecto utiliza el módulo **Qt SQL** para interactuar con una base de datos SQLite, donde se almacenan de forma persistente los perfiles de usuario, el historial de partidas y las puntuaciones para las clasificaciones.

* **Lógica de Juego Externa:** El proyecto se vincula a una librería externa (`Connect4Lib.dll`) que encapsula la lógica principal del juego (movimientos, detección de ganador, gestión de datos), promoviendo una separación clara entre la interfaz de usuario (frontend) y la lógica de negocio (backend).

## Tecnologías y Componentes

* **Lenguaje:** C++17
* **Framework:** Qt (Módulos Widgets, SQL, Charts, Multimedia)
* **Interfaz de Usuario (UI):** Diseñada con **Qt Designer** (múltiples archivos `.ui`) y estilizada de forma avanzada mediante **Qt Style Sheets (QSS)** externos.
* **Sistema de Build:** QMake (`.pro`)
* **Recursos:** El proyecto utiliza un archivo de recursos (`resources.qrc`) para empaquetar todos los assets necesarios (imágenes para avatares, tableros e iconos; hojas de estilo; archivos de audio `.mp3`; y archivos de traducción), garantizando que la aplicación sea portable y autocontenida.

## Estructura de Archivos

El proyecto está compuesto por numerosas clases, cada una representando una ventana o una funcionalidad específica:

* **`mainwindow.h/.cpp/.ui`**: La ventana principal que actúa como contenedor y gestor de los demás widgets.
* **`welcomewindow.h/.cpp/.ui`**: Implementa la pantalla de bienvenida y el menú principal de navegación.
* **`singwindow.h/.cpp/.ui` y `logwindow.h/.cpp/.ui`**: Gestionan las ventanas de registro e inicio de sesión de usuarios, incluyendo la lógica de recuperación de contraseña.
* **`gameboard.h/.cpp/.ui`**: Define la interfaz y la interacción con el tablero de juego, incluyendo animaciones y la lógica de turnos.
* **`profilewindow.h`, `historywindow.h`, `rankingwindow.h`**: Clases para las pantallas de perfil, historial y clasificaciones, respectivamente.
* **`optionswindow.h/.cpp/.ui`**: Permite al usuario configurar las opciones de la aplicación como el tema, el color, el idioma y la música.
* **`validation.h/.cpp`**: Contiene las funciones de validación utilizadas en los formularios de registro e inicio de sesión.
* **`lib/`**: Carpeta que contiene la librería `Connect4Lib` y sus correspondientes headers (`connect4.h`, `player.h`, etc.) para la gestión de la lógica y la base de datos.
* **`media/`**: Contiene todos los recursos visuales (`images/`) y de audio (`audio/`).
* **`styles/`**: Almacena las hojas de estilo `.qss` para los diferentes temas y colores.
* **`translations/`**: Contiene los archivos de internacionalización (`.ts` y `.qm`).
* **`connect4.pro`**: Archivo de proyecto de QMake que define las dependencias, los archivos fuente/header/ui y la vinculación con la librería externa.
