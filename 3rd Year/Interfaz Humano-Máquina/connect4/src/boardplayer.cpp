/**
 * @file     boardplayer.cpp
 *
 * @brief    Implementation of BoardPlayer class functions.
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

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QColor>

/// Own includes
#include "boardplayer.h"


//-----[ IMPLEMENTATION OF PUBLIC BOARDPLAYER FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief   BoardPlayer class constructor implementation.
 * @param   void
 */
BoardPlayer::BoardPlayer(void)
{
    // Empty on purpose

}   /* BoardPlayer() */

/******************************************************************************/
/*!
 * @brief   Implementation of the BoardPlayer class destructor.
 * @param   void
 */
BoardPlayer::~BoardPlayer(void)
{
    // Empty on purpose

}   /* ~BoardPlayer() */

/******************************************************************************/
/*!
 * @brief   Disc Color Management Functions.
 * @param   void
 * @return  Return the selected color
 */
QColor BoardPlayer::getCircleColor(void)
{
    QColor color;

    // Select the color based on the value of discColor
    switch (discColor)
    {
        case BLUE:
            color = Qt::blue;
            break;

        case PURPLE:
            // Assign the color purple using RGB values
            color = QColor(128, 0, 128);
            break;

        case GREEN:
            color = Qt::green;
            break;

        case RED:
            color = Qt::red;
            break;

        default:
            break;
    }

    return (color);

}   /* getCircleColor() */

/******************************************************************************/
/*!
 * @brief   Disc Color Management Functions.
 * @param   newDiscColor  The new color to be assigned to the disc.
 * @return  void
 */
void BoardPlayer::setCircleColor(circleColor_t newDiscColor)
{
    discColor = newDiscColor;

}   /* setCircleColor() */


/*** end of file ***/
