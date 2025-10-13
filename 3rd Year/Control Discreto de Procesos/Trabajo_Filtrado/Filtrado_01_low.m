% *********************************************************************** %
% Filtrado_01_low.m                                                       %
%                                                                         %
% Este código tiene como objetivo diseñar un filtro bilineal que elimine  %
% o reduzca los componentes frecuenciales identificados como ruido        %
% cumpliendo las siguientes condiciones:                                  %                                      
%   - Distinguir lo que es ruido de lo que no es (comparar espectros en   %
%     frecuencia de y1_sinruido con y1.                                   %
%   - Elegir las frecuencias de corte más adecuadas e implementar el      %
%     código de la ecuación en diferencias del filtro.                    %
%   - Dibujar el espectro en frecuencia de la señal filtrada y compararlo %
%     con los espectros en frecuencias de las señales y1_sinruido e y1.   %
%     Según el ruido eliminado, considerar la opción de probar una nueva  %
%     frecuencia de corte.                                                %
%   - Obtener el diagrama de Bode del filtro diseñado.                    %
%   - Dibujar las señales y1, y1_sinruido e y1_filtrada en el dominio del %
%     tiempo.                                                             %
% *********************************************************************** %
% Autores/as - Correo:                                                    %
%                                                                         %
%   Ángela Espert Cornejo   - aespcor@etsinf.upv.es                       %
%   Lourdes Francés Llimerá - lfralli@epsa.upv.es                         %
% *********************************************************************** %

% ----------------------------------------------------------------------- %
% EJECUCIONES INICIALES                                                   %
% ----------------------------------------------------------------------- %
clc;                                        % Limpieza de la Command Window
clear all;                                  % Limpieza del Workspace
close all;                                  % Limpieza de figuras abiertas
load('Espert_Frances_57370_datos_T_F.mat'); % Carga de datos
% --------------------------------- %

% ----------------------------------------------------------------------- %
% ASIGNACIÓN DE VALORES DE LOS PARÁMETROS                                 %
% ----------------------------------------------------------------------- %
% Parámetros fijos
% --------------------------------- %
T = T1;            % Periodo de muestreo de la señal
wS = (pi * 2) / T; % Frecuencia de muestreo (Hz)
y = y1;            % Array con la señal discreta
tt = tt1;          % Array con los tiempos
% --------------------------------- %
% wC propuesta
% --------------------------------- %
wC_low = 0.88;            % Frecuencia de corte para paso bajo (Hz) 
wC_rechazo = [0.62, 1.8]; % Banda de rechazo (Hz)
% --------------------------------- %

% ----------------------------------------------------------------------- %
% FILTROS BUTTERWORTH                                                     %
% ----------------------------------------------------------------------- %
% Normalización de las frecuencias de corte
% --------------------------------- %
wC_low_norm = wC_low / (wS / 2);
wC_rechazo_norm = wC_rechazo / (wS / 2);
% --------------------------------- %
% Asegurar que las frecuencias están en el rango permitido
% --------------------------------- %
if any(wC_low_norm <= 0 | wC_low_norm >= 1)
    error(['La frecuencia de corte normalizada del paso bajo está fuera del rango permitido (0,1). ' ...
        'wC debe ser menor de %.2f y mayor que 0'], wS / 2);
end
if any(wC_rechazo_norm <= 0 | wC_rechazo_norm >= 1)
    error(['La frecuencia de corte del rechazo de banda está fuera del rango permitido (0,1). ' ...
        'wC debe ser menor de %.2f y mayor que 0'], wS / 2);
end
% --------------------------------- %
% Obtención de a y b 
% --------------------------------- %
[b_low, a_low] = butter(1, wC_low_norm, 'low');              % Paso bajo
[b_rechazo, a_rechazo] = butter(1, wC_rechazo_norm, 'stop'); % Rechazo de banda
% --------------------------------- %
% Obtención de la salida filtrada
% --------------------------------- %
y_filtrada_low = filter(b_low, a_low, y);
y_filtrada_rechazo = filter(b_rechazo, a_rechazo, y);
% --------------------------------- %

% ----------------------------------------------------------------------- %
% VISUALIZACIÓN DE LOS RESULTADOS                                         %
% ----------------------------------------------------------------------- %
figure;
% --------------------------------- %
% Paso Bajo
% --------------------------------- %
subplot(2,1,1);
plot(tt, y, 'b', tt, y_filtrada_low, 'r', 'LineWidth', 1.5)
legend('Original', 'Paso Bajo')
title('Filtro de Paso Bajo')
% --------------------------------- %
% Rechazo Banda
% --------------------------------- %
subplot(2,1,2);
plot(tt, y, 'b', tt, y_filtrada_rechazo, 'r', 'LineWidth', 1.5)
legend('Original', 'Rechazo de Banda')
title('Filtro de Rechazo de Banda')
% --------------------------------- %
% Ejes
% --------------------------------- %
xlabel('Tiempo (s)')
ylabel('Amplitud')
% --------------------------------- %

% ----------------------------------------------------------------------- %
% ESPECTROS DE FRECUENCIA                                                 %
% ----------------------------------------------------------------------- %
% Transformada de Fourier
% --------------------------------- %
Y = abs(fft(y1));
Y_sinruido = abs(fft(y1_sinruido));
Y_low = abs(fft(y_filtrada_low));
Y_rechazo = abs(fft(y_filtrada_rechazo));
% --------------------------------- %
% Cálculos de los vectores de frecuencia
% --------------------------------- %
f = (0 : length(Y) - 1)' * wS / length(Y);
f_sinruido = (0 : length(Y_sinruido) - 1)' * wS / length(Y_sinruido);
f_low = (0 : length(Y_low) - 1)' * wS / length(Y_low);
f_rechazo = (0 : length(Y_rechazo) - 1)' * wS / length(Y_rechazo);
% --------------------------------- %
% Espectro en Frecuencia Original vs Sin Ruido
% --------------------------------- %
figure;
hold on;
plot(f, Y(1:length(f)), 'b', 'LineWidth', 1.5);
plot(f_sinruido, Y_sinruido(1:length(f_sinruido)), 'r', 'LineWidth', 1.5);
legend('Original', 'Sin ruido', 'Location', 'best');
title('Espectro - Original vs Sin ruido')
hold off;
grid on;
% --------------------------------- %
% Espectro en Frecuencia según el filtro
% --------------------------------- %
figure;
% --------------------------------- %
% Paso Bajo
% --------------------------------- %
subplot(4,1,1);
plot(f, Y(1:length(f)), 'b', f_low, Y_low(1:length(f)), 'r', 'LineWidth', 1.5)
legend('Original', 'Paso Bajo')
title('Espectro - Filtro de Paso Bajo')

subplot(4,1,2);
plot(f_sinruido, Y_sinruido(1:length(f_sinruido)), 'b', f_low, Y_low(1:length(f)), 'r', 'LineWidth', 1.5)
legend('Sin Ruido', 'Paso Bajo')
title('Espectro - Filtro de Paso Bajo')
% --------------------------------- %
% Rechazo Banda
% --------------------------------- %
subplot(4,1,3);
plot(f, Y(1:length(f)), 'b', f_rechazo, Y_rechazo(1:length(f)), 'r', 'LineWidth', 1.5)
legend('Original', 'Rechazo de Banda')
title('Espectro - Filtro de Rechazo de Banda')

subplot(4,1,4);
plot(f_sinruido, Y_sinruido(1:length(f_sinruido)), 'b', f_rechazo, Y_rechazo(1:length(f)), 'r', 'LineWidth', 1.5)
legend('Sin Ruido', 'Rechazo de Banda')
title('Espectro - Filtro de Rechazo de Banda')
% --------------------------------- %
% Ejes
% --------------------------------- %
xlabel('Frecuencia (Hz)')
ylabel('Amplitud')
% --------------------------------- %

% ----------------------------------------------------------------------- %
% DIAGRAMA DE BODE DE LOS FILTROS                                         %
% ----------------------------------------------------------------------- %
figure;
% --------------------------------- %
% Paso Bajo
% --------------------------------- %
subplot(2,1,1);
dbode(b_low, a_low, T * 2 * pi);
title('Bode - Filtro Paso Bajo');
xlabel('Frecuencia (Hz)'); 
ylabel('Magnitud (dB)'); 
grid on;
% --------------------------------- %
% Rechazo Banda
% --------------------------------- %
subplot(2,1,2);
dbode(b_rechazo, a_rechazo, T * 2 * pi);
title('Bode - Filtro Rechazo de Banda');
xlabel('Frecuencia (Hz)'); 
ylabel('Magnitud (dB)'); 
grid on;
% --------------------------------- %

% ----------------------------------------------------------------------- %
% CONCLUSIONES TRAS ANALIZAR LOS RESULTADOS                               %
% ----------------------------------------------------------------------- %
% Paso bajo proporciona el mejor resultado.
% --------------------------------- %
figure;
hold on;
plot(tt, y, 'b', 'LineWidth', 1.2); 
plot(tt, y1_sinruido, 'g', 'LineWidth', 1.2);
plot(tt, y_filtrada_low, 'r', 'LineWidth', 1.5); 
hold off;
legend('Señal original', 'Señal sin ruido', 'Señal filtrada paso bajo')
xlabel('Tiempo (s)')
ylabel('Amplitud')
title('Comparación de señales')
% ----------------------------------------------------------------------- %