% *********************************************************************** %
% Obtener_Ku.m                                                            %
%                                                                         %
% Este c�digo permite obtener el valor de Ku mediante prueba y error      %
% *********************************************************************** %
% Autores/as - Correo:                                                    %
%                                                                         %
%   �ngela Espert Cornejo   - aespcor@etsinf.upv.es                       %
%   Lourdes Franc�s Llimer� - lfralli@epsa.upv.es                         %
% *********************************************************************** %

% ----------------------------------------------------------------------- %
% ASIGNACI�N DE VALORES DE LOS PAR�METROS                                 %
% ----------------------------------------------------------------------- %
% Ku propuesta
% --------------------------------- %
 Ku = 0.549;
% --------------------------------- %
% Par�metros fijos
% --------------------------------- %
a = [-0.7719, 0.1747];  % Valores de ai obtenidos en Sal_sist.m
b = [0.6341, 0.4540];   % Valores de bi obtenidos en Sal_sist.m
periodo = T;            % Periodo de muestreo
d = 3;                  % Retardo
Escalon = 2;            % Escalon proporcinado
MUEST = length(tt);     % Tama�o de la muestra (igual al proporcionado en la figura)
% --------------------------------- %
% Inicializaci�n de variables
% --------------------------------- %
y = []; 
error = []; 
u = []; 
t = [];
% --------------------------------- %

% ----------------------------------------------------------------------- %
% BUCLE DE GENERACI�N DE DATOS                                            %
% ----------------------------------------------------------------------- %
for i = 1 : MUEST
    % --------------------------------- %
    % Almacenar valor del istante actual
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
    u(i) = Ku * error(i); 
    % --------------------------------- %
end

% ----------------------------------------------------------------------- %
% VISUALIZACI�N DE LOS RESULTADOS                                         %
% ----------------------------------------------------------------------- %
% Almacenar datos en un fichero de texto
% --------------------------------- %
t = t'; y = y'; u = u';error = error';
% --------------------------------- %
% Graficar los resultados obtenidos
% --------------------------------- %
figure;
plot(t, y);
% ----------------------------------------------------------------------- %