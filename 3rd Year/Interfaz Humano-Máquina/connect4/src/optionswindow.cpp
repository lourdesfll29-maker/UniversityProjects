/**
 * @file     optionswindow.cpp
 *
 * @brief    Implementation of OptionsWindow class functions.
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

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes

/// Own includes
#include "optionswindow.h"
#include "qaudiooutput.h"
#include "ui_optionswindow.h"
#include "mainwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC OPTIONSWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   OptionsWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of OptionsWindow.
 */
OptionsWindow::OptionsWindow(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::OptionsWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    ui->setupUi(this);

    if (mainwindow->currentLanguage == SPANISH)
    {
        ui->languageComboBox->setCurrentIndex(0);
    }
    else if (mainwindow->currentLanguage == ENGLISH)
    {
        ui->languageComboBox->setCurrentIndex(1);
    }

    ui->volumeSlider->setRange(0, 100); // 0 to 100%
    ui->volumeSlider->setValue(5); // Match initial volume (5%)

    // Connect the slider to set the volume dynamically
    connect(
        ui->volumeSlider
        , &QSlider::valueChanged
        , this
        , &OptionsWindow::setVolume);

    // Connect the combobox to set the desired song
    connect(
        ui->musicComboBox
        , &QComboBox::currentIndexChanged
        , this
        , &OptionsWindow::setMusic);

    ui->blueButton->setChecked(false);
    ui->greenButton->setChecked(false);
    ui->pinkButton->setChecked(false);
    ui->orangeButton->setChecked(false);

    switch (mainwindow->currentColorTheme)
    {
        case BLUE_COLOR:
            ui->blueButton->setChecked(true);
            break;

        case GREEN_COLOR:
            ui->greenButton->setChecked(true);
            break;

        case PINK_COLOR:
            ui->pinkButton->setChecked(true);
            break;

        case ORANGE_COLOR:
            ui->orangeButton->setChecked(true);
            break;

        default:
            break;
    }

    ui->lightButton->setChecked(false);
    ui->darkButton->setChecked(false);

    switch (mainwindow->currentTheme)
    {
        case LIGHT_THEME:
            ui->lightButton->setChecked(true);
            break;

        case DARK_THEME:
            ui->darkButton->setChecked(true);
            break;

        default:
            break;
    }

}   /* OptionsWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the OptionsWindow class destructor.
 * @param   void
 */
OptionsWindow::~OptionsWindow(void)
{
    delete ui;

}   /* ~OptionsWindow() */

//-----[ IMPLEMENTATION OF PROTECTED OPTIONSWINDOW FUNCTIONS ]----------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT OPTIONSWINDOW FUNCTIONS ]-------------//

/******************************************************************************/
/*!
 * @brief   Navigates back to the previous widget from the Options Window.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_a_backButton_clicked(void)
{
    mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WIDGET);

}   /* on_a_backButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the language combo box selection changes.
 * @param   index  The index of the selected language.
 * @return  void
 */
void OptionsWindow::on_languageComboBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        mainwindow->currentLanguage = SPANISH;
    }
    else if (index == 1)
    {
        mainwindow->currentLanguage = ENGLISH;
    }
    else
    {
        ; // Do nothing
    }

    mainwindow->setUILanguage(mainwindow->currentLanguage);
    ui->retranslateUi(this);

}   /* on_languageComboBox_currentIndexChanged() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the blue button is clicked.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_blueButton_clicked(void)
{
    ui->greenButton->setChecked(false);
    ui->pinkButton->setChecked(false);
    ui->orangeButton->setChecked(false);

    mainwindow->currentColorTheme = BLUE_COLOR;

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

}   /* on_blueButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the green button is clicked.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_greenButton_clicked(void)
{
    ui->blueButton->setChecked(false);
    ui->pinkButton->setChecked(false);
    ui->orangeButton->setChecked(false);

    mainwindow->currentColorTheme = GREEN_COLOR;

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

}   /* on_greenButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the pink button is clicked.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_pinkButton_clicked()
{
    ui->blueButton->setChecked(false);
    ui->greenButton->setChecked(false);
    ui->orangeButton->setChecked(false);

    mainwindow->currentColorTheme = PINK_COLOR;

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

}   /* on_greenButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the orange button is clicked.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_orangeButton_clicked(void)
{
    ui->blueButton->setChecked(false);
    ui->greenButton->setChecked(false);
    ui->pinkButton->setChecked(false);

    mainwindow->currentColorTheme = ORANGE_COLOR;

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

}   /* on_orangeButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the light theme button is clicked.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_lightButton_clicked(void)
{
    ui->darkButton->setChecked(false);

    mainwindow->currentTheme = LIGHT_THEME;

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

}   /* on_lightButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the dark theme button is clicked.
 * @param   void
 * @return  void
 */
void OptionsWindow::on_darkButton_clicked(void)
{
    ui->lightButton->setChecked(false);

    mainwindow->currentTheme = DARK_THEME;

    mainwindow->setUITheme(
        mainwindow->currentTheme,
        mainwindow->currentColorTheme);

    emit mainwindow->themeChanged();

}   /* on_darkButton_clicked() */


//-----[ IMPLEMENTATION OF PRIVATE OPTIONSWINDOW FUNCTIONS ]------------------//

/******************************************************************************/
/*!
 * @brief   Sets the volume of the media player based on the slider value.
 * @param   void
 * @return  void
 */
void OptionsWindow::setVolume(void)
{
    // Get the current slider value (0 to 100)
    int sliderValue = ui->volumeSlider->value();

    // Convert the slider value to a volume range (0.0 to 1.0)
    float volume = sliderValue / 100.0;

    // Set the media player's volume
    mainwindow->audioOutput->setVolume(volume);

    // Optionally, you can display the volume value in a label
    qDebug() << "Volume set to:" << volume;

}   /* setVolume() */

/******************************************************************************/
/*!
 * @brief   Sets the volume of the media player based on the slider value.
 * @param   void
 * @return  void
 */
void OptionsWindow::setMusic(void)
{
    QString selectedSong = ui->musicComboBox->currentText();

    if (selectedSong == tr("Full Playlist"))
    {
        mainwindow->currentSongIndex = 0;
        mainwindow->currentSong = mainwindow->playlist[mainwindow->currentSongIndex];
        mainwindow->currentSongIndex++;
        mainwindow->fullPlaylist = true;
    }
    else
    {
        if (selectedSong == tr("Abyss Theme"))
        {
            selectedSong = mainwindow->Abyss_Theme;
        }
        else if (selectedSong == tr("Mario Theme"))
        {
            selectedSong = mainwindow->Mario_Theme;
        }
        else if (selectedSong == tr("Mario & Luigi Theme"))
        {
            selectedSong = mainwindow->MarioLuigi_Theme;
        }
        else if (selectedSong == tr("Pokemon Theme"))
        {
            selectedSong = mainwindow->Pokemon_Theme;
        }
        else if (selectedSong == tr("Smash Theme"))
        {
            selectedSong = mainwindow->Smash_Theme;
        }
        else if (selectedSong == tr("SpongeBob Theme"))
        {
            selectedSong = mainwindow->SpongeBob_Theme;
        }
        else if (selectedSong == tr("Wii Theme"))
        {
            selectedSong = mainwindow->Wii_Theme;
        }
        else if (selectedSong == tr("Yoshi Theme"))
        {
            selectedSong = mainwindow->Yoshi_Theme;
        }
        else if (selectedSong == tr("Zelda Theme"))
        {
            selectedSong = mainwindow->Zelda_Theme;
        }

        mainwindow->currentSong = selectedSong;
        mainwindow->fullPlaylist = false;
    }

    qDebug() << "Playing:" << mainwindow->currentSong;
    mainwindow->mediaplayer->setSource(QUrl(mainwindow->currentSong));
    mainwindow->mediaplayer->play();

}   /* setMusic() */


/*** end of file ***/
