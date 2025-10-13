/**
 * @file     mainwindow.cpp
 *
 * @brief    Implementation of MainWindow class functions.
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

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QMediaPlayer>
#include <QAudioOutput>

/// Own includes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boardplayer.h"
#include "connect4.h"
#include "connect4dao.h"
#include "aboutwindow.h"
#include "gameboard.h"
#include "individualrankingwindow.h"
#include "logwindow.h"
#include "newgamewindow.h"
#include "newgamepart2window.h"
#include "optionswindow.h"
#include "profilewindow.h"
#include "rankingwindow.h"
#include "singwindow.h"
#include "startwindow.h"
#include "welcomewindow.h"
#include "mainmenu.h"


//-----[ IMPLEMENTATION OF PUBLIC MAINWINDOW FUNCTIONS ]----------------------//

/******************************************************************************/
/*!
 * @brief   MainWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of MainWindow.
 */
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    /// INITIALIZATION OF PUBLIC VARIABLES
    , currentPlayer(nullptr)
    , currentTheme(DARK_THEME)
    , currentColorTheme(BLUE_COLOR)
    , currentLanguage(SPANISH)
    , currentSongIndex(0)
    , fullPlaylist(true)
    , firstStart(true)
    , player1(nullptr)
    , player2(nullptr)
    , previousWindow(nullptr)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::MainWindow)
    , currentWidget(nullptr) // Will be set later
    , previousWidget(nullptr)
{
    ui->setupUi(this);

    setUILanguage(currentLanguage);
    setUITheme(currentTheme, currentColorTheme);

    setMusic();

    // Set the current widget as the Welcome window
    currentWidget = new WelcomeWindow(this);

    // Add the widget to the layout
    ui->layout->addWidget(currentWidget);

    // Register the default players (CPU and guests) with their details
    // Using the Connect4Lib library to manage the game
    Connect4 & game = Connect4::getInstance();
    QImage userAvatar(":/media/images/avatars/default.png");

    guest1 = game.registerPlayer(
        "Invitado 1"                          /* Nickname                     */
        , "invitado1@example.com"             /* Email                        */
        , "Password123!"                      /* Password                     */
        , QDate(1990, 1, 1)                   /* Date of birth                */
        , 0                                   /* Score (initially set to 100) */
        , userAvatar);                        /* User avatar                  */

    guest2 = game.registerPlayer(
        "Invitado 2"                          /* Nickname                     */
        , "invitado2@example.com"             /* Email                        */
        , "Password123!"                      /* Password                     */
        , QDate(1990, 1, 1)                   /* Date of birth                */
        , 0                                   /* Score (initially set to 100) */
        , userAvatar);                        /* User avatar                  */

    cpu = game.registerPlayer(
        "CPU"                                 /* Nickname                     */
        , "CPU@example.com"                   /* Email                        */
        , "Password123!"                      /* Password                     */
        , QDate(1990, 1, 1)                   /* Date of birth                */
        , 0                                   /* Score (initially set to 100) */
        , userAvatar);                        /* User avatar                  */

}   /* MainWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the MainWindow class destructor.
 * @param   void
 */
MainWindow::~MainWindow(void)
{
    // Delete the user interface (UI) object
    delete ui;

    if (currentWidget)
    {
        delete currentWidget;
    }

    if (previousWidget)
    {
        delete previousWidget;
    }

    if (previousWindow)
    {
        delete previousWindow;
    }

}   /* ~MainWindow() */

/******************************************************************************/
/*!
 * @brief   Returns the current widget displayed in the main window.
 * @param   void
 * @return  The current widget.
 */
QWidget * MainWindow::getCurrentWidget(void)
{
    return (currentWidget);

}   /* getCentralWidget() */

/******************************************************************************/
/*!
 * @brief   Returns the previous window before the current one.
 * @param   void
 * @return  The previous window.
 */
QWidget * MainWindow::getPreviousWindow(void)
{
    return (previousWindow);

}   /* getCentralWidget() */

/******************************************************************************/
/*!
 * @brief   Implementation of the MainWindow class's method to change the
 *          central widget.
 * @param   newWidget   Pointer to the new widget that will be set as the
 *                      central widget.
 * @param   changeType  Specifies the type of change (e.g., SAVE_LAST_WINDOW,
 *                      RECOVER_PREV_WIDGET).
 * @return  void
 */
void MainWindow::changeCentralWidget(
    QWidget * newWidget,
    changeType_t changeType)
{
    qDebug() << "newWidget: " << newWidget;

    if (!newWidget && (changeType != RECOVER_PREV_WIDGET &&
                       changeType != RECOVER_PREV_WINDOW))
    {
        qDebug() << "newWidget is null, can't change center widget";
    }
    else if (!previousWidget && (changeType == RECOVER_PREV_WIDGET))
    {
        qDebug() << "previousWidget is null, can't change center widget";
    }
    else
    {
        ui->layout->removeWidget(currentWidget);

        if (changeType == SAVE_PREV_WINDOW)
        {
            if (currentWidget)
            {
                if (previousWindow)
                {
                    delete previousWindow;
                    previousWindow = nullptr;
                }

                if (previousWindow == nullptr)
                {
                    previousWindow = currentWidget;
                }
                previousWidget = currentWidget;
                previousWidget->hide();
            }

            currentWidget = newWidget;
        }
        else if (changeType == RECOVER_PREV_WINDOW)
        {
            if (previousWindow)
            {
                qDebug() << "A previousWindow: " << previousWindow;
            }
            else
            {
                qDebug() << "A previousWindow no longer exists";
            }

            if (currentWidget)
            {
                delete currentWidget;
                currentWidget = nullptr;
            }

            if (previousWidget != previousWindow)
            {
                delete previousWidget;
            }

            previousWidget = nullptr;

            if (previousWindow)
            {
                currentWidget = previousWindow;
                previousWindow = nullptr;
                currentWidget->show();
            }

            qDebug() << "B currentWidget: " << currentWidget;
            qDebug() << "B previousWidget: " << previousWidget;
            qDebug() << "B previousWindow: " << previousWindow;
        }
        else if (changeType == DELETE_PREV_WIDGET)
        {
            if (currentWidget)
            {
                delete currentWidget;
                currentWidget = nullptr;
            }

            if (previousWidget != previousWindow)
            {
                delete previousWidget;
            }

            previousWidget = nullptr;

            currentWidget = newWidget;
        }
        else if (changeType == SAVE_PREV_WIDGET)
        {
            if (currentWidget)
            {
                previousWidget = currentWidget;
                previousWidget->hide();
            }

            currentWidget = newWidget;
        }
        else // (changeType == RECOVER_PREV_WIDGET)
        {
            if (currentWidget)
            {
                delete currentWidget;
                currentWidget = nullptr;
            }
            if (previousWidget)
            {
                currentWidget = previousWidget;
                previousWidget = nullptr;
                currentWidget->show();
            }
        }

        ui->layout->addWidget(currentWidget);

        qDebug() << "currentWidget: " << currentWidget;
        qDebug() << "previousWidget: " << previousWidget;
    }

}   /* changeCentralWidget() */

/******************************************************************************/
/*!
 * @brief   Sets the UI theme and color scheme.
 * @param   uiTheme  The theme type (e.g., DARK_THEME or LIGHT_THEME).
 * @param   uiColor  The color scheme type (e.g., BLUE_COLOR).
 * @return  void
 */
void MainWindow::setUITheme(uiTheme_t uiTheme, uiColorTheme_t uiColor)
{
    QString theme, color;

    switch (uiTheme)
    {
        case DARK_THEME:
            theme = "dark-theme";
            break;

        case LIGHT_THEME:
        default:
            theme = "light-theme";
            break;
    }

    switch (uiColor)
    {
        case BLUE_COLOR:
            color = "blue";
            break;

        case GREEN_COLOR:
            color = "green";
            break;

        case PINK_COLOR:
            color = "pink";
            break;

        case ORANGE_COLOR:
            color = "orange";
            break;

        default:
            color = "";
            break;
    }

    // Create a file path for the corresponding QSS file in resources
    QString path = ":/styles/" + theme + "/" + color + ".qss";

    qDebug() << path;

    QFile file(":/styles/" + theme + "/" + color + ".qss");
    if (file.open(QFile::ReadOnly)) // Try to open the file for reading
    {
        // Read the stylesheet from the file
        QString styleSheet = QString::fromUtf8(file.readAll());
        setStyleSheet(styleSheet);

        // Debug output for new style sheet selection
        qDebug() << "Theme set to dark mode (main.cpp)";

        file.close();
    }

}   /* setUITheme() */

/******************************************************************************/
/*!
 * @brief   Sets the UI theme and color scheme.
 * @param   uiLanguage  The language type (e.g., ENGLISH or SPANISH).
 * @return  void
 */
void MainWindow::setUILanguage(uiLanguage_t uiLanguage)
{
    Q_UNUSED(uiLanguage);

    QString language = "";

    if (currentLanguage == SPANISH)
    {
        language = "_es_ES";
    }
    else if (currentLanguage == ENGLISH)
    {
        language = "_en_GB";
    }
    else
    {
        ; // Do nothing
    }

    // Load translation file
    if (translator.load(":/translations/connect4" + language))
    {
        qDebug() << "Translation file loaded successfully: connect4" + language;
        // Install the translator to the application
        qApp->installTranslator(&translator);

        emit retranslateUIs();
    }
    else
    {
        qDebug() << "Translation file not found: connect4" + language;
    }

}   /* setUILanguage() */

/******************************************************************************/
/*!
 * @brief   Initializes the media player, audio output, and playlist.
 *          Sets up the playback configuration and starts playing the first song.
 * @param   void
 * @return  void
 */
void MainWindow::setMusic(void)
{
    // Create the media player and audio output
    mediaplayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;

    // Connect the mediaStatusChanged signal to the slot for handling song transitions
    connect(mediaplayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    // Set the volume to 5%
    audioOutput->setVolume(0.05); // Volume is in range from 0.0 to 1.0 (5% = 0.05)

    // Set the audio output for the media player
    mediaplayer->setAudioOutput(audioOutput);

    // Initialize song names
    Abyss_Theme = "qrc:/media/audio/Abyss_Theme.mp3";
    Mario_Theme = "qrc:/media/audio/Mario_Theme.mp3";
    MarioLuigi_Theme = "qrc:/media/audio/MarioLuigi_Theme.mp3";
    Pokemon_Theme = "qrc:/media/audio/Pokemon_Theme.mp3";
    Smash_Theme = "qrc:/media/audio/Smash_Theme.mp3";
    SpongeBob_Theme = "qrc:/media/audio/SpongeBob_Theme.mp3";
    Wii_Theme = "qrc:/media/audio/Wii_Theme.mp3";
    Yoshi_Theme = "qrc:/media/audio/Yoshi_Theme.mp3";
    Zelda_Theme = "qrc:/media/audio/Zelda_Theme.mp3";

    // Initialize the playlist
    playlist =
    {
        Abyss_Theme,
        Mario_Theme,
        MarioLuigi_Theme,
        Pokemon_Theme,
        Smash_Theme,
        SpongeBob_Theme,
        Wii_Theme,
        Yoshi_Theme,
        Zelda_Theme
    };

    // Set the first song for playback
    currentSong = playlist[currentSongIndex];

    qDebug() << "Playing:" << currentSong;
    mediaplayer->setSource(QUrl(currentSong));
    mediaplayer->play();

    // Increment the current song index for the next track
    currentSongIndex++;

}   /* setMusic() */

/******************************************************************************/
/*!
 * @brief   Handles the media status change event for the media player.
 *          This function is triggered when the media player changes its status,
 *          such as when a song finishes playing.
 * @param   status  The current media status, provided by QMediaPlayer::MediaStatus.
 * @return  void
 */
void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        qDebug() << "La canción ha terminado";

        if (fullPlaylist)
        {
            // Play the next song in the playlist
            currentSong = playlist[currentSongIndex];

            qDebug() << "Playing:" << currentSong;
            mediaplayer->setSource(QUrl(currentSong));
            mediaplayer->play();

            // Move to the next song, or loop back to the beginning
            currentSongIndex++;
            if (currentSongIndex >= playlist.size())
            {
                currentSongIndex = 0; // Restart the playlist
            }
        }
        else
        {
            // Replay the same song
            qDebug() << "Replaying:" << currentSong;
            mediaplayer->setSource(QUrl(currentSong));
            mediaplayer->play();
        }
    }

}   /* onMediaStatusChanged() */

/******************************************************************************/
/*!
 * @brief   Board Size Management Features.
 * @param   void
 * @return  The current board size.
 */
boardSize_t MainWindow::getBoardSize(void)
{
    return (boardSize);

}   /* getBoardSize() */

/******************************************************************************/
/*!
 * @brief   Board Size Management Features.
 * @param   newBoardSize  The new game mode to be set.
 * @return  void
 */
void MainWindow::setBoardSize(boardSize_t newBoardSize)
{
    boardSize = newBoardSize;

}   /* setBoardSize() */

/******************************************************************************/
/*!
 * @brief   Game Mode Management Features.
 * @param   void
 * @return  The current game mode.
 */
gameMode_t MainWindow::getGameMode(void)
{
    return (gameMode);

}   /* getGameMode() */

/******************************************************************************/
/*!
 * @brief   Game Mode Management Features.
 * @param   newGameMode  The new game mode to be set.
 * @return  void
 */
void MainWindow::setGameMode(gameMode_t newGameMode)
{
    gameMode = newGameMode;

}   /* setGameMode() */

/******************************************************************************/
/*!
 * @brief   Rounds Management Features.
 * @param   void
 * @return  The current number of rounds.
 */
int MainWindow::getRounds(void)
{
    return (rounds);

}   /* getRounds() */

/******************************************************************************/
/*!
 * @brief   Rounds Management Features.
 * @param   newRounds  The new number of rounds to be set.
 * @return  void
 */
void MainWindow::setRounds(int newRounds)
{
    rounds = newRounds;

}   /* setRounds() */


//-----[ IMPLEMENTATION OF PROTECTED MAINWINDOW FUNCTIONS ]-------------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINWINDOW FUNCTIONS ]---------------//


//-----[ IMPLEMENTATION OF PRIVATE MAINWINDOW FUNCTIONS ]---------------------//


/*** end of file ***/
