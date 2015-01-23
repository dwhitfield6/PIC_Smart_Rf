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
/* Contains functions that are called when an command is entered.
 *
/******************************************************************************/

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
#include <stdio.h>       /* For true/false definition */

#endif

#include "system.h"      /* System funct/params, like osc/peripheral config */
#include "user.h"        /* User funct/params, such as InitApp */

#include "MISC.h"
#include "UART.h"
#include "StringCommand.h"
#include "RF.h"
#include "PWM.h"
#include "LCD.h"
#include "RTC.h"

extern TIME NowTime;
extern unsigned char LCDwait;
extern unsigned long LCDclearCount;

void Command(char* Input)
{
    int channel = 20; //invalid channel. This is set in the function
    unsigned int ADC_Value = 0;
    unsigned int RTC_Battery_Value =0;
    unsigned int RTC_Battery_Value_HIGH_decimal =0;
    unsigned int RTC_Battery_Value_LOW_decimal =0;
    int duty = 0;
    int contrast = 0;
    unsigned char buf[200];

    UARTstring("\r\n\n");
    lowercase(Input);
    RemoveSpaces(Input);
    //--------------------------------------------------------------------------
    // LEDs
    if (StartsWith(Input, "led"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StringMatch(Input, "led0"))
            {
                LATD ^= LED0;
                UARTstring("LED0 Toggled");
                LCDdisplayFeedback("LED0 Toggled");
            }
            else if (StringMatch(Input, "led0=on"))
            {
                LATD |= LED0;
                UARTstring("LED0 On");
                LCDdisplayFeedback("LED0 On");
            }
            else if (StringMatch(Input, "led0=off"))
            {
                LATD &= ~LED0;
                UARTstring("LED0 Off");
                LCDdisplayFeedback("LED0 Off");
            }
            else if (StringMatch(Input, "led1"))
            {
                LATD ^= LED1;
                UARTstring("LED1 Toggled");
                LCDdisplayFeedback("LED1 Toggled");
            }
            else if (StringMatch(Input, "led1=on"))
            {
                LATD |= LED1;
                UARTstring("LED1 On");
                LCDdisplayFeedback("LED1 On");
            }
            else if (StringMatch(Input, "led1=off"))
            {
                LATD &= ~LED1;
                UARTstring("LED1 Off");
                LCDdisplayFeedback("LED1 Off");
            }
            else
            {
                UARTstring("LED What???");
                LCDdisplayFeedback("LED What???");
            }
        }
        else
        {
            UARTstring("LED Help Menu\r\n\n");
            UARTstring("Devices:\r\n");
            UARTstring("LED0\r\n");
            UARTstring("LED1\r\n\n");
            UARTstring("To control a device enter the device followed by '=' and 'On/Off'\r\n");
            UARTstring("i.e. LED1 = On\r\n");
            LCD_Scroll("LED Menu");
            LCD_Scroll("Devices:");
            LCD_Scroll("LED0");
            LCD_Scroll("LED1");
        }
    }        //--------------------------------------------------------------------------
        // Livingroom
    else if (StartsWith(Input, "livingroom"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StringMatch(Input, "livingroomlight=on") || StringMatch(Input, "livingroomlight=off"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config1(RFtx, TXdataD);
                Send_Config1(RFtx, TXdataD);
                Send_Config1(RFtx, TXdataD);
                Send_Config1(RFtx, TXdataD);
                Send_Config1(RFtx, TXdataD);
                Send_Config1(RFtx, TXdataD);
                Send_Config1(RFtx, TXdataD);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Livingroom Light Toggled");
                LCDdisplayFeedback("LVNG Rm Lt TGLD");
            }
            else if (StringMatch(Input, "livingroomfan=on"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                Send_Config2(RFtx, fanON);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Livingroom Fan On");
                LCDdisplayFeedback("LVNG Rm Lt On");
            }
            else if (StringMatch(Input, "livingroomfan=off"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                Send_Config2(RFtx, fanOFF);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Livingroom Fan Off");
                LCDdisplayFeedback("LVNG Rm Lt Off");
            }
            else
            {
                UARTstring("Livingroom Task???");
                LCDdisplayFeedback("LVNG Rm Task???");
            }
        }
        else
        {
            UARTstring("Livingroom Help Menu\r\n\n");
            UARTstring("Devices:\r\n");
            UARTstring("light\r\n");
            UARTstring("fan\r\n\n");
            UARTstring("To control a device enter the device followed by '=' and 'On/Off'\r\n");
            UARTstring("i.e. Livingroom fan = On\r\n");
            LCDdisplayFeedback("LVNG Rm Lt help");
            LCD_Scroll("LVNGroom Menu");
            LCD_Scroll("Devices:");
            LCD_Scroll("Light");
            LCD_Scroll("Fan");
        }
    }        //--------------------------------------------------------------------------
        // Bedroom
    else if (StartsWith(Input, "bedroom"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StringContains(Input, "light") || StringContains(Input, "fan"))
            {
                if (StringMatch(Input, "bedroomlight=on") || StringMatch(Input, "bedroomlight=off"))
                {
                    INTCONbits.GIE = 0; //global interrupts enabled
                    Send_Config1(RFtx, TXdataE);
                    Send_Config1(RFtx, TXdataE);
                    Send_Config1(RFtx, TXdataE);
                    Send_Config1(RFtx, TXdataE);
                    Send_Config1(RFtx, TXdataE);
                    Send_Config1(RFtx, TXdataE);
                    Send_Config1(RFtx, TXdataE);
                    INTCONbits.GIE = 1; //global interrupts enabled
                    UARTstring("Bedroom Light Toggled");
                    LCDdisplayFeedback("Bedroom Lt TGLD");
                }
                else if (StringMatch(Input, "bedroomfan=on"))
                {
                    INTCONbits.GIE = 0; //global interrupts enabled
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    Send_Config2(RFtx, AuxON);
                    INTCONbits.GIE = 1; //global interrupts enabled
                    UARTstring("Bedroom Fan On");
                    LCDdisplayFeedback("Bedroom Lt On");
                }
                else if (StringMatch(Input, "bedroomfan=off"))
                {
                    INTCONbits.GIE = 0; //global interrupts disabled
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    Send_Config2(RFtx, AuxOFF);
                    INTCONbits.GIE = 1; //global interrupts enabled
                    UARTstring("Bedroom Fan Off");
                    LCDdisplayFeedback("Bedroom Lt Off");
                }
                else if (StringMatch(Input, "bedroomshaker=on"))
                {
                    INTCONbits.GIE = 0; //global interrupts disabled
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    Send_Config2(RFtx, BedON);
                    INTCONbits.GIE = 1; //global interrupts enabled
                    UARTstring("Bedroom Shaker On");
                    LCDdisplayFeedback("Bedroom SKR On");
                }
                else if (StringMatch(Input, "bedroomshaker=off"))
                {
                    INTCONbits.GIE = 0; //global interrupts enabled
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    Send_Config2(RFtx, BedOFF);
                    INTCONbits.GIE = 1; //global interrupts enabled
                    UARTstring("Bedroom Shaker Off");
                    LCDdisplayFeedback("Bedroom SKR Off");
                }
            }
            else
            {
                UARTstring("Bedroom Task???");
                LCDdisplayFeedback("Bedroom Task???");
            }
        }
        else
        {
            UARTstring("Bedroom Help Menu\r\n\n");
            UARTstring("Devices:\r\n");
            UARTstring("light\r\n");
            UARTstring("fan\r\n");
            UARTstring("shaker\r\n\n");
            UARTstring("To control a device enter the device followed by '=' and 'On/Off'\r\n");
            UARTstring("i.e. Bedroom fan = On\r\n");
            LCDdisplayFeedback("Bedroom Help???");
            LCD_Scroll("Bedroom Menu");
            LCD_Scroll("Devices:");
            LCD_Scroll("Light");
            LCD_Scroll("Fan");
            LCD_Scroll("Shaker");
        }
    }        //--------------------------------------------------------------------------
        // Christmas
    else if (StringContains(Input, "color") || StringContains(Input, "white") || StringContains(Input, "christmas"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StringContains(Input, "color") && StringContains(Input, "="))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config1(RFtx, TXdataF);
                Send_Config1(RFtx, TXdataF);
                Send_Config1(RFtx, TXdataF);
                Send_Config1(RFtx, TXdataF);
                Send_Config1(RFtx, TXdataF);
                Send_Config1(RFtx, TXdataF);
                Send_Config1(RFtx, TXdataF);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Christmas Tree Colored Lights Toggled");
                LCDdisplayFeedback("XMAS Color TGLD");
            }
            else if (StringContains(Input, "white") && StringContains(Input, "=on"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config2(RFtx, Aux2ON);
                Send_Config2(RFtx, Aux2ON);
                Send_Config2(RFtx, Aux2ON);
                Send_Config2(RFtx, Aux2ON);
                Send_Config2(RFtx, Aux2ON);
                Send_Config2(RFtx, Aux2ON);
                Send_Config2(RFtx, Aux2ON);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Christmas Tree White Lights On");
                LCDdisplayFeedback("XMAS White On");
            }
            else if (StringContains(Input, "white") && StringContains(Input, "=off"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config2(RFtx, Aux2OFF);
                Send_Config2(RFtx, Aux2OFF);
                Send_Config2(RFtx, Aux2OFF);
                Send_Config2(RFtx, Aux2OFF);
                Send_Config2(RFtx, Aux2OFF);
                Send_Config2(RFtx, Aux2OFF);
                Send_Config2(RFtx, Aux2OFF);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Christmas Tree White Lights Off");
                LCDdisplayFeedback("XMAS White Off");
            }
            else
            {
                UARTstring("Christmas What???");
                LCDdisplayFeedback("XMAS What???");
            }
        }
        else
        {
            UARTstring("Christmas tree Help Menu\r\n\n");
            UARTstring("Devices:\r\n");
            UARTstring("white lights\r\n");
            UARTstring("colored lights\r\n\n");
            UARTstring("To control a device enter the device followed by '=' and 'On/Off'\r\n");
            UARTstring("i.e. Christmas Colored lights = On\r\n");
            LCD_Scroll("XMAS Menu");
            LCD_Scroll("Devices:");
            LCD_Scroll("white");
            LCD_Scroll("color");
        }
    }        //--------------------------------------------------------------------------
        // red led PWM
    else if (StartsWith(Input, "red"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StartsWith(Input, "redled") || StartsWith(Input, "redledbrightness"))
            {
                if (!StringContains(Input, "="))
                {
                    UARTstring("there is no question mark\r\n");
                }
                duty = GetEnteredNumber(Input);
                if (duty == 32001)
                {
                    UARTstring("Error: No number entered\r\n");
                    LCDdisplayFeedback("ERR: no number");
                }
                else if (duty > 100)
                {
                    UARTstring("Invalid Dutycycle: Greater than 100\r\n");
                    LCDdisplayFeedback("ERR: Duty > 100");
                }
                else if (duty < 0)
                {
                    UARTstring("Invalid Dutycycle: Less than 0\r\n");
                    LCDdisplayFeedback("ERR: Duty < 0");
                }
                else
                {
                    SetDuty(duty);
                    sprintf(buf, "Dutycycle set to %d\r\n", duty);
                    UARTstring(buf);
                    sprintf(buf, "Duty set to %d", duty);
                    LCDdisplayFeedback(buf);
                }
            }
            else
            {
                UARTstring("redled what??? Do you mean Red Led Brightness???");
                LCDdisplayFeedback("Red LED What???");
            }
        }
        else
        {
            UARTstring("Red LED PWM Help Menu\r\n\n");
            UARTstring("To set te PWM brightness enter 'Red LED brightness = dutycycle'\r\n");
            UARTstring("where the dutycycle is a number 0 - 100\r\n");
            LCD_Scroll("Red LED Contrast");
            LCD_Scroll("Commands:");
            LCD_Scroll("Red LED=#");
        }
    }        //--------------------------------------------------------------------------
        // Contrast PWM
    else if (StartsWith(Input, "contrast"))
    {
        if (!StringContains(Input, "?"))
        {
            if (!StringContains(Input, "="))
            {
                StringAddEqual(Input);
            }
            contrast = GetEnteredNumber(Input);
            if (contrast == 32001)
            {
                UARTstring("Error: No number entered\r\n");
                LCDdisplayFeedback("ERR: No number");
            }
            else if (contrast > 100)
            {
                UARTstring("Invalid Contrast Duty cycle: Greater than 100\r\n");
                LCDdisplayFeedback("ERR: CNST > 100");
            }
            else if (contrast < 0)
            {
                UARTstring("Invalid Contrast Duty Cycle: Less than 0\r\n");
                LCDdisplayFeedback("ERR: CNST < 0");
            }
            else
            {
                SetContrast(contrast);
                sprintf(buf, "Contrast Duty Cycle set to %d\r\n", contrast);
                UARTstring(buf);
                sprintf(buf, "CNST set to %d", duty);
                LCDdisplayFeedback(buf);
            }
        }
        else
        {
            UARTstring("Contrast PWM Help Menu\r\n\n");
            UARTstring("To set Contrast PWM brightness enter 'Contrast = dutycycle'\r\n");
            UARTstring("where the dutycycle is a number 0 - 100\r\n");
            LCD_Scroll("LCD Contrast");
            LCD_Scroll("Commands:");
            LCD_Scroll("Contrast#");
        }
    }        //--------------------------------------------------------------------------
        // ADC
    else if (StartsWith(Input, "adc"))
    {
        if (!StringContains(Input, "?"))
        {
            if (!StringContains(Input, "="))
            {
                StringAddEqual(Input);
            }
            channel = GetEnteredNumber(Input);
            if (channel == 32002) {
                UARTstring("Error: only use one equal sign\r\n");
                LCDdisplayFeedback("ERR: Use 1 =");
            }
            else if (channel < 0)
            {
                UARTstring("Error: No negative channels\r\n");
                LCDdisplayFeedback("ERR: Use + Chnl");
            }
            else if (channel >= 32000)
            {
                UARTstring("Error: in parsing command\r\n");
                LCDdisplayFeedback("ERR: Cant parse");
            }
            else if (channel != 0)
            {
                sprintf(buf, "Channel %d is not configured", channel);
                UARTstring(buf);
                sprintf(buf, "Chnl set to %d\r\n", duty);
                LCDdisplayFeedback(buf);
            }
            else
            {
                LATE |= POT_ON;
                delayUS(1000);
                ADC_Value = InternalADC_Read(channel);
                sprintf(buf, "ADC channel %d = %d", channel, ADC_Value);
                LATE &= ~POT_ON;
                UARTstring(buf);
                sprintf(buf, "Chnl %d = %d", channel, ADC_Value);
                LCDdisplayFeedback(buf);
            }
        }
        else
        {
            UARTstring("ADC Help Menu\r\n\n");
            UARTstring("Channels:\r\n");
            UARTstring("0\r\n\n");
            UARTstring("To read and adc channel, type the command\r\n ADC"
                    " followed by 'Channel' \r\n");
            UARTstring("i.e. ADC 0\r\n");
            LCD_Scroll("ADC Menu");
            LCD_Scroll("Commands:");
            LCD_Scroll("0");
        }
    }        //--------------------------------------------------------------------------
        // RTC
    else if (StartsWith(Input, "rtc"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StringContains(Input, "battery"))
            {
                RTC_Battery_Value = (InternalADC_Read(2) + 8);
                RTC_Battery_Value_HIGH_decimal = (unsigned int)(((float)RTC_Battery_Value / 1024) * 5);
                RTC_Battery_Value_LOW_decimal = (unsigned int)((((float)RTC_Battery_Value / 1024) * 500));
                RTC_Battery_Value_LOW_decimal = RTC_Battery_Value_LOW_decimal % 100;
                sprintf(buf, " 3 Volt RTC Battery Voltage = %d.%d",
                        RTC_Battery_Value_HIGH_decimal, RTC_Battery_Value_LOW_decimal);
                UARTstring(buf);
                sprintf(buf, "RTC Bty = %d.%d", RTC_Battery_Value_HIGH_decimal,
                        RTC_Battery_Value_LOW_decimal);
                LCDdisplayFeedback(buf);
            }
            else if(StringContains(Input, "read"))
            {
                if(!READ_RTC_TIME())
                {
                    SprintDate(buf, NowTime, 0);
                    UARTstring(buf);
                    UARTstring("\r\n");
                    SetLCDcursor(0, 0);
                    LCDPrintString("RTC Time is:    ");
                    if(NowTime.HoursAM_PM)
                    {
                        sprintf(buf,"%02d/%02d %02d:%02d PM", NowTime.Month, NowTime.Date, NowTime.Hours, NowTime.Minutes, NowTime.Seconds);
                    }
                    else
                    {
                        sprintf(buf,"%02d/%02d %02d:%02d AM", NowTime.Month, NowTime.Date, NowTime.Hours, NowTime.Minutes, NowTime.Seconds);
                    }
                    SetLCDcursor(1, 0);
                    LCDPrintString(buf);
                }
                else
                {
                    UARTstring("ERROR: Can't Read RTC\r\n");
                    SetLCDcursor(1, 0);
                    LCDPrintString("RTC isnt working");
                }
                LCDclearCount =0;
            }
            else if(StringContains(Input, "set"))
            {
                UARTstring("Enter Time MM/DD/YY HH:MM:SS AM or\r\n");
                UARTstring("Enter Time MM/DD/YY HH:MM:SS PM\r\n");
                cleanBuffer(Input, 100);
                UARTstring(">");
                while(1)
                {
                    if(Input[0] != 0)
                    {
                        lowercase(Input);
                        if(Parse_Time(Input))
                        {
                            if(!SET_RTC_TIME(NowTime))
                            {
                                UARTstring("\r\nTime Set Successfully\r\n");
                                READ_RTC_TIME();
                                SprintDate(buf, NowTime, 0);
                                UARTstring(buf);
                                UARTstring("\r\n");
                            }
                            else
                            {
                                UARTstring("\r\nTime Not Set Successfully");
                            }
                        }
                        else
                        {
                            UARTstring("\r\nInvalid Time Entered\r\n");
                            UARTstring("Format is MM/DD/YY HH:MM:SS AM\r\n");
                        }
                        break;
                    }
                }
            }
            else
            {
            UARTstring("RTC What???\n");
            }
        }
        else
        {
            UARTstring("Real Time Clock Help Menu\r\n\n");
            UARTstring("Actions:\r\n");
            UARTstring("Battery\r\n");
            UARTstring("Read\r\n");
            UARTstring("Set\r\n\n");
            UARTstring("To command the RTC, type the action\r\n followed by"
                    " the parameters \r\n\n");
            UARTstring("further sub menus available by entering the action and a '?'");
            UARTstring("i.e. RTC Set Time?\r\n");
            LCD_Scroll("RTC Menu");
            LCD_Scroll("Commands:");
            LCD_Scroll("Battery");
            LCD_Scroll("Read");
            LCD_Scroll("Set");

        }
    }        //--------------------------------------------------------------------------
        // Joes Room
    else if (StartsWith(Input, "joe"))
    {
        if (!StringContains(Input, "?"))
        {
            if (StringMatch(Input, "joesroomlight=on"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                Send_Config2(RFtx, SubLightON);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Joe's room light On");
                LCDdisplayFeedback("Joe room lt On");
            }
            else if (StringMatch(Input, "joesroomlight=off"))
            {
                INTCONbits.GIE = 0; //global interrupts disabled
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                Send_Config2(RFtx, SubLightOFF);
                INTCONbits.GIE = 1; //global interrupts enabled
                UARTstring("Joes room light Off");
                LCDdisplayFeedback("Joe room lt Off");
            }
            else
            {
                UARTstring("Joe's room what???");
                LCDdisplayFeedback("Joe room what??");
            }
        }
        else
        {
            UARTstring("Joes Room Help Menu\r\n\n");
            UARTstring("Devices:\r\n");
            UARTstring("Light\r\n\n");
            UARTstring("To control a device enter the device followed by '=' and 'On/Off'\r\n");
            UARTstring("i.e. Joes room light = On\r\n");
            LCD_Scroll("Joe room Menu");
            LCD_Scroll("Devices:");
            LCD_Scroll("Light");
        }
    }        //--------------------------------------------------------------------------
        // ?
    else if (StringMatch(Input, "?"))
    {
        UARTstring("Devices:\r\n\n");
        UARTstring("Bedroom\r\n");
        UARTstring("Livingroom\r\n");
        UARTstring("Joes room\r\n");
        UARTstring("Christmas Tree\r\n");
        UARTstring("Contrast\r\n");
        UARTstring("ADC\r\n");
        UARTstring("RTC\r\n\n");
        UARTstring("For Device help enter device followed by a '?' \r\n");
        UARTstring("i.e.  Livingroom?\r\n\r\n");
        UARTstring("Current Time\r\n");
        SetLCDcursor(0, 0);
        LCDPrintString("Help Menu");
        if(!READ_RTC_TIME())
        {
            SprintDate(buf, NowTime, 0);
            UARTstring(buf);
            UARTstring("\r\n");

            if(NowTime.HoursAM_PM)
            {
                sprintf(buf,"%02d/%02d %02d:%02d PM", NowTime.Month, NowTime.Date, NowTime.Hours, NowTime.Minutes, NowTime.Seconds);
            }
            else
            {
                sprintf(buf,"%02d/%02d %02d:%02d AM", NowTime.Month, NowTime.Date, NowTime.Hours, NowTime.Minutes, NowTime.Seconds);
            }
            SetLCDcursor(1, 0);
            LCDPrintString(buf);
            LCD_Scroll("Device Menus:");
            LCD_Scroll("Bedroom?");
            LCD_Scroll("Livingroom?");
            LCD_Scroll("Joes Room?");
            LCD_Scroll("Christmas Tree?");
            LCD_Scroll("Contrast?");
            LCD_Scroll("ADC?");
            LCD_Scroll("RTC?");
        }
        else
        {
            UARTstring("ERROR: Can't Read RTC\r\n");
            SetLCDcursor(1, 0);
            LCDPrintString("RTC isnt working");
        }
        LCDclearCount =0;
    }        //--------------------------------------------------------------------------
        // Invalid
    else
    {
        UARTstring("Invalid Command");
        LCDdisplayFeedback("Invalid Command");
    }

    UARTstring("\r\n\r\n");
}

