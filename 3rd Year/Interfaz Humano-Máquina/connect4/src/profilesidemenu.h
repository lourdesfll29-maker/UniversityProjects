/**
 * @file     profilesidemenu.h
 *
 * @brief    Definition of ProfileSideMenu class functions.
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

#ifndef PROFILESIDEMENU_H
#define PROFILESIDEMENU_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class ProfileSideMenu; }
QT_END_NAMESPACE


//-----[ PROFILESIDEMENU CLASS ]----------------------------------------------//

class ProfileSideMenu : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        ProfileSideMenu(QWidget * parent = nullptr);
        ~ProfileSideMenu(void);

        bool isMenuVisible(void);
        void showMenu(void);
        void hideMenu(void);
        void resizeMenu(void);

    signals:

        /*-----------------------------------------------------+
         |     DECLARATION OF SIGNAL FUNCTIONS                 |
         +-----------------------------------------------------*/

        void logoutRequested(void);

    protected:

        /*-----------------------------------------------------+
         |     DECLARATION OF PROTECTED FUNCTIONS              |
         +-----------------------------------------------------*/

        void keyPressEvent(QKeyEvent * event) override;

    private slots:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

        void onConfigButtonClicked(void);
        void onSeeProfileButtonClicked(void);
        void onRankingButtonClicked(void);
        void onLogOutButtonClicked(void);
        void onChangeThemeButtonClicked(void);
        void onHelpButtonClicked(void);
        void on_singInButton_clicked(void);

        void onMusicButtonClicked(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES & FUNCTIONS   |
         +-----------------------------------------------------*/

        /// Variables

        Ui::ProfileSideMenu * ui;
        bool menuIsVisible;
        QWidget * parent;
        MainWindow * mainwindow;

        /// Functions

        void showAnimation(void);
        void hideAnimation(void);

        void updateUser(void);
};

#endif /* PROFILESIDEMENU_H */

/*** end of file ***/
