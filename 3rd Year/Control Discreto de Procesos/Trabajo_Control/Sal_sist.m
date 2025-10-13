% *********************************************************************** %
% SAL_SIST.m                                                              %
%                                                                         %
% Este código permite analizar qué na y nb deben emplearse a la hora de   %
% obtener Ku y realizar el control.                                       %
% *********************************************************************** %
% Autores/as - Correo:                                                    %
%                                                                         %
%   Ángela Espert Cornejo   - aespcor@etsinf.upv.es                       %
%   Lourdes Francés Llimerá - lfralli@epsa.upv.es                         %
% *********************************************************************** %

% ----------------------------------------------------------------------- %
% ASIGNACIÓN DE VALORES DE LOS PARÁMETROS                                 %
% ----------------------------------------------------------------------- %
% Parámetros extraídos del caso proporcionado
% --------------------------------- %
tiempo_fin = tt(length(tt));    %Tiempo final de simulación 
Escalon = 2;                    % Entrada constante 
periodo = T;                    % Periodo de muestreo
% --------------------------------- %
% Parámetros para la extracción de theta
% --------------------------------- %
d = 3; % Retardo del sistema
H = 7; % Elementos iniciales de los vectores de entrada y salida despreciables.        
% --------------------------------- %
% Parámetros para el cálculo de la salida
% --------------------------------- %
k = 1;
t = 0;
% --------------------------------- %

% ----------------------------------------------------------------------- %
% CÁLCULO DE THETA SEGÚN EL CASO                                          %
% ----------------------------------------------------------------------- %
% Caso 11 (y = ye, u = ue, na = 1, nb = 1, d = 3, H = 7)
% --------------------------------- %
theta_11 = ident(ye, ue, 1, 1, d, H);
% Parámetros ai del sistema  
a_11_01 = theta_11(1);            
% Parámetros bi del sistema
b_11_01 = theta_11(2);       
% --------------------------------- %
% Caso 12 (y = ye, u = ue, na = 1, nb = 2, d = 3, H = 7)
% --------------------------------- %
theta_12 = ident(ye, ue, 1, 2, d, H);
% Parámetros ai del sistema  
a_12_01 = theta_12(1);           
% Parámetros bi del sistema
b_12_01 = theta_12(2);          
b_12_02 = theta_12(3);          
% --------------------------------- %
% Caso 21 (y = ye, u = ue, na = 2, nb = 1, d = 3, H = 7)
% --------------------------------- %
theta_21 = ident(ye, ue, 2, 1, d, H);
% Parámetros ai del sistema
a_21_01 = theta_21(1);             
a_21_02 = theta_21(2);   
% Parámetros bi del sistema
b_21_01 = theta_21(3);           
% --------------------------------- %
% Caso 22 (y = ye, u = ue, na = 2, nb = 2, d = 3, H = 7)
% --------------------------------- %
theta_22 = ident(ye, ue, 2, 2, d, H);
% Parámetros ai del sistema
a_22_01 = theta_22(1);          
a_22_02 = theta_22(2);          
% Parámetros bi del sistema
b_22_01 = theta_22(3);           
b_22_02 = theta_22(4);          
% --------------------------------- %
% Caso 23 (y = ye, u = ue, na = 2, nb = 3, d = 3, H = 7)
% --------------------------------- %
theta_23 = ident(ye, ue, 2, 3, d, H);
% Parámetros ai del sistema
a_23_01 = theta_23(1);          
a_23_02 = theta_23(2);          
% Parámetros bi del sistema
b_23_01 = theta_23(3);          
b_23_02 = theta_23(4);          
b_23_03 = theta_23(5);          
% --------------------------------- %
% Caso 32 (y = ye, u = ue, na = 3, nb = 2, d = 3, H = 7)
% --------------------------------- %
theta_32 = ident(ye, ue, 3, 2, d, H);
% Parámetros ai del sistema
a_32_01 = theta_32(1);         
a_32_02 = theta_32(2);          
a_32_03 = theta_32(3);          
% Parámetros bi del sistema
b_32_01 = theta_32(4);          
b_32_02 = theta_32(5);         
% --------------------------------- %
% Caso 33 (y = ye, u = ue, na = 3, nb = 3, d = 3, H = 7)
% --------------------------------- %
theta_33 = ident(ye, ue, 3, 3, d, H);
% Parámetros ai del sistema
a_33_01 = theta_33(1);          
a_33_02 = theta_33(2);     
a_33_03 = theta_33(3);  
% Parámetros bi del sistema
b_33_01 = theta_33(4);           
b_33_02 = theta_33(5);           
b_33_03 = theta_33(6);     
% --------------------------------- %

% ----------------------------------------------------------------------- %
% CÁLCULO DE LA SALIDA SEGÚN EL CASO                                      %
% ----------------------------------------------------------------------- %
for t = 0 : periodo : tiempo_fin
    in(k) = Escalon;
    % --------------------------------- %
    % Caso 11
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_11(k) = 0;
    else 
        if (k - d) <= 0 % (k - 1) >= 1
            sal_11(k) = - (a_11_01 * sal_11(k - 1)) + (b_11_01 * 0);
        else            % (k - 1) >= 1,               (k - d) >= 1
            sal_11(k) = - (a_11_01 * sal_11(k - 1)) + (b_11_01 * in(k - d));
        end
    end
    % --------------------------------- %
    % Caso 12
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_12(k) = 0;
    else 
        if (k - d - 1) <= 0
            if (k - d) <= 0 % (k - 1) >= 1
                sal_12(k) = - (a_12_01 * sal_12(k - 1)) + (b_12_01 * 0 + b_12_02 * 0);
            else            % (k - 1) >= 1,               (k - d) >= 1
                sal_12(k) = - (a_12_01 * sal_12(k - 1)) + (b_12_01 * in(k - d) + b_12_02 * 0);
            end
        else            % (k - 1) >= 1,               (k - d) >= 1,          (k - d - 1) >= 1
            sal_12(k) = - (a_12_01 * sal_12(k - 1)) + (b_12_01 * in(k - d) + b_12_02 * in(k - d - 1));
        end
    end
    % --------------------------------- %
    % Caso 21
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_21(k) = 0;
    elseif (k - 2) == 0 
        if (k - d) <= 0 % (k - 1) >= 1
            sal_21(k) = - (a_21_01 * sal_21(k - 1) + a_21_02 * 0) + (b_21_01 * 0);
        else            % (k - 1) >= 1,                             (k - d) >= 1
            sal_21(k) = - (a_21_01 * sal_21(k - 1) + a_21_02 * 0) + (b_21_01 * in(k - d));
        end
    else            
        if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1
            sal_21(k) = - (a_21_01 * sal_21(k - 1) + a_21_02 * sal_21(k - 2)) + (b_21_01 * 0);
        else            % (k - 1) >= 1,              (k - 2) >= 1,              (k - d) >= 1
            sal_21(k) = - (a_21_01 * sal_21(k - 1) + a_21_02 * sal_21(k - 2)) + (b_21_01 * in(k - d));
        end
    end
    % --------------------------------- %
    % Caso 22
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_22(k) = 0;
    elseif (k - 2) == 0 
        if (k - d - 1) <= 0
            if (k - d) <= 0 % (k - 1) >= 1
                sal_22(k) = - (a_22_01 * sal_22(k - 1) + a_22_02 * 0) + (b_22_01 * 0 + b_22_02 * 0);
            else            % (k - 1) >= 1,                             (k - d) >= 1
                sal_22(k) = - (a_22_01 * sal_22(k - 1) + a_22_02 * 0) + (b_22_01 * in(k - d) + b_22_02 * 0);
            end
        else            % (k - 1) >= 1,                             (k - d) >= 1,          (k - d - 1) >= 1
            sal_22(k) = - (a_22_01 * sal_22(k - 1) + a_22_02 * 0) + (b_22_01 * in(k - d) + b_22_02 * in(k - d - 1));
        end
    else            
        if (k - d - 1) <= 0
            if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1
                sal_22(k) = - (a_22_01 * sal_22(k - 1) + a_22_02 * sal_22(k - 2)) + (b_22_01 * 0 + b_22_02 * 0);
            else            % (k - 1) >= 1,              (k - 2) >= 1,               (k - d) >= 1
                sal_22(k) = - (a_22_01 * sal_22(k - 1) + a_22_02 * sal_22(k - 2)) + (b_22_01 * in(k - d) + b_22_02 * 0);
            end
        else            % (k - 1) >= 1,              (k - 2) >= 1,               (k - d) >= 1,          (k - d - 1) >= 1
            sal_22(k) = - (a_22_01 * sal_22(k - 1) + a_22_02 * sal_22(k - 2)) + (b_22_01 * in(k - d) + b_22_02 * in(k - d - 1));
        end
    end   
    % --------------------------------- %
    % Caso 23
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_23(k) = 0;
    elseif (k - 2) == 0 
        if (k - d - 2) <= 0
            if (k - d - 1) <= 0
                if (k - d) <= 0 % (k - 1) >= 1
                    sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * 0) + (b_23_01 * 0 + b_23_02 * 0 + b_23_03 * 0);
                else            % (k - 1) >= 1,                             (k - d) >= 1
                    sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * 0) + (b_23_01 * in(k - d) + b_23_02 * 0 + b_23_03 * 0);
                end
            else            % (k - 1) >= 1,                             (k - d) >= 1,          (k - d - 1) >= 1
                sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * 0) + (b_23_01 * in(k - d) + b_23_02 * in(k - d - 1) + b_23_03 * 0);
            end
        else            % (k - 1) >= 1,                             (k - d) >= 1,          (k - d - 1) >= 1,         (k - d - 2) >= 1
            sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * 0) + (b_23_01 * in(k - d) + b_23_02 * in(k - d - 1) + b_23_03 * in(k - d - 2));
        end
    else
        if (k - d - 2) <= 0
            if (k - d - 1) <= 0
                if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1
                    sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * sal_23(k - 2)) + (b_23_01 * 0 + b_23_02 * 0 + b_23_03 * 0);
                else            % (k - 1) >= 1,              (k - 2) >= 1,              (k - d) >= 1
                    sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * sal_23(k - 2)) + (b_23_01 * in(k - d) + b_23_02 * 0 + b_23_03 * 0);
                end
            else            % (k - 1) >= 1,              (k - 2) >= 1,              (k - d) >= 1,         (k - d - 1) >= 1
                sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * sal_23(k - 2)) + (b_23_01 * in(k - d) + b_23_02 * in(k - d - 1) + b_23_03 * 0);
            end
        else            % (k - 1) >= 1,              (k - 2) >= 1,              (k - d) >= 1,          (k - d - 1) >= 1,         (k - d - 2) >= 1
            sal_23(k) = - (a_23_01 * sal_23(k - 1) + a_23_02 * sal_23(k - 2)) + (b_23_01 * in(k - d) + b_23_02 * in(k - d - 1) + b_23_03 * in(k - d - 2));
        end
    end  
    % --------------------------------- %
    % Caso 32
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_32(k) = 0;
    elseif (k - 2) == 0 
        if (k - d - 1) <= 0
            if (k - d) <= 0 % (k - 1) >= 1
                sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * 0 + a_32_03 * 0) + (b_32_01 * 0 + b_32_02 * 0);
            else            % (k - 1) >= 1,                                           (k - d) >= 1
                sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * 0 + a_32_03 * 0) + (b_32_01 * in(k - d) + b_32_02 * 0);
            end
        else            % (k - 1) >= 1,                                           (k - d) >= 1,          (k - d - 1) >= 1
            sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * 0 + a_32_03 * 0) + (b_32_01 * in(k - d) + b_32_02 * in(k - d - 1));
        end
    elseif (k - 3) == 0            
        if (k - d - 1) <= 0
            if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1
                sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * sal_32(k - 2) + a_32_03 * 0) + (b_32_01 * 0 + b_32_02 * 0);
            else            % (k - 1) >= 1,              (k - 2) >= 1,                            (k - d) >= 1
                sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * sal_32(k - 2) + a_32_03 * 0) + (b_32_01 * in(k - d) + b_32_02 * 0);
            end
        else            % (k - 1) >= 1,              (k - 2) >= 1,                            (k - d) >= 1,          (k - d - 1) >= 1
            sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * sal_32(k - 2) + a_32_03 * 0) + (b_32_01 * in(k - d) + b_32_02 * in(k - d - 1));
        end
    else
        if (k - d - 1) <= 0
            if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1
                sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * sal_32(k - 2) + a_32_03 * sal_32(k - 3)) + (b_32_01 * 0 + b_32_02 * 0);
            else            % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1,              (k - d) >= 1
                sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * sal_32(k - 2) + a_32_03 * sal_32(k - 3)) + (b_32_01 * in(k - d) + b_32_02 * 0);
            end
        else            % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1,              (k - d) >= 1,          (k - d - 1) >= 1
            sal_32(k) = - (a_32_01 * sal_32(k - 1) + a_32_02 * sal_32(k - 2) + a_32_03 * sal_32(k - 3)) + (b_32_01 * in(k - d) + b_32_02 * in(k - d - 1));
        end
    end  
    % --------------------------------- %
    % Caso 33
    % --------------------------------- %
    if (k - 1) == 0 % k <= d, por lo que no hace falta comprobar (k - d <= 0) ya que siempre lo será
        sal_33(k) = 0;
    elseif (k - 2) == 0 
        if (k - d - 2) <= 0
            if (k - d - 1) <= 0
                if (k - d) <= 0 % (k - 1) >= 1
                    sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * 0 + a_33_03 * 0) + (b_33_01 * 0 + b_33_02 * 0 + b_33_03 * 0);
                else            % (k - 1) >= 1,                                           (k - d) >= 1
                    sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * 0 + a_33_03 * 0) + (b_33_01 * in(k - d) + b_33_02 * 0 + b_33_03 * 0);
                end
            else            % (k - 1) >= 1,                                           (k - d) >= 1,          (k - d - 1) >= 1
                sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * 0 + a_33_03 * 0) + (b_33_01 * in(k - d) + b_33_02 * in(k - d - 1) + b_33_03 * 0);
            end
        else            % (k - 1) >= 1,                                           (k - d) >= 1,          (k - d - 1) >= 1,         (k - d - 2) >= 1
            sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * 0 + a_33_03 * 0) + (b_33_01 * in(k - d) + b_33_02 * in(k - d - 1) + b_33_03 * in(k - d - 2));
        end
    elseif (k - 3) == 0
        if (k - d - 2) <= 0
            if (k - d - 1) <= 0
                if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1
                    sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * 0) + (b_33_01 * 0 + b_33_02 * 0 + b_33_03 * 0);
                else            % (k - 1) >= 1,              (k - 2) >= 1,                            (k - d) >= 1
                    sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * 0) + (b_33_01 * in(k - d) + b_33_02 * 0 + b_33_03 * 0);
                end
            else            % (k - 1) >= 1,              (k - 2) >= 1,                            (k - d) >= 1,          (k - d - 1) >= 1
                sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * 0) + (b_33_01 * in(k - d) + b_33_02 * in(k - d - 1) + b_33_03 * 0);
            end
        else            % (k - 1) >= 1,              (k - 2) >= 1,                            (k - d) >= 1,          (k - d - 1) >= 1,         (k - d - 2) >= 1
            sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * 0) + (b_33_01 * in(k - d) + b_33_02 * in(k - d - 1) + b_33_03 * in(k - d - 2));
        end
    else
        if (k - d - 2) <= 0
            if (k - d - 1) <= 0
                if (k - d) <= 0 % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1
                    sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * sal_33(k - 3)) + (b_33_01 * 0 + b_33_02 * 0 + b_33_03 * 0);
                else            % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1,              (k - d) >= 1
                    sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * sal_33(k - 3)) + (b_33_01 * in(k - d) + b_33_02 * 0 + b_33_03 * 0);
                end
            else            % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1,              (k - d) >= 1,          (k - d - 1) >= 1
                sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * sal_33(k - 3)) + (b_33_01 * in(k - d) + b_33_02 * in(k - d - 1) + b_33_03 * 0);
            end
        else            % (k - 1) >= 1,              (k - 2) >= 1,             (k - 3) >= 1,              (k - d) >= 1,          (k - d - 1) >= 1,         (k - d - 2) >= 1
            sal_33(k) = - (a_33_01 * sal_33(k - 1) + a_33_02 * sal_33(k - 2) + a_33_03 * sal_33(k - 3)) + (b_33_01 * in(k - d) + b_33_02 * in(k - d - 1) + b_33_03 * in(k - d - 2));
        end
    end 
    % --------------------------------- % 
    k = k + 1;
end

% ----------------------------------------------------------------------- %
% VISUALIZACIÓN DE LOS RESULTADOS                                         %
% ----------------------------------------------------------------------- %
% Señales de salida obtenidas
% --------------------------------- %
salidas = {sal_11, sal_12, sal_21, sal_22, sal_23, sal_32, sal_33}; % Nombres de las variables
nombres = {'Salida caso 11', 'Salida caso 12', 'Salida caso 21', 'Salida caso 22', 'Salida caso 23', 'Salida caso 32', 'Salida caso 33'};  % Nombres en leyenda
% --------------------------------- %
% Visualización de las señales individualmente junto a la respuesta ante escalón
% --------------------------------- %
for i = 1:length(salidas)
    % --------------------------------- %
    % Abrir la figura proporcionada
    % --------------------------------- %
    open('8806_respuesta_ante_escalon.fig'); 
    hold on; % Mantener la figura para dibujar la curva sobre esta
    % --------------------------------- %
    % Graficar la salida correspondiente
    % --------------------------------- %
    hi = plot(tt, salidas{i}, 'g', 'LineWidth', 1.5);
    % --------------------------------- %
    % Añadir la leyenda
    % --------------------------------- %
    legend(hi, {nombres{i}}, 'Location', 'best');
    % --------------------------------- %
    % Ajustar título y ejes
    % --------------------------------- %
    title(['Comparación de Entrada con ' nombres{i}]);
    xlabel('Tiempo');
    ylabel('Valor de Salida');
    grid on;
    % --------------------------------- %
    % Forzar actualización de la figura
    % --------------------------------- %
    drawnow;
end

% ----------------------------------------------------------------------- %
% CONCLUSIONES TRAS ANALIZAR LOS RESULTADOS                               %
% ----------------------------------------------------------------------- %
% El mejor caso observable es el 22 con na = 2, nb = 2
% El cual obtiene como resultado:
% theta_22 =
%    -0.7719 (a_22_01)
%     0.1747 (a_22_02)
%     0.6341 (b_22_01)
%     0.4540 (b_22_02)
% ----------------------------------------------------------------------- %