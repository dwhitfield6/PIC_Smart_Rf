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

#include "user.h"          /* User funct/params, such as InitApp */
#include "RF.h"          /* User funct/params, such as InitApp */
#include "MISC.h"          /* User funct/params, such as InitApp */

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

void Zero_Config2(unsigned char pin)
{
	LATD |= pin;
	delayUS(ZEROshortCONF2);
	LATD &= ~pin;
	delayUS(ZEROlongCONF2);
}

void One_Config2(unsigned char pin)
{
	LATD |= pin;
	delayUS(ONElongCONF2);
	LATD &= ~pin;
	delayUS(ONEshortCONF2);
}

