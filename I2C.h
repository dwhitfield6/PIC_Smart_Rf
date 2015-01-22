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

#define I2Cbaud	5

/* SSPCON1 REGISTER */
#define   SSPENB    			0b00100000  	/* Enable serial port and configures SCK, SDO, SDI*/
#define   SLAVE_7   			0b00000110     	/* I2C Slave mode, 7-bit address*/
#define   SLAVE_10  			0b00000111    	/* I2C Slave mode, 10-bit address*/
#define   MASTER    			0b00001000     	/* I2C Master mode */
#define   MASTER_FIRMW			0b00001011		//I2C Firmware Controlled Master mode (slave Idle)
#define   SLAVE_7_STSP_INT 		0b00001110		//I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
#define   SLAVE_10_STSP_INT             0b00001111		//I2C Slave mode, 10-bit address with Start and Stop bit interrupts enabled

/* SSPSTAT REGISTER */
#define   SLEW_OFF  			0b10000000  	/* Slew rate disabled for 100kHz mode */
#define   SLEW_ON   			0b00000000  	/* Slew rate enabled for 400kHz mode  */

#define StartI2C()  SSPCON2bits.SEN=1;while(SSPCON2bits.SEN)

void Init_I2C_Master(void);
int I2C_Read_At_Address( unsigned char I2Caddress, unsigned char Register );
char I2C_Write_At_Address( unsigned char I2Caddress, unsigned char Register, unsigned char data );
char I2C_Read_Sequential( unsigned char I2Caddress, unsigned char Register, unsigned char *rdptr, unsigned char length );
void ResetI2C( void );
char Multiple_I2C_Read( unsigned char *rdptr, unsigned char length );
unsigned char readI2C( void );
