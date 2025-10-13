/**
 * @file     rankingwindow.h
 *
 * @brief    Definition of RankingWindow class functions.
 *
 * @version  0.5  (2024/01/25) Final version
 * @version  0.4  (2025/01/18) Almost final version
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

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

#include <QWidget>
#include <QDateTime>
#include <QDate>

#include "mainwindow.h"
#include "connect4.h"


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class HistoryWindow; }
QT_END_NAMESPACE


//-----[ LOGWINDOW CLASS ]----------------------------------------------------//

class HistoryWindow : public QWidget
{
    Q_OBJECT

    public:

        /*----------------------------------------------------+
        |     DECLARATION OF PUBLIC FUNCTIONS                 |
        +-----------------------------------------------------*/

        HistoryWindow(QWidget *parent = nullptr);
        ~HistoryWindow(void);

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

        void filterTablewithDate(void);

        void onBackPressed(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES & FUNCTIONS    |
         +-----------------------------------------------------*/

        /// Variables

        Ui::HistoryWindow *ui;
        MainWindow *mainwindow;

        /// Functions

        void initializeTable(void);
        void initializeDateEdits(void);
        void gameHistoryTable(void);

        void showDailyRoundsChart(const QList<Round*>& rounds);

        void showGamesGraphic(const QList<Round*>& filteredRounds);

        void updateTable(const QDate& startDate, const QDate& endDate);
        void updateGraphic(const QDate& startDate, const QDate& endDate);
};

#endif // HISTORYWINDOW_H

/*** end of file ***/
