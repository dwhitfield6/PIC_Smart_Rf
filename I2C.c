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
/* Contains functions for I2C.
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

#endif

#include "I2C.h"        /* System funct/params, like osc/peripheral config */
#include "system.h"
#include "user.h"
#include "MISC.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_I2C_Master
 *
 * The function initializes the I2C bus. This function initializes the system
 *   as an I2C master.
/******************************************************************************/
void Init_I2C_Master(void)
{
    unsigned int baud = 0;
    
    //clock = FOSC/(4 * (SSPADD + 1))
    baud = (unsigned char)(FOSC /((4* (long)I2Cbaud * 1000L) - 1));
    if(baud > 127 || baud == 0)
    {
        baud = 127;
    }
    SSPADD = baud;
    OpenI2C(MASTER, SLEW_OFF);
}

/******************************************************************************/
/* OpenI2C
 *
 * The function activates teh I2C bus.
/******************************************************************************/
void OpenI2C( unsigned char sync_mode, unsigned char slew )
{
  SSPSTAT &= 0x3F;                // power on state
  SSPCON1 = 0x00;                 // power on state
  SSPCON2 = 0x00;                 // power on state
  SSPCON1 |= sync_mode;           // select serial mode
  SSPSTAT |= slew;                // slew rate on/off

  I2C_SCL = INPUT;
  I2C_SDA = INPUT;
  SSPCON1 |= SSPENB;              // enable synchronous serial port

}

/******************************************************************************/
/* ResetI2C
 *
 * The function resets the I2C bus. It "clocks" through the problem.
/******************************************************************************/
void ResetI2C( void )
{
    SSPCON1 &= 0xDF;                // disable synchronous serial port
    I2C_SCL = OUTPUT;
    LATC &= ~I2Cclock;
    delayUS(200);
    I2C_SCL = INPUT;
    Init_I2C_Master();
}

/******************************************************************************/
/* IdleI2C
 *
 * The function waits until the bus is idle.
/******************************************************************************/
void IdleI2C( void )
{
  while ( ( SSPCON2 & 0x1F ) || ( SSPSTATbits.R_W ) )
     continue;
}

/******************************************************************************/
/* WriteI2C
 *
 * The function sends a byte over the I2C bus.
/******************************************************************************/
signed char WriteI2C( unsigned char data_out )
{
  SSPBUF = data_out;           // write single byte to SSPBUF
  if ( SSPCON1bits.WCOL )      // test if write collision occurred
   return ( -1 );              // if WCOL bit is set return negative #
  else
  {
	if( ((SSPCON1&0x0F)!=0x08) && ((SSPCON1&0x0F)!=0x0B) )	//Slave mode only
	{
	      SSPCON1bits.CKP = 1;        // release clock line
	      while ( !PIR1bits.SSPIF );  // wait until ninth clock pulse received

	      if ( ( !SSPSTATbits.R_W ) && ( !SSPSTATbits.BF ) )// if R/W=0 and BF=0, NOT ACK was received
	      {
	        return ( -2 );           //return NACK
	      }
		  else
		  {
			return ( 0 );				//return ACK
		  }
	}
	else if( ((SSPCON1&0x0F)==0x08) || ((SSPCON1&0x0F)==0x0B) )	//master mode only
	{
	    while( SSPSTATbits.BF );   // wait until write cycle is complete
	    IdleI2C();                 // ensure module is idle
	    if ( SSPCON2bits.ACKSTAT ) // test for ACK condition received
	    	 return ( -2 );			// return NACK
		else return ( 0 );              //return ACK
	}
          else
          {
              return (-4);
          }

  }
}

/******************************************************************************/
/* I2C_Write_At_Address
 *
 * The function writes the address then register then data.
/******************************************************************************/
char I2C_Write_At_Address( unsigned char I2Caddress, unsigned char Register, unsigned char data )
{
  IdleI2C();                      // ensure module is idle
  StartI2C();                     // initiate START condition
  while ( SSPCON2bits.SEN );      // wait until start condition is over
  if ( PIR2bits.BCLIF )           // test for bus collision
  {
    return ( -1 );                // return with Bus Collision error
  }
  else
  {
    if ( WriteI2C( I2Caddress << 1) & 0xFE )    // write 1 byte - R/W bit should be 0
    {
	  StopI2C();
      return ( -3 );              // return with write collision error
    }

    //IdleI2C();                    // ensure module is idle
    if ( !SSPCON2bits.ACKSTAT )   // test for ACK condition, if received
    {
      if ( WriteI2C( Register ) ) // write address byte to EEPROM
      {
	    StopI2C();
        return ( -3 );            // return with write collision error
      }

      //IdleI2C();                  // ensure module is idle
      if ( !SSPCON2bits.ACKSTAT ) // test for ACK condition, if received
      {
        if ( WriteI2C ( data ) )
        {
		  StopI2C();
          return( -4 );           // bus device responded possible error
        }
      }
      else
      {
	    StopI2C();
        return ( -2 );            // return with Not Ack error
      }
    }
    else
    {
	  StopI2C();
      return ( -2 );              // return with Not Ack error
    }
  }

 //IdleI2C();                      // ensure module is idle
  StopI2C();                      // send STOP condition
  while ( SSPCON2bits.PEN );      // wait until stop condition is over
  if ( PIR2bits.BCLIF )           // test for Bus collision
  {
    return ( -1 );                // return with Bus Collision error
  }
  return ( 0 );                   // return with no error
}

/******************************************************************************/
/* I2C_Read_At_Address
 *
 * The function reads data after telling the slave which register to read from.
/******************************************************************************/
int I2C_Read_At_Address( unsigned char I2Caddress, unsigned char Register )
{
  IdleI2C();                      // ensure module is idle
  StartI2C();                     // initiate START condition
  while ( SSPCON2bits.SEN );      // wait until start condition is over
  if ( PIR2bits.BCLIF )           // test for bus collision
  {
    return ( -1 );                // return with Bus Collision error
  }
  else
  {
    if ( WriteI2C( (I2Caddress << 1 ) & 0xFE ) )   // write 1 byte
    {
	  StopI2C();
      return ( -3 );              // return with write collision error
    }

    //IdleI2C();                    // ensure module is idle
    if ( !SSPCON2bits.ACKSTAT )   // test for ACK condition, if received
    {
      if ( WriteI2C( Register ) ) // WRITE word address for EEPROM
      {
	    StopI2C();
        return ( -3 );            // return with write collision error
      }

      //IdleI2C();                  // ensure module is idle
      if ( !SSPCON2bits.ACKSTAT ) // test for ACK condition, if received
      {
        RestartI2C();             // generate I2C bus restart condition
        while ( SSPCON2bits.RSEN );// wait until re-start condition is over
        if ( PIR2bits.BCLIF )     // test for bus collision
        {
          return ( -1 );          // return with Bus Collision error
        }

        if ( WriteI2C( (I2Caddress << 1) + 1 ) )// write 1 byte - R/W bit should be 1
        {
		  StopI2C();
          return ( -3 );          // return with write collision error
        }

        //IdleI2C();                // ensure module is idle
        if ( !SSPCON2bits.ACKSTAT )// test for ACK condition, if received
        {
          SSPCON2bits.RCEN = 1;       // enable master for 1 byte reception
          while ( SSPCON2bits.RCEN ); // check that receive sequence is over
          NotAckI2C();              // send ACK condition
          while ( SSPCON2bits.ACKEN ); // wait until ACK sequence is over
          StopI2C();              // send STOP condition
          while ( SSPCON2bits.PEN ); // wait until stop condition is over
          if ( PIR2bits.BCLIF )   // test for bus collision
          {
           return ( -1 );         // return with Bus Collision error
          }
        }
        else
        {
		  StopI2C();
          return ( -2 );          // return with Not Ack error
        }

      }
      else
      {
	    StopI2C();
        return ( -2 );            // return with Not Ack error
      }
    }
    else
    {
	  StopI2C();
      return ( -2 );              // return with Not Ack error
    }
  }
  return ( (unsigned int) SSPBUF );     // return with data
}

/******************************************************************************/
/* I2C_Read_Sequential
 *
 * The function reads a length of bytes and stores it at the pointer.
/******************************************************************************/
char I2C_Read_Sequential( unsigned char I2Caddress, unsigned char Register, unsigned char *rdptr, unsigned char length )
{
  IdleI2C();                      // ensure module is idle
  StartI2C();                     // initiate START condition
  while ( SSPCON2bits.SEN );      // wait until start condition is over
  if ( PIR2bits.BCLIF )           // test for bus collision
  {
    return ( -1 );                // return with Bus Collision error
  }
  else
  {
    if ( WriteI2C( (I2Caddress << 1) & 0xFE ) )    // write 1 byte
    {
	  StopI2C();
      return ( -3 );              // set error for write collision
    }

    //IdleI2C();                    // ensure module is idle
    if ( !SSPCON2bits.ACKSTAT )   // test for ACK condition, if received
    {
      if ( WriteI2C( Register ) )  // WRITE word address to EEPROM
      {
	    StopI2C();
        return ( -3 );            // return with write collision error
      }

      //IdleI2C();                  // ensure module is idle
      if ( !SSPCON2bits.ACKSTAT ) // test for ACK condition received
      {
        RestartI2C();             // generate I2C bus restart condition
        while ( SSPCON2bits.RSEN );  // wait until re-start condition is over
        if ( WriteI2C( (I2Caddress << 1) +1 ) )// WRITE 1 byte - R/W bit should be 1 for read
        {
		  StopI2C();
          return ( -3 );          // set error for write collision
        }

        //IdleI2C();                // ensure module is idle
        if ( !SSPCON2bits.ACKSTAT )// test for ACK condition received
        {
          if ( Multiple_I2C_Read( rdptr, length ) )// read in multiple bytes
          {
		    return ( -1 );        // return with Bus Collision error
          }

          NotAckI2C();            // send not ACK condition
          while ( SSPCON2bits.ACKEN );// wait until ACK sequence is over
          StopI2C();              // send STOP condition
          while ( SSPCON2bits.PEN );// wait until stop condition is over
          if ( PIR2bits.BCLIF )   // test for bus collision
          {
            return ( -1 );        // return with Bus Collision error
          }
        }
        else
        {
		  StopI2C();
          return ( -2 );          // return with Not Ack error
        }
      }
      else
      {
	    StopI2C();
        return ( -2 );            // return with Not Ack error
      }
    }
    else
    {
	  StopI2C();
      return ( -2 );              // return with Not Ack error
    }
  }
  return ( 0 );                   // return with no error
}

/******************************************************************************/
/* Multiple_I2C_Read
 *
 * The function reads a lenght of data and stores it at the pointer.
/******************************************************************************/
char Multiple_I2C_Read( unsigned char *rdptr, unsigned char length )
{
    while ( length-- )           // perform getcI2C() for 'length' number of bytes
    {
      *rdptr++ = readI2C();       // save byte received
      while ( SSPCON2bits.RCEN ); // check that receive sequence is over

      if ( PIR2bits.BCLIF )       // test for bus collision
      {
        return ( -1 );            // return with Bus Collision error
      }

	if( ((SSPCON1&0x0F)==0x08) || ((SSPCON1&0x0F)==0x0B) )	//master mode only
	{
      if ( length )               // test if 'length' bytes have been read
      {
        SSPCON2bits.ACKDT = 0;    // set acknowledge bit state for ACK
        SSPCON2bits.ACKEN = 1;    // initiate bus acknowledge sequence
        while ( SSPCON2bits.ACKEN ); // wait until ACK sequence is over
      }
	}

    }
    return ( 0 );                 // last byte received so don't send ACK
}

/******************************************************************************/
/* readI2C
 *
 * The function does a single byte i2c read.
/******************************************************************************/
unsigned char readI2C( void )
{
if( ((SSPCON1&0x0F)==0x08) || ((SSPCON1&0x0F)==0x0B) )	//master mode only
  SSPCON2bits.RCEN = 1;           // enable master for 1 byte reception
  while ( !SSPSTATbits.BF );      // wait until byte received
  return ( SSPBUF );              // return with read byte
}