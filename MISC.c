/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/11/15     1.0         Created log.
 *                          Added functions to reverse bits in char, int,
 *                            and long.
/******************************************************************************/

/******************************************************************************/
/* Contains misceleneous functions.
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

#include "MISC.h"
#include "system.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DelayUs
 * Input the number in microseconds to be delayed.
 *
 * The function waists loops for the entered bumber of cycles.
 * The actual delay can be scewed when interrupts are enabled.
/******************************************************************************/
void delayUS(long US)
{
    long i;
    unsigned char j;
    for(i=0;i<US;i+=delayConst)
    {
        for(j=0;j<SYS_FREQ_US;j++)
        {
          NOP();
        }
    }
}

/******************************************************************************/
/* cleanBuffer
 *
 * This function sets an amount of data in the array as 0.
/******************************************************************************/
void cleanBuffer(unsigned char* data, int count)
{
    unsigned char i=0;
    for(i=0; i<count;i++)
    {
        data[i]= 0;
    }
}

/******************************************************************************/
/* cleanBuffer16bit
 *
 * This function sets an amount of data in the array as 0.
/******************************************************************************/
void cleanBuffer16bit(unsigned int* data, int count)
{
    unsigned char i=0;
    for(i=0; i<count;i++)
    {
        data[i]= 0;
    }
}

/******************************************************************************/
/* BufferCopy
 *
 * This function copies the 'from' array to the 'to' array.
/******************************************************************************/
void BufferCopy(unsigned char* from,unsigned char* to, unsigned char count, unsigned char shift)
{
    unsigned char i=0;
    cleanBuffer(to,count);
    for(i = shift; i>0;i--)
    {
        *to = ' ';
        to++;
    }
    while(*from != 0 && count >0)
    {
    *to = *from;
    from++;
    to++;
    count--;
    }
}

/******************************************************************************/
/* StringMatch
 *
 * This function returns TRUE if the array 'This' matches the array 'That'.
/******************************************************************************/
unsigned char StringMatch(unsigned char* This,const unsigned char* That)
{
    while(*This != 0)
    {
       if(*This != *That || *That == 0)
       {
           return FALSE;
       }
       This++;
       That++;
    }
    if(*That == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/* StringContains
 *
 * This function returns TRUE if the array 'That' is contained in the array
 *   'This'.
/******************************************************************************/
unsigned char StringContains(unsigned char* This, const unsigned char* That)
{
    while(*This != 0)
    {
       while(*This == *That)
       {
           That++;
           This++;
           if(*That == 0)
           {
               return TRUE;
           }
           if(*This == 0)
           {
               return FALSE;
           }
       }
       This++;
    }
    return FALSE;
}

/******************************************************************************/
/* StartsWith
 *
 * This function returns TRUE if the array 'That' is contained in the array
 *   'This'.
/******************************************************************************/
unsigned char StartsWith(unsigned char* This,const unsigned char* That)
{
    while(*This != 0)
    {
        if(*This != *That)
        {
            if(*That != 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        This++;
        That++;
    }
    if(*This == 0 && *That == 0)
    {
        // the arrays are equal
        return TRUE;
    }
    else
    {
        // array 'That' is bigger than array 'This'
        return FALSE;
    }
}

/******************************************************************************/
/* GetEnteredNumber
 *
 * This function returns the number that is contained in the string.
/******************************************************************************/
long GetEnteredNumber(unsigned char* This)
{
    unsigned char i =0;
    long temp =0;
    unsigned char negative =0;
    while(*This != '=' && *This != 0)
    {
        This++;
        //go all the way to the equal
    }
    if(*This == 0)
    {
        return NOEQUAL; // there is no equal
    }
    This++;
    if(*This == ' ')
    {
      This++;
      //move past the space
    }
    else if (*This == '=')
    {
         return DOUBLEEQUAL; //double equal
    }
    else if(*This == '-')
    {
      negative = 1;
      This++;
      //move past the space
    }
    else if(*This == 0)
    {
        return NOVALUE; // there is no value after the equal
    }
    while(*This >=48 && *This <= 57)
    {
        temp = temp * 10;
        temp += *This - 48;
        i++;
        if(i>32)
        {
            return TOOBIG;//number too big
        }
        This++;
    }
    if(negative)
    {
        temp = (temp * -1);
    }
    return temp;
}

/******************************************************************************/
/* RemoveSpaces
 *
 * This function removes any spaces in the string.
/******************************************************************************/
void RemoveSpaces(unsigned char* This)
{
    //must match size of *This which is 100
    unsigned char i =0;
    unsigned char* address = This;
    unsigned char That[100];
    cleanBuffer(That,100);
    while(*This != 0)
    {
        if(*This != ' ')
        {
            That[i] = *This;
            i++;
        }
        This++;
    }
    BufferCopy(That,address, 100, 0);
}

/******************************************************************************/
/* lowercase
 *
 * This function turns all characters to in the string to lowercase.
/******************************************************************************/
void lowercase(unsigned char* Input)
{
    unsigned char i =0;
    while((Input[i]!= 0) && (i < 100))
    {
        if((Input[i] >= 65) && (Input[i] <= 90 ))
        {
            Input[i] += 32;
        }
        i++;
    }
}

/******************************************************************************/
/* StringAddEqual
 *
 * This function puts an equal sign between letters and a number value.
/******************************************************************************/
unsigned char StringAddEqual(unsigned char* Input)
{
    unsigned char i =0;
    unsigned char j =0;
    unsigned char firstnumber =200;
    unsigned char temp[100];
    cleanBuffer(temp,100);
    while(Input[i] != 0)
    {
            if((Input[i] >= 48) && (Input[i] <= 57) || (Input[i] == '-'))
            {
                firstnumber=i;
                break;
            }
            i++;
    }
    if(firstnumber ==200)
    {
        return FALSE;
    }
    for(j=0;j<firstnumber;j++)
    {
           temp[j] = Input[j];
    }
    temp[firstnumber] = '=';
    for(j = (firstnumber);j<99;j++)
    {
        temp[j+1] = Input[j];
        if(Input[j] ==0)
        {
            break;
        }
    }
    BufferCopy(temp,Input,100, 0);
    return TRUE;
}

/******************************************************************************/
/* CheckSum_byte
 *
 * The function reads the value of 'This' and retunrn the checksum. The Odd_Even
 * parameter determines if it returns an even polarity bit or an odd polarity
 * bit.
/******************************************************************************/
unsigned char CheckSum_byte(unsigned int This, unsigned char Odd_Even)
{
    char i = 0;
    unsigned char Parity = 0;
    unsigned int ThisTemp = This;

    for (i =0; i<8; i++)
    {
        if((ThisTemp & 0x01) == 1)
        {
            Parity++;
        }
        ThisTemp >>= 1;
    }
    if(Odd_Even == Odd)
    {
        if(Parity % 2 == 0) //even
        {
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        if(Parity % 2 == 1) //odd
        {
            return TRUE;
        }
        return FALSE;
    }
}

/******************************************************************************/
/* ISNUMBER
 *
 * The function returns TRUE if the ascii value is associated with a number.
/******************************************************************************/
unsigned char ISNUMBER(unsigned char ascii)
{
    if(ascii >= '0' && ascii <='9')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/* IsPrintableASCII
 *
 * The function returns TRUE the input character is a printable ascii value.
/******************************************************************************/
unsigned char IsPrintableASCII(unsigned char data)
{
    if(data >= 0x20 && data < 0x7F)
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************************************/
/* Reverse_Byte
 *
 * The function reads the value of 'This' and returns the reverse of the data.
/******************************************************************************/
unsigned char Reverse_Byte(unsigned char This)
{
    unsigned char temp=0;

    temp += (This & 0x01) << 7;
    temp += (This & 0x02) << 5;
    temp += (This & 0x04) << 3;
    temp += (This & 0x08) << 1;
    temp += (This & 0x10) >> 1;
    temp += (This & 0x20) >> 3;
    temp += (This & 0x40) >> 5;
    temp += (This & 0x80) >> 7;

    return temp;
}

/******************************************************************************/
/* Reverse_2Byte
 *
 * The function reads the value of 'This' and returns the reverse of the data.
/******************************************************************************/
unsigned int Reverse_2Byte(unsigned int This)
{
    unsigned int temp=0;

    temp += (This & 0x0001) << 15;
    temp += (This & 0x0002) << 13;
    temp += (This & 0x0004) << 11;
    temp += (This & 0x0008) << 9;
    temp += (This & 0x0010) << 7;
    temp += (This & 0x0020) << 5;
    temp += (This & 0x0040) << 3;
    temp += (This & 0x0080) << 1;
    temp += (This & 0x0100) >> 1;
    temp += (This & 0x0200) >> 3;
    temp += (This & 0x0400) >> 5;
    temp += (This & 0x0800) >> 7;
    temp += (This & 0x1000) >> 9;
    temp += (This & 0x2000) >> 11;
    temp += (This & 0x4000) >> 13;
    temp += (This & 0x8000) >> 15;

    return temp;
}

/******************************************************************************/
/* Reverse_4Byte
 *
 * The function reads the value of 'This' and returns the reverse of the data.
/******************************************************************************/
unsigned long Reverse_4Byte(unsigned long This)
{
    unsigned long temp=0;

    temp += (This & 0x00000001) << 31;
    temp += (This & 0x00000002) << 29;
    temp += (This & 0x00000004) << 27;
    temp += (This & 0x00000008) << 25;
    temp += (This & 0x00000010) << 23;
    temp += (This & 0x00000020) << 21;
    temp += (This & 0x00000040) << 19;
    temp += (This & 0x00000080) << 17;
    temp += (This & 0x00000100) >> 15;
    temp += (This & 0x00000200) >> 13;
    temp += (This & 0x00000400) >> 11;
    temp += (This & 0x00000800) >> 9;
    temp += (This & 0x00001000) >> 7;
    temp += (This & 0x00002000) >> 5;
    temp += (This & 0x00004000) >> 3;
    temp += (This & 0x00008000) >> 1;
    temp += (This & 0x00010000) << 1;
    temp += (This & 0x00020000) << 3;
    temp += (This & 0x00040000) << 5;
    temp += (This & 0x00080000) << 7;
    temp += (This & 0x00100000) << 9;
    temp += (This & 0x00200000) << 11;
    temp += (This & 0x00400000) << 13;
    temp += (This & 0x00800000) << 15;
    temp += (This & 0x01000000) >> 17;
    temp += (This & 0x02000000) >> 19;
    temp += (This & 0x04000000) >> 21;
    temp += (This & 0x08000000) >> 23;
    temp += (This & 0x10000000) >> 25;
    temp += (This & 0x20000000) >> 27;
    temp += (This & 0x40000000) >> 29;
    temp += (This & 0x80000000) >> 31;

    return temp;
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/