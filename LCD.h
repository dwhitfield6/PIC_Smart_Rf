/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#define USE_OR_MASKS
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

#define LCDdelay    25000
#define SCROLLspeed 15000 //scroll speed is inversly related to LCD delay and must be less

unsigned long LCDclearCount=LCDdelay;
unsigned char LCDwait=FALSE;

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
