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
/* Contains ISR
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

#include "MISC.h"
#include "UART.h"
#include "LCD.h"
#include "StringCommand.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

extern char Txdata[100];
extern char Rxdata[100];
extern unsigned char bufferCount;
extern unsigned char TX_OLD;

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/******************************************************************************/
/* High-Priority Interrupt Routine                                            */
/******************************************************************************/

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0
    
      /* TODO Add High Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif

}

/******************************************************************************/
/* Low-Priority Interrupt Routine                                             */
/******************************************************************************/
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    unsigned char rx;
    unsigned char TEMP_Rxdata[100];
    unsigned char LCD_TEMP [16];
    int i=0;
    unsigned char j=0;
    
    INTCONbits.PEIE = 0; //Disable pheripheral interrupt
      
    //check if the interrupt is caused by RX pin
    if(PIR1bits.RCIF == 1)
    {
        // UART rx interrupt
        PIE1bits.RCIE = 0; //Disable RX interrupt
        RCSTAbits.CREN = 0;
        rx = ReadUSART(); // read the byte from rx register

        if(TX_OLD == '>' && rx == '>' && bufferCount == 0)
        {
           // there are repeat characters of ">>" in the very beginning
           NOP();// dont use this character
        }
        else
        {
            // this is a valid character
            if(rx != '\n' && rx != '\r')
            {
                //not a carriage return
                if(rx == 0x7f || rx == 0x08)
                {
                    // the backspace or delete key has been hit.
                    if(bufferCount !=0)
                    {
                        // There are characters in the buffer already
                        UARTstring("\r");
                        // go back to beginning home point
                        for(i=0;i<=bufferCount;i++)
                        {
                            //erase the screen on the serial port monitor
                            UARTstring(" ");
                        }
                        // go back to beginning home point
                        UARTstring("\r");
                        UARTstring(">");
                        bufferCount--;
                        TEMP_Rxdata[bufferCount] = 0;
                        UARTstring(TEMP_Rxdata);
                        if(bufferCount < LCD_CharacterWidth)
                        {
                            //erase a charaacter on the LCD
                            for(j=0; j<15;j++)
                            {
                                   LCD_TEMP[j] = TEMP_Rxdata[j];
                            }
                            ClearLCD();
                            SetLCDcursor(0, 0);
                            LCDPrintChar('>');
                            SetLCDcursor(0, 1);
                            LCDPrintString(LCD_TEMP);
                        }
                    }
                }
                else
                {
                    // this is a character that is not a backspace delete
                    // key carriage return or line feed
                    if(IsPrintableASCII(rx))
                    {
                        // only use printable ascii
                        UARTchar(rx, NO, 0); //echo the data
                        if(bufferCount < LCD_CharacterWidth)
                        {
                            // there is enough room on the LCD screen
                            LCDPrintChar(rx);
                        }
                        TEMP_Rxdata[bufferCount] = rx;
                        bufferCount++;
                        if(bufferCount>=99)
                        {
                            UARTstring("\r\nBuffer overflow! Deleting Buffer \r\n");
                            cleanBuffer(TEMP_Rxdata, 100);
                            bufferCount=0;
                        }
                    }
                    else
                    {

                    }
                }
            }
            else if((rx == '\n' || rx == '\r') && bufferCount == 0)
            {
                // no data here but enter was hit
                cleanBuffer(TEMP_Rxdata, 100);
            }
            else
            {
                // carraige return or line feed and there is data in the buffer
                if(TEMP_Rxdata[0] == 0 && TEMP_Rxdata[1] != 0)
                {
                    BufferCopy(TEMP_Rxdata, Rxdata, 100,1);
                }
                else
                {
                    BufferCopy(TEMP_Rxdata, Rxdata, 100,0);
                }
                ClearLCD();
                SetLCDcursor(0, 0);
                LCDPrintChar('>');
                cleanBuffer(TEMP_Rxdata, 100);
                bufferCount=0;
            }
        }
    }
    PIE1bits.RCIE = 1; //Enable RX interrupt
    RCSTAbits.CREN = 1;
    INTCONbits.PEIE = 1; //Enable pheripheral interrupt
}
