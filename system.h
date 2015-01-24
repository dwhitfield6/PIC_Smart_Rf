/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/21/15     1.0         Created log.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

/******************************************************************************/
/* System frequency
 *
 * This is the CPU clock frequency.
 *
 * For this system the clock is 41 MHz and the PLL is used. The crytals value is
 *   10.240MHz. Clock freq is crystal * 4 for the PLL.
 *
/******************************************************************************/

#define SYS_FREQ        40960000L

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define OSTS 0b00001000
#define FCY             SYS_FREQ/4
#define FOSC             FCY

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
