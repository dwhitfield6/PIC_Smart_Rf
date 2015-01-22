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

#include "user.h"          /* User funct/params, such as InitApp */
#include "LCD.h"          /* User funct/params, such as InitApp */
#include "MISC.h"          /* User funct/params, such as InitApp */

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

void init_LCD()
{
    LATD &= ~(RW);//write mode
    PulseEnablePin();
    LATD &= ~(Enable + RS);//clear command bits
    PulseEnablePin();

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

}

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

void PulseEnablePin()
{
    // pull EN bit high
    LATD |= (Enable);
    delayUS(2500);
    // pull EN bit low again
    LATD &= ~(Enable);
    delayUS(2500);
}

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

// Clear the screen data and return the
// cursor to home position
//
// Parameters:
//
//    void.
//
// Return
//
//     void.
//
void ClearLCD()
{
    //
    // Clear display, return home
    //
    SendLCDbyte(0x01, FALSE);
    SendLCDbyte(0x02, FALSE);
}

void LCDPrintString(unsigned char *Text)
{
    unsigned char *c;

    c = Text;

    while ((c != 0) && (*c != 0))
    {
        SendLCDbyte(*c, TRUE);
        c++;
    }
}

void LCDdisplayFeedback(unsigned char *Text)
{
    ClearLCD();
    SetLCDcursor(1, 0);
    LCDPrintString(Text);
    LCDwait = 1;
    LCDclearCount =0;
}

void LCDPrintChar(unsigned char character)
{
        SendLCDbyte(character, TRUE);
}

void LCDScreenUpdate(void)
{
    //Timer for clearing screen
    delayUS(10);
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

void LCD_Scroll(unsigned char* This)
{
    if(!LCDwait)
    {
        LCDclearCount = SCROLLspeed;
        ScrollFIFOCount++;
        if(ScrollFIFOCount == 1)
        {
           BufferCopy(This,Scroll_1, 16, 0);
        }
        else if(ScrollFIFOCount == 2)
        {
           BufferCopy(This,Scroll_2, 16, 0);
        }
        else if(ScrollFIFOCount == 3)
        {
           BufferCopy(This,Scroll_3, 16, 0);
        }
        else if(ScrollFIFOCount == 4)
        {
           BufferCopy(This,Scroll_4, 16, 0);
        }
        else if(ScrollFIFOCount == 5)
        {
           BufferCopy(This,Scroll_5, 16, 0);
        }
        else if(ScrollFIFOCount == 6)
        {
           BufferCopy(This,Scroll_6, 16, 0);
        }
        else if(ScrollFIFOCount == 7)
        {
           BufferCopy(This,Scroll_7, 16, 0);
        }
        else if(ScrollFIFOCount == 8)
        {
           BufferCopy(This,Scroll_8, 16, 0);
        }
        else if(ScrollFIFOCount == 9)
        {
           BufferCopy(This,Scroll_9, 16, 0);
        }
        else if(ScrollFIFOCount == 10)
        {
           BufferCopy(This,Scroll_10, 16, 0);
        }
    }
}