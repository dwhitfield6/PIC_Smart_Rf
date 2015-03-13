/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/11/15     1.0         Initial coding.
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
/* Defines                                                                    */
/******************************************************************************/

#define IR_LED_TIMEOUT 2500
#define IR_SIZE MaxScanEdgeChange

/******************************************************************************/
/* MaxScanEdgeChange
 *
 * Max number to bit flip flops.
/******************************************************************************/
/* this is equal to (number of bits + header) *2*/
#define MaxScanEdgeChange 72

/******************************************************************************/
/* NEC bit Timing for 40.96 MHz                                               */
/******************************************************************************/
/* Starting bit High */
/* Nominal 9mS*/
/* High Tolerance is 10mS */
#define StartbitHIGHupper 3200
/* Low Tolerance is 8mS */
#define StartbitHIGHlower 2560

/* Starting bit Low */
/* Nominal 4.5mS*/
/* High Tolerance is 5.5mS */
#define StartbitLOWupper 1760
/* Low Tolerance is 3.5mS */
#define StartbitLOWlower 1120

/* Logical 0 burst/Space and Logical 1 burst*/
/* Nominal 0.5625mS*/
/* High Tolerance is 1.2mS */
#define DataShortupper 384
/* Low Tolerance is 0.25mS */
#define DataShortlower 80

/* Logical 1 space */
/* Nominal 1.6875mS*/
/* High Tolerance is 1.9mS */
#define DataLongupper 608
/* Low Tolerance is 1.4mS */
#define DataLonglower 448

/* Pause */
/* Nominal 40mS*/
/* High Tolerance is 42mS */
#define PauseBurstupper 13440
/* Low Tolerance is 38mS */
#define PauseBurstlower 12160

/* Nominal 2.25mS*/
/* High Tolerance is 2.5mS */
#define PauseSpaceupper 800
/* Low Tolerance is 1.85mS */
#define PauseSpacelower 592

/* Repeat 96mS*/
/* High Tolerance is 90mS */
#define Repeatupper 32000L
/* Low Tolerance is 100mS */
#define Repeatlower 28800L

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

extern unsigned int BlueLEDcount;
extern unsigned int IRRawCode[IR_SIZE];
extern unsigned long IR_NEC;
extern unsigned int IRrawCount;
extern unsigned char IRrawCodeNum;
extern unsigned char IR_New_Code;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void init_IR(void);
unsigned char ReadIRpin(void);
unsigned char IRrawToNEC(unsigned int* Raw, unsigned long* NEC, unsigned char Invert);
void UseIRCode(unsigned char* Code, unsigned long NEC);
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/