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

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define TOSC            SYS_FREQ
#define TMR2_Prescale   16
#define TMR2ON          0b00000100


/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void PWM_init();
void SetDuty(unsigned char Duty);
void SetFreq(unsigned int freq);
void SetContrast(unsigned char Duty);
void ContrastPWM_init();
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/