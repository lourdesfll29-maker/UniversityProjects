% *********************************************************************** %
% Controlador_III_PI.m                                                    %
%                                                                         %
% Este c�digo permite obtener el valor de Kc y Ti mediante prueba y error %
% que permiten controlar la salida mediante un controlador PI cumpliendo  %
% las siguientes condiciones:                                             %
%   - Error nulo.                                                         %
%   - La respuesta tenga un m�ximo (sobreoscilaci�n) que no sea superior  %
%     a un incremento del 15%, ni inferior a un incremento del 10% del    %
%     valor al que se estabiliza.                                         %
%   - Se valorar� m�s positivamente, aparte de lo anterior, que la        %
%     respuesta se estabilice m�s r�pidamente.                            %
% *********************************************************************** %
% Autores/as - Correo:                                                    %
%                                                                         %
%   �ngela Espert Cornejo   - aespcor@etsinf.upv.es                       %
%   Lourdes Franc�s Llimer� - lfralli@epsa.upv.es                         %
% *********************************************************************** %

% ----------------------------------------------------------------------- %
% ASIGNACI�N DE VALORES DE LOS PAR�METROS                                 %
% ----------------------------------------------------------------------- %
% Par�metros fijos
% --------------------------------- %
Ku = 0.549;             % Ku obtenida en Obtener_Ku.m
Tu = 0.02;
a = [-0.7719, 0.1747];  % Valores de ai obtenidos en Sal_sist.m
b = [0.6341, 0.4540];   % Valores de bi obtenidos en Sal_sist.m
periodo = T;            % Periodo de muestreo
d = 3;                  % Retardo
Escalon = 2;            % Escalon proporcinado
MUEST = length(tt);     % Tama�o de la muestra (igual al proporcionado en la figura)
% --------------------------------- %
% Kc y Ti propuestos para el controlador PI
% --------------------------------- %
Kc = (0.45 * Ku) * 0.686; 
Ti = (Tu / 1.2) * 13.77; 
% --------------------------------- %
% Inicializaci�n de variables
% --------------------------------- %
y = []; 
error = []; 
u = []; 
t = [];
% --------------------------------- %
% C�lculo de p
% --------------------------------- %
p = Kc * (1 - (T / Ti));
% --------------------------------- %

% ----------------------------------------------------------------------- %
% BUCLE DE GENERACI�N DE DATOS                                            %
% ----------------------------------------------------------------------- %
for i = 1 : MUEST
    % --------------------------------- %
    % Almacenar valor del instante actual
    % --------------------------------- %
    t(i) = ((i - 1) * periodo);
    % --------------------------------- %
    % C�lculo Salida Bucle Cerrado (con el c�lculo usado en el caso 22)
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
    % C�lculo del error
    % --------------------------------- %
    error(i) = Escalon - y(i);
    % --------------------------------- %
    % C�lculo de la acci�n de control
    % --------------------------------- %
    if (i == 1) 
        u(i) = Kc * error(i);
    else 
        u(i) = u(i - 1) + Kc * error(i) - p * error(i - 1);
    end
    % --------------------------------- %
end

% ----------------------------------------------------------------------- %
% VISUALIZACI�N DE LOS RESULTADOS                                         %
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
% Acci�n de control
% --------------------------------- %
plot(t, u, 'c', 'LineWidth', 1.5);
% --------------------------------- %
% Velocidad del motor
% --------------------------------- %
plot(t, y, 'r', 'LineWidth', 1.5); 
% --------------------------------- %
% L�neas discontinuas marcando el +-20%
% --------------------------------- %
yline(2.3, '--g', 'LineWidth', 1.5);
yline(2, 'k', 'LineWidth', 1.5);
yline(2.2, '--m', 'LineWidth', 1.5); 
% --------------------------------- %
% A�adir la leyenda
% --------------------------------- %
legend('Acci�n de Control', 'Velocidad Motor', 'L�mite superior', 'Escal�n', 'L�mite inferior', 'Location', 'best');  
% --------------------------------- %
% Ajustar t�tulo y ejes
% --------------------------------- %
title('Acci�n de Control - Salida Controlador');
xlabel('Tiempo');
ylabel('Acci�n de Control');
grid on;
% ----------------------------------------------------------------------- %

% ----------------------------------------------------------------------- %
% CONCLUSIONES TRAS ANALIZAR LOS RESULTADOS                               %
% ----------------------------------------------------------------------- %
% Tras probar m�ltiples valores, se ha concluido que el mejor resultado se
% obtiene con:
% Kc = (0.45 * Ku) * 0.686; 
% Ti = (Tu / 1.2) * 13.77; 
% Donde la estabilizaci�n de la respuesta se da en x = 2.3, superando 
% anteriormente el valor del escal�n en un 10%.
% -----------------------------------------------------------------------�%