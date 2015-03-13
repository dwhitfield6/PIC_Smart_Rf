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
/* Contains functions that control the internal ADC.
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

#include "ADC.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "UART.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* EnableInternalADC
 *
 * The function initializes the internal ADCs.
/******************************************************************************/
void EnableInternalADC(unsigned char channels)
{
    unsigned char config =0;
         //-----configure USART -----
    if(channels == 1)
    {
        config |= ADC_1ANA_0REF;
    }
    else if(channels ==2)
    {
        config |= ADC_2ANA_0REF;
    }
    else if(channels ==3)
    {
        config |= ADC_3ANA_0REF;
    }
    else if(channels ==4)
    {
        config |= ADC_4ANA_0REF;
    }
    else if(channels ==5)
    {
        config |= ADC_5ANA_0REF;
    }
    else if(channels ==6)
    {
        config |= ADC_6ANA_0REF;
    }
    else if(channels ==7)
    {
        config |= ADC_7ANA_0REF;
    }
    else if(channels ==8)
    {
        config |= ADC_8ANA_0REF;
    }
    else if(channels ==9)
    {
        config |= ADC_9ANA_0REF;
    }
    else if(channels ==10)
    {
        config |= ADC_10ANA_0REF;
    }
    else
    {
        config |= ADC_11ANA_0REF;
    }

    ADCON1 = config;
}

/******************************************************************************/
/* InternalADC_Read
 *
 * The function reads the internal ADC channel.
/******************************************************************************/
int InternalADC_Read(unsigned char channel)
{
    ADCON0 =0;
    if(channel > 10)
    {
        UARTstring("ADC Channel Out of Bounds");
        return 0;
    }
    ADCON0 |= (channel << 2);
    ADCON2 |= 0b10100000; //right justified 8TAD
    ADCON2 |= 0b00000110; //and FOSC/64
    ADCON0 |= ADON;
    ADC_INT_DISABLE();
    ADCON0 |= ADC_GO; //GO
    while(ADCON0 & ADC_GO);
    return ((ADRESH << 8) + ADRESL);
}

/******************************************************************************/
/* DisableInternalADC
 *
 * The function disables the internal ADCs.
/******************************************************************************/
void DisableInternalADC(void)
{
    ADCON0 &= ~ADON;
    ADCON1 = ADC_0ANA_0REF;
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/