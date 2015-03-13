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
/* Contains functions that control the UART.
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

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "UART.h"          /* User funct/params, such as InitApp */
#include "MISC.h"          /* User funct/params, such as InitApp */
#include "EEPROM.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Defines                                                         */
/******************************************************************************/

#define CloseUSART( ) RCSTA&=0b01001111,TXSTAbits.TXEN=0,PIE1&=0b11001111

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

unsigned char Txdata[100];
unsigned char Rxdata[100];
unsigned char TX_OLD =0;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* InitUART
 *
 * The function initializes the UART. The UART is started with baud and parity
 *   settings.
/******************************************************************************/
void InitUART(unsigned long Baud, unsigned char parity)
{
    unsigned long temp;
    unsigned char config;
    unsigned int spbrg;
    unsigned char baudconfig;
    /*
     * parity options
     * 0 is none
     * 1 is Odd
     * 2 is Even
     * 3 is mark
     * 4 is space
     */

    CloseUSART(); //turn off usart if was previously on
    config =0;
    baudconfig =0;
    BAUDCON =0;

     //-----configure USART -----

    config |= _USART_RX_INT_ON;
    config |= _USART_CONT_RX;
    config |= _USART_ASYNCH_MODE;
    config |= _USART_BRGH_HIGH;

    if(parity)
    {
        config |= _USART_NINE_BIT;
    }
    else
    {
        config &= ~_USART_NINE_BIT;
    }
    baudconfig |= _USART_BRG16;

    //calculate the spbrg from the baud rate.
    temp = (SYS_FREQ / Baud) - 1;
    temp = (temp >> 2) - 1;
    spbrg = (unsigned int)temp;

    OpenUSART(config, spbrg);
    baudUSART(baudconfig);
    temp = ReadUSART(); //clear the buffer
    temp = ReadUSART(); //clear the buffer
    temp = ReadUSART(); //clear the buffer
    temp = ReadUSART(); //clear the buffer
    temp = ReadUSART(); //clear the buffer

    IPR1bits.RCIP = 0; //Not high priority
}

/******************************************************************************/
/* baudUSART
 *
 * The function sets up the special functions registers to set up the Baud.
/******************************************************************************/
void baudUSART (unsigned char baudconfig)
{
    BAUDCON =0;
    BAUDCON = baudconfig;
}

/******************************************************************************/
/* OpenUSART
 *
 * The function sets up the UART transmitter and receiver.
/******************************************************************************/
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
  {
    PIE1bits.RCIE = 1;
  }
  else
  {
    PIE1bits.RCIE = 0;
  }

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

/******************************************************************************/
/* UARTchar
 *
 * The function sends one character over the UART.
/******************************************************************************/
void UARTchar(unsigned char data, unsigned char NinethBit_override, unsigned char NinethBit_override_data)
{
    if(TXSTAbits.TX9)
    {
        // 9-bit mode
        if(!NinethBit_override)
        {
            //no override 9th bit so we will calculate it
            switch (Global.Parity)
            {
                case 1:
                    TXSTAbits.TX9D = CheckSum_byte((unsigned int) data, Odd);//Odd parity
                    break;
                case 2:
                    TXSTAbits.TX9D = CheckSum_byte((unsigned int) data, Even);//Even parity
                    break;
                case 3:
                    TXSTAbits.TX9D = 1;//mark
                    break;
                default:
                    TXSTAbits.TX9D = 0;//space
                    break;
            }
        }
        else
        {
            TXSTAbits.TX9D = (NinethBit_override_data & 0x01);
        }
    }
    TX_OLD = data; //previous rx
    TXREG = data;      // Write the data byte to the USART
    while(!TXSTAbits.TRMT);
}

/******************************************************************************/
/* UARTstring
 *
 * The function sends a group of characters over the UART.
/******************************************************************************/
void UARTstring(const unsigned char *data)
{
    do
    {  // Transmit a byte
        if(*data != 0)
        {
            UARTchar((unsigned char) *data, NO, 0);
            while(BusyUSART());
        }
    } while( *data++);
}

/******************************************************************************/
/* ReadUSART
 *
 * The function reads the UART and returns the data read.
/******************************************************************************/
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

/******************************************************************************/
/* UART_send_break
 *
 * The function sends the break character over the UART. This break is always
 *   12 bits of 0.
/******************************************************************************/
void UART_send_break(void)
{
    TXSTAbits.SENDB =1;
    TXREG = 'd';      // arbitrary
}

/******************************************************************************/
/* UARTstringWAIT
 *
 * The function sends a group of characters over the UART. There is a wait of
 *   the character_spacing value between character sends.
/******************************************************************************/
void UARTstringWAIT(const unsigned char *data)
{
  do
  {  // Transmit a byte
      delayUS(Character_Spacing);
      if(*data != 0)
      {
    UARTchar((unsigned char) *data, NO, 0);
    while(BusyUSART());
      }
  } while( *data++);
}

/******************************************************************************/
/* SetBaud
 *
 * The function is called when a new baud rate is to be saved to internal
 *   EEPROM. It also sets the baud rate and parity setting by calling
 *   InitUART();
/******************************************************************************/
void SetBaud(unsigned long Baud, unsigned char Parity)
{
    unsigned char buf[50];
    unsigned char status=0;
    //Program Baud to Flash
    if(Parity)
    {
        switch (Parity)
        {
            case 1:
                sprintf(buf,"System Baud will be set to %lu with Odd parity bit\r\n",Baud);//Odd parity
                break;
            case 2:
                sprintf(buf,"System Baud will be set to %lu with Even parity bit\r\n",Baud);//Even parity
                break;
            case 3:
                sprintf(buf,"System Baud will be set to %lu with Mark bit\r\n",Baud);//mark
                break;
            default:
                sprintf(buf,"System Baud will be set to %lu with Space bit\r\n",Baud);//Space
                break;
        }
    }
    else
    {
        sprintf(buf,"System Baud will be set to %lu with no parity bit\r\n",Baud);
    }

    status = SetMemoryBaud(Baud);
    if(status)
    {
        //Baud write passed
        status = SetMemoryParity(Parity);
    }
    InitUART(Baud, Parity);
    UARTstring("\r\n");
    delayUS(Word_Spacing);
    if(!status)
    {
        UARTstring("System Program Fail\r\n");
        delayUS(Word_Spacing);
    }
    sprintf(buf,"System Baud is %lu",Baud);
    UARTstring(buf);
    if(Parity)
    {
        switch (Parity)
        {
            case 1:
                UARTstringWAIT(" with Odd parity bit\r\n");//Odd parity
                break;
            case 2:
                UARTstringWAIT(" with Even parity bit\r\n");//Even parity
                break;
            case 3:
                UARTstringWAIT(" with Mark bit\r\n");//mark
                break;
            default:
                UARTstringWAIT(" with Space bit\r\n");//Space
                break;
        }
    }
    else
    {
        UARTstring(" with no parity bit\r\n");
    }
    
    delayUS(Word_Spacing);
}

/******************************************************************************/
/* GetBaud
 *
 * The function returns the Baud rate
/******************************************************************************/
unsigned long GetBaud(void)
{
    return Global.Baud;
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/