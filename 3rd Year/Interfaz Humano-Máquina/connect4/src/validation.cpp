/**
 * @file     validation.cpp
 *
 * @brief    Implementation of Validation functions.
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

//-----[ INCLUDES ]-----------------------------------------------------------//

/// External includes
#include <QRegularExpression>
#include <QDate>
#include "random"

/// Own includes
#include "validation.h"


//-----[ VALIDATION FUNCTIONS ]-----------------------------------------------//

/******************************************************************************/
/*!
 * @brief   This function checks if the provided nickname is valid based on
 *          specific criteria.
 * @param   nickName  The nickname string to be validated.
 * @return  lineCheck_t value based on the validity of the nickname.
 */
lineCheck_t isValidNickName(const QString & nickName)
{
    lineCheck_t answer;

    // Define the regex pattern for a valid nickname
    static QRegularExpression nickNamePattern("^[a-zA-Z0-9._-]+$");

    // Check if the nickname is empty
    if (nickName.isEmpty())
    {
        answer = EMPTY;
    }
    // Check the length of the nickname
    else if (nickName.length() < 3)
    {
        answer = TOO_SHORT;
    }
    else if (nickName.length() > 16)
    {
        answer = TOO_LONG;
    }
    // Check if the nickname matches the regex pattern
    else if (nickNamePattern.match(nickName).hasMatch())
    {
        answer = IS_VALID;
    }
    else
    {
        answer = NOT_VALID;
    }

    return (answer);

}   /* isValidNickName() */

/******************************************************************************/
/*!
 * @brief   Checks if a person is older than 16 years based on their birth date.
 * @param   day    The day of birth (1-31).
 * @param   month  The month of birth (1-12).
 * @param   year   The year of birth (e.g., 2007).
 * @return  True if the person is 16 years old or older; false otherwise.
 */
bool isOlderThan16Years(int day, int month, int year)
{
    // Get the current date
    QDate currentDate = QDate::currentDate();

    // Create the birth date from the parameters
    QDate birthDate(year, month, day);

    // Calculate the age
    int age = currentDate.year() - birthDate.year();

    // Adjust the age if the birthday has not occurred yet this year
    if (currentDate < birthDate.addYears(age))
    {
        age--;
    }

    // Return true if the age is 16 or older
    return (age >= 16);

}   /* isOlderThan16Years() */

/******************************************************************************/
/*!
 * @brief   This function checks if the provided email address is valid based
 *          on a regex pattern.
 * @param   email  The email string to be validated.
 * @return  True if the email address is valid, false otherwise.
 */
bool isValidEmail(const QString & email)
{
    // Define the regex pattern for a valid email address
    static QRegularExpression emailPattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");

    // Check if the email matches the regex pattern
    return (emailPattern.match(email).hasMatch());

}   /* isValidEmail() */

/******************************************************************************/
/*!
 * @brief   This function checks if the provided security code is valid.
 * @param   securityCode  The securityCode string to be validated, which must be
 *                        exactly 6 digits.
 * @return  True if the security code is valid (6 digits), false otherwise.
 */
bool isValidSecurityCode(const QString & securityCode)
{
    bool answer = false;

    if (securityCode.length() == 6)
    {
        // Define the regex pattern for a valid email address
        static QRegularExpression securityCodePattern("^[0-9]+$");

        // Check if the email matches the regex pattern
        answer = (securityCodePattern.match(securityCode).hasMatch());
    }

    return (answer);

}   /* isValidSecurityCode() */

/******************************************************************************/
/*!
 * @brief   This function checks if the provided password is valid based on
 *          specific criteria.
 * @param   password  The password string to be validated.
 * @return  A value of type passwordCheck_t indicating the validity of the
 *          password.
 */
lineCheck_t isValidPassword(const QString & password)
{
    lineCheck_t answer = IS_VALID;

    if (password.isEmpty())
    {
        answer = EMPTY;
    }
    else if (password.length() < 8)
    {
        answer = TOO_SHORT;
    }
    else if (password.length() > 20)
    {
        answer = TOO_LONG;
    }
    else
    {
        // Regular expression to check for at least one uppercase letter
        static QRegularExpression upperCase("[A-Z]");
        // Regular expression to check for at least one number
        static QRegularExpression number("[0-9]");
        // Regular expression to check for at least one special character
        static QRegularExpression specialCharacter("[!@#$%&*()\\-=+]");
        // Checks that there are no spaces
        static QRegularExpression space("\\s");

        if (!(password.contains(upperCase))        ||
            !(password.contains(number))           ||
            !(password.contains(specialCharacter)) ||
            (password.contains(space)))
        {
            answer = NOT_VALID;
        }
    }

    return (answer);

}   /* isValidPassword() */

/******************************************************************************/
/*!
 * @brief   Function to generate a 6-digit random number.
 * @param   void
 * @return  void
 */
int generateRandomCode(void)
{
    std::random_device rd; // Gets a random number from the hardware
    std::mt19937 gen(rd()); // Initializes the random number generator
    std::uniform_int_distribution<> dis(100000, 999999); // 6-digit range

    return (dis(gen)); // Generates and returns a random number

}   /* generateRandomCode() */

/******************************************************************************/
/*!
 * @brief   Check if a year is a leap year.
 * @param   year  The year to check.
 * @return  True if the year is a leap year, false otherwise.
 */
bool isLeap(int year)
{
    bool answer = false;

    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
            {
                answer = true; // Leap year
            }
            else
            {
                // Not a leap year
            }
        }
        else
        {
            answer = true; // Leap year
        }
    }

    return (answer);

}   /* isLeap() */


/*** end of file ***/
