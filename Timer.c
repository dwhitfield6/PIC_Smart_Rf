/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/11/15     1.0         Initial Coding.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for Timers.
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

#include "user.h"          /* User funct/params, such as InitApp */
#include "IR.h"          /* User funct/params, such as InitApp */
#include "Timer.h"          /* User funct/params, such as InitApp */

/* Timer 2 is used for the PWM */

/******************************************************************************/
/* init_Timers
 *
 * The function initializes the timers.
/******************************************************************************/
void init_Timers(void)
{
    /* Initialize timer 0 */
    init_Timer0();
    /* Initialize timer 1 */
    init_Timer1();
}

/******************************************************************************/
/* init_Timer0
 *
 * The function initializes Timer0 wich is used for the IR receiver
/******************************************************************************/
void init_Timer0(void)
{
    Stop_Timer0();
    /* Low priority */
    INTCON2bits.T0IP = 0;
    /* 16 bits */
    T0CONbits.T08BIT = 0;
    /* clocked by FOSC/4 */
    T0CONbits.T0CS = 0;
    /* Turn on prescaler */
    T0CONbits.PSA = 0;
    /* prescaler 32*/
    T0CONbits.T0PS = 0x4;

}

/******************************************************************************/
/* Stop_Timer0
 *
 * The function starts timer 0.
/******************************************************************************/
void Stop_Timer0(void)
{
    /*turn off timer 0*/
    T0CONbits.TMR0ON =0;
}

/******************************************************************************/
/* Start_Timer0
 *
 * The function starts timer 0.
/******************************************************************************/
void Start_Timer0(void)
{
    /*turn on timer 0*/
    T0CONbits.TMR0ON =1;
}

/******************************************************************************/
/* Reset_Timer0
 *
 * The function resets the counter on Timer 0.
/******************************************************************************/
void Reset_Timer0(void)
{
    /*
     * Reset count to 2 because when the TMR0 register is written to,
     * the increment is inhibited for the following two instruction cycles
     */
    TMR0H = 0;
    TMR0L = 2;
}


/******************************************************************************/
/* init_Timer1
 *
 * The function initializes Timer1 which is used for the LCD timer.
/******************************************************************************/
void init_Timer1(void)
{
    /* 16 bits */
    T1CONbits.RD16 = 1;
    /* Source is FOSC/4 */
    T1CONbits.TMR1CS = 0;
    /* Prescaler is 8 */
    T1CONbits.T1CKPS = 0x3;
    /* Low priority Interrupt */
    IPR1bits.TMR1IP = 0;
    Stop_Timer1();
}

/******************************************************************************/
/* Stop_Timer1
 *
 * The function starts timer 1.
/******************************************************************************/
void Stop_Timer1(void)
{
    /*turn off timer 1*/
    T1CONbits.TMR1ON =0;
    /* Interrupt Off */
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 0;
}

/******************************************************************************/
/* Start_Timer1
 *
 * The function starts timer 1.
/******************************************************************************/
void Start_Timer1(void)
{
    /*turn on timer 1*/
    T1CONbits.TMR1ON =1;
    /* Reset Timer */
    Reset_Timer1();
    /* Interrupt On */
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
}

/******************************************************************************/
/* Reset_Timer1
 *
 * The function resets the counter on Timer 1.
/******************************************************************************/
void Reset_Timer1(void)
{
    /*
     * Reset count to 2 because when the TMR0 register is written to,
     * the increment is inhibited for the following two instruction cycles
     */
    TMR1H = 0;
    TMR1L = 2;
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/