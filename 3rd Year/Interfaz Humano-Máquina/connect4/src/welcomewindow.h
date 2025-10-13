/**
 * @file     welcomewindow.h
 *
 * @brief    Definition of WelcomeWindow class functions.
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

#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class WelcomeWindow; }
QT_END_NAMESPACE


//-----[ WELCOMEWINDOW CLASS ]------------------------------------------------//

class WelcomeWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit WelcomeWindow(QWidget * parent = nullptr);
        ~WelcomeWindow(void);

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

        void on_startButton_clicked(void);
        void on_aboutButton_clicked(void);
        void on_quitButton_clicked(void);

        void on_historyButton_clicked(void);
        void on_rankingButton_clicked(void);
        void on_optionsButton_clicked(void);

        void onMusicButtonClicked(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES               |
         +-----------------------------------------------------*/

        Ui::WelcomeWindow * ui;
        MainWindow * mainwindow;
};

#endif /* WELCOMEWINDOW_H */

/*** end of file ***/
