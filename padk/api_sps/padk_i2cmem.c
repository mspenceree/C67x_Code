/***************************************************************************
 *
 *
 *         **   **    **  ******  ********  ********  ********  **    **    
 *        **    **   **  **   ** ********  ********  ********  **    **
 *       **     *****   **   **    **     **        **        **    **
 *      **       **    ******     **     ****      **        ********
 *     **       **    **  **     **     **        **        **    **
 *    *******  **    **   **    **     ********  ********  **    **
 *   *******  **    **    **   **     ********  ********  **    **  
 *
 *            L Y R T E C H   S I G N A L   P R O C E S S I N G              
 *
 ***************************************************************************
 *                                                                          
 *  Project     : PADK
 *  File        : PADK_I2CMEM.c
 *  Description	: Utility functions for writing/reading the I2C memory
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:18 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/
#define MEM_I2C_ADDR	0x50

/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *     	I2CMEM_Write
 *
 * PURPOSE:
 *      This function writes a vector of bytes in the I2C EEPROM.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2CMEM_Write( unsigned short ofst, unsigned char buf[], int n ); 
 *
 *		ofst  - (i)	Offset where to write to
 *		buf   - (i) Data to be written 
 *		n     - (i) Number of bytes to be written
 *
 * RETURN VALUE
 *		Return the number of bytes written
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2CMEM_Write( unsigned short offset, unsigned char buf[], int n )
{
	int i, k;		

	for ( i=0; i<n; i+= k )
	{
		k = ((n-i)>32) ? 32 : (n-i);
		if ( (offset+k) >= 8192 ) k = 8192 - offset;
		if ( I2C_WriteMem( MEM_I2C_ADDR, offset, buf, k ) < 0 )
		{
		    return i;
        }
		offset += k;
        buf += k;
        if ( offset >= 8192 )
        {
            // Wait the end of the write operation in the EEPROM
            I2C_WriteMem( MEM_I2C_ADDR, 0, buf, 0 );
            return i;
        }
	}
    // Wait the end of the write operation in the EEPROM
    I2C_WriteMem( MEM_I2C_ADDR, 0, buf, 0 );
	return n;
}


/**************************************************************************** 
 *
 * NAME  
 *     	I2CMEM_Read
 *
 * PURPOSE:
 *      This function reads a vector of bytes in the I2C EEPROM.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2CMEM_Read( unsigned short ofst, unsigned char buf[], int n ); 
 *
 *		ofst  - (i)	Offset where to read from
 *		buf   - (o) Data to be read
 *		n     - (i) Number of bytes to be read
 *
 * RETURN VALUE
 *		Return the number of bytes read
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2CMEM_Read( unsigned short offset, unsigned char buf[], int n )
{
    if ( (offset+n) > 8192 )
    {
        n = 8192 - offset;
    }
    if ( I2C_ReadMem( MEM_I2C_ADDR, offset, buf, n ) < 0 ) return -1;
    return n;
}

