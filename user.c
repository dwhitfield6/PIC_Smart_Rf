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
/* Contains initialization functions.
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
#include "MISC.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* InitApp
 *
 * The function initializes the application. It also sets up the interrupts.
/******************************************************************************/
void InitApp(void)
{
    TRISD.LED0_TRIS = OUTPUT;
    TRISD.LED1_TRIS = OUTPUT;
    TRISD.RFtx_TRIS = OUTPUT;
    TRISA.POT_IN_Analog_TRIS = INPUT;
    TRISC.BiLEDred_TRIS = OUTPUT;
    TRISC.BiLEDgreen_TRIS = OUTPUT;
    TRISE.POT_IN_Digital_TRIS = INPUT;
    TRISE.POT_ON_TRIS = OUTPUT;

    I2C_SCL = OUTPUT;
    I2C_SDA = OUTPUT;

    LATC &= ~I2Cclock;
    delayUS(200);
    I2C_SCL = INPUT;

    LATD &= ~(LED0 + LED1 + RFtx);
    LATE &= ~POT_ON;
    LATC &= ~(BiLEDgreen);
    
    //LCD   
    TRISA.D7_TRIS = OUTPUT;
    TRISA.D6_TRIS = OUTPUT;
    TRISA.D5_TRIS = OUTPUT;
    TRISE.D4_TRIS = OUTPUT;
    TRISD.Contrast_TRIS = OUTPUT;
    TRISD.Enable_TRIS = OUTPUT;
    TRISD.RW_TRIS = OUTPUT;
    TRISD.RS_TRIS = OUTPUT;

    LATA &= ~(D7+D6+D5);
    LATD &= ~(Enable + RS + RW);
    LATE &= ~D4;

    //IR
    IR_INPUT_TRIS = INPUT;

    //PushBotton
    Button1_TRIS = INPUT;

    RCONbits.IPEN = 1; //enable priority interrupts
    INTCONbits.PEIE = 1; //Enable pheripheral interrupt
    INTCONbits.GIE = 1; //global interrupts enabled 
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
