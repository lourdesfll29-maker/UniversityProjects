/**
 * @file     gameboard.h
 *
 * @brief    Definition of GameBoard class functions.
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

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef GAMEBOARD_H
#define GAMEBOARD_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>
#include <QTimer>
#include <QPushButton>

/// Own includes
#include "mainwindow.h"
#include "player.h"
#include "profilesidemenu.h"
#include "newgamewindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class GameBoard; }
QT_END_NAMESPACE


//-----[ GAMEBOARD CLASS ]---------------------------------------------------//

class GameBoard : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit GameBoard(QWidget * parent = nullptr);
        ~GameBoard(void);

    signals:

        /*-----------------------------------------------------+
         |     DECLARATION OF SIGNAL FUNCTIONS                 |
         +-----------------------------------------------------*/

    protected:

        /*-----------------------------------------------------+
         |     DECLARATION OF PROTECTED FUNCTIONS              |
         +-----------------------------------------------------*/

        void paintEvent(QPaintEvent * event) override;
        void mouseMoveEvent(QMouseEvent * event) override;
        void mousePressEvent(QMouseEvent * event) override;
        void keyPressEvent(QKeyEvent * event) override;
        void resizeEvent(QResizeEvent * event) override;

    private slots:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

        void onPauseButtonClicked(void);
        void onResetButton_pauseViewClicked(void);
        void onResetButton_victoryViewClicked(void);
        void onContinueButtonClicked(void);
        void onMainMenuButtonClicked(void);
        void onProfileButtonClicked(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES & FUNCTIONS   |
         +-----------------------------------------------------*/

        /// Variables

        Ui::GameBoard * ui;
        ProfileSideMenu * sideMenu;
        NewGameWindow * gameWindow;
        MainWindow * mainwindow;

        boardSize_t boardSize;
        int rows;
        int cols;
        QString boardPath;
        QString bluredBoardPath;

        gameMode_t gameMode;
        bool CPU_on;

        int amountOfRounds;
        int currentRound;

        bool userInputEnabled;
        int winningPoints;

        Player * finalWinner;
        Player * finalLoser;

        QVector<Player *> winner;
        QVector<Player *> loser;

        QString player1Name;
        QString player2Name;

        QString finalWinnerName;
        QString finalLoserName;

        QColor circle1Color;
        QColor circle2Color;

        int points1;
        int points2;

        float espacioJuegoWidth;
        float espacioJuegoHeight;
        float cellSize;

        int x0; // pixel where the game board starts (x)
        int y0; // pixel where the game board starts (y)

        int previewColumn;
        bool previewActive;
        int currentPlayer;

        QVector<QVector<int>> grid;

        // Variables for animation
        bool isAnimating;
        int animColumn;
        int animRow;
        double animY; // Current Y position of the tile in the animation
        QColor animColor;

        QTimer animationTimer;
        QTimer animationTimerCPU;

        // Time variables for ranking
        QDateTime startTime;

        /// Functions

        void initializeGameBoard(void);

        bool dropDiscPreview(int column, int &row);
        bool dropDiscCPU(int column, int &row);
        bool checkWin(int row, int col);
        bool checkFull(void);

        void startAnimation(void);
        void startAnimationCPU(void);
        void updatePoints(QVector<Player *> winner);
        void getWinner(void);
        void updateVictoryView(void);

        void connectButtons(void);
        void resetBoard(void);
        void changeButtonSize(QPushButton * button, float size);
        void resizeButtons(void);

        void setIcons(void);
};

#endif /* GAMEBOARD_H */

/*** end of file ***/
