/**
 * @file     rankingwindow.h
 *
 * @brief    Definition of RankingWindow class functions.
 *
 * @version  0.3  (2024/01/25) Final version
 * @version  0.2  (2024/01/24) Almost final version
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

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef RANKINGWINDOW_H
#define RANKINGWINDOW_H

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QDateTime>

/// Own includes
#include "mainwindow.h"
#include "connect4.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class RankingWindow; }
QT_END_NAMESPACE


//-----[ LOGWINDOW CLASS ]----------------------------------------------------//

class RankingWindow : public QWidget
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        RankingWindow(QWidget *parent = nullptr);
        ~RankingWindow(void);

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
         |     DECLARATION OF PRIVATE SLOTS FUNCTIONS          |
         +-----------------------------------------------------*/

        void onBackPressed(void);
        void filterRanking(void);
        void onTableCellClicked(int row);
        void onNextPressed(void);

    private:

        /*----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS   |
         +----------------------------------------------------*/

        /// Variables

        Ui::RankingWindow * ui;
        MainWindow * mainwindow;

        /// Functions

        void initializeTable(void);
        void initializeDateEdits(void);

        void loadRanking(void);

        void updateTable(const QList<Player*>& players);

        void setTotalPlayers(void);
        void setTotalGames(void);
        void showPodium(void);
};

#endif /* RANKINGWINDOW_H */

/*** end of file ***/
