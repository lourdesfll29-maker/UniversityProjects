/**
 * @file     mainmenu.h
 *
 * @brief    Definition of mainMenu class functions.
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

#ifndef MAINMENU_H
#define MAINMENU_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>
#include <QMouseEvent>

/// Own includes
#include "mainwindow.h"
#include "profilesidemenu.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class mainMenu; }
QT_END_NAMESPACE


//-----[ MAINMENU CLASS ]-----------------------------------------------------//

class mainMenu : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit mainMenu(QWidget *parent = nullptr);
        ~mainMenu(void);

    signals:

        /*-----------------------------------------------------+
         |     DECLARATION OF SIGNAL FUNCTIONS                 |
         +-----------------------------------------------------*/

    protected:

        /*-----------------------------------------------------+
         |     DECLARATION OF PROTECTED FUNCTIONS              |
         +-----------------------------------------------------*/

        void resizeEvent(QResizeEvent * event) override;
        void mousePressEvent(QMouseEvent * event) override;

    private slots:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

        void onProfileButtonClicked(void);

        void onNewGameClicked(void);
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

        Ui::mainMenu * ui;
        ProfileSideMenu * sideMenu;
        MainWindow * mainwindow;
};

#endif /* MAINMENU_H */

/*** end of file ***/
