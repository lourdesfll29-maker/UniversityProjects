/**
 * @file     optionswindow.h
 *
 * @brief    Definition of OptionsWindow class functions.
 *
 * @version  0.2  (2024/01/25) Final version
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

#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>

/// Own includes
#include "mainwindow.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class OptionsWindow; }
QT_END_NAMESPACE


//-----[ OPTIONSWINDOW CLASS ]------------------------------------------------//

class OptionsWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        explicit OptionsWindow(QWidget * parent = nullptr);
        ~OptionsWindow(void);

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

        void on_a_backButton_clicked(void);
        void on_languageComboBox_currentIndexChanged(int index);

        void on_blueButton_clicked(void);
        void on_greenButton_clicked(void);
        void on_pinkButton_clicked(void);
        void on_orangeButton_clicked(void);
        void on_lightButton_clicked(void);
        void on_darkButton_clicked(void);

    private:

        /*----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS   |
         +----------------------------------------------------*/

        /// Variables

        Ui::OptionsWindow * ui;
        MainWindow * mainwindow;

        /// Functions

        void setVolume(void);
        void setMusic(void);
};

#endif // OPTIONSWINDOW_H

/*** end of file ***/
