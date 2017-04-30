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
 *  File        : PADK_FLASH.h
 *  Description	: CFI Flash Memory driver.
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:17 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/
#ifndef __PADK_FLASH_H__
#define __PADK_FLASH_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/
#define	FLASH_ERROR_NONE				( 0)
#define	FLASH_ERROR_NODEVICE			(-1)
#define	FLASH_ERROR_ERASE_TO			(-2)
#define	FLASH_ERROR_WRITE_TO			(-3)
#define	FLASH_ERROR_NOMEMORY			(-4)
#define	FLASH_ERROR_DEV_UNSUPPORTED		(-5)
#define	FLASH_ERROR_DEV_LOCKED			(-6)
#define	FLASH_ERROR_OVERFLOW			(-7)

/****************************************************************************
 *                                  MACROS                                  *
 ****************************************************************************/
#define FLASH_WRITE_WORD(a,v)    ((FPGA_REG(5)=(((unsigned)(a))>>12)),FPGA_FLASH[(a)&0x0FFF]=(v))
#define FLASH_READ_WORD(a)       ((FPGA_REG(5)=(((unsigned)(a))>>12)),FPGA_FLASH[(a)&0x0FFF])



/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      FLASH_Write
 *
 * PURPOSE:
 *      This function write any number of bytes of data in the FLASH memory
 * 		at any offset. Only the data being written is mofified, the remainder
 * 		of the data in the FLASH memory is preserved.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int FLASH_Write( unsigned offset, unsigned char *data, unsigned length );
 *
 *      offset - (i) offset in the FLASH memory where to write the data
 *      data   - (i) pointer to the data to be written
 *      length - (i) length in bytes of the data buffer.
 *
 * REFERENCE
 *
 * ASSUMPTION
 * 		This function allocates (malloc) a buffer in the HEAP memory that is 
 * 		twice the size of the maximum sector size of the FLASH memory.
 * 		(128 kB for the Am29LV160D)
 *
 ****************************************************************************/	
far int FLASH_Write( unsigned offset, unsigned char *data, unsigned length );

/**************************************************************************** 
 *
 * NAME  
 *      FLASH_Read
 *
 * PURPOSE:
 *      This function read any number of bytes of data in the FLASH memory
 * 		at any offset. 
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int FLASH_Read( unsigned offset, unsigned char *data, unsigned length );
 *
 *      offset - (i) offset in the FLASH memory where to read the data
 *      data   - (o) buffer pointer
 *      length - (i) length in bytes to read.
 *
 * REFERENCE
 *
 * ASSUMPTION
 *
 ****************************************************************************/	
far int FLASH_Read( unsigned offset, unsigned char *data, unsigned length );


/**************************************************************************** 
 *
 * NAME  
 *      FLASH_ReadByte
 *
 * PURPOSE:
 *      This function read a single byte of data in the FLASH memory.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		unsigned char FLASH_ReadByte( unsigned offset );
 *
 *      offset - (i) byte offset in the FLASH memory where to read
 *
 * REFERENCE
 *
 * ASSUMPTION
 *
 ****************************************************************************/	
static inline unsigned char FLASH_ReadByte( unsigned offset )
{
     // Set Page pointer
    FPGA_REG(5) = offset >> 12;

    // Read the data
    return ((unsigned char *)FPGA_FLASH)[offset];
}


/**************************************************************************** 
 *
 * NAME  
 *      FLASH_ReadWord
 *
 * PURPOSE:
 *      This function read a single word of data in the FLASH memory.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		unsigned short FLASH_ReadWord( unsigned offset );
 *
 *      offset - (i) word offset in the FLASH memory where to read
 *
 * REFERENCE
 *
 * ASSUMPTION
 *
 ****************************************************************************/	
static inline unsigned short FLASH_ReadWord( unsigned offset )
{
     // Set Page pointer
    FPGA_REG(5) = offset >> 12;

    // Read the data
    return FPGA_FLASH[offset];
}


/**************************************************************************** 
 *
 * NAME  
 *      FLASH_ReadDWord
 *
 * PURPOSE:
 *      This function read a single double word of data in the FLASH memory.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		unsigned FLASH_ReadDWord( unsigned offset );
 *
 *      offset - (i) double word offset in the FLASH memory where to read
 *
 * REFERENCE
 *
 * ASSUMPTION
 *
 ****************************************************************************/	
static inline unsigned FLASH_ReadDWord( unsigned offset )
{
     // Set Page pointer
    FPGA_REG(5) = offset >> 12;

    // Read the data
    return ((unsigned *)FPGA_FLASH)[offset];
}

#endif	// __PADK_FLASH_H__

