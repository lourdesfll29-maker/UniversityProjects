/**
 * @file     mainwindow.h
 *
 * @brief    Definition of MainWindow class functions.
 *
 * @version  0.5  (2024/01/24) Final version
 * @version  0.4  (2024/01/23) BARR-C programming standard needs to be revised
 * @version  0.3  (2025/01/01) Initial Functional Prototype
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QMainWindow>
#include <QTranslator>
#include <QMediaPlayer>

/// Own includes
#include "boardplayer.h"
#include "player.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//

// Definition of window exchange types
typedef enum
{
    DELETE_PREV_WIDGET,
    SAVE_PREV_WIDGET,
    RECOVER_PREV_WIDGET,
    SAVE_PREV_WINDOW,
    RECOVER_PREV_WINDOW

}   changeType_t;

// Defining the board size
typedef enum
{
    SMALL,
    MEDIUM,
    BIG

}   boardSize_t;

// Game mode definition
typedef enum
{
    VS_PLAYER,
    VS_CPU

}   gameMode_t;

// Interface theme definition
typedef enum
{
    UNKNOWN_THEME,
    DARK_THEME,
    LIGHT_THEME

}   uiTheme_t;

// Interface language definition
typedef enum
{
    ENGLISH,
    SPANISH

}   uiLanguage_t;

// Defining the interface color
typedef enum
{
    UNKNOWN_COLOR,
    BLUE_COLOR,
    GREEN_COLOR,
    PINK_COLOR,
    ORANGE_COLOR

}   uiColorTheme_t;

// Defining the player  to be logged
typedef enum
{
    PLAYER1,
    PLAYER2

}   playerToBeLogged_t;


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


//-----[ MAINWINDOW CLASS ]---------------------------------------------------//

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC VARIABLES & FUNCTIONS     |
         +-----------------------------------------------------*/

        /// Variables

        Player * currentPlayer;
        uiTheme_t currentTheme;
        uiColorTheme_t currentColorTheme;
        uiLanguage_t currentLanguage;

        // Music variables
        QMediaPlayer *mediaplayer;
        QAudioOutput *audioOutput;
        QStringList playlist;
        QString Abyss_Theme;
        QString Mario_Theme;
        QString MarioLuigi_Theme;
        QString Pokemon_Theme;
        QString Smash_Theme;
        QString SpongeBob_Theme;
        QString Wii_Theme;
        QString Yoshi_Theme;
        QString Zelda_Theme;
        int currentSongIndex;
        QString currentSong;
        bool fullPlaylist;
        bool firstStart;

        QString playerSelected;

        BoardPlayer boardPlayer1;
        BoardPlayer boardPlayer2;

        Player * player1;
        Player * player2;
        Player * guest1;
        Player * guest2;
        Player * cpu;

        bool playingAsGuest;

        QTranslator translator; // Translator for languages

        /// Functions

        explicit MainWindow(QWidget * parent = nullptr);
        ~MainWindow(void);

        QWidget * getCurrentWidget(void);
        QWidget * getPreviousWindow(void);

        // Window switching function
        void changeCentralWidget(
            QWidget * newWidget = nullptr,
            changeType_t changeType = DELETE_PREV_WIDGET);

        // Window UI theme switching function
        void setUITheme(
            uiTheme_t theme,
            uiColorTheme_t color);

        // Window UI language switching function
        void setUILanguage(uiLanguage_t uiLanguage);

        // Sets initial music features
        void setMusic(void);
        void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

        // Board Size Management Features
        boardSize_t getBoardSize(void);
        void setBoardSize(boardSize_t newBoardSize);

        // Game Mode Management Features
        gameMode_t getGameMode(void);
        void setGameMode(gameMode_t newGameMode);

        // Rounds Management Features
        int getRounds(void);
        void setRounds(int newRounds);

    signals:

        /*-----------------------------------------------------+
         |     DECLARATION OF SIGNAL FUNCTIONS                 |
         +-----------------------------------------------------*/

        void themeChanged(void);
        void userLogged(void);
        void retranslateUIs(void);
        void musicChangedState(void);

    protected:

        /*-----------------------------------------------------+
         |     DECLARATION OF PROTECTED FUNCTIONS              |
         +-----------------------------------------------------*/

    private slots:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES               |
         +-----------------------------------------------------*/

        Ui::MainWindow * ui;

        boardSize_t boardSize;
        gameMode_t gameMode;
        int rounds;

        QWidget * currentWidget;
        QWidget * previousWidget;
        QWidget * previousWindow;
};

#endif /* MAINWINDOW_H */

/*** end of file ***/
