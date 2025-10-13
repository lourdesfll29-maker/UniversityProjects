/**
 * @file     newgamepart2window.cpp
 *
 * @brief    Implementation of NewGamePart2Window class functions.
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
#include "newgamepart2window.h"
#include "ui_newgamepart2window.h"
#include "mainwindow.h"
#include "newgamewindow.h"
#include "gameboard.h"


//-----[ IMPLEMENTATION OF PUBLIC NEWGAMEPART2WINDOW FUNCTIONS ]--------------//

/******************************************************************************/
/*!
 * @brief   NewGamePart2Window class constructor implementation.
 * @param   parent  Used to set the parent widget of NewGamePart2Window.
 */
NewGamePart2Window::NewGamePart2Window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewGamePart2Window)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    ui->setupUi(this);

    // By default, these parameters are set
    this->mainwindow->setBoardSize(SMALL);
    this->mainwindow->setRounds(1);

    setBoardPreview();

    connect(
        ui->backButton
        , &QPushButton::clicked
        , this
        , &NewGamePart2Window::onBackPressed);

    connect(
        ui->startGameButton
        , &QPushButton::clicked
        , this
        , &NewGamePart2Window::onStartGamePressed);

    connect(
        ui->smallButton
        , &QRadioButton::toggled
        , this
        , &NewGamePart2Window::onSizeRadioButtonToggled);

    connect(
        ui->mediumButton
        , &QRadioButton::toggled
        , this
        , &NewGamePart2Window::onSizeRadioButtonToggled);

    connect(
        ui->bigButton
        , &QRadioButton::toggled
        , this
        , &NewGamePart2Window::onSizeRadioButtonToggled);

    connect(
        ui->oneButton
        , &QRadioButton::toggled
        , this
        , &NewGamePart2Window::onRoundsRadioButtonToggled);

    connect(
        ui->twoButton
        , &QRadioButton::toggled
        , this
        , &NewGamePart2Window::onRoundsRadioButtonToggled);

    connect(
        ui->threeButton
        , &QRadioButton::toggled
        , this
        , &NewGamePart2Window::onRoundsRadioButtonToggled);

}   /* NewGamePart2Window() */

/******************************************************************************/
/*!
 * @brief   Implementation of the NewGamePart2Window class destructor.
 * @param   void
 */
NewGamePart2Window::~NewGamePart2Window()
{
    delete ui;

}   /* ~NewGamePart2Window() */


//-----[ IMPLEMENTATION OF PROTECTED NEWGAMEPART2WINDOW FUNCTIONS ]-----------//


//-----[ IMPLEMENTATION OF PRIVATE SLOTS NEWGAMEPART2WINDOW FUNCTIONS ]-------//

/******************************************************************************/
/*!
 * @brief   This function returns the player to the previous widget
 *          (newgamewindow).
 * @param   void
 * @return  void
 */
void NewGamePart2Window::onBackPressed(void)
{
    mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WIDGET);

}   /* onBackPressed() */

/******************************************************************************/
/*!
 * @brief  This function leads the player to the gameboard widget.
 * @param  void
 * @return void
 */
void NewGamePart2Window::onStartGamePressed(void)
{
    QWidget * newWidget = new GameBoard(mainwindow);
    mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);

}   /* onStartGamePressed() */

/******************************************************************************/
/*!
 * @brief   This function manages the radio buttons dedicated to the board size.
 * @param   void
 * @return  void
 */
void NewGamePart2Window::onSizeRadioButtonToggled(void)
{
    QRadioButton *pressedButton = qobject_cast<QRadioButton *>(sender());

    if (!pressedButton)
    {
        return;
    }

    setBoardPreview();

}   /* onSizeRadioButtonToggled() */

/******************************************************************************/
/*!
 * @brief   This function manages the radio buttons dedicated to the board
 *          rounds.
 * @param   void
 * @return  void
 */
void NewGamePart2Window::onRoundsRadioButtonToggled(void)
{
    QRadioButton * pressedButton = qobject_cast<QRadioButton *>(sender());

    if (!pressedButton)
    {
        return;
    }

    if (ui->oneButton->isChecked())
    {
        this->mainwindow->setRounds(1);
    }
    else if (ui->twoButton->isChecked())
    {
        this->mainwindow->setRounds(2);
    }
    else if (ui->threeButton->isChecked())
    {
        this->mainwindow->setRounds(3);
    }

}   /* onRoundsRadioButtonToggled() */


//-----[ IMPLEMENTATION OF PRIVATE NEWGAMEPART2WINDOW FUNCTIONS ]-------------//

/******************************************************************************/
/*!
 * @brief   This function displays the board preview image when a size is
 *          selected.
 * @param   void
 * @return  void
 */
void NewGamePart2Window::setBoardPreview(void)
{
    QString theme;
    QString size;

    if (mainwindow->currentTheme == DARK_THEME)
    {
        theme = "dark-theme";
    }
    else // (mainwindow->currentTheme == LIGHT_THEME)
    {
        theme = "light-theme";
    }

    if (ui->smallButton->isChecked())
    {
        this->mainwindow->setBoardSize(SMALL);
        size = "board_small";
    }
    else if (ui->mediumButton->isChecked())
    {
        this->mainwindow->setBoardSize(MEDIUM);
        size = "board_medium";
    }
    else if (ui->bigButton->isChecked())
    {
        this->mainwindow->setBoardSize(BIG);
        size = "board_big";
    }

    QPixmap boardPreview;

    boardPreview = ":media/images/boards/" + theme + "/" + size + ".png";

    // Adjusted size so that the width is greater than the height
    boardPreview = boardPreview.scaled(180.0, 180.0 / 1.16);

    ui->sizeimageLabel->setPixmap(boardPreview);

}   /* setBoardPreview() */


/*** end of file ***/
