/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/21/15     1.0         Created log.
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

/******************************************************************************/
/* Timing for bits of RF config 2. This is for the Westek device.                                                                 */
/******************************************************************************/

#define ONEshortCONF2 705
#define ONElongCONF2 1955
#define ZEROshortCONF2 705
#define ZEROlongCONF2 1910
#define SYNClongCONF2 11700
#define SYNCshortCONF2 705

/******************************************************************************/
/* Timing for bits of RF config 1. This is for the standalone black receiver
 *   made by amertac.                                                                */
/******************************************************************************/

#define ONEshortCONF1 216
#define ONElongCONF1 614
#define ZEROshortCONF1 216
#define ZEROlongCONF1 614
#define FshortCONF1 216
#define FlongCONF1 614
#define SYNClongCONF1 7330
#define SYNCshortCONF1 222

/******************************************************************************/
/* Channel bits for config 1 devices.                                         */
/******************************************************************************/

const unsigned char TXdataE[12] = {0,2,2,2,2,2,0,0,1,0,0,0}; // Bedroom http://en.chiptrue.com/images/LX2260_en.pdf
const unsigned char TXdataD[12] = {2,2,2,2,2,2,0,0,1,0,0,0}; // Livingroom light(A1 A0 x x x x x x)
const unsigned char TXdataF[12] = {2,0,2,2,2,2,0,0,1,0,0,0}; // Christmas Color (A1 A0 x x x x x x)
const unsigned char TXToiletMyBathroom[12] = {0,0,0,0,0,0,0,0,0,0,0,1}; // david toilet
const unsigned char TXToiletAlexBathroom[12] = {0,0,0,0,0,0,0,0,0,0,0,2}; // Alex toilet

/******************************************************************************/
/* Channel bits for config 1 devices that are arbitrary.                      */
/******************************************************************************/
const unsigned char TXdataFrontOPENED[12] = {0,0,0,0,0,0,0,0,0,0,1,0}; // door opened
const unsigned char TXdataFrontCLOSED[12] = {0,0,0,0,0,0,0,0,0,0,2,0}; // door closed
const unsigned char TXdataBedroomOPENED[12] = {0,0,0,0,0,0,0,0,0,2,0,0}; // door opened
const unsigned char TXdataBedroomCLOSED[12] = {0,0,0,0,0,0,0,0,0,1,0,0}; // door closed
const unsigned char TXdataLabOPENED[12] = {0,0,0,0,0,0,0,0,1,0,0,0}; // door opened
const unsigned char TXdataLabCLOSED[12] = {0,0,0,0,0,0,0,0,2,0,0,0}; // door closed

/******************************************************************************/
/* Channel bits for config 2 devices.                                         */
/******************************************************************************/

/*
 * these have different timing then the one above
 * zero is 650uS high folowed by 1.75mS low
 * full pulse is 2.42mS
 * one is 1.75mS high followed by 650uS high
 * sync is short high foloowed by 11.89 mS low
 */

//Channel H
const unsigned char fanON[] =  {0,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0}; // livingroom fan
const unsigned char fanOFF[] = {0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0}; // livingroom fan
const unsigned char BedON[] =  {0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0}; // door closed
const unsigned char BedOFF[] = {0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,0}; // door closed
const unsigned char AuxON[] =  {0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0}; // bedroom fan
const unsigned char AuxOFF[] = {0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0}; // bedroom fan

//Channel D
const unsigned char Aux2ON[] =  {0,1,1,0,1,0,0,0,0,0,1,0,0,0,0,1}; // christmas white
const unsigned char Aux2OFF[] = {0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,1}; // christmas white

//Channel B
const unsigned char SubLightON[] =  {0,1,1,0,1,0,0,0,0,0,1,0,0,1,0,0}; // door closed
const unsigned char SubLightOFF[] = {0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0}; // door closed

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void Send_Config1( unsigned char pin,const unsigned char* data);
void Zero_Config1(unsigned char pin);
void One_Config1(unsigned char pin);
void f_Config1(unsigned char pin);
void Send_Config2( unsigned char pin,const unsigned char* data);
void Zero_Config2(unsigned char pin);
void One_Config2(unsigned char pin);
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/