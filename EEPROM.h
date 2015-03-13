/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 02/05/15     1.0         Created log.
 * 03/12/15     1.0_DW0a    Added New functons prototypes.
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
/* Structures                                                                 */
/******************************************************************************/
typedef struct EEdata
{
    unsigned long Baud;
    unsigned char Parity;
    unsigned long NEC1;
}GBLdata;

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

GBLdata Global =0;

/******************************************************************************/
/* EEPROM Data Addresses                                                      */
/******************************************************************************/
/* Long Baud */
#define EE_BaudMSB          0
#define EE_BaudMHSB         1
#define EE_BaudMLSB         2
#define EE_BaudLSB          3

/* char parity */
#define EE_Parity           4

/* Long NEC1 */
#define EE_NEC1_byte1       5
#define EE_NEC1_byte2       6
#define EE_NEC1_byte3       7
#define EE_NEC1_byte4       8

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
GBLdata GetEEPROM(void);
unsigned long SetEEPROM(GBLdata Temp,unsigned long burn);
unsigned long GetMemoryLong(unsigned char AddressFirst);
unsigned char SetMemoryLong(unsigned long Data, unsigned char AddressFirst);
unsigned long GetMemoryInt(unsigned char AddressFirst);
unsigned char SetMemoryInt(unsigned int Data, unsigned char AddressFirst);
unsigned long GetMemoryChar(unsigned char AddressFirst);
unsigned char SetMemoryChar(unsigned char Data, unsigned char AddressFirst);
void SyncEEPROMandGLOBAL(void);
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/