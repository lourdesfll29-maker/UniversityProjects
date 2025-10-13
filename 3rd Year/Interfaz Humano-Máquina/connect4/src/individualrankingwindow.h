/**
 * @file     individualrankingwindow.h
 *
 * @brief    Definition of RankingWindow class functions.
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

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef INDIVIDUALRANKINGWINDOW_H
#define INDIVIDUALRANKINGWINDOW_H

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QWidget>
#include <QDateTime>
#include <QDate>

/// Own includes
#include "mainwindow.h"
#include "connect4.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class IndividualRankingWindow; }
QT_END_NAMESPACE


//-----[ INDIVIDUALRANKINGWINDOW CLASS ]--------------------------------------//

class IndividualRankingWindow : public QWidget
{
    Q_OBJECT

    public:

        /*----------------------------------------------------+
        |     DECLARATION OF PUBLIC FUNCTIONS                 |
        +-----------------------------------------------------*/

        IndividualRankingWindow(QWidget * parent = nullptr,
                                QString playerName = nullptr);

        ~IndividualRankingWindow(void);

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

        void filteredTableandGraphics(const QDate & date);
        void onBackPressed(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS    |
         +-----------------------------------------------------*/

        /// Variables

        Ui::IndividualRankingWindow * ui;
        MainWindow * mainwindow;
        QString playerName;

        /// Functions

        void initializeTable(void);
        void initializeDateEdits(void);
        void setNickNameandAvatarandPoints(void);
        void setBestPoints(void);

        void showResultsBarChart(const QList<Round *> & filteredRounds);
        void showOpponentBarChart(const QList<Round *> & filteredRounds);

        void gameHistoryTable(void);

        void updateTable(const QList<Round *> & filteredRounds);
};

#endif /* INDIVIDUALRANKINGWINDOW_H */

/*** end of file ***/
