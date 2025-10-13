# Proyecto de Filtrado Digital (Control Discreto)

## Contexto del Proyecto

Este proyecto, desarrollado para la asignatura de **Control Discreto**, se centra en el diseño, implementación y análisis de diferentes tipos de filtros digitales para procesar señales discretas afectadas por ruido. Utilizando **MATLAB**, se aplican diversas técnicas de filtrado para eliminar o atenuar componentes frecuenciales no deseadas, comparando los resultados de la señal filtrada con la señal original y la señal ideal sin ruido.

El proyecto parte de un conjunto de datos (`Espert_Frances_57370_datos_T_F.mat`) que contiene varias señales (`y1`, `y2`, `y3`) con sus respectivas versiones sin ruido.

## Descripción de los Ejercicios y Filtros Implementados

El trabajo se divide en tres bloques principales, cada uno abordando un tipo de filtro y un problema de ruido específico.

### 1. Filtro Bilineal Paso Bajo (`Filtrado_01_low.m`)

El objetivo de este primer ejercicio es eliminar el ruido de alta frecuencia presente en la señal `y1`.
* **Análisis:** Se utiliza la Transformada Rápida de Fourier (FFT) para analizar el espectro de la señal ruidosa (`y1`) y la señal limpia (`y1_sinruido`), identificando así las frecuencias que constituyen el ruido.
* **Diseño:** Se diseña un **filtro paso bajo** mediante la transformación **bilineal**, eligiendo una frecuencia de corte adecuada para atenuar las componentes de alta frecuencia no deseadas.
* **Resultados:** Se compara la señal filtrada con la original y la ideal, tanto en el dominio del tiempo como en el de la frecuencia, y se analiza la respuesta del filtro mediante su diagrama de Bode.

### 2. Filtro Bilineal de Rechazo de Banda (`Filtrado_02_rechazo.m`)

Este ejercicio aborda la eliminación de ruido concentrado en una banda de frecuencia específica en la señal `y2`.
* **Análisis:** Al igual que en el caso anterior, se comparan los espectros de `y2` e `y2_sinruido` para identificar la banda de frecuencia del ruido a eliminar.
* **Diseño:** Se implementa un **filtro de rechazo de banda** para atenuar específicamente las frecuencias de ruido, preservando el resto de los componentes de la señal.

### 3. Diseño de Filtros Elípticos (`Filtrado_03_xx.m`)

El tercer bloque se centra en el diseño de **filtros elípticos**, que ofrecen una transición muy abrupta entre la banda de paso y la banda de rechazo, para la señal `y3`. Se diseñan tres filtros diferentes:

* **`Filtrado_03_01.m` (Rechazo de Banda):** Para eliminar las frecuencias intermedias.
* **`Filtrado_03_02.m` (Paso Banda):** Para conservar solo las frecuencias intermedias.
* **`Filtrado_03_03.m` (Paso Alto):** Para preservar únicamente la componente de mayor frecuencia.

## Metodología y Herramientas

* **Software:** **MATLAB** es la herramienta principal utilizada para todo el proyecto.
* **Análisis Frecuencial:** Se utiliza la **Transformada Rápida de Fourier (FFT)** para visualizar el espectro de las señales.
* **Diseño de Filtros:** Se emplean funciones como `bilinear`, `ellipord` y `ellip`.
* **Análisis de Filtros:** Se utiliza el **diagrama de Bode** (`dbode`) para analizar la respuesta en frecuencia de los filtros diseñados.
