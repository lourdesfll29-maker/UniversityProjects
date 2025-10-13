/**
 * @file     startwindow.cpp
 *
 * @brief    Definition of StartWindow class functions.
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
#include "startwindow.h"
#include "ui_startwindow.h"
#include "singwindow.h"
#include "logwindow.h"
#include "mainmenu.h"
#include "welcomewindow.h"
#include "optionswindow.h"


//-----[ IMPLEMENTATION OF PUBLIC STARTWINDOW FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief   StartWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of StartWindow.
 */
StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    ui->setupUi(this);

}   /* StartWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the SingWindow class destructor.
 * @param   void
 */
StartWindow::~StartWindow(void)
{
    delete ui;

}   /* ~StartWindow() */


//-----[ IMPLEMENTATION OF PROTECTED STARTWINDOW FUNCTIONS ]------------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT STARTWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief   Opens the login window when the log button is clicked.
 * @param   void
 * @return  void
 */
void StartWindow::on_logButton_clicked(void)
{
    QWidget * newWidget = new LogWindow(mainwindow, PLAYER1);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

}   /* on_logButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Opens the registration window when the register button is clicked.
 * @param   void
 * @return  void
 */
void StartWindow::on_registerButton_clicked(void)
{
    QWidget * newWidget = new SingWindow(mainwindow, PLAYER1);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WINDOW);

}   /* on_registerButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Sets player1 to a guest and opens the main menu.
 * @param   void
 * @return  void
 */
void StartWindow::on_guestButton_clicked(void)
{
    mainwindow->player1 = mainwindow->guest1;

    QWidget * newWidget = new mainMenu(mainwindow);
    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_guestButton_clicked() */

/******************************************************************************/
/*!
 * @brief  Returns to the welcome window when the back button is clicked.
 * @param  void
 * @return void
 */
void StartWindow::on_backButton_clicked(void)
{
    QWidget * newWidget = new WelcomeWindow(mainwindow);
    mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);

}   /* on_backButton_clicked() */


//-----[ IMPLEMENTATION OF PRIVATE STARTWINDOW FUNCTIONS ]--------------------//


/*** end of file ***/
