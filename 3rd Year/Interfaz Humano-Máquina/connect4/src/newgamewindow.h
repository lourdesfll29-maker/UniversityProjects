/**
 * @file     newgamewindow.h
 *
 * @brief    Definition of NewGameWindow class functions.
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

#ifndef NEWGAMEWINDOW_H
#define NEWGAMEWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>
#include <QPushButton>

/// Own includes
#include "mainwindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class NewGameWindow; }
QT_END_NAMESPACE


//-----[ NEWGAMEWINDOW CLASS ]------------------------------------------------//

class NewGameWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit NewGameWindow(QWidget * parent = nullptr);
        ~NewGameWindow(void);

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
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

        void onRadioButtonToggled(void);
        void onGuestPressed(void);
        void onLogInPressed(void);
        void onRegisterPressed(void);

        void onCircle1Pressed(void);
        void onCircle2Pressed(void);

        void onBackPressed(void);
        void onNextPressed(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES & FUNCTIONS   |
         +-----------------------------------------------------*/

        /// Variables

        Ui::NewGameWindow * ui;
        MainWindow * mainwindow;

        QString blue_active;
        QString red_active;
        QString green_active;
        QString purple_active;
        QString blue_inactive;
        QString red_inactive;
        QString green_inactive;
        QString purple_inactive;

        QPushButton * disabledCircle1;
        QPushButton * disabledCircle2;

        bool gameModeSelected;
        bool player2Selected;
        bool circleColor1Selected;
        bool circleColor2Selected;

        /// Functions

        void initializeIconNames(void);
        void initializeIcons(void);
        void initializeButtons(void);

        void checkActivateNext(void);

        void onUserLogged(void);
};

#endif /* NEWGAMEWINDOW_H */

/*** end of file ***/
