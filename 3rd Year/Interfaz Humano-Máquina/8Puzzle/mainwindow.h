#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

// Includes necesarios para el arrastre de los botones
#include <QDrag>
#include <QDropEvent>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void confirmResetGame();  // Función para confirmar el reinicio
    void enableButtons(); // Función para activar los botones
    void resetGame(); // Slot que se conecta al botón de reset

    // Funciones de manejo del arrastre
    void handleFichaPressed();
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::MainWindow *ui;

    QLabel *labelMoves; // Referencia al label de movimientos
    int moves; // Variable que guarda el número de movimientos

    QPushButton *buttons[8]; // Vector para referenciar a los botones en un for
    QPushButton *emptyButton; // Botón sin texto
    int emptyButtonRow, emptyButtonCol; // Posición del botón vacío

    QGridLayout *gridLayout; // Referencia al gridLayout

    void updateMoves();
    void randomizeButtons();
    bool isSolvable();

    bool gameStarted; // Variable para comprobar si es la primera vez que se inicia el juego
    bool notDragged; // Variable para comprobar si se activa el estado de arrastre

    // Funciones de adquisición y comprobación de la posición del botón presionado
    bool isAdjacentToEmpty(int buttonRow, int buttonCol);
    void getButtonPosition(QPushButton *button, int &row, int &col);
    bool isMouseInEmptyButton(QDropEvent *event);

    QPushButton *shadowButton; // Botón que simula la posición futura del botón
    QString shadowStyle; // Estilo del botón transparente

    // Función que permite previsualizar la posición de drop del botón
    void showShadow(QString text, bool visible);

    // Función para comprobar la condición de victoria
    bool checkWinCondition();
};

#endif // MAINWINDOW_H

