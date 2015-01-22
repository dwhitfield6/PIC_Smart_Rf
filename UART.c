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

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "UART.h"          /* User funct/params, such as InitApp */

#define CloseUSART( ) RCSTA&=0b01001111,TXSTAbits.TXEN=0,PIE1&=0b11001111



unsigned char Txdata[100];
unsigned char Rxdata[100];


void InitUART(void)
{
int temp;
unsigned char config;
unsigned int spbrg;
unsigned char baudconfig;
CloseUSART(); //turn off usart if was previously on
config =0;
baudconfig =0;
     //-----configure USART -----
config |= USART_TX_INT_OFF;
config |= USART_RX_INT_ON;
config |= USART_ASYNCH_MODE;
config |= USART_EIGHT_BIT;
config |= USART_CONT_RX;//single mode
config |= USART_BRGH_LOW;
spbrg = 66;
BAUDCON =0;
//baudconfig |= BAUD_8_BIT_RATE;
//baudconfig |= BAUD_AUTO_OFF;
//baudconfig |= BAUD_WAKEUP_ON;
OpenUSART(config, spbrg);
baudUSART(baudconfig);
temp = ReadUSART(); //clear the buffer
temp = ReadUSART(); //clear the buffer
temp = ReadUSART(); //clear the buffer
temp = ReadUSART(); //clear the buffer
temp = ReadUSART(); //clear the buffer
    //compare with the table above
    IPR1bits.RCIP = 0; //Not high priority
    PIE1bits.RCIE = 1; //Enable RX interrupt
    PIR1bits.RCIF = 0; //reset RX pin flag    PIR1bits.RCIF = 0; //reset RX pin flag
}

void baudUSART (unsigned char baudconfig)
{
    BAUDCON = baudconfig;
}

void OpenUSART( unsigned char config, unsigned int spbrg)
{
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;

  if(config&0x01)      // Sync or async operation
    TXSTAbits.SYNC = 1;

  if(config&0x02)      // 8- or 9-bit mode
  {
    TXSTAbits.TX9 = 1;
    RCSTAbits.RX9 = 1;
  }

  if(config&0x04)      // Master or Slave (sync only)
    TXSTAbits.CSRC = 1;

  if(config&0x08)      // Continuous or single reception
    RCSTAbits.CREN = 1;
  else
    RCSTAbits.SREN = 1;

  if(config&0x10)      // Baud rate select (asychronous mode only)
    TXSTAbits.BRGH = 1;

    PIR1bits.TXIF = 0;

  if(config&0x20)  // Address Detect Enable
	 RCSTAbits.ADEN = 1;

  if(config&0x40)      // Interrupt on receipt
    PIE1bits.RCIE = 1;
  else
    PIE1bits.RCIE = 0;

  PIR1bits.RCIF = 0;

  if(config&0x80)      // Interrupt on transmission
    PIE1bits.TXIE = 1;
  else
    PIE1bits.TXIE = 0;

  SPBRG = spbrg;       // Write baudrate to SPBRG1
  SPBRGH = spbrg >> 8; // For 16-bit baud rate generation

  TXSTAbits.TXEN = 1;  // Enable transmitter
  RCSTAbits.SPEN = 1;  // Enable receiver
}

void UARTchar(unsigned char data)
{
  if(TXSTAbits.TX9)  // 9-bit mode?
  {
    TXSTAbits.TX9D = 0;       // Set the TX9D bit according to the
    if(USART_Status.TX_NINE)  // USART Tx 9th bit in status reg
      TXSTAbits.TX9D = 1;
  }

  TXREG = data;      // Write the data byte to the USART
}

void UARTstring(const unsigned char *data)
{
  do
  {  // Transmit a byte
      if(*data != 0)
      {
    putcUSART(*data);
    while(BusyUSART());
      }
  } while( *data++);
}

char ReadUSART(void)		//this function can be removed by macro #define ReadUSART RCREG
{
  char data;   // Holds received data

  USART_Status.val &= 0xf2;          // Clear previous status flags

  if(RCSTAbits.RX9)                  // If 9-bit mode
  {
    USART_Status.RX_NINE = 0;        // Clear the receive bit 9 for USART
    if(RCSTAbits.RX9D)               // according to the RX9D bit
      USART_Status.RX_NINE = 1;
  }

  if(RCSTAbits.FERR)                 // If a framing error occured
    USART_Status.FRAME_ERROR = 1;    // Set the status bit

  if(RCSTAbits.OERR)                 // If an overrun error occured
    USART_Status.OVERRUN_ERROR = 1;  // Set the status bit

  data = RCREG;                      // Read data

  return (data);                     // Return the received data
}