/**
 * @file     individualrankingwindow.cpp
 *
 * @brief    Implementation of IndividualRankingWindow class functions.
 *
 * @version  0.2  (2024/01/25) Final version
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
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QHash>
#include <QSet>
#include <QPen>
#include <QBrush>

/// Own includes
#include "individualrankingwindow.h"
#include "ui_individualrankingwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC INDIVIDUALRANKINGWINDOW FUNCTIONS ]---------//

/******************************************************************************/
/*!
 * @brief   IndividualRankingWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of IndividualRankingWindow.
 */
IndividualRankingWindow::IndividualRankingWindow(QWidget *parent, QString playerName)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::IndividualRankingWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
    , playerName(playerName)
{
    // Set up the UI components for RankingWindow
    ui->setupUi(this);

    /// CONNECTING SIGNALS AND SLOTS

    connect(ui->a_backButton
            , &QPushButton::clicked
            , this
            , &IndividualRankingWindow::onBackPressed);

    connect(ui->startDate
            , &QDateEdit::dateChanged
            , this
            , &IndividualRankingWindow::filteredTableandGraphics);

    connect(ui->endDate
            , &QDateEdit::dateChanged
            , this
            , &IndividualRankingWindow::filteredTableandGraphics);

    /// SET INITIAL CONFIGURATION OF THE INTERFACE

    setNickNameandAvatarandPoints();

    initializeTable();
    initializeDateEdits();

    gameHistoryTable();

}   /* IndividualRankingWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the IndividualRankingWindow class destructor.
 * @param   void
 */
IndividualRankingWindow::~IndividualRankingWindow(void)
{
    // Delete the UI components associated with IndividualRankingWindow
    delete ui;

}   /* ~IndividualRankingWindow() */


//-----[ IMPLEMENTATION OF PROTECTED INDIVIDUALRANKINGWINDOW FUNCTIONS ]------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT INDIVIDUALRANKINGWINDOW FUNCTIONS ]---//

/*****************************************************************************/
/*!
 * @brief   Handles the event when the "Back" button is pressed.
 * @param   void
 * @return  void
 */
void IndividualRankingWindow::onBackPressed(void)
{
    mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WIDGET);

}   /* onBackPressed() */

/******************************************************************************/
/*!
 * @brief   Filters the game history table and updates the graphics based on the
 *          selected date range.
 * @param   date  The date used to trigger the filtering and updating process.
 * @return  void
 */
void IndividualRankingWindow::filteredTableandGraphics(const QDate &date)
{
    Q_UNUSED(date);
    // Get the instance of Connect4
    Connect4 & c4 = Connect4::getInstance();

    // Find the player by name
    Player* player = c4.getPlayer(playerName);

    // Get the player's rounds
    QList<Round*> rounds = c4.getRoundsForPlayer(player);

    // Get the dates
    QDate startDate = ui->startDate->date();
    QDate endDate = ui->endDate->date();

    // Filter the rounds within the date range
    QList<Round*> filteredRounds;

    for (Round* round : rounds) {
        QDate roundDate = round->getTimestamp().date();
        QString winner = round->getWinner()->getNickName();
        QString loser = round->getLoser()->getNickName();

        if ((roundDate >= startDate && roundDate <= endDate) &&
            winner != "CPU" && winner != "Invitado 1" &&
            winner != "Invitado 2" && loser != "CPU" &&
            loser != "Invitado 1" && loser != "Invitado 2")
        {
            filteredRounds.append(round);
        }
    }

    if (!filteredRounds.isEmpty())
    {
        // Sort the rounds by time (most recent first)
        std::sort(filteredRounds.begin(), filteredRounds.end(), [](Round* a,
                                                                   Round* b) {
            return a->getTimestamp() > b->getTimestamp();
        });

        // Update the table with the filtered players
        updateTable(filteredRounds);

        // Update the charts with the filtered data
        showResultsBarChart(filteredRounds); // Update the results chart
        showOpponentBarChart(filteredRounds); // Update the opponents chart

        // Free memory of the unused rounds
        qDeleteAll(rounds);
    }
    else
    {
        // Update the table with the filtered players
        updateTable(filteredRounds);

        // Update the charts with the filtered data
        showResultsBarChart(filteredRounds); // Update the results chart
        showOpponentBarChart(filteredRounds); // Update the opponents chart
    }

}   /* filteredTableandGraphics() */


//-----[ IMPLEMENTATION OF PRIVATE INDIVIDUALRANKINGWINDOW FUNCTIONS ]--------//

/******************************************************************************/
/*!
 * @brief   Initialize the history table.
 * @param   void
 * @return  void
 */
void IndividualRankingWindow::initializeTable(void)
{
    ui->opponentsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->opponentsTableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->opponentsTableWidget->setSelectionMode(QTableWidget::NoSelection);
    ui->opponentsTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

}   /* initializeTable() */

/******************************************************************************/
/*!
 * @brief   Initialize the date edits for the individual ranking window.
 * @param   void
 * @return  void
 */
void IndividualRankingWindow::initializeDateEdits(void)
{
    // Get the instance of Connect4
    Connect4 & c4 = Connect4::getInstance();

    // Find the player by name
    Player* player = c4.getPlayer(playerName);

    // Get the player's rounds
    QList<Round*> rounds = c4.getRoundsForPlayer(player);

    // Determine the earliest date recorded for the player
    QDate earliestDate = QDate::currentDate();
    for (Round* round : rounds) {
        QDate roundDate = round->getTimestamp().date();
        if (roundDate < earliestDate) {
            earliestDate = roundDate;
        }
    }

    // Configure the QDateEdits
    ui->startDate->setMinimumDate(earliestDate);
    ui->startDate->setMaximumDate(QDate::currentDate());
    ui->startDate->setDate(earliestDate);

    ui->endDate->setMinimumDate(earliestDate);
    ui->endDate->setMaximumDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());

    // Validate that the start date is not greater than the end date
    if (ui->startDate->date() > ui->endDate->date()) {
        ui->endDate->setDate(ui->startDate->date());
    }

    // Validate that the end date is not less than the start date
    if (ui->endDate->date() < ui->startDate->date()) {
        ui->startDate->setDate(ui->endDate->date());
    }

}   /* initializeDateEdits() */

/******************************************************************************/
/*!
 * @brief   Sets the player's nickname, avatar, and total points in the UI.
 * @param   void
 * @return  void
 */
void IndividualRankingWindow::setNickNameandAvatarandPoints(void)
{
    // Get the instance of Connect4
    Connect4 &c4 = Connect4::getInstance();

    ui->usernameLabel->setText(playerName);

    Player* player = c4.getPlayer(playerName);
    QPixmap playerAvatar = QPixmap::fromImage(player->getAvatar());
    ui->avatarLabel->setPixmap(playerAvatar.scaled(70, 70,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));

    ui->rankingPointsLabel->setText(tr("<b>Puntuación Total :</b> ") +
                             QString::number(player->getPoints()));

}   /* setNickNameandAvatarandPoints() */

/******************************************************************************/
/*!
 * @brief   Displays a bar chart of the player's game results.
 * @param   filteredRounds  The list of filtered rounds to display in the chart.
 * @return  void
 */
void IndividualRankingWindow::showResultsBarChart(const QList<Round*>& filteredRounds)
{
    Connect4 &c4 = Connect4::getInstance();

    Player *player = c4.getPlayer(playerName);
    if (!player) return;

    QGraphicsScene *scene = new QGraphicsScene(this);

    int barWidth = 50;    // Width of the bars
    int maxHeight = 200;  // Maximum height of the bars
    int xOffset = 100;    // Initial horizontal offset
    int yOffset = 20;     // Vertical offset from the top edge

    // Count total victories and defeats
    int totalVictories = 0;
    int totalDefeats = 0;

    int maxCount = 5;

    if (!filteredRounds.isEmpty())
    {
        for (Round *round : filteredRounds)
        {
            QString opponentName = (round->getWinner() == player) ?
                                       round->getLoser()->getNickName() :
                                       round->getWinner()->getNickName();

            if (opponentName == "CPU" ||
                opponentName == "Invitado 1" ||
                opponentName == "Invitado 2")
            {
                continue; // Exclude these games
            }

            if (round->getWinner()->getNickName() == player->getNickName())
            {  totalVictories++;  }
            else {  totalDefeats++;    }
        }

        maxCount = std::max(totalVictories, totalDefeats);

        // Calculate proportional height
        if (maxCount == 0) maxCount = 1; // Avoid division by zero

        int victoryHeight = (totalVictories * maxHeight) / maxCount;
        int defeatHeight = (totalDefeats * maxHeight) / maxCount;

        // Draw victory bar
        QRectF victoryBar(xOffset
                          , maxHeight - victoryHeight + yOffset
                          , barWidth
                          , victoryHeight);
        scene->addRect(victoryBar, QPen(Qt::black), QBrush(Qt::green));

        // Draw defeat bar
        QRectF defeatBar(xOffset + barWidth + 50
                         , maxHeight - defeatHeight + yOffset
                         , barWidth
                         , defeatHeight);
        scene->addRect(defeatBar, QPen(Qt::black), QBrush(Qt::red));

        // Bar labels
        QGraphicsTextItem *victoryLabel = scene->addText("Victorias");
        victoryLabel->setPos(xOffset + barWidth / 2 - 20
                             , maxHeight + yOffset + 10);

        QGraphicsTextItem *defeatLabel = scene->addText("Derrotas");
        defeatLabel->setPos(xOffset + barWidth + 50 + barWidth / 2 - 20
                            , maxHeight + yOffset + 10);

        // Draw vertical axis (Number of results)
        scene->addLine(xOffset - 10
                       , yOffset
                       , xOffset - 10
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        // Set the number of divisions
        int divisions = 5;

        if (maxCount > 5)
        {
            for (int i = 0; i <= divisions; ++i)
            {
                // Calculate the corresponding value
                int value = (maxCount * i) / divisions;
                // Calculate the Y position
                int y = maxHeight - (i * maxHeight / divisions) + yOffset;

                // Draw a small line on the axis for the division
                scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

                // Label with the value
                QGraphicsTextItem *numberLabel = scene->addText(QString::number(value));
                numberLabel->setPos(xOffset - 40, y - 10);
            }
        }
        else
        {
            // Draw vertical axis (Number of results)
            scene->addLine(xOffset - 10
                           , yOffset
                           , xOffset - 10
                           , maxHeight + yOffset
                           , QPen(Qt::black));

            for (int i = 0; i <= maxCount; ++i)
            {
                int y = maxHeight - (i * maxHeight / maxCount) + yOffset;

                scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

                QGraphicsTextItem *numberLabel = scene->addText(QString::number(i));
                numberLabel->setPos(xOffset - 40, y - 10);
            }

        }

        // Draw horizontal axis
        scene->addLine(xOffset - 10
                       , maxHeight + yOffset
                       , xOffset + 2 * (barWidth + 50)
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        ui->gamesGraphicsView->setScene(scene);

    }
    else
    {
        // Draw vertical axis (Number of results)
        scene->addLine(xOffset - 10
                       , yOffset
                       , xOffset - 10
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        for (int i = 0; i <= maxCount; ++i)
        {
            int y = maxHeight - (i * maxHeight / maxCount) + yOffset;

            scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

            QGraphicsTextItem *numberLabel = scene->addText(QString::number(i));
            numberLabel->setPos(xOffset - 40, y - 10);
        }

        // Draw horizontal axis
        scene->addLine(xOffset - 10
                       , maxHeight + yOffset
                       , xOffset + 2 * (barWidth + 50)
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        ui->gamesGraphicsView->setScene(scene);

    }

    // Adjust the view to the content
    ui->gamesGraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->gamesGraphicsView->setSceneRect(scene->itemsBoundingRect());
    ui->gamesGraphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    // Configure scroll bars to avoid interference
    ui->gamesGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gamesGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}   /* showResultsBarChart() */

/******************************************************************************/
/*!
 * @brief   Displays a bar chart of the player's oppponents.
 * @param   filteredRounds  The list of filtered rounds to display in the chart.
 * @return  void
 */
void IndividualRankingWindow::showOpponentBarChart(const QList<Round*>& filteredRounds)
{
    Connect4 &c4 = Connect4::getInstance();

    Player *player = c4.getPlayer(playerName);
    if (!player) return;

    QHash<QDate, QSet<QString>> opponentsByDate;

    QGraphicsScene *scene = new QGraphicsScene(this);

    int barWidth = 40;    // Width of the bars
    int maxHeight = 200;  // Maximum height of the bars
    int xOffset = 50;     // Initial horizontal offset
    int yOffset = 20;     // Vertical offset from the top edge
    int spacing = 10;     // Spacing between bars

    int index = 0;

    int maxOpponents = 1;

    if (!filteredRounds.isEmpty())
    {
        for (Round *round : filteredRounds)
        {
            QDate date = round->getTimestamp().date();
            QString opponentName = (round->getWinner() == player) ?
                                       round->getLoser()->getNickName() :
                                       round->getWinner()->getNickName();

            // Exclude unwanted opponents
            if (opponentName != "CPU" &&
                opponentName != "Invitado 1" &&
                opponentName != "Invitado 2")
            {
                opponentsByDate[date].insert(opponentName);
            }
        }

        // Sort dates from most recent to oldest
        QList<QDate> sortedDates = opponentsByDate.keys();
        std::sort(sortedDates.begin(), sortedDates.end(), std::greater<QDate>());

        // Limit to the 6 most recent dates
        sortedDates = sortedDates.mid(0, 6);

        // Colors for the bars //////////////////////////// POR MODIFICAR ////
        QVector<QColor> barColors = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta, Qt::cyan};

        // Draw the bars
        for (const auto &date : sortedDates)
        {
            maxOpponents = std::max(maxOpponents,
                                    static_cast<int>(opponentsByDate[date].size()));
        }

        for (const auto &date : sortedDates)
        {
            int uniqueCount = opponentsByDate[date].size();

            // Scale the height proportionally
            int barHeight = (uniqueCount * maxHeight) / maxOpponents;

            QRectF barRect(xOffset + index * (barWidth + spacing)
                           , maxHeight - barHeight + yOffset
                           , barWidth
                           , barHeight);
            scene->addRect(barRect
                           , QPen(Qt::black)
                           , QBrush(barColors[index % barColors.size()]));

            // Date labels on the horizontal axis
            QGraphicsTextItem *dateLabel = scene->addText(date.toString("dd/MM"));
            dateLabel->setPos(xOffset + index * (barWidth + spacing)
                              , maxHeight + yOffset + 10);

            index++;
        }

        // Draw vertical axis (Number of opponents)
        scene->addLine(xOffset - 10
                       , yOffset
                       , xOffset - 10
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        for (int i = 0; i <= maxOpponents; ++i)
        {
            int y = maxHeight - (i * maxHeight / maxOpponents) + yOffset;

            scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

            QGraphicsTextItem *numberLabel = scene->addText(QString::number(i));
            numberLabel->setPos(xOffset - 40, y - 10);
        }

        // Draw horizontal axis
        scene->addLine(xOffset - 10
                       , maxHeight + yOffset
                       , xOffset + index * (barWidth + spacing) - spacing
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        ui->opponentsGraphicView->setScene(scene);

    }
    else
    {
        index = 5;
        maxOpponents = 5;

        // Draw vertical axis (Number of opponents)
        scene->addLine(xOffset - 10
                       , yOffset
                       , xOffset - 10
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        for (int i = 0; i <= maxOpponents; ++i)
        {
            int y = maxHeight - (i * maxHeight / maxOpponents) + yOffset;

            scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

            QGraphicsTextItem *numberLabel = scene->addText(QString::number(i));
            numberLabel->setPos(xOffset - 40, y - 10);
        }

        // Draw horizontal axis
        scene->addLine(xOffset - 10
                       , maxHeight + yOffset
                       , xOffset + 2 * (barWidth + 50)
                       , maxHeight + yOffset, QPen(Qt::black));

        ui->opponentsGraphicView->setScene(scene);

    }

    // Adjust the view to the content
    ui->opponentsGraphicView->setRenderHint(QPainter::Antialiasing);
    ui->opponentsGraphicView->setSceneRect(scene->itemsBoundingRect());
    ui->opponentsGraphicView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    // Configure scroll bars to avoid interference
    ui->opponentsGraphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->opponentsGraphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}   /* showOpponentsBarChart() */

/******************************************************************************/
/*!
 * @brief   Populates the game history table with the player's game data.
 * @param   void
 * @return  void
 */
void IndividualRankingWindow::gameHistoryTable(void)
{
    // Get the instance of Connect4
    Connect4 &c4 = Connect4::getInstance();
    Player *player = c4.getPlayer(playerName);

    QList<Round*> rounds = c4.getRoundsForPlayer(player);

    // Sort the rounds by date and time in descending order
    std::sort(rounds.begin(), rounds.end(), [](Round* a, Round* b) {
        return a->getTimestamp() > b->getTimestamp();
    });

    QList<Round*> filteredRounds;

    // Filter rounds with excluded opponents
    for (Round* round : rounds) {
        Player* opponent = (round->getWinner()->getNickName() == playerName)
        ? round->getLoser()
        : round->getWinner();

        QString opponentName = opponent->getNickName();

        // Exclude specific opponents
        if (opponentName != "CPU" &&
            opponentName != "Invitado 1" &&
            opponentName != "Invitado 2")
        {
            filteredRounds.append(round);
        }
    }

    // Set the number of rows
    ui->opponentsTableWidget->setRowCount(filteredRounds.size());

    // Fill in the data
    for (int i = 0; i < filteredRounds.size(); ++i) {
        Round *round = filteredRounds[i];

        // Opponent column
        Player* opponent = (round->getWinner()->getNickName() == playerName) ?
                               round->getLoser() :
                               round->getWinner();

        QWidget *cellWidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout(cellWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Opponent's avatar
        QLabel *avatarLabel = new QLabel(cellWidget);
        QPixmap avatarPixmap = QPixmap::fromImage(opponent->getAvatar());
        avatarLabel->setPixmap(avatarPixmap.scaled(40, 40,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
        avatarLabel->setFixedSize(40, 40);

        // Opponent's name
        QLabel *nicknameLabel = new QLabel(opponent->getNickName(), cellWidget);

        // Add widgets to the layout
        layout->addWidget(avatarLabel);
        layout->addWidget(nicknameLabel);
        ui->opponentsTableWidget->setCellWidget(i, 0, cellWidget);

        // Result column
        QString winnerName = round->getWinner()->getNickName();
        QString result = (winnerName == playerName) ? "VICTORIA" : "DERROTA";

        QTableWidgetItem *resultItem = new QTableWidgetItem(result);

        // Bold font configuration
        QFont boldFont;
        boldFont.setBold(true);

        // Change text color based on the result
        if (result == "VICTORIA") {
            resultItem->setBackground(Qt::darkGreen);
        } else {
            resultItem->setBackground(Qt::darkRed);
        }

        resultItem->setFont(boldFont);
        resultItem->setTextAlignment(Qt::AlignCenter);

        ui->opponentsTableWidget->setItem(i, 1, resultItem);

        // Date and Time column
        QDateTime roundTime = round->getTimestamp();
        QTableWidgetItem *dateTimeItem =
            new QTableWidgetItem(roundTime.toString("dd/MM/yyyy HH:mm"));
        dateTimeItem->setTextAlignment(Qt::AlignCenter);
        ui->opponentsTableWidget->setItem(i, 2, dateTimeItem);
    }

    // Free memory of the rounds
    qDeleteAll(rounds);

}   /* gameHistoryTable() */

/******************************************************************************/
/*!
 * @brief   Filters the game history table and updates the table based on the
 *          selected date range.
 * @param   void
 * @return  void
 */
void IndividualRankingWindow::updateTable(const QList<Round*>& filteredRounds)
{
    // Set the number of rows in the table
    ui->opponentsTableWidget->setRowCount(filteredRounds.size());

    // Fill in the data in the table
    for (int i = 0; i < filteredRounds.size(); ++i) {
        Round *round = filteredRounds[i];

        // Opponent column
        Player* opponent;

        if (round->getWinner()->getNickName() == playerName)
        {   opponent = round->getLoser();   }
        else
        {   opponent = round->getWinner();  }

        QWidget *cellWidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout(cellWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Opponent's avatar
        QLabel *avatarLabel = new QLabel(cellWidget);
        QPixmap avatarPixmap = QPixmap::fromImage(opponent->getAvatar());
        avatarLabel->setPixmap(avatarPixmap.scaled(40, 40,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
        avatarLabel->setFixedSize(40, 40);

        // Opponent's name
        QLabel *nicknameLabel = new QLabel(opponent->getNickName(), cellWidget);

        // Add widgets to the layout
        layout->addWidget(avatarLabel);
        layout->addWidget(nicknameLabel);
        ui->opponentsTableWidget->setCellWidget(i, 0, cellWidget);

        // Result column
        QString winnerName = round->getWinner()->getNickName();
        QString result;

        if (winnerName == playerName) { result = "VICTORIA"; }
        else { result = "DERROTA"; }

        QTableWidgetItem *resultItem = new QTableWidgetItem(result);
        ui->opponentsTableWidget->setItem(i, 1, resultItem);

        // Date and Time column
        QDateTime roundTime = round->getTimestamp();
        QTableWidgetItem *dateTimeItem =
            new QTableWidgetItem(roundTime.toString("dd/MM/yyyy HH:mm"));
        ui->opponentsTableWidget->setItem(i, 2, dateTimeItem);
    }

}   /* updateTable() */


/*** end of file ***/
