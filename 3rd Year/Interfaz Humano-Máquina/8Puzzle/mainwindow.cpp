#include "mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <algorithm> // std::random_shuffle
#include <cstdlib> // std::rand, std::srand
#include <ctime> // std::time

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , moves(0)
    , gameStarted(false)
    , notDragged(true)
    , shadowButton(new QPushButton(this))
    , shadowStyle("background-color: rgba(168, 168, 240, 150); color: #000000; border-radius: 5px; padding: 5px;")
{
    ui->setupUi(this);

    QFile file(":/estilos/estilos.qss"); // Ruta al archivo en el recurso
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = QString::fromUtf8(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }

    // Inicializar el randomizador
    std::srand(std::time(nullptr));

    // Vincular los elementos de la ventana principal con el código
    labelMoves = ui->counter;

    buttons[0] = ui->Ficha1;
    buttons[1] = ui->Ficha2;
    buttons[2] = ui->Ficha3;
    buttons[3] = ui->Ficha4;
    buttons[4] = ui->Ficha5;
    buttons[5] = ui->Ficha6;
    buttons[6] = ui->Ficha7;
    buttons[7] = ui->Ficha8;
    emptyButton = ui->emptyButton;

    gridLayout = ui->gridLayout;

    // Permitir que los botones sean manipulados y conectarlos a la función manejadora
    for (int i = 0; i < 8; i++)
    {
        buttons[i]->setAcceptDrops(true);
        connect(buttons[i], &QPushButton::pressed, this, &MainWindow::handleFichaPressed);
    }
    emptyButton->setAcceptDrops(true);

    // Conectar el botón "Start" con la función de reinicio
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::confirmResetGame);

    // Inicializar el botón transparente "sombra"
    shadowButton->setVisible(false); // Ocultar el botón inicialmente
    shadowButton->setStyleSheet(shadowStyle); // Color base del botón (lila) con transparencia
    shadowButton->setEnabled(false); // Botón no interactivo
    shadowButton->setFont(QFont("Bell MT", 24)); // Ajustar la fuente del botón
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Función que permite al usuario decidir si quiere reiniciar o no el juego
void MainWindow::confirmResetGame()
{
    // Comprueba si el juego se está iniciando por primera vez
    if (gameStarted)
    {
        // Muestra un mensaje preguntando si se quiere reiniciar el juego
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmar Reinicio", "¿Seguro quieres reiniciar el juego?", QMessageBox::Yes|QMessageBox::No);

        // Si se desea reiniciar, se llama a la función de reinicio
        if (reply == QMessageBox::Yes)
        {
            resetGame();
        }
    }

    // Si es la primera vez que se inicia, se cambia el texto del botón y se inicia el juego
    else
    {
        ui->resetButton->setText("Restart");
        resetGame();
    }
}

// Función que activa los botones (Fichas) para que puedan manipularse
void MainWindow::enableButtons()
{
    for (int i = 0; i < 8; i++)
    {
        buttons[i]->setEnabled(true);
    }
}

// Función de reinicio del juego
void MainWindow::resetGame()
{
    randomizeButtons();

    while (!isSolvable())
    {
        randomizeButtons();
    }

    moves = 0;
    updateMoves();

    // Establecemos que el juego se ha iniciado
    gameStarted = true;

    // Activamos los botones
    enableButtons();
}

void MainWindow::updateMoves()
{
    labelMoves->setText("Steps: " + QString::number(moves));
}

void MainWindow::randomizeButtons()
{
    // Obtener una lista de las posiciones posibles en el grid
    QList<QPair<int, int>> positions;

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            positions.append(qMakePair(row, col));
        }
    }

    // Mezclar las posiciones aleatoriamente
    std::random_shuffle(positions.begin(), positions.end());

    // Asignar los botones a las posiciones aleatorias
    for (int i = 0; i < 8; i++)
    {
        gridLayout->addWidget(buttons[i], positions[i].first, positions[i].second);
    }

    // Asignar la posición del hueco vacío
    emptyButtonRow = positions[8].first;
    emptyButtonCol = positions[8].second;
    gridLayout->addWidget(emptyButton, emptyButtonRow, emptyButtonCol);
}

bool MainWindow::isSolvable()
{
    // Crear una lista con los números de las piezas, omitiendo el hueco vacío
    QList<int> list;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // Obtener el QLayoutItem en la posición 'i'
            QLayoutItem *item = gridLayout->itemAtPosition(i, j);

            // Comprobar si el QLayoutItem tiene un widget asociado
            if (item->widget())
            {
                // Obtener el widget del item
                QWidget *widget = item->widget();

                // Saber el tipo de widget
                if (QPushButton *button = qobject_cast<QPushButton *>(widget))
                {
                    if (button != emptyButton)
                    {
                        list.append(button->text().toInt());
                    }
                }
            }
        }
    }

    // Calcular el número de inversiones
    int inversions = 0;

    for (int i = 0; i < list.size() - 1; i++)
    {
        for (int j = i + 1; j < list.size(); j++)
        {
            if (list[i] > list[j])
            {
                inversions++;
            }
        }
    }

    // Un puzzle es resoluble si el número de inversiones es par
    return (inversions % 2 == 0);
}

// Función de inicio de la operación de arrastre al presionar un botón
void MainWindow::handleFichaPressed()
{
    // Detectar el botón presionado
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    if (button)
    {
        // Crear datos MIME con el texto del botón como dato
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("boton_activo", button->text().toUtf8());

        // Iniciar la operación de arrastre
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setHotSpot(QPoint(16, 16)); // Ajustar al centro
        drag->exec(Qt::MoveAction);
    }

    // Comprobar si el botón está siendo pulsado y no arrastrado
    if (notDragged) // Se pone en false en el drag (inicio del arrastre) y se pone en true en el drop (fin del arrastre)
    {
        for (int i = 0; i < 8; i++)
        {
            buttons[i]->setDown(false);
        }
    }
}

// Función que verifica si la operación de arrastre puede ser aceptada
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // Aceptar el evento si contiene el tipo MIME correcto
    if (event->mimeData()->hasFormat("boton_activo"))
    {
        event->acceptProposedAction();

        // Establece que se ha iniciado el arrastre
        notDragged = false;
    }
}

// Función que maneja aquello que sucede mientras se mueve la ficha
void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    // Verifica que el evento de arrastre contenga el formato correcto
    if (event->mimeData()->hasFormat("boton_activo"))
    {
        // Se extrae el contenido del evento (texto del botón arrastrado)
        QByteArray buttonData = event->mimeData()->data("boton_activo");
        QString buttonText = QString::fromUtf8(buttonData);

        // Verificar si el ratón está sobre el espacio vacío
        if (isMouseInEmptyButton(event))
        {
            for (int i = 0; i < 8; i++)
            {
                if (buttons[i]->text() == buttonText)
                {
                    // Se obtiene la posición del botón
                    int buttonRow, buttonCol;
                    getButtonPosition(buttons[i], buttonRow, buttonCol);

                    // Comprobar si el botón está adyacente al espacio vacío
                    if (isAdjacentToEmpty(buttonRow, buttonCol))
                    {
                        // Mostrar la sombra
                        showShadow(buttonText, true);
                    }
                    else
                    {
                        // Mostrar la sombra de color rojo
                        showShadow("ERROR" + buttonText, true);
                    }
                }
            }
        }

        else
        {
            // Ocultar la sombra
            showShadow("", false);
        }

        event->acceptProposedAction();
    }
}

// Función que maneja lo que sucede cuando se suelta el elemento arrastrado
void MainWindow::dropEvent(QDropEvent *event)
{
    // Verifica que el evento de arrastre contenga el formato correcto
    if (event->mimeData()->hasFormat("boton_activo"))
    {
        // Se extrae el contenido del evento (texto del botón arrastrado)
        QByteArray buttonData = event->mimeData()->data("boton_activo");
        QString buttonText = QString::fromUtf8(buttonData);

        // Se recorre la lista de botones en busca del que coincide con el texto
        for (int i = 0; i < 8; i++)
        {
            if (buttons[i]->text() == buttonText)
            {
                // Se obtiene la posición del botón
                int buttonRow, buttonCol;
                getButtonPosition(buttons[i], buttonRow, buttonCol);

                // Se obtiene el estilo original del botón
                QString originalStyle = buttons[i]->styleSheet();

                // Comprobar si el botón está adyacente al espacio vacío
                if (isAdjacentToEmpty(buttonRow, buttonCol))
                {
                    // Comprobar si el ratón está en la fila/columna del botón vacío
                    if (isMouseInEmptyButton(event))
                    {
                        // Crear una animación para el movimiento de los botones
                        QPropertyAnimation *animation = new QPropertyAnimation(buttons[i], "geometry");
                        animation->setDuration(300); // Duración de la animación en milisegundos

                        // Obtener la geometría del botón vacío
                        QRect emptyButtonSpace = emptyButton->geometry();

                        // Definir el inicio y el final de la animación
                        animation->setStartValue(buttons[i]->geometry());
                        animation->setEndValue(emptyButtonSpace); // Termina al llegar al espacio vacío

                        // Cambiar color del botón durante la animación
                        buttons[i]->setStyleSheet("background-color: #6a6ad8; border-radius: 5px; padding: 5px;");

                        // Conectar la finalización de la animación y actualizar la posición del botón y espacio vacío
                        connect(animation, &QPropertyAnimation::finished, this, [this, buttonRow, buttonCol, i, originalStyle]()
                        {
                            // Eliminar ambos widgets de sus posiciones
                            gridLayout->removeWidget(buttons[i]);
                            gridLayout->removeWidget(emptyButton);

                            // Colocar el botón en la posición del espacio vacío
                            gridLayout->addWidget(buttons[i], emptyButtonRow, emptyButtonCol);
                            // Volvemos a cambiar el color del botón para evitar que se muestre gris
                            buttons[i]->setStyleSheet("background-color: #6a6ad8; border-radius: 5px; padding: 5px;");

                            // Colocar el widget del espacio vacío en la posición original del botón
                            gridLayout->addWidget(emptyButton, buttonRow, buttonCol);

                            // Actualizar la posición del espacio vacío
                            emptyButtonRow = buttonRow;
                            emptyButtonCol = buttonCol;

                            // Aumentar el conteo de movimientos y actualizar el label
                            moves++;
                            updateMoves();

                            // Comprobar si se ha llegado al estado final
                            if (checkWinCondition())
                            {
                                // Mostrar un mensaje de victoria
                                QMessageBox::information(this, "¡Ganaste!", "Enhorabuena, has completado el puzzle");

                                // Consultar si se desea reiniciar el juego o cerrar la ventana
                                QMessageBox::StandardButton reply;
                                reply = QMessageBox::question(this, "Confirmar Reinicio", "¿Quieres reiniciar el juego?", QMessageBox::Yes|QMessageBox::No);

                                // Si se desea reiniciar, se activa la función de reinicio de juego
                                if (reply == QMessageBox::Yes)
                                {
                                    resetGame();
                                }

                                // Si no se desea reiniciar, se cierra el juego tras mostrar un aviso
                                else
                                {
                                    QMessageBox::information(this, "Cerrando el juego...", "¡Gracias por jugar a 8Puzzle!");
                                    this->close();
                                }
                            }

                            // Reestablecer el estilo original del botón
                            buttons[i]->setStyleSheet(originalStyle);
                        });

                        // Iniciar la animación
                        animation->start();
                    }
                }

                // Si el botón se suelta en una casilla no adyacente
                else
                {
                    // Si el ratón está sobre el hueco vacío, muestra un mensaje de advertencia
                    if (isMouseInEmptyButton(event))
                    {
                        QMessageBox::warning(this, "¡Movimiento inválido!", "Solo se pueden mover las fichas que sean adyacentes al hueco vacío.");
                    }
                }

                // Finaliza el arrastre
                event->acceptProposedAction();
                notDragged = true;

                // Forzar el botón a estar "no presionado"
                buttons[i]->setDown(false);

                // Ocultar el botón transparente
                showShadow("", false);

                // Salir del bucle después de encontrar y mover el botón
                break;
            }
        }
    }
}

// Obtener la posición del botón dentro del grid 3x3
void MainWindow::getButtonPosition(QPushButton *button, int &row, int &col)
{
    for (row = 0; row < 3; row++)
    {
        for (col = 0; col < 3; col++)
        {
            // La función devuelve el widget en la posición (row, col)
            // Si coincide con el botón que se busca, la función termina
            if (gridLayout->itemAtPosition(row, col)->widget() == button)
            {
                return;
            }
        }
    }

    // Valor por defecto en caso de no encontrar el botón
    row = -1; col = -1;
}

// Devulve true si el botón está adyacente al espacio vacío
bool MainWindow::isAdjacentToEmpty(int buttonRow, int buttonCol)
{
    // Comprobar si el botón está arriba o abajo del espacio vacío
    bool isAboveOrBelow = (abs(buttonRow - emptyButtonRow) == 1) && (buttonCol == emptyButtonCol);

    // Comprobar si está a la izquierda o derecha del espacio vacío
    bool isLeftOrRight = (abs(buttonCol - emptyButtonCol) == 1) && (buttonRow == emptyButtonRow);

    return isAboveOrBelow || isLeftOrRight;
}

// Verifica si el ratón está sobre el botón vacío
bool MainWindow::isMouseInEmptyButton(QDropEvent *event)
{
    // Obtener la posición del ratón desde el evento drop
    QPointF mousePos = event->position();

    // Obtener la geometría del botón vacío
    QRect emptyButtonSpace = emptyButton->geometry();

    // Verificar si la posición del ratón está dentro de los límites del botón vacío
    return emptyButtonSpace.contains(mousePos.toPoint());
}

// Visualiza el botón que está siendo arrastrado en la posición vacía de forma semitransparente
void MainWindow::showShadow(QString text, bool visible)
{
    // Si el ratón está en el hueco vacío visible es true
    if (visible)
    {
        // Si el ratón no es adyacente
        if (text.startsWith("ERROR"))
        {
            // Se elimina el texto "ERROR" para poder mostrar el número de la ficha
            text.remove("ERROR");

            // Se establece el color del botón como rojo
            shadowButton->setStyleSheet("background-color: rgba(255, 99, 71, 150); color: #000000; border-radius: 5px; padding: 5px;");
        }

        // Si el botón es adyacente
        else
        {
            shadowButton->setStyleSheet(shadowStyle);
        }

        // Asigna el texto del botón a desplazar
        shadowButton->setText(text);

        // Se coloca en el lugar del hueco vacío
        shadowButton->setGeometry(emptyButton->geometry());
    }

    // Se muestra u oculta el botón dependiendo de la variable
    shadowButton->setVisible(visible);
}

// Devuelve true si todas las fichas se encuentran en la posición adecuada
bool MainWindow::checkWinCondition()
{
    // Se establece el primer número de ficha a buscar "1"
    int expectedNumber = 1;

    // Se recorre todo el grid 3x3
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            // Obtener el item en la posición (fila, columna)
            QLayoutItem *item = gridLayout->itemAtPosition(row, col);

            // Comprueba si el item en la posición en cuestión es un QPushButton
            if (QPushButton *button = qobject_cast<QPushButton *>(item->widget()))
            {
                // Si el botón se encuentra en la última posición y no es el vacío, devuelve false
                if (row == 2 && col == 2)
                {
                    if (button != emptyButton)

                    return false;
                }

                // Si el texto del botón no coincide con el número esperado, devuelve false
                else
                {
                    if (button->text().toInt() != expectedNumber)
                    {
                        return false;
                    }

                    // Incrementar el número esperado para la próxima pieza
                    expectedNumber++;
                }
            }
        }
    }
    // Si se verifica que todas las piezas están en el orden correcto, devuelve true
    return true;
}
