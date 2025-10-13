/**
 * @file     newgamewindow.cpp
 *
 * @version  0.2  (2024/01/24) Final version
 * @brief    Implementation of NewGameWindow class functions.
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
#include <QMessageBox>

/// Own includes
#include "newgamewindow.h"
#include "ui_newgamewindow.h"
#include "mainwindow.h"
#include "boardplayer.h"
#include "newgamepart2window.h"
#include "logwindow.h"
#include "singwindow.h"
#include "mainmenu.h"


//-----[ IMPLEMENTATION OF PUBLIC NEWGAMEWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   NewGameWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of NewGameWindow.
 */
NewGameWindow::NewGameWindow(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::NewGameWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
    // blue_active
    // red_active
    // green_active
    // purple_active
    // blue_inactive
    // red_inactive
    // green_inactive
    // purple_inactive
    , disabledCircle1(nullptr)
    , disabledCircle2(nullptr)
    // gameModeSelected;
    // player2Selected;
    , circleColor1Selected(false)
    , circleColor2Selected(false)

{
    ui->setupUi(this);

    mainwindow->playingAsGuest = false;

    ui->mainInitializeplayer2Frame->hide();
    ui->mainColorSelectionFrame->hide();
    ui->nextFrame->hide();

    ui->player1Label->setText(tr("Player 1 (") +
                              mainwindow->player1->getNickName() + ")");

    initializeIconNames();
    initializeIcons();
    initializeButtons();

    /// CONNECTING SIGNALS AND SLOTS

    connect(
        mainwindow
        , &MainWindow::userLogged
        , this
        , &NewGameWindow::onUserLogged);

}   /* NewGameWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the NewGameWindow class destructor.
 * @param   void
 */
NewGameWindow::~NewGameWindow(void)
{
    delete ui;

}   /* ~NewGameWindow() */


//-----[ IMPLEMENTATION OF PROTECTED NEWGAMEWINDOW FUNCTIONS ]----------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOTS NEWGAMEWINDOW FUNCTIONS ]------------//

/******************************************************************************/
/*!
 * @brief   This function manages the radio buttons that set the gamemode.
 * @param   void
 * @return  void
 */
void NewGameWindow::onRadioButtonToggled(void)
{
    QRadioButton * pressedButton = qobject_cast<QRadioButton *>(sender());

    if (!pressedButton)
    {
        return;
    }

    if (ui->PvsCPUButton->isChecked())
    {
        mainwindow->setGameMode(VS_CPU);
        ui->player2Label->setText("CPU");
        ui->mainInitializeplayer2Frame->hide();
    }
    else if (ui->PvsPButton->isChecked())
    {
        mainwindow->setGameMode(VS_PLAYER);
        ui->player2Label->setText(tr("Player 2"));
        ui->mainInitializeplayer2Frame->show();
    }

    ui->mainColorSelectionFrame->show();

    checkActivateNext();

}   /* onRadioButtonToggled() */

/******************************************************************************/
/*!
 * @brief   This function sets the second player as a guest.
 * @param   void
 * @return  void
 */
void NewGameWindow::onGuestPressed(void)
{
    if (ui->NGW_guestButton->isChecked())
    {
        player2Selected = true;
        this->mainwindow->playingAsGuest = true;
        ui->NGW_logInButton->setEnabled(false);
        ui->NGW_registerButton->setEnabled(false);

        if (mainwindow->player1 == mainwindow->guest1)
        {
            ui->player2Label->setText(tr("Player 2 (") +
                                      mainwindow->guest2->getNickName() + ")");
        }
        else
        {
            ui->player2Label->setText(tr("Player 2 (") +
                                      mainwindow->guest1->getNickName() + ")");
        }
    }
    else
    {
        player2Selected = false;
        this->mainwindow->playingAsGuest = false;
        ui->NGW_logInButton->setEnabled(true);
        ui->NGW_registerButton->setEnabled(true);

        ui->player2Label->setText(tr("Player 2"));
    }

    checkActivateNext();

}   /* onGuestPressed() */

/******************************************************************************/
/*!
 * @brief   This function leads the player to the login widget.
 * @param   void
 * @return  void
 */
void NewGameWindow::onLogInPressed(void)
{
    QWidget * newWidget = new LogWindow(mainwindow, PLAYER2);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

    checkActivateNext();

}   /* onLogInPressed() */

/******************************************************************************/
/*!
 * @brief   This function leads the player to the register widget.
 * @param   void
 * @return  void
 */
void NewGameWindow::onRegisterPressed(void)
{
    QWidget * newWidget = new SingWindow(mainwindow, PLAYER2);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

    checkActivateNext();

}   /* onRegisterPressed() */

/******************************************************************************/
/*!
 * @brief   This function manages the circle color selection of the player1.
 * @param   void
 * @return  void
 */
void NewGameWindow::onCircle1Pressed(void)
{
    QPushButton * pressedCircle = qobject_cast<QPushButton *>(sender());

    if (!pressedCircle)
    {
        return;
    }

    if (pressedCircle == disabledCircle1)
    {
        QMessageBox::warning(
            nullptr,
            tr("Error"),
            tr("Color already selected, please choose another."),
            QMessageBox::Ok);

        circleColor1Selected = false;
    }
    else
    {
        ui->blue1Button->setIcon(QIcon(blue_inactive));
        ui->red1Button->setIcon(QIcon(red_inactive));
        ui->green1Button->setIcon(QIcon(green_inactive));
        ui->purple1Button->setIcon(QIcon(purple_inactive));

        if (disabledCircle1 == nullptr)
        {
            ui->blue2Button->setIcon(QIcon(blue_active));
            ui->red2Button->setIcon(QIcon(red_active));
            ui->green2Button->setIcon(QIcon(green_active));
            ui->purple2Button->setIcon(QIcon(purple_active));
        }

        if (disabledCircle2 != nullptr)
        {
            disabledCircle2->setCursor(QCursor(Qt::PointingHandCursor));
        }

        if (pressedCircle == ui->blue1Button)
        {
            ui->blue1Button->setIcon(QIcon(blue_active));

            ui->blue2Button->setIcon(QIcon(blue_inactive));
            disabledCircle2 = ui->blue2Button;
            mainwindow->boardPlayer1.setCircleColor(BLUE);
        }
        else if (pressedCircle == ui->red1Button)
        {
            ui->red1Button->setIcon(QIcon(red_active));

            ui->red2Button->setIcon(QIcon(red_inactive));
            disabledCircle2 = ui->red2Button;
            mainwindow->boardPlayer1.setCircleColor(RED);
        }
        else if (pressedCircle == ui->green1Button)
        {
            ui->green1Button->setIcon(QIcon(green_active));

            ui->green2Button->setIcon(QIcon(green_inactive));
            disabledCircle2 = ui->green2Button;
            mainwindow->boardPlayer1.setCircleColor(GREEN);
        }
        else if (pressedCircle == ui->purple1Button)
        {
            ui->purple1Button->setIcon(QIcon(purple_active));

            ui->purple2Button->setIcon(QIcon(purple_inactive));
            disabledCircle2 = ui->purple2Button;
            mainwindow->boardPlayer1.setCircleColor(PURPLE);
        }

        // disabledCircle2 is not null when reaching this point
        disabledCircle2->setCursor(QCursor(Qt::ForbiddenCursor));

        circleColor1Selected = true;
    }

    checkActivateNext();

}   /* onCircle1Pressed() */

/******************************************************************************/
/*!
 * @brief   This function manages the circle color selection of the
 *          player2/guest/cpu.
 * @param   void
 * @return  void
 */
void NewGameWindow::onCircle2Pressed(void)
{
    QPushButton * pressedCircle = qobject_cast<QPushButton *>(sender());

    if (!pressedCircle)
    {
        return;
    }

    if (pressedCircle == disabledCircle2)
    {
        QMessageBox::warning(
            nullptr,
            tr("Error"),
            tr("Color already selected, please choose another."),
            QMessageBox::Ok);

        circleColor2Selected = false;
    }
    else
    {
        ui->blue2Button->setIcon(QIcon(blue_inactive));
        ui->red2Button->setIcon(QIcon(red_inactive));
        ui->green2Button->setIcon(QIcon(green_inactive));
        ui->purple2Button->setIcon(QIcon(purple_inactive));

        if (disabledCircle2 == nullptr)
        {
            ui->blue1Button->setIcon(QIcon(blue_active));
            ui->red1Button->setIcon(QIcon(red_active));
            ui->green1Button->setIcon(QIcon(green_active));
            ui->purple1Button->setIcon(QIcon(purple_active));
        }

        if (disabledCircle1 != nullptr)
        {
            disabledCircle1->setCursor(QCursor(Qt::PointingHandCursor));
        }

        if (pressedCircle == ui->blue2Button)
        {
            ui->blue2Button->setIcon(QIcon(blue_active));

            ui->blue1Button->setIcon(QIcon(blue_inactive));
            disabledCircle1 = ui->blue1Button;
            mainwindow->boardPlayer2.setCircleColor(BLUE);
        }
        else if (pressedCircle == ui->red2Button)
        {
            ui->red2Button->setIcon(QIcon(red_active));

            ui->red1Button->setIcon(QIcon(red_inactive));
            disabledCircle1 = ui->red1Button;
            mainwindow->boardPlayer2.setCircleColor(RED);
        }
        else if (pressedCircle == ui->green2Button)
        {
            ui->green2Button->setIcon(QIcon(green_active));

            ui->green1Button->setIcon(QIcon(green_inactive));
            disabledCircle1 = ui->green1Button;
            mainwindow->boardPlayer2.setCircleColor(GREEN);
        }
        else if (pressedCircle == ui->purple2Button)
        {
            ui->purple2Button->setIcon(QIcon(purple_active));

            ui->purple1Button->setIcon(QIcon(purple_inactive));
            disabledCircle1 = ui->purple1Button;
            mainwindow->boardPlayer2.setCircleColor(PURPLE);
        }

        // disabledCircle1 is not null when reaching this point
        disabledCircle1->setCursor(QCursor(Qt::ForbiddenCursor));

        circleColor2Selected = true;
    }

    checkActivateNext();

}   /* onCircle2Pressed() */

/******************************************************************************/
/*!
 * @brief   This function returns the player to the previous widget if there
 *          is one.
 * @param   void
 * @return  void
 */
void NewGameWindow::onBackPressed(void)
{
    QWidget * newWidget = new mainMenu(mainwindow);
    mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);

}   /* onBackPressed() */

/******************************************************************************/
/*!
 * @brief   This function leads the player to the next widget
 *          (newgamepart2window).
 * @param   void
 * @return  void
 */
void NewGameWindow::onNextPressed(void)
{
    QWidget * newWidget = new NewGamePart2Window(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* onNextPressed() */


//-----[ IMPLEMENTATION OF PRIVATE NEWGAMEWINDOW FUNCTIONS ]------------------//

/******************************************************************************/
/*!
 * @brief  This function sets the button icons names.
 * @param  void
 * @return void
 */
void NewGameWindow::initializeIconNames(void)
{
    blue_active     = ":/media/images/circles/active/blue.png";
    red_active      = ":/media/images/circles/active/red.png";
    green_active    = ":/media/images/circles/active/green.png";
    purple_active   = ":/media/images/circles/active/purple.png";

    blue_inactive   = ":/media/images/circles/inactive/blue.png";
    red_inactive    = ":/media/images/circles/inactive/red.png";
    green_inactive  = ":/media/images/circles/inactive/green.png";
    purple_inactive = ":/media/images/circles/inactive/purple.png";

}   /* initializeIconNames() */

/******************************************************************************/
/*!
 * @brief  This function sets the initial button icons.
 * @param  void
 * @return void
 */
void NewGameWindow::initializeIcons(void)
{
    ui->blue1Button->setIcon(QIcon(blue_active));
    ui->red1Button->setIcon(QIcon(red_active));
    ui->green1Button->setIcon(QIcon(green_active));
    ui->purple1Button->setIcon(QIcon(purple_active));

    ui->blue2Button->setIcon(QIcon(blue_active));
    ui->red2Button->setIcon(QIcon(red_active));
    ui->green2Button->setIcon(QIcon(green_active));
    ui->purple2Button->setIcon(QIcon(purple_active));

}   /* initializeIcons() */

/******************************************************************************/
/*!
 * @brief   This function connects the buttons with the linked functions.
 * @param   void
 * @return  void
 */
void NewGameWindow::initializeButtons(void)
{
    connect(
        ui->PvsCPUButton
        , &QRadioButton::toggled
        , this
        , &NewGameWindow::onRadioButtonToggled);

    connect(
        ui->PvsPButton
        , &QRadioButton::toggled
        , this
        , &NewGameWindow::onRadioButtonToggled);


    connect(
        ui->NGW_registerButton
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onRegisterPressed);

    connect(
        ui->NGW_logInButton
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onLogInPressed);

    connect(
        ui->NGW_guestButton
        , &QCheckBox::clicked
        , this
        , &NewGameWindow::onGuestPressed);


    connect(
        ui->blue1Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle1Pressed);

    connect(
        ui->red1Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle1Pressed);

    connect(
        ui->green1Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle1Pressed);

    connect(
        ui->purple1Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle1Pressed);


    connect(
        ui->blue2Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle2Pressed);

    connect(
        ui->red2Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle2Pressed);

    connect(
        ui->green2Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle2Pressed);

    connect(
        ui->purple2Button
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onCircle2Pressed);


    connect(
        ui->backButton
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onBackPressed);

    connect(
        ui->nextButton
        , &QPushButton::clicked
        , this
        , &NewGameWindow::onNextPressed);

}   /* initializeButtons() */

/******************************************************************************/
/*!
 * @brief   This function evaluates conditions to determine if the next button
 *          should be enabled.
 * @param   void
 * @return  void
 */
void NewGameWindow::checkActivateNext(void)
{
    bool enable = false;

    if (circleColor1Selected && circleColor2Selected)
    {
        if (player2Selected || mainwindow->getGameMode()== VS_CPU)
        {
            enable = true;
            ui->nextFrame->show();
        }
        else
        {
            ui->nextFrame->hide();
        }
    }

    ui->nextButton->setEnabled(enable);

}   /* checkActivateNext() */

/******************************************************************************/
/*!
 * @brief   This function is called when a user logs in.
 *          It sets the player2Selected variable to true and updates the UI.
 * @param   void
 * @return  void
 */
void NewGameWindow::onUserLogged(void)
{
    player2Selected = true;
    checkActivateNext();

    ui->player2Label->setText(tr("Player 2 (") +
                              mainwindow->player2->getNickName() + ")");

}   /* onUserLogged() */


/*** end of file ***/
