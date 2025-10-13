# Proyecto de Agentes Inteligentes: Equipo Táctico BDI en Pygomas

## Contexto del Proyecto

Este proyecto, desarrollado para la asignatura de **Agentes Inteligentes**, se centra en la implementación de un equipo de agentes autónomos con comportamiento táctico y coordinado dentro del entorno de simulación **Pygomas**. El objetivo es diseñar un equipo de agentes, utilizando la arquitectura **BDI (Beliefs-Desires-Intentions)**, capaz de competir eficazmente en un escenario de "capturar la bandera", demostrando una inteligencia y coordinación superiores a las de un equipo con comportamiento básico.

## La Propuesta: Un Equipo Coordinado y Jerárquico

La solución se basa en la creación de un equipo de agentes heterogéneo y comunicativo, liderado por un **agente Mánager**. Este mánager centraliza la toma de decisiones estratégicas y asigna roles dinámicamente a los agentes de tropa, optimizando la respuesta del equipo a las condiciones cambiantes del entorno.

El equipo está compuesto por dos roles principales:

* **Mánager (`manager.py`):** Es el cerebro del equipo. No participa directamente en el combate, sino que su función es analizar el estado global del juego y coordinar a las tropas. Se encarga de asignar los roles de **Atacante** y **Defensor** a los agentes BDI.
* **Tropa (`bditroop.py`):** Son los agentes BDI que ejecutan las acciones en el campo de batalla. Su comportamiento es dinámico y se adapta según el rol que el mánager les asigna.
    * **Rol de Atacante:** Su objetivo es avanzar hacia la bandera enemiga, capturarla y regresar a la base.
    * **Rol de Defensor:** Su misión es proteger la bandera propia, patrullando la base y neutralizando a los enemigos que se acerquen.

## Arquitectura y Tecnologías

El comportamiento de los agentes se ha implementado utilizando la plataforma **Pygomas**, que se integra con el framework **Spade** para la creación de sistemas multiagente.

* **Python:** Todo el comportamiento de los agentes y la lógica de gestión están implementados en Python.
    * **`manager.py`**: Contiene la lógica del agente Mánager, incluyendo la inicialización del equipo, la asignación de roles y la monitorización del estado de los agentes de tropa.
    * **`bditroop.py`**: Define la clase base para los agentes de tropa. Implementa la capacidad de recibir y adoptar roles asignados por el mánager, así como las acciones básicas de movimiento, patrulla y ataque.

* **Arquitectura BDI:** Aunque los archivos `.asl` no se utilizan directamente en esta implementación, la lógica sigue un paradigma BDI implícito:
    * **Creencias (Beliefs):** Los agentes mantienen información sobre su estado (salud, munición), su rol actual y la posición de objetivos clave como las banderas y la base.
    * **Deseos (Desires):** Los objetivos principales son capturar la bandera enemiga y defender la propia.
    * **Intenciones (Intentions):** Los agentes adoptan planes específicos (patrullar, atacar, ir a la base) basados en el rol que les ha sido asignado por el mánager.

## Comportamiento y Estrategia

La inteligencia del equipo se basa en una clara división de roles y una estrategia coordinada:

* **Asignación Dinámica de Roles:** Al inicio de la partida, el agente Mánager designa a la mitad de las tropas como atacantes y a la otra mitad como defensores. Esta asignación permite una estrategia equilibrada entre ofensiva y defensiva.
* **Comunicación Jerárquica:** Las tropas informan periódicamente de su estado al Mánager. La comunicación es principalmente vertical (Tropa -> Mánager -> Tropa), lo que centraliza la estrategia y evita la sobrecarga de mensajes entre todos los agentes.
* **Patrullaje y Defensa:** Los agentes defensores patrullan las inmediaciones de su base. Si un enemigo entra en su campo de visión, abandonan la patrulla para neutralizar la amenaza.
* **Ataque Coordinado:** Los agentes atacantes se dirigen hacia la bandera enemiga. Si encuentran un oponente, intentan eliminarlo antes de continuar con su objetivo principal. Una vez capturada la bandera, su objetivo cambia a regresar a la base para anotar un punto.
