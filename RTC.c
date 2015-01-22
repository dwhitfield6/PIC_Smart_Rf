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

#include <stdint.h>      /* For uint8_t definition */
#include <stdio.h>
#include <stdbool.h>      /* For true/false definition */

#endif

#include "I2C.h"
#include "RTC.h"
#include "MISC.h"
#include "ADC.h"

TIME NowTime;

void RTC_INIT(void)
{
    RTC_Clock_Enable();
    RTC_TIME_NONMilitary();
    InternalADC_Read(2);
}
void RTC_Clock_Enable(void)
{
    int tempSeconds =0;
    unsigned char Error=0;

    tempSeconds = I2C_Read_At_Address(RTCaddress, RTCseconds);
    Error = I2C_Write_At_Address( RTCaddress, RTCseconds, (tempSeconds & 0x7F) );

  if(Error  < 0 )
  {
       ResetI2C(); //error
  }
}

void RTC_TIME_NONMilitary(void)
{
    int temphours = 0;
    int Error = -1;

    temphours = I2C_Read_At_Address(RTCaddress, RTChours);
    if(temphours >= 0)
    {
        if((temphours & 0x40) == 0)
        {
            temphours |= 0x40;
            Error = I2C_Write_At_Address( RTCaddress, RTChours, temphours );
        }   
    }
    else
    {
        Error = -1;
    }
  if(Error  < 0 )
  {
       ResetI2C(); //error
  }
}

void RTC_Clock_Disable(void)
{
    int tempSeconds =0;
    unsigned char Error=0;

    tempSeconds = I2C_Read_At_Address(RTCaddress, RTCseconds);
    Error = I2C_Write_At_Address( RTCaddress, RTCseconds, (tempSeconds | 0x80) );

  if(Error  < 0 )
  {
       ResetI2C(); //error
  }
}

unsigned char READ_RTC_TIME(void)
{
    unsigned char Error =0;
    unsigned char temp[7];

    Error = I2C_Read_Sequential( RTCaddress, RTCseconds, temp, 7 );

  if(Error  == 0 )
  {
      NowTime.Seconds = (temp[0] & 0x0F) + (((temp[0] & 0x70)>> 4) * 10);
      NowTime.Minutes = (temp[1] & 0x0F) + (((temp[1] & 0x70) >> 4) * 10);
      NowTime.Hours = (temp[2] & 0x0F) + (((temp[2] & 0x10) >> 4) * 10);
      NowTime.HoursAM_PM = temp[2] & 0x20;
      NowTime.HoursMilitary = NowTime.Hours + 12 * NowTime.HoursAM_PM;
      NowTime.Day = temp[3] & 0x07;
      NowTime.Date = (temp[4] & 0x0F) + (((temp[4] & 0x30)>> 4) * 10);
      NowTime.Month = (temp[5] & 0x0F) + (((temp[5] & 0x10) >> 4) * 10);
      NowTime.Year = (temp[6] & 0x0F) + (((temp[6] & 0xF0) >> 4) * 10);
  }
  else
  {
       ResetI2C(); //error
  }
    return Error;
}
unsigned char SET_RTC_TIME(TIME SetTime)
{
    unsigned char Error =0;
    unsigned char temp[7];

    temp[0] = ((SetTime.Seconds /10) << 4) + (SetTime.Seconds % 10);
    Error = I2C_Write_At_Address( RTCaddress, RTCseconds, temp[0] );
    if(Error)
    {
        return 1;
    }

    temp[1] = ((SetTime.Minutes /10) << 4) + (SetTime.Minutes % 10);
    Error = I2C_Write_At_Address( RTCaddress, RTCminutes, temp[1] );
    if(Error)
    {
        return 2;
    }

    temp[2] = ((SetTime.Hours /10) << 4) + (SetTime.Hours % 10) + (SetTime.HoursAM_PM << 5);
    Error = I2C_Write_At_Address( RTCaddress, RTChours, temp[2] );
    if(Error)
    {
        return 3;
    }

    temp[3] = (SetTime.Day);
    Error = I2C_Write_At_Address( RTCaddress, RTCday, temp[3] );
    if(Error)
    {
        return 4;
    }

    temp[4] = ((SetTime.Date /10) << 4) + (SetTime.Date % 10);
    Error = I2C_Write_At_Address( RTCaddress, RTCdate, temp[4] );
    if(Error)
    {
        return 5;
    }

    temp[5] = ((SetTime.Month /10) << 4) + (SetTime.Month % 10);
    Error = I2C_Write_At_Address( RTCaddress, RTCmonth, temp[5] );
    if(Error)
    {
        return 6;
    }

    temp[6] = ((SetTime.Year /10) << 4) + (SetTime.Year % 10);
    Error = I2C_Write_At_Address( RTCaddress, RTCyear, temp[6] );
    if(Error)
    {
        return 7;
    }

    return 0;
}
void READ_RTC_SECONDS(void)
{
    int tempSeconds =0;

    tempSeconds = I2C_Read_At_Address(RTCaddress, RTCseconds);

  if(tempSeconds  >= 0 )
  {
      NowTime.Seconds = (unsigned char)tempSeconds;
  }
  else
  {
       ResetI2C(); //error
  }
}

void SprintDate(unsigned char* ReturnBuf, TIME DisplayTime, unsigned char Military)
{
    if(!Military)
    {
        if(!DisplayTime.HoursAM_PM)
        {
        sprintf(ReturnBuf,"%02d/%02d/20%02d %02d:%02d:%02d AM", DisplayTime.Month, DisplayTime.Date, DisplayTime.Year, DisplayTime.Hours, DisplayTime.Minutes, DisplayTime.Seconds);
        }
        else
        {
        sprintf(ReturnBuf,"%02d/%02d/20%02d %02d:%02d:%02d PM", DisplayTime.Month, DisplayTime.Date, DisplayTime.Year, DisplayTime.Hours, DisplayTime.Minutes, DisplayTime.Seconds);
        }
    }
    else
    {
        sprintf(ReturnBuf,"%02d/%02d/20%02d %02d:%02d:%02d", DisplayTime.Month, DisplayTime.Date, DisplayTime.Year, DisplayTime.HoursMilitary, DisplayTime.Minutes, DisplayTime.Seconds);
    }
}

unsigned char Parse_Time(unsigned char* This)
{
    TIME TempTime;
    unsigned char temp = 0;

    if(*This != 0 && *This == ' ')
    {
        This++;//remove a space if it is the first character
    }

    //Month
    if (*This >= 48 && *This <= 57)
    {
        temp = *This - 48;
        This++;
    }
    else
    {
        return 0; //first digit of month
    }

    if(*This >= 48 && *This <= 57)
    {
        temp *= 10;
        temp += *This - 48;
        This++;//two digit month
    }
    else if(*This == '/')
    {

    }
    else
    {
        return 0;
    }
    TempTime.Month = temp;

    if(*This == '/')
    {
        This++;// Now go to day
    }
    else
    {
        return 0;
    }

    //day
    if (*This >= 48 && *This <= 57)
    {
        temp = *This - 48;
        This++;
    }
    else
    {
        return 0; //first digit of date
    }

    if(*This >= 48 && *This <= 57)
    {
        temp *= 10;
        temp += *This - 48;
        This++;//two digit date
    }
    else if(*This == '/')
    {

    }
    else
    {
        return 0;
    }
    TempTime.Date = temp;

    if(*This == '/')
    {
        This++;// Now go to day
    }
    else
    {
        return 0;
    }

    //Year
    if (*This >= 48 && *This <= 57)
    {
        temp = *This - 48;
        This++;
    }
    else
    {
        return 0; //first digit of year
    }
    if (*This >= 48 && *This <= 57)
    {
        temp *= 10;
        temp += *This - 48;
        This++;
    }
    else
    {
        return 0; //second digit of year
    }
    TempTime.Year = temp;

    if(*This == ' ')
    {
        This++;//remove a space if it is the first character
    }

    //Hour
    if (*This >= 48 && *This <= 57)
    {
        temp = *This - 48;
        This++;
    }
    else
    {
        return 0; //first digit of hour
    }

    if(*This >= 48 && *This <= 57)
    {
        temp *= 10;
        temp += *This - 48;
        This++;//two digit hour
    }
    else if(*This == ':')
    {

    }
    else
    {
        return 0;
    }
    TempTime.Hours = temp;

    if(*This == ':')
    {
        This++;// Now go to day
    }
    else
    {
        return 0;
    }

    //Minutes
    if (*This >= 48 && *This <= 57)
    {
        temp = *This - 48;
        This++;
    }
    else
    {
        return 0; //first digit of hour
    }

    if(*This >= 48 && *This <= 57)
    {
        temp *= 10;
        temp += *This - 48;
        This++;//two digit hour
    }
    else if(*This == ':')
    {

    }
    else
    {
        return 0;
    }
    TempTime.Minutes = temp;

    if(*This == ':')
    {
        This++;// Now go to day
    }
    else
    {
        return 0;
    }

    //Second
    if (*This >= 48 && *This <= 57)
    {
        temp = *This - 48;
        This++;
    }
    else
    {
        return 0;
    }
    if (*This >= 48 && *This <= 57)
    {
        temp *= 10;
        temp += *This - 48;
        This++;
    }
    else
    {
        return 0; //second digit of year
    }
    TempTime.Seconds = temp;

    if(*This == ' ')
    {
        This++;//remove a space if it is the first character
    }
    if(*This == 'a')
    {
        This++;
        if(*This == 'm')
        {
            TempTime.HoursAM_PM = 0;
        }
        else
        {
            return 0;
        }
    }
    else if(*This == 'p')
    {
        This++;
        if(*This == 'm')
        {
            TempTime.HoursAM_PM = 1;           
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    if(TempTime.Month > 12 || TempTime.Date > 31 || TempTime.Year > 99 || TempTime.Hours > 12 || TempTime.Minutes > 59 || TempTime.Seconds > 59)
    {
        return 0;
    }
    else
    {
        NowTime.Month = TempTime.Month;
        NowTime.Date = TempTime.Date;
        NowTime.Year= TempTime.Year;
        NowTime.Hours = TempTime.Hours;
        NowTime.Minutes = TempTime.Minutes;
        NowTime.Seconds= TempTime.Seconds;
        NowTime.HoursAM_PM = TempTime.HoursAM_PM;
        return 1;
    }
}
