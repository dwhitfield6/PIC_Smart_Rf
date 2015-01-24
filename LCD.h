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
/* LCD delay. This controls the amount of time that the new command is held on
 *   the screen.
/******************************************************************************/

#define LCDdelay    25000

/******************************************************************************/
/* SCROLLspeed. This controls the amount of time that a scrolled string waits
 *  until it is incremented and changed.
/******************************************************************************/

#define SCROLLspeed 15000

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/



/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

unsigned long LCDclearCount=LCDdelay;
unsigned char LCDwait=FALSE;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void init_LCD();
void SendLCDbyte(unsigned char data, unsigned char isCommand);
void PulseEnablePin();
void SetLCDcursor(char Row, char Col);
void ClearLCD();
void LCDPrintString(unsigned char *Text);
void LCDPrintChar(unsigned char character);
void LCDScreenUpdate();
void LCDdisplayFeedback(unsigned char *Text);
void LCD_Scroll(unsigned char* This);
