/**
 * @file     singwindow.cpp
 *
 * @brief    Implementation of SingWindow class functions.
 *
 * @version  0.2  (2024/01/24) Final version
 * @version  0.1  (2024/01/23) BARR-C programming standard needs to be reviewed
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
#include <QDebug>
#include <QStyle>
#include <QStyleOption>
#include <QList>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <regex>
#include <random>
#include <QFileDialog>
#include <QPainter>

/// Own includes
#include "singwindow.h"
#include "ui_singwindow.h"
#include "connect4.h"
#include "player.h"
#include "validation.h"
#include "logwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC SINGWELCOME FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief   SingWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of SingWindow.
 */
SingWindow::SingWindow(QWidget * parent, playerToBeLogged_t playerToBeLogged)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::SingWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
    , loggedPlayer(nullptr)
    , playerToBeLogged(playerToBeLogged)
    , securityCode(0)
    , lastDayIndex(-1)
    , lastMonthIndex(100)
    , game(Connect4::getInstance())
    //, selectedDate()
    , selectedAvatar(":/media/images/avatars/default.png")
{
    // Set up the UI components for SingWindow
    ui->setupUi(this);

    // Install event filters for the input lines
    ui->dayComboBox->installEventFilter(this);
    ui->monthComboBox->installEventFilter(this);
    ui->yearComboBox->installEventFilter(this);

    ui->emailLine->installEventFilter(this);
    ui->nickNameLine->installEventFilter(this);
    ui->passwordLine->installEventFilter(this);
    ui->repeatPasswordLine->installEventFilter(this);

    for (int i = 1; i<= 31; i++)
    {
        ui->dayComboBox->addItem(QString::fromStdString(std::to_string(i)));
    }

    for (int i = 2025; i >= 1900; i--)
    {
        ui->yearComboBox->addItem(QString::fromStdString(std::to_string(i)));
    }

    /// CONNECTING SIGNALS AND SLOTS

    connect(
        ui->a_backButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->SW_logInButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->dayComboBox
        , &QComboBox::highlighted
        , this
        , &SingWindow::onActivated);

    connect(
        ui->monthComboBox
        , &QComboBox::highlighted
        , this
        , &SingWindow::onActivated);

    connect(
        ui->monthComboBox
        , &QComboBox::currentIndexChanged
        , this
        , &SingWindow::onCurrentIndexChanged);

    connect(
        ui->yearComboBox
        , &QComboBox::highlighted
        , this
        , &SingWindow::onActivated);

    connect(
        ui->yearComboBox
        , &QComboBox::currentIndexChanged
        , this
        , &SingWindow::onCurrentIndexChanged);

    connect(
        ui->SW_continueButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->SW_continueButton_2
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);



    connect(
        ui->b_backButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->c_backButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->emailErrorLabel
        , &QLabel::linkActivated
        , this
        , &SingWindow::onLogInErrorLinkActivated);

    connect(
        ui->emailLine
        , &QLineEdit::returnPressed
        , this
        , &SingWindow::onLineReturnPressed);

    connect(
        ui->nickNameLine
        , &QLineEdit::returnPressed
        , this
        , &SingWindow::onLineReturnPressed);

    connect(
        ui->passwordLine
        , &QLineEdit::returnPressed
        , this
        , &SingWindow::onLineReturnPressed);

    connect(
        ui->showPasswordButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onShowPasswordClicked);

    connect(
        ui->repeatPasswordLine
        , &QLineEdit::returnPressed
        , this
        , &SingWindow::onLineReturnPressed);

    connect(
        ui->showRepeatPasswordButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onShowPasswordClicked);

    connect(
        ui->SW_createAccountButton
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->SW_logInButton_2
        , &QPushButton::clicked
        , this
        , &SingWindow::onButtonClicked);

    connect(
        ui->accountCreatedLabel
        , &QLabel::linkActivated
        , this
        , &SingWindow::onLogInErrorLinkActivated);

    /// SET INITIAL CONFIGURATION OF THE INTERFACE

    resetUI();

}   /* SingWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the SingWindow class destructor.
 * @param   void
 */
SingWindow::~SingWindow()
{
    // Delete the UI components associated with SingWindow
    delete ui;

}   /* ~SingWindow() */


//-----[ IMPLEMENTATION OF PROTECTED SINGWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   Handles events for the SingWindow class.
 *
 * This function filters events to manage focus state and
 * validate user input for the login process.
 *
 * @param   obj Pointer to the object that triggered the event.
 * @param   event Pointer to the event that occurred.
 * @return  Returns true if the event is handled, false otherwise.
 */
bool SingWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::FocusIn)
    {
        // Update frame state to focused
        editFrameState(obj, IN_FOCUS);
    }
    // Check if the event is a focus-out event
    else if (event->type() == QEvent::FocusOut)
    {
        // Update frame state to not focused
        editFrameState(obj, NOT_IN_FOCUS);

        if ((qobject_cast<QLineEdit *>(obj)) != nullptr)
        {
            // Handle line editing completion
            onLineEditingFinished(obj);
        }
    }
    // Ignore other events
    else
    {
        ; // Do nothing
    }

    if (ui->stackedWidget->currentWidget() == ui->a_dateOfBirth)
    {
        // Check if the combobox input is correct
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

        // Check if SW_continueButton can be enabled
        if (ui->dayComboBox->currentIndex()   != -1 &&
            ui->monthComboBox->currentIndex() != -1 &&
            ui->yearComboBox->currentIndex()  != -1)
        {
            ui->SW_continueButton->setEnabled(true);
        }
        else
        {
            ui->SW_continueButton->setEnabled(false);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->b_createAccount)
    {
        // Validate user input for login button activation
        if (isValidEmail(ui->emailLine->text()) == true           &&
            isValidNickName(ui->nickNameLine->text()) == IS_VALID &&
            !game.existsNickName(ui->nickNameLine->text())        &&
            isValidPassword(ui->passwordLine->text()) == IS_VALID &&
            ui->passwordLine->text() == ui->repeatPasswordLine->text())
        {
            // Enable login button
            ui->SW_createAccountButton->setEnabled(true);
        }
        else
        {
            // Disable login button
            ui->SW_createAccountButton->setEnabled(false);
        }
    }
    else
    {
        ; // Do nothing
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
void SingWindow::mousePressEvent(QMouseEvent * event)
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


//-----[ IMPLEMENTATION OF PRIVATE SLOT SINGWINDOW FUNCTIONS ]----------------//

/******************************************************************************/
/*!
 * @brief   Handles the addition of an item by showing the side menu.
 * @param   void
 * @return  void
 */
void SingWindow::onButtonClicked(void)
{
    // Retrieve the QPushButton that triggered this event
    QPushButton * buttonClicked = qobject_cast<QPushButton *>(sender());

    if (buttonClicked == ui->a_backButton)
    {
        if (mainwindow->getPreviousWindow())
        {
            mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WINDOW);
        }
        else
        {
            mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WIDGET);
        }
    }
    else if (buttonClicked == ui->b_backButton)
    {
        ui->stackedWidget->setCurrentWidget(ui->b_chooseAvatar);
        resetUI();
    }
    else if (buttonClicked == ui->c_backButton)
    {
        ui->stackedWidget->setCurrentWidget(ui->a_dateOfBirth);
        resetUI();
    }
    else if (buttonClicked == ui->SW_continueButton)
    {
        if (isOlderThan16Years(
                (ui->dayComboBox->currentIndex() + 1),
                (ui->monthComboBox->currentIndex() + 1),
                (ui->yearComboBox->itemText(
                      ui->yearComboBox->currentIndex())).toInt()))
        {
            selectedDate.setDate(
                (ui->yearComboBox->itemText(
                     ui->yearComboBox->currentIndex())).toInt(),
                (ui->monthComboBox->currentIndex() + 1),
                (ui->dayComboBox->currentIndex() + 1));

            qDebug() << "day: " << (ui->dayComboBox->currentIndex() + 1);
            qDebug() << "month: " << (ui->monthComboBox->currentIndex() + 1);
            qDebug() << "year: " << (ui->yearComboBox->itemText(
                     ui->yearComboBox->currentIndex())).toInt();

            qDebug() << selectedDate;

            ui->stackedWidget->setCurrentWidget(ui->b_chooseAvatar);
            resetUI();
        }
        else
        {
            ui->ageErrorFrame->show();
        }
    }
    else if (buttonClicked == ui->SW_continueButton_2)
    {
        ui->stackedWidget->setCurrentWidget(ui->b_createAccount);
        resetUI();
    }
    else if (buttonClicked == ui->SW_createAccountButton)
    {
        bool error = false;

        QList<Player *> players = game.getRanking();

        if (players[0]->getEmail() == ui->emailLine->text())
        {
            error = true;
        }

        if (error == true)
        {
            ui->emailErrorFrame->show();

            ui->emailWarningLabel->setText(
                tr("* Sorry, this email is already in use"));

            ui->emailWarningLabel->show();

            ui->emailFrame->setProperty("lineInFocus", "false");
            ui->emailFrame->setProperty("validInputText", "false");
            ui->emailFrame->style()->unpolish(ui->emailFrame);
            ui->emailFrame->style()->polish(ui->emailFrame);
            ui->emailFrame->update();
        }
        else
        {
            if (true)
            {
                // Register a player with their details
                game.registerPlayer(
                    ui->nickNameLine->text()    /* Nickname                   */
                    , ui->emailLine->text()     /* Email                      */
                    , ui->passwordLine->text()  /* Password                   */
                    , selectedDate              /* Date of birth              */
                    , 100                       /* Score (initial value)      */
                    , selectedAvatar);          /* User avatar                */
            }

            qDebug() << "playerToBeLogged: " << playerToBeLogged;

            QWidget * newWidget = new LogWindow(mainwindow, playerToBeLogged);

            if (!(mainwindow->getPreviousWindow()))
            {
                QPushButton * button;
                button = newWidget->findChild<QPushButton *>("a_backButton");
                button->setEnabled(false);
            }

            mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
        }
    }
    else if (buttonClicked == ui->SW_logInButton ||
               buttonClicked == ui->SW_logInButton_2)
    {
        QWidget * newWidget = new LogWindow(mainwindow, playerToBeLogged);

        if (!(mainwindow->getPreviousWindow()))
        {
            QPushButton * button;
            button = newWidget->findChild<QPushButton *>("a_backButton");
            button->setEnabled(false);
        }

        mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
    }
    else
    {
        ; // Do nothing
    }

}   /* onAddClicked() */

/******************************************************************************/
/*!
 * @brief   Handles activation of links in the log-in error window.
 * @param   link  The link that was activated.
 * @return  void
 */
void SingWindow::onLogInErrorLinkActivated(const QString & link)
{
    if (link == "#go_to_logIn")
    {
        qDebug() << "playerToBeLogged: " << playerToBeLogged;

        QWidget * newWidget = new LogWindow(mainwindow, playerToBeLogged);

        if (!(mainwindow->getPreviousWindow()))
        {
            QPushButton * button;
            button = newWidget->findChild<QPushButton *>("a_backButton");
            button->setEnabled(false);
        }

        mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
    }

}   /* onLogInErrorLinkActivated() */

/******************************************************************************/
/*!
 * @brief   Toggles the visibility of the password in the password field.
 * @param   void
 * @return  void
 */
void SingWindow::onShowPasswordClicked(void)
{
    // Retrieve the QPushButton that triggered this event
    QPushButton * pushButton = qobject_cast<QPushButton *>(sender());

    if (pushButton == ui->showPasswordButton)
    {
        if (ui->showPasswordButton->isChecked())
        {
            // Show password
            ui->passwordLine->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            // Hide password
            ui->passwordLine->setEchoMode(QLineEdit::Password);
        }
    }
    else if (pushButton == ui->showRepeatPasswordButton)
    {
        if (ui->showRepeatPasswordButton->isChecked())
        {
            // Show password
            ui->repeatPasswordLine->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            // Hide password
            ui->repeatPasswordLine->setEchoMode(QLineEdit::Password);
        }
    }
    else
    {
        ; // Do nothing
    }

} /* onShowPasswordClicked() */

/******************************************************************************/
/*!
 * @brief   Handles the activation of combo box items.
 * @param   index  The index of the item that was activated.
 * @return  void
 */
void SingWindow::onActivated(int index)
{
    Q_UNUSED(index);

    // Retrieve the QComboBox that triggered this event
    QComboBox * comboBox = qobject_cast<QComboBox *>(sender());

    if (ui->dayComboBox->property("autoUpdating") == false)
    {
        comboBox->setProperty("activated", true);
    }

}   /* onActivated() */

/******************************************************************************/
/*!
 * @brief   Handles changes to combo box indices, updating the days based on
 *          selected month and year.
 * @param   index  The index of the item that was changed.
 * @return  void
 */
void SingWindow::onCurrentIndexChanged(int index)
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
        // Do nothing
    }

}   /* onCurrentIndexChanged() */

/******************************************************************************/
/*!
 * @brief   Handles return key press events on the form's line edits,
 *          moving focus or triggering actions.
 * @param   void
 * @return  void
 */
void SingWindow::onLineReturnPressed(void)
{
    // Retrieve the QLineEdit that triggered this event
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(sender());

    // Check if the sender is the email line edit
    if (lineEdit == ui->emailLine)
    {
        // Clear focus from the email line edit and set focus to nickname
        ui->emailLine->clearFocus();
        ui->nickNameLine->setFocus();
    }
    // Check if the sender is the nickname line edit
    else if (lineEdit == ui->nickNameLine)
    {
        // Clear focus from the nickname line edit and set focus to password
        ui->nickNameLine->clearFocus();
        ui->passwordLine->setFocus();
    }
    // Check if the sender is the password line edit
    else if (lineEdit == ui->passwordLine)
    {
        // Clear focus from the password line edit and set focus to repeat
        // password
        ui->passwordLine->clearFocus();
        ui->repeatPasswordLine->setFocus();
    }
    // Check if the sender is the repeat password line edit
    else if (lineEdit == ui->repeatPasswordLine)
    {
        // Clear focus from the repeat password line edit and trigger login
        ui->repeatPasswordLine->clearFocus();
        ui->SW_createAccountButton->click();
    }
    // If the sender is neither, do nothing
    else
    {
        ; // Do nothing
    }

}   /* onLineReturnPressed() */

/******************************************************************************/
/*!
 * @brief   Allows the user to choose and set an avatar image.
 * @param   void
 * @return  void
 */
void SingWindow::on_avatarButton_clicked(void)
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Please choose an avatar image"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters({"Images (*.png *.jpg)"});

    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileUrl = dialog.selectedFiles().first();

        QPixmap pixmap(fileUrl);
        QPixmap croppedPixmap = pixmap.scaled(
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

        selectedAvatar = circularPixmap.toImage();

        ui->avatarButton->setIcon(
            QPixmap::fromImage(selectedAvatar));
    }
    else
    {
        ; // Do nothing
    }

}   /* on_avatarButton_clicked() */


//-----[ IMPLEMENTATION OF PRIVATE SINGWINDOW FUNCTIONS ]--------------------//

/*****************************************************************************/
/*!
 * @brief   Resets the user interface elements based on the current widget.
 * @param   void
 * @return  void
 */
void SingWindow::resetUI(void)
{
    this->setFocus();

    if (ui->stackedWidget->currentWidget() == ui->a_dateOfBirth)
    {
        ui->dayOfBirthFrame->setProperty("lineInFocus", "false");
        ui->dayOfBirthFrame->setProperty("validInputText", "true");
        ui->dayOfBirthFrame->style()->unpolish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->style()->polish(ui->dayOfBirthFrame);
        ui->dayOfBirthFrame->update();

        ui->dayWarningLabel->hide();

        ui->dayComboBox->setCurrentIndex(-1);

        ui->monthOfBirthFrame->setProperty("lineInFocus", "false");
        ui->monthOfBirthFrame->setProperty("validInputText", "true");
        ui->monthOfBirthFrame->style()->unpolish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->style()->polish(ui->monthOfBirthFrame);
        ui->monthOfBirthFrame->update();

        ui->monthWarningLabel->hide();

        ui->monthComboBox->setCurrentIndex(-1);

        ui->yearOfBirthFrame->setProperty("lineInFocus", "false");
        ui->yearOfBirthFrame->setProperty("validInputText", "true");
        ui->yearOfBirthFrame->style()->unpolish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->style()->polish(ui->yearOfBirthFrame);
        ui->yearOfBirthFrame->update();

        ui->yearWarningLabel->hide();


        ui->ageErrorFrame->hide();

        ui->yearComboBox->setCurrentIndex(-1);

        ui->SW_continueButton->setEnabled(false);
    }
    else if (ui->stackedWidget->currentWidget() == ui->b_createAccount)
    {
        ui->emailFrame->setProperty("lineInFocus", "false");
        ui->emailFrame->setProperty("validInputText", "true");
        ui->emailFrame->style()->unpolish(ui->emailFrame);
        ui->emailFrame->style()->polish(ui->emailFrame);
        ui->emailFrame->update();

        ui->emailWarningLabel->hide();
        ui->emailLine->clear();

        ui->nickNameFrame->setProperty("lineInFocus", "false");
        ui->nickNameFrame->setProperty("validInputText", "true");
        ui->nickNameFrame->style()->unpolish(ui->nickNameFrame);
        ui->nickNameFrame->style()->polish(ui->nickNameFrame);
        ui->nickNameFrame->update();

        ui->nickNameWarningLabel->hide();
        ui->nickNameLine->clear();

        ui->passwordFrame->setProperty("lineInFocus", "false");
        ui->passwordFrame->setProperty("validInputText", "true");
        ui->passwordFrame->style()->unpolish(ui->passwordFrame);
        ui->passwordFrame->style()->polish(ui->passwordFrame);
        ui->passwordFrame->update();

        ui->passwordWarningLabel->hide();
        ui->passwordLine->clear();

        ui->repeatPasswordFrame->setProperty("lineInFocus", "false");
        ui->repeatPasswordFrame->setProperty("validInputText", "true");
        ui->repeatPasswordFrame->style()->unpolish(ui->repeatPasswordFrame);
        ui->repeatPasswordFrame->style()->polish(ui->repeatPasswordFrame);
        ui->repeatPasswordFrame->update();

        ui->repeatPasswordWarningLabel->hide();
        ui->repeatPasswordLine->clear();

        ui->emailErrorFrame->hide();
        ui->SW_createAccountButton->setEnabled(false);

        ui->accountCreatedLabel->hide();
    }
    else
    {
        ; // Do nothing
    }

}   /* resetUI() */

/******************************************************************************/
/*!
 * @brief   Updates the visual state of the input frames based on focus and
 *          input validity.
 * @param   obj    Pointer to the (nickname or password) edited object.
 * @param   state  The state of the frame (IN_FOCUS, NOT_IN_FOCUS,
 *                 CORRECT_INPUT, WRONG_INPUT).
 * @return  void
 */
void SingWindow::editFrameState(QObject * obj, frameState_t state)
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
            ui->dayWarningLabel->show();
        }
        else if (obj == ui->monthComboBox)
        {
            ui->monthWarningLabel->show();
        }
        else if (obj == ui->yearComboBox)
        {
            ui->yearWarningLabel->show();
        }
        else if (obj == ui->emailLine)
        {
            ui->emailWarningLabel->show();
        }
        else if (obj == ui->nickNameLine)
        {
            ui->nickNameWarningLabel->show();
        }
        else if (obj == ui->passwordLine)
        {
            ui->passwordWarningLabel->show();
        }
        else if (obj == ui->repeatPasswordLine)
        {
            ui->repeatPasswordWarningLabel->show();
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
            ui->dayWarningLabel->hide();
        }
        else if (obj == ui->monthComboBox)
        {
            ui->monthWarningLabel->hide();
        }
        else if (obj == ui->yearComboBox)
        {
            ui->yearWarningLabel->hide();
        }
        else if (obj == ui->emailLine)
        {
            ui->emailWarningLabel->hide();
        }
        else if (obj == ui->nickNameLine)
        {
            ui->nickNameWarningLabel->hide();
        }
        else if (obj == ui->passwordLine)
        {
            ui->passwordWarningLabel->hide();
        }
        else if (obj == ui->repeatPasswordLine)
        {
            ui->repeatPasswordWarningLabel->hide();
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
    else if (obj == ui->repeatPasswordLine)
    {
        frameInFocus = ui->repeatPasswordFrame;
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
void SingWindow::onLineEditingFinished(QObject * obj)
{
    // Cast the object to QLineEdit
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(obj);
    QString text = lineEdit->text(); // Get the text from the line edit

    if (ui->stackedWidget->currentWidget() == ui->b_createAccount)
    {
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
                        tr("* Email is not valid"));
                }
            }
            else
            {
                frameState = CORRECT_INPUT;
            }

            // Update frame state for wrong input
            editFrameState(obj, frameState);
        }
        // Validate the input for the nickname line edit
        else if (lineEdit == ui->nickNameLine)
        {
            lineCheck_t nickNameCheck = isValidNickName(text);

            if (nickNameCheck == IS_VALID && !game.existsNickName(text))
            {
                // Update frame state
                editFrameState(obj, CORRECT_INPUT);
            }
            else
            {
                if (game.existsNickName(text))
                {
                    ui->nickNameWarningLabel->setText(
                        tr("* Sorry, this username is already in use"));
                }
                else
                {
                switch (nickNameCheck)
                {
                    case EMPTY:
                        ui->nickNameWarningLabel->setText(
                            tr("* Mandatory o"));
                        break;

                    case NOT_VALID:
                        ui->nickNameWarningLabel->setText(
                            tr("* Username is not valid"));
                        break;

                    case TOO_SHORT:
                        ui->nickNameWarningLabel->setText(
                            tr("* Too short o"));
                        break;

                    case TOO_LONG:
                        ui->nickNameWarningLabel->setText(
                            tr("* Too long o"));
                        break;

                    default:
                        break;
                    }
                }

                // Update frame state for wrong input
                editFrameState(obj, WRONG_INPUT);
            }
        }
        // Validate the input for the password line edit
        // Validate the input for the repeat passwoer line edit
        else if (lineEdit == ui->passwordLine ||
                 lineEdit == ui->repeatPasswordLine)
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

            frameState_t frameState = WRONG_INPUT;

            if (ui->repeatPasswordLine->text() == "")
            {
                ui->repeatPasswordWarningLabel->setText(
                    tr("* Mandatory a"));
            }
            else if (ui->passwordLine->text() !=
                     ui->repeatPasswordLine->text())
            {
                ui->repeatPasswordWarningLabel->setText(
                    tr("* Passwords do not match"));
            }
            else
            {
                frameState = CORRECT_INPUT;
            }

            editFrameState(
                qobject_cast<QObject *>(ui->repeatPasswordLine),
                frameState); // Update frame state for wrong input
        }
        else
        {
            ; // Do nothing
        }
    }

}   /* onLineEditingFinished() */


/*** end of file ***/
