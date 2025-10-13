/**
 * @file     welcomewindow.cpp
 *
 * @brief    Implementation of WelcomeWindow class functions.
 *
 * @version  0.2  (2024/01/24) Final version
 * @version  0.1  (2024/01/23) BARR-C programming standard needs to be reviewed
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
#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include "startwindow.h"
#include "aboutwindow.h"
#include "optionswindow.h"
#include "rankingwindow.h"
#include "historywindow.h"


//-----[ IMPLEMENTATION OF PUBLIC WELCOMEWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   WelcomeWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of WelcomeWindow.
 */
WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    ui->setupUi(this);

    connect(
        ui->musicButton
        , &QPushButton::clicked
        , this
        , &WelcomeWindow::onMusicButtonClicked);

    connect(
        mainwindow
        , &MainWindow::retranslateUIs
        , this
        , [=](void) { ui->retranslateUi(this); });

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

    if (!(mainwindow->firstStart))
    {
        if (mainwindow->mediaplayer->isPlaying())
        {
            ui->musicButton->setChecked(false);
        }
        else
        {
            ui->musicButton->setChecked(true);
        }
    }
    else
    {
        mainwindow->firstStart = false;
    }

}   /* WelcomeWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the WelcomeWindow class destructor.
 * @param   void
 */
WelcomeWindow::~WelcomeWindow(void)
{
    delete ui;

}   /* ~WelcomeWindow() */


//-----[ IMPLEMENTATION OF PROTECTED WELCOMEWINDOW FUNCTIONS ]----------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT WELCOMEWINDOW FUNCTIONS ]-------------//

/******************************************************************************/
/*!
 * @brief   Navigate to the Start Window.
 * @param   void
 * @return  void
 */
void WelcomeWindow::on_startButton_clicked(void)
{
    QWidget * newWidget = new StartWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);

}   /* on_startButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Navigate to the About Window.
 * @param   void
 * @return  void
 */
void WelcomeWindow::on_aboutButton_clicked(void)
{
    QWidget * newWidget = new AboutWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_aboutButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Close the main window.
 * @param   void
 * @return  void
 */
void WelcomeWindow::on_quitButton_clicked(void)
{
    mainwindow->close();

}   /* on_quitButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Navigate to the Hystory Window.
 * @param   void
 * @return  void
 */
void WelcomeWindow::on_historyButton_clicked(void)
{
    QWidget * newWidget = new HistoryWindow(mainwindow);

    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_historyButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Navigate to the Ranking Window.
 * @param   void
 * @return  void
 */
void WelcomeWindow::on_rankingButton_clicked(void)
{
    QWidget * newWidget = new RankingWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

}   /* on_rankingButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Navigate to the Options Window.
 * @param   void
 * @return  void
 */
void WelcomeWindow::on_optionsButton_clicked(void)
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
void WelcomeWindow::onMusicButtonClicked(void)
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


//-----[ IMPLEMENTATION OF PRIVATE WELCOMEWINDOW FUNCTIONS ]------------------//


/*** end of file ***/
