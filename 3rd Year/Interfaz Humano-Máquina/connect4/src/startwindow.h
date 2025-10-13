/**
 * @file     startwindow.h
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

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef STARTWINDOW_H
#define STARTWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE


//-----[ STARTWINDOW CLASS ]--------------------------------------------------//

class StartWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit StartWindow(QWidget * parent = nullptr);
        ~StartWindow(void);

    signals:

        /*-----------------------------------------------------+
         |     DECLARATION OF SIGNAL FUNCTIONS                 |
         +-----------------------------------------------------*/

    protected:

        /*-----------------------------------------------------+
         |     DECLARATION OF PROTECTED FUNCTIONS              |
         +-----------------------------------------------------*/

    private slots:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE SLOT FUNCTIONS          |
         +-----------------------------------------------------*/

        void on_logButton_clicked(void);
        void on_registerButton_clicked(void);
        void on_guestButton_clicked(void);
        void on_backButton_clicked(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES               |
         +-----------------------------------------------------*/

        Ui::StartWindow * ui;
        MainWindow * mainwindow;
};

#endif /* STARTWINDOW_H */

/*** end of file ***/
