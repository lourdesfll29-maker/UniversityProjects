/**
 * @file     logwindow.cpp
 *
 * @brief    Implementation of LogWindow class functions.
 *
 * @version  0.5  (2024/01/24) Final version
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
#include <QMessageBox>

/// Own includes
#include "logwindow.h"
#include "singwindow.h"
#include "ui_logwindow.h"
#include "connect4.h"
#include "player.h"
#include "mainmenu.h"
#include "startwindow.h"
#include "gameboard.h"


//-----[ IMPLEMENTATION OF PUBLIC LOGWINDOW FUNCTIONS ]-----------------------//

/******************************************************************************/
/*!
 * @brief   LogWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of LogWindow.
 * @param   playerToBeLogged  Blabla.
 */
LogWindow::LogWindow(QWidget * parent, playerToBeLogged_t playerToBeLogged)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::LogWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
    , loggedPlayer(nullptr)
    , playerToBeLogged(playerToBeLogged)
    , securityCode(0)
{
    // Set up the UI components for LogWindow
    ui->setupUi(this);

    // Install event filters for the input lines
    ui->nickNameLine->installEventFilter(this);
    ui->passwordLine->installEventFilter(this);

    ui->emailLine->installEventFilter(this);

    ui->securityCodeLine->installEventFilter(this);

    ui->newPasswordLine->installEventFilter(this);

    /// CONNECTING SIGNALS AND SLOTS

    connect(
        ui->a_backButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);

    connect(
        ui->forgottenPasswordButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);

    connect(
        ui->logInErrorLabel
        , &QLabel::linkActivated
        , this
        , &LogWindow::onLogInErrorLinkActivated);

    connect(
        ui->showPasswordButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onShowPasswordClicked);

    connect(
        ui->logInButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onLogInClicked);

    connect(
        ui->nickNameLine
        , &QLineEdit::returnPressed
        , this
        , &LogWindow::onLineReturnPressed);

    connect(
        ui->passwordLine
        , &QLineEdit::returnPressed
        , this
        , &LogWindow::onLineReturnPressed);

    connect(
        ui->singInButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);



    connect(
        ui->b_backButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);

    connect(
        ui->emailLine
        , &QLineEdit::returnPressed
        , this
        , &LogWindow::onLineReturnPressed);

    connect(
        ui->sendSecurityCodeButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);



    connect(
        ui->c_backButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);

    connect(
        ui->securityCodeLine
        , &QLineEdit::returnPressed
        , this
        , &LogWindow::onLineReturnPressed);

    connect(
        ui->helpLabel
        , &QLabel::linkActivated
        , this
        , &LogWindow::onLogInErrorLinkActivated);

    connect(
        ui->checkSecurityCodeButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);



    connect(
        ui->d_backButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);

    connect(
        ui->newPasswordLine
        , &QLineEdit::returnPressed
        , this
        , &LogWindow::onLineReturnPressed);

    connect(
        ui->showNewPasswordButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onShowPasswordClicked);

    connect(
        ui->createNewPasswordButton
        , &QPushButton::clicked
        , this
        , &LogWindow::onButtonClicked);

    connect(
        ui->passwordChangedLabel
        , &QLabel::linkActivated
        , this
        , &LogWindow::onLogInErrorLinkActivated);

    /// SET INITIAL CONFIGURATION OF THE INTERFACE

    resetUI();

}   /* LogWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the LogWindow class destructor.
 * @param   void
 */
LogWindow::~LogWindow(void)
{
    // Delete the UI components associated with LogWindow
    delete ui;

}   /* ~LogWindow() */


//-----[ IMPLEMENTATION OF PROTECTED LOGWINDOW FUNCTIONS ]--------------------//

/******************************************************************************/
/*!
 * @brief   Handles events for the LogWindow class.
 *
 * This function filters events to manage focus state and
 * validate user input for the login process.
 *
 * @param   obj    Pointer to the object that triggered the event.
 * @param   event  Pointer to the event that occurred.
 * @return  Returns true if the event is handled, false otherwise.
 */
bool LogWindow::eventFilter(QObject * obj, QEvent * event)
{
    // Check if the event is a focus-in event
    if (event->type() == QEvent::FocusIn)
    {
        editFrameState(obj, IN_FOCUS); // Update frame state to focused
    }
    // Check if the event is a focus-out event
    else if (event->type() == QEvent::FocusOut)
    {
        editFrameState(obj, NOT_IN_FOCUS); // Update frame state to not focused
        onLineEditingFinished(obj); // Handle line editing completion
    }
    // Ignore other events
    else
    {
        ; // Do nothing
    }

    if (ui->stackedWidget->currentWidget() == ui->a_logIn)
    {
        // Validate user input for login button activation
        if (isValidNickName(ui->nickNameLine->text()) == IS_VALID &&
            (!ui->passwordLine->text().isEmpty()))
        {
            ui->logInButton->setEnabled(true);
        }
        else
        {
            ui->logInButton->setEnabled(false);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->b_resetPassword)
    {
        // Validate user input for login button activation
        if (isValidEmail(ui->emailLine->text()))
        {
            ui->sendSecurityCodeButton->setEnabled(true);
        }
        else
        {
            ui->sendSecurityCodeButton->setEnabled(false);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->c_checkInbox)
    {
        // Validate user input for login button activation
        if (ui->securityCodeLine->text().length() == 6)
        {
            ui->checkSecurityCodeButton->setEnabled(true);
        }
        else
        {
            ui->checkSecurityCodeButton->setEnabled(false);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->d_createNewPassword)
    {
        // Validate user input for login button activation
        if (isValidPassword(ui->newPasswordLine->text()) == IS_VALID)
        {
            ui->createNewPasswordButton->setEnabled(true);
        }
        else
        {
            ui->createNewPasswordButton->setEnabled(false);
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
void LogWindow::mousePressEvent(QMouseEvent * event)
{
    // Check if the click is outside the nickname and password frames
    if (!(ui->nickNameFrame->geometry().contains(event->pos()))     ||
        !(ui->passwordFrame->geometry().contains(event->pos()))     ||
        !(ui->emailFrame->geometry().contains(event->pos()))        ||
        !(ui->securityCodeFrame->geometry().contains(event->pos())) ||
        !(ui->newPasswordFrame->geometry().contains(event->pos())))
    {
        QWidget * widget = qobject_cast<QWidget *>(QApplication::focusObject());

        if (widget)
        {
            widget->clearFocus(); // Clear focus from the widget
        }

        editFrameState(QApplication::focusObject(), NOT_IN_FOCUS);
    }

}   /* mousePressEvent() */


//-----[ IMPLEMENTATION OF PRIVATE SLOT LOGWINDOW FUNCTIONS ]-----------------//

/******************************************************************************/
/*!
 * @brief   Handles the addition of an item by showing the side menu.
 * @param   void
 * @return  void
 */
void LogWindow::onButtonClicked(void)
{
    // Retrieve the QPushButton that triggered this event
    QPushButton * buttonClicked = qobject_cast<QPushButton *>(sender());

    if (buttonClicked == ui->a_backButton)
    {
        if (qobject_cast<GameBoard *>(
                mainwindow->getPreviousWindow()) != nullptr)
        {
            StartWindow * newWidget = new StartWindow(mainwindow);
            mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
        }
        else if (mainwindow->getPreviousWindow())
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
        ui->stackedWidget->setCurrentWidget(ui->a_logIn);
        resetUI();
    }
    else if ((buttonClicked == ui->c_backButton) ||
             (buttonClicked == ui->forgottenPasswordButton))
    {
        ui->stackedWidget->setCurrentWidget(ui->b_resetPassword);
        resetUI();
    }
    else if ((buttonClicked == ui->d_backButton) ||
             (buttonClicked == ui->forgottenPasswordButton))
    {
        ui->stackedWidget->setCurrentWidget(ui->c_checkInbox);
        resetUI();
    }
    else if (buttonClicked == ui->singInButton)
    {
        QWidget * newWidget = new SingWindow(mainwindow, playerToBeLogged);

        if (!(mainwindow->getPreviousWindow()))
        {
            QPushButton * button;
            button = newWidget->findChild<QPushButton *>("a_backButton");
            button->setEnabled(false);
        }

        mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
    }

    else if (buttonClicked == ui->sendSecurityCodeButton)
    {
        this->email = ui->emailLine->text();

        // Access the Connect4 game instance
        Connect4 & game = Connect4::getInstance();

        // Attempt to log in the player with provided credentials
        QList<Player*> playersList = game.getRanking();

        bool emailFound = false;

        // Iterate through the list of players
        for (Player * player : playersList)
        {
            // Check if the player's email matches the one we're looking for
            if (player->getEmail() == this->email)
            {
                // Generate a 6-digit random number
                this->securityCode = generateRandomCode();

                qDebug() << "Player found with email:" << this->email
                         << ", Security Code: " << this->securityCode;

                this->loggedPlayer = player;
                emailFound = true;
                break; // Exit the loop if the player is found
            }
        }

        ui->stackedWidget->setCurrentWidget(ui->c_checkInbox);
        resetUI();

        if (!emailFound)
        {
            // If no player was found with the given email
            qDebug() << "No player found with email:" << this->email;
        }
        else
        {
            // Create the QMessageBox
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Security Code"));

            // Create the message with HTML formatting
            QString message;
            message = tr("Copy the following code to recover your account:<br><br>"
                         "<span style='font-size: 24px; font-weight: bold;'>") +
                         QString::number(securityCode) + "</span>";

            msgBox.setText(message);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);

            // Adjust the size of the QMessageBox
            msgBox.setFixedSize(300, 200);

            // Display the QMessageBox
            msgBox.exec();
        }
    }
    else if (buttonClicked == ui->checkSecurityCodeButton)
    {
        if (ui->securityCodeLine->text().toInt() == this->securityCode)
        {
            ui->stackedWidget->setCurrentWidget(ui->d_createNewPassword);
            resetUI();
        }
        else
        {
            ui->securityCodeErrorFrame->show();
        }

    }
    else if (buttonClicked == ui->createNewPasswordButton)
    {
        if (this->loggedPlayer)
        {            
            qDebug() << "password changed";
            this->loggedPlayer->setPassword(ui->newPasswordLine->text());
        }

        // Display that the password has been changed
        ui->passwordChangedFrame->show();

        ui->youMustFrame->hide();
        ui->verticalSpacer_19->changeSize(0, 0);

        ui->preCreateNewPasswordFrame->setEnabled(false);

        QList<QWidget *> children;
        children = ui->preCreateNewPasswordFrame->findChildren<QWidget *>();

        // Iterate through all children and set their enabled state
        for (QWidget * child : children)
        {
            child->setEnabled(false); // Disable child widgets
        }
    }

}   /* onAddClicked() */

/******************************************************************************/
/*!
 * @brief   Handles activation of links in the log-in error window.
 * @param   link  The link that was activated.
 * @return  void
 */
void LogWindow::onLogInErrorLinkActivated(const QString & link)
{
    if (link == "#resend_code")
    {
        // Create the QMessageBox
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Security Code"));

        // Create the message with HTML formatting
        QString message;
        message = tr("Copy the following code to recover your account:<br><br>"
                     "<span style='font-size: 24px; font-weight: bold;'>") +
                      QString::number(securityCode) + "</span>";

        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);

        // Adjust the size of the QMessageBox
        msgBox.setFixedSize(300, 200);

        // Show the QMessageBox
        msgBox.show();
    }
    else if (link == "#go_to_b_preResetPassword")
    {
        ui->stackedWidget->setCurrentWidget(ui->b_resetPassword);
        resetUI();
    }
    else if (link == "#go_to_a_logIn")
    {
        QWidget * newWidget = new LogWindow(mainwindow);

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

}   /* onLogInErrorLinkActivated() */

/******************************************************************************/
/*!
 * @brief   Toggles the visibility of the password in the password field.
 * @param   void
 * @return  void
 */
void LogWindow::onShowPasswordClicked(void)
{
    // Check if the show password button is checked
    if (ui->stackedWidget->currentWidget() == ui->a_logIn)
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
    else if (ui->stackedWidget->currentWidget() == ui->d_createNewPassword)
    {
        if (ui->showNewPasswordButton->isChecked())
        {
            // Show password
            ui->newPasswordLine->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            // Hide password
            ui->newPasswordLine->setEchoMode(QLineEdit::Password);
        }
    }
    else
    {
        ; // Do nothing
    }

} /* onShowPasswordClicked() */

/******************************************************************************/
/*!
 * @brief   Handles the login process when the login button is clicked.
 * @param   void
 * @return  void
 */
void LogWindow::onLogInClicked(void)
{
    // Access the Connect4 game instance
    Connect4 & game = Connect4::getInstance();

    // Attempt to log in the player with provided credentials
    Player * player = game.loginPlayer(
        ui->nickNameLine->text(),
        ui->passwordLine->text());

    // Check if the login was successful
    if (player)
    {
        qDebug() << "User login successful";
        mainwindow->currentPlayer = player;

        if (playerToBeLogged == PLAYER1)
        {
            mainwindow->player1 = player;
        }
        else
        {
            mainwindow->player2 = player;
        }

        mainwindow->userLogged();

        if (mainwindow->getPreviousWindow() &&
            (qobject_cast<StartWindow *>(
                mainwindow->getPreviousWindow())) == nullptr)
        {
            mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WINDOW);
        }
        else
        {
            QWidget * newWidget = new mainMenu(mainwindow);
            mainwindow->changeCentralWidget(newWidget, DELETE_PREV_WIDGET);
        }
    }
    else
    {
        ui->logInErrorFrame->show(); // Show error frame on login failure
    }

}   /* onLogInClicked() */

/******************************************************************************/
/*!
 * @brief   Handles the return key press event in the line edit fields.
 * @param   void
 * @return  void
 */
void LogWindow::onLineReturnPressed(void)
{
    // Retrieve the QLineEdit that triggered this event
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(sender());

    // Check if the sender is the nickname line edit
    if (lineEdit == ui->nickNameLine)
    {
        // Clear focus from the nickname line edit and set focus to password
        ui->nickNameLine->clearFocus();
        ui->passwordLine->setFocus();
    }
    // Check if the sender is the password line edit
    else if (lineEdit == ui->passwordLine)
    {
        // Clear focus from the password line edit and trigger login
        ui->passwordLine->clearFocus();
        ui->logInButton->click();
    }
    // Check if the sender is the password line edit
    else if (lineEdit == ui->emailLine)
    {
        // Clear focus from the password line edit and trigger login
        ui->emailLine->clearFocus();
        ui->sendSecurityCodeButton->click();
    }
    // Check if the sender is the password line edit
    else if (lineEdit == ui->securityCodeLine)
    {
        // Clear focus from the password line edit and trigger login
        ui->securityCodeLine->clearFocus();
        ui->checkSecurityCodeButton->click();
    }
    // Check if the sender is the password line edit
    else if (lineEdit == ui->newPasswordLine)
    {
        // Clear focus from the password line edit and trigger login
        ui->newPasswordLine->clearFocus();
        ui->createNewPasswordButton->click();
    }
    // If the sender is neither, do nothing
    else
    {
        ; // Do nothing
    }

}   /* onLineReturnPressed() */


//-----[ IMPLEMENTATION OF PRIVATE LOGWINDOW FUNCTIONS ]----------------------//

/*****************************************************************************/
/*!
 * @brief   Resets the user interface elements based on the current widget.
 * @param   void
 * @return  void
 */
void LogWindow::resetUI(void)
{
    this->setFocus();

    QString aux = tr("<html><head/><body><p>Enter the 6-digit security"
                     " code we sent to <span style=\" font-weight:700;\">")
                  + (this->email) + "</span>.</p></body></html>";

    ui->checkInboxInfoLabel->setText(tr((aux.toUtf8()).constData()));


    if (ui->stackedWidget->currentWidget() == ui->a_logIn)
    {
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

        ui->showPasswordButton->setChecked(false);
        ui->passwordLine->setEchoMode(QLineEdit::Password); // Hide password

        ui->passwordWarningLabel->hide();
        ui->passwordLine->clear();

        ui->logInErrorFrame->hide();

        ui->logInButton->setEnabled(false);
    }
    else if (ui->stackedWidget->currentWidget() == ui->b_resetPassword)
    {
        ui->emailFrame->setProperty("lineInFocus", "false");
        ui->emailFrame->setProperty("validInputText", "true");
        ui->emailFrame->style()->unpolish(ui->emailFrame);
        ui->emailFrame->style()->polish(ui->emailFrame);
        ui->emailFrame->update();

        ui->emailWarningLabel->hide();
        ui->emailLine->clear();

        ui->sendSecurityCodeButton->setEnabled(false);
    }
    else if (ui->stackedWidget->currentWidget() == ui->c_checkInbox)
    {
        ui->securityCodeFrame->setProperty("lineInFocus", "false");
        ui->securityCodeFrame->setProperty("validInputText", "true");
        ui->securityCodeFrame->style()->unpolish(ui->securityCodeFrame);
        ui->securityCodeFrame->style()->polish(ui->securityCodeFrame);
        ui->securityCodeFrame->update();

        ui->securityCodeWarningLabel->hide();
        ui->securityCodeLine->clear();

        ui->securityCodeErrorFrame->hide();

        ui->checkSecurityCodeButton->setEnabled(false);
    }
    else if (ui->stackedWidget->currentWidget() == ui->d_createNewPassword)
    {
        ui->preCreateNewPasswordFrame->setEnabled(true);

        ui->newPasswordFrame->setProperty("lineInFocus", "false");
        ui->newPasswordFrame->setProperty("validInputText", "true");
        ui->newPasswordFrame->style()->unpolish(ui->newPasswordFrame);
        ui->newPasswordFrame->style()->polish(ui->newPasswordFrame);
        ui->newPasswordFrame->update();

        ui->showNewPasswordButton->setChecked(false);
        ui->newPasswordLine->setEchoMode(QLineEdit::Password); // Hide password

        ui->newPasswordWarningLabel->hide();
        ui->newPasswordLine->clear();

        ui->passwordChangedFrame->hide();
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
void LogWindow::editFrameState(QObject * obj, frameState_t state)
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
        if (obj == ui->nickNameLine)
        {
            ui->nickNameWarningLabel->show();
        }
        else if (obj == ui->passwordLine)
        {
            ui->passwordWarningLabel->show();
        }
        else if (obj == ui->emailLine)
        {
            ui->emailWarningLabel->show();
        }
        else if (obj == ui->securityCodeLine)
        {
            ui->securityCodeWarningLabel->show();
        }
        else if (obj == ui->newPasswordLine)
        {
            ui->newPasswordWarningLabel->show();
        }
        else
        {
            ; // Do nothing
        }
    }
    else
    {
        if (obj == ui->nickNameLine)
        {
            ui->nickNameWarningLabel->hide();
        }
        else if (obj == ui->passwordLine)
        {
            ui->passwordWarningLabel->hide();
        }
        else if (obj == ui->emailLine)
        {
            ui->emailWarningLabel->hide();
        }
        else if (obj == ui->securityCodeLine)
        {
            ui->securityCodeWarningLabel->hide();
        }
        else if (obj == ui->newPasswordLine)
        {
            ui->newPasswordWarningLabel->hide();
        }
        else
        {
            ; // Do nothing
        }
    }

    if (obj == ui->nickNameLine)
    {
        frameInFocus = ui->nickNameFrame;
    }
    else if (obj == ui->passwordLine)
    {
        frameInFocus = ui->passwordFrame;
    }
    else if (obj == ui->emailLine)
    {
        frameInFocus = ui->emailFrame;
    }
    else if (obj == ui->securityCodeLine)
    {
        frameInFocus = ui->securityCodeFrame;
    }
    else if (obj == ui->newPasswordLine)
    {
        frameInFocus = ui->newPasswordFrame;
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
void LogWindow::onLineEditingFinished(QObject * obj)
{
    // Cast the object to QLineEdit
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(obj);
    QString text = lineEdit->text(); // Get the text from the line edit

    if (ui->stackedWidget->currentWidget() == ui->a_logIn)
    {
        // Validate the input for the nickname line edit
        if (lineEdit == ui->nickNameLine)
        {
            lineCheck_t nickNameCheck = isValidNickName(text);

            if (nickNameCheck == IS_VALID)
            {
                editFrameState(obj, CORRECT_INPUT); // Update frame state
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

                // Update frame state for wrong input
                editFrameState(obj, WRONG_INPUT);
            }
        }
        // Validate the input for the password line edit
        else if (lineEdit == ui->passwordLine)
        {
            if (text == "")
            {
                // Update frame state for wrong input
                editFrameState(obj, WRONG_INPUT);
            }
            else
            {
                // Update frame state
                editFrameState(obj, CORRECT_INPUT);
            }
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->b_resetPassword)
    {
        // Validate the input for the nickname line edit
        if ((!(isValidEmail(text)) && lineEdit == ui->emailLine) ||
            text == "")
        {
            // Handle empty input for nickname line
            if (text == "")
            {
                if (lineEdit == ui->emailLine)
                {
                    ui->emailWarningLabel->setText(
                        tr("* Mandatory o"));
                }
            }
            // Handle invalid characters in nickname
            else
            {
                if (lineEdit == ui->emailLine)
                {
                    ui->emailWarningLabel->setText(
                        tr("* Email is not valid"));
                }
            }

            // Update frame state for wrong input
            editFrameState(obj, WRONG_INPUT);
        }
        else // Input is valid
        {
            // Update frame state
            editFrameState(obj, CORRECT_INPUT);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->c_checkInbox)
    {
        // Validate the input for the nickname line edit
        if (!(isValidSecurityCode(text)) || text == "")
        {
            // Handle empty input for security code line
            if (text == "")
            {
                if (lineEdit == ui->securityCodeLine)
                {
                    ui->securityCodeWarningLabel->setText(
                        tr("* Mandatory o"));
                }
            }
            // Handle invalid characters in nickname
            else
            {
                if (lineEdit == ui->securityCodeLine)
                {
                    ui->securityCodeWarningLabel->setText(
                        tr("* Security code is not valid"));
                }
            }

            // Update frame state for wrong input
            editFrameState(obj, WRONG_INPUT);
        }
        else // Input is valid
        {
            // Update frame state
            editFrameState(obj, CORRECT_INPUT);
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->d_createNewPassword)
    {
        if (lineEdit == ui->newPasswordLine)
        {
            lineCheck_t passwordCheck = isValidPassword(text);

            if (passwordCheck == IS_VALID)
            {
                editFrameState(obj, CORRECT_INPUT); // Update frame state
            }
            else
            {
                switch (passwordCheck)
                {
                    case EMPTY:
                        ui->newPasswordWarningLabel->setText(
                            tr("* Mandatory a"));
                        break;

                    case NOT_VALID:
                        ui->newPasswordWarningLabel->setText(
                            tr("* Password is not valid"));
                        break;

                    case TOO_SHORT:
                        ui->newPasswordWarningLabel->setText(
                            tr("* Too short a"));
                        break;

                    case TOO_LONG:
                        ui->newPasswordWarningLabel->setText(
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
