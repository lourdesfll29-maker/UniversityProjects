/**
 * @file     boardplayer.h
 *
 * @brief    Definition of BoardPlayer class functions.
 *           A player class containing their name and disc color.
 *
 * @version  0.2  (2024/01/24) Final version
 * @version  0.1  (2024/01/23) BARR-C programming standard needs to be revised
 *
 * @authors  Marcos Belda Martinez' <mbelmar@etsinf.upv.es>,
 *           Angela Espert Cornejo' <aespcor@etsinf.upv.es>,
 *           Lourdes Frances Llimera' <lfralli@epsa.upv.es>
 *
 * @date     January, 2025
 * @section  IHM-GIIROB
 */

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef BOARDPLAYER_H
#define BOARDPLAYER_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QColor>

/// Own includes


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//

// Defining the disc color
typedef enum
{
    BLUE,
    PURPLE,
    GREEN,
    RED

}   circleColor_t;


//-----[ BOARDPLAYER CLASS ]--------------------------------------------------//

class BoardPlayer
{
    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        BoardPlayer(void);
        ~BoardPlayer(void);

        QColor getCircleColor(void);
        void setCircleColor(circleColor_t newColorFicha);

    private:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE VARIABLES                |
         +-----------------------------------------------------*/

        QString playerNickName;
        circleColor_t discColor;
};

#endif /* BOARDPLAYER_H */

/*** end of file ***/
