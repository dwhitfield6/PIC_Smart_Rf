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

#include "PWM.h"
#include "user.h"
#include "system.h"

void PWM_init()
{
    //this is for CCP1
    SetFreq(60);
    SetDuty(0);
    T2CON &= ~0x03;
    if(TMR2_Prescale == 1)
    {
        NOP();
    }
    else if (TMR2_Prescale == 4)
    {
        T2CON |= 0x01;
    }
    else
    {
        T2CON |= 0x10; // prescaler is 16
    }
    T2CON |= TMR2ON;
    CCP1CON |= 0b00001100; //pwm mode
}
void ContrastPWM_init()
{
    //this is for ECCP1
    //PWM init must be called first
    SetDuty(0);
    ECCP1CON |= 0b00001100; //Single pwm mode
}
void SetContrast(unsigned char Duty)
{
    unsigned int temp=0;
    float tempPR2=0;
    unsigned char MSB,LSB;

    Duty = 100 - Duty; //inverse relationship
    Duty = (Duty>>1);//scale down by 2

    tempPR2 = (float) PR2;

    temp = (int)(tempPR2 * ((float)Duty/100));
    MSB = (temp & 0x300)>>8;
    LSB = temp & 0xff;

    ECCP1CON &= ~0b00110000;
    ECCP1CON |= MSB << 4;
    ECCPR1L = LSB;
    NOP();
}
void SetDuty(unsigned char Duty)
{
    unsigned int temp=0;
    float tempPR2=0;
    unsigned char MSB,LSB;

    tempPR2 = (float) PR2;

    temp = (int)(tempPR2 * ((float)Duty/100));
    MSB = (temp & 0x300)>>8;
    LSB = temp & 0xff;

    CCP1CON &= ~(0b00110000);
    CCP1CON |= MSB << 4;
    CCPR1L = LSB;
    NOP();

}
void SetFreq(unsigned int freq)
{
    //lowest number is 39.25 kHz
    unsigned char temp =0;
    temp = (unsigned char)(((TOSC >> 2)/(freq*1000))-1);
    //temp = (unsigned char)((float)period /(4*(float)TOSC*(float)TMR2_Prescale))-1;
    PR2 = temp;
}