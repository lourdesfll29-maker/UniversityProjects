/**
 * @file     singwindow.h
 *
 * @brief    Definition of SingWindow class functions.
 *
 * @version  0.2  (2024/01/24) Final version
 * @version  0.1  (2024/01/23) BARR-C programming standard needs to be revised
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

#ifndef SINGWINDOW_H
#define SINGWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"
#include "validation.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class SingWindow; }
QT_END_NAMESPACE


//-----[ SINGWINDOW CLASS ]---------------------------------------------------//

class SingWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit SingWindow(QWidget * parent = nullptr,
                            playerToBeLogged_t playerToBeLogged = PLAYER1);

        ~SingWindow(void);

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
        void onActivated(int index);
        void onCurrentIndexChanged(int index);
        void onLineReturnPressed(void);
        void on_avatarButton_clicked(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS    |
         +-----------------------------------------------------*/

        /// Variables

        Ui::SingWindow * ui;
        MainWindow * mainwindow;

        Player * loggedPlayer;
        playerToBeLogged_t playerToBeLogged;
        QString email;
        int securityCode;

        int lastDayIndex;
        int lastMonthIndex;

        Connect4 & game;
        QDate selectedDate;
        QImage selectedAvatar;

        /// Functions

        void resetUI(void);
        void editFrameState(QObject * obj, frameState_t state);
        void onLineEditingFinished(QObject * obj);
};

#endif /* SINGWINDOWW_H */

/*** end of file ***/
