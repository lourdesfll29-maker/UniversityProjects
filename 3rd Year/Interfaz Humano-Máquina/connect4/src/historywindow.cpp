/**
 * @file     HistoryWindow.cpp
 *
 * @brief    Implementation of RankingWindow class functions.
 *
 * @version  0.5  (2024/01/25) Final version
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
#include <QtCharts>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QHash>
#include <QSet>
#include <QPen>
#include <QBrush>

/// Own includes
#include "historywindow.h"
#include "ui_historywindow.h"


//-----[ IMPLEMENTATION OF PUBLIC HISTORYWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   HistoryWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of HistoryWindow.
 */
HistoryWindow::HistoryWindow(QWidget *parent)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::HistoryWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    // Set up the UI components for RankingWindow
    ui->setupUi(this);

    /// CONNECTING SIGNALS AND SLOTS

    connect(ui->a_backButton
            , &QPushButton::clicked
            , this
            , &HistoryWindow::onBackPressed);

    connect(ui->startDate
            , &QDateEdit::dateChanged
            , this
            , &HistoryWindow::filterTablewithDate);

    connect(ui->endDate
            , &QDateEdit::dateChanged
            , this
            , &HistoryWindow::filterTablewithDate);

    /// SET INITIAL CONFIGURATION OF THE INTERFACE

    initializeTable();
    initializeDateEdits();

    gameHistoryTable();

}   /* HistoryWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the HistoryWindow class destructor.
 * @param   void
 */
HistoryWindow::~HistoryWindow(void)
{
    // Delete the UI components associated with HistoryWindow
    delete ui;

}   /* ~HistoryWindow() */


//-----[ IMPLEMENTATION OF PROTECTED HISTORYWINDOW FUNCTIONS ]---------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT HISTORYWINDOW FUNCTIONS ]------------//

/*****************************************************************************/
/*!
 * @brief   Handles the event when the "Back" button is pressed.
 * @param   void
 * @return  void
 */
void HistoryWindow::onBackPressed(void)
{
    mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WIDGET);

}   /* onBackPressed() */

/*****************************************************************************/
    /*!
 * @brief   Filters the game history table and updates the graphics based on
 *          the selected date range.
 * @param   void
 * @return  void
 */
    void HistoryWindow::filterTablewithDate(void)
{
    QDate startDate = ui->startDate->date();
    QDate endDate = ui->endDate->date();

    updateTable(startDate, endDate);
    updateGraphic(startDate, endDate);

}   /* filterTablewithDate() */


//-----[ IMPLEMENTATION OF PRIVATE HISTORYWINDOW FUNCTIONS ]------------------//

/******************************************************************************/
/*!
 * @brief   Initialize the history table.
 * @param   void
 * @return  void
 */
void HistoryWindow::initializeTable(void)
{
    ui->gamesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gamesTableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->gamesTableWidget->setSelectionMode(QTableWidget::NoSelection);
    ui->gamesTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

}   /* initializeTable() */

/******************************************************************************/
/*!
 * @brief   Initialize the date edits for the history window.
 * @param   void
 * @return  void
 */
void HistoryWindow::initializeDateEdits(void)
{
    // Get the instance of Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> allPlayers = c4.getRanking();

    // Get the rounds for all players
    QList<Round*> rounds;

    // Iterate through each player and get their rounds
    for (Player* player : allPlayers)
    {
        QList<Round*> playerRounds = c4.getRoundsForPlayer(player);
        // Add the player's rounds to the total list
        rounds.append(playerRounds);
    }

    // Determine the earliest recorded date
    QDate earliestDate = QDate::currentDate();

    for (Round* round : rounds)
    {
        QDate roundDate = round->getTimestamp().date();

        if (roundDate < earliestDate)
        {
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
 * @brief   Populates the game history table with the filtered game data.
 * @param   void
 * @return  void
 */
void HistoryWindow::gameHistoryTable(void)
{
    // Get the ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filter players excluding CPU, guest1, and guest2
    QList<Round*> filteredRoundRanking;

    for (Player* player : ranking) {

        QString playerName = player->getNickName();
        if (playerName != "CPU" &&
            playerName != "Invitado 1" &&
            playerName != "Invitado 2")
        {
            QList<Round*> rounds = c4.getRoundsForPlayer(player);
            filteredRoundRanking.append(rounds);
        }
    }

    // Sort the rounds by date and time (most recent to oldest)
    std::sort(filteredRoundRanking.begin(), filteredRoundRanking.end(), [](Round* a,
                                                                           Round* b) {
        return a->getTimestamp() > b->getTimestamp();
    });

    // Use a QSet to avoid duplicates
    QSet<QString> uniqueGames;

    // Clear the table
    ui->gamesTableWidget->setRowCount(0);

    int row = 0; // Row counter

    // Fill in the data
    for (Round *round : filteredRoundRanking)
    {
        // Get the data
        Player *winner = round->getWinner();
        Player *loser = round->getLoser();
        QDateTime date = round->getTimestamp();
        QString formattedDate = date.toString("dd-MM-yyyy HH:mm:ss");

        // Create a unique key for the game
        QString gameKey = QString("%1-%2-%3").arg(winner->getNickName())
                                             .arg(loser->getNickName())
                                             .arg(formattedDate);

        // Check if the game has already been added
        if (!uniqueGames.contains(gameKey))
        {
            // Add the key to the set
            uniqueGames.insert(gameKey);

            // Add a new row to the table
            ui->gamesTableWidget->insertRow(row);

            // Fill in the cells
            QTableWidgetItem* winnerItem =
                new QTableWidgetItem(winner->getNickName());
            winnerItem->setTextAlignment(Qt::AlignCenter);
            ui->gamesTableWidget->setItem(row, 0, winnerItem);

            QTableWidgetItem* loserItem =
                new QTableWidgetItem(loser->getNickName());
            loserItem->setTextAlignment(Qt::AlignCenter);
            ui->gamesTableWidget->setItem(row, 1, loserItem);

            QTableWidgetItem* dateItem = new QTableWidgetItem(formattedDate);
            dateItem->setTextAlignment(Qt::AlignCenter);
            ui->gamesTableWidget->setItem(row, 2, dateItem);

            // Increment the row counter
            row++;
        }
    }

    // Call showDailyRoundsChart to update the chart with the unique list
    QList<Round*> uniqueFilteredRounds = filteredRoundRanking;
    showDailyRoundsChart(uniqueFilteredRounds);

}   /* gameHistoryTable() */

/******************************************************************************/
/*!
 * @brief   Updates the game history table based on the selected date range.
 * @param   startDate  The start date of the range.
 * @param   endDate    The end date of the range.
 * @return  void
 */
void HistoryWindow::updateTable(const QDate& startDate, const QDate& endDate)
{
    // Get the ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filtrar jugadores excluyendo cpu, guest1 y guest2
    QList<Round*> filteredRoundRanking;

    for (Player* player : ranking)
    {
        QString playerName = player->getNickName();

        if (playerName != "CPU" &&
            playerName != "Invitado 1" &&
            playerName != "Invitado 2")
        {
            QList<Round*> rounds = c4.getRoundsForPlayer(player);
            filteredRoundRanking.append(rounds);
        }
    }

    // Filter the rounds by the date range
    QList<Round*> filteredRounds;

    for (Round* round : filteredRoundRanking)
    {
        QDateTime date = round->getTimestamp();

        if (date.date() >= startDate && date.date() <= endDate)
        {
            filteredRounds.append(round);
        }
    }

    // Sort the rounds by date and time (most recent to oldest)
    std::sort(filteredRounds.begin(), filteredRounds.end(), [](Round* a,
                                                               Round* b) {
        return a->getTimestamp() > b->getTimestamp();
    });

    // Use a QSet to avoid duplicates
    QSet<QString> uniqueGames;

    // Set the number of rows
    ui->gamesTableWidget->setRowCount(0);

    // Fill in the data
    for (Round *round : filteredRounds)
    {
        // Get the data
        Player *winner = round->getWinner();
        Player *loser = round->getLoser();
        QDateTime date = round->getTimestamp();
        QString formattedDate = date.toString("dd-MM-yyyy HH:mm:ss");

        // Create a unique key for the game
        QString gameKey = QString("%1-%2-%3").arg(winner->getNickName())
                                             .arg(loser->getNickName())
                                             .arg(formattedDate);

        // Check if the game has already been added
        if (!uniqueGames.contains(gameKey))
        {
            // Add the key to the set
            uniqueGames.insert(gameKey);

            // Add a new row to the table
            int row = ui->gamesTableWidget->rowCount();
            ui->gamesTableWidget->insertRow(row);

            // Fill in the cells
            QTableWidgetItem* winnerItem =
                new QTableWidgetItem(winner->getNickName());
            winnerItem->setTextAlignment(Qt::AlignCenter);
            ui->gamesTableWidget->setItem(row, 0, winnerItem);

            QTableWidgetItem* loserItem =
                new QTableWidgetItem(loser->getNickName());
            loserItem->setTextAlignment(Qt::AlignCenter);
            ui->gamesTableWidget->setItem(row, 1, loserItem);

            QTableWidgetItem* dateItem = new QTableWidgetItem(formattedDate);
            dateItem->setTextAlignment(Qt::AlignCenter);
            ui->gamesTableWidget->setItem(row, 2, dateItem);
        }
    }

}   /* updateTable() */

/******************************************************************************/
/*!
 * @brief   Displays a line chart of the number of rounds played per day.
 * @param   rounds  The list of rounds to display in the chart.
 * @return  void
 */
void HistoryWindow::showDailyRoundsChart(const QList<Round*>& rounds)
{
    // Get the GraphicsView from the UI
    QGraphicsView* graphicsView = ui->gamesGraphicsView;
    QGraphicsScene* scene = new QGraphicsScene(this);

    // Constants for the graph layout
    int barWidth = 40;
    int maxHeight = 200;
    int xOffset = 50;
    int yOffset = 20;
    int spacing = 10;

    int index = 0;
    int maxGames = 0;

    // Count games per day
    QMap<QDate, int> gamesPerDay;
    QSet<QString> uniqueGames;

    if (!rounds.isEmpty())
    {
        for (Round* round : rounds) {
            QDate date = round->getTimestamp().date();
            QString gameKey = QString("%1-%2-%3").arg(round->getWinner()->getNickName())
                                  .arg(round->getLoser()->getNickName())
                                  .arg(date.toString("yyyy-MM-dd"));

            if (!uniqueGames.contains(gameKey)) {
                uniqueGames.insert(gameKey);
                gamesPerDay[date]++;
            }
        }

        // Sort the dates
        QList<QDate> sortedDates = gamesPerDay.keys();
        std::sort(sortedDates.begin(), sortedDates.end());


        // Find the maximum number of games in a day
        for (int count : gamesPerDay.values()) {
            if (count > maxGames) {
                maxGames = count;
            }
        }

        // Avoid division by zero
        if (maxGames == 0) maxGames = 1;

        // Draw the bars
        for (const QDate& date : sortedDates) {
            int gameCount = gamesPerDay[date];

            // Scale the height of the bar
            int barHeight = (gameCount * maxHeight) / maxGames;

            // Draw the bar
            QRectF barRect(xOffset + index * (barWidth + spacing),
                           maxHeight - barHeight + yOffset,
                           barWidth,
                           barHeight);
            scene->addRect(barRect, QPen(Qt::black), QBrush(Qt::blue));

            // Add the date label below the bar
            QGraphicsTextItem* dateLabel = scene->addText(date.toString("dd-MM"));
            dateLabel->setPos(xOffset + index * (barWidth + spacing) + barWidth / 4,
                              maxHeight + yOffset + 10);

            // Increment index for the next bar
            index++;
        }

        // Draw the vertical axis
        scene->addLine(xOffset - 10
                       , yOffset
                       , xOffset - 10
                       , maxHeight + yOffset
                       , QPen(Qt::black));

        // Add labels to the vertical axis
        int divisions = 5;

        if (maxGames > 5)
        {
            for (int i = 0; i <= divisions; ++i) {
                int value = (maxGames * i) / divisions;
                int y = maxHeight - (i * maxHeight / divisions) + yOffset;

                // Draw the tick mark
                scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

                // Add the label
                QGraphicsTextItem* label = scene->addText(QString::number(value));
                label->setPos(xOffset - 40, y - 10);
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

            for (int i = 0; i <= maxGames; ++i)
            {
                int y = maxHeight - (i * maxHeight / maxGames) + yOffset;

                scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

                QGraphicsTextItem *numberLabel = scene->addText(QString::number(i));
                numberLabel->setPos(xOffset - 40, y - 10);
            }
        }

        // Draw the horizontal axis
        scene->addLine(xOffset - 10, maxHeight + yOffset,
                       xOffset + index * (barWidth + spacing) - spacing,
                       maxHeight + yOffset, QPen(Qt::black));

        // Set the scene to the graphics view
        graphicsView->setScene(scene);
     }
    else
    {
        index = 5;
        maxGames = 5;

        // Draw the vertical axis
        scene->addLine(xOffset - 10, yOffset, xOffset - 10, maxHeight + yOffset, QPen(Qt::black));

        // Add labels to the vertical axis
        int divisions = 5;
        for (int i = 0; i <= divisions; ++i) {
            int value = (maxGames * i) / divisions;
            int y = maxHeight - (i * maxHeight / divisions) + yOffset;

            // Draw the tick mark
            scene->addLine(xOffset - 15, y, xOffset - 5, y, QPen(Qt::black));

            // Add the label
            QGraphicsTextItem* label = scene->addText(QString::number(value));
            label->setPos(xOffset - 40, y - 10);
        }

        // Draw the horizontal axis
        scene->addLine(xOffset - 10, maxHeight + yOffset,
                       xOffset + index * (barWidth + spacing) - spacing,
                       maxHeight + yOffset, QPen(Qt::black));

        // Set the scene to the graphics view
        graphicsView->setScene(scene);
    }

    // Adjust the view to fit the scene
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setSceneRect(scene->itemsBoundingRect());
    graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    // Disable scrollbars
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}   /* showDailyRoundsChart() */

/******************************************************************************/
/*!
 * @brief   Updates the game history graphics based on the selected date range.
 * @param   startDate  The start date of the range.
 * @param   endDate    The end date of the range.
 * @return  void
 */
void HistoryWindow::updateGraphic(const QDate& startDate, const QDate& endDate)
{
    // Get the ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filter players excluding CPU, guest1, and guest2
    QList<Round*> filteredRoundRanking;

    for (Player* player : ranking)
    {
        QString playerName = player->getNickName();

        if (playerName != "CPU" &&
            playerName != "Invitado 1" &&
            playerName != "Invitado 2")
        {
            QList<Round*> rounds = c4.getRoundsForPlayer(player);
            filteredRoundRanking.append(rounds);
        }
    }

    // Filter the rounds by the date range
    QList<Round*> filteredRounds;

    for (Round* round : filteredRoundRanking)
    {
        QDateTime date = round->getTimestamp();

        if (date.date() >= startDate && date.date() <= endDate)
        {
            filteredRounds.append(round);
        }
    }

    // Sort the rounds by date and time (most recent to oldest)
    std::sort(filteredRounds.begin(), filteredRounds.end(), [](Round* a,
                                                               Round* b) {
        return a->getTimestamp() > b->getTimestamp();
    });

    // Create the chart
    showDailyRoundsChart(filteredRounds);

}   /* updateGraphic() */


/*** end of file ***/
