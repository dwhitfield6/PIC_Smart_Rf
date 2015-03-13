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
/* Contains functions that control the RF transmitter.
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

#include "user.h"          /* User funct/params, such as InitApp */
#include "RF.h"          /* User funct/params, such as InitApp */
#include "MISC.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Send_Config1
 *
 * The function sends a data array of configuration 1.
/******************************************************************************/
void Send_Config1( unsigned char pin,const unsigned char* data)
{
    unsigned char i=0;

    for(i=0;i<12;i++)
    {
            if(data[i] == 0)
            {
                    Zero_Config1(pin);
            }
            else if(data[i] ==1)
            {
                    One_Config1(pin);
            }
            else
            {
                    f_Config1(pin);
            }
    }

    LATD |= pin;
    delayUS(SYNCshortCONF1);
    LATD &= ~pin;
    delayUS(SYNClongCONF1);

}

/******************************************************************************/
/* Zero_Config1
 *
 * The function sends a configuration 1 Zero waveform over the RF pin.
/******************************************************************************/
void Zero_Config1(unsigned char pin)
{
	LATD |= pin;
	delayUS(ZEROshortCONF1 );
	LATD &= ~pin;
	delayUS(ZEROlongCONF1 );
	LATD |= pin;
	delayUS(ZEROshortCONF1 );
	LATD &= ~pin;
	delayUS(ZEROlongCONF1 );
}

/******************************************************************************/
/* One_Config1
 *
 * The function sends a configuration 1 One waveform over the RF pin.
/******************************************************************************/
void One_Config1(unsigned char pin)
{
	LATD |= pin;
	delayUS(ONElongCONF1);
	LATD &= ~pin;
	delayUS(ONEshortCONF1);
	LATD |= pin;
	delayUS(ONElongCONF1);
	LATD &= ~pin;
	delayUS(ONEshortCONF1);
}

/******************************************************************************/
/* f_Config1
 *
 * The function sends a configuration 1 float waveform over the RF pin.
/******************************************************************************/
void f_Config1(unsigned char pin)
{
	LATD |= pin;
	delayUS(FshortCONF1);
	LATD &= ~pin;
	delayUS(FlongCONF1);
	LATD |= pin;
	delayUS(FlongCONF1);
	LATD &= ~pin;
	delayUS(FshortCONF1);
}

/******************************************************************************/
/* Send_Config2
 *
 * The function sends a data array of configuration 2.
/******************************************************************************/
void Send_Config2( unsigned char pin,const unsigned char* data)
{
    unsigned char i=0;

    for(i=0;i<16;i++)
    {
            if(data[i] == 0)
            {
                    Zero_Config2(pin);
            }
            else
            {
                    One_Config2(pin);
            }
    }

    LATD &= ~pin;
    delayUS(SYNClongCONF2);

}

/******************************************************************************/
/* Zero_Config2
 *
 * The function sends a configuration 2 Zero waveform over the RF pin.
/******************************************************************************/
void Zero_Config2(unsigned char pin)
{
	LATD |= pin;
	delayUS(ZEROshortCONF2);
	LATD &= ~pin;
	delayUS(ZEROlongCONF2);
}

/******************************************************************************/
/* One_Config2
 *
 * The function sends a configuration 2 One waveform over the RF pin.
/******************************************************************************/
void One_Config2(unsigned char pin)
{
	LATD |= pin;
	delayUS(ONElongCONF2);
	LATD &= ~pin;
	delayUS(ONEshortCONF2);
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
