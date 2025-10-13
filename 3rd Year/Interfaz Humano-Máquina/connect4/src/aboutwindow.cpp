/**
 * @file     aboutwindow.cpp
 *
 * @brief    Implementation of AboutWindow class functions.
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
#include "aboutwindow.h"
#include "ui_aboutwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC ABOUTWINDOW FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief   AboutWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of AboutWindow.
 */
AboutWindow::AboutWindow(QWidget * parent)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::AboutWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    ui->setupUi(this);

}   /* AboutWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the AboutWindow class destructor.
 * @param   void
 */
AboutWindow::~AboutWindow(void)
{
    delete ui;

}   /* ~AboutWindow() */


//-----[ IMPLEMENTATION OF PROTECTED ABOUTWINDOW FUNCTIONS ]------------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT ABOUTWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief   Navigates back to the previous widget from the About Window.
 * @param   void
 * @return  void
 */
void AboutWindow::on_a_backButton_clicked(void)
{
    mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WIDGET);

}   /* on_a_backButton_clicked() */


//-----[ IMPLEMENTATION OF PRIVATE ABOUTWINDOW FUNCTIONS ]--------------------//


/*** end of file ***/
