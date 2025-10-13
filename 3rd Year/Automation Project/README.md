# Proyecto de Tercer Año: Automatización y Optimización Logística de un Almacén

## Contexto del Proyecto

Este proyecto es una propuesta de la consultora ficticia **EOB (Eficiencia, Optimización y Bienestar)** para automatizar la logística de un cliente del sector de la distribución. El objetivo es optimizar un almacén de 13.000 m² dedicado exclusivamente a la preparación de pedidos en línea (modelo *dark store* o "colmena"), que actualmente gestiona una media de 800 pedidos diarios.

El proceso actual es totalmente manual y se basa en una rígida planificación por horarios para la recogida de los distintos tipos de productos:
* **A partir de las 22:00h:** Preparación de productos secos.
* **A partir de las 03:00h:** Recogida de productos horneados y platos preparados.
* **A partir de las 05:00h:** Incorporación de productos frescos.
* **A partir de las 06/07h:** Inclusión de los productos congelados.

Este método manual presenta oportunidades de mejora significativas en cuanto a eficiencia, reducción de errores y mejora de las condiciones ergonómicas de los trabajadores.

## La Propuesta de Automatización

Se propone una solución de **automatización híbrida**, donde los robots y los operarios colaboran para lograr la máxima eficiencia, reconociendo que no todos los productos pueden ser manipulados de forma autónoma con un coste razonable.

### Funciones y Roles del Sistema

1.  **Automatización Total (Picking Robotizado):**
    Para productos con un envase regular, no frágiles y de peso manejable (la mayoría de productos secos y refrigerados), un **AMR (Robot Móvil Autónomo)** se encarga del proceso completo: localiza el producto, se desplaza hasta la estantería y utiliza su brazo robótico para realizar el *pick & place* en la caja del pedido.

2.  **Colaboración Humano-Robot (Modelo "Goods to Person"):**
    Para productos complejos (grandes, frágiles, con formas irregulares o de apertura fácil), el sistema aplica el modelo "goods to person". El AMR transporta la caja que contiene el producto hasta un puesto de trabajo fijo, donde un operario realiza la manipulación manual. De este modo, se elimina el desplazamiento del operario por el almacén.

3.  **Transporte Automatizado:**
    Los AMR también se encargan de transportar las cajas de pedidos ya completadas desde las zonas manuales (frescos, horno, congelados) hasta el área de parking, optimizando el flujo de materiales en todo el almacén.

### Roles Específicos de los AMR

La flota de robots se organiza en roles dinámicos según la necesidad:
* **Picker AMR:** Rellena las cajas de los pedidos, ya sea de forma autónoma o solicitando ayuda a un operario.
* **Resupply AMR:** Se encarga de la reposición, transportando cajas de productos desde la zona de *resupply* hasta su lugar correspondiente en las estanterías del almacén.
* **Transporter AMR:** Recoge las cajas ya preparadas por los operarios en las zonas no automatizadas y las lleva a la zona de parking.
* **Parking AMR:** Gestiona la zona final, moviendo las cajas completadas desde las estanterías de parking hasta las bases con ruedas de cada pedido, preparándolas para su carga en el camión.

## Diseño y Simulación en FlexSim

Toda la propuesta fue diseñada, validada y optimizada mediante una simulación detallada del "gemelo digital" del almacén en el software **FlexSim**.

### Equipamiento Clave

* **Robot Móvil (AMR):** La solución se basa en una base móvil **MiR250**, sobre la que se monta un sistema de elevadores, un brazo robótico **Fanuc SR-3ia (SCARA)** para manipulaciones rápidas y una ventosa de fuelle como efector final.
* **Infraestructura:** Se reutilizan las estanterías existentes, pero se reconfiguran para alcanzar 10 baldas de altura y aumentar la densidad de almacenamiento. Se diseñan puestos de trabajo "Goods to Person" seguros y ergonómicos.

### Tecnologías y Algoritmos Aplicados

* **Lógica de Procesos en FlexSim:** Toda la lógica del sistema (asignación de tareas, flujos de trabajo, comunicación entre agentes) fue programada visualmente usando el módulo **Process Flow**.
* **Navegación Inteligente (Algoritmo A*):** Se utiliza el algoritmo **A*** para la gestión de rutas de la flota de AMR. Se ha personalizado mediante **FlexScript** el evento de detección de obstáculos para forzar un recálculo de la ruta hacia el destino final, evitando bloqueos y mejorando la fluidez del tráfico.
* **Gestión Dinámica de Tareas y Recursos:** El sistema asigna tareas con una lógica de prioridades (ej: la reposición de productos tiene prioridad sobre el picking). Además, los AMR cuyo rol no es necesario en un momento dado son reasignados dinámicamente para apoyar otras tareas, maximizando así su tiempo de actividad.
* **Optimización por Experimentación:** A través de múltiples experimentos en la simulación, se concluyó que la configuración óptima para manejar la demanda actual requiere **15 AMRs** y **11 operarios**.

## Documentación y Vídeo

A continuación se adjuntan los enlaces a la memoria técnica del proyecto y al vídeo explicativo.

### Descargables

* [Memoria Técnica del Proyecto](URL_DEL_PDF_MEMORIA)
* [Vídeo Explicativo del Proyecto](URL_DEL_VIDEO)

## Conclusiones y Puntos Clave

El proyecto concluye que la implementación de un sistema híbrido de automatización es una solución viable, escalable y rentable que mejora drásticamente la eficiencia del almacén y las condiciones de trabajo. La simulación fue una herramienta fundamental para validar el diseño, optimizar el número de recursos y demostrar la viabilidad del retorno de inversión.

Este trabajo práctico permitió aplicar de forma interdisciplinaria conocimientos de robótica móvil, redes industriales, visión 3D y logística, tomando como referencia soluciones innovadoras del mercado como la de **Brightpick**.
