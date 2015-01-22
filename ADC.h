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


#define USE_OR_MASKS

#define ADC_INT_ENABLE()     (PIR1bits.ADIF=0,INTCONbits.PEIE=1,PIE1bits.ADIE=1)
#define ADC_INT_DISABLE()    (PIE1bits.ADIE=0)
#define ADC_0ANA_0REF    0b00001111
#define ADC_1ANA_0REF    0b00001110
#define ADC_2ANA_0REF    0b00001101
#define ADC_3ANA_0REF    0b00001100
#define ADC_4ANA_0REF    0b00001011
#define ADC_5ANA_0REF    0b00001010
#define ADC_6ANA_0REF    0b00001001
#define ADC_7ANA_0REF    0b00001000
#define ADC_8ANA_0REF    0b00000111
#define ADC_9ANA_0REF    0b00000110
#define ADC_10ANA_0REF   0b00000101
#define ADC_11ANA_0REF   0b00000100
#define ADC_GO   0b00000010
#define ADON    0b00000001

void EnableInternalADC(unsigned char channels);
void DisableInternalADC(void);
int InternalADC_Read(unsigned char channel);
