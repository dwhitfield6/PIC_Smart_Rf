/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/21/15     8.0_DW0a    Changed version number to match new scheme.
 * 03/12/15     1.0_DW0a    Added version banner.
 *                          Added EEPROM functionaity.
 *                          Added code to save the Baud rate in EEPROM.
 *                          Added IR functionality.
 *                          Added pushbotton functionality.
 *                          Added new implementation of gloabls and EEPROM
 *                            data.
 *                          Added new EEPROM functions.
 *                          LCD screen loop is called from interrupt instead
 *                            of main loop.
 * 01/22/15     1.0         Created log.
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
#include "EEPROM.h"          /* User funct/params, such as InitApp */
#include "IR.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Version                                                                    */
/******************************************************************************/

unsigned char Version[20] = "8.0_DW0a";
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
    unsigned char ii = 0;
    unsigned char count = 0;
    unsigned char MemoryBurnflag = 0;
    
    ConfigureOscillator();
    InitApp();
    EnableInternalADC(1);
    PWM_init();
    ContrastPWM_init();
    Init_I2C_Master();
    RTC_INIT();
    init_LCD();

    SyncEEPROMandGLOBAL();

    if(Global.Baud < 2400 || Global.Baud > 115200)
    {
        /* EEPROM Baud and is not in range */
        Global.Baud = defaultBaud;
        Global.Parity = NONE;
        /* Burn Baud and Parity */
        if(!SetEEPROM(Global,0x3))
        {
            /* Burned Baud and Parity */
            MemoryBurnflag = 1;
        }
    }
    SyncEEPROMandGLOBAL();
    InitUART(Global.Baud, Global.Parity);
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
    LATD &= ~LED1;
    SetContrast(65);
    delayUS(20);
    SetLCDcursor(0, 0);
    if(MemoryBurnflag)
    {
        UARTstring("Error: Could not burn default Baud rate into Memory\r\n");
        LCDPrintString("Err: Baud Mem");
        delayUS(100000);
        ClearLCD();
        SetLCDcursor(0, 0);
    }
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

   init_IR();
   
   while(1)
   {
        if(BlueLEDcount < IR_LED_TIMEOUT)
        {
            BlueLEDcount++;
        }
        else
        {
            LATD &= ~LED0;
            LATD &= ~LED1;
        }
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
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
