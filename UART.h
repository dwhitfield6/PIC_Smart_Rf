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

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Uart character and word spacing
 *
 * This parameter modifies the amount of time waited between characters and
 *   words when printing over the UART.
/******************************************************************************/

#define Character_Spacing   5000

/*
 * A typical Character_Spacing value for fast printing is 5000. For slow
 *   printing 200,000 is common.
 */

#define Word_Spacing        10000

/******************************************************************************/
/* default Baud rate
 *
 * This is the baud rate at first start up after programming or after
 *  catastrophic fault.
/******************************************************************************/

#define defaultBaud         9600

/******************************************************************************/
/* Defines                                                         */
/******************************************************************************/

#define NONE    0
#define ODD     1
#define EVEN    2
#define MARK    3
#define SPACE   4
#define INVALID 5

#define _USART_BRG16     0b00001000
#define YES                 1
#define NO                  0

#define _USART_TX_INT_ON   		0b10000000  // Transmit interrupt on
#define _USART_TX_INT_OFF  		0b00000000  // Transmit interrupt off
#define _USART_TX_INT_MASK		(~_USART_TX_INT_ON)	//Mask Trnasmit Interrupt select bit

#define _USART_RX_INT_ON   		0b01000000  // Receive interrupt on
#define _USART_RX_INT_OFF  		0b00000000  // Receive interrupt off
#define _USART_RX_INT_MASK		(~_USART_RX_INT_ON)	//Mask Receive Interrupt select bit

#define _USART_ADDEN_ON    		0b00100000  // Enables address detection
#define _USART_ADDEN_OFF   		0b00000000  // Disables address detection
#define _USART_ADDEN_MASK		(~_USART_ADDEN_ON)	//Mask address detection select bit

#define _USART_BRGH_HIGH   		0b00010000  // High baud rate
#define _USART_BRGH_LOW    		0b00000000  // Low baud rate
#define _USART_BRGH_MASK			(~_USART_BRGH_HIGH)	//Mask baud rate select bit

#define _USART_CONT_RX     		0b00001000  // Continuous reception
#define _USART_SINGLE_RX   		0b00000000  // Single reception
#define _USART_CONT_RX_MASK		(~_USART_CONT_RX) 	//Mask Continuous Reception select bit

#define _USART_SYNC_MASTER 		0b00000100  // Synchrounous master mode
#define _USART_SYNC_SLAVE  		0b00000000  // Synchrounous slave mode
#define _USART_SYNC_MASK			(~_USART_SYNC_MASTER)	//Mask usart mode select bit

#define _USART_NINE_BIT    		0b00000010  // 9-bit data
#define _USART_EIGHT_BIT   		0b00000000  // 8-bit data
#define _USART_BIT_MASK 			(~_USART_NINE_BIT)		//Mask 9 bit transmit  select bit

#define _USART_SYNCH_MODE  		0b00000001  // Synchronous mode
#define _USART_ASYNCH_MODE 		0b00000000  // Asynchronous mode
#define _USART_MODE_MASK			(~_USART_SYNCH_MODE) 	//Mask sync/async mode select bit

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void InitUART(unsigned long Baud, unsigned char parity);
void UARTstring(const unsigned char *data);
void UARTstringWAIT(const unsigned char *data);
void UARTchar(unsigned char data, unsigned char NinethBit_override, unsigned char NinethBit_override_data);
void OpenUSART( unsigned char config, unsigned int spbrg);
char ReadUSART(void);
void baudUSART (unsigned char baudconfig);
void OpenUSART( unsigned char config, unsigned int spbrg);
void SetBaud(unsigned long Baud, unsigned char Parity);
unsigned long GetBaud(void);
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
