/**
 * @file     mainmenu.cpp
 *
 * @brief    Implementation of GameBoard class functions.
 *
 * @version  0.2  (2024/01/24) Final version
 * @version  0.1  (2024/01/23) BARR-C programming standard needs to be revised
 *
 * @authors  Marcos Belda Martinez' <mbelmar@etsinf.upv.es>,
 *           Angela Espert Cornejo' <aespcor@etsinf.upv.es>,
 *           Lourdes Frances Llimera' <lfralli@epsa.upv.es>
 *
 * @date     January, 2025
 * @section  IHM-GIIROB
 */

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes

/// Own includes
#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "profilesidemenu.h"
#include "newgamewindow.h"
#include "aboutwindow.h"
#include "rankingwindow.h"
#include "optionswindow.h"
#include "historywindow.h"
#include "mainwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC MAINMENU FUNCTIONS ]------------------------//

/******************************************************************************/
/*!
 * @brief   mainMenu class constructor implementation.
 * @param   parent  Used to set the parent widget of mainMenu.
 */
mainMenu::mainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainMenu)
    , sideMenu(new ProfileSideMenu(this))
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    ui->setupUi(this);

    // Set the icon for the profile button
    ui->profileButton->setIcon(
        QPixmap::fromImage(mainwindow->player1->getAvatar()));

    connect(
        ui->profileButton
        , &QPushButton::clicked
        , this
        , &mainMenu::onProfileButtonClicked);

    connect(
        ui->newGameButton
        , &QPushButton::clicked
        , this
        , &mainMenu::onNewGameClicked);

    connect(
        ui->rankingButton
        , &QPushButton::clicked
        , this
        , &mainMenu::on_rankingButton_clicked);

    connect(
        ui->musicButton
        , &QPushButton::clicked
        , this
        , &mainMenu::onMusicButtonClicked);

    connect(
        mainwindow
        , &MainWindow::userLogged
        , this
        , [=](void) {
            ui->profileButton->setIcon(
                QPixmap::fromImage(mainwindow->player1->getAvatar())); });

    connect(
        mainwindow
        , &MainWindow::retranslateUIs
        , this
        , [=](void) { ui->retranslateUi(this); });

    connect(
        mainwindow
        , &MainWindow::musicChangedState
        , this
        , [=](void) {
            if (mainwindow->mediaplayer->isPlaying())
            {
                ui->musicButton->setChecked(false);
            }
            else
            {
                ui->musicButton->setChecked(true);
            }
        });

    if (mainwindow->mediaplayer->isPlaying())
    {
        ui->musicButton->setChecked(false);
    }
    else
    {
        ui->musicButton->setChecked(true);
    }

}   /* mainMenu() */

/******************************************************************************/
/*!
 * @brief   Implementation of the mainMenu class destructor.
 * @param   void
 */
mainMenu::~mainMenu(void)
{
    delete ui;

}   /* ~mainMenu() */


//-----[ IMPLEMENTATION OF PROTECTED MAINMENU FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief   This function runs each time a window resize is detected.
 * @param   event  Recieves the current QResizeEvent
 * @return  void
 */
void mainMenu::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event);

    // Resize the side menu if it is visible
    if (sideMenu->isMenuVisible())
    {
        sideMenu->resizeMenu(); // Adjust the size of the side menu
    }

}   /* resizeEvent() */

/******************************************************************************/
/*!
 * @brief  This function runs each time a mouse click is detected.
 * @param  event recieves the current QMouseEvent
 * @return void
 */
void mainMenu::mousePressEvent(QMouseEvent * event)
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

}   /* mousePressEvent() */


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINMENU FUNCTIONS ]-----------------//

/******************************************************************************/
/*!
 * @brief   This function shows the sideMenu when the profile button is pressed.
 * @param   void
 * @return  void
 */
void mainMenu::onProfileButtonClicked(void)
{
    // Call the function to display the side menu
    sideMenu->showMenu();

}   /* onProfileButtonClicked() */

/******************************************************************************/
/*!
 * @brief   This function leads to the newGameWindow when de newGame button is
 *          pressed.
 * @param   void
 * @return  void
 */
void mainMenu::onNewGameClicked(void)
{
    QWidget * newWidget = new NewGameWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* onNewGameClicked() */

/******************************************************************************/
/*!
 * @brief   Opens the About Window when the "About" button is clicked.
 * @param   void
 * @return  void
 */
void mainMenu::on_aboutButton_clicked(void)
{
    QWidget * newWidget = new AboutWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_aboutButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Exits the application when the "Quit" button is clicked.
 * @param   void
 * @return  void
 */
void mainMenu::on_quitButton_clicked(void)
{
    mainwindow->close();

}   /* on_quitButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Opens the ranking window when the "Ranking" button is clicked.
 * @param   void
 * @return  void
 */
void mainMenu::on_rankingButton_clicked(void)
{
    QWidget * newWidget = new RankingWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

}   /* on_rankingButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Navigate to the Hystory Window.
 * @param   void
 * @return  void
 */
void mainMenu::on_historyButton_clicked(void)
{
    QWidget * newWidget = new HistoryWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_historyButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Opens the options window when the "Options" button is clicked.
 * @param   void
 * @return  void
 */
void mainMenu::on_optionsButton_clicked(void)
{
    QWidget * newWidget = new OptionsWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_optionsButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Toggles the playback state of the media player. If the music is
 *          currently playing, it will pause it. Otherwise, it starts playback.
 * @param   void
 * @return  void
 */
void mainMenu::onMusicButtonClicked(void)
{
    // Check the state of the QMediaPlayer
    if (mainwindow->mediaplayer->isPlaying())
    {
        // Pause the song if it is currently playing
        mainwindow->mediaplayer->pause();
    }
    else
    {
        // Play the song if it is paused or stopped
        mainwindow->mediaplayer->play();
    }

    emit mainwindow->musicChangedState();

}   /* onMusicButtonClicked() */

//-----[ IMPLEMENTATION OF PRIVATE MAINMENU FUNCTIONS ]------------------//


/*** end of file ***/

