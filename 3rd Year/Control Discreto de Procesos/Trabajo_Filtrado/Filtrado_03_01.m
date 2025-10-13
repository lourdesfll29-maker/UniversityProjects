% *********************************************************************** %
% Filtrado_03_01.m                                                        %
%                                                                         %
% Este código tiene como objetivo diseñar un filtro elíptico que elimine  %
% o reduzca los componentes frecuenciales excepto los de menor y mayor    %
% frecuencia cumpliendo las siguientes condiciones:                       %                                      
%   - Dibujar el espectro en frecuencia de la señal discreta y3.          %                                 
%   - Elegir los valores de las frecuencias wP y wR (en Hz), así como las %
%     atenuaciones Rp y Rs (en dB) para eliminar o reducir ruidos de y3.  %
%   - Emplear los comandos "ellipord" y "ellip".                          %
%   - Dibujar el espectro en frecuencia de la señal filtrada y compararlo %
%     con el espectros de la señal y3.                                    %
%     Decidir si el fitro es mejorable eligiendo otros valores de         %
%     frecuencia de corte y de rechazo, así como otros valores de         %
%     atenuación Rp y Rs.                                                 %
%   - Obtener el diagrama de Bode del filtro diseñado.                    %
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
T = T3;            % Periodo de muestreo de la señal
wS = (pi * 2) / T; % Frecuencia de muestreo de la señal
y = y3;            % Array con la señal discreta
tt = tt3;          % Array con los tiempos
% --------------------------------- %
% Parámetros propuestos
% --------------------------------- %
wP_rechazo = [11, 35]; % Frecuencia de corte en Hz (frecuencias menores que wP_1 y mayores que wP_2 pasan)
wR_rechazo = [14, 32]; % Frecuencia de rechazo en Hz (frecuencias entre wR_1 y wR_2 se eliminan)
Rp = 0.05;             % Atenuación máxima para la zona de paso en dB (valor reducido 0-3 dB)
Rs = 40;               % Atenuación mínima para la zona de rechazo en dB (valor elevado 20-60 dB)
% --------------------------------- %

% ----------------------------------------------------------------------- %
% CÁLCULO DEL ESPECTRO DE FRECUENCIA                                      %
% ----------------------------------------------------------------------- %
% Transformada de Fourier
% --------------------------------- %
Y = fft(y);
% --------------------------------- %
% Cálculos
% --------------------------------- %
f = (0 : length(Y) - 1)' * wS / length(Y); % Vector de frecuencia
m = abs(Y) * 2 / wS;                       % Amplitud mostrada en el espectro
% --------------------------------- %
% Dibujo del espectro en frecuencia
% --------------------------------- %
figure;
subplot(2, 1, 1)
hold on;
plot(tt, y(1:length(f)), 'b', 'LineWidth', 1.0);
title('Espectro en Frecuencia (Señal original)')
hold off;
subplot(2, 1, 2)
hold on;
plot(f, m(1:length(f)), 'b', 'LineWidth', 1.0);
title('Espectro en Frecuencia (Fast Fourier Transform) de la Señal Original')
hold off;
% ---------------------------

% ----------------------------------------------------------------------- %
% FILTRO DE RECHAZO BANDA                                                 %
% ----------------------------------------------------------------------- %
% Normalización de la frecuencia de corte
% --------------------------------- %
wP_norm = wP_rechazo / (wS / 2);
wR_norm = wR_rechazo / (wS / 2);
% --------------------------------- %
% Asegurar que la frecuencia está en el rango permitido
% --------------------------------- %
if any(wP_norm <= 0 | wP_norm >= 1)
    error(['Las frecuencias de corte normalizadas wP deben estar en el rango (0,1). ' ...
        'wP debe ser menor de %.2f y mayor que 0'], wS / 2);
end

if any(wR_norm <= 0 | wR_norm >= 1)
    error(['Las frecuencias de corte normalizadas wP deben estar en el rango (0,1). ' ...
        'wP debe ser menor de %.2f y mayor que 0'], wS / 2);
end

if wP_rechazo(1) >= wR_rechazo(1) || ...
   wR_rechazo(1) >= wR_rechazo(2) || ...
   wR_rechazo(2) >= wP_rechazo(2)
    error('El orden de frecuencias debe cumplir: wP[1] < wR[1] < wR[2] < wP[2]');
end
% --------------------------------- %
% Obtención de a y b
% --------------------------------- %
[N, wN] = ellipord(wP_norm, wR_norm, Rp, Rs);
[b_rechazo, a_rechazo] = ellip(N, Rp, Rs, wN, 'stop');
% --------------------------------- %
% Obtención de la salida filtrada
% --------------------------------- %
y_filtrada_rechazo = filter(b_rechazo, a_rechazo, y);
% --------------------------------- %
% Dibujo del espectro de la señal original y filtrada
% --------------------------------- %
figure;
hold on;
plot(tt, y, 'b', 'LineWidth', 1.0);                  % Señal original
plot(tt, y_filtrada_rechazo, 'm', 'LineWidth', 1.0); % Señal filtrada rechazo
legend('Señal original', 'Señal filtrada');          % Añadir la leyenda
title('Dibujo del espectro de la señal (Rechazo banda)');
xlabel('Tiempo (s)');                                % Etiqueta para el eje X
ylabel('Amplitud');                                  % Etiqueta para el eje Y
hold off;
grid on;                                             % Activar la cuadrícula
% --------------------------------- %

% ----------------------------------------------------------------------- %
% DIAGRAMA DE BODE DEL FILTRO                                             %
% ----------------------------------------------------------------------- %
figure;
% --------------------------------- %
% Rechazo Banda
% --------------------------------- %
dbode(b_rechazo, a_rechazo, T * 2 * pi);
title('Diagrama de BODE del filtro elíptico (rechazo banda)');
xlabel('Frecuencia (Hz)');
ylabel('Ganancia (dB)');
grid on;
% --------------------------------- %

% ----------------------------------------------------------------------- %
% ESPECTRO DE FRECUENCIA DE LA SEÑAL FILTRADA                             %
% ----------------------------------------------------------------------- %
% Transformada de Fourier
% --------------------------------- %
Y_filtrada = fft(y_filtrada_rechazo);
% Y_filtrada = abs(fft(y_filtrada_rechazo));
% --------------------------------- %
% Cálculos
% --------------------------------- %
f_filtrada = (0 : length(Y_filtrada) - 1)' * wS / length(Y_filtrada); % Vector de frecuencia
m_filtrada = abs(Y_filtrada) * 2 / wS;                                % Amplitud mostrada en el espectro
% --------------------------------- %
% Dibujo del espectro en frecuencia
% --------------------------------- %
figure;
subplot(2, 1, 1);
hold on;
plot(tt, y_filtrada_rechazo(1:length(f)), 'm', 'LineWidth', 1.0);
title('Espectro en Frecuencia (Señal filtrada)')
hold off;
subplot(2, 1, 2);
hold on;
plot(f_filtrada, m_filtrada(1:length(f)), 'm', 'LineWidth', 1.0);
title('Espectro en Frecuencia (Fast Fourier Transform) de la Señal Filtrada')
hold off;
% ---------------------------

% ----------------------------------------------------------------------- %
% VISUALIZACIÓN DE TODOS LOS RESULTADOS                                   %
% ----------------------------------------------------------------------- %
figure;
subplot(2, 1, 1);
hold on;
plot(tt, y, 'b', 'LineWidth', 1.0);
plot(tt, y_filtrada_rechazo, 'm', 'LineWidth', 1.0);
legend('Señal original', 'Señal filtrada', 'Location', 'best');
title('Espectro en Frecuencia')
hold off;
subplot(2, 1, 2);
hold on;
plot(f, m, 'b', 'LineWidth', 1.0);
plot(f_filtrada, m_filtrada, 'm', 'LineWidth', 1.0);
legend('Señal original', 'Señal filtrada', 'Location', 'best');
title('Espectro en Frecuencia (FFT)')
hold off;
% --------------------------------- %

% ----------------------------------------------------------------------- %
% CONCLUSIONES TRAS ANALIZAR LOS RESULTADOS                               %
% ----------------------------------------------------------------------- %
% El filtro de rechazo de banda es aquel que permite que pasen frecuencias
% fuera del rango de valores wP y que elimina frecuencias dentro del rango
% wR. Por tanto, ya que se pide un filtro que reduzca los componentes
% frecuenciales con excepción de los de menor y mayor frecuencia, se
% considera este filtro como el más adecuado.
% ----------------------------------------------------------------------- %