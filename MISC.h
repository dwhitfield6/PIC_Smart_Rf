/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/21/15     1.0         Created log.
 * 02/05/15     1.0_DW0     Added funtcion ISNUMBER.
 *                          Added function IsPrintableASCII.
 *                          Added macro for TRUE and FALSE.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "system.h"

/******************************************************************************/
/* Delay constant
 *
 * This parameter is set to calibrate the delayUS() function. Parameter found
 *   from guess and check.
/******************************************************************************/

#define delayConst 31
//inverse relationship

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define SYS_FREQ_US 41
#define Even 2
#define Odd 3

#define TRUE 1
#define FALSE 0
#define NOEQUAL 2147483647L
#define DOUBLEEQUAL 2147483646L
#define NOVALUE 2147483645L
#define TOOBIG 2147483644L

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void delayUS(long US);
void cleanBuffer(unsigned char* data, int count);
void BufferCopy(unsigned char* from, unsigned char* to, char count, char shift);
unsigned char StringMatch(unsigned char* This, const unsigned char* That);
unsigned char StringContains(unsigned char* This, const unsigned char* That);
unsigned char StartsWith(unsigned char* This, const unsigned char* That);
long GetEnteredNumber(unsigned char* This);
void RemoveSpaces(unsigned char* This);
void lowercase(unsigned char* Input);
unsigned char StringAddEqual(unsigned char* Input);
unsigned char CheckSum_byte(unsigned int This, unsigned char Odd_Even);
unsigned char ISNUMBER(unsigned char ascii);
unsigned char IsPrintableASCII(unsigned char data);
