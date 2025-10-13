/**
 * @file     gameboard.cpp
 *
 * @brief    Implementation of GameBoard class functions.
 *
 * @version  0.5  (2024/01/24) Final version
 * @version  0.4  (2024/01/23) BARR-C programming standard needs to be revised
 * @version  0.3  (2024/12/27) Inserted GameBoard into a gridLayout
 * @version  0.2  (2024/12/16) Applying BARR-C programming standard
 * @version  0.1  (2024/12/16) Teacher's example project
 *
 * @authors  Marcos Belda Martinez' <mbelmar@etsinf.upv.es>,
 *           Angela Espert Cornejo' <aespcor@etsinf.upv.es>,
 *           Lourdes Frances Llimera' <lfralli@epsa.upv.es>
 *
 * @warning  For the creation of this source code, the Ecosia AI Chat tool has
 *           been used as a source of knowledge and help in the implementation
 *           of some functions.
 *
 * @date     January, 2025
 * @section  IHM-GIIROB
 */

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QMessageBox>
#include <QPainter>
#include <QImage>
#include <QPainterPath>
#include <QBrush>
#include <QPixmap>
#include <cstdlib> // rand() and srand()
#include <ctime>   // time()

/// Own includes
#include "gameboard.h"
#include "ui_gameboard.h"
#include "profilesidemenu.h"
#include "mainwindow.h"
#include "newgamewindow.h"
#include "connect4.h"
#include "mainmenu.h"


//-----[ IMPLEMENTATION OF PUBLIC GAMEBOARD FUNCTIONS ]-----------------------//

/******************************************************************************/
/*!
 * @brief   GameBoard class constructor implementation.
 * @param   parent  Used to set the parent widget of GameBoard.
 */
GameBoard::GameBoard(QWidget * parent)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::GameBoard)
    , sideMenu(new ProfileSideMenu(this))
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
    //, currentTheme(UNKNOWN_THEME)
    // boardSize
    // rows
    // cols
    // boardPath
    // bluredBoardPath
    // gameMode
    // CPU_on
    // amountOfRounds
    , currentRound(1)
    , userInputEnabled(true)
    // winningPoints
    // finalWinner
    // finalLoser
    // winner
    // loser
    // player1Name
    // player2Name
    // finalWinnerName
    // finalLoserName
    // circle1Color
    // circle2Color
    // points1
    // points2
    // espacioJuegoWidth
    // espacioJuegoHeight
    // cellSize
    // x0
    // y0
    , previewColumn(-1)
    , previewActive(false)
    , currentPlayer(1)
    // grid
    , isAnimating(false)
    , animColumn(-1)
    , animRow(-1)
    , animY(-1)
    , animColor(-1)
    // animationTimer
    // animationTimerCPU
{
    ui->setupUi(this);

    // Initialize the random generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Set icons to buttons and connect them
    setIcons();
    connectButtons();

    // Set the icon for the profile button
    ui->profileButton->setIcon(
        QPixmap::fromImage(mainwindow->player1->getAvatar()));

    initializeGameBoard();

    grid.resize(rows, QVector<int>(cols, 0));
    setMinimumSize(cols * 40, rows * 40);

    // Access the boardView
    ui->stackedWidget->setCurrentWidget(ui->boardView);

    // Connect the timers
    connect(
        &animationTimer
        , &QTimer::timeout
        , this
        , &GameBoard::startAnimation);

    connect(
        &animationTimerCPU
        , &QTimer::timeout
        , this
        , &GameBoard::startAnimationCPU);

    connect(
        mainwindow
        , &MainWindow::retranslateUIs
        , this
        , [=](void) { ui->retranslateUi(this); });

}   /* GameBoard() */

/******************************************************************************/
/*!
 * @brief  Implementation of the GameBoard class destructor.
 * @param  void
 */
GameBoard::~GameBoard()
{
    delete ui;

    // Check if sideMenu is not null before deleting
    if (sideMenu)
    {
        delete sideMenu;
    }

}   /* ~GameBoard() */


//-----[ IMPLEMENTATION OF PROTECTED GAMEBOARD FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   Implementation of the board painter function.
 * @param   event  Receives the current QPaintEvent.
 * @return  void
 */
void GameBoard::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (ui->stackedWidget->currentWidget() == ui->boardView)
    {
        // Draw background
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::transparent);

        // Only if sideMenu is not visible
        QPen circle_pen(Qt::DotLine);

        // Calculation of the game area (embedding the board in a widget)
        espacioJuegoWidth = ui->GameBoardGrid->width();
        espacioJuegoHeight = ui->GameBoardGrid->height();
        // Calculation of the top margin (to display the preview disc)
        float topMargin = (espacioJuegoHeight / rows) * 1.2;
        espacioJuegoHeight -= topMargin;

        // Calculation of the cell size
        if (espacioJuegoWidth / cols < espacioJuegoHeight / rows)
        {
            cellSize = espacioJuegoWidth / cols;
        }
        else
        {
            cellSize = espacioJuegoHeight / rows;
        }

        /// Calculation of the initial position (x0, y0) to center the board
        // Origin x of GameBoardGrid plus half of the difference between the
        // total width of the widget and the current width of the board
        x0 = (ui->GameBoardGrid->pos().x() +
              ((espacioJuegoWidth - (cellSize * cols)) / 2));

        // Origin y of GameBoardGrid plus half of the difference between the
        // total height of the widget and the current height of the board
        y0 = (ui->GameBoardGrid->pos().y() +
              ((espacioJuegoHeight - (cellSize * rows)) / 2) + topMargin);

        // Draw the background
        QColor previewColor;

        if (mainwindow->currentTheme == DARK_THEME)
        {
            previewColor = QColor(42, 42, 42);
        }
        else
        {
            previewColor = QColor(213, 213, 213);
        }

        painter.setBrush(previewColor);
        painter.drawRect(0, 0, width(), height());

        // Draw cells
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                // Draw a circle for each cell
                QRectF circleRect(x0 + (c * cellSize + (cellSize * 0.11)),
                                  y0 + (r * cellSize + (cellSize * 0.11)),
                                  cellSize - (cellSize * 0.22),
                                  cellSize - (cellSize * 0.22));

                QColor previewColor;
                QPen pen;

                if (grid[r][c] == 0)
                {
                    previewColor = Qt::transparent;

                    pen.setColor(previewColor);
                    pen.setWidth(cellSize / 9);
                    painter.setPen(pen);
                    painter.setBrush(previewColor);

                }
                else if (grid[r][c] == 1)
                {
                    previewColor = circle1Color;

                    if (sideMenu->isMenuVisible())
                    {
                        previewColor.setAlpha(20);
                    }

                    pen.setColor(previewColor.darker(150));
                    pen.setWidth(cellSize / 9);
                    painter.setPen(pen);
                    painter.setBrush(previewColor);
                }
                else
                {
                    previewColor = circle2Color;

                    if (sideMenu->isMenuVisible())
                    {
                        previewColor.setAlpha(20);
                    }

                    pen.setColor(previewColor.darker(150));
                    pen.setWidth(cellSize / 9);
                    painter.setPen(pen);
                    painter.setBrush(previewColor);
                }

                painter.drawEllipse(circleRect);
            }
        }

        painter.setPen(Qt::transparent);

        // Draw disc in animation
        if (isAnimating)
        {
            QRectF animCircle(
                x0 + (animColumn * cellSize + (cellSize * 0.11)),
                y0 + (animY + (cellSize * 0.11)),
                cellSize - (cellSize * 0.22),
                cellSize - (cellSize * 0.22));

            QColor animColorAux;
            QPen pen;

            animColorAux = animColor;

            if (sideMenu->isMenuVisible())
            {
                animColorAux.setAlpha(20);
            }

            pen.setColor(animColorAux.darker(150));
            pen.setWidth(cellSize / 9);
            painter.setPen(pen);
            painter.setBrush(animColorAux);

            painter.drawEllipse(animCircle);
        }

        // Automatic positioning of the disc if it's the CPU
        if (CPU_on && currentPlayer == 2)
        {
            userInputEnabled = false;

            int rand_column, rand_row;
            do
            {
                rand_column = rand() % cols;

            } while(!(dropDiscCPU(rand_column, rand_row)));

            // Animation variables
            animColumn = rand_column;
            animRow = rand_row;
            animY = -cellSize; // Starts the animation from above the board
            animColor = circle2Color;

            // Start animation
            isAnimating = true;
            currentPlayer = 1;
            animationTimerCPU.start(16); // Approximately 60 FPS
        }
        // Non-automatic positioning if it's a player
        else
        {
            // Draw preview disc
            if ((previewActive && previewColumn >= 0) &&
                !(sideMenu->isMenuVisible()))
            {
                int previewRow;
                if (dropDiscPreview(previewColumn, previewRow))
                {
                    QRectF previewCircle(
                        x0 + (previewColumn * cellSize + (cellSize * 0.11)),
                        y0 + (previewRow * cellSize + (cellSize * 0.11)),
                        cellSize - (cellSize * 0.22),
                        cellSize - (cellSize * 0.22));


                    if (currentPlayer == 1)
                    {
                        previewColor = circle1Color;
                    }
                    else
                    {
                        previewColor = circle2Color;
                    }

                    previewColor.setAlpha(128); // Translucent color
                    circle_pen.setColor(previewColor);
                    circle_pen.setWidth(cellSize / 9);

                    painter.setPen(circle_pen);
                    painter.setBrush(Qt::transparent);
                    painter.drawEllipse(previewCircle);

                    QRectF columnCircle(
                        x0 + (previewColumn * cellSize + (cellSize * 0.11)),
                        y0 - (cellSize + (cellSize * 0.11)) * 1.05,
                        cellSize - (cellSize * 0.22),
                        cellSize - (cellSize * 0.22));

                    QPen pen(previewColor.darker(150));
                    pen.setWidth(cellSize / 9);
                    painter.setPen(pen);
                    painter.setBrush(previewColor);

                    painter.drawEllipse(columnCircle);
                }
            }
        }

        QString currentBoardPath;

        if (sideMenu->isMenuVisible())
        {
            currentBoardPath = bluredBoardPath;
        }
        else
        {
            currentBoardPath = boardPath;
        }
        // Draw the background image (adjusted to the board size)
        QPixmap pixmap(currentBoardPath);

        float h_margin, v_margin;

        switch (mainwindow->getBoardSize())
        {
            case SMALL:
                h_margin = cellSize * cols * 0.04739;
                v_margin = cellSize * rows * 0.05242;
                break;

            case MEDIUM:
                h_margin = cellSize * cols * 0.036;
                v_margin = cellSize * rows * 0.048;
                break;

            case BIG:
                h_margin = cellSize * cols * 0.0378;
                v_margin = cellSize * rows * 0.042;
                break;

            default:
                h_margin = 0.0;
                v_margin = 0.0;
                break;
        }

        pixmap = pixmap.scaled(
            cellSize * cols + h_margin,
            cellSize * rows + v_margin,
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation);

        painter.drawPixmap(
            x0 - int(h_margin / 2.0),
            y0 - int(v_margin / 2.0),
            pixmap);
    }

    // Adjust the size of the buttons to fit the board size
    resizeButtons();

    ui->boardView->update();

}   /* paintEvent() */

/******************************************************************************/
/*!
 * @brief   This function runs each time a mouse move is detected.
 * @param   event  Recieves the current QMouseEvent.
 * @return  void
 */
void GameBoard::mouseMoveEvent(QMouseEvent * event)
{
    if (!userInputEnabled)
    {
        return; // Ignore event if the CPU is acting
    }

    if (!isAnimating)
    {
        int x = event->pos().x();
        int y = event->pos().y();

        int column = -1, row;

        // Check that the mouse is over the game board (width check)
        // Check that the mouse is over the game board (height check)
        if (((x >= x0) && (x <= (x0 + (cellSize * cols)))) &&
            ((y >= y0 - cellSize) &&
                (y <= (y0 - cellSize + (cellSize * cols)))))
        {
            // Calculate column based on mouse position
            column = (x - x0) / cellSize;
        }

        dropDiscPreview(column, row);
    }

}   /* mouseMoveEvent() */

/******************************************************************************/
/*!
 * @brief   This function runs each time a mouse click is detected.
 * @param   event  Recieves the current QMouseEvent.
 * @return  void
 */
void GameBoard::mousePressEvent(QMouseEvent * event)
{
    // Check if the side menu is visible and the click is outside its area
    if (sideMenu->isMenuVisible())
    {
        if (!sideMenu->geometry().contains(event->pos()))
        {
            sideMenu->hideMenu();
        }
        else
        {
            ; // Do nothing
        }
    }
    else
    {
        if (!userInputEnabled)
        {
            return; // Ignore event if the CPU is acting
        }

        // Avoid clicks during animations
        if (event->button() == Qt::LeftButton && !isAnimating)
        {
            int x = event->pos().x();
            int y = event->pos().y();

            int column = -1;

            // Check that the mouse is over the game board (width check)
            // Check that the mouse is over the game board (height check)
            if (((x >= x0) && (x <= (x0 + (cellSize * cols)))) &&
                ((y >= y0 - cellSize) &&
                    (y <= (y0 - cellSize + (cellSize * cols)))))
            {
                // Calculate column based on mouse position
                column = (x - x0) / cellSize;
            }

            if (column >= 0 && column < cols)
            {
                int row;
                if (dropDiscPreview(column, row))
                {
                    animColumn = column;
                    animRow = row;

                    // Starts the animation from above the board
                    animY = -cellSize;

                    if (currentPlayer == 1)
                    {
                        animColor = circle1Color;
                    }
                    else
                    {
                        animColor = circle2Color;
                    }

                    // Start animation
                    isAnimating = true;
                    previewActive = false;
                    animationTimer.start(16); // Approximately 60 FPS
                }
                else
                {
                    QMessageBox::warning(
                        this
                        , tr("Full Column")
                        , tr("The selected column is full. "
                           "Please choose another one."));
                }
            }
        }
    }

}   /* mousePressEvent() */

/******************************************************************************/
/*!
 * @brief   This function runs each time a key is pressed.
 * @param   event  Recieves the current QKeyEvent.
 * @return  void
 */
void GameBoard::keyPressEvent(QKeyEvent * event)
{
    // If the escape key is pressed...
    if (event->key() == Qt::Key_Escape)
    {
        // If it's in the boardView...
        if (ui->stackedWidget->currentWidget() == ui->boardView)
        {
            ui->stackedWidget->setCurrentWidget(ui->pauseView);
            userInputEnabled = false;
        }
        // If it's in the pauseView...
        else if (ui->stackedWidget->currentWidget() == ui->pauseView)
        {
            // Close the pause menu
            ui->stackedWidget->setCurrentWidget(ui->boardView);
            userInputEnabled = true;
        }
    }

}   /* keyPressEvent() */

/******************************************************************************/
/*!
 * @brief   This function runs each time a window resize is detected.
 * @param   event  Recieves the current QResizeEvent.
 * @return  void
 */
void GameBoard::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event);

    // Buttons are resized
    resizeButtons();

    // Resize the side menu if it is visible
    if (sideMenu->isMenuVisible())
    {
        sideMenu->resizeMenu(); // Adjust the size of the side menu
    }

}   /* resizeEvent() */


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief   This function shows the pause menu when the pause button is pressed.
 * @param   void
 * @return  void
 */
void GameBoard::onPauseButtonClicked(void)
{
    ui->stackedWidget->setCurrentWidget(ui->pauseView);
    userInputEnabled = false;

}   /* onPauseButtonClicked() */

/******************************************************************************/
/*!
 * @brief   This function is used to reset the game when the reset button on
 *          the pauseMenu is pressed.
 * @param   void
 * @return  void
 */
void GameBoard::onResetButton_pauseViewClicked(void)
{
    resetBoard();

}   /* onResetButton_pauseViewClicked() */

/******************************************************************************/
/*!
 * @brief   This function is used to reset the game when the reset button on
 *          the victoryMenu is pressed.
 * @param   void
 * @return  void
 */
void GameBoard::onResetButton_victoryViewClicked(void)
{
    currentRound = 1;
    ui->roundLabel->setText(
        QString(tr("Round %1"))
            .arg(currentRound));

    resetBoard();

}   /* onResetButton_victoryViewClicked() */

/******************************************************************************/
/*!
 * @brief   This function resumes the current game when the continue button is
 *          pressed.
 * @param   void
 * @return  void
 */
void GameBoard::onContinueButtonClicked(void)
{
    ui->stackedWidget->setCurrentWidget(ui->boardView);
    userInputEnabled = true;

}   /* onContinueButtonClicked() */

/******************************************************************************/
/*!
 * @brief   This function returns the player to the main menu when one of the
 *          menu buttons is pressed. Used in the pause and victory menu.
 * @param   void
 * @return  void
 */
void GameBoard::onMainMenuButtonClicked(void)
{
    // Reset the scores of Player guest and CPU
    // (data that should not be stored between new games)
    int resetPoints;
    if (mainwindow->playingAsGuest)
    {
        resetPoints = mainwindow->player2->getPoints();
        mainwindow->player2->addPoints(-resetPoints);
    }
    else if (CPU_on)
    {
        resetPoints = mainwindow->player2->getPoints();
        mainwindow->player2->addPoints(-resetPoints);
    }

    QWidget * newWidget = new mainMenu(mainwindow);
    mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);

}   /* onMainMenuButtonClicked() */

/******************************************************************************/
/*!
 * @brief   This function shows the sideMenu when the profile button is pressed.
 * @param   void
 * @return  void
 */
void GameBoard::onProfileButtonClicked(void)
{
    // Call the function to display the side menu
    sideMenu->showMenu();

}   /* onProfileButtonClicked() */


//-----[ IMPLEMENTATION OF PRIVATE MAINWINDOW FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief   Implementation of the GameBoard parameter's initializer.
 * @param   void
 * @return  void
 */
void GameBoard::initializeGameBoard(void)
{
    // Set the board size according to the set size
    boardSize = this->mainwindow->getBoardSize();

    switch(boardSize)
    {
        case BIG:
            rows = 8;
            cols = 9;
            break;

        case MEDIUM:
            rows = 7;
            cols = 8;
            break;

        case SMALL:
        default: // By default, the board size is small
            rows = 6;
            cols = 7;
            break;
    }

    // Set the game mode based on whether playing against the CPU or not
    gameMode = this->mainwindow->getGameMode();

    // Get the name of player 1
    player1Name = this->mainwindow->player1->getNickName();

    if (player1Name == "Invitado 1")
    {
        player1Name = tr("Invitado 1");
    }

    points1 = this->mainwindow->player1->getPoints();

    switch(gameMode)
    {
    case VS_PLAYER:
        CPU_on = false;

        // Get the name of player 2
        if (this->mainwindow->playingAsGuest)
        {
            if (mainwindow->player1 == mainwindow->guest1)
            {
                this->mainwindow->player2 = this->mainwindow->guest2;
            }
            else // Guest 1
            {
                this->mainwindow->player2 = this->mainwindow->guest1;
            }

            player2Name = this->mainwindow->player2->getNickName();
            points2 = 0;
        }
        else
        {
            player2Name = this->mainwindow->player2->getNickName();
            points2 = this->mainwindow->player2->getPoints();
        }

        if (player1Name == "Invitado 1")
        {
            player1Name = tr("Invitado 1");
        }
        else if (player1Name == "Invitado 2")
        {
            player1Name = tr("Invitado 2");
        }
        else
        {
            ; // Do nothing
        }

        // Set the points to be obtained if won to 100
        winningPoints = 100;
        break;

    case VS_CPU:
        CPU_on = true;
        // Set the name of player 2 as "CPU"
        this->mainwindow->player2 = this->mainwindow->cpu;
        player2Name = this->mainwindow->player2->getNickName();
        // Set the points to be obtained if won to 50
        winningPoints = 50;
        break;
    }

    // Get the disc color of the players
    circle1Color = this->mainwindow->boardPlayer1.getCircleColor();
    circle2Color = this->mainwindow->boardPlayer2.getCircleColor();

    // Get the number of rounds
    amountOfRounds = this->mainwindow->getRounds();
    qDebug() << "Rounds: " << amountOfRounds;

    if (amountOfRounds > 3 || amountOfRounds < 1)
    {
        amountOfRounds = 1; // by default, there is 1 round
    }

    winner.resize(amountOfRounds + 1);
    loser.resize(amountOfRounds + 1);

    for (int i = 1; i <= amountOfRounds; i++)
    {
        winner[i] = nullptr;
        loser[i] = nullptr;
    }

    // Set the player's name and current round in the turn label
    ui->playerTurnLabel->setText(
        QString(tr("%1's turn"))
            .arg(player1Name));
    ui->roundLabel->setText(
        QString(tr("Round %1"))
            .arg(currentRound));

    startTime = QDateTime::currentDateTime(); // Current date and time

}   /* initializeGameBoard() */

/******************************************************************************/
/*!
 * @brief  This function checks if the selected column is full.
 * @param  column  This parameter gets the selected column.
 * @param  row     This parameter will be set to the first free row.
 * @return bool    True if the column is valid, false otherwise.
 */
bool GameBoard::dropDiscPreview(int column, int & row)
{
    // Check if the column is within bounds
    if (column < 0 || column >= cols)
    {
        previewActive = false;
        previewColumn = -1;
        update(); // Update the view to remove the preview disc
        return (false);
    }

    // Find the lowest available row in the column
    for (int r = rows - 1; r >= 0; --r)
    {
        if (grid[r][column] == 0)
        {
            row = r;
            previewActive = true;
            previewColumn = column;
            update(); // Update the view to show the preview disc
            return (true);
        }
    }

    // If the column is full, disable the preview
    previewActive = false;
    previewColumn = -1;
    update(); // Update the view to remove the preview disc

    return (false);
}

/******************************************************************************/
/*!
 * @brief   This function checks if the selected column by the CPU is full.
 * @param   column  This parameter gets the selected column.
 * @param   row     This parameter will be set to the first free row.
 * @return  True if the column is valid, false otherwise.
 */
bool GameBoard::dropDiscCPU(int column, int & row)
{
    if (column < 0 || column >= cols)
    {
        return (false);
    }

    for (int r = rows - 1; r >= 0; r--)
    {
        if (grid[r][column] == 0)
        {
            row = r;
            return (true);
        }
    }

    return (false); // Full column

}   /* dropDiscCPU() */

/******************************************************************************/
/*!
 * @brief   This function checks if there is a winner.
 * @param   row  The row where the last move was made.
 * @param   col  The column where the last move was made.
 * @return  True if there is a winner, false otherwise.
 */
bool GameBoard::checkWin(int row, int col)
{
    int player = grid[row][col];

    if (player == 0)
    {
        return (false);
    }

    // Directions: horizontal (-), vertical (|), diagonal (/), diagonal (\)
    const QVector<QPair<int, int>> directions
        = {
            {0, 1}, {1, 0}, {1, 1}, {1, -1}
        };

    for (auto &dir : directions)
    {
        int count = 1;

        // Positive direction
        int r = row + dir.first;
        int c = col + dir.second;
        while (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == player)
        {
            count++;
            r += dir.first;
            c += dir.second;
        }

        // Negative direction
        r = row - dir.first;
        c = col - dir.second;
        while (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == player)
        {
            count++;
            r -= dir.first;
            c -= dir.second;
        }

        if (count >= 4)
        {
            return (true);
        }
    }

    return (false);

}   /* checkWin() */

/******************************************************************************/
/*!
 * @brief   This function checks if the board is full.
 * @param   void
 * @return  True if the board is full, false otherwise.
 */
bool GameBoard::checkFull(void)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == 0)
            {
                return (false);
            }
        }
    }

    return (true);

}   /* checkFull() */

/******************************************************************************/
/*!
 * @brief   This function animates the moving disc, registers the new disc,
 *          checks the win condition and sets the winner view if there is a
 *          winner.
 * @param   void
 * @return  void
 */
void GameBoard::startAnimation(void)
{
    if (!isAnimating)
    {
        return;
    }

    animY += cellSize / 10.0; // Animation speed

    if (animY >= animRow * cellSize)
    {
        animY = animRow * cellSize; // Set exact position
        isAnimating = false;
        animationTimer.stop();

        // Register the disc in the grid once the animation ends
        grid[animRow][animColumn] = currentPlayer;

        // Adjust exact position
        // Verify victoriaanimY = animRow * cellSize;
        isAnimating = false;
        animationTimer.stop();

        // Register the disc in the grid once the animation ends
        grid[animRow][animColumn] = currentPlayer;

        // Check for victory
        if (checkWin(animRow, animColumn))
        {
            if (currentPlayer == 1)
            {
                QMessageBox::information(
                    nullptr
                    , tr("Round Finished")
                    , QString(tr("%1 has won!")).arg(player1Name));

                winner[currentRound] = this->mainwindow->player1;
                loser[currentRound]  = this->mainwindow->player2;
            }
            else // (currentPlayer == 2)
            {
                QMessageBox::information(
                    nullptr
                    , tr("Round Finished")
                    , QString(tr("%1 has won!")).arg(player2Name));

                winner[currentRound] = this->mainwindow->player2;
                loser[currentRound]  = this->mainwindow->player1;
            }

            Connect4::getInstance().registerRound(
                startTime, winner[currentRound], loser[currentRound]);

            if (currentRound == amountOfRounds)
            {
                getWinner();
                updateVictoryView();

                ui->stackedWidget->setCurrentWidget(ui->victoryView);
            }
            else
            {
                currentRound++;
                ui->roundLabel->setText(
                    QString(tr("Round %1"))
                        .arg(currentRound));

                // Current date and time
                startTime = QDateTime::currentDateTime();
                resetBoard();
            }
        }
        // Check if the board is full
        else if (checkFull())
        {
            QMessageBox::StandardButton answer = QMessageBox::question(
                this
                , tr("Full Board")
                , tr("Restart the Round?")
                , QMessageBox::Yes | QMessageBox::No);

            if (answer == QMessageBox::Yes)
            {
                resetBoard();
            }
            else
            {
                QMessageBox::information(
                    nullptr
                    , tr("Round Finished")
                    , tr("Draw!"));

                winner[currentRound] = nullptr;
                loser[currentRound] = nullptr;

                if (currentRound == amountOfRounds)
                {
                    getWinner();
                    updateVictoryView();

                    ui->stackedWidget->setCurrentWidget(ui->victoryView);
                }

                currentRound++;
                ui->roundLabel->setText(
                    QString(tr("Round %1"))
                        .arg(currentRound));

                // Current date and time
                startTime = QDateTime::currentDateTime();
                resetBoard();
            }
        }
        else
        {
            currentPlayer = (currentPlayer == 1) ? 2 : 1;

            if (currentPlayer == 1)
            {
                ui->playerTurnLabel->setText(
                    QString(tr("%1's turn"))
                        .arg(player1Name));
            }
            else
            {
                ui->playerTurnLabel->setText(
                    QString(tr("%1's turn"))
                        .arg(player2Name));
            }
        }
    }

    update(); // Redraw

}   /* startAnimation() */

/******************************************************************************/
/*!
 * @brief   This function resets the board.
 * @param   void
 * @return  void
 */
void GameBoard::startAnimationCPU(void)
{
    if (!isAnimating) return;

    animY += cellSize / 10.0; // Animation speed

    if (animY >= animRow * cellSize) {
        animY = animRow * cellSize; // Adjust exact position
        isAnimating = false;
        animationTimerCPU.stop();

        // After the animation, place the disc in the corresponding cell
        grid[animRow][animColumn] = 2;

        // Check for victory
        if (checkWin(animRow, animColumn))
        {
            QMessageBox::information(
                nullptr
                , tr("Round Finished")
                , QString(tr("%1 has won!")).arg(player2Name));

            winner[currentRound] = this->mainwindow->cpu;
            loser[currentRound] = this->mainwindow->player1;

            Connect4::getInstance().registerRound(
                startTime, winner[currentRound], loser[currentRound]);

            if (currentRound == amountOfRounds)
            {
                getWinner();
                updateVictoryView();

                ui->stackedWidget->setCurrentWidget(ui->victoryView);
            }
            else
            {
                currentRound++;
                ui->roundLabel->setText(
                    QString(tr("Round %1"))
                        .arg(currentRound));

                // Current date and time
                startTime = QDateTime::currentDateTime();
                resetBoard();
            }
        }
        // Check if the board is full
        else if (checkFull())
        {
            QMessageBox::StandardButton answer = QMessageBox::question(
                this
                , tr("Full Board")
                , tr("Restart the Round?")
                , QMessageBox::Yes | QMessageBox::No);

            if (answer == QMessageBox::Yes)
            {
                resetBoard();
            }
            else
            {
                QMessageBox::information(
                    nullptr
                    , tr("Round Finished")
                    , tr("Draw!"));

                winner[currentRound] = nullptr;
                loser[currentRound] = nullptr;

                if (currentRound == amountOfRounds)
                {
                    getWinner();
                    updateVictoryView();

                    ui->stackedWidget->setCurrentWidget(ui->victoryView);
                }
                else
                {
                    currentRound++;
                    ui->roundLabel->setText(
                        QString(tr("Round %1"))
                            .arg(currentRound));

                    // Current date and time
                    startTime = QDateTime::currentDateTime();
                    resetBoard();
                }
            }
        }
        else
        {
            ui->playerTurnLabel->setText(
                QString(tr("%1's turn")).arg(player1Name));
        }
    }

    userInputEnabled = true;
    update(); // Redraw

}   /* startAnimationCPU() */

/******************************************************************************/
/*!
 * @brief   This function gets the winner after playing all the rounds.
 * @param   void
 * @return  void
 */
void GameBoard::getWinner(void)
{
    int p1RoundsWon = 0;
    int p2RoundsWon = 0;

    for (int i = 1; i <= amountOfRounds; i++)
    {
        qDebug() << "winner[i]: " << winner[i]->getNickName();

        if (winner[i]->getNickName() ==
            this->mainwindow->player1->getNickName())
        {
            p1RoundsWon++;
            this->mainwindow->player1->addPoints(winningPoints);
        }
        else if (winner[i]->getNickName() ==
                   this->mainwindow->player2->getNickName())
        {
            p2RoundsWon++;
            this->mainwindow->player2->addPoints(winningPoints);
        }
        else // Draw (tie)
        {
            ; // Do nothing
        }

        winner[i] = nullptr;
    }

    qDebug() << "p1RoundsWon: " << p1RoundsWon;
    qDebug() << "p2RoundsWon: " << p2RoundsWon;

    if (p1RoundsWon > p2RoundsWon)
    {
        finalWinner = this->mainwindow->player1;
        finalWinnerName = player1Name;

        finalLoser = this->mainwindow->player2;
        finalLoserName = player2Name;
    }
    else if (p1RoundsWon < p2RoundsWon)
    {
        finalWinner = this->mainwindow->player2;
        finalWinnerName = player2Name;

        finalLoser = this->mainwindow->player1;
        finalLoserName = player1Name;
    }
    else // Draw (tie)
    {
        finalWinner = nullptr;
        finalWinnerName = nullptr;
        finalLoser = nullptr;
        finalLoserName = nullptr;
    }

}   /* getWinner() */

/******************************************************************************/
/*!
 * @brief   This function updates the victoryView labels.
 * @param   void
 * @return  void
 */
void GameBoard::updateVictoryView(void)
{
    if (finalWinnerName != nullptr)
    {
        // Update the label displaying the winner
        ui->victoryLabel->setText(
            QString(tr("%1 has won!"))
                .arg(finalWinnerName));
    }
    else // Draw (tie)
    {
        ui->victoryLabel->setText(tr("Draw!"));
    }

    if (CPU_on)
    {
        ui->pointsBeforeLabel->setText(
            QString("%1: %2")
                .arg(player1Name)
                .arg(points1));

        ui->pointsAfterLabel->setText(
            QString("%1: %2")
                .arg(player1Name)
                .arg(this->mainwindow->player1->getPoints()));
    }
    else
    {
        ui->pointsBeforeLabel->setText(
            QString("%1: %2\n%3: %4")
                .arg(player1Name)
                .arg(points1)
                .arg(player2Name)
                .arg(points2));

        ui->pointsAfterLabel->setText(
            QString("%1: %2\n%3: %4")
                .arg(player1Name)
                .arg(this->mainwindow->player1->getPoints())
                .arg(player2Name)
                .arg(this->mainwindow->player2->getPoints()));
    }

}   /* updateVictoryView() */

/******************************************************************************/
/*!
 * @brief   This function connects the buttons with the linked functions.
 * @param   void
 * @return  void
 */
void GameBoard::connectButtons(void)
{
    // Connect the theme change signal
    connect(
        mainwindow
        , &MainWindow::themeChanged
        , this
        , &GameBoard::setIcons);

    // Connect the pause button
    connect(
        ui->pauseButton
        , &QPushButton::clicked
        , this
        , &GameBoard::onPauseButtonClicked);

    // Connect the resume and exit buttons from the pause menu
    connect(
        ui->continueButton
        , &QPushButton::clicked
        , this
        , &GameBoard::onContinueButtonClicked);

    connect(
        ui->GBW_exitButton
        , &QPushButton::clicked
        , this
        , &GameBoard::onContinueButtonClicked);

    // Connect the reset buttons
    connect(
        ui->resetButton_pauseView
        , &QPushButton::clicked
        , this
        , &GameBoard::onResetButton_pauseViewClicked);

    connect(
        ui->resetButton_victoryView
        , &QPushButton::clicked
        , this
        , &GameBoard::onResetButton_victoryViewClicked);

    // Connect the buttons to return to the main menu
    connect(
        ui->mainMenuButton_pauseView
        , &QPushButton::clicked
        , this
        , &GameBoard::onMainMenuButtonClicked);

    connect(
        ui->mainMenuButton_victoryView
        , &QPushButton::clicked
        , this
        , &GameBoard::onMainMenuButtonClicked);

    // Connect the profile button
    connect(
        ui->profileButton
        , &QPushButton::clicked
        , this
        , &GameBoard::onProfileButtonClicked);

}   /* connectButtons() */

/******************************************************************************/
/*!
 * @brief   This function resets the board.
 * @param   void
 * @return  void
 */
void GameBoard::resetBoard(void)
{
    // Reset the layout
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            grid[r][c] = 0;
        }
    }

    currentPlayer = 1;

    ui->playerTurnLabel->setText(
        QString(tr("%1's turn"))
            .arg(player1Name));

    ui->stackedWidget->setCurrentWidget(ui->boardView);
    userInputEnabled = true;

}   /* resetBoard() */

/******************************************************************************/
/*!
 * @brief   This function resizes the button passed as parameter.
 * @param   button  This parameter gets the button that will be resized.
 * @param   size    This parameter gets the wanted button size.
 * @return  void
 */
void GameBoard::changeButtonSize(QPushButton * button, float size)
{
    if (button)
    {
        button->setFixedHeight(size);
        button->setFixedWidth(size);
        button->setIconSize(button->size());
    }

}   /* changeButtonSize() */

/******************************************************************************/
/*!
 * @brief   This function sets the buttons new size.
 * @param   void
 * @return  void
 */
void GameBoard::resizeButtons(void)
{
    float buttonsSize;
    QSize newSize;

    if (ui->stackedWidget->currentWidget() == ui->boardView)
    {
        if (ui->GameBoardGrid->size().width() >=
            ui->GameBoardGrid->size().height())
        {
            buttonsSize = espacioJuegoHeight / rows;
        }
        else
        {
            buttonsSize = espacioJuegoWidth / cols;
        }

        if (buttonsSize >= 30 && buttonsSize <= 80)
        {
            // Update the size of the buttons
            changeButtonSize(ui->pauseButton, buttonsSize);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->pauseView)
    {
        newSize = ui->pauseMenuFrame->size();
        if (newSize.width() >= newSize.height())
        {
            buttonsSize = newSize.height() / rows;
        }
        else
        {
            buttonsSize = newSize.width() / cols;
        }

        // Update the button sizes
        changeButtonSize(ui->continueButton, buttonsSize);
        changeButtonSize(ui->resetButton_pauseView, buttonsSize);
        changeButtonSize(ui->mainMenuButton_pauseView, buttonsSize);
    }
    else if (ui->stackedWidget->currentWidget() == ui->victoryView)
    {
        newSize = ui->victoryFrame->size();
        if (newSize.width() >= newSize.height())
        {
            buttonsSize = newSize.height() / rows;
        }
        else
        {
            buttonsSize = newSize.width() / cols;
        }

        // Update the button sizes
        changeButtonSize(ui->resetButton_victoryView, buttonsSize);
        changeButtonSize(ui->mainMenuButton_victoryView, buttonsSize);
    }
    else
    {
        ; // Do nothing
    }

}   /* resizeButtons() */

/******************************************************************************/
/*!
 * @brief   This function sets the button icons.
 * @param   void
 * @return  void
 */
void GameBoard::setIcons(void)
{
    QString theme;

    if (mainwindow->currentTheme == DARK_THEME)
    {
        theme = "dark-theme";
    }
    else // (mainwindow->currentTheme == LIGHT_THEME)
    {
        theme = "light-theme";
    }

    // Set the board icons according to the set size
    boardSize = this->mainwindow->getBoardSize();

    switch(boardSize)
    {
        case BIG:
            boardPath = ":media/images/boards/" +
                        theme + "/board_big.png";

            bluredBoardPath = ":media/images/boards/" +
                              theme + "/blured_board_big.png";
            break;

        case MEDIUM:
            boardPath = ":media/images/boards/" +
                        theme + "/board_medium.png";

            bluredBoardPath = ":media/images/boards/" +
                              theme + "/blured_board_medium.png";
            break;

        case SMALL:
        default: // by default the board size is small
            boardPath = ":media/images/boards/" +
                        theme + "/board_small.png";

            bluredBoardPath = ":media/images/boards/" +
                              theme + "/blured_board_small.png";
            break;
    }

}   /* setIcons() */


/*** end of file ***/
