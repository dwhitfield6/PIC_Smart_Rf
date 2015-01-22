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

#include "system.h"

void delayUS(long US);
void cleanBuffer(unsigned char* data, int count);
void BufferCopy(unsigned char* from, unsigned char* to, char count, char shift);
unsigned char StringMatch(unsigned char* This, const unsigned char* That);
unsigned char StringContains(unsigned char* This, const unsigned char* That);
unsigned char StartsWith(unsigned char* This, const unsigned char* That);
int GetEnteredNumber(unsigned char* This);
void RemoveSpaces(unsigned char* This);
void lowercase(unsigned char* Input);
unsigned char StringAddEqual(unsigned char* Input);
unsigned char CheckSum_byte(unsigned int This);
