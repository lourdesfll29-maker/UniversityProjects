/**
 * @file     main.cpp
 *
 * @brief    Implementation of main function for the Qt framework application.
 *
 * @version  0.3  (2024/01/24) Final version
 * @version  0.2  (2024/12/16) Applying BARR-C programming standard
 * @version  0.1  (2024/12/16) Teacher's example project
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
#include <QApplication>

/// Own includes
#include "mainwindow.h"


//-----[ MAIN FUNCTION ]------------------------------------------------------//

/******************************************************************************/
/*!
 * @brief   This function is the entry point for the Qt framework application.
 * @param   argc  Number of arguments passed to the application.
 * @param   argv  Contains the arguments passed to the application.
 * @return  When the event loop terminates, exec() returns an exit code.
 */
int main(int argc, char * argv[])
{
    // Create an instance of QApplication to manage the GUI application
    QApplication a(argc, argv);

    freopen("NUL", "w", stderr); // Redirección de los mensajes de error (eliminar en caso de fallos)

    // Create an instance of MainWindow, representing the main window
    MainWindow w;

    // Show the main window
    w.show();

    // Start the application's event loop
    return (a.exec());

}   /* main() */


/*** end of file ***/
