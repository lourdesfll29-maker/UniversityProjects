/**
 * @file     profilewindow.cpp
 *
 * @brief    Implementation of ProfileWindow class functions.
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

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QMouseEvent>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

/// Own includes
#include "profilewindow.h"
#include "ui_profilewindow.h"
#include "individualrankingwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC PROFILEWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   ProfileWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of ProfileWindow.
 */
ProfileWindow::ProfileWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfileWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
    , lastDayIndex(-1)
{
    ui->setupUi(this);

    // Install event filters for the input lines
    ui->dayComboBox->installEventFilter(this);
    ui->monthComboBox->installEventFilter(this);
    ui->yearComboBox->installEventFilter(this);

    ui->emailLine->installEventFilter(this);
    ui->passwordLine->installEventFilter(this);

    ui->changeBirthDayButton->hide();
    ui->changeEmailButton->hide();
    ui->changePasswordButton->hide();

    ui->dateOfBirthWarningLabel->hide();
    ui->emailWarningLabel->hide();
    ui->passwordWarningLabel->hide();

    for (int i = 1; i<= 31; i++)
    {
        ui->dayComboBox->addItem(QString::fromStdString(std::to_string(i)));
    }

    for (int i = 2025; i >= 1900; i--)
    {
        ui->yearComboBox->addItem(QString::fromStdString(std::to_string(i)));
    }

    if (mainwindow->currentPlayer)
    {
        ui->avatarButton->setIcon(
            QPixmap::fromImage(
                mainwindow->currentPlayer->getAvatar()));

        ui->nickNameLine->setText(
            mainwindow->currentPlayer->getNickName());

        ui->dayComboBox->setCurrentIndex(
            (mainwindow->currentPlayer->getBirthdate().day()) - 1);

        ui->monthComboBox->setCurrentIndex(
            (mainwindow->currentPlayer->getBirthdate().month()) - 1);

        int yearIndex = ui->yearComboBox->findText(
            QString::number(mainwindow->currentPlayer->getBirthdate().year()));

        ui->yearComboBox->setCurrentIndex(yearIndex);

        ui->emailLine->setText(
            mainwindow->currentPlayer->getEmail());

        ui->passwordLine->setText(
            mainwindow->currentPlayer->getPassword());
    }

    ui->dayOfBirthFrame->setEnabled(false);
    ui->monthOfBirthFrame->setEnabled(false);
    ui->yearOfBirthFrame->setEnabled(false);

    ui->emailFrame->setEnabled(false);

    ui->passwordFrame->setEnabled(false);

    connect(
        ui->dayComboBox
        , &QComboBox::highlighted
        , this
        , &ProfileWindow::onActivated);

    connect(
        ui->monthComboBox
        , &QComboBox::highlighted
        , this
        , &ProfileWindow::onActivated);

    connect(
        ui->monthComboBox
        , &QComboBox::currentIndexChanged
        , this
        , &ProfileWindow::onCurrentIndexChanged);

    connect(
        ui->yearComboBox
        , &QComboBox::highlighted
        , this
        , &ProfileWindow::onActivated);

    connect(
        ui->yearComboBox
        , &QComboBox::currentIndexChanged
        , this
        , &ProfileWindow::onCurrentIndexChanged);

}   /* ProfileWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the ProfileSideMenu class destructor.
 * @param   void
 */
ProfileWindow::~ProfileWindow(void)
{
    delete ui;

}   /* ~ProfileWindow() */


//-----[ IMPLEMENTATION OF PROTECTED PROFILEWINDOW FUNCTIONS ]----------------//

/******************************************************************************/
/*!
 * @brief   Handles events for the SingWindow class.
 *
 * This function filters events to manage focus state and
 * validate user input for the login process.
 *
 * @param   obj    Pointer to the object that triggered the event.
 * @param   event  Pointer to the event that occurred.
 * @return  Returns true if the event is handled, false otherwise.
 */
bool ProfileWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::FocusIn)
    {
        editFrameState(obj, IN_FOCUS); // Update frame state to focused
    }
    // Check if the event is a focus-out event
    else if (event->type() == QEvent::FocusOut)
    {
        editFrameState(obj, NOT_IN_FOCUS); // Update frame state to not focused
        if ((qobject_cast<QLineEdit *>(obj)) != nullptr)
        {
            onLineEditingFinished(obj); // Handle line editing completion
        }
    }
    // Ignore other events
    else
    {
        // Do nothing
    }

    // Combrobar si es correcto el input de los combobox
    if (obj->property("activated") == true)
    {
        if ((qobject_cast<QComboBox *>(obj))->currentIndex() == -1)
        {
            editFrameState(obj, WRONG_INPUT);
        }
        else
        {
            editFrameState(obj, IN_FOCUS);
            obj->setProperty("activated", false);
        }
    }

    // Validate user input for login button activation
    if (isValidEmail(ui->emailLine->text()) == true)
    {
        ui->changeEmailButton->setEnabled(true);
    }
    else
    {
        ui->changeEmailButton->setEnabled(false);
    }

    // Validate user input for login button activation
    if (isValidPassword(ui->passwordLine->text()) == IS_VALID)
    {
        ui->changePasswordButton->setEnabled(true);
    }
    else
    {
        ui->changePasswordButton->setEnabled(false);
    }

    if (isOlderThan16Years(
            (ui->dayComboBox->itemText(
                 ui->dayComboBox->currentIndex())).toInt(),
            (ui->monthComboBox->currentIndex() + 1),
            (ui->yearComboBox->itemText(
                 ui->yearComboBox->currentIndex())).toInt()))
    {
        ui->dayOfBirthFrame->setProperty("validInputText", "true");
        ui->dayOfBirthFrame->style()->unpolish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->style()->polish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->update();

        ui->monthOfBirthFrame->setProperty("validInputText", "true");
        ui->monthOfBirthFrame->style()->unpolish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->style()->polish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->update();

        ui->yearOfBirthFrame->setProperty("validInputText", "true");
        ui->yearOfBirthFrame->style()->unpolish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->style()->polish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->update();

        ui->dateOfBirthWarningLabel->hide();
        ui->changeBirthDayButton->setEnabled(true);
    }
    else
    {
        ui->dayOfBirthFrame->setProperty("lineInFocus", "false");
        ui->dayOfBirthFrame->setProperty("validInputText", "false");
        ui->dayOfBirthFrame->style()->unpolish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->style()->polish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->update();

        ui->monthOfBirthFrame->setProperty("lineInFocus", "false");
        ui->monthOfBirthFrame->setProperty("validInputText", "false");
        ui->monthOfBirthFrame->style()->unpolish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->style()->polish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->update();

        ui->yearOfBirthFrame->setProperty("lineInFocus", "false");
        ui->yearOfBirthFrame->setProperty("validInputText", "false");
        ui->yearOfBirthFrame->style()->unpolish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->style()->polish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->update();

        ui->dateOfBirthWarningLabel->show();
        ui->changeBirthDayButton->setEnabled(false);
    }

    // Call the base event handler
    return (QWidget::eventFilter(obj, event));

}   /* eventFilter() */

/******************************************************************************/
/*!
 * @brief   Handles the mouse press event for the log window.
 * @param   event  The resize event information.
 * @return  void
 */
void ProfileWindow::mousePressEvent(QMouseEvent * event)
{
    // Check if the click is outside the nickname and password frames
    if (!(ui->dayOfBirthFrame->geometry().contains(event->pos()))   ||
        !(ui->monthOfBirthFrame->geometry().contains(event->pos())) ||
        !(ui->yearOfBirthFrame->geometry().contains(event->pos())))
    {
        QWidget * widget = qobject_cast<QWidget *>(QApplication::focusObject());

        if (widget)
        {
            widget->clearFocus(); // Clear focus from the widget
        }

        editFrameState(QApplication::focusObject(), NOT_IN_FOCUS);
    }

}   /* mousePressEvent() */


//-----[ IMPLEMENTATION OF PRIVATE SLOT PROFILEWINDOW FUNCTIONS ]-------------//

/******************************************************************************/
/*!
 * @brief  Handles the back button click event on the profile window.
 * @param  void
 * @return void
 */
void ProfileWindow::on_a_backButton_clicked(void)
{
    bool recoverPrevWidget = true;

    if (ui->editBirthDayButton->isChecked() ||
        ui->editEmailButton->isChecked()    ||
        ui->editPasswordButton->isChecked())
    {
        // Create the QMessageBox
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Confirmation needed"));

        // Create the message with HTML to format it
        QString message = tr("Do you really want to go back to the previous"
                             "window? <br>Any unsaved changes will be lost.");

        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Warning);

        // Add Yes and No buttons
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        // Adjust the size of the QMessageBox
        msgBox.setFixedSize(300, 200);

        // Show the QMessageBox and get the user’s response
        int ret = msgBox.exec();

        if (ret == QMessageBox::No)
        {
            // If the user cancels, do not switch to the previous window
            recoverPrevWidget = false;
        }
    }

    if (recoverPrevWidget)
    {
        mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WINDOW);
    }

}   /* on_a_backButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Opens a file dialog for selecting an image, scales it to 150x150,
 *          crops it into a circle, and sets it as the player's avatar.
 * @param   void
 * @return  void
 */
void ProfileWindow::on_avatarButton_clicked(void)
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Please choose an avatar image"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters({"Images (*.png *.jpg)"});

    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileUrl = dialog.selectedFiles().first();

        QPixmap pixmap(fileUrl);
        QPixmap croppedPixmap;

        croppedPixmap = pixmap.scaled(
            150
            , 150
            , Qt::KeepAspectRatioByExpanding
            , Qt::SmoothTransformation).copy(0, 0, 150, 150);

        QPixmap circularPixmap(150, 150);
        circularPixmap.fill(Qt::transparent);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(croppedPixmap));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 150, 150);
        painter.end();

        mainwindow->currentPlayer->setAvatar(circularPixmap.toImage());

        ui->avatarButton->setIcon(
            QPixmap::fromImage(
                mainwindow->currentPlayer->getAvatar()));
    }
    else
    {
        ; // Do nothing
    }

}   /* on_avatarButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles the event when the ranking button is clicked.
 *          It creates a new IndividualRankingWindow and sets it as the central
 *          widget.
 * @param   void
 * @return  void
 */
void ProfileWindow::on_rankingButton_clicked(void)
{
    QWidget * newWidget = new IndividualRankingWindow(
        mainwindow, mainwindow->player1->getNickName());

    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* on_rankingButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Toggles the editability of the birthday fields based on the button
 *          state and updates the UI accordingly.
 * @param   checked  Indicates whether the button is checked or not.
 * @return  void
 */
void ProfileWindow::on_editBirthDayButton_clicked(bool checked)
{
    if (checked)
    {
        ui->dayOfBirthFrame->setEnabled(true);
        ui->monthOfBirthFrame->setEnabled(true);
        ui->yearOfBirthFrame->setEnabled(true);
        ui->changeBirthDayButton->show();
    }
    else // Cancelar
    {

        int yearIndex = ui->yearComboBox->findText(
            QString::number(mainwindow->currentPlayer->getBirthdate().year()));

        ui->yearComboBox->setCurrentIndex(yearIndex);

        ui->monthComboBox->setCurrentIndex(
            (mainwindow->currentPlayer->getBirthdate().month()) - 1);

        ui->dayComboBox->setCurrentIndex(
            (mainwindow->currentPlayer->getBirthdate().day()) - 1);

        ui->dayOfBirthFrame->setProperty("lineInFocus", "false");
        ui->dayOfBirthFrame->setProperty("validInputText", "true");
        ui->dayOfBirthFrame->style()->unpolish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->style()->polish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->update();

        ui->monthOfBirthFrame->setProperty("lineInFocus", "false");
        ui->monthOfBirthFrame->setProperty("validInputText", "true");
        ui->monthOfBirthFrame->style()->unpolish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->style()->polish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->update();

        ui->yearOfBirthFrame->setProperty("lineInFocus", "false");
        ui->yearOfBirthFrame->setProperty("validInputText", "true");
        ui->yearOfBirthFrame->style()->unpolish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->style()->polish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->update();

        ui->dayOfBirthFrame->setEnabled(false);
        ui->monthOfBirthFrame->setEnabled(false);
        ui->yearOfBirthFrame->setEnabled(false);
        ui->changeBirthDayButton->hide();
    }

}   /* on_editBirthDayButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles changes to the player's birthday, saving the updated data.
 * @param   void
 * @return  void
 */
void ProfileWindow::on_changeBirthDayButton_clicked(void)
{
    ui->editBirthDayButton->setChecked(false);

    ui->dayOfBirthFrame->setEnabled(false);
    ui->monthOfBirthFrame->setEnabled(false);
    ui->yearOfBirthFrame->setEnabled(false);
    ui->changeBirthDayButton->hide();

    if (mainwindow)
    {
        QDate birthday;

        birthday.setDate(
            (ui->yearComboBox->itemText(
                 ui->yearComboBox->currentIndex())).toInt(),
            ui->monthComboBox->currentIndex() + 1,
            ui->dayComboBox->currentIndex() + 1);

        mainwindow->currentPlayer->setBirthdate(birthday);
    }

}   /* on_changeBirthDayButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Toggles the email field for editing based on the button state.
 * @param   checked  Indicates whether the button is checked or not.
 * @return  void
 */
void ProfileWindow::on_editEmailButton_clicked(bool checked)
{
    if (checked)
    {
        ui->emailFrame->setEnabled(true);
        ui->changeEmailButton->show();
    }
    else // Cancelar
    {
        if (mainwindow->currentPlayer)
        {
            ui->emailLine->setText(
                mainwindow->currentPlayer->getEmail());
        }

        ui->emailFrame->setEnabled(false);
        ui->changeEmailButton->hide();
    }

}   /* on_editEmailButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Saves the updated email address for the player.
 * @param   void
 * @return  void
 */
void ProfileWindow::on_changeEmailButton_clicked(void)
{
    ui->editEmailButton->setChecked(false);

    ui->emailFrame->setEnabled(false);
    ui->changeEmailButton->hide();

    if (mainwindow)
    {
        mainwindow->currentPlayer->setEmail(
            ui->emailLine->text());
    }

}   /* on_changeEmailButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Toggles the password field for editing based on the button state.
 * @param   checked  Indicates whether the button is checked or not.
 * @return  void
 */
void ProfileWindow::on_editPasswordButton_clicked(bool checked)
{
    if (checked)
    {
        ui->passwordLine->clear();

        ui->passwordFrame->setEnabled(true);
        ui->changePasswordButton->show();
    }
    else // Cancelar
    {
        if (mainwindow->currentPlayer)
        {
            ui->passwordLine->setText(
                mainwindow->currentPlayer->getPassword());
        }

        ui->showPasswordButton->setChecked(false);
        ui->passwordLine->setEchoMode(QLineEdit::Password);
        ui->passwordFrame->setEnabled(false);
        ui->changePasswordButton->hide();
    }

}   /* on_editPasswordButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Saves the updated password for the player.
 * @param   void
 * @return  void
 */
void ProfileWindow::on_changePasswordButton_clicked(void)
{
    ui->editPasswordButton->setChecked(false);

    ui->showPasswordButton->setChecked(false);
    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->passwordFrame->setEnabled(false);
    ui->changePasswordButton->hide();

    if (mainwindow)
    {
        mainwindow->currentPlayer->setPassword(
            ui->passwordLine->text());
    }

}   /* on_changePasswordButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Toggles the visibility of the password in the password field.
 * @param   checked  Indicates whether the show password checkbox is checked.
 * @return  void
 */
void ProfileWindow::on_showPasswordButton_clicked(bool checked)
{
    if (checked)
    {
        // Show password
        ui->passwordLine->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        // Hide password
        ui->passwordLine->setEchoMode(QLineEdit::Password);
    }

}   /* on_showPasswordButton_clicked() */

/******************************************************************************/
/*!
 * @brief   Handles changes in the combobox selections for month and year.
 *          Updates the day combobox accordingly based on the selected
 *          month/year.
 * @param   index  The index of the combobox item that triggered the event
 *                 (unused).
 * @return  void
 */
void ProfileWindow::onCurrentIndexChanged(int index)
{
    Q_UNUSED(index);

    // Retrieve the QComboBox that triggered this event
    QComboBox * comboBox = qobject_cast<QComboBox *>(sender());

    int daysLimit;

    if (comboBox == ui->monthComboBox)
    {
        lastDayIndex = ui->dayComboBox->currentIndex();

        if (ui->monthComboBox->currentIndex() ==  3 ||
            ui->monthComboBox->currentIndex() ==  5 ||
            ui->monthComboBox->currentIndex() ==  8 ||
            ui->monthComboBox->currentIndex() == 10)
        {
            daysLimit = 30;
        }
        else if (ui->monthComboBox->currentIndex() ==  1)
        {
            if ((ui->yearComboBox->currentIndex() !=  -1) &&
                (!isLeap(
                    (ui->yearComboBox->itemText(
                         ui->yearComboBox->currentIndex())).toInt())))
            {
                daysLimit = 28;
            }
            else
            {
                daysLimit = 29;
            }
        }
        else
        {
            daysLimit = 31;
        }

        ui->dayComboBox->clear();

        for (int i = 1; i <= daysLimit; i++)
        {
            ui->dayComboBox->addItem(
                QString::fromStdString(std::to_string(i)));
        }

        if (lastDayIndex > daysLimit - 1)
        {
            lastDayIndex = daysLimit - 1;
        }
        else
        {
            // Do not modify lastDayIndex
        }

        ui->dayComboBox->setProperty("autoUpdating", true);
        ui->dayComboBox->setCurrentIndex(lastDayIndex);
        ui->dayComboBox->setProperty("autoUpdating", false);
    }
    else if (comboBox == ui->yearComboBox)
    {
        lastDayIndex = ui->dayComboBox->currentIndex();

        if (ui->monthComboBox->currentIndex() ==  1)
        {
            if ((ui->yearComboBox->currentIndex() !=  -1) &&
                (!isLeap(
                    (ui->yearComboBox->itemText(
                         ui->yearComboBox->currentIndex())).toInt())))
            {
                daysLimit = 28;
            }
            else
            {
                daysLimit = 29;
            }

            ui->dayComboBox->clear();

            for (int i = 1; i <= daysLimit; i++)
            {
                ui->dayComboBox->addItem(
                    QString::fromStdString(std::to_string(i)));
            }

            if (lastDayIndex > daysLimit - 1)
            {
                lastDayIndex = daysLimit - 1;
            }
            else
            {
                // Do not modify lastDayIndex
            }

            ui->dayComboBox->setProperty("autoUpdating", true);
            ui->dayComboBox->setCurrentIndex(lastDayIndex);
            ui->dayComboBox->setProperty("autoUpdating", false);
        }
    }
    else
    {
        ; // Do nothing
    }

}   /* onCurrentIndexChanged() */

/******************************************************************************/
/*!
 * @brief   Handles the activation of a combobox item.
 *          Sets the "activated" property of the combobox unless auto-updating
 *          is active for the day combobox.
 * @param   index  The index of the combobox item that was activated (unused).
 * @return  void
 */
void ProfileWindow::onActivated(int index)
{
    Q_UNUSED(index);

    // Retrieve the QComboBox that triggered this event
    QComboBox * comboBox = qobject_cast<QComboBox *>(sender());

    if (ui->dayComboBox->property("autoUpdating") == false)
    {
        comboBox->setProperty("activated", true);
    }

}   /* onActivated() */


//-----[ IMPLEMENTATION OF PRIVATE PROFILEWINDOW FUNCTIONS ]------------------//

/******************************************************************************/
/*!
 * @brief   Updates the visual state of the input frames based on focus and
 *          input validity.
 * @param   obj    Pointer to the (nickname or password) edited object.
 * @param   state  The state of the frame (IN_FOCUS, NOT_IN_FOCUS,
 *                 CORRECT_INPUT, WRONG_INPUT).
 * @return  void
 */
void ProfileWindow::editFrameState(QObject * obj, frameState_t state)
{
    QFrame * frameInFocus;
    QString lineInFocus    = "";
    QString validInputText = "";

    switch (state)
    {
    case IN_FOCUS:
        lineInFocus    = "true";
        validInputText = "true";
        break;

    case NOT_IN_FOCUS:
        lineInFocus    = "false";
        break;

    case CORRECT_INPUT:
        validInputText = "true";
        break;

    case WRONG_INPUT:
        lineInFocus    = "false";
        validInputText = "false";
        break;

    default:
        break;
    }

    if (state == WRONG_INPUT)
    {
        if (obj == ui->dayComboBox)
        {
            //ui->dayWarningLabel->show();
        }
        else if (obj == ui->monthComboBox)
        {
            //ui->monthWarningLabel->show();
        }
        else if (obj == ui->yearComboBox)
        {
            //ui->yearWarningLabel->show();
        }
        else if (obj == ui->emailLine)
        {
            ui->emailWarningLabel->show();
        }
        else if (obj == ui->nickNameLine)
        {
            //ui->nickNameWarningLabel->show();
        }
        else if (obj == ui->passwordLine)
        {
            ui->passwordWarningLabel->show();
        }
        else
        {
            ; // Do nothing
        }
    }
    else
    {
        if (obj == ui->dayComboBox)
        {
            //ui->dayWarningLabel->hide();
        }
        else if (obj == ui->monthComboBox)
        {
            //ui->monthWarningLabel->hide();
        }
        else if (obj == ui->yearComboBox)
        {
            //ui->yearWarningLabel->hide();
        }
        else if (obj == ui->emailLine)
        {
            ui->emailWarningLabel->hide();
        }
        else if (obj == ui->nickNameLine)
        {
            //ui->nickNameWarningLabel->hide();
        }
        else if (obj == ui->passwordLine)
        {
            ui->passwordWarningLabel->hide();
        }
        else
        {
            ; // Do nothing
        }
    }

    if (obj == ui->dayComboBox)
    {
        frameInFocus = ui->dayOfBirthFrame;
    }
    else if (obj == ui->monthComboBox)
    {
        frameInFocus = ui->monthOfBirthFrame;
    }
    else if (obj == ui->yearComboBox)
    {
        frameInFocus = ui->yearOfBirthFrame;
    }
    else if (obj == ui->emailLine)
    {
        frameInFocus = ui->emailFrame;
    }
    else if (obj == ui->nickNameLine)
    {
        frameInFocus = ui->nickNameFrame;
    }
    else if (obj == ui->passwordLine)
    {
        frameInFocus = ui->passwordFrame;
    }
    else
    {
        frameInFocus = nullptr;
    }

    if (frameInFocus)
    {
        if (lineInFocus != "")
        {
            frameInFocus->setProperty("lineInFocus", lineInFocus);
            frameInFocus->style()->unpolish(frameInFocus);
            frameInFocus->style()->polish(frameInFocus);
            frameInFocus->update();
        }

        if (validInputText != "")
        {
            frameInFocus->setProperty("validInputText", validInputText);
            frameInFocus->style()->unpolish(frameInFocus);
            frameInFocus->style()->polish(frameInFocus);
            frameInFocus->update();
        }
    }

}   /* editFrameState() */

/******************************************************************************/
/*!
 * @brief   Validates the input in the line edit when editing is finished.
 * @param   obj  Pointer to the object that triggered this function (QLineEdit).
 * @return  void
 */
void ProfileWindow::onLineEditingFinished(QObject * obj)
{
    // Cast the object to QLineEdit
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(obj);
    QString text = lineEdit->text(); // Get the text from the line edit

    // Validate the input for the nickname line edit
    if (lineEdit == ui->emailLine)
    {
        frameState_t frameState = WRONG_INPUT;

        // Handle empty input for email line
        if (text == "")
        {
            if (lineEdit == ui->emailLine)
            {
                ui->emailWarningLabel->setText(
                    tr("* Mandatory o"));
            }
        }
        // Handle invalid characters in email
        else if (!(isValidEmail(text)))
        {
            if (lineEdit == ui->emailLine)
            {
                ui->emailWarningLabel->setText(
                    tr("* Email is not valid o"));
            }
        }
        else
        {
            frameState = CORRECT_INPUT;
        }

        // Update frame state for wrong input
        editFrameState(obj, frameState);
    }
    // Validate the input for the password line edit
    else if (lineEdit == ui->passwordLine)
    {
        if (lineEdit == ui->passwordLine)
        {
            lineCheck_t passwordCheck = isValidPassword(text);

            if (passwordCheck == IS_VALID)
            {
                // Update frame state
                editFrameState(obj, CORRECT_INPUT);
            }
            else
            {
                switch (passwordCheck)
                {
                    case EMPTY:
                        ui->passwordWarningLabel->setText(
                            tr("* Mandatory a"));
                        break;

                    case NOT_VALID:
                        ui->passwordWarningLabel->setText(
                            tr("* Password is not valid"));
                        break;

                    case TOO_SHORT:
                        ui->passwordWarningLabel->setText(
                            tr("* Too short a"));
                        break;

                    case TOO_LONG:
                        ui->passwordWarningLabel->setText(
                            tr("* Too long a"));
                        break;

                    default:
                        break;
                }

                // Update frame state for wrong input
                editFrameState(obj, WRONG_INPUT);
            }
        }
    }
    else
    {
        ; // Do nothing
    }

}   /* onLineEditingFinished() */


/*** end of file ***/

