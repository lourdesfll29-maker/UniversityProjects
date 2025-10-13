/**
 * @file     rankingwindow.cpp
 *
 * @brief    Implementation of RankingWindow class functions.
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
#include <QSet>

/// Own includes
#include "rankingwindow.h"
#include "ui_rankingwindow.h"
#include "individualrankingwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC RANKINGWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief   RankingWindow class constructor implementation.
 * @param   parent  Used to set the parent widget of RankingWindow.
 */
RankingWindow::RankingWindow(QWidget *parent)
    : QWidget(parent)
    /// INITIALIZATION OF PRIVATE VARIABLES
    , ui(new Ui::RankingWindow)
    , mainwindow(qobject_cast<MainWindow *>(parentWidget()))
{
    // Set up the UI components for RankingWindow
    ui->setupUi(this);

    /// CONNECTING SIGNALS AND SLOTS

    connect(
        ui->a_backButton
        , &QPushButton::clicked
        , this
        , &RankingWindow::onBackPressed);

    connect(
        ui->searchButton
        , &QPushButton::clicked
        , this
        , &RankingWindow::filterRanking);

    connect(
        ui->rankingTable
        , &QTableWidget::cellClicked
        , this
        , &RankingWindow::onTableCellClicked);

    /// SET INITIAL CONFIGURATION OF THE INTERFACE

    // Initialize the table
    initializeTable();

    // Load the ranking
    showPodium();
    setTotalPlayers();
    setTotalGames();
    loadRanking();

}   /* RankingWindow() */

/******************************************************************************/
/*!
 * @brief   Implementation of the RankingWindow class destructor.
 * @param   void
 */
RankingWindow::~RankingWindow(void)
{
    // Delete the UI components associated with RankingWindow
    delete ui;

} /* RankingWindow() */


//-----[ IMPLEMENTATION OF PROTECTED RANKINGWINDOW FUNCTIONS ]---------------//


//-----[ IMPLEMENTATION OF PRIVATE SLOT RANKINGWINDOW FUNCTIONS ]------------//

/*****************************************************************************/
/*!
 * @brief   Handles the event when the "Back" button is pressed.
 * @param   void
 * @return  void
 */
void RankingWindow::onBackPressed(void)
{
    mainwindow->changeCentralWidget(nullptr, RECOVER_PREV_WINDOW);

}   /* onBackPressed() */

/*****************************************************************************/
/*!
 * @brief   Gets the QLineEdit text, filters the players in a period of time and
 *          updates the table with the results.
 * @param   void
 * @return  void
 */
void RankingWindow::filterRanking(void)
{
    // Get the text from the QLineEdit
    QString searchText = ui->searchLineEdit->text().trimmed();
    QList<Player*> filteredPlayers;

    // Get the complete ranking from Connect4
    Connect4 &c4 = Connect4::getInstance();
    QList<Player*> allPlayers = c4.getRanking();

    // Filter players whose nickname contains the entered text
    for (Player* player : allPlayers)
    {
        QString nickname = player->getNickName();

        if (nickname != "CPU" && nickname != "Invitado 1" &&
            nickname != "Invitado 2" &&
            nickname.contains(searchText,Qt::CaseInsensitive))
        {
            filteredPlayers.append(player);
        }
    }

    // Update the table with the filtered players
    updateTable(filteredPlayers);

    // Free up memory from unused players
    qDeleteAll(allPlayers);

}   /* filterRanking() */

/*****************************************************************************/
/*!
 * @brief   Handles the event when a table cell is clicked.
 * @param   row The row number of the clicked cell.
 * @return  void
 */
void RankingWindow::onTableCellClicked(int row)
{

    // Get the widget in the cell that combines avatar and nickname
    QWidget* cellWidget = ui->rankingTable->cellWidget(row, 0); // Column 0
    if (!cellWidget) {
        qDebug() << "No se encontró el widget en la fila:" << row;
        return;
    }

    // Get the layout of the cell
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(cellWidget->layout());
    if (!layout) {
        qDebug() << "No se encontró un QHBoxLayout en la celda de la fila:" << row;
        return;
    }

    // Check if the layout has at least two widgets
    if (layout->count() < 2) {
        qDebug() << "El layout no contiene suficientes widgets en la fila:" << row;
        return;
    }

    // Get the second QLabel (which contains the nickname)
    QLabel* nicknameLabel = qobject_cast<QLabel*>(layout->itemAt(1)->widget());
    if (!nicknameLabel) {
        qDebug() << "No se encontró un QLabel en la segunda posición del layout de la fila:" << row;
        return;
    }

    // Save the nickname in the public variable playerSelected
    if (mainwindow) {
        mainwindow->playerSelected = nicknameLabel->text();
    } else {
        qWarning() << "mainwindow es nulo.";
    }

    // Open another window
    onNextPressed();

}   /* onTableCellClicked() */

/*****************************************************************************/
/*!
 * @brief   Handles the event when the "Next" button is pressed.
 * @param   void
 * @return  void
 */
void RankingWindow::onNextPressed(void)
{
    QWidget * newWidget = new IndividualRankingWindow(
        mainwindow, mainwindow->playerSelected);

    mainwindow->changeCentralWidget(newWidget, SAVE_PREV_WIDGET);

}   /* onNextPressed() */


//-----[ IMPLEMENTATION OF PRIVATE RANKINGWINDOW FUNCTIONS ]------------------//

/******************************************************************************/
/*!
 * @brief   Initialize the ranking table.
 * @param   void
 * @return  void
 */
void RankingWindow::initializeTable(void)
{
    ui->rankingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rankingTable->setSelectionBehavior(QTableWidget::SelectRows);
    ui->rankingTable->setSelectionMode(QTableWidget::NoSelection);
    ui->rankingTable->setEditTriggers(QTableWidget::NoEditTriggers);

}   /* initializeTable() */

/******************************************************************************/
/*!
 * @brief   Load data into table.
 * @param   void
 * @return  void
 */
void RankingWindow::loadRanking(void)
{
    // Get the ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filter players excluding CPU, guest1, and guest2
    QList<Player*> filteredRanking;
    for (Player* player : ranking) {
        QString playerName = player->getNickName();
        if (playerName != "CPU" &&
            playerName != "Invitado 1" &&
            playerName != "Invitado 2")
        {
            filteredRanking.append(player);
        }
    }

    // Sort players by score in descending order
    std::sort(filteredRanking.begin(), filteredRanking.end(), [](Player* a,
                                                                 Player* b)
              {
                  // Descending order by points
                  return a->getPoints() > b->getPoints();
              });

    // Set the number of rows
    ui->rankingTable->setRowCount(filteredRanking.size());

    // Fill in the data
    for (int i = 0; i < filteredRanking.size(); ++i) {
        Player *player = filteredRanking[i];

        // Create a QWidget for the cell (Avatar + Nickname)
        QWidget *cellWidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout(cellWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Create QLabel for the avatar
        QLabel *avatarLabel = new QLabel(cellWidget);
        QPixmap avatarPixmap = QPixmap::fromImage(player->getAvatar());
        avatarLabel->setPixmap(avatarPixmap.scaled(40, 40,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
        avatarLabel->setFixedSize(40, 40);

        // Create QLabel for the nickname
        QLabel *nicknameLabel = new QLabel(player->getNickName(), cellWidget);

        // Add widgets to the layout
        layout->addWidget(avatarLabel);
        layout->addWidget(nicknameLabel);

        // Assign the combined widget to the cell
        ui->rankingTable->setCellWidget(i, 0, cellWidget);

        // Points column
        QTableWidgetItem *pointsItem = new QTableWidgetItem(QString::number(player->getPoints()));
        pointsItem->setTextAlignment(Qt::AlignCenter); // Center text

        ui->rankingTable->setItem(i, 1, pointsItem);
    }

    // Free memory of players
    qDeleteAll(filteredRanking);

}   /* loadRanking() */

/******************************************************************************/
/*!
 * @brief   Updates the ranking table with the given list of players.
 * @param   players  The list of players to display in the ranking table.
 * @return  void
 */
void RankingWindow::updateTable(const QList<Player *> & players)
{
    // Clear the current contents of the table
    ui->rankingTable->clearContents();
    // Adjust the number of rows
    ui->rankingTable->setRowCount(players.size());

    for (int i = 0; i < players.size(); ++i)
    {
        Player* player = players[i];

        // Create a QWidget for the cell that combines avatar and nickname
        QWidget* cellWidget = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(cellWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        QLabel* avatarLabel = new QLabel(cellWidget);
        QPixmap avatarPixmap = QPixmap::fromImage(player->getAvatar());
        avatarLabel->setPixmap(avatarPixmap.scaled(40, 40,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
        avatarLabel->setFixedSize(40, 40);

        QLabel* nicknameLabel = new QLabel(player->getNickName(), cellWidget);

        layout->addWidget(avatarLabel);
        layout->addWidget(nicknameLabel);

        ui->rankingTable->setCellWidget(i, 0, cellWidget);

        // Points column
        QTableWidgetItem* pointsItem =
            new QTableWidgetItem(QString::number(player->getPoints()));
        pointsItem->setTextAlignment(Qt::AlignCenter); // Center text

        ui->rankingTable->setItem(i, 1, pointsItem);
    }

}   /* updateTable() */

/******************************************************************************/
/*!
 * @brief   Sets the total number of players in the ranking.
 * @param   void
 * @return  void
 */
void RankingWindow::setTotalPlayers(void)
{
    // Get the ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filter players excluding CPU, guest1, and guest2
    QList<Player*> filteredRanking;
    for (Player* player : ranking) {
        QString playerName = player->getNickName();
        if (playerName != "CPU" && playerName != "Invitado 1" && playerName != "Invitado 2") {
            filteredRanking.append(player);
        }
    }

    ui->numberplayersLabel->setText(tr("<b>Total Number of Players:</b> ") +
                                    QString::number(filteredRanking.size()));

}   /* setTotalPlayers() */

/******************************************************************************/
/*!
 * @brief   Sets the total number of games played by all players.
 * @param   void
 * @return  void
 */
void RankingWindow::setTotalGames(void)
{
    // Get the ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filter players excluding CPU, guest1, and guest2
    QList<Player*> filteredRanking;
    for (Player* player : ranking) {
        QString playerName = player->getNickName();
        if (playerName != "CPU" && playerName != "Invitado 1" && playerName != "Invitado 2") {
            filteredRanking.append(player);
        }
    }

    // Use a QSet to store unique rounds
    QSet<QString> uniqueRounds;

    for (Player * player : filteredRanking)
    {
        QList<Round *> rounds = c4.getRoundsForPlayer(player);

        // Add the rounds to the set of unique rounds
        for (Round * round : rounds)
        {
            /*
            uniqueRounds.insert(round);
            */

            // Get the data
            Player * winner = round->getWinner();
            Player * loser  = round->getLoser();
            QDateTime date  = round->getTimestamp();
            QString formattedDate = date.toString("dd-MM-yyyy HH:mm:ss");

            // Create a unique key for the game
            QString gameKey = QString("%1-%2-%3").arg(winner->getNickName())
                                  .arg(loser->getNickName())
                                  .arg(formattedDate);

            // Check if the game has already been added
            if (!uniqueRounds.contains(gameKey))
            {
                // Add the key to the set
                uniqueRounds.insert(gameKey);
            }
        }
    }

    int totalRounds = uniqueRounds.size();

    ui->numbergamesLabel->setText(tr("<b>Total Number of Games:</b> ") +
                                  QString::number(totalRounds));

}   /* setTotalGames() */

/******************************************************************************/
/*!
 * @brief   Displays the top three players (podium) in the ranking table.
 * @param   void
 * @return  void
 */
void RankingWindow::showPodium(void)
{
    // Get the player ranking from Connect4
    Connect4 & c4 = Connect4::getInstance();
    QList<Player*> ranking = c4.getRanking();

    // Filter players excluding CPU, guest1, and guest2
    QList<Player*> filteredRanking;
    for (Player* player : ranking) {
        QString playerName = player->getNickName();
        if (playerName != "CPU" && playerName != "Invitado 1" && playerName != "Invitado 2") {
            filteredRanking.append(player);
        }
    }

    // Sort players by score in descending order
    std::sort(filteredRanking.begin(), filteredRanking.end(), [](Player* a, Player* b) {
        return a->getPoints() > b->getPoints();  // Descending order by points
    });

    // First place: Best player
    if (filteredRanking.size() > 0)
    {
        Player* firstPlayer = filteredRanking[0];
        QPixmap firstAvatar = QPixmap::fromImage(firstPlayer->getAvatar());
        ui->firstPlaceAvatar->setPixmap(firstAvatar.scaled(70, 70,
                                                           Qt::KeepAspectRatio,
                                                           Qt::SmoothTransformation));
        ui->firstPlaceName->setText(firstPlayer->getNickName());
    }
    else
    {
        ui->firstPlaceAvatar->clear();
        ui->firstPlaceName->setText(tr("No player"));
    }

    // Second place: Second best player
    if (filteredRanking.size() > 1)
    {
        Player* secondPlayer = filteredRanking[1];
        QPixmap secondAvatar = QPixmap::fromImage(secondPlayer->getAvatar());
        ui->secondPlaceAvatar->setPixmap(secondAvatar.scaled(70, 70,
                                                             Qt::KeepAspectRatio,
                                                             Qt::SmoothTransformation));
        ui->secondPlaceName->setText(secondPlayer->getNickName());
    } else {
        ui->secondPlaceAvatar->clear();
        ui->secondPlaceName->setText(tr("No player"));
    }

    // Third place: Third best player
    if (filteredRanking.size() > 2) {
        Player* thirdPlayer = filteredRanking[2];
        QPixmap thirdAvatar = QPixmap::fromImage(thirdPlayer->getAvatar());
        ui->thirdPlaceAvatar->setPixmap(thirdAvatar.scaled(70, 70,
                                                           Qt::KeepAspectRatio,
                                                           Qt::SmoothTransformation));
        ui->thirdPlaceName->setText(thirdPlayer->getNickName());
    } else {
        ui->thirdPlaceAvatar->clear();
        ui->thirdPlaceName->setText(tr("No player"));
    }

    // Free memory of the player list
    qDeleteAll(filteredRanking);

}   /* showPodium() */


/*** end of file ***/
