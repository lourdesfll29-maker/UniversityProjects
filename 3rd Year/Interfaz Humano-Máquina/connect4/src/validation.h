/**
 * @file     validation.h
 *
 * @brief    Definition of Validation structs and functions.
 *
 * @version  0.2  (2024/01/24) Final version
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

#ifndef VALIDATION_H
#define VALIDATION_H


//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QString>

/// Own includes


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//

typedef enum
{
    IN_FOCUS,
    NOT_IN_FOCUS,
    CORRECT_INPUT,
    WRONG_INPUT

}   frameState_t;

typedef enum
{
    EMPTY,
    IS_VALID,
    NOT_VALID,
    TOO_SHORT,
    TOO_LONG

}   lineCheck_t;


//-----[ VALIDATION FUNCTIONS ]-----------------------------------------------//

lineCheck_t isValidNickName(const QString & nickName);
bool isValidEmail(const QString & email);
bool isValidSecurityCode(const QString & securityCode);
lineCheck_t isValidPassword(const QString & password);
int generateRandomCode(void);
bool isLeap(int year);
bool isOlderThan16Years(int day, int month, int year);

#endif /* VALIDATION_H */

/*** end of file ***/
