/**
 * @file     newgamepart2window.h
 *
 * @brief    Definition of NewGamePart2Window class functions.
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

#ifndef NEWGAMEPART2WINDOW_H
#define NEWGAMEPART2WINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class NewGamePart2Window; }
QT_END_NAMESPACE


//-----[ NEWGAMEPART2WINDOW CLASS ]-------------------------------------------//

class NewGamePart2Window : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit NewGamePart2Window(QWidget *parent = nullptr);
        ~NewGamePart2Window(void);

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

        void onBackPressed(void);
        void onStartGamePressed(void);
        void onSizeRadioButtonToggled(void);
        void onRoundsRadioButtonToggled(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES & FUNCTIONS   |
         +-----------------------------------------------------*/

        /// Variables

        Ui::NewGamePart2Window * ui;
        MainWindow * mainwindow;

        /// Functions

        void setBoardPreview(void);
};

#endif /* NEWGAMEPART2WINDOW_H */

/*** end of file ***/
