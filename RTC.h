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

#include "I2C.h"

#define RTCaddress 0b1101000

#define RTCseconds 0x00
#define RTCminutes 0x01
#define RTChours 0x02
#define RTCday 0x03
#define RTCdate 0x04
#define RTCmonth 0x05
#define RTCyear 0x06

//RTC external memory ends at 0x3F
#define RTCpage0 0x08

extern struct TIME NowTime;

typedef struct TIME{
  unsigned char Seconds;
  unsigned char Minutes;
  unsigned char Hours;
  unsigned char HoursMilitary;
  unsigned char HoursAM_PM;
  unsigned char Day;
  unsigned char Date;
  unsigned char Month;
  unsigned char Year;
}TIME ;

unsigned char READ_RTC_TIME(void);
void RTC_Clock_Disable(void);
void RTC_Clock_Enable(void);
void SprintDate(unsigned char* ReturnBuf, TIME DisplayTime, unsigned char Military);
void RTC_TIME_NONMilitary(void);
void RTC_INIT(void);
unsigned char SET_RTC_TIME(TIME SetTime);
unsigned char Parse_Time(unsigned char* This);