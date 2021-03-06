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
/* Defines                                                                    */
/******************************************************************************/

#define OUTPUT 0
#define INPUT 1
#define FALSE 0
#define TRUE 1

/******************************************************************************/
/* Pin definitions                                                            */
/******************************************************************************/
#define RFtx 0b00000001 //D0 315 MHz transmitter
#define LED0 0b00000010 //D1 blue led
#define LED1 0b00000100 //D2 blue led
#define POT_ON 0b00000010 //E1 Potentiometer power
#define BiLEDred 0b00000100 //RC2
#define BiLEDgreen 0b00000010 //RC1

#define LED1_TRIS TRISD2
#define LED0_TRIS TRISD1
#define RFtx_TRIS TRISD0
#define BiLEDred_TRIS TRISC2
#define BiLEDgreen_TRIS TRISC1
#define POT_IN_Digital_TRIS TRISE2
#define POT_IN_Analog_TRIS TRISA0
#define POT_ON_TRIS TRISE1
//LCD
#define Contrast_TRIS TRISD4//contrast VO is D4
#define Enable_TRIS TRISD7//Enable is D7
#define RW_TRIS TRISD6//R/W is D6
#define RS_TRIS TRISD5//RS is D5
#define D7_TRIS TRISA3
#define D6_TRIS TRISA4
#define D5_TRIS TRISA5
#define D4_TRIS TRISE0
#define D7 0b00001000 //A3
#define D6 0b00010000 //A4
#define D5 0b00100000 //A5
#define D4 0b00000001 //E0
#define Enable 0b10000000 //RD7
#define RW 0b01000000 //RD6
#define RS 0b00100000 //RD5

#define I2C_SCL	TRISCbits.TRISC3
#define I2C_SDA	TRISCbits.TRISC4
#define I2Cclock 0b00001000 //RC3

#define IR_INPUT_TRIS TRISBbits.TRISB4
#define IR_INPUT 0b00010000 //RB4 used as KBI0

#define Button1_TRIS TRISBbits.TRISB0
#define Button1 0b00000001 //RB0 used as INT0

#define TRISA TRISAbits
#define TRISB TRISBbits
#define TRISC TRISCbits
#define TRISD TRISDbits
#define TRISE TRISEbits
#define TRISF TRISFbits

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
