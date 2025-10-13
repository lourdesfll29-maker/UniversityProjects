/**
 * @file     logwindow.h
 *
 * @brief    Definition of LogWindow class functions.
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

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef LOGWINDOW_H
#define LOGWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"
#include "validation.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class LogWindow; }
QT_END_NAMESPACE


//-----[ LOGWINDOW CLASS ]----------------------------------------------------//

class LogWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit LogWindow(QWidget * parent = nullptr,
                           playerToBeLogged_t playerToBeLogged = PLAYER1);

        ~LogWindow(void);

    signals:

        /*-----------------------------------------------------+
         |     DECLARATION OF SIGNAL FUNCTIONS                 |
         +-----------------------------------------------------*/

    protected:

        /*-----------------------------------------------------+
         |     DECLARATION OF PROTECTED FUNCTIONS              |
         +-----------------------------------------------------*/

        bool eventFilter(QObject * obj, QEvent * event) override;
        void mousePressEvent(QMouseEvent * event) override;

    private slots:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

        void onButtonClicked(void);
        void onLogInErrorLinkActivated(const QString & link);
        void onShowPasswordClicked(void);
        void onLogInClicked(void);
        void onLineReturnPressed(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS    |
         +-----------------------------------------------------*/
        
        /// Variables

        Ui::LogWindow * ui;
        MainWindow * mainwindow;

        Player * loggedPlayer;
        playerToBeLogged_t playerToBeLogged;
        QString email;
        int securityCode;

        /// Functions

        void resetUI(void);
        void editFrameState(QObject * obj, frameState_t state);
        void onLineEditingFinished(QObject * obj);
};

#endif /* LOGWINDOW_H */

/*** end of file ***/
