/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/22/15     1.0         Created log.
 * 01/22/15     1.0_DW0     Added version banner.
/******************************************************************************/

/******************************************************************************/
/* Contains the main function.
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
#include <stdio.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

#include "MISC.h"
#include "UART.h"
#include "StringCommand.h"
#include "RF.h"
#include "ADC.h"
#include "PWM.h"
#include "LCD.h"
#include "I2C.h"
#include "RTC.h"

/******************************************************************************/
/* Version                                                                    */
/******************************************************************************/

unsigned char Version[20] = "1.0_DW0";
/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

extern unsigned char config;
extern unsigned int spbrg;
extern unsigned char baudconfig;
extern struct TIME NowTime;
extern char Txdata[100];
extern char Rxdata[100];
unsigned char buf[100];
unsigned char bufferCount =0;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    unsigned char ii =0;
    unsigned char count =0;
    
    ConfigureOscillator();
    InitApp();
    InitUART(115200, 0);
    EnableInternalADC(1);
    PWM_init();
    ContrastPWM_init();
    init_LCD();
    Init_I2C_Master();
    RTC_INIT();

    UARTstring("Firmware Version: ");
    UARTstring(Version);
    UARTstring("\r\n");

    for(ii =0; ii <10;ii++)
    {
        LATD |= LED0;
        LATD &= ~LED1;
        delayUS(50000);
        LATD |= LED1;
        LATD &= ~LED0;
        delayUS(50000);
    }
    SetContrast(65);
    delayUS(20);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting");
    UARTstring("Starting...");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting.");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting..");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting...");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting....");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting.....");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting......");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting.......");
    delayUS(100000);
    SetLCDcursor(0, 0);
    LCDPrintString("Starting........");
    delayUS(100000);
    SetLCDcursor(1, 0);
    LCDPrintString("Enter '?'");
    delayUS(100000);
    UARTstring("\r\nEnter '?' for Help Menu\r\n>");
    
    for(count =100; count >0; count-=5)
    {
        SetDuty(count);
        delayUS(50000);
    }

   LATD &= ~(LED0+LED1);
   ReadUSART(); //clear the buffer
   ReadUSART(); //clear the buffer
   ReadUSART(); //clear the buffer

   ClearLCD();
   SetLCDcursor(0, 0);
   LCDPrintChar('>');
   
   while(1)
   {
        //sprintf(Txdata,"%u \n",count);
        LCDScreenUpdate();
        if(Rxdata[0] != 0)
        {
            if (Rxdata[0] != '\r' && Rxdata[0] != '\n')
            {
                if(Command(Rxdata))
                {
                    UARTstring("\r\n>");
                }
            }
            cleanBuffer(Rxdata, 100);            
        }
    }
}

