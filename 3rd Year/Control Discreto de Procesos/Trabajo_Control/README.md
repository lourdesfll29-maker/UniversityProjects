# Proyecto de Diseño de Controladores P/PI (Control Discreto)

## Contexto del Proyecto

Este proyecto, segunda parte de la práctica de **Control Discreto**, se enfoca en el diseño y sintonización de controladores Proporcionales (P) y Proporcionales-Integrales (PI) para un sistema discreto. El objetivo es modelar un sistema a partir de datos de entrada/salida y luego diseñar controladores que cumplan con especificaciones de error, sobreoscilación y tiempo de estabilización.

El proyecto utiliza **MATLAB** para la identificación del sistema y la simulación de los controladores diseñados.

## Descripción de las Fases del Proyecto

### 1. Identificación del Sistema (`Sal_sist.m`)

El primer paso consiste en obtener un modelo matemático del sistema a controlar.
* **Análisis de Datos:** Se utilizan los datos de entrada (`u`) y salida (`y`) del sistema para encontrar los parámetros que mejor lo representan.
* **Mínimos Cuadrados:** Se implementa el método de mínimos cuadrados (`ident.m`) para estimar los coeficientes `a` y `b` de la función de transferencia del sistema para diferentes órdenes (`na`, `nb`).
* **Selección del Modelo:** Se simulan las respuestas de los diferentes modelos obtenidos y se comparan con la respuesta real del sistema para seleccionar el modelo más preciso (en este caso, `na=2`, `nb=2`).

### 2. Obtención de Parámetros de Ziegler-Nichols (`Obtener_Ku.m`)

Antes de diseñar los controladores PI, se utiliza el método de Ziegler-Nichols en lazo cerrado para obtener los parámetros fundamentales del sistema:
* **Ganancia Crítica (Ku):** Se busca de forma iterativa el valor de ganancia `Ku` que lleva al sistema al límite de la estabilidad (oscilaciones mantenidas).
* **Periodo Crítico (Tu):** Se mide el periodo de estas oscilaciones.

Estos dos parámetros (`Ku` y `Tu`) sirven como base para la sintonización inicial de los controladores PI.

### 3. Diseño de Controladores P y PI (`Controlador_I_P.m` y `Controlador_I_PI.m`)

En esta fase se diseñan y sintonizan por prueba y error un controlador Proporcional y un Proporcional-Integral para cumplir una primera especificación:
* **Controlador P:** Se busca un valor de `Kc` que minimice el error de estado estacionario, aunque no lo elimina.
* **Controlador PI:** Se ajustan `Kc` y `Ti` para conseguir un error inferior al 20% y con las menores oscilaciones posibles.

### 4. Sintonización Fina de Controladores PI (`Controlador_II_PI.m` y `Controlador_III_PI.m`)

Finalmente, se diseñan dos controladores PI más con especificaciones más estrictas, partiendo de los valores recomendados por Ziegler-Nichols y ajustándolos finamente por prueba y error:

* **Controlador II (Error nulo sin sobreoscilación):** Se sintonizan `Kc` y `Ti` para que la respuesta alcance el valor de referencia lo más rápido posible, pero sin superarlo en ningún momento.
* **Controlador III (Error nulo con sobreoscilación acotada):** Se ajustan los parámetros para lograr un error nulo, permitiendo una sobreoscilación controlada (entre el 10% y el 15% del valor final) para conseguir un tiempo de estabilización más rápido.

## Metodología y Herramientas

* **Software:** **MATLAB** es la herramienta principal para la identificación, simulación y visualización.
* **Identificación de Sistemas:** Se utiliza el método de **mínimos cuadrados** para obtener el modelo del sistema.
* **Sintonización de Controladores:** Se emplea el método de **Ziegler-Nichols** para la sintonización inicial y un ajuste fino por **prueba y error** para cumplir las especificaciones finales.
* **Visualización:** Se utilizan las gráficas de MATLAB (`plot`) y figuras (`.fig`) para comparar la acción de control y la salida del sistema con la referencia, verificando el cumplimiento de los requisitos.
