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

#define TOSC            SYS_FREQ
#define TMR2_Prescale   16
#define TMR2ON          0b00000100

void PWM_init();
void SetDuty(unsigned char Duty);
void SetFreq(unsigned int freq);
void SetContrast(unsigned char Duty);
void ContrastPWM_init();
