/**
 * @file     profilewindow.h
 *
 * @brief    Definition of ProfileWindow class functions.
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

#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"
#include "validation.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class ProfileWindow; }
QT_END_NAMESPACE


//-----[ PROFILEWINDOW CLASS ]------------------------------------------------//

class ProfileWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit ProfileWindow(QWidget * parent = nullptr);
        ~ProfileWindow(void);

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
         |     DECLARIATION OF PRIVATE SLOT FUNCTIONS          |
         +-----------------------------------------------------*/

        void on_a_backButton_clicked(void);

        void on_avatarButton_clicked(void);

        void on_rankingButton_clicked(void);

        void on_editBirthDayButton_clicked(bool checked);
        void on_changeBirthDayButton_clicked(void);

        void on_editEmailButton_clicked(bool checked);
        void on_changeEmailButton_clicked(void);

        void on_editPasswordButton_clicked(bool checked);
        void on_changePasswordButton_clicked(void);

        void on_showPasswordButton_clicked(bool checked);

        void onCurrentIndexChanged(int index);
        void onActivated(int index);

    private:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS    |
         +-----------------------------------------------------*/

        /// Variables

        Ui::ProfileWindow * ui;
        MainWindow * mainwindow;

        int lastDayIndex;

        /// Functions

        void editFrameState(QObject * obj, frameState_t state);
        void onLineEditingFinished(QObject * obj);
};

#endif /* PROFILEWINDOW_H */

/*** end of file ***/
