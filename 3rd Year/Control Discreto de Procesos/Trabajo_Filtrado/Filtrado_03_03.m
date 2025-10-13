% *********************************************************************** %
% Filtrado_03_03.m                                                        %
%                                                                         %
% Este código tiene como objetivo diseñar un filtro elíptico que elimine  %
% todos los componentes frecuenciales menos el de mayor frecuencia        %
% cumpliendo las siguientes condiciones:                                  %                                      
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
wP_high = 33; % Frecuencia de corte en Hz (frecuencias mayores pasan)
wR_high = 30; % Frecuencia de rechazo en Hz (frecuencias menores se eliminan)
Rp = 0.05;    % Atenuación máxima para la zona de paso en dB (valor reducido 0-3 dB)
Rs = 50;      % Atenuación mínima para la zona de rechazo en dB (valor elevado 20-60 dB)
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
% FILTRO DE PASO HIGH                                                     %
% ----------------------------------------------------------------------- %
% Normalización de la frecuencia de corte
% --------------------------------- %
wP_norm = wP_high / (wS / 2);
wR_norm = wR_high / (wS / 2);
% --------------------------------- %
% Asegurar que la frecuencia está en el rango permitido
% --------------------------------- %
if (wP_norm <= 0 || wP_norm >= 1)
    error(['La frecuencia de corte normalizada wP debe estar en el rango (0,1). ' ...
        'wP debe ser menor de %.2f y mayor que 0'], wS / 2);
end

if (wR_norm <= 0 || wR_norm >= 1)
    error(['La frecuencia de corte normalizada wR debe estar en el rango (0,1).' ...
        'wR debe ser menor de %.2f y mayor que 0'], wS / 2);
end

if wR_high >= wP_high
    error('La frecuencia de paso wP debe ser mayor que la frecuencia de rechazo wR');
end
% --------------------------------- %
% Obtención de a y b
% --------------------------------- %
[N, wN] = ellipord(wP_norm, wR_norm, Rp, Rs);
[b_high, a_high] = ellip(N, Rp, Rs, wN, 'high');
% --------------------------------- %
% Obtención de la salida filtrada
% --------------------------------- %
y_filtrada_high = filter(b_high, a_high, y);
% --------------------------------- %
% Dibujo del espectro de la señal original y filtrada
% --------------------------------- %
figure;
hold on;
plot(tt, y(1:length(f)), 'b', 'LineWidth', 1.0);               % Señal original
plot(tt, y_filtrada_high(1:length(f)), 'm', 'LineWidth', 1.0); % Señal filtrada high
legend('Señal original', 'Señal filtrada');                    % Añadir la leyenda
title('Dibujo del espectro de la señal (Paso high)');
xlabel('Tiempo (s)');                                          % Etiqueta para el eje X
ylabel('Amplitud');                                            % Etiqueta para el eje Y
hold off;
grid on;                                                       % Activar la cuadrícula
% --------------------------------- %

% ----------------------------------------------------------------------- %
% DIAGRAMA DE BODE DEL FILTRO                                             %
% ----------------------------------------------------------------------- %
figure;
% --------------------------------- %
% Paso High
% --------------------------------- %
dbode(b_high, a_high, T * 2 * pi);
title('Diagrama de BODE del filtro elíptico (paso high)');
xlabel('Frecuencia (Hz)');
ylabel('Ganancia (dB)');
grid on;
% --------------------------------- %

% ----------------------------------------------------------------------- %
% ESPECTRO DE FRECUENCIA DE LA SEÑAL FILTRADA                             %
% ----------------------------------------------------------------------- %
% Transformada de Fourier
% --------------------------------- %
Y_filtrada = fft(y_filtrada_high);
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
plot(tt, y_filtrada_high(1:length(f)), 'm', 'LineWidth', 1.0);
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
plot(tt, y_filtrada_high(1:length(f)), 'm', 'LineWidth', 1.0);
legend('Señal original', 'Señal filtrada', 'Location', 'best');
title('Espectro en Frecuencia')
hold off;
subplot(2, 1, 2);
hold on;
plot(f, m, 'b', 'LineWidth', 1.0);
plot(f_filtrada, m_filtrada(1:length(f)), 'm', 'LineWidth', 1.0);
legend('Señal original', 'Señal filtrada', 'Location', 'best');
title('Espectro en Frecuencia (FFT)')
hold off;
% --------------------------------- %

% ----------------------------------------------------------------------- %
% CONCLUSIONES TRAS ANALIZAR LOS RESULTADOS                               %
% ----------------------------------------------------------------------- %
% El filtro de paso alto es aquel que permite que pasen frecuencias mayores
% que wP y que elimina frecuencias menores que wR. Por tanto, ya que se 
% pide un filtro que elimine todos los componentes frecuenciales menos el 
% de mayor frecuencia, se considera este filtro como el más adecuado.
% ----------------------------------------------------------------------- %