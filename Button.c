/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/12/15     1.0         Initial Coding.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the pushbotton switch.
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

#include "user.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* init_Button1_INT
 *
 * The function initializes the pushbutton.
/******************************************************************************/
void init_Button1_INT(void)
{
    /* Turn on port B pullups */
    INTCON2bits.RBPU = 0;
    /* Rising edge */
    INTCON2bits.INTEDG0 = 1;
    /* Turn on INT0 external interrupt */
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
}

/******************************************************************************/
/* ReadButton1
 *
 * The function reads the pushbutton.
/******************************************************************************/
unsigned char ReadButton1(void)
{
    //READ RB0
    if((PORTB & Button1) == Button1)
    {
        return (1);
    }
    else
    {
        return 0;
    }
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/