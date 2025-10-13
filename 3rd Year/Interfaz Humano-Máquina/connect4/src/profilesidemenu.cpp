/**
 * @file     profilesidemenu.cpp
 *
 * @brief    Implementation of ProfileSideMenu class functions.
 *
 * @version  0.6  (2024/01/24) Final version
 * @version  0.5  (2024/01/23) BARR-C programming standard needs to be revised
 * @version  0.4  (2025/01/07) Almost final version
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
#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QImage>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QMessageBox>

/// Own includes
#include "profilesidemenu.h"
#include "ui_profilesidemenu.h"
#include "mainwindow.h"
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


//-----[ IMPLEMENTATION OF PUBLIC PROFILESIDEMENU FUNCTIONS ]-----------------//

/******************************************************************************/
/*!
 * @brief   ProfileSideMenu class constructor implementation.
 * @param   parent  Used to set the parent widget of ProfileSideMenu.
 */
ProfileSideMenu::ProfileSideMenu(QWidget * parent)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::ProfileSideMenu)
    , menuIsVisible(false)
    , parent(parentWidget())
    , mainwindow(qobject_cast<MainWindow *>(parent->parentWidget()))
{
    // Set up the UI components for ProfileSideMenu
    ui->setupUi(this);

    /// CONNECTING SIGNALS AND SLOTS

    connect(
        mainwindow
        , &MainWindow::userLogged
        , this
        , &ProfileSideMenu::updateUser);

    connect(
        ui->exitButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::hideMenu);

    connect(
        ui->configButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onConfigButtonClicked);

    connect(
        ui->seeProfileButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onSeeProfileButtonClicked);

    connect(
        ui->rankingButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onRankingButtonClicked);

    connect(
        ui->logoutButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onLogOutButtonClicked);

    connect(
        ui->changeThemeButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onChangeThemeButtonClicked);

    connect(
        ui->helpButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onHelpButtonClicked);

    connect(
        ui->musicButton
        , &QPushButton::clicked
        , this
        , &ProfileSideMenu::onMusicButtonClicked);

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
                ui->musicButton->setText(tr("Music on"));
                ui->musicButton->setChecked(false);
            }
            else
            {
                ui->musicButton->setText(tr("Music off"));
                ui->musicButton->setChecked(true);
            }
        });

    /// SET INITIAL CONFIGURATION OF THE INTERFACE

    this->hide();
    this->setFixedWidth(300);

    if (mainwindow->mediaplayer->isPlaying())
    {
        ui->musicButton->setText(tr("Music on"));
        ui->musicButton->setChecked(false);
    }
    else
    {
        ui->musicButton->setText(tr("Music off"));
        ui->musicButton->setChecked(true);
    }

    updateUser();

}   /* ProfileSideMenu() */

/******************************************************************************/
/*!
 * @brief   Implementation of the ProfileSideMenu class destructor.
 * @param   void
 */
ProfileSideMenu::~ProfileSideMenu(void)
{
    // Delete the UI components associated with ProfileSideMenu
    delete ui;

}   /* ~ProfileSideMenu() */

/******************************************************************************/
/*!
 * @brief   Checks if the menu is currently visible.
 * @param   void
 * @return  True if the menu is visible, false otherwise.
 */
bool ProfileSideMenu::isMenuVisible(void)
{
    return (menuIsVisible);

}   /* isMenuVisible() */

/******************************************************************************/
/*!
 * @brief   Displays the side menu.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::showMenu(void)
{
    /// SET BLUR FILTER ON THE CENTRAL WIDGET OF THE PARENT

    // Ensure the central widget is accessible
    QWidget * widget = parent->findChild<QWidget *>("centralwidget");

    if (widget)
    {
        QList<QWidget*> children = widget->findChildren<QWidget*>();

        // Iterate through all children and set their enabled state
        for (QWidget * child : children)
        {
            child->setEnabled(false); // Disable child widgets
            child->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        }

        // Apply a blur effect to the main window
        QGraphicsBlurEffect * blurEffect = new QGraphicsBlurEffect(widget);
        blurEffect->setBlurRadius(10); // Set the blur radius
        widget->setGraphicsEffect(blurEffect);
    }
    else
    {
        // Log if central widget is not found
        qDebug() << "Centralwidget not found (ProfileSideMenu).";
    }

    /// SET FOCUS SO THAT ESCAPE KEY CAN BE DETECTED

    setFocus();

    /// SHOW MENU AND PROFILE INFORMATION

    //Player * player = mainwindow->currentPlayer;
    Player * player = mainwindow->player1;

    if (player)
    {
        QImage userAvatar = player->getAvatar();

        // Convert QImage to QPixmap, then to QIcon
        QPixmap avatarPixmap = QPixmap::fromImage(userAvatar);
        ui->userAvatar->setPixmap(avatarPixmap); // Set user avatar

        ui->nickNameLabel->setText(player->getNickName());

        QString puntuacion = tr("Score: ") +
                             QString::number(player->getPoints());
        ui->pointsLabel->setText(puntuacion); // Set player score
    }

    this->showAnimation();

    updateUser();

}   /* showMenu() */

/******************************************************************************/
/*!
 * @brief   Hides the side menu.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::hideMenu(void)
{
    /// HIDE MENU

    this->hideAnimation();

    /// CLEAR FOCUS SO THAT ESCAPE KEY CAN BE DETECTED

    clearFocus();

    /// RESTORE NORMAL BEHAVIOR OF PARENT'S CENTRAL WIDGET

    // Ensure the central widget is accessible
    QWidget * widget = parent->findChild<QWidget *>("centralwidget");
    widget->setFocus();

    if (widget)
    {
        QList<QWidget*> children = widget->findChildren<QWidget*>();

        // Iterate through all children and set their enabled state
        for (QWidget * child : children)
        {
            child->setEnabled(true);
            child->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        }

        // Restore the graphical effect of the main window
        widget->setGraphicsEffect(nullptr);
    }
    else
    {
        // Log if central widget is not found
        qDebug() << "Centralwidget not found (ProfileSideMenu).";
    }

}   /* hideMenu() */

/******************************************************************************/
/*!
 * @brief   Resizes the side menu based on parent widget.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::resizeMenu(void)
{
    // Adjust widget size
    this->setFixedHeight(parent->height());

    // Position the widget on the right side of parent widget
    this->move(parent->width() - this->width(), 0);

}   /* resizeMenu() */


//-----[ IMPLEMENTATION OF PROTECTED PROFILESIDEMENU FUNCTIONS ]--------------//

/******************************************************************************/
/*!
 * @brief   This function runs each time a key is pressed.
 * @param   event  Recieves the current QKeyEvent.
 * @return  void
 */
void ProfileSideMenu::keyPressEvent(QKeyEvent * event)
{
    // If the escape key is pressed...
    if (event->key() == Qt::Key_Escape)
    {
        hideMenu();
    }
    else
    {
        ; // Do nothing
    }

}   /* keyPressEvent() */


//-----[ IMPLEMENTATION OF PRIVATE SLOT PROFILESIDEMENU FUNCTIONS ]-----------//

/******************************************************************************/
/*!
 * @brief   Opens the profile window when the "See Profile" button is clicked.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onSeeProfileButtonClicked(void)
{
    QWidget * newWidget = new ProfileWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

}   /* onSeeProfileButtonClicked() */

/******************************************************************************/
/*!
 * @brief   Opens the ranking window when the "Ranking" button is clicked.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onRankingButtonClicked(void)
{
    // TODO
    //QWidget * newWidget = new RankingWindow(mainwindow);
    //mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

}   /* onRankingButtonClicked() */

/******************************************************************************/
/*!
 * @brief   Handles the logout functionality when the "Log Out" button is
 *          clicked.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onLogOutButtonClicked(void)
{
    // Create the QMessageBox
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Confirmation needed"));

    // Create the message with HTML to format it
    QString message = tr("Are you sure you want to log in? Your <br>"
                         "progress in the current game will be lost.");

    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    // Adjust the size of the QMessageBox
    msgBox.setFixedSize(300, 200);

    // Display the QMessageBox in modal mode
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        QWidget * newWidget = new WelcomeWindow(mainwindow);
        mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
    }
    else
    {
        hideMenu();
    }

}   /* onLogOutButtonClicked() */

/******************************************************************************/
/*!
 * @brief   Toggles between light and dark themes.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onChangeThemeButtonClicked(void)
{
    if (mainwindow->currentTheme == DARK_THEME)
    {
        mainwindow->currentTheme = LIGHT_THEME;
    }
    else
    {
        mainwindow->currentTheme = DARK_THEME;
    }

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

    /// SET BLUR FILTER ON THE CENTRAL WIDGET OF THE PARENT (AGAIN)

    // Ensure the central widget is accessible
    QWidget * widget = parent->findChild<QWidget *>("centralwidget");

    if (widget)
    {
        // Restore the graphical effect of the main window
        widget->setGraphicsEffect(nullptr);

        // Apply a blur effect to the main window
        QGraphicsBlurEffect * blurEffect = new QGraphicsBlurEffect(widget);
        blurEffect->setBlurRadius(10); // Set the blur radius
        widget->setGraphicsEffect(blurEffect);
    }
    else
    {
        // Log if central widget is not found
        qDebug() << "Centralwidget not found (ProfileSideMenu).";
    }

}   /* onChangeThemeButtonClicked() */

/******************************************************************************/
/*!
 * @brief   Opens the options window to configure the application settings.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onConfigButtonClicked(void)
{
    QWidget * newWidget = new OptionsWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* onConfigButtonClicked() */

/******************************************************************************/
/*!
 * @brief   Opens the About window to display information about the application.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onHelpButtonClicked(void)
{
    QWidget * newWidget = new AboutWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* onHelpButtonClicked() */

/******************************************************************************/
/*!
 * @brief   Handles the "Sign In" button click. Asks for confirmation if the
 *          user is in a game.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::on_singInButton_clicked(void)
{
    if (qobject_cast<GameBoard *>(mainwindow->getCurrentWidget()) != nullptr)
    {
        // Create the QMessageBox
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Confirmation needed"));

        // Create the message with HTML to format it
        QString message = tr("Are you sure you want to log in? Your <br>"
                             "progress in the current game will be lost.");

        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        // Adjust the size of the QMessageBox
        msgBox.setFixedSize(300, 200);

        // Display the QMessageBox in modal mode
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            QWidget * newWidget = new LogWindow(mainwindow, PLAYER1);
            mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);
        }
        else
        {
            hideMenu();
        }
    }
    else
    {
        QWidget * newWidget = new LogWindow(mainwindow, PLAYER1);
        mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);
    }

}   /* on_singInButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Toggles the playback state of the media player. If the music is
 *          currently playing, it will pause it. Otherwise, it starts playback.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::onMusicButtonClicked(void)
{  
    if (ui->musicButton->isChecked())
    {
        ui->musicButton->setText(tr("Music off"));
        // Pause the song if it is currently playing
        mainwindow->mediaplayer->pause();
    }
    else
    {
        ui->musicButton->setText(tr("Music on"));
        // Play the song if it is paused or stopped
        mainwindow->mediaplayer->play();
    }

    emit mainwindow->musicChangedState();

}   /* onMusicButtonClicked() */


//-----[ IMPLEMENTATION OF PRIVATE PROFILESIDEMENU FUNCTIONS ]----------------//

/******************************************************************************/
/*!
 * @brief  Show the side menu with an animation.
 * @param  void
 * @return void
 */
void ProfileSideMenu::showAnimation(void)
{
    menuIsVisible = true; // Mark menu as visible

    resizeMenu(); // Resize the menu

    // Ensure that the widget is above other widgets
    this->raise();

    // Create animation to show the menu
    QPropertyAnimation * showAnimation = new QPropertyAnimation(this, "pos");
    showAnimation->setDuration(100); // Duration of the animation
    showAnimation->setStartValue(QPoint(parent->width(), 0));
    showAnimation->setEndValue(QPoint(parent->width() - this->width(), 0));
    showAnimation->start();

    this->show();

}   /* showAnimation() */

/******************************************************************************/
/*!
 * @brief  Hides the side menu with an animation.
 * @param  void
 * @return void
 */
void ProfileSideMenu::hideAnimation(void)
{
    menuIsVisible = false;

    // Create animation to hide the menu
    QPropertyAnimation * hideAnimation = new QPropertyAnimation(this, "pos");
    hideAnimation->setDuration(50); // Duration of the animation
    hideAnimation->setStartValue(this->pos()); // Current position
    hideAnimation->setEndValue(QPoint(parent->width(), 0)); // Move off-screen

    // Connect the finished signal to hide the widget after the animation
    connect(
        hideAnimation
        , &QPropertyAnimation::finished
        , this
        , [this]() { this->hide(); } ); // Hide the widget after the animation

    hideAnimation->start(); // Start the animation

}   /* hideAnimation() */

/******************************************************************************/
/*!
 * @brief   Updates the user interface based on the current player status.
 *          If player1 is logged in and is not a guest, it shows the
 *          player-related UI elements.
 *
 *          Otherwise, it hides them and shows the guest-related UI elements.
 * @param   void
 * @return  void
 */
void ProfileSideMenu::updateUser(void)
{
    if (mainwindow->player1 &&
        mainwindow->player1 != mainwindow->guest1)
    {
        ui->withoutPlayer->hide();
        ui->verticalSpacer_5->changeSize(
            0, 40, QSizePolicy::Ignored, QSizePolicy::Maximum);
        ui->withPlayer->show();
        ui->line_1->show();
        ui->logoutButton->show();
    }
    else
    {
        ui->withoutPlayer->show();
        ui->verticalSpacer_5->changeSize(
            0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->withPlayer->hide();
        ui->line_1->hide();
        ui->logoutButton->hide();
    }

}   /* updateUser() */


/*** end of file ***/
