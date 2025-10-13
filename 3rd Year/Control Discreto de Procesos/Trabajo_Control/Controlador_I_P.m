% *********************************************************************** %
% Controlador_I_P.m                                                       %
%                                                                         %
% Este código permite obtener el valor de Kc mediante prueba y error que  %
% permite controlar la salida mediante un controlador P cumpliendo las    %
% siguientes condiciones:                                                 %
%   - Error no nulo, pero inferior al 20%, esto es, que la respuesta del  %
%     sistema se estabilice entre un -20% y +20% del valor que la entrada %
%     escalón proporcionado.                                              %
%   - Las menores oscilaciones posibles.                                  %
% *********************************************************************** %
% Autores/as - Correo:                                                    %
%                                                                         %
%   Ángela Espert Cornejo   - aespcor@etsinf.upv.es                       %
%   Lourdes Francés Llimerá - lfralli@epsa.upv.es                         %
% *********************************************************************** %

% ----------------------------------------------------------------------- %
% ASIGNACIÓN DE VALORES DE LOS PARÁMETROS                                 %
% ----------------------------------------------------------------------- %
% Parámetros fijos
% --------------------------------- %
Ku = 0.549;             % Ku obtenida en Obtener_Ku.m
a = [-0.7719, 0.1747];  % Valores de ai obtenidos en Sal_sist.m
b = [0.6341, 0.4540];   % Valores de bi obtenidos en Sal_sist.m
periodo = T;            % Periodo de muestreo
d = 3;                  % Retardo
Escalon = 2;            % Escalon proporcinado
MUEST = length(tt);     % Tamaño de la muestra (igual al proporcionado en la figura)
% --------------------------------- %
% Kc propuesta
% --------------------------------- %
Kc = (0.5 * Ku) * 1.2;
% --------------------------------- %
% Inicialización de variables
% --------------------------------- %
y = []; 
error = []; 
u = []; 
t = [];
% --------------------------------- %

% ----------------------------------------------------------------------- %
% BUCLE DE GENERACIÓN DE DATOS                                            %
% ----------------------------------------------------------------------- %
for i = 1 : MUEST
    % --------------------------------- %
    % Almacenar valor del instante actual
    % --------------------------------- %
    t(i) = ((i - 1) * periodo);
    % --------------------------------- %
    % Cálculo Salida Bucle Cerrado (con el cálculo usado en el caso 22)
    % --------------------------------- %
    if (i - 1) == 0 
        y(i) = 0;
    elseif (i - 2) == 0 
        if (i - d - 1) <= 0
            if (i - d) <= 0 
                y(i) = - (a(1) * y(i - 1) + a(2) * 0) + (b(1) * 0 + b(2) * 0);
            else    
                y(i) = - (a(1) * y(i - 1) + a(2) * 0) + (b(1) * u(i - d) + b(2) * 0);
            end
        else        
            y(i) = - (a(1) * y(i - 1) + a(2) * 0) + (b(1) * u(i - d) + b(2) * u(i - d - 1));
        end
    else            
        if (i - d - 1) <= 0
            if (i - d) <= 0 
                y(i) = - (a(1) * y(i - 1) + a(2) * y(i - 2)) + (b(1) * 0 + b(2) * 0);
            else        
                y(i) = - (a(1) * y(i - 1) + a(2) * y(i - 2)) + (b(1) * u(i - d) + b(2) * 0);
            end
        else           
            y(i) = - (a(1) * y(i - 1) + a(2) * y(i - 2)) + (b(1) * u(i - d) + b(2) * u(i - d - 1));
        end
    end  
    % --------------------------------- %
    % Cálculo del error
    % --------------------------------- %
    error(i) = Escalon - y(i);
    % --------------------------------- %
    % Cálculo de la acción de control
    % --------------------------------- %
    u(i) = Kc * error(i);   
end

% ----------------------------------------------------------------------- %
% VISUALIZACIÓN DE LOS RESULTADOS                                         %
% ----------------------------------------------------------------------- %
% Almacenar datos en un fichero de texto
% --------------------------------- %
t = t'; y = y'; u = u';error = error';
% --------------------------------- %
% Graficar los resultados obtenidos en una figura
% --------------------------------- %
figure;
hold on; % Mantener la figura y agregar nuevas curvas
% --------------------------------- %
% Acción de control
% --------------------------------- %
plot(t, u, 'c', 'LineWidth', 1.5);
% --------------------------------- %
% Velocidad del motor
% --------------------------------- %
plot(t, y, 'r', 'LineWidth', 1.5); 
% --------------------------------- %
% Líneas discontinuas marcando el +-20%
% --------------------------------- %
yline(2.4, '--g', 'LineWidth', 1.5);
yline(2, 'k', 'LineWidth', 1.5);
yline(1.6, '--m', 'LineWidth', 1.5); 
% --------------------------------- %
% Añadir la leyenda
% --------------------------------- %
legend('Acción de Control', 'Velocidad Motor', 'Límite superior', 'Escalón', 'Límite inferior', 'Location', 'best');  
% --------------------------------- %
% Ajustar título y ejes
% --------------------------------- %
title('Acción de Control - Salida Controlador');
xlabel('Tiempo');
ylabel('Acción de Control');
grid on;
% ----------------------------------------------------------------------- %

% ----------------------------------------------------------------------- %
% CONCLUSIONES TRAS ANALIZAR LOS RESULTADOS                               %
% ----------------------------------------------------------------------- %
% El error es superior al 20%, siendo de un 53%.
% Valores superiores aumentan la oscilación y desestabilizan la respuesta.
% Valores inferiores suponen un mayor error (superior a 20% en todos los 
% casos).
% Por tanto, se procede a realizar un controlador PI que logre estabilizar 
% la respuesta con error inferior al 20%.
% ----------------------------------------------------------------------- %
