/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/22/15     1.0         Created log.
/******************************************************************************/

/******************************************************************************/
/* Contains functions that control the LCD screen.
 *
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
#include "LCD.h"          /* User funct/params, such as InitApp */
#include "MISC.h"          /* User funct/params, such as InitApp */
#include "Timer.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

unsigned char Scroll_1[16];
unsigned char Scroll_2[16];
unsigned char Scroll_3[16];
unsigned char Scroll_4[16];
unsigned char Scroll_5[16];
unsigned char Scroll_6[16];
unsigned char Scroll_7[16];
unsigned char Scroll_8[16];
unsigned char Scroll_9[16];
unsigned char Scroll_10[16];
unsigned char ScrollDisplayPosition=0;
unsigned char ScrollFIFOCount = 0;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* init_LCD
 *
 * The function initializes LCD screen. The Module is the 1602A.
/******************************************************************************/
void init_LCD()
{
    LATD &= ~(RW);//write mode
    PulseEnablePin();
    LATD &= ~(Enable + RS);//clear command bits
    PulseEnablePin();

    delayUS(50000);
    SendLCDbyte(0x00, TRUE);//clear buffer
    delayUS(50000);
    SendLCDbyte(0x00, FALSE);//clear buffer
    delayUS(50000);

    LATA |= (D5);//set to 4bit mode
    delayUS(50000);
    PulseEnablePin();
    delayUS(50000);

    // set 4-bit input - second time.
    // (as reqd by the spec.)
    SendLCDbyte(0x28, FALSE);
    delayUS(50000);
    
    // 2. Display on, cursor on, blink cursor
    SendLCDbyte(0x0C, FALSE);
    delayUS(50000);

    // 3. Cursor move auto-increment
    SendLCDbyte(0x06, FALSE);
    delayUS(50000);

    ClearLCD();
    delayUS(50000);

    init_Timer1();
}

/******************************************************************************/
/* SendLCDbyte
 *
 * The function sends one byte over the parallel port to the lcd.
/******************************************************************************/
void SendLCDbyte(unsigned char data, unsigned char isCommand)
{
    unsigned char MSB_nibble=0;
    unsigned char LSB_nibble=0;

    LATA &= ~(D7+D6+D5);//clear data bits
    LATE &= ~D4;//cleat data bits
    LATD &= ~(Enable + RS);//clear command bits
    delayUS(10);
    MSB_nibble = (data & 0xF0)>>4;
    LSB_nibble = (data & 0x0F);
    if(!LCDwait)
    {
        if(isCommand)
        {
            LATD |= (RS);//clear command bits
        }
        else
        {
            LATD &= ~(RS);//clear command bits
        }
        if(MSB_nibble&0b1000)
        {
            LATA |= D7;
        }
        if(MSB_nibble&0b0100)
        {
            LATA |= D6;
        }
        if(MSB_nibble&0b0010)
        {
            LATA |= D5;
        }
        if(MSB_nibble&0b0001)
        {
            LATE |= D4;
        }
        PulseEnablePin();//send MSB nibble data
        LATA &= ~(D7+D6+D5);//clear data bits
        LATE &= ~D4;//cleat data bits
        delayUS(10);
        if(LSB_nibble&0b1000)
        {
            LATA |= D7;
        }
        if(LSB_nibble&0b0100)
        {
            LATA |= D6;
        }
        if(LSB_nibble&0b0010)
        {
            LATA |= D5;
        }
        if(LSB_nibble&0b0001)
        {
            LATE |= D4;
        }
        PulseEnablePin();//send MSB nibble data
    }
    else
    {
        delayUS(20);
    }
}

/******************************************************************************/
/* PulseEnablePin
 *
 * The function pulses the Enable pin. The toggling of this pin is required for
 *   reset and to put into 4-bit mode.
/******************************************************************************/
void PulseEnablePin()
{
    // pull EN bit high
    LATD |= (Enable);
    delayUS(2500);
    // pull EN bit low again
    LATD &= ~(Enable);
    delayUS(2500);
}

/******************************************************************************/
/* SetLCDcursor
 *
 * The function sets the LCD curcor so that a print is on the correct line.
/******************************************************************************/
void SetLCDcursor(char Row, char Col)
{
    char address;

    // construct address from (Row, Col) pair
    if (Row == 0)
    {
        address = 0;
    }
    else
    {
        address = 0x40;
    }

    address |= Col;
    if(!LCDwait)
    {
        SendLCDbyte(0x80 | address, FALSE);
    }
}

/******************************************************************************/
/* ClearLCD
 *
 * The function clears the lcd screen data and returns the
 * cursor to home position.
/******************************************************************************/
void ClearLCD()
{
    //
    // Clear display, return home
    //
    SendLCDbyte(0x01, FALSE);
    SendLCDbyte(0x02, FALSE);
}

/******************************************************************************/
/* LCDPrintString
 *
 * The function prints a string of characters on the LCD.
/******************************************************************************/
void LCDPrintString(const unsigned char *Text)
{
    unsigned char *c;

    c = (unsigned char*)Text;

    while ((c != 0) && (*c != 0))
    {
        SendLCDbyte(*c, TRUE);
        c++;
    }
}

/******************************************************************************/
/* LCDdisplayFeedback
 *
 * The function prints a string of characters on the second line of the lcd
 *   after a command is sent aka the enter key is hit.
/******************************************************************************/
void LCDdisplayFeedback(const unsigned char *Text)
{
    ClearLCD();
    SetLCDcursor(1, 0);
    LCDPrintString(Text);
    LCDwait = 1;
    Reset_Timer1();
    Start_Timer1();
    LCDclearCount =0;
}

/******************************************************************************/
/* LCDPrintChar
 *
 * The function prints a char on the LCD screen.
/******************************************************************************/
void LCDPrintChar(unsigned char character)
{
        SendLCDbyte(character, TRUE);
}

/******************************************************************************/
/* LCDScreenUpdate
 *
 * The function is called to know when to update the lcd. The screen must not
 *   change too fast and can not be written to until the delay is cleared.
/******************************************************************************/
void LCDScreenUpdate(void)
{
    //Timer for clearing screen
    if(LCDclearCount<(LCDdelay-1))
    {
        LCDclearCount++;
        LCDwait = 1;
    }
    else if(LCDclearCount == (LCDdelay-1))
    {
        LCDclearCount++;
        LCDwait = 0;
        if(ScrollFIFOCount == 0)
        {
            Stop_Timer1();
            ScrollDisplayPosition = 0;
            ClearLCD();
            SetLCDcursor(0, 0);
            LCDPrintChar('>');       
        }
        else
        { 
            ScrollDisplayPosition++;
            if(ScrollDisplayPosition == 1)
            {
                ClearLCD();
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_1);
            }
            else if(ScrollDisplayPosition == 2)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_1);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_2);
                cleanBuffer(Scroll_1, 16);
            }
            else if(ScrollDisplayPosition == 3)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_2);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_3);
                cleanBuffer(Scroll_2, 16);
            }
            else if(ScrollDisplayPosition == 4)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_3);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_4);
                cleanBuffer(Scroll_3, 16);
            }
            else if(ScrollDisplayPosition == 5)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_4);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_5);
                cleanBuffer(Scroll_4, 16);
            }
            else if(ScrollDisplayPosition == 6)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_5);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_6);
                cleanBuffer(Scroll_5, 16);
            }
            else if(ScrollDisplayPosition == 7)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_6);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_7);
                cleanBuffer(Scroll_6, 16);
            }
            else if(ScrollDisplayPosition == 8)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_7);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_8);
                cleanBuffer(Scroll_7, 16);
            }
            else if(ScrollDisplayPosition == 9)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_8);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_9);
                cleanBuffer(Scroll_8, 16);
            }
            else if(ScrollDisplayPosition == 10)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_9);
                SetLCDcursor(1, 0);
                LCDPrintString(Scroll_10);
                cleanBuffer(Scroll_9, 16);
            }
            else if(ScrollDisplayPosition == 11)
            {
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintString(Scroll_10);
                cleanBuffer(Scroll_10, 16);
            }
            if(ScrollDisplayPosition > ScrollFIFOCount)
            {
                ScrollFIFOCount=0;
                ScrollDisplayPosition=0;
            }
            LCDclearCount = SCROLLspeed;
        }
    }
}

/******************************************************************************/
/* LCD_Scroll
 *
 * The function is called when lines are qued up on the buffer.
/******************************************************************************/
void LCD_Scroll(const unsigned char* This)
{
    Start_Timer1();
    if(!LCDwait)
    {
        LCDclearCount = SCROLLspeed;
        ScrollFIFOCount++;
        if(ScrollFIFOCount == 1)
        {
           BufferCopy((unsigned char*) This,Scroll_1, 16, 0);
        }
        else if(ScrollFIFOCount == 2)
        {
           BufferCopy((unsigned char*) This,Scroll_2, 16, 0);
        }
        else if(ScrollFIFOCount == 3)
        {
           BufferCopy((unsigned char*) This,Scroll_3, 16, 0);
        }
        else if(ScrollFIFOCount == 4)
        {
           BufferCopy((unsigned char*) This,Scroll_4, 16, 0);
        }
        else if(ScrollFIFOCount == 5)
        {
           BufferCopy((unsigned char*) This,Scroll_5, 16, 0);
        }
        else if(ScrollFIFOCount == 6)
        {
           BufferCopy((unsigned char*) This,Scroll_6, 16, 0);
        }
        else if(ScrollFIFOCount == 7)
        {
           BufferCopy((unsigned char*) This,Scroll_7, 16, 0);
        }
        else if(ScrollFIFOCount == 8)
        {
           BufferCopy((unsigned char*) This,Scroll_8, 16, 0);
        }
        else if(ScrollFIFOCount == 9)
        {
           BufferCopy((unsigned char*) This,Scroll_9, 16, 0);
        }
        else if(ScrollFIFOCount == 10)
        {
           BufferCopy((unsigned char*) This,Scroll_10, 16, 0);
        }
    }
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/