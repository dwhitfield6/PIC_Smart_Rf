/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 02/05/15     1.0         Created log.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* EEPROM Data Addresses                                                      */
/******************************************************************************/

#define EE_BaudMSB          0
#define EE_BaudMHSB         1
#define EE_BaudMLSB         2
#define EE_BaudLSB          3
#define EE_Parity           4
#define EE_ShutDown_Time    5

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define PASS 1
#define FAIL 0
/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

unsigned int ReadEEPROM_1Byte(unsigned int address);
void EEPROM_UNLOCK(void);
void WriteEEPROM_1Byte(unsigned int address, unsigned char data);
unsigned long GetMemoryBaud(void);
unsigned char SetMemoryBaud(unsigned long Baud);
unsigned char GetMemoryParity(void);
unsigned char SetMemoryParity(unsigned char Parity);