% *********************************************************************** %
% ident.m                                                                 %
%                                                                         %
% Este código permite obtener el valor de theta teniendo en cuenta los    %
% parámetros proporcionados                                               %
% *********************************************************************** %
% Autores/as - Correo:                                                    %
%                                                                         %
%   Ángela Espert Cornejo   - aespcor@etsinf.upv.es                       %
%   Lourdes Francés Llimerá - lfralli@epsa.upv.es                         %
% *********************************************************************** %

function [theta] = ident(y, u, na, nb, d, H) 
    % ------------------------------------------------------------------- %
    % INICIALIZACIÓN DE VALORES                                           %
    % ------------------------------------------------------------------- %
    % Crear vector Y
    % --------------------------------- %
    Y = [];
    % --------------------------------- %
    % Crear matriz fi (Φ)
    % --------------------------------- %
    fi = [];
    % --------------------------------- %
    % Crear matriz theta (θ)
    % --------------------------------- %
    theta = [];
    % --------------------------------- %
    % Calcular N
    % --------------------------------- %
    N = length(y);
    % --------------------------------- %
    % Calcular M
    % --------------------------------- %
    M = H + 1 + max(na, nb - 1 + d);
    % --------------------------------- %
    
    % ------------------------------------------------------------------- %
    % ASIGNACIÓN DE VALORES AL SUBVECTOR Y                                %
    % ------------------------------------------------------------------- %    
    % Crea el subvector desde M hasta el final del vector de y
    % --------------------------------- %
    for i = 0 : N - M
        Y(i + 1, 1) = y(M + i);
    end
    % --------------------------------- %    

    % ------------------------------------------------------------------- %
    % ASIGNACIÓN DE VALORES A LA MATRIZ FI (Φ)                            %
    % ------------------------------------------------------------------- %    
    % Columna auxiliar col
    % --------------------------------- %
    col = []; 
    % --------------------------------- %
    % Rellenar primera parte de la matriz fi:
    % --------------------------------- %
    for i = 1 : na               % i columnas
        for j = 0 : N - M        % j filas
            col(j + 1, 1) = -y((M + j) - i);
        end
        if i == 1
            % Si fi está vacía añadir la columna col
            fi = col;
        else
            % Si fi ya tiene columnas, añadir col a la derecha de fi
            fi = [fi col];
        end
    end
    % --------------------------------- %
    % Vaciar contenido de col
    % --------------------------------- %
    col = [];
    % --------------------------------- %
    % Rellenar segunda parte de la matriz fi:    
    % --------------------------------- %
    for i = 0 : nb - 1           % i columnas
        for j = 0 : N - M        % j filas
            col(j + 1, 1) = u((M + j) - d - i);
        end
        % Como fi ya tiene columnas, añadir col a la derecha de fi
        fi = [fi col];
    end
    % --------------------------------- %

    % ------------------------------------------------------------------- %
    % ASIGNACIÓN DE VALORES A LA MATRIZ THETA (θ)                         %
    % ------------------------------------------------------------------- %    
    theta = inv(fi' * fi) * fi' * Y % Sin ; para que se imprima
    % --------------------------------- %
end
